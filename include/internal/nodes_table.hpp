#ifndef PERSISTENT_DATA_STRUCTURE_NODES_TABLE_HPP
#define PERSISTENT_DATA_STRUCTURE_NODES_TABLE_HPP

#include "utils.hpp"
#include "excep.hpp"

namespace pds{
    
    template <class FN>
    class nodes_table{

        std::unordered_map<pds::version_t, std::shared_ptr<FN>> table;

        std::vector<std::pair<pds::version_t, pds::version_t>> cow_stack;

    public:
        nodes_table(const pds::version_t create_version)
        
            : table{{create_version, nullptr}} {
        }

        // performing an insertion if such key does not already exist.
        std::shared_ptr<FN>& operator[](const pds::version_t key){

            return table[key];
        }

        // return copy of val. excep if key out of range
        std::shared_ptr<FN>& at(const pds::version_t key){

            try{
                return table.at(key);
            }
            catch(const std::out_of_range& e){

                throw pds::VersionOutOfRange(
                    "nodes_table::at: std::out_of_range::what(): " + std::string(e.what())
                );
            }
        }

        void copy_map(const pds::version_t key){

            for(auto& new_map : cow_stack){

                table[new_map.first] = table[new_map.second];
            }

            if(table.at(key) != nullptr){

                table.at(key)->left.extend_stack(cow_stack);
                table.at(key)->right.extend_stack(cow_stack);
            }

            cow_stack.clear();
        }

        void extend_stack(const std::vector<std::pair<pds::version_t, pds::version_t>>& to_push){

            cow_stack.insert(cow_stack.end(), to_push.begin(), to_push.end());
        }    
    };
};

#endif /* PERSISTENT_DATA_STRUCTURE_NODES_TABLE_HPP */