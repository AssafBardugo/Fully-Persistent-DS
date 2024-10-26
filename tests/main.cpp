#include "pds_test.h"


int main(int argc, char* argv[]){

    bool run_all = (argc < 2) || std::strcmp(argv[1], "all") == 0;

    for(int i = 0; i < PDS_TESTS_NUM; ++i){

        if(run_all || argv[1] == pds::tests_name[i]){

            try{
                pds::TestFuncArr[i]();
            }
            catch(const pds::pdsExcept& e){

                std::cout << e.what() << std::endl;
                exit(1);
            }
            catch(...){

                std::cout << "\n\nUNKNOWN EXCEPTION WAS THROWN!!!\n\n";
                exit(1);
            }
        }
    }
    return 0;
}
