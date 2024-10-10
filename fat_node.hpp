#ifndef FAT_NODE_HPP
#define FAT_NODE_HPP

#include "node_table_t.hpp"

namespace pds{

    template <class OBJ>
    class fat_node{
        const OBJ obj;
        pds::node_table_t<fat_node<OBJ>> left;
        pds::node_table_t<fat_node<OBJ>> right;

        const pds::version_t first_version;
        pds::version_t last_version;
        pds::version_t last_mapped;

    public:
        fat_node(const OBJ& obj, const pds::version_t version) 
            : obj(obj), first_version(version), 

            // TODO:
            left(node_table_t()), right(node_table_t()) {
                
            last_version = last_mapped = version;
        }

        fat_node(OBJ&& obj, const pds::version_t version) 
            : obj(std::move(obj)), first_version(version), 

            // TODO:
            left(node_table_t()), right(node_table_t()) {

            last_version = last_mapped = version;
        }

        // note: If eventually I will need a D'tor, it has to be supply to the shared_ptr C'tor.
        fat_node(const fat_node&) = delete;
        fat_node& operator=(const fat_node&) = delete;

        std::shared_ptr<fat_node<OBJ>> left_ptr(const pds::version_t version);
        std::shared_ptr<fat_node<OBJ>> right_ptr(const pds::version_t version);

        // fat_node<OBJ>& left_ref(const pds::version_t version);
        // fat_node<OBJ>& right_ref(const pds::version_t version);

        // TODO: for the new_version: node_ptr->left&right[new_version] should be nullptr
        void insert_left(std::shared_ptr<fat_node<OBJ>>& node_ptr, const pds::version_t new_version);
        void insert_right(std::shared_ptr<fat_node<OBJ>>& node_ptr, const pds::version_t new_version);

        void map_new_version(const pds::version_t new_version);

        bool operator<(const fat_node& other) const {
            return obj < other.obj;
        }

        bool operator<(const OBJ& other_obj) const {
            return obj < other_obj;
        }

        bool operator>(const OBJ& other_obj) const {
            return other_obj < obj;
        }

    private:

    };
};

#endif /* FAT_NODE_HPP */