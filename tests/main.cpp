#include "pds_test.hpp"


int main(int argc, char* argv[]){

    bool run_all = (argc < 2) || std::strcmp(argv[1], "all") == 0;

    for(int i = 0; i < PDS_TESTS_NUM; ++i){

        if(run_all || argv[1] == pds::tests_name[i]){

            pds::TestFuncArr[i]();
        }
    }

    return 0;
}
