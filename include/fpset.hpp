/**
 * @file fpset.hpp
 * @author Assaf Bardugo (https://github.com/AssafBardugo)
 * 
 * @brief fully persistent set container.
 * @version 0.1
 * @date 2024-10-09
 */

#ifndef FULLY_PERSISTENT_SET_HPP
#define FULLY_PERSISTENT_SET_HPP

#include "internal/fptracker.hpp"

namespace pds{

    /**
     * @brief default_version uses to be 'last_version' in insert & remove.
     */
    const pds::version_t default_version = std::numeric_limits<pds::version_t>::max();

    /**
     * @brief fully persistent set container for sorted UNIQUE objects.
     * 
     * @tparam OBJ The object type. SHOULD SUPPORT operator< and copy constructor.
     */
    template <class OBJ>
    class fpset{

        pds::fat_node_ptr<OBJ> root;
        pds::version_t last_version;
        std::vector<pds::version_t> sizes;

    public:
        fpset();

        /**
         * @brief Insert an object to store in 'version'.
         *  If the operation succeeds, a new version will be created.
         * 
         * @param obj object to insert.
         * @attention Since fpset saves the objects sorted, it will save a copy of 'obj', 
         *  so the move option is more recommended.
         * 
         * @param version The version to insert. 
         *  if 'version'=default_version insert to last version.
         * 
         * @exception 
         *  - pds::ObjectAlreadyExist()
         *      if 'version'=default_version and contains('obj', 'curr_version()') != 0 
         *      or 'version' specify and contains('obj', 'version') == true 
         * 
         * - pds::VersionOutOfRange()
         *      if version is 0 or version is bigger than what returned with 'curr_version()'
         * 
         * @return pds::version_t of the new version.
         * 
         * @note Time complexity: O(log(size()))
         */
        pds::version_t insert(const OBJ& obj, pds::version_t version = default_version);


        /**
         * @brief Insert an rvalue object to store in 'version'.
         *  If the operation succeeds, a new version will be created.
         * 
         *  Especially recommended for complex OBJ types, since it can significantly 
         *  improve performance by avoiding deep copies of data.
         * 
         * @param obj object to insert.
         * 
         * @param version The version to insert. 
         *  if 'version'=default_version insert to last version.
         * 
         * @exception 
         *  - pds::ObjectAlreadyExist()
         *      if 'version'=default_version and contains('obj', 'curr_version()') != 0 
         *      or 'version' specify and contains('obj', 'version') == true 
         * 
         * - pds::VersionOutOfRange()
         *      if version is 0 or version is bigger than what returned with 'curr_version()'
         * 
         * @return pds::version_t of the new version.
         * 
         * @note Time complexity: O(log(size()))
         */
        pds::version_t insert(OBJ&& obj, pds::version_t version = default_version);


        /**
         * @brief remove obj from the set with 'version'.
         *  If the operation succeeds, a new version will be created.
         * 
         * @param obj object to remove.
         * 
         * @param version the version to remove from. 
         *  if 'version'=default_version remove from last_version. 
         * @attention if contains('obj', 'version') == false: exception will thrown.
         * 
         * @exception 
         * - pds::VersionOutOfRange()
         *      if version is 0 or version is bigger than what returned with 'curr_version()'
         * 
         * @return pds::version_t of the new version. 
         * 
         * @note 
         * Time complexity: O(log(size()))
         */
        // pds::version_t remove(const OBJ& obj, pds::version_t version = default_version);


        // pds::version_t remove(OBJ&& obj, pds::version_t version = default_version);


        /**
         * @brief check if obj in the set.
         * 
         * @param obj the object to query for.
         * @param v if v=MasterVersion search in all versions. else, search for version v.
         *  
         * @return true if 'obj' exists in 'version', false otherwise.
         * 
         * @note 
         * Time complexity: O(log(size()))
         */
        bool contains(const OBJ& obj, pds::version_t version = MasterVersion);


        std::vector<OBJ> to_vector(const pds::version_t version = MasterVersion);


        pds::version_t size(pds::version_t version = MasterVersion) const;


        pds::version_t curr_version() const;


        void print(pds::version_t version = MasterVersion);

    private:
        template <typename T>
        pds::version_t insert_impl(T&& obj, pds::version_t version);
    };
};


template <class OBJ>
pds::fpset<OBJ>::fpset() : root(1), last_version(1), sizes{0, 0} {
}


template <class OBJ>
pds::version_t pds::fpset<OBJ>::insert(const OBJ& obj, pds::version_t version){

    return insert_impl(obj, version);
}


template <class OBJ>
pds::version_t pds::fpset<OBJ>::insert(OBJ&& obj, pds::version_t version){

    return insert_impl(std::move(obj), version);
}


template <class OBJ>
template <typename T>
pds::version_t pds::fpset<OBJ>::insert_impl(T&& obj, pds::version_t version){

    if(version == default_version)
        version = last_version;

    if(version == MasterVersion)
        throw pds::VersionZeroIllegal("Version 0 is not valid for insert");

    PDS_THROW_IF_VERSION_NOT_EXIST("fpset::insert_impl", version, last_version);


    pds::fat_node_tracker<OBJ> tracker(root);

    while(tracker.not_null_at(version)){

        if(obj < tracker.obj_at(version)){

            tracker = tracker.left_at(version);
        }
        else if(tracker.obj_at(version) < obj){

            tracker = tracker.right_at(version);
        }
        else{
            throw pds::ObjectAlreadyExist(
                "fpset::insert: Version " + std::to_string(version) + " already contains this object" 
            );
        }
    }

    pds::version_t new_version = last_version + 1;

    pds::fat_node_tracker<OBJ> track_master(root);

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

        track_master[MasterVersion] = std::make_shared<pds::fat_node<OBJ>>(std::forward<T>(obj), new_version);

        ++sizes[MasterVersion];
        tracker[new_version] = track_master.at(MasterVersion);
    }
    else{
        if(tracker.left_not_null_at(new_version)){

            tracker.set_left_at(new_version) = nullptr;
        }
        if(tracker.right_not_null_at(new_version)){

            tracker.set_right_at(new_version) = nullptr;
        }
    }

    // push the size of the new version
    sizes.push_back(sizes[version] + 1);

    return (last_version = new_version);
}


// template <class OBJ>
// pds::version_t pds::fpset<OBJ>::remove(const OBJ& obj, pds::version_t version){

// //     if(version == default_version)
// //         version = last_version;

// //     if(version == 0 || version > last_version)
// //         throw pds::VersionNotExist(
// //             "pds::fpset::remove: Version " + std::to_string(version) + " is out of range"
// //         );

// //     pds::fat_node_tracker<OBJ> tracker(root);

// //     while(tracker.not_null_at(version)){

// //         if(obj < tracker.obj_at(version)){

// //             tracker = tracker.left_at(version);
// //         }
// //         else if(tracker.obj_at(version) < obj){

// //             tracker = tracker.right_at(version);
// //         }
// //         else break;

// //         tracker.copy_map(version);
// //     }

// //     if(tracker.null_at(version))
// //         throw pds::ObjectNotExist(
// //             "pds::fpset::remove: Attempting to remove an object from Version "
// //             + std::to_string(version) + ". But the object is not exists for this Version" 
// //         );

    
// //     pds::fat_node_tracker<OBJ> to_remove = tracker;
// //     pds::version_t new_version = last_version + 1;

// //     if(to_remove.left_at(version).null_at(version)){

// //         to_remove[new_version] = to_remove.ref_right_at(version);
// //     }
// //     else if(to_remove.right_at(version).null_at(version)){

// //         to_remove[new_version] = to_remove.ref_left_at(version);
// //     }
// //     else{

// // /***************  ILLUSTRATION  **************************

// //           +++ version +++    |   +++ new_version +++
// //                              |
// //       to_remove:[6]          |           [9]
/*                /   \        ==>>        /   \
// //             [4]     [16]     |       [4]     [16]
// //                    /         |              /
// //                [11]          |          [11]
// //               /              |
// //            [9]               |
 */
// /***********************************************************/

// //         tracker = to_remove.right_at(version);

// //         while(tracker.left_at(version).not_null_at(version)){

// //             tracker[new_version] = tracker.at(version);
// //             tracker = tracker.left_at(version);
// //         }
// //         tracker[new_version] = nullptr;

// //         to_remove[new_version] = tracker.at(version);
// //     }

// //     add_new_version(tracker.obj_at(new_version), version);

// //     // push the size of the new version
// //     sizes.push_back(sizes[version] - 1);

//     return (last_version = new_version);
// }


// template <class OBJ>
// pds::version_t pds::fpset<OBJ>::remove(OBJ&& obj, pds::version_t version){

//     return remove(std::as_const(obj), version);
// }


template <class OBJ>
bool pds::fpset<OBJ>::contains(const OBJ& obj, pds::version_t version){

    PDS_THROW_IF_VERSION_NOT_EXIST("fpset::contains", version, last_version);

    pds::fat_node_tracker<OBJ> tracker(root);

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
std::vector<OBJ> pds::fpset<OBJ>::to_vector(const pds::version_t version) {

    PDS_THROW_IF_VERSION_NOT_EXIST("fpset::to_vector", version, last_version);

    std::vector<OBJ> obj_vec;

    /*** Inorder Stack Traversal ***/
    std::stack<pds::fat_node_tracker<OBJ>> trav_stack;
    pds::fat_node_tracker<OBJ> tracker(root);

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
pds::version_t pds::fpset<OBJ>::size(pds::version_t version) const {

    try{
        return sizes.at(version);
    }
    catch(const std::out_of_range&){

        return 0;
    }
}


template <class OBJ>
pds::version_t pds::fpset<OBJ>::curr_version() const {

    return last_version;
}


template <class OBJ>
void pds::fpset<OBJ>::print(pds::version_t version){

    PDS_THROW_IF_VERSION_NOT_EXIST("pset::print", version, last_version);

    std::vector<OBJ> vec = to_vector(version);

    std::cout << "Version " << version << ": {";
    if(!vec.empty()){
        std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<OBJ>(std::cout, ", "));
        std::cout << vec.back();
    }
    std::cout << "}" << std::endl;
}

#endif /* FULLY_PERSISTENT_SET_HPP */