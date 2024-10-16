#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_TRACKER_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_TRACKER_HPP

#include "fat_node.hpp"

namespace pds{

    template <class OBJ>
    class fat_node_tracker{

        pds::nodes_table<fat_node<OBJ>>* ptr;

    public:
        fat_node_tracker() : ptr(nullptr){
        }

        explicit fat_node_tracker(pds::nodes_table<fat_node<OBJ>>& root) 

            : ptr(&root){
        }

        fat_node_tracker(const fat_node_tracker&) = default;
        fat_node_tracker(fat_node_tracker&&) = default;
        fat_node_tracker& operator=(const fat_node_tracker&) = default;
        fat_node_tracker& operator=(fat_node_tracker&&) = default;

        fat_node_tracker& operator=(const pds::nodes_table<fat_node<OBJ>>& ref){

            ptr = &ref;
        }


        std::shared_ptr<fat_node<OBJ>>& operator[](const pds::version_t key){

            // maybe: add_new_version();

            return ptr->table[key];
        }

        const std::shared_ptr<fat_node<OBJ>>& at(const pds::version_t key) const {

            try{
                return ptr->table.at(key);
            }
            catch(const std::out_of_range& e){

                throw pds::VersionOutOfRange(
                    "nodes_table::at: std::out_of_range::what(): " + std::string(e.what())
                );
            }
        }

        bool null_at(const pds::version_t key) const {

            return ptr->table.at(key) == nullptr;
        }
        
        bool not_null_at(const pds::version_t key) const {

            return ptr->table.at(key) != nullptr; 
        }

        const OBJ& obj_at(const pds::version_t key) const {

            if(ptr->table.at(key) == nullptr)
                throw pds::NullTracker(
                    "obj_at: table.at(" + std::to_string(key) + ") is nullptr"
                );

            return this->at(key)->get_obj();
        }

        fat_node_tracker<OBJ>& left_at(const pds::version_t key){

            if(ptr->table.at(key) == nullptr)
                throw pds::NullTracker(
                    "left_at: table.at(" + std::to_string(key) + ") is nullptr"
                );

            try{
                ptr = &(ptr->table.at(key)->left);
            }
            catch(const std::out_of_range& e){

                throw pds::VersionOutOfRange(
                    "nodes_table::left_at: std::out_of_range::what(): " + std::string(e.what())
                );
            }
            return *this;
        }

        fat_node_tracker<OBJ>& right_at(const pds::version_t key){

            if(ptr->table.at(key) == nullptr)
                throw pds::NullTracker(
                    "right_at: table.at(" + std::to_string(key) + ") is nullptr"
                );

            try{
                ptr = &(ptr->table.at(key)->right);
            }
            catch(const std::out_of_range& e){

                throw pds::VersionOutOfRange(
                    "nodes_table::right_at: std::out_of_range::what(): " + std::string(e.what())
                );
            }
            return *this;
        }

        std::shared_ptr<fat_node<OBJ>>& ref_left_at(const pds::version_t key){

            return this->left_at(key).operator[](key);
        }

        std::shared_ptr<fat_node<OBJ>>& ref_right_at(const pds::version_t key){

            return this->right_at(key).operator[](key);
        }

        void copy_map(const pds::version_t key){

            for(auto& new_map : ptr->cow_stack){

                ptr->table[new_map.first] = ptr->table[new_map.second];
            }

            if(ptr->table.at(key) != nullptr){

                ptr->table.at(key)->left.extend_stack(ptr->cow_stack);
                ptr->table.at(key)->right.extend_stack(ptr->cow_stack);
            }

            ptr->cow_stack.clear();
        }

    };
};

#endif /* PERSISTENT_DATA_STRUCTURE_FAT_NODE_TRACKER_HPP */