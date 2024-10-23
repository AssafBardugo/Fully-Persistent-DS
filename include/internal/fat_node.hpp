#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP

#include "utils.hpp"
#include "excep.hpp"

namespace pds{

    template <class OBJ>
    class fat_node;

    template <class OBJ>
    struct fat_node_ptr{
        std::unordered_map<pds::version_t, std::shared_ptr<pds::fat_node<OBJ>>> table;
        std::vector<pds::version_t> nodes_versions;

        fat_node_ptr(pds::version_t first_version) 
            : table{first_version, nullptr}, nodes_versions{first_version} {}

        // for fully persistent only
        pds::version_t map(pds::version_t version){

            return *std::lower_bound(nodes_versions.begin(), nodes_versions.end(), version);
        }
    };

    template <class OBJ>
    struct fat_node{
        const std::shared_ptr<OBJ> obj_ptr;

        pds::fat_node_ptr<OBJ> left;
        pds::fat_node_ptr<OBJ> right;

        fat_node(std::shared_ptr<OBJ>& obj_ptr, pds::version_t version);

        const OBJ& get_obj() const;
    };
};

template <class OBJ>
pds::fat_node<OBJ>::fat_node(std::shared_ptr<OBJ>& obj_ptr, pds::version_t version)

    : obj_ptr(obj_ptr), left(version), right(version) {
}

template <class OBJ>
const OBJ& pds::fat_node<OBJ>::get_obj() const {

    return *obj_ptr;
}

#endif /* PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP */