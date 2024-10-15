#ifndef FULLY_PERSISTENT_SET_IMPLEMENTATION_HPP
#define FULLY_PERSISTENT_SET_IMPLEMENTATION_HPP

#include "fpset.hpp"

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

    if(this->contains(obj, version))
        throw ObjectAlreadyExist(
            "fpset::insert: Attempting to insert an object that already exists for Version " 
            + std::to_string(version)
        );


    version_t new_version = last_version + 1;

    // push the size of the new version
    sizes.push_back(sizes[version] + 1);

    std::shared_ptr<fat_node<OBJ>> new_node_ptr = nullptr;

    if(auto search = v_master.find(dummy_sp<OBJ>(obj)); search != v_master.end()){

        new_node_ptr = *search;
        new_node_ptr->left[new_version] = nullptr;
        new_node_ptr->right[new_version] = nullptr;
    }
    else{
        auto result = v_master.insert(std::make_shared<fat_node<OBJ>>(std::forward<T>(obj), new_version));
        new_node_ptr = *result.first;
    }

    // extend_stack get the type: vector<pair<version_t, version_t>>
    root.extend_stack({{new_version, version}});

    pds::nodes_table<pds::fat_node<OBJ>> tracker = root;

    while(tracker.at(version)){

        tracker.copy_map(new_version);

        if(obj < tracker.at(new_version)->obj){

            tracker = tracker.at(new_version)->left;
        }
        else{
            tracker = tracker.at(new_version)->right;
        }
    }

    tracker[new_version] = new_node_ptr;

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

    if(this->contains(obj, version) == false)
        throw ObjectNotExist(
            "fpset::remove: Attempting to remove an object from Version "
            + std::to_string(version) + ". But the object is not exists for this Version" 
        );
    
    version_t new_version = last_version + 1;

    // push the size of the new version
    sizes.push_back(sizes[version] - 1);

    nodes_table<fat_node<OBJ>> to_remove = root;

    while(true){

        if(obj < to_remove.at(version)->obj)

            to_remove = to_remove.at(new_version)->left;

        else if(to_remove.at(version)->obj < obj)

            to_remove = to_remove.at(new_version)->right;

        else break;
    }

    if(to_remove.at(version)->left == nullptr){

        to_remove[new_version] = to_remove.at(version)->right;
    }
    else if(to_remove.at(version)->right == nullptr){

        to_remove[new_version] = to_remove.at(version)->left;
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

        nodes_table<fat_node<OBJ>> tracker = to_remove.at(version)->right;

        while(tracker.at(version)->left){

            tracker[new_version] = tracker.at(version);
            tracker = tracker.at(version)->left;
        }
        tracker[new_version] = nullptr;

        to_remove[new_version] = tracker.at(version);
    }

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

    std::shared_ptr<fat_node<OBJ>> tracker = root.at(version);

    while(tracker){

        if(obj < tracker->obj)

            tracker = tracker->left.at(version);

        else if(tracker->obj < obj)

            tracker = tracker->right.at(version);

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
static void to_set_aux(std::shared_ptr<fat_node<OBJ>>& root, 
    const version_t v, std::set<OBJ>& obj_set){

    if(root == nullptr)
        return;

    to_set_aux(root->left.at(v), v, obj_set);

    obj_set.insert(root->obj);

    to_set_aux(root->right.at(v), v, obj_set);
}


template <class OBJ>
std::set<OBJ> fpset<OBJ>::to_set(const version_t version) const {

    std::set<OBJ> obj_set;

    if(version == master_version){

        for(std::shared_ptr<fat_node<OBJ>>& node_ptr : v_master){

            obj_set.insert(node_ptr->obj);
        }
    }
    else{
        to_set_aux(root.at(version), version, obj_set);
    }

    return obj_set;
}


template <class OBJ>
bool fpset<OBJ>::equal(const version_t version, std::set<OBJ>&& obj_set) const {

    return to_set(version) == obj_set;
}


#endif /* FULLY_PERSISTENT_SET_IMPLEMENTATION_HPP */