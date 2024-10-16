#include "pds_test.hpp"


int main(int argc, char* argv[]){

    bool run_all = (argc < 2) || std::strcmp(argv[1], "all") == 0;

    for(int i = 0; i < PDS_TESTS_NUM; ++i){

        if(run_all || argv[1] == pds::tests_name[i]){

            try{
                pds::TestFuncArr[i]();
            }
            catch(const pds::pdsExcept& e){

                std::cout << std::endl << "pdsExcept: " << std::string(e.what());
                exit(1);
            }
            catch(const std::exception& e){

                std::cout << std::endl << "std::exception: " << std::string(e.what());
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
