#include "fpset.hpp"
#include "pds_excep.hpp"

using namespace pds;


template<class OBJ>
static bool comp_obj_with_node(const OBJ& obj, const fat_node<OBJ>& node){
    return obj < node.obj;
}


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
    
    // here: We will not be able to create a new node and not insert it 
    // since either insertion will end successfully or obj already exists. Thus, it exists in v_master.

    version_t new_version = last_version + 1;
    std::shared_ptr<fat_node<OBJ>> node_ptr = nullptr;

    if(auto search = v_master.find(obj); search != v_master.end()){

        node_ptr = *search;
    }
    else{
        auto result = v_master.insert(std::make_shared<fat_node<OBJ>>(obj, new_version));
        node_ptr = *result.first;
    }

    std::shared_ptr<fat_node<OBJ>> tracker = root[version];

    if(tracker == nullptr){

        // TODO: insert node_ptr to root[new_version]

        return (last_version = new_version);
    }

    while(true){

        tracker->map_new_version(new_version);

        if(obj < tracker->obj){

            if(tracker->left_ptr(version) == nullptr){

                tracker->insert_left(node_ptr, new_version);
                break;
            }

            tracker = tracker->left_ptr(version);
        }
        else{

            if(tracker->right_ptr(version) == nullptr){

                tracker->insert_right(node_ptr, new_version);
                break;
            }

            tracker = tracker->right_ptr(version);
        }
    }

    return (last_version = new_version);
}


template <class OBJ>
version_t fpset<OBJ>::remove(OBJ obj, version_t version){

    
}


template <class OBJ>
bool fpset<OBJ>::contains(OBJ obj, version_t version){

    
}


template <class OBJ>
std::set<OBJ> fpset<OBJ>::to_set(pds::version_t version){


}
