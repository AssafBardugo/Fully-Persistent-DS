#include "pds_test.hpp"

using namespace pds;


int main(int argc, char* argv[]){

    bool run_all = (argc < 2) || argv[1] == "all";

    for(int i = 0; i < PDS_TESTS_NUM; ++i){

        if(run_all || argv[1] == tests_name[i]){

            tests_func[i]();
        }
    }

    return 0;
}
