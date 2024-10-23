#ifndef PARTIALLY_PERSISTENT_SET_HPP
#define PARTIALLY_PERSISTENT_SET_HPP

#include "internal/fat_node_tracker.hpp"

namespace pds{

    const version_t master_version = 0;

    template <class OBJ>
    class pset{

        pds::fat_node_ptr<OBJ> root;
        pds::version_t last_version;
        std::set<std::shared_ptr<OBJ>> v_master;
        std::vector<pds::version_t> sizes;

    public:
        pset() : last_version(1), sizes{0, 0} {
        }

        pds::version_t insert(const OBJ& obj){
            return insert_impl(obj);
        }

        pds::version_t insert(OBJ&& obj){
            return insert_impl(std::move(obj));
        }

        pds::version_t remove(const OBJ& obj){

            pds::fat_node_tracker<OBJ> tracker(root, last_version);

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
                    "pds::pset::remove: Attempting to remove an object but the object is not exists" 
                );

            pds::version_t new_version = last_version + 1;

            pds::fat_node_tracker<OBJ> to_remove = tracker;

            if(to_remove.left_null()){

                to_remove[new_version] = to_remove.copy_right();
            }
            else if(to_remove.right_null()){

                to_remove[new_version] = to_remove.copy_left();
            }
            else{
                tracker = tracker.right();

                while(!tracker.left_null()){

                    tracker = tracker.left();
                }
                tracker[new_version] = nullptr;

                to_remove[new_version] = *tracker;
                to_remove.set_left(new_version) = tracker.get_left();
                to_remove.set_right(new_version) = tracker.get_right();
            }

            // push the size of the new version
            sizes.push_back(sizes[last_version] - 1);

            return (last_version = new_version);
        }

        bool contains(const OBJ& obj, pds::version_t version = master_version){

            if(version == master_version)
                return v_master.contains(obj);

            if(version > last_version)
                throw pds::VersionOutOfRange(
                    "pds::pset::contains: Version " + std::to_string(version) + " is out of range"
                );

            pds::fat_node_tracker<OBJ> tracker(root, version);

            while(tracker.not_null()){

                if(obj < tracker.obj()){

                    tracker = tracker.left();
                }
                else if(tracker.obj() < obj){

                    tracker = tracker.right();
                }
                else return true;
            }
            return false; 
        }
    
    private:
        template <typename T>
        pds::version_t insert_impl(T&& obj){

            pds::fat_node_tracker<OBJ> tracker(root, last_version);

            while(tracker.not_null()){

                if(obj < tracker.obj()){

                    tracker = tracker.left();
                }
                else if(tracker.obj() < obj){

                    tracker = tracker.right();
                }
                else{
                    throw pds::ObjectAlreadyExist(
                        "pds::pset::insert: Attempting to insert an object that already exists"
                    );
                }
            }

            pds::version_t new_version = last_version + 1;

            if(auto search = v_master.find(obj); search != v_master.end()){

                tracker[new_version] = fat_node<OBJ>(*search);
            }
            else{
                auto result = v_master.insert(std::make_shared<OBJ>(std::forward<T>(obj)));
                tracker[new_version] = fat_node<OBJ>(*result.first);
            }

            sizes.push_back(sizes[last_version] + 1);

            return (last_version = new_version);
        }

    };
};

#endif /* PARTIALLY_PERSISTENT_SET_HPP */