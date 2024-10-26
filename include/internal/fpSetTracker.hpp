#ifndef FULLY_PERSISTENT_SET_TRACKER_HPP
#define FULLY_PERSISTENT_SET_TRACKER_HPP

#include "fpFatNode.hpp"

namespace pds{

    template <class OBJ>
    class fpSetTracker{

        pds::fpFatNodePtr<OBJ>* ptr;
        pds::version_t track_version;

    public:
        fpSetTracker(pds::fpFatNodePtr<OBJ>& root, pds::version_t version);

        std::shared_ptr<pds::fpFatNode<OBJ>>& operator[](const pds::version_t new_version);

        std::shared_ptr<pds::fpFatNode<OBJ>> operator*() const;

        const OBJ& obj() const;

        fpSetTracker<OBJ>& left();
        fpSetTracker<OBJ>& right();

        void add_left_map(const pds::version_t new_version);
        void add_right_map(const pds::version_t new_version);

        std::shared_ptr<fpFatNode<OBJ>> get_left() const;
        std::shared_ptr<fpFatNode<OBJ>> get_right() const;

        std::shared_ptr<fpFatNode<OBJ>>& set_left_at(const pds::version_t new_version);
        std::shared_ptr<fpFatNode<OBJ>>& set_right_at(const pds::version_t new_version);

        bool null() const;
        bool not_null() const;

        bool left_null() const;
        bool left_not_null() const;
        bool right_null() const;
        bool right_not_null() const;
    };
};

template <class OBJ>
pds::fpSetTracker<OBJ>::fpSetTracker(pds::fpFatNodePtr<OBJ>& root, pds::version_t version) 
        
    : ptr(&root), track_version(version) {
}

template <class OBJ>
std::shared_ptr<pds::fpFatNode<OBJ>>& pds::fpSetTracker<OBJ>::operator[](const pds::version_t new_version){

    if(new_version != MasterVersion){

        ptr->versions_map.add(new_version);
    }
    return ptr->table[new_version];
}

template <class OBJ>
std::shared_ptr<pds::fpFatNode<OBJ>> pds::fpSetTracker<OBJ>::operator*() const {

    try{
        return ptr->table.at(track_version);
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fpSetTracker::operator*: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fpSetTracker<OBJ>::null() const {
    
    try{
        return ptr->table.at(track_version) == nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fpSetTracker::null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
bool pds::fpSetTracker<OBJ>::not_null() const {

    try{
        return ptr->table.at(track_version) != nullptr;
    }
    catch(const std::out_of_range&){

        throw pds::VersionNotExist(
            "fpSetTracker::not_null: Version " + std::to_string(track_version) + " is not exist"
        );
    }
}

template <class OBJ>
const OBJ& pds::fpSetTracker<OBJ>::obj() const {

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("obj", track_version);

    return ptr->table.at(track_version)->get_obj();
}

template <class OBJ>
void pds::fpSetTracker<OBJ>::add_left_map(const pds::version_t new_version){

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("add_left_map", new_version);

    ptr->table.at(new_version)->left.versions_map.add(new_version);
    ptr->table.at(new_version)->left.versions_map.Union(new_version, track_version);
}

template <class OBJ>
void pds::fpSetTracker<OBJ>::add_right_map(const pds::version_t new_version){

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("add_right_map", new_version);
    
    ptr->table.at(new_version)->right.versions_map.add(new_version);
    ptr->table.at(new_version)->right.versions_map.Union(new_version, track_version);
}

template <class OBJ>
pds::fpSetTracker<OBJ>& pds::fpSetTracker<OBJ>::left(){

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("left", track_version);
    
    ptr = &(ptr->table.at(track_version)->left);
    track_version = ptr->map(track_version);
    return *this;
}

template <class OBJ>
pds::fpSetTracker<OBJ>& pds::fpSetTracker<OBJ>::right(){

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("right", track_version);
    
    ptr = &(ptr->table.at(track_version)->right);
    track_version = ptr->map(track_version);
    return *this;
}

template <class OBJ>
bool pds::fpSetTracker<OBJ>::left_null() const {

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("left_null", track_version);

    pds::fpFatNodePtr<OBJ>* left_ptr = &(ptr->table.at(track_version)->left);

    return left_ptr->table.at(left_ptr->map(track_version)) == nullptr;
}

template <class OBJ>
bool pds::fpSetTracker<OBJ>::right_null() const {

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("right_null", track_version);

    pds::fpFatNodePtr<OBJ>* right_ptr = &(ptr->table.at(track_version)->right);

    return right_ptr->table.at(right_ptr->map(track_version)) == nullptr;
}

template <class OBJ>
bool pds::fpSetTracker<OBJ>::left_not_null() const {

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("left_not_null", track_version);

    pds::fpFatNodePtr<OBJ>* left_ptr = &(ptr->table.at(track_version)->left);

    return left_ptr->table.at(left_ptr->map(track_version)) != nullptr;
}

template <class OBJ>
bool pds::fpSetTracker<OBJ>::right_not_null() const {

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("right_not_null", track_version);

    pds::fpFatNodePtr<OBJ>* right_ptr = &(ptr->table.at(track_version)->right);

    return right_ptr->table.at(right_ptr->map(track_version)) != nullptr;
}

template <class OBJ>
std::shared_ptr<pds::fpFatNode<OBJ>> pds::fpSetTracker<OBJ>::get_left() const {

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("get_left", track_version);

    pds::fpFatNodePtr<OBJ>* left_ptr = &(ptr->table.at(track_version)->left);
    return left_ptr->table.at(left_ptr->map(track_version));
}

template <class OBJ>
std::shared_ptr<pds::fpFatNode<OBJ>> pds::fpSetTracker<OBJ>::get_right() const {

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("get_right", track_version);

    pds::fpFatNodePtr<OBJ>* right_ptr = &(ptr->table.at(track_version)->right);
    return right_ptr->table.at(right_ptr->map(track_version));
}

template <class OBJ>
std::shared_ptr<pds::fpFatNode<OBJ>>& pds::fpSetTracker<OBJ>::set_left_at(const pds::version_t new_version){

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("set_left_at", new_version);

    pds::fpFatNodePtr<OBJ>* left_ptr = &(ptr->table.at(new_version)->left);
    left_ptr->versions_map.add(new_version);
    return left_ptr->table[new_version];
}

template <class OBJ>
std::shared_ptr<pds::fpFatNode<OBJ>>& pds::fpSetTracker<OBJ>::set_right_at(const pds::version_t new_version){

    PDS_THROW_IF_TRACK_VERSION_NOT_EXIST("set_right_at", new_version);

    pds::fpFatNodePtr<OBJ>* right_ptr = &(ptr->table.at(new_version)->right);
    right_ptr->versions_map.add(new_version);
    return right_ptr->table[new_version];
}

#endif /* FULLY_PERSISTENT_SET_TRACKER_HPP */