#ifndef PERSISTENT_DATA_STRUCTURE_TEST_HPP
#define PERSISTENT_DATA_STRUCTURE_TEST_HPP

#include "pSet.hpp"
#include "fpSet.hpp"

#define PDS_TESTS_NUM 2

void test_pSet();
void test_fpSet();


namespace pds{

    using TestFunc_t = void (*)();

    TestFunc_t TestFuncArr[PDS_TESTS_NUM] = {
        test_pSet,
        test_fpSet
    };

    const std::string tests_name[PDS_TESTS_NUM] = {
        "test_pSet", 
        "test_fpSet"
    };
};

#endif /* PERSISTENT_DATA_STRUCTURE_TEST_HPP */