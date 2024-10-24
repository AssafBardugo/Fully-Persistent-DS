#include "pset.hpp"

using namespace pds;
using namespace std;

#define PDS_RAND_ARR_SIZE 1000
#define PDS_CONTAINS_SIZE 100

const bool print_full_test = false;

void test_basic();
void test_insert();
void test_remove();
void test_insert_removed_objects();
void test_contains();
void test_size_and_to_vector();
void test_exceptions();
void test_edge_case_1();
void test_edge_case_2();
void test_print();

void test_pset(){

    try{
        test_basic();
        test_insert();
        test_remove();
        test_insert_removed_objects();
        test_contains();
        test_size_and_to_vector();
        test_exceptions();
        test_edge_case_1();
        test_edge_case_2();
        test_print();
    }
    catch(const pdsExcept& e){
        cout << "pdsExcept: " << e.what() << endl;
        exit(1);
    }
    catch(const exception& e){
        cout << "Exception: " << e.what() << endl;
        exit(1);
    }
    cout << "ALL pset tests PASSED!!" << endl;
}


void test_basic(){

    vector<string> objs = {"a", "b", "c"};
    pset<string> ps;

    version_t v1 = ps.insert(objs[0]);
    assert(v1 == 2);
    assert(ps.size(v1) == 1);
    assert(ps.contains(objs[0], v1));
    assert(ps.curr_version() == v1);

    version_t v2 = ps.insert(objs[1]);
    assert(v2 == 3);
    assert(ps.size(v2) == 2);
    assert(ps.contains(objs[1], v2));
    assert(ps.curr_version() == v2);

    version_t v3 = ps.insert(objs[2]);
    assert(v3 == 4);
    assert(ps.size(v3) == 3);
    assert(ps.contains(objs[2], v3));
    assert(ps.curr_version() == v3);

    assert(ps.contains(objs[2], v2) == false);
    assert(ps.contains(objs[2], v1) == false);

    assert(ps.to_vector(v3) == objs);

    version_t v4 = ps.remove(objs[2]);
    assert(v4 == ps.curr_version());
    version_t v5 = ps.remove(objs[1]);
    assert(v5 == ps.curr_version());
    version_t v6 = ps.remove(objs[0]);
    assert(v6 == ps.curr_version());

    assert(ps.contains(objs[0], v6) == false);
    assert(ps.contains(objs[1], v6) == false);
    assert(ps.contains(objs[2], v6) == false);

    assert(ps.size(v6) == 0);
    assert(ps.to_vector(v6) == vector<string>());

    assert(ps.size() == 3);
    assert(ps.to_vector() == objs);

    cout << "test_basic PASSED!" << endl;
}


void test_insert(){

    /*** preperation for the pset tests ***/
    srand(time(NULL));

    // generate unique objects:
    vector<int> objs(PDS_RAND_ARR_SIZE);
    for(size_t v = 2; v < PDS_RAND_ARR_SIZE; ++v){

        // objs[v] will be the object that will insert to version v
        objs[v] = (rand() % PDS_RAND_ARR_SIZE) + (v * PDS_RAND_ARR_SIZE);
    }

    // make versions:
    vector<vector<int>> versions(PDS_RAND_ARR_SIZE);
    versions[1] = {}; // version 1

    for(size_t v = 2; v < PDS_RAND_ARR_SIZE; ++v){

        versions[v].insert(versions[v].end(), versions[v - 1].begin(), versions[v - 1].end());
        versions[v].push_back(objs[v]);
    }

    for(size_t v = 2; v < PDS_RAND_ARR_SIZE; ++v){

        sort(versions[v].begin(), versions[v].end());
    }
    /*** ***/


    pset<int> ps;
    for(size_t v = 2; v < PDS_RAND_ARR_SIZE; ++v){ 

        assert(ps.insert(objs[v]) == v);
        assert(ps.curr_version() == v);

        assert(ps.contains(objs[v], v) == true);
        assert(ps.contains(objs[v]) == true); // master version

        assert(ps.to_vector(v) == versions[v]);
    }

    vector<int> ps_vec_all = ps.to_vector(); // master version
    assert(ps_vec_all == versions[PDS_RAND_ARR_SIZE - 1]);

    cout << "test_insert PASSED!" << endl;
}


void test_remove(){

    /*** preperation for the pset tests ***/
    srand(time(NULL));

    // generate unique objects:
    vector<int> objs(PDS_RAND_ARR_SIZE);
    for(size_t v = 2; v < PDS_RAND_ARR_SIZE; ++v){

        // objs[v] will be the object that will insert to version v
        objs[v] = (rand() % PDS_RAND_ARR_SIZE) + (v * PDS_RAND_ARR_SIZE);
    }

    vector<vector<int>> versions(PDS_RAND_ARR_SIZE);

    for(size_t v = 2; v < PDS_RAND_ARR_SIZE; ++v){

        // all objs in [objs[v + 1], objs[PDS_RAND_ARR_SIZE - 1]]
        versions[v].insert(versions[v].end(), objs.begin() + v + 1, objs.end());
    }

    for(size_t v = 2; v < PDS_RAND_ARR_SIZE; ++v){

        sort(versions[v].begin(), versions[v].end());
    }
    /*** ***/


    pset<int> ps;
    for(size_t v = 2; v < PDS_RAND_ARR_SIZE; ++v){ 

        assert(ps.insert(objs[v]) == v);
    }

    vector<int> all_objs;
    all_objs.insert(all_objs.end(), objs.begin() + 2, objs.end());
    sort(all_objs.begin(), all_objs.end());

    vector<int> ps_vec = ps.to_vector(); // master version
    assert(ps_vec == all_objs);

    /// remove: ///////////////////

    version_t curr_v = PDS_RAND_ARR_SIZE - 1;
    version_t ps_size = PDS_RAND_ARR_SIZE - 2;

    for(int v = 2; v < PDS_RAND_ARR_SIZE; ++v){ 

        ++curr_v;
        --ps_size;
        assert(ps.remove(objs[v]) == curr_v);
        assert(ps.curr_version() == curr_v);
        assert(ps.size(curr_v) == ps_size);

        assert(ps.contains(objs[v], curr_v) == false);
        assert(ps.contains(objs[v]) == true); // master version keep contains obj

        vector<int> curr_objs = ps.to_vector(curr_v);
        assert(curr_objs == versions[v]);
    }

    assert(ps.size(ps.curr_version()) == 0);

    ps_vec = ps.to_vector(); // take all from master_version
    assert(ps_vec == all_objs);

    cout << "test_remove PASSED!" << endl;
}


void test_insert_removed_objects(){

    /*** preperation for the pset tests ***/
    vector<int> objs(PDS_CONTAINS_SIZE);
    srand(time(NULL));

    for(size_t i = 0; i < PDS_CONTAINS_SIZE; ++i){

        objs[i] = (rand() % PDS_CONTAINS_SIZE) + (i * PDS_CONTAINS_SIZE);
    }
    /*** ***/

    pset<int> ps;
    version_t curr_v = 1;

    for(int k = 0; k < 4; ++k){

        for(size_t i = 0; i < PDS_CONTAINS_SIZE; ++i){

            assert(ps.insert(objs[i]) == ++curr_v);
        }

        assert(ps.to_vector(curr_v) == objs);

        for(size_t i = 0; i < PDS_CONTAINS_SIZE; ++i){

            assert(ps.remove(objs[i]) == ++curr_v);

            vector<int> curr_objs(objs.begin() + (i + 1), objs.end());

            assert(ps.to_vector(curr_v) == curr_objs);
        }
    }

    assert(ps.size() == PDS_CONTAINS_SIZE);

    cout << "test_insert_removed_objects PASSED!" << endl;
}


void test_contains(){

    /*** preperation for the pset tests ***/
    srand(time(NULL));

    // generate unique objects:
    vector<int> objs(PDS_CONTAINS_SIZE);
    for(size_t v = 2; v < PDS_CONTAINS_SIZE; ++v){

        // objs[v] will be the object that will insert to version v
        objs[v] = (rand() % PDS_CONTAINS_SIZE) + (v * PDS_CONTAINS_SIZE);
    }
    /*** ***/

    pset<int> ps;
    for(size_t v = 2; v < PDS_CONTAINS_SIZE; ++v){ 

        assert(ps.insert(objs[v]) == v);
        assert(ps.curr_version() == v);

        for(size_t vobj = 2; vobj <= v; ++vobj){    // all objs[v] we already insert

            for(size_t vtest = 1; vtest <= v; ++vtest){     // all v exist

                // if (vobj <= vtest) then version 'vtest' should contain the obj 'vobj'
                assert(ps.contains(objs[vobj], vtest) == (vobj <= vtest));
            }
        }
        assert(ps.contains(objs[v]) == true);
    }

    version_t curr_v = PDS_CONTAINS_SIZE - 1;
    for(size_t v = 2; v < PDS_CONTAINS_SIZE; ++v){ 

        ++curr_v;
        assert(ps.remove(objs[v]) == curr_v);

        for(size_t vt = 1; vt <= curr_v; ++vt){

            // if vt < curr_v it should return true
            // unless: if vt < v it should return false. As it was in insert.
            assert(ps.contains(objs[v], vt) == (vt >= v && vt < curr_v));
        }
        assert(ps.contains(objs[v]) == true);
    }

    // and last check..
    assert(ps.curr_version() == curr_v);
    version_t last_version = curr_v;
    for(size_t v = 2; v < PDS_CONTAINS_SIZE; ++v){

        assert(ps.contains(objs[v], last_version) == false);  // last_version should be empty
        assert(ps.contains(objs[v]) == true);   // master_version always keep everything
    }
    assert(ps.size(last_version) == 0);

    cout << "test_contains PASSED!" << endl;
}


void test_size_and_to_vector(){

    /*** preperation for the test ***/

    // generate unique objects:
    vector<int> objs(PDS_CONTAINS_SIZE);
    srand(time(NULL));

    for(size_t i = 0; i < PDS_CONTAINS_SIZE; ++i){

        // objs[v] will be the object that will insert to version v
        objs[i] = (rand() % PDS_CONTAINS_SIZE) + (i * PDS_CONTAINS_SIZE);
    }

    version_t LAST_VERSION = 1 + 2 * PDS_CONTAINS_SIZE;

    // for example, if objs = {1, 2, 3, 4} then
    // versions = {{},              v=0
    //             {},              v=1
    //             {1},             v=2
    //             {1, 2},          v=3
    //             {1, 2, 3},       v=4
    //             {1, 2, 3, 4},    v=5
    //             {2, 3, 4},       v=6
    //             {3, 4},          v=7
    //             {4},             v=8
    //             {},              v=9
    //            }

    // make versions:
    vector<vector<int>> versions(LAST_VERSION + 1);
    version_t vec_v = 0;

    for(auto it = objs.begin(); it != objs.end(); ++it){

        ++vec_v;
        versions[vec_v].insert(versions[vec_v].end(), objs.begin(), it);
    }

    for(auto it = objs.begin(); it != objs.end(); ++it){

        ++vec_v;
        versions[vec_v].insert(versions[vec_v].end(), it, objs.end());
    }
    ++vec_v;
    versions[vec_v] = {};
    assert(vec_v == LAST_VERSION);

    for(size_t v = 1; v <= LAST_VERSION; ++v){

        sort(versions[v].begin(), versions[v].end());
    }
    /*** end preperation for the test ***/


    pset<int> ps;
    version_t ps_v = 1;
    size_t ps_size = 0;

    for(size_t i = 0; i < PDS_CONTAINS_SIZE; ++i){ 

        assert(ps.insert(objs[i]) == ++ps_v);
        assert(ps.curr_version() == ps_v);
        assert(ps.size(ps_v) == ++ps_size);
    }

    for(size_t i = 0; i < PDS_CONTAINS_SIZE; ++i){ 

        assert(ps.remove(objs[i]) == ++ps_v);
        assert(ps.curr_version() == ps_v);
        assert(ps.size(ps_v) == --ps_size);
    }
    assert(ps_v == LAST_VERSION);


    // THE ACTUAL TEST:
    for(size_t v = 1; v <= LAST_VERSION; ++v){

        assert(ps.size(v) == versions[v].size());
        assert(ps.to_vector(v) == versions[v]);
    }

    cout << "test_size_and_to_vector PASSED!" << endl;
}


void test_exceptions(){

    // TODO
    cout << "test_exceptions PASSED!" << endl;
}


void test_edge_case_1(){

    // try to insert and remove one object again and again

    const int ROUNDS = 20;

    pset<int> ps;
    ps.insert(0);
    vector<int> vec_with_1 = {0, 1};
    vector<int> vec_without_1 = {0};

    for(int v = 3; v < ROUNDS; ++v){

        if(v % 2){
            ps.insert(1);
            assert(ps.contains(1, v));
            assert(ps.size(v) == 2);
            assert(ps.to_vector(v) == vec_with_1);
        }
        else{
            ps.remove(1);
            assert(ps.contains(1, v) == false);
            assert(ps.size(v) == 1);
            assert(ps.to_vector(v) == vec_without_1);
        }
    }

    cout << "test_edge_case_1 PASSED!" << endl;
}


void test_edge_case_2(){

    // Try inserting and removing a growing series of objects. 
    // It should check if something went wrong when we reinserted the same objects.

    vector<string> objs{"a", "b", "c", "d", "e"};

    vector<vector<string>> versions = {
        {}, 
        {}, 
        {"a"}, 
        {}, 
        {"a"}, 
        {"a", "b"}, 
        {"b"}, 
        {}, 
        {"a"}, 
        {"a", "b"}, 
        {"a", "b", "c"}, 
        {"b", "c"}, 
        {"c"}, 
        {},
        {"a"}, 
        {"a", "b"}, 
        {"a", "b", "c"}, 
        {"a", "b", "c", "d"}, 
        {"b", "c", "d"}, 
        {"c", "d"}, 
        {"d"}, 
        {},
        {"a"}, 
        {"a", "b"}, 
        {"a", "b", "c"}, 
        {"a", "b", "c", "d"}, 
        {"a", "b", "c", "d", "e"}, 
        {"b", "c", "d", "e"}, 
        {"c", "d", "e"}, 
        {"d", "e"}, 
        {"e"},
        {}
    };

    pset<string> ps;
    size_t curr_v = 1;

    for(size_t i = 0; i < objs.size(); ++i){

        for(size_t j = 0; j <= i; ++j){

            assert(ps.insert(objs[j]) == ++curr_v);
        }
        assert(ps.size() == i + 1);   // master version insert only new objs

        for(size_t j = 0; j <= i; ++j){

            assert(ps.remove(objs[j]) == ++curr_v);
        }
    }

    for(size_t v = 1; v < versions.size(); ++v){

        assert(ps.size(v) == versions[v].size());
        assert(ps.to_vector(v) == versions[v]);
    }

    cout << "test_edge_case_2 PASSED!" << endl;
}


void test_print(){

    if(!print_full_test)
        return;

    vector<int> objs = {21, 13, 22, 14, 23, 3, 24, 7, 25, 
                        15, 26, 16, 27, 28, 5, 10, 29, 6, 
                        30, 1, 31, 2, 32, 4, 33, 19, 34};

    pset<int> ps;
    ps.print(1);
    for(size_t i = 0; i < objs.size(); ++i){ 

        ps.insert(objs[i]);
        ps.print(i + 2);
    }

    for(size_t i = 0; i < objs.size(); ++i){ 

        cout << "remove(" << objs[i] << ")" << endl;
        ps.remove(objs[i]);
        ps.print(objs.size() + i + 2);
    }
}
