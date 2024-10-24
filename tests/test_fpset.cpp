#include "fpset.hpp"

using namespace pds;
using namespace std;


void test_basic();
// void test_readme_example();

int main(){

    try{
        test_basic();

    }
    catch(const pdsExcept& e){
        cout << "pdsExcept: " << e.what() << endl;
        exit(1);
    }
    catch(const exception& e){
        cout << "Exception: " << e.what() << endl;
        exit(1);
    }
    cout << "ALL fpset tests PASSED!!" << endl;
    return 0;
}


void test_basic(){

    vector<string> objs = {"a", "b", "c", "d"};
    vector<vector<string>> versions = {{"a"}, 
                                       {"a", "b"}, 
                                       {"a", "b", "c"}, 
                                       {"a", "b", "c", "d"}};

    fpset<string> fps;
    version_t curr_v = 1;

    for(size_t i = 0; i < objs.size(); ++i){

        assert(fps.insert(objs[i]) == ++curr_v);

        assert(fps.curr_version() == curr_v);

        assert(fps.contains(objs[i], curr_v));
        assert(fps.contains(objs[i]));

        assert(fps.size(curr_v) == i + 1);
        assert(fps.size() == i + 1);

        assert(fps.to_vector(curr_v) == versions[i]);
    }
    assert(fps.to_vector() == versions.back());


    string new_obj = "e";
    vector<vector<string>> new_versions = {{"a", "e"}, 
                                           {"a", "b", "e"}, 
                                           {"a", "b", "c", "e"}, 
                                           {"a", "b", "c", "d", "e"}};

    // insert new_obj to every version
    for(size_t i = 0; i < objs.size(); ++i){

        assert(fps.insert(new_obj, i + 2) == ++curr_v);

        assert(fps.curr_version() == curr_v);

        assert(fps.contains(new_obj, curr_v));
        assert(fps.contains(new_obj, i + 2) == false);
        assert(fps.contains(new_obj));

        assert(fps.size(curr_v) == fps.size(i + 2) + 1);
        assert(fps.size() == objs.size() + 1);

        fps.print(curr_v);
        // assert(fps.to_vector(curr_v) == new_versions[i]);
        assert(fps.to_vector(i + 2) == versions[i]); // check that old version remain the same
    }
    assert(fps.to_vector() == new_versions.back());


    cout << "fpset::test_basic PASSED!" << endl;
}


// void test_readme_example() {

//     fpset<string> my_fpset; // Create a new fpset with first version
//     // Version 1: {}

//     assert(my_fpset.insert("a") == 2); // Create Version 2 by insert "a"
//     // Version 1: {}
//     // Version 2: {"a"}

//     assert(my_fpset.insert("b") == 3); // Create Version 3 by insert "b" to last version
//     // Version 1: {}
//     // Version 2: {"a"}
//     // Version 3: {"a", "b"}

//     assert(my_fpset.insert("c", 2) == 4); // Insert "c" to version 2
//     // Version 1: {}
//     // Version 2: {"a"}
//     // Version 3: {"a", "b"}
//     // Version 4: {"a", "c"}

//     assert(my_fpset.remove("c") == 5); // Remove "c" from last version
//     // Version 1: {}
//     // Version 2: {"a"}
//     // Version 3: {"a", "b"}
//     // Version 4: {"a", "c"}
//     // Version 5: {"a"}

//     assert(my_fpset.contains("c", 4) == true); // Version 4 stil contain "a"

//     assert(my_fpset.remove("b", 3) == 6); // Create a new version from version 3 without "b"
//     // Version 1: {}
//     // Version 2: {"a"}
//     // Version 3: {"a", "b"}
//     // Version 4: {"a", "c"}
//     // Version 5: {"a"}
//     // Version 6: {"a"}

//     assert(my_fpset.contains("b") == true); // "b" exist in some version
    
//     assert(my_fpset.size(1) == 0); // Version 1 has no elements
//     assert(my_fpset.size() == 3); // The size of all elements is 3: {"a", "b", "c"}

//     // fpset can return a set of specific version
//     assert(my_fpset.to_set(4) == set<string>({"a", "c"})); 
//     assert(my_fpset.to_set() == set<string>({"a", "b", "c"})); // All elements
// }
