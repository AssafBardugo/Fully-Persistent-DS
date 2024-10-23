#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP

#include "utils.hpp"
#include "excep.hpp"

namespace pds{

    using version_t = std::size_t;

    const version_t MASTER_VERSION = 0;

    template <class OBJ>
    struct fat_node_ptr;

    template <class OBJ>
    class fat_node{
        const OBJ obj;
    public:
        pds::fat_node_ptr<OBJ> left;
        pds::fat_node_ptr<OBJ> right;

        fat_node(const OBJ& obj, pds::version_t version);
        fat_node(OBJ&& obj, pds::version_t version);

        const OBJ& get_obj() const;
    };

    template <class OBJ>
    struct fat_node_ptr{
        std::unordered_map<pds::version_t, std::shared_ptr<pds::fat_node<OBJ>>> table;
        std::vector<pds::version_t> nodes_versions;

        fat_node_ptr(pds::version_t first_version);
        pds::version_t map(pds::version_t version);
    };
};


////////////////////////////////////
/// fat_node
////////////////////////////////////

template <class OBJ>
pds::fat_node<OBJ>::fat_node(const OBJ& obj, pds::version_t version)

    : obj(obj), left(version), right(version) {
}

template <class OBJ>
pds::fat_node<OBJ>::fat_node(OBJ&& obj, pds::version_t version)

    : obj(std::move(obj)), left(version), right(version) {
}

template <class OBJ>
const OBJ& pds::fat_node<OBJ>::get_obj() const {

    return obj;
}


////////////////////////////////////
/// fat_node_ptr
////////////////////////////////////

template <class OBJ>
pds::fat_node_ptr<OBJ>::fat_node_ptr(pds::version_t first_version) 
            
    : table{{MASTER_VERSION, nullptr}, {first_version, nullptr}}, 
        nodes_versions{MASTER_VERSION, first_version} {
}

template <class OBJ>
pds::version_t pds::fat_node_ptr<OBJ>::map(pds::version_t version){

    if(version == MASTER_VERSION)
        return version;

    // Find the latest version that is less than or equal to the given version.
    // Since 'nodes_versions' is sorted, we can use binary search!

    /***    Binary Search    ***/

    int left = 0;
    int right = nodes_versions.size() - 1;
    int mapped_index = right;

    while(left <= right){

        int mid = (left + right) / 2;

        if(nodes_versions[mid] <= version){

            mapped_index = mid;
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }

    return nodes_versions[mapped_index];
}


#endif /* PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP */