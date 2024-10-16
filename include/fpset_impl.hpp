#ifndef FULLY_PERSISTENT_SET_IMPLEMENTATION_HPP
#define FULLY_PERSISTENT_SET_IMPLEMENTATION_HPP

#include "fpset.hpp"
#include "internal/fat_node_tracker.hpp"

using namespace pds;


template <class OBJ>
fpset<OBJ>::fpset() : root(1), last_version(1), sizes{0, 0} {
}


template <class OBJ>
version_t fpset<OBJ>::insert(const OBJ& obj, version_t version){

    return insert_impl(obj, version);
}


template <class OBJ>
version_t fpset<OBJ>::insert(OBJ&& obj, version_t version){

    return insert_impl(std::move(obj), version);
}


template <class OBJ>
template <typename T>
version_t fpset<OBJ>::insert_impl(T&& obj, version_t version){

    if(version == default_version)
        version = last_version;

    if(version == 0 || version > last_version)
        throw VersionOutOfRange(
            "fpset::insert: Version " + std::to_string(version) + " is out of range"
        );

    fat_node_tracker<OBJ> tracker(root);

    while(tracker.not_null_at(version)){

        if(obj < tracker.obj_at(version)){

            tracker = tracker.left_at(version);
        }
        else if(tracker.obj_at(version) < obj){

            tracker = tracker.right_at(version);
        }
        else{
            throw ObjectAlreadyExist(
                "fpset::insert: Attempting to insert an object that already exists for Version " 
                + std::to_string(version)
            );
        }
        tracker.copy_map(version);
    }

    version_t new_version = last_version + 1;

    if(auto search = v_master.find(dummy_sp<OBJ>(obj)); search != v_master.end()){

        tracker[new_version] = *search;
        tracker.ref_left_at(new_version) = nullptr;
        tracker.ref_right_at(new_version) = nullptr;
    }
    else{
        auto result = v_master.insert(std::make_shared<fat_node<OBJ>>(std::forward<T>(obj), new_version));
        tracker[new_version] = *result.first;
    }

    add_new_version(tracker.obj_at(new_version), version);

    // push the size of the new version
    sizes.push_back(sizes[version] + 1);

    return (last_version = new_version);
}


template <class OBJ>
version_t fpset<OBJ>::remove(const OBJ& obj, version_t version){

    if(version == default_version)
        version = last_version;

    if(version == 0 || version > last_version)
        throw VersionOutOfRange(
            "fpset::remove: Version " + std::to_string(version) + " is out of range"
        );

    fat_node_tracker<OBJ> tracker(root);

    while(tracker.not_null_at(version)){

        if(obj < tracker.obj_at(version)){

            tracker = tracker.left_at(version);
        }
        else if(tracker.obj_at(version) < obj){

            tracker = tracker.right_at(version);
        }
        else break;

        tracker.copy_map(version);
    }

    if(tracker.null_at(version))
        throw ObjectNotExist(
            "fpset::remove: Attempting to remove an object from Version "
            + std::to_string(version) + ". But the object is not exists for this Version" 
        );

    
    fat_node_tracker<OBJ> to_remove = tracker;
    version_t new_version = last_version + 1;

    if(to_remove.left_at(version).null_at(version)){

        to_remove[new_version] = to_remove.ref_right_at(version);
    }
    else if(to_remove.right_at(version).null_at(version)){

        to_remove[new_version] = to_remove.ref_left_at(version);
    }
    else{

/***************  ILLUSTRATION  **************************

          +++ version +++    |   +++ new_version +++
                             |
      to_remove:[6]          |           [9]
               /   \        ==>>        /   \
            [4]     [16]     |       [4]     [16]
                   /         |              /
               [11]          |          [11]
              /              |
           [9]               |
  
**********************************************************/

        tracker = to_remove.right_at(version);

        while(tracker.left_at(version).not_null_at(version)){

            tracker[new_version] = tracker.at(version);
            tracker = tracker.left_at(version);
        }
        tracker[new_version] = nullptr;

        to_remove[new_version] = tracker.at(version);
    }

    add_new_version(tracker.obj_at(new_version), version);

    // push the size of the new version
    sizes.push_back(sizes[version] - 1);

    return (last_version = new_version);
}


template <class OBJ>
bool fpset<OBJ>::contains(const OBJ& obj, version_t version){

    if(version == master_version)
        return v_master.contains(dummy_sp<OBJ>(obj));

    if(version > last_version)
        throw VersionOutOfRange(
            "fpset::contains: Version " + std::to_string(version) + " is out of range"
        );

    fat_node_tracker<OBJ> tracker(root);

    while(tracker.not_null_at(version)){

        tracker.copy_map(version);

        if(obj < tracker.obj_at(version)){

            tracker = tracker.left_at(version);
        }
        else if(tracker.obj_at(version) < obj){

            tracker = tracker.right_at(version);
        }
        else return true;
    }

    return false;
}


template <class OBJ>
version_t fpset<OBJ>::size(version_t version) const {

    if(version == master_version)
        return v_master.size();

    return sizes[version];
}


template <class OBJ>
version_t fpset<OBJ>::curr_version() const {
    return last_version;
}


template <class OBJ>
static void to_set_aux(const std::shared_ptr<fat_node<OBJ>>& root, 
    const version_t v, std::set<OBJ>& obj_set){

    if(root == nullptr)
        return;

    to_set_aux(root->left.table.at(v), v, obj_set);

    obj_set.insert(root->get_obj());

    to_set_aux(root->right.table.at(v), v, obj_set);
}


template <class OBJ>
std::set<OBJ> fpset<OBJ>::to_set(const version_t version) const {

    std::set<OBJ> obj_set;

    if(version == master_version){

        for(auto& node_ptr : v_master){

            obj_set.insert(node_ptr->get_obj());
        }
    }
    else{
        to_set_aux(root.table.at(version), version, obj_set);
    }

    return obj_set;
}


template <class OBJ>
bool fpset<OBJ>::equal(const version_t version, std::set<OBJ>&& obj_set) const {

    return to_set(version) == obj_set;
}


#endif /* FULLY_PERSISTENT_SET_IMPLEMENTATION_HPP */