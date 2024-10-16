#include "internal/fat_node.hpp"

using namespace pds;

void test_fat_node_basic();

void test_fat_node(){

    test_fat_node_basic();
    std::cout << "\ntest_fat_node_basic PASS\n";

    
    
    std::cout << "\nALL fat_node TESTS PASSED!!\n";
}


void test_fat_node_basic(){

    std::string obj1 = "a";

    fat_node<std::string> fn1(obj1, 1);
    fat_node<std::string> fn2("b", 1);

    assert(fn1 < fn2);
}

