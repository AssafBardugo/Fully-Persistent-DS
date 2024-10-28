/**
 * @file pSet.hpp
 * @author Assaf Bardugo (https://github.com/AssafBardugo)
 * 
 * @brief Partially persistent set container.
 * @version 0.1
 * @date 2024-10-28
 */
#ifndef PARTIALLY_PERSISTENT_SET_HPP
#define PARTIALLY_PERSISTENT_SET_HPP

#include "internal/pSetTracker.hpp"

namespace pds{

    /**
     * @class pSet
     * @brief Partially persistent set container for sorted unique objects.
     * @tparam OBJ The object type, which must support `operator<` for sorting and provide
     *             either copy or move constructors.
     * 
     * @note Space Complexity: O(N)
     *  while N is the number of versions, i.e. N is last_version.
     * Every 'obj' will save exactly once regardless of the number of versions it was inserted into.
     */
    /**
     * @class pSet
     * @brief Partially persistent set container for sorted, unique objects.
     * 
     * The pSet class allows storing objects in a way that maintains historical versions
     * of the set after each modification. Each version is preserved, enabling search and get
     *  methods for any past state of the set.
     * 
     * @tparam OBJ The object type, which must support `operator<` for sorting and provide
     *             either copy or move constructors.
     * 
     * @note Space Complexity: O(N)
     *       - N represents the number of versions maintained (i.e., `last_version`).
     *       - Every object is saved only once, regardless of how many versions it exists in.
     * 
     * @example
     * ```
     * pds::fpSet<int> vrsSet;
     * vrsSet.insert(5);
     * vrsSet.insert(10);
     * vrsSet.remove(5);
     * // Access previous version states
     * assert(vrsSet.contains(5, 2) == true);
     * ```
     */
    template <class OBJ>
    class pSet{

        pds::pFatNodePtr<OBJ> root;    ///< root of a BST that save the data
        pds::version_t last_version;    ///< in the range of [1, MAX_size_t]
        std::vector<pds::version_t> sizes;  ///< keep the size for each version (including 0 for MasterVersion)

    public:
        /**
         * @brief Construct a new object.
         * 
         * @details Also insert two init Versions:
         *  - Version 0: see @ref MasterVersion.
         *  - Version 1: will save the init state version. (will always be empty).
         */
        pSet();


        /**
         * @brief Insert an object to the set.
         *  If the operation succeeds, a new version will be created.
         * 
         * @param obj object to insert.
         * @attention Since pSet saves a copy of 'obj', the move option is recommended.
         * 
         * @exception 
         *  - pds::ObjectAlreadyExist
         *      thrown if: 'obj' exists in last version, i.e. contains('obj', curr_version()) return true.
         * 
         * @return pds::version_t of the new version. see @ref pds::version_t.
         * 
         * @note Time complexity: O((log(N))^2)
         *  while N is the number of versions, i.e. N is last_version.
         */
        pds::version_t insert(const OBJ& obj);


        /**
         * @brief Insert an rvalue object to the set.
         *  If the operation succeeds, a new version will be created.
         * 
         * This is especially recommended for complex OBJ types, 
         *  as it can significantly improve performance by avoiding deep copies of data.
         * 
         * @param obj object to insert.
         * @attention Since pSet saves a copy of 'obj', the move option is more recommended.
         * 
         * @exception 
         *  - pds::ObjectAlreadyExist
         *      thrown if: 'obj' exists in last version, i.e. contains('obj', curr_version()) return true.
         * 
         * @return pds::version_t of the new version. see @ref pds::version_t.
         * 
         * @note Time complexity: O((log(N))^2)
         *  while N is the number of versions, i.e. N is last_version.
         */
        pds::version_t insert(OBJ&& obj);


        /**
         * @brief remove obj from the set.
         *  If the operation succeeds, a new version will be created.
         * 
         * @param obj object to remove.
         * 
         * @exception 
         * - pds::ObjectNotExist
         *      thrown if: 'obj' does not exist in the last version, i.e. contains('obj', curr_version()) return false.
         * 
         * @return pds::version_t of the new version. 
         * 
         * @note Time complexity: O((log(N))^2)
         *  while N is the number of versions, i.e. N is last_version.
         */
        pds::version_t remove(const OBJ& obj);


        /**
         * @brief remove an rvalue object from the set.
         *  If the operation succeeds, a new version will be created.
         * 
         * @param obj object to remove.
         * 
         * @exception 
         * - pds::ObjectNotExist
         *      thrown if: 'obj' not exists in last version, 
         *          i.e. contains('obj', curr_version()) return false.
         * 
         * @return pds::version_t of the new version. 
         * 
         * @note Time complexity: O((log(N))^2)
         *  while N is the number of versions, i.e. N is last_version.
         */
        pds::version_t remove(OBJ&& obj);


        /**
         * @brief check if obj is in the set.
         * 
         * @param obj the object to query for.
         * 
         * @param version version to check for.
         *  If the version is not specified then check for MasterVersion which mean: any version.
         * 
         * @exception
         * - pds::VersionNotExist
         *      thrown if: version is bigger than what returned with 'curr_version()'
         *
         * @return true if 'obj' exists in 'version', false otherwise.
         * 
         * @note Time complexity: O((log(N))^2)
         *  while N is the number of versions, i.e. N is last_version.
         */
        bool contains(const OBJ& obj, pds::version_t version = MasterVersion);
    

        /**
         * @brief return a sorted set as std::vector<OBJ>.
         * 
         * @param version the version to return for.
         *  If version is not specify then return the all objects in all versions as a sorted set.
         * 
         * @exception
         * - pds::VersionNotExist
         *      thrown if: version is bigger than what returned with 'curr_version()'
         * 
         * @return std::vector<OBJ> a sorted set.
         * 
         * @note Time complexity: O(K log(N))
         *  while K is the number of objects in 'version'
         *  and N is the number of versions, i.e. N is last_version.
         */
        std::vector<OBJ> to_vector(const pds::version_t version = MasterVersion);


        /**
         * @brief size of the set for 'version'.
         * 
         * @param version the version to return for.
         * 
         * @exception No exceptions.
         * 
         * @return pds::version_t a std::size_t. see @ref pds::version_t.
         *  If version is not specify: the size of all unique objects in all versions.
         *  If version not exists: 0
         */
        std::size_t size(pds::version_t version = MasterVersion) const;


        /**
         * @brief current version
         * 
         * @exception No exceptions.
         * 
         * @return pds::version_t 'last_version'.
         */
        pds::version_t curr_version() const;


        /**
         * @brief print the set sorted.
         * @details The print style is: "Version X: {obj1, obj2, ...}"
         * 
         * @param version to print.
         *  If no version is specify then print the all versions sorted.
         * 
         * @exception
         * - pds::VersionNotExist
         *      thrown if: version is bigger than what returned with 'curr_version()'
         * 
         * @note Time complexity: O(K log(N))
         *  while K is the number of objects in 'version'
         *  and N is the number of versions, i.e. N is last_version.
         */
        void print(pds::version_t version = MasterVersion);

    private:
        /**
         * @brief insert implementation. 
         * For internal use, only to avoid code duplication.
         * @tparam T Generic type for one of the options: const OBJ&, OBJ&&
         */
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
        tracker.set_track_version(new_version);

        if(tracker.left_null() == false){

            tracker.set_left(new_version) = nullptr;
        }
        if(tracker.right_null() == false){

            tracker.set_right(new_version) = nullptr;
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
            "pds::pset::remove: Attempting to remove an object but the object is not exists" 
        );

    pds::version_t new_version = last_version + 1;

    pds::pSetTracker<OBJ> to_remove = tracker;
    pds::pSetTracker<OBJ> track_to_leaf = tracker;

    if(to_remove.left_null()){

        to_remove[new_version] = tracker.get_right();
    }
    else if(to_remove.right_null()){

        to_remove[new_version] = tracker.get_left();
    }
    else{
        track_to_leaf = track_to_leaf.right();

        if(track_to_leaf.left_null()){

            to_remove[new_version] = *track_to_leaf;
            to_remove.set_track_version(new_version);
            to_remove.set_left(new_version) = tracker.get_left();
        }
        else{
            while(!track_to_leaf.left_null()){

                track_to_leaf = track_to_leaf.left();
            }
            track_to_leaf[new_version] = track_to_leaf.get_right();

            to_remove[new_version] = *track_to_leaf;
            to_remove.set_track_version(new_version);
            to_remove.set_left(new_version) = tracker.get_left();
            to_remove.set_right(new_version) = tracker.get_right();
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