#include "fpset.hpp"
#include "pds_excep.hpp"

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
fpset<OBJ>::fpset() : v_master{pds::FatNodeAndObjComp<OBJ>()}, last_version(1) {
}


template <class OBJ>
version_t fpset<OBJ>::insert(const OBJ& obj, version_t version){

    if(version == default_version)
        version = last_version;
    
    if(version == 0 || version > last_version)
        throw VersionOutOfRange(
            "fpset::insert: Version " + std::to_string(version) + " is out of range"
        );

    if(this->contains(obj, version) != 0)
        throw ObjectAlreadyExist(
            "fpset::insert: Attempting to insert an object that already exists for Version " 
            + std::to_string(version)
        );

    version_t new_version = last_version + 1;
    std::shared_ptr<fat_node<OBJ>> new_node_ptr = nullptr;

    if(auto search = v_master.find(obj); search != v_master.end()){

        new_node_ptr = *search;
    }
    else{
        auto result = v_master.insert(std::make_shared<fat_node<OBJ>>(obj, new_version));
        new_node_ptr = *result.first;
    }

    std::shared_ptr<fat_node<OBJ>> tracker = root[version];

    if(tracker == nullptr){

        // TODO: insert new_node_ptr to root[new_version]

        return (last_version = new_version);
    }

    while(true){

        tracker->map_new_version(new_version);

        if(obj < tracker->obj){

            if(tracker->left_ptr(version) == nullptr){

                tracker->insert_left(new_node_ptr, new_version);
                break;
            }

            tracker = tracker->left_ptr(version);
        }
        else{

            if(tracker->right_ptr(version) == nullptr){

                tracker->insert_right(new_node_ptr, new_version);
                break;
            }

            tracker = tracker->right_ptr(version);
        }
    }

    return (last_version = new_version);
}


template <class OBJ>
version_t fpset<OBJ>::remove(const OBJ& obj, version_t version){

    if(version == default_version)
        version = last_version;

    
}


template <class OBJ>
bool fpset<OBJ>::contains(const OBJ& obj, version_t version) const {

    
}


template <class OBJ>
pds::version_t fpset<OBJ>::size(pds::version_t version = master_version) const {

    if(version == master_version)
        return v_master.size();

    
}


template <class OBJ>
pds::version_t fpset<OBJ>::curr_version() const {
    return last_version;
}


template <class OBJ>
std::set<OBJ> fpset<OBJ>::to_set(pds::version_t version) const {


}
