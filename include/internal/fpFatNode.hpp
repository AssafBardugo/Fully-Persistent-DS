#ifndef FULLY_PERSISTENT_FAT_NODE_HPP
#define FULLY_PERSISTENT_FAT_NODE_HPP

#include "Utils.hpp"
#include "Excep.hpp"
#include "UnionFind.hpp"

namespace pds{

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