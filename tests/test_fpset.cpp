#include "pds_test.hpp"

using namespace pds;

void readme_example();

void test_fpset(){
    

    //std::cout << "\nALL fpset TESTS PASSED!!\n";
}



void readme_example() {

    pds::fpset<std::string> my_fpset; // Create a new fpset with first version
    // Version 1: {}

    assert(my_fpset.insert("a") == 2); // Create Version 2 by insert "a"
    // Version 1: {}
    // Version 2: {"a"}

    assert(my_fpset.insert("b") == 3); // Create Version 3 by insert "b" to last version
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}

    assert(my_fpset.insert("c", 2) == 4); // Insert "c" to version 2
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}
    // Version 4: {"a", "c"}

    assert(my_fpset.remove("c") == 5); // Remove "c" from last version
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}
    // Version 4: {"a", "c"}
    // Version 5: {"a"}

    assert(my_fpset.contains("c", 4) == true); // Version 4 stil contain "a"

    assert(my_fpset.remove("b", 3) == 6); // Create a new version from version 3 without "b"
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}
    // Version 4: {"a", "c"}
    // Version 5: {"a"}
    // Version 6: {"a"}

    assert(my_fpset.contains("b") == true); // "b" exist in some version
    
    assert(my_fpset.size(1) == 0); // Version 1 has no elements
    assert(my_fpset.size() == 3); // The size of all elements is 3: {"a", "b", "c"}

    // fpset can return a set of specific version
    assert(my_fpset.to_set(4) == std::set<std::string>({"a", "c"})); 
    assert(my_fpset.to_set() == std::set<std::string>({"a", "b", "c"})); // All elements
}
