#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP

#include "nodes_table.hpp"

namespace pds{

    template <class OBJ>
    class fat_node{

        const OBJ obj;

    public:
        pds::nodes_table<fat_node<OBJ>> left;
        pds::nodes_table<fat_node<OBJ>> right;

        fat_node(const OBJ& obj, const pds::version_t new_version)

            : obj(obj), left(new_version), right(new_version) {
        }

        fat_node(OBJ&& obj, const pds::version_t new_version)

            : obj(std::move(obj)), left(new_version), right(new_version) {
        }

        fat_node(const fat_node&) = delete;
        fat_node& operator=(const fat_node&) = delete;

        bool operator<(const fat_node& other) const{
            return obj < other.obj;
        }

        bool operator<(const OBJ& other_obj) const{
            return obj < other_obj;
        }

        bool operator>(const OBJ& other_obj) const{
            return other_obj < obj;
        }
    };
};

#endif /* PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP */