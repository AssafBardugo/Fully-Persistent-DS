#ifndef PERSISTENT_DATA_STRUCTURE_TEST_HPP
#define PERSISTENT_DATA_STRUCTURE_TEST_HPP

#include "internal/fat_node.hpp"
#include "internal/fat_node_tracker.hpp"
#include "fpset.hpp"

#define PDS_TESTS_NUM 3

void test_nodes_table();
void test_fat_node();
void test_fpset();


namespace pds{

    using TestFunc_t = void (*)();

    TestFunc_t TestFuncArr[PDS_TESTS_NUM] = {
        test_nodes_table,
        test_fat_node,
        test_fpset
    };

    const std::string tests_name[PDS_TESTS_NUM] = {
        "test_nodes_table", 
        "test_fat_node", 
        "test_fpset"
    };
};

#endif /* PERSISTENT_DATA_STRUCTURE_TEST_HPP */