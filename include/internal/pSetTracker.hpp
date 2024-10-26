#ifndef PERSISTENT_SET_TRACKER_HPP
#define PERSISTENT_SET_TRACKER_HPP

#include "pFatNode.hpp"

namespace pds{

    template <class OBJ>
    class pSetTracker{

        pds::pFatNodePtr<OBJ>* ptr;
        pds::version_t track_version;

    public:
        pSetTracker(pds::pFatNodePtr<OBJ>& root);

        std::shared_ptr<pds::pFatNode<OBJ>>& operator[](const pds::version_t new_version);

        std::shared_ptr<pds::pFatNode<OBJ>> operator*() const;

        std::shared_ptr<pds::pFatNode<OBJ>> at(pds::version_t version) const;

        bool null() const;
        bool null_at(pds::version_t version) const;
        bool not_null() const;
        bool not_null_at(pds::version_t version) const;

        const OBJ& obj() const;
        const OBJ& obj_at(pds::version_t version) const;

        pSetTracker<OBJ>& left();
        pSetTracker<OBJ>& right();
        pSetTracker<OBJ>& left_at(pds::version_t version);
        pSetTracker<OBJ>& right_at(pds::version_t version);

        bool left_null() const;
        bool right_null() const;

        std::shared_ptr<pFatNode<OBJ>> get_left() const;
        std::shared_ptr<pFatNode<OBJ>> get_right() const;

        std::shared_ptr<pFatNode<OBJ>>& set_left(const pds::version_t new_version);
        std::shared_ptr<pFatNode<OBJ>>& set_right(const pds::version_t new_version);

        void set_track_version(const pds::version_t new_version);
    };
};

template <class OBJ>
pds::pSetTracker<OBJ>::pSetTracker(pds::pFatNodePtr<OBJ>& root) 
        
    : ptr(&root), track_version(ptr->nodes_versions.back()) {
}

template <class OBJ>
std::shared_ptr<pds::pFatNode<OBJ>>& pds::pSetTracker<OBJ>::operator[](const pds::version_t new_version){

    if(new_version != MasterVersion){
        ptr->nodes_versions.push_back(new_version);
    }
    return ptr->table[new_version];
}

template <class OBJ>
std::shared_ptr<pds::pFatNode<OBJ>> pds::pSetTracker<OBJ>::operator*() const {

    try{
        return ptr->table.at(track_version);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::operator*: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::pFatNode<OBJ>> pds::pSetTracker<OBJ>::at(pds::version_t version) const {

    version = ptr->map(version);

    try{
        return ptr->table.at(version);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::pSetTracker<OBJ>::null() const {
    
    try{
        return ptr->table.at(track_version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::pSetTracker<OBJ>::null_at(pds::version_t version) const {

    version = ptr->map(version);
    
    try{
        return ptr->table.at(version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::null_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::pSetTracker<OBJ>::not_null() const {

    try{
        return ptr->table.at(track_version) != nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::not_null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::pSetTracker<OBJ>::not_null_at(pds::version_t version) const {

    version = ptr->map(version);

    try{
        return ptr->table.at(version) != nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::not_null_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
const OBJ& pds::pSetTracker<OBJ>::obj() const {

    PDS_THROW_IF_NULL_TRACKER("obj", track_version);

    try{
        return ptr->table.at(track_version)->get_obj();
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::obj: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
const OBJ& pds::pSetTracker<OBJ>::obj_at(pds::version_t version) const {

    version = ptr->map(version);

    PDS_THROW_IF_NULL_TRACKER("obj_at", version);

    try{
        return ptr->table.at(version)->get_obj();
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::obj_at: Version " + std::to_string(version) + " is not exist"
        );
    }
}

template <class OBJ>
pds::pSetTracker<OBJ>& pds::pSetTracker<OBJ>::left(){

    PDS_THROW_IF_NULL_TRACKER("left", track_version);

    try{
        ptr = &(ptr->table.at(track_version)->left);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::left: Version " + std::to_string(track_version) + " is not exist"
        );
    }
    track_version = ptr->nodes_versions.back();
    return *this;
}

template <class OBJ>
pds::pSetTracker<OBJ>& pds::pSetTracker<OBJ>::right(){
    
    PDS_THROW_IF_NULL_TRACKER("right", track_version);

    try{
        ptr = &(ptr->table.at(track_version)->right);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::right: Version " + std::to_string(track_version) + " is not exist"
        );
    }
    track_version = ptr->nodes_versions.back();
    return *this;
}

template <class OBJ>
pds::pSetTracker<OBJ>& pds::pSetTracker<OBJ>::left_at(pds::version_t version){

    version = ptr->map(version);

    PDS_THROW_IF_NULL_TRACKER("left_at", version);

    try{
        ptr = &(ptr->table.at(version)->left);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::left_at: Version " + std::to_string(version) + " is not exist"
        );
    }
    return *this;
}

template <class OBJ>
pds::pSetTracker<OBJ>& pds::pSetTracker<OBJ>::right_at(pds::version_t version){

    version = ptr->map(version);
    
    PDS_THROW_IF_NULL_TRACKER("right_at", version);

    try{
        ptr = &(ptr->table.at(version)->right);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::right_at: Version " + std::to_string(version) + " is not exist"
        );
    }
    return *this;
}

template <class OBJ>
bool pds::pSetTracker<OBJ>::left_null() const {

    PDS_THROW_IF_NULL_TRACKER("left_null", track_version);
    
    try{
        pds::version_t left_track_version = ptr->table.at(track_version)->left.nodes_versions.back();
        return ptr->table.at(track_version)->left.table.at(left_track_version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::left_null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::pSetTracker<OBJ>::right_null() const {

    PDS_THROW_IF_NULL_TRACKER("right_null", track_version);

    try{
        pds::version_t right_track_version = ptr->table.at(track_version)->right.nodes_versions.back();
        return ptr->table.at(track_version)->right.table.at(right_track_version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::right_null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::pFatNode<OBJ>> pds::pSetTracker<OBJ>::get_left() const {

    PDS_THROW_IF_NULL_TRACKER("get_left", track_version);

    try{
        pds::version_t left_track_version = ptr->table.at(track_version)->left.nodes_versions.back();
        return ptr->table.at(track_version)->left.table.at(left_track_version);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::get_left: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::pFatNode<OBJ>> pds::pSetTracker<OBJ>::get_right() const {

    PDS_THROW_IF_NULL_TRACKER("get_right", track_version);

    try{
        pds::version_t right_track_version = ptr->table.at(track_version)->right.nodes_versions.back();
        return ptr->table.at(track_version)->right.table.at(right_track_version);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::get_right: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::pFatNode<OBJ>>& pds::pSetTracker<OBJ>::set_left(const pds::version_t new_version){

    PDS_THROW_IF_NULL_TRACKER("set_left", new_version);

    try{
        ptr->table.at(track_version)->left.nodes_versions.push_back(new_version);
        return ptr->table.at(track_version)->left.table[new_version];
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::set_left: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
std::shared_ptr<pds::pFatNode<OBJ>>& pds::pSetTracker<OBJ>::set_right(const pds::version_t new_version){

    PDS_THROW_IF_NULL_TRACKER("set_right", track_version);

    try{
        ptr->table.at(track_version)->right.nodes_versions.push_back(new_version);
        return ptr->table.at(track_version)->right.table[new_version];
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "pSetTracker::set_right: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
void pds::pSetTracker<OBJ>::set_track_version(const pds::version_t new_version){

    PDS_THROW_IF_NULL_TRACKER("set_track_version", new_version);

    track_version = new_version;
}

#endif /* PERSISTENT_SET_TRACKER_HPP */