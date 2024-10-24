#include "internal/fptracker.hpp"

using namespace pds;

void test_nodes_table_basic();
void test_nodes_table_map();


namespace pds{
    struct nodes_table_test_t{
        pds::nodes_table<pds::nodes_table_test_t> left;
        pds::nodes_table<pds::nodes_table_test_t> right;
        nodes_table_test_t(const pds::version_t v) 
            : left(v), right(v) {}
    };
};

void test_nodes_table(){

    test_nodes_table_basic();
    std::cout << "\ntest_nodes_table_basic PASS\n";

    test_nodes_table_map();
    std::cout << "test_nodes_table_map PASS\n";

    std::cout << "\nALL nodes_table TESTS PASSED!!\n";
}


void test_nodes_table_basic(){

    // nodes_table<nodes_table_test_t> n_table(1);  // n_table.table = {1: nullptr}
    // assert(n_table.at(1) == nullptr);

    // /* create sp1 */
    // std::shared_ptr<nodes_table_test_t> sp1 = std::make_shared<nodes_table_test_t>(1);
    // assert(sp1.use_count() == 1);
    // assert(sp1->left.at(1) == nullptr);
    // assert(sp1->right.at(1) == nullptr);

    // /* map version 1 to sp1 */
    // n_table[1] = sp1; // n_table.table = {1: sp1}
    // assert(n_table[1] == sp1);
    // assert(n_table.at(1) == sp1);
    // assert(sp1->left.at(1) == nullptr);
    // assert(sp1->right.at(1) == nullptr);
    // assert(sp1.use_count() == 2);
}

void test_nodes_table_map(){

    // nodes_table<nodes_table_test_t> n_table(1);  // n_table.table = {1: nullptr}
    // assert(n_table.at(1) == nullptr);

    // /* create sp1 */
    // std::shared_ptr<nodes_table_test_t> sp1 = std::make_shared<nodes_table_test_t>(1);
    // assert(sp1.use_count() == 1);
    // assert(sp1->left.at(1) == nullptr);
    // assert(sp1->right.at(1) == nullptr);

    // /* map version 1 to sp1 */
    // n_table[1] = sp1; // n_table.table = {1: sp1}
    // assert(n_table[1] == sp1);
    // assert(n_table.at(1) == sp1);
    // assert(sp1->left.at(1) == nullptr);
    // assert(sp1->right.at(1) == nullptr);
    // assert(sp1.use_count() == 2);

    // /* add map from version 1 to version 2 */
    
    // n_table.extend_stack({{2, 1}});  // n_table.cow_stack = {{2, 1}}
    // assert(n_table.at(2) == sp1); // n_table.table = {1: sp1, 2: sp1}
    // assert(sp1.use_count() == 3);

    // /* create sp2 */
    // std::shared_ptr<nodes_table_test_t> sp2 = std::make_shared<nodes_table_test_t>(2);
    // assert(sp2.use_count() == 1);
    // assert(sp2->left.at(2) == nullptr);
    // assert(sp2->right.at(2) == nullptr);

    // /* map version 2 to sp2 */
    // n_table[2] = sp2; // n_table.table = {1: sp1, 2: sp2}
    // assert(n_table.at(2) == sp2);
    // assert(sp2.use_count() == 2);
    // assert(sp1.use_count() == 2);

    // /* add map from version 2 to version 1 */
    // n_table.extend_stack({{1, 2}});  // n_table.cow_stack = {{1, 2}}
    // assert(n_table.at(1) == sp2); // n_table.table = {1: sp2, 2: sp2}
    // assert(sp2.use_count() == 3);
    // assert(sp1.use_count() == 1);

    // assert(n_table.at(1) == n_table.at(2));
    // assert(n_table[1] == n_table[2]);

    // /* add version 3 with map to null */
    // n_table[3] = nullptr; // n_table.table = {1: sp2, 2: sp2, 3: nullptr}

    // /* map version 1 and version 2 to version 3 */
    // n_table.extend_stack({{1, 3}, {2, 3}}); // map 1->3, 2->3

    // assert(n_table.at(2) == nullptr);
    // assert(n_table.at(1) == nullptr); // n_table.table = {1: nullptr, 2: nullptr, 3: nullptr}
    // assert(sp2.use_count() == 1);
    // assert(sp1.use_count() == 1);
}

