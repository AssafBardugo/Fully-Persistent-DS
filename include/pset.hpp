#ifndef PARTIALLY_PERSISTENT_SET_HPP
#define PARTIALLY_PERSISTENT_SET_HPP

#include "internal/pSetTracker.hpp"

namespace pds{

    template <class OBJ>
    class pSet{

        pds::pFatNodePtr<OBJ> root;
        pds::version_t last_version;
        std::vector<pds::version_t> sizes;

    public:
        pSet();

        pds::version_t insert(const OBJ& obj);
        pds::version_t insert(OBJ&& obj);

        pds::version_t remove(const OBJ& obj);
        pds::version_t remove(OBJ&& obj);

        bool contains(const OBJ& obj, pds::version_t version = MasterVersion);
    
        std::vector<OBJ> to_vector(const pds::version_t version = MasterVersion);

        std::size_t size(pds::version_t version = MasterVersion) const;

        pds::version_t curr_version() const;

        void print(pds::version_t version = MasterVersion);

    private:
        template <typename T>
        pds::version_t insert_impl(T&& obj);
    };
};


template <class OBJ>
pds::pSet<OBJ>::pSet() : root(1), last_version(1), sizes{0, 0} {
}

template <class OBJ>
pds::version_t pds::pSet<OBJ>::insert(const OBJ& obj){

    return insert_impl(obj);
}

template <class OBJ>
pds::version_t pds::pSet<OBJ>::insert(OBJ&& obj){

    return insert_impl(std::move(obj));
}

template <class OBJ>
template <typename T>
pds::version_t pds::pSet<OBJ>::insert_impl(T&& obj){

    pds::pSetTracker<OBJ> tracker(root);

    while(tracker.not_null()){

        if(obj < tracker.obj()){

            tracker = tracker.left();
        }
        else if(tracker.obj() < obj){

            tracker = tracker.right();
        }
        else{
            throw pds::ObjectAlreadyExist(
                "pSet::insert: Version " + std::to_string(last_version) + " already contains this object"
            );
        }
    }

    pds::version_t new_version = last_version + 1;

    pds::pSetTracker<OBJ> track_master(root);

    while(track_master.not_null_at(MasterVersion)){

        if(obj < track_master.obj_at(MasterVersion)){

            track_master = track_master.left_at(MasterVersion);
        }
        else if(track_master.obj_at(MasterVersion) < obj){

            track_master = track_master.right_at(MasterVersion);
        }
        else{
            tracker[new_version] = track_master.at(MasterVersion);
            break;
        }
    }

    if(track_master.null_at(MasterVersion)){

        track_master[MasterVersion] = std::make_shared<pds::pFatNode<OBJ>>(std::forward<T>(obj), new_version);

        ++sizes[MasterVersion];
        tracker[new_version] = track_master.at(MasterVersion);
    }
    else{
        if(tracker.left_not_null()){

            tracker.set_left_at(new_version) = nullptr;
        }
        if(tracker.right_not_null()){

            tracker.set_right_at(new_version) = nullptr;
        }
    }

    sizes.push_back(sizes[last_version] + 1);
    
    return (last_version = new_version);
}

template <class OBJ>
pds::version_t pds::pSet<OBJ>::remove(OBJ&& obj){

    return remove(std::as_const(obj));
}

template <class OBJ>
pds::version_t pds::pSet<OBJ>::remove(const OBJ& obj){

    pds::pSetTracker<OBJ> tracker(root);

    while(tracker.not_null()){

        if(obj < tracker.obj()){

            tracker = tracker.left();
        }
        else if(tracker.obj() < obj){

            tracker = tracker.right();
        }
        else break;
    }

    if(tracker.null())
        throw pds::ObjectNotExist(
            "pds::pSet::remove: Attempting to remove an object but the object is not exists" 
        );

    pds::version_t new_version = last_version + 1;
    pds::pSetTracker<OBJ> to_remove = tracker;

    if(to_remove.left_null()){

        to_remove[new_version] = tracker.get_right();
    }
    else if(to_remove.right_null()){

        to_remove[new_version] = tracker.get_left();
    }
    else{
        pds::pSetTracker<OBJ> track_to_leaf = tracker;
        track_to_leaf = track_to_leaf.right();

        if(track_to_leaf.left_null()){

            to_remove[new_version] = *track_to_leaf;
            to_remove.set_left_at(new_version) = tracker.get_left();
        }
        else{
            while(!track_to_leaf.left_null()){

                track_to_leaf = track_to_leaf.left();
            }
            track_to_leaf[new_version] = track_to_leaf.get_right();

            to_remove[new_version] = *track_to_leaf;
            to_remove.set_left_at(new_version) = tracker.get_left();
            to_remove.set_right_at(new_version) = tracker.get_right();
        }
    }
    // push the size of the new version
    sizes.push_back(sizes[last_version] - 1);

    return (last_version = new_version);
}

template <class OBJ>
bool pds::pSet<OBJ>::contains(const OBJ& obj, pds::version_t version) {

    PDS_THROW_IF_VERSION_NOT_EXIST("pSet::contains", version, last_version);

    pds::pSetTracker<OBJ> tracker(root);

    while(tracker.not_null_at(version)){

        if(obj < tracker.obj_at(version)){

            tracker = tracker.left_at(version);
        }
        else if(tracker.obj_at(version) < obj){

            tracker = tracker.right_at(version);
        }
        else return true;
    }
    return false; 
}

template <class OBJ>
std::vector<OBJ> pds::pSet<OBJ>::to_vector(const pds::version_t version) {

    PDS_THROW_IF_VERSION_NOT_EXIST("pSet::to_vector", version, last_version);

    std::vector<OBJ> obj_vec;

    /*** Inorder Stack Traversal ***/
    std::stack<pds::pSetTracker<OBJ>> trav_stack;
    pds::pSetTracker<OBJ> tracker(root);

    while(tracker.not_null_at(version) || !trav_stack.empty()){

        while(tracker.not_null_at(version)){

            trav_stack.push(tracker);
            tracker = tracker.left_at(version);
        }
        tracker = trav_stack.top();
        trav_stack.pop();

        obj_vec.push_back(tracker.obj_at(version));
        tracker = tracker.right_at(version);
    }
    return obj_vec;
}

template <class OBJ>
std::size_t pds::pSet<OBJ>::size(pds::version_t version) const { 
    
    try{
        return sizes.at(version);
    }
    catch(const std::out_of_range&){

        return 0;
    }
}

template <class OBJ>
pds::version_t pds::pSet<OBJ>::curr_version() const {

    return last_version;
}

template <class OBJ>
void pds::pSet<OBJ>::print(pds::version_t version){

    PDS_THROW_IF_VERSION_NOT_EXIST("pSet::print", version, last_version);

    std::vector<OBJ> vec = to_vector(version);

    std::cout << "Version " << version << ": {";
    if(!vec.empty()){
        std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<OBJ>(std::cout, ", "));
        std::cout << vec.back();
    }
    std::cout << "}" << std::endl;
}


#endif /* PARTIALLY_PERSISTENT_SET_HPP */