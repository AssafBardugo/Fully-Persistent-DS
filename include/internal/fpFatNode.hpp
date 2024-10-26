#ifndef FULLY_PERSISTENT_FAT_NODE_HPP
#define FULLY_PERSISTENT_FAT_NODE_HPP

#include "Utils.hpp"
#include "Excep.hpp"
#include "UnionFind.hpp"

namespace pds{

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
    struct fpFatNodePtr;

    template <class OBJ>
    class fpFatNode{
        const OBJ obj;
    public:
        pds::fpFatNodePtr<OBJ> left;
        pds::fpFatNodePtr<OBJ> right;

        fpFatNode(const OBJ& obj, pds::version_t version);
        fpFatNode(OBJ&& obj, pds::version_t version);

        const OBJ& get_obj() const;
    };

    template <class OBJ>
    struct fpFatNodePtr{
        std::unordered_map<pds::version_t, std::shared_ptr<pds::fpFatNode<OBJ>>> table;
        pds::UnionFind versions_map;

        fpFatNodePtr(pds::version_t first_version);
        pds::version_t map(const pds::version_t version);
    };
};


////////////////////////////////////
/// fpFatNode
////////////////////////////////////

template <class OBJ>
pds::fpFatNode<OBJ>::fpFatNode(const OBJ& obj, pds::version_t version)

    : obj(obj), left(version), right(version) {
}

template <class OBJ>
pds::fpFatNode<OBJ>::fpFatNode(OBJ&& obj, pds::version_t version)

    : obj(std::move(obj)), left(version), right(version) {
}

template <class OBJ>
const OBJ& pds::fpFatNode<OBJ>::get_obj() const {

    return obj;
}


////////////////////////////////////
/// fpFatNodePtr
////////////////////////////////////

template <class OBJ>
pds::fpFatNodePtr<OBJ>::fpFatNodePtr(pds::version_t first_version) 
            
    : table{{MasterVersion, nullptr}, {first_version, nullptr}} {

    versions_map.add(first_version);
}

template <class OBJ>
pds::version_t pds::fpFatNodePtr<OBJ>::map(const pds::version_t version){

    if(version == MasterVersion)
        return MasterVersion;

    return versions_map.Find(version);
}


#endif /* FULLY_PERSISTENT_FAT_NODE_HPP */