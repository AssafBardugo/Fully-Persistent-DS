#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP

#include "nodes_table.hpp"

namespace pds{

    template <class OBJ>
    class fat_node{
    public:
        const OBJ obj;
        pds::nodes_table<fat_node<OBJ>> left;
        pds::nodes_table<fat_node<OBJ>> right;

        fat_node(const OBJ& obj, const pds::version_t new_version);
        fat_node(OBJ&& obj, const pds::version_t new_version);

        bool operator<(const fat_node<OBJ>& other) const;
        bool operator==(const fat_node<OBJ>& other) const;
    };


    template <class OBJ>
    fat_node<OBJ>::fat_node(const OBJ& obj, const pds::version_t new_version)

        : obj(obj), left(new_version), right(new_version) {
    }

    template <class OBJ>
    fat_node<OBJ>::fat_node(OBJ&& obj, const pds::version_t new_version)

        : obj(std::move(obj)), left(new_version), right(new_version) {
    }

    template <class OBJ>
    bool fat_node<OBJ>::operator<(const fat_node<OBJ>& other) const {
 
        return obj < other.obj;
    }

    template <class OBJ>
    bool fat_node<OBJ>::operator==(const fat_node<OBJ>& other) const {

        return obj == other.obj;
    }
};

#endif /* PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP */