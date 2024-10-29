#ifndef PERSISTENT_pFatNode_HPP
#define PERSISTENT_pFatNode_HPP

#include "Utils.hpp"

namespace pds{

    template <class OBJ>
    struct pFatNodePtr;

    template <class OBJ>
    class pFatNode{
        const OBJ obj;
    public:
        pds::pFatNodePtr<OBJ> left;
        pds::pFatNodePtr<OBJ> right;

        pFatNode(const OBJ& obj, pds::version_t version);
        pFatNode(OBJ&& obj, pds::version_t version);

        const OBJ& get_obj() const;
    };

    template <class OBJ>
    struct pFatNodePtr{
        std::unordered_map<pds::version_t, std::shared_ptr<pds::pFatNode<OBJ>>> table;
        std::vector<pds::version_t> nodes_versions;

        pFatNodePtr(pds::version_t first_version);
        pds::version_t map(const pds::version_t version) const;
    };
};


////////////////////////////////////
/// pFatNode
////////////////////////////////////

template <class OBJ>
pds::pFatNode<OBJ>::pFatNode(const OBJ& obj, pds::version_t version)

    : obj(obj), left(version), right(version) {
}

template <class OBJ>
pds::pFatNode<OBJ>::pFatNode(OBJ&& obj, pds::version_t version)

    : obj(std::move(obj)), left(version), right(version) {
}

template <class OBJ>
const OBJ& pds::pFatNode<OBJ>::get_obj() const {

    return obj;
}


////////////////////////////////////
/// pFatNodePtr
////////////////////////////////////

template <class OBJ>
pds::pFatNodePtr<OBJ>::pFatNodePtr(pds::version_t first_version) 
            
    : table{{MasterVersion, nullptr}, {first_version, nullptr}}, 
        nodes_versions{MasterVersion, first_version} {
}

template <class OBJ>
pds::version_t pds::pFatNodePtr<OBJ>::map(const pds::version_t version) const {

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


#endif /* PERSISTENT_pFatNode_HPP */