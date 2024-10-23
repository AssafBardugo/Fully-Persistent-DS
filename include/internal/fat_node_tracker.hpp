#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_TRACKER_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_TRACKER_HPP

#include "fat_node.hpp"

namespace pds{

    template <class OBJ>
    class fat_node_tracker{

        pds::fat_node_ptr<OBJ>* ptr;
        pds::version_t track_version;

        /** for fpset support **/
        // pds::version_t prev_key;
        /**/

    public:
        fat_node_tracker(pds::fat_node_ptr<OBJ>& root, pds::version_t version) 

            : ptr(&root), track_version(version) {
        }

        fat_node_tracker(const fat_node_tracker&) = default;
        fat_node_tracker(fat_node_tracker&&) = default;
        fat_node_tracker& operator=(const fat_node_tracker&) = default;
        fat_node_tracker& operator=(fat_node_tracker&&) = default;


        std::shared_ptr<pds::fat_node<OBJ>>& operator[](const pds::version_t new_version){

            ptr->nodes_versions.push_back(new_version);
            return ptr->table[new_version];
        }

        std::shared_ptr<fat_node<OBJ>> operator*() const {

            return ptr->table.at(track_version);
        }

        bool null() const {

            return ptr->table.at(track_version) == nullptr;
        }

        bool not_null() const {

            return ptr->table.at(track_version) != nullptr;
        }

        const std::shared_ptr<OBJ> obj() const {

            if(this->null())
                throw pds::NullTracker(
                    "obj: table.at(" + std::to_string(track_version) + ") is nullptr"
                );

            return ptr->table.at(track_version)->obj_ptr;
        }

        fat_node_tracker<OBJ>& left(){

            if(this->null())
                throw pds::NullTracker(
                    "left: table.at(" + std::to_string(track_version) + ") is nullptr"
                );

            try{
                ptr = &(ptr->table.at(track_version)->left);
            }
            catch(const std::out_of_range& e){

                throw pds::VersionOutOfRange(
                    "fat_node_tracker::left: std::out_of_range::what(): " + std::string(e.what())
                );
            }
            track_version = ptr->nodes_versions.back();
            return *this;
        }

        fat_node_tracker<OBJ>& right(){
            
            if(this->null())
                throw pds::NullTracker(
                    "right: table.at(" + std::to_string(track_version) + ") is nullptr"
                );

            try{
                ptr = &(ptr->table.at(track_version)->right);
            }
            catch(const std::out_of_range& e){

                throw pds::VersionOutOfRange(
                    "fat_node_tracker::right: std::out_of_range::what(): " + std::string(e.what())
                );
            }
            track_version = ptr->nodes_versions.back();
            return *this;
        }

        bool left_null(){

            if(this->null())
                throw pds::NullTracker(
                    "left_null: table.at(" + std::to_string(track_version) + ") is nullptr"
                );
            
            return ptr->table.at(track_version)->left->table.at(track_version) == nullptr;
        }

        bool right_null(){

            if(this->null())
                throw pds::NullTracker(
                    "right_null: table.at(" + std::to_string(track_version) + ") is nullptr"
                );
            
            return ptr->table.at(track_version)->right->table.at(track_version) == nullptr;
        }

        std::shared_ptr<fat_node<OBJ>> get_left(){

            if(this->null())
                throw pds::NullTracker(
                    "copy_left: table.at(" + std::to_string(track_version) + ") is nullptr"
                );

            return ptr->table.at(track_version)->left->table.at(track_version);
        }

        std::shared_ptr<fat_node<OBJ>> get_right(){

            if(this->null())
                throw pds::NullTracker(
                    "copy_right: table.at(" + std::to_string(track_version) + ") is nullptr"
                );

            return ptr->table.at(track_version)->right->table.at(track_version);
        }

        std::shared_ptr<fat_node<OBJ>>& set_left(const pds::version_t new_version){

            if(this->null())
                throw pds::NullTracker(
                    "set_left: table.at(" + std::to_string(track_version) + ") is nullptr"
                );

            return ptr->table.at(track_version)->left->table[new_version];
        }

        std::shared_ptr<fat_node<OBJ>>& set_right(const pds::version_t new_version){

            if(this->null())
                throw pds::NullTracker(
                    "set_right: table.at(" + std::to_string(track_version) + ") is nullptr"
                );

            return ptr->table.at(track_version)->right->table[new_version];
        }



        const std::shared_ptr<fat_node<OBJ>>& at(const pds::version_t key) const {

            try{
                return ptr->table.at(track_version);
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

            /** for fpset support **/
            // if(ptr->table.contains(track_key) == false){

            //     prev_key = ptr->table.lower_bound(prev_key);
            // }
            /**/

            try{
                ptr = &(ptr->table.at(key)->left);
            }
            catch(const std::out_of_range& e){

                throw pds::VersionOutOfRange(
                    "nodes_table::left_at: std::out_of_range::what(): " + std::string(e.what())
                );
            }
            track_version = ptr->map(track_version);
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