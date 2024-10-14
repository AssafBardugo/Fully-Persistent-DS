#include "internal/fat_node.hpp"

using namespace pds;

void test_fat_node_basic();
void test_fat_node_with_nodes_table();

void test_fat_node(){

    test_fat_node_basic();
    std::cout << "\ntest_fat_node_basic PASS\n";

    test_fat_node_with_nodes_table();
    std::cout << "test_fat_node_with_nodes_table PASS\n";
    
    std::cout << "\nALL fat_node TESTS PASSED!!\n";
}


void test_fat_node_basic(){

    std::string obj1 = "a";
    std::string obj2 = "b";

    fat_node<std::string> fn1(obj1, 1);
    fat_node<std::string> fn2("b", 1);

    assert(fn1 < fn2);
    assert(fn1 < obj2);
    assert((fn1 > obj2) == false);
}

void test_fat_node_with_nodes_table(){

    std::string objs[] = {"a", "b", "c"};

    fat_node<std::string> fn1(objs[0], 1);

    std::shared_ptr<fat_node<std::string>> sp2 = std::make_shared<fat_node<std::string>>(objs[1], 2);
    std::shared_ptr<fat_node<std::string>> sp3 = std::make_shared<fat_node<std::string>>(objs[2], 3);

    fn1.right[2] = sp2;
    sp2->right[2] = sp3;

    assert(fn1.right.at(2) == sp2);
    assert(fn1.right.at(2)->right.at(2) == sp3);

    assert(sp2.use_count() == 2);
    assert(sp3.use_count() == 2);
}
