#ifndef FAT_NODE_HPP
#define FAT_NODE_HPP

#include "utils.hpp"

namespace pds{
    
    template <class FN>
    class node_table{
        std::unordered_map<pds::version_t, std::shared_ptr<FN>> table;

        // TODO: replace the pair with a struct
        std::vector<std::pair<pds::version_t, pds::version_t>> cow_stack;
        pds::version_t last_version;
        pds::version_t last_mapped;
        
    public:
        explicit node_table(const pds::version_t create_version);

        // performing an insertion if such key does not already exist.
        std::shared_ptr<FN>& operator[](const pds::version_t key);

        // return copy of val. excep if key out of range
        std::shared_ptr<FN>& at(const pds::version_t key);

        void map(const std::vector<std::pair<pds::version_t, pds::version_t>>& to_push);
    };

    template <class OBJ>
    class fat_node{
        const OBJ obj;

    public:
        pds::node_table<fat_node<OBJ>> left;
        pds::node_table<fat_node<OBJ>> right;

        fat_node(const OBJ& obj, const pds::version_t new_version);
        fat_node(OBJ&& obj, const pds::version_t new_version);

        fat_node(const fat_node&) = delete;
        fat_node& operator=(const fat_node&) = delete;

        bool operator<(const fat_node& other) const;
        bool operator<(const OBJ& other_obj) const;
        bool operator>(const OBJ& other_obj) const;
    };
};

#endif /* FAT_NODE_HPP */