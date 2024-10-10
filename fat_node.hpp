#ifndef FAT_NODE_HPP
#define FAT_NODE_HPP

#include "node_table_t.hpp"

namespace pds{

    template <class OBJ>
    class fat_node{
        OBJ obj;
        pds::node_table_t<fat_node<OBJ>> left;
        pds::node_table_t<fat_node<OBJ>> right;

        const pds::version_t first_version;
        pds::version_t last_version;
        pds::version_t last_mapped;

    public:
        fat_node();
        fat_node(const fat_node&) = delete;
        fat_node& operator=(const fat_node&) = delete;

        std::shared_ptr<fat_node<T>> left_ptr(const pds::version_t version);
        std::shared_ptr<fat_node<T>> right_ptr(const pds::version_t version);

        fat_node<OBJ>& left_ref(const pds::version_t version);
        fat_node<OBJ>& right_ref(const pds::version_t version);

    private:



    public:
        /*
            If we give an order-relation on fat_node according to the insert-version of the object, 
            we can have duplicates of objects in the structure.
            And so we can define a List structure.
        */
        bool operator<(fat_node& other) const;
    };
};


// template <class T>
// class fplist_node : public fat_node{

// public:
//     /*
//         If we give an order-relation on fat_node according to the insert-version of the object, 
//         we can have duplicates of objects in the structure.
//         And so we can define a List structure.
//     */
//     bool operator<(fat_node& other) const;
// };

#endif /* FAT_NODE_HPP */