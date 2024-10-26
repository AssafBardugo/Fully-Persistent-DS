#include "fpSet.hpp"

using namespace pds;
using namespace std;

#define PDS_RAND_ARR_SIZE 1000

void test_fpSet_basic();
void test_fpSet_readme_example();
void test_fpSet_insert();
void test_fpSet_insert_to_2_or_3();
void test_fpSet_remove();
void test_fpSet_contains();
void test_fpSet_to_vector();

void test_fpSet(){

    try{
        test_fpSet_basic();
        test_fpSet_readme_example();
        test_fpSet_insert();
        test_fpSet_insert_to_2_or_3();
        test_fpSet_remove();
        test_fpSet_contains();
        test_fpSet_to_vector();
    }
    catch(const pdsExcept& e){

        throw pdsExcept("test_fpSet: pdsExcept: " + string(e.what()));
    }
    catch(const exception& e){

        throw pdsExcept("test_fpSet: std::Exception: " + string(e.what()));
    }
    cout << "ALL fpSet tests PASSED!!" << endl;
}


void test_fpSet_basic(){

    vector<string> objs = {"a", "b", "c", "d"};
    vector<vector<string>> versions = {{"a"}, 
                                       {"a", "b"}, 
                                       {"a", "b", "c"}, 
                                       {"a", "b", "c", "d"}};

    fpSet<string> fps;
    version_t curr_v = 1;

    for(size_t i = 0; i < objs.size(); ++i){

        assert(fps.insert(objs[i]) == ++curr_v);

        assert(fps.curr_version() == curr_v);

        assert(fps.contains(objs[i], curr_v));
        assert(fps.contains(objs[i]));

        assert(fps.size(curr_v) == curr_v - 1);
        assert(fps.size() == curr_v - 1);

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

        assert(fps.to_vector(curr_v) == new_versions[i]);
        assert(fps.to_vector(i + 2) == versions[i]); // check that old version remain the same
    }
    assert(fps.to_vector() == new_versions.back());

    
    size_t v = curr_v;

    for(int i = objs.size() - 1; i >= 0; --i, --v){

        assert(fps.remove(new_versions[i][1], v) == ++curr_v);
        assert(fps.contains(new_versions[i][1], curr_v) == false);
    }

    cout << "fpSet::test_fpSet_basic PASSED!" << endl;
}


void test_fpSet_readme_example() {

    fpSet<string> my_fpSet; // Create a new fpSet with first version
    // Version 1: {}

    assert(my_fpSet.insert("a") == 2); // Create Version 2 by insert "a"
    // Version 1: {}
    // Version 2: {"a"}

    assert(my_fpSet.insert("b") == 3); // Create Version 3 by insert "b" to last version
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}

    assert(my_fpSet.insert("c", 2) == 4); // Insert "c" to version 2
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}
    // Version 4: {"a", "c"}

    assert(my_fpSet.remove("c") == 5); // Remove "c" from last version
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}
    // Version 4: {"a", "c"}
    // Version 5: {"a"}

    assert(my_fpSet.contains("c", 4) == true); // Version 4 stil contain "a"

    assert(my_fpSet.remove("b", 3) == 6); // Create a new version from version 3 without "b"
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}
    // Version 4: {"a", "c"}
    // Version 5: {"a"}
    // Version 6: {"a"}

    assert(my_fpSet.contains("b") == true); // "b" exist in some version
    
    assert(my_fpSet.size(1) == 0); // Version 1 has no elements
    assert(my_fpSet.size() == 3); // The size of all elements is 3: {"a", "b", "c"}

    // fpSet can return a set of specific version
    assert(my_fpSet.to_vector(4) == vector<string>({"a", "c"})); 
    assert(my_fpSet.to_vector() == vector<string>({"a", "b", "c"})); // All elements

    cout << "fpSet::test_fpSet_readme_example PASSED!" << endl;
}


void test_fpSet_insert(){

    /*** preperation for the pset tests ***/

    vector<int> objs(PDS_RAND_ARR_SIZE);
    srand(time(NULL));

    for(size_t i = 0; i < PDS_RAND_ARR_SIZE; ++i){

        objs[i] = (rand() % PDS_RAND_ARR_SIZE) + (i * PDS_RAND_ARR_SIZE);
    }

    // make versions:
    vector<vector<int>> versions(PDS_RAND_ARR_SIZE + 2);
    versions[1] = {};
    version_t curr_v = 2;

    for(size_t i = 0; i < PDS_RAND_ARR_SIZE / 2; ++i){

        versions[curr_v].insert(versions[curr_v].end(), versions[curr_v - 1].begin(), versions[curr_v - 1].end());
        versions[curr_v].push_back(objs[i]);
        sort(versions[curr_v].begin(), versions[curr_v].end());
        ++curr_v;
    }
    /*** ***/


    fpSet<int> fps;
    curr_v = 1;

    for(size_t i = 0; i < PDS_RAND_ARR_SIZE / 2; ++i){

        assert(fps.insert(objs[i]) == ++curr_v);
        assert(fps.curr_version() == curr_v);

        assert(fps.contains(objs[i], curr_v) == true);
        assert(fps.contains(objs[i]) == true); // master version

        assert(fps.to_vector(curr_v) == versions[curr_v]);
    }

    for(size_t i = PDS_RAND_ARR_SIZE / 2; i < PDS_RAND_ARR_SIZE; ++i){

        version_t insert_to = 2 + (rand() % (curr_v - 2));

        assert(fps.insert(objs[i], insert_to) == ++curr_v);
        assert(fps.curr_version() == curr_v);

        assert(fps.contains(objs[i], curr_v));
        assert(fps.contains(objs[i], insert_to) == false);
        assert(fps.contains(objs[i]));

        versions[curr_v].insert(versions[curr_v].end(), versions[insert_to].begin(), versions[insert_to].end());
        versions[curr_v].push_back(objs[i]);
        sort(versions[curr_v].begin(), versions[curr_v].end());

        assert(fps.size(curr_v) == versions[curr_v].size());
        assert(fps.to_vector(curr_v) == versions[curr_v]);
    }

    sort(objs.begin(), objs.end());
    assert(fps.to_vector() == objs);

    cout << "fpSet::test_fpSet_insert PASSED!" << endl;
}


void test_fpSet_insert_to_2_or_3(){

    /*** preperation for the pset tests ***/

    vector<int> objs(PDS_RAND_ARR_SIZE);
    srand(time(NULL));

    for(size_t i = 0; i < PDS_RAND_ARR_SIZE; ++i){

        objs[i] = (rand() % PDS_RAND_ARR_SIZE) + (i * PDS_RAND_ARR_SIZE);
    }

    // make versions:
    vector<vector<int>> versions(PDS_RAND_ARR_SIZE + 3);
    versions[2] = {objs[0]};
    versions[3] = {objs[0], objs[1]};
    version_t curr_v = 4;

    for(size_t i = 2; i < PDS_RAND_ARR_SIZE; ++i){

        version_t add_to = 2 + (i % 2);
        versions[curr_v].insert(versions[curr_v].end(), versions[add_to].begin(), versions[add_to].end());
        versions[curr_v].push_back(objs[i]);
        sort(versions[curr_v].begin(), versions[curr_v].end());
        ++curr_v;
    }
    /*** ***/

    fpSet<int> fps;
    curr_v = 1;
    assert(fps.insert(objs[0]) == ++curr_v);
    assert(fps.insert(objs[1]) == ++curr_v);

    for(size_t i = 2; i < PDS_RAND_ARR_SIZE; ++i){

        version_t insert_to = 2 + (i % 2);

        assert(fps.insert(objs[i], insert_to) == ++curr_v);
        assert(fps.curr_version() == curr_v);

        assert(fps.contains(objs[i], curr_v));
        assert(fps.contains(objs[i]));

        assert(fps.to_vector(curr_v) == versions[curr_v]);
    }

    sort(objs.begin(), objs.end());
    assert(fps.to_vector() == objs);

    cout << "fpSet::test_fpSet_insert_to_2_or_3 PASSED!" << endl;
}


void test_fpSet_remove(){

    // TODO
}


void test_fpSet_contains(){

    // TODO
}


void test_fpSet_to_vector(){

    // TODO
}

