#ifndef FULLY_PERSISTENT_SET_IMPLEMENTATION_HPP
#define FULLY_PERSISTENT_SET_IMPLEMENTATION_HPP

#include "fpset.hpp"
#include "excep.hpp"

using namespace pds;

// Custom comparator that works for both shared_ptr<fat_node<OBJ>> and OBJ
template <class OBJ>
struct FatNodeAndObjComp{
    bool operator()(const std::shared_ptr<fat_node<OBJ>>& node_a, 
                    const std::shared_ptr<fat_node<OBJ>>& node_b) const {
        return *node_a < *node_b;
    }

    bool operator()(const std::shared_ptr<fat_node<OBJ>>& node, const OBJ& obj) const {
        return *node < obj;
    }

    bool operator()(const OBJ& obj, const std::shared_ptr<fat_node<OBJ>>& node) const {
        return *node > obj;
    }
};


template <class OBJ>
fpset<OBJ>::fpset() : v_master{FatNodeAndObjComp<OBJ>()}, 
    sizes{-1, 0}, last_version(1) {
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

    if(auto search = v_master.find(obj); search != v_master.end()){

        new_node_ptr = *search;
        new_node_ptr->left[new_version] = nullptr;
        new_node_ptr->right[new_version] = nullptr;
    }
    else{
        auto result = v_master.insert(
            std::make_shared<fat_node<OBJ>>(std::forward<T>(obj), new_version));
        new_node_ptr = *result.first;
    }

    if(root.at(version) == nullptr){

        root[new_version] = new_node_ptr;
        return (last_version = new_version);
    }

    pds::map_cow new_map = {new_version, version};
    root.map({new_map});

    std::shared_ptr<fat_node<OBJ>>& tracker = root.at(version);

    while(true){

        if(obj < tracker->obj){

            if(tracker->left.at(version) == nullptr){

                tracker->left[new_version] = new_node_ptr;
                break;
            }
            tracker = tracker->left.at(new_version);
        }
        else{
            if(tracker->right.at(version) == nullptr){

                tracker->right[new_version] = new_node_ptr;
                break;
            }
            tracker = tracker->right.at(new_version);
        }
    }

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

    std::shared_ptr<fat_node<OBJ>> tracker = root.at(version);

    if(tracker->obj == obj){
        // TODO: new_version has to keep version childrens
        root[new_version] = nullptr;
        return (last_version = new_version);
    }

    while(true){

        if(obj < tracker->obj){

            if(tracker->left[version]->obj == obj){
                
                // TODO: new_version has to keep version childrens
                tracker->left[new_version] = nullptr;
                break;
            }
            tracker = tracker->left.at(new_version);
        }
        else{
            if(tracker->right[version]->obj == obj){

                tracker->right[new_version] = nullptr;
                break;
            }
            tracker = tracker->right.at(new_version);
        }
    }

    return (last_version = new_version);
}


template <class OBJ>
bool fpset<OBJ>::contains(const OBJ& obj, version_t version) const {

    if(version == master_version)
        return v_master.contains(obj);

    if(version > last_version)
        throw VersionOutOfRange(
            "fpset::contains: Version " + std::to_string(version) + " is out of range"
        );

    std::shared_ptr<fat_node<OBJ>>& tracker = root.at(version);

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
version_t fpset<OBJ>::size(version_t version = master_version) const {

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