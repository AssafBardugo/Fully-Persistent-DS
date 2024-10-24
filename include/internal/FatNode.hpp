#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP

#include "utils.hpp"
#include "excep.hpp"

namespace pds{

    using version_t = std::size_t;

    /**
     * @anchor MasterVersion
     * @brief Defines an exceptional version that is intended for internal use.
     * 
     * @details 
     * What is it: Version 0 (to be called 'MasterVersion') is reserve for a unique mission, to save 
     *  the all T-nodes together. A new T-node will insert first to MasterVersion and then its 'std::shared_ptr' 
     *  will copied to store it also for its real version.
     * 
     * @remarks
     * Why its good: The uses of groups all T-nodes (@ref T-node VS fat-node) under one version are:
     * 
     *      A. It helps to insert or remove in BST time-search by determine if a new T-node 
     *          is already exist in other version and we can avoid its unnecessary duplicate.
     * 
     *      B. Avoiding self loops between versions. 
     *          For example: {insert(obj=7, v=1), insert(obj=9, v=2), remove(obj=7, v=3), insert(obj=7, v=4)}
     *          will create the loop [7]<--->[9] between versions 2 and 4.
     * 
     *      C. A simple memory managment. Thanks to the fact that we avoiding self loops, we can use 'std::shared_ptr'
     *          and T-node will free itself when the all fat-nodes who points on it will removes its copy.
     * 
     *      D. Finally, for easy insert & remove algorithms.
     */
    const version_t MasterVersion = 0;

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
        pds::version_t map(const pds::version_t version) const;
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
            
    : table{{MasterVersion, nullptr}, {first_version, nullptr}}, 
        nodes_versions{MasterVersion, first_version} {
}

template <class OBJ>
pds::version_t pds::fat_node_ptr<OBJ>::map(const pds::version_t version) const {

    if(version == MasterVersion)
        return version;

    if(version >= nodes_versions.back())
        return nodes_versions.back();

    // Find the latest version that is less than or equal to the given version.
    // Since 'nodes_versions' is sorted, we can use binary search!

    /***    Binary Search    ***/

    int left = 0;
    int right = nodes_versions.size() - 1;
    int mapped_version = nodes_versions.back();

    while(left <= right){

        int mid = (left + right) / 2;

        if(nodes_versions[mid] <= version){

            mapped_version = nodes_versions[mid];
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }

    return mapped_version;
}


#endif /* PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP */