#ifndef PERSISTENT_DATA_STRUCTURE_TOOLS_TESTS_HPP
#define PERSISTENT_DATA_STRUCTURE_TOOLS_TESTS_HPP

#include "internal/nodes_table.hpp"
#include "internal/fat_node.hpp"
#include "fpset.hpp"
#include "utils.hpp"

#define PDS_TESTS_NUM 3


namespace pds{

    using test_func_t = void (*)(void);

    test_func_t tests_func[PDS_TESTS_NUM] = {
        test_nodes_table, 
        test_fat_node, 
        test_fpset
    };

    const char* tests_name[PDS_TESTS_NUM] = {
        "test_nodes_table", 
        "test_fat_node", 
        "test_fpset"
    };


    struct nodes_table_test_t{
        pds::nodes_table<pds::nodes_table_test_t> left;
        pds::nodes_table<pds::nodes_table_test_t> right;
        nodes_table_test_t(const pds::version_t v) 
            : left(v), right(v) {}
    };

};

#endif /* PERSISTENT_DATA_STRUCTURE_TOOLS_TESTS_HPP */