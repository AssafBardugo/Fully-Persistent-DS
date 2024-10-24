#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_TRACKER_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_TRACKER_HPP

#include "FatNode.hpp"

namespace pds{

    template <class OBJ>
    class fat_node_tracker{

        pds::fat_node_ptr<OBJ>* ptr;
        pds::version_t track_version;

    public:
        fat_node_tracker(pds::fat_node_ptr<OBJ>& root);

        std::shared_ptr<pds::fat_node<OBJ>>& operator[](const pds::version_t new_version);

        std::shared_ptr<pds::fat_node<OBJ>> operator*() const;

        std::shared_ptr<pds::fat_node<OBJ>> at(pds::version_t version) const;

        bool null() const;
        bool null_at(pds::version_t version) const;
        bool not_null() const;
        bool not_null_at(pds::version_t version) const;

        const OBJ& obj() const;
        const OBJ& obj_at(pds::version_t version) const;

        fat_node_tracker<OBJ>& left();
        fat_node_tracker<OBJ>& right();
        fat_node_tracker<OBJ>& left_at(pds::version_t version);
        fat_node_tracker<OBJ>& right_at(pds::version_t version);

        bool left_null() const;
        bool right_null() const;
        bool left_null_at(pds::version_t version) const;
        bool right_null_at(pds::version_t version) const;

        bool left_not_null() const;
        bool right_not_null() const;
        bool left_not_null_at(pds::version_t version) const;
        bool right_not_null_at(pds::version_t version) const;

        std::shared_ptr<fat_node<OBJ>> get_left() const;
        std::shared_ptr<fat_node<OBJ>> get_right() const;

        std::shared_ptr<fat_node<OBJ>>& set_left_at(const pds::version_t new_version);
        std::shared_ptr<fat_node<OBJ>>& set_right_at(const pds::version_t new_version);
    };
};

template <class OBJ>
pds::fat_node_tracker<OBJ>::fat_node_tracker(pds::fat_node_ptr<OBJ>& root) 
        
    : ptr(&root), track_version(ptr->nodes_versions.back()) {
}

template <class OBJ>
std::shared_ptr<pds::fat_node<OBJ>>& pds::fat_node_tracker<OBJ>::operator[](const pds::version_t new_version){

    if(new_version != MasterVersion){
        ptr->nodes_versions.push_back(new_version);
    }
    return ptr->table[new_version];
}

template <class OBJ>
std::shared_ptr<pds::fat_node<OBJ>> pds::fat_node_tracker<OBJ>::operator*() const {

    try{
        return ptr->table.at(track_version);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::operator*: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::fat_node<OBJ>> pds::fat_node_tracker<OBJ>::at(pds::version_t version) const {

    version = ptr->map(version);

    try{
        return ptr->table.at(version);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::null() const {
    
    try{
        return ptr->table.at(track_version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::null_at(pds::version_t version) const {

    version = ptr->map(version);
    
    try{
        return ptr->table.at(version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::null_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::not_null() const {

    try{
        return ptr->table.at(track_version) != nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::not_null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::not_null_at(pds::version_t version) const {

    version = ptr->map(version);

    try{
        return ptr->table.at(version) != nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::not_null_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
const OBJ& pds::fat_node_tracker<OBJ>::obj() const {

    PDS_THROW_IF_NULL_TRACKER("obj", track_version);

    try{
        return ptr->table.at(track_version)->get_obj();
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::obj: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
const OBJ& pds::fat_node_tracker<OBJ>::obj_at(pds::version_t version) const {

    version = ptr->map(version);

    PDS_THROW_IF_NULL_TRACKER("obj_at", version);

    try{
        return ptr->table.at(version)->get_obj();
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::obj_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
pds::fat_node_tracker<OBJ>& pds::fat_node_tracker<OBJ>::left(){

    PDS_THROW_IF_NULL_TRACKER("left", track_version);

    try{
        ptr = &(ptr->table.at(track_version)->left);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::left: Version " + std::to_string(track_version) + " is not exist"
        );
    }
    track_version = ptr->nodes_versions.back();
    return *this;
}

template <class OBJ>
pds::fat_node_tracker<OBJ>& pds::fat_node_tracker<OBJ>::right(){
    
    PDS_THROW_IF_NULL_TRACKER("right", track_version);

    try{
        ptr = &(ptr->table.at(track_version)->right);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::right: Version " + std::to_string(track_version) + " is not exist"
        );
    }
    track_version = ptr->nodes_versions.back();
    return *this;
}

template <class OBJ>
pds::fat_node_tracker<OBJ>& pds::fat_node_tracker<OBJ>::left_at(pds::version_t version){

    version = ptr->map(version);

    PDS_THROW_IF_NULL_TRACKER("left_at", version);

    try{
        ptr = &(ptr->table.at(version)->left);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::left_at: Version " + std::to_string(version) + " is not exist"
        );
    }
    return *this;
}

template <class OBJ>
pds::fat_node_tracker<OBJ>& pds::fat_node_tracker<OBJ>::right_at(pds::version_t version){

    version = ptr->map(version);
    
    PDS_THROW_IF_NULL_TRACKER("right_at", version);

    try{
        ptr = &(ptr->table.at(version)->right);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::right_at: Version " + std::to_string(version) + " is not exist"
        );
    }
    return *this;
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::left_null() const {

    PDS_THROW_IF_NULL_TRACKER("left_null", track_version);
    
    try{
        pds::version_t left_track_version = ptr->table.at(track_version)->left.nodes_versions.back();
        return ptr->table.at(track_version)->left.table.at(left_track_version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::left_null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::right_null() const {

    PDS_THROW_IF_NULL_TRACKER("right_null", track_version);

    try{
        pds::version_t right_track_version = ptr->table.at(track_version)->right.nodes_versions.back();
        return ptr->table.at(track_version)->right.table.at(right_track_version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::right_null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::left_null_at(pds::version_t version) const {

    version = ptr->map(version);

    PDS_THROW_IF_NULL_TRACKER("left_null_at", version);
    
    try{
        pds::version_t left_version = ptr->table.at(version)->left.nodes_versions.back();
        return ptr->table.at(version)->left.table.at(left_version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::left_null_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::right_null_at(pds::version_t version) const {

    version = ptr->map(version);

    PDS_THROW_IF_NULL_TRACKER("right_null_at", version);

    try{
        pds::version_t right_version = ptr->table.at(version)->right.nodes_versions.back();
        return ptr->table.at(version)->right.table.at(right_version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::right_null_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::left_not_null() const {

    PDS_THROW_IF_NULL_TRACKER("left_not_null", track_version);
    
    try{
        pds::version_t left_track_version = ptr->table.at(track_version)->left.nodes_versions.back();
        return ptr->table.at(track_version)->left.table.at(left_track_version) != nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::left_not_null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::right_not_null() const {

    PDS_THROW_IF_NULL_TRACKER("right_not_null", track_version);

    try{
        pds::version_t right_track_version = ptr->table.at(track_version)->right.nodes_versions.back();
        return ptr->table.at(track_version)->right.table.at(right_track_version) != nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::right_not_null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::left_not_null_at(pds::version_t version) const {

    version = ptr->map(version);

    PDS_THROW_IF_NULL_TRACKER("left_not_null_at", version);
    
    try{
        pds::version_t left_version = ptr->table.at(version)->left.nodes_versions.back();
        return ptr->table.at(version)->left.table.at(left_version) != nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::left_not_null_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fat_node_tracker<OBJ>::right_not_null_at(pds::version_t version) const {

    version = ptr->map(version);

    PDS_THROW_IF_NULL_TRACKER("right_not_null_at", version);

    try{
        pds::version_t right_version = ptr->table.at(version)->right.nodes_versions.back();
        return ptr->table.at(version)->right.table.at(right_version) != nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::right_not_null_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::fat_node<OBJ>> pds::fat_node_tracker<OBJ>::get_left() const {

    PDS_THROW_IF_NULL_TRACKER("get_left", track_version);

    try{
        pds::version_t left_track_version = ptr->table.at(track_version)->left.nodes_versions.back();
        return ptr->table.at(track_version)->left.table.at(left_track_version);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::get_left: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::fat_node<OBJ>> pds::fat_node_tracker<OBJ>::get_right() const {

    PDS_THROW_IF_NULL_TRACKER("get_right", track_version);

    try{
        pds::version_t right_track_version = ptr->table.at(track_version)->right.nodes_versions.back();
        return ptr->table.at(track_version)->right.table.at(right_track_version);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::get_right: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::fat_node<OBJ>>& pds::fat_node_tracker<OBJ>::set_left_at(const pds::version_t new_version){

    PDS_THROW_IF_NULL_TRACKER("set_left", new_version);

    try{
        ptr->table.at(new_version)->left.nodes_versions.push_back(new_version);
        return ptr->table.at(new_version)->left.table[new_version];
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::set_left: Version " + std::to_string(new_version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::fat_node<OBJ>>& pds::fat_node_tracker<OBJ>::set_right_at(const pds::version_t new_version){

    PDS_THROW_IF_NULL_TRACKER("set_right", new_version);

    try{
        ptr->table.at(new_version)->right.nodes_versions.push_back(new_version);
        return ptr->table.at(new_version)->right.table[new_version];
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fat_node_tracker::set_right: Version " + std::to_string(new_version) + " is not exist"
        );
    }
}

#endif /* PERSISTENT_DATA_STRUCTURE_FAT_NODE_TRACKER_HPP */