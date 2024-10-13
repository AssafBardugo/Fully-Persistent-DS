#ifndef PERSISTENT_DATA_STRUCTURE_TOOLS_HPP
#define PERSISTENT_DATA_STRUCTURE_TOOLS_HPP

#include "internal/fat_node_impl.hpp"

namespace pds{

    struct test_node_table{
        pds::node_table<pds::test_node_table> left;
        pds::node_table<pds::test_node_table> right;
        test_node_table(const pds::version_t v) 
            : left(v), right(v) {}
    };

};

#endif /* PERSISTENT_DATA_STRUCTURE_TOOLS_HPP */