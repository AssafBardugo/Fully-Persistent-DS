#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP

#include "utils.hpp"
#include "excep.hpp"

namespace pds{

    template <class FN>
    struct nodes_table{

        std::unordered_map<pds::version_t, std::shared_ptr<FN>> table;
        std::vector<std::pair<pds::version_t, pds::version_t>> cow_stack;

        nodes_table(const pds::version_t create_version);

        nodes_table(const nodes_table&) = delete;
        nodes_table& operator=(const nodes_table&) = delete;

        void extend_stack(const std::vector<std::pair<pds::version_t, pds::version_t>>& to_push){

            cow_stack.insert(cow_stack.end(), to_push.begin(), to_push.end());
        }
    };

    template <class OBJ>
    class fat_node{

        const OBJ obj;

    public:
        pds::nodes_table<fat_node<OBJ>> left;
        pds::nodes_table<fat_node<OBJ>> right;

        fat_node(const OBJ& obj, const pds::version_t new_version);
        fat_node(OBJ&& obj, const pds::version_t new_version);

        fat_node(const fat_node&) = delete;
        fat_node& operator=(const fat_node&) = delete;

        const OBJ& get_obj() const;

        bool operator<(const fat_node<OBJ>& other) const;
        bool operator==(const fat_node<OBJ>& other) const;
    };
};

using namespace pds;

template <class FN>
nodes_table<FN>::nodes_table(const version_t create_version)

    : table{{create_version, nullptr}} {
}

template <class OBJ>
fat_node<OBJ>::fat_node(const OBJ& obj, const version_t new_version)

    : obj(obj), left(new_version), right(new_version) {
}

template <class OBJ>
fat_node<OBJ>::fat_node(OBJ&& obj, const version_t new_version)

    : obj(std::move(obj)), left(new_version), right(new_version) {
}

template <class OBJ>
const OBJ& fat_node<OBJ>::get_obj() const {

    return obj;
}

template <class OBJ>
bool fat_node<OBJ>::operator<(const fat_node<OBJ>& other) const {

    return obj < other.obj;
}

template <class OBJ>
bool fat_node<OBJ>::operator==(const fat_node<OBJ>& other) const {

    return obj == other.obj;
}


#endif /* PERSISTENT_DATA_STRUCTURE_FAT_NODE_HPP */