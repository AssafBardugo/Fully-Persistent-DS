#ifndef PERSISTENT_DATA_STRUCTURE_TOOLS_HPP
#define PERSISTENT_DATA_STRUCTURE_TOOLS_HPP

#include "internal/fat_node_impl.hpp"

namespace pds{

    struct nodes_table_test_t{
        pds::nodes_table<pds::nodes_table_test_t> left;
        pds::nodes_table<pds::nodes_table_test_t> right;
        nodes_table_test_t(const pds::version_t v) 
            : left(v), right(v) {}
    };

};

#endif /* PERSISTENT_DATA_STRUCTURE_TOOLS_HPP */