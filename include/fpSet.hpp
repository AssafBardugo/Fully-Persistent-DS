/**
 * @file fpSet.hpp
 * @author Assaf Bardugo (https://github.com/AssafBardugo)
 * 
 * @brief fully persistent set container.
 * @version 0.1
 * @date 2024-10-09
 */

#ifndef FULLY_PERSISTENT_SET_HPP
#define FULLY_PERSISTENT_SET_HPP

#include "internal/fpSetTracker.hpp"

namespace pds{

    /// @brief Default version indicating the 'last_version' for insert & remove operations.
    const pds::version_t default_version = std::numeric_limits<pds::version_t>::max();


    /**
     * @class fpSet
     * @brief Fully persistent set container for sorted, unique objects.
     * 
     * The fpSet class allows storing objects in a way that maintains historical versions
     * of the set after each modification. Each version is preserved, enabling access to any 
     * past state of the set.
     * 
     * @tparam OBJ The object type, which must support `operator<` for sorting and provide
     *             either copy or move constructors.
     * 
     * @note Space Complexity: O(N log(N))
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
     * ```
     */
    template <class OBJ>
    class fpSet{

        pds::fpFatNodePtr<OBJ> root;    ///< root of a BST that stores the data.
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
        fpSet();

        /**
         * @brief Inserts an object into the set at a specific version.
         * 
         * This method adds the given object to the set and ensures that it appears
         *      in the specified version and subsequent versions.
         * If the operation succeeds, a new version will be created.
         * 
         * @param obj object to insert.
         * @attention Since fpSet saves a copy of 'obj', the move option is recommended.
         * 
         * @param version The version to insert. 
         *  if 'version'=default_version insert to last version.
         * 
         * @exception 
         *  - pds::ObjectAlreadyExist
         *      thrown if: 'version'=default_version and contains('obj', 'curr_version()') return true,
         *      or 'version' specify and contains('obj', 'version') return true.
         * 
         * - pds::VersionZeroIllegal
         *      thrown if: version is 0
         * 
         * - pds::VersionNotExist
         *      thrown if: version is bigger than what returned with 'curr_version()'
         * 
         * @return pds::version_t of the new version.
         * 
         * @note Time complexity: O(log(N) * log*(N))
         *  while N is the number of versions, i.e. N is last_version.
         */
        pds::version_t insert(const OBJ& obj, pds::version_t version = default_version);


        /**
         * @brief Inserts an object into the set at a specific version.
         * 
         * This method adds the given object to the set and ensures that it appears
         *      in the specified version and subsequent versions.
         * If the operation succeeds, a new version will be created.
         * 
         * This is especially recommended for complex OBJ types, 
         *  as it can significantly improve performance by avoiding deep copies of data.
         * 
         * @param obj object to insert.
         * 
         * @param version The version to insert. 
         *  if 'version'=default_version insert to last version.
         * 
         * @exception 
         *  - pds::ObjectAlreadyExist
         *      thrown if: 'version'=default_version and contains('obj', 'curr_version()') return true,
         *      or 'version' specify and contains('obj', 'version') return true.
         * 
         * - pds::VersionZeroIllegal
         *      thrown if: version is 0
         * 
         * - pds::VersionNotExist
         *      thrown if: version is bigger than what returned with 'curr_version()'
         * 
         * @return pds::version_t of the new version.
         * 
         * @note Time complexity: O(log(N) * log*(N))
         *  while N is the number of versions, i.e. N is last_version.
         */
        pds::version_t insert(OBJ&& obj, pds::version_t version = default_version);


        /**
         * @brief Removes an object from the set at a specific version.
         * 
         * This method removes the object from the set, making it unavailable in the specified
         *      version and all subsequent versions.
         * If the operation succeeds, a new version will be created.
         * 
         * @param obj object to remove.
         * 
         * @param version the version to remove from. 
         *  If 'version'=default_version remove from last_version. 
         * @attention if contains('obj', 'version') == false: exception will thrown.
         * 
         * @exception 
         * - pds::ObjectNotExist
         *      thrown if: 'obj' not exists in 'version'
         * 
         * - pds::VersionZeroIllegal
         *      thrown if: version is 0
         * 
         * - pds::VersionNotExist
         *      thrown if: version is bigger than what returned with 'curr_version()'
         * 
         * @return pds::version_t of the new version. 
         * 
         * @note Time complexity: O(log(N) * log*(N))
         *  while N is the number of versions, i.e. N is last_version.
         */
        pds::version_t remove(const OBJ& obj, pds::version_t version = default_version);


        /**
         * @brief Removes an object from the set at a specific version.
         * 
         * This method removes the object from the set, making it unavailable in the specified
         *      version and all subsequent versions.
         * If the operation succeeds, a new version will be created.
         * 
         * Recommended for complex OBJ types, for avoiding deep copy of 'obj'.
         *
         * @param obj object to remove.
         * 
         * @param version the version to remove from. 
         *  If 'version'=default_version remove from last_version. 
         * @attention if contains('obj', 'version') == false: exception will thrown.
         * 
         * @exception 
         * - pds::ObjectNotExist
         *      thrown if: 'obj' not exists in 'version'
         * 
         * - pds::VersionZeroIllegal
         *      thrown if: version is 0
         * 
         * - pds::VersionNotExist
         *      thrown if: version is bigger than what returned with 'curr_version()'
         * 
         * @return pds::version_t of the new version.
         * 
         * @note Time complexity: O(log(N) * log*(N))
         *  while N is the number of versions, i.e. N is last_version.
         */
        pds::version_t remove(OBJ&& obj, pds::version_t version = default_version);


        /**
         * @brief Check if an object is in the set in a specific version.
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
         * @return true if the object exists in the specified version; otherwise, false.
         * 
         * @note Time complexity: O(log(N) * log*(N))
         *  while N is the number of versions, i.e. N is last_version.
         */
        bool contains(const OBJ& obj, pds::version_t version = MasterVersion);


        /**
         * @brief return a sorted set as std::vector<OBJ>.
         * 
         * @param version the version to return for.
         *  If the version is not specified then return the all objects in all versions as a sorted set.
         * 
         * @exception
         * - pds::VersionNotExist
         *      thrown if: version is bigger than what returned with 'curr_version()'
         * 
         * @return std::vector<OBJ> a sorted set.
         * 
         * @note Time complexity: O((K log*(N)) + log(N))
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
         *  If the version is not specified: the size of all unique objects in all versions.
         *  If version not exists: 0
         */
        pds::version_t size(pds::version_t version = MasterVersion) const noexcept;


        /**
         * @brief current version
         * 
         * @exception No exceptions.
         * 
         * @return pds::version_t 'last_version'.
         */
        pds::version_t curr_version() const noexcept;


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
         * @note Time complexity: O((K log*(N)) + log(N))
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
        pds::version_t insert_impl(T&& obj, pds::version_t version);
    };
};


template <class OBJ>
pds::fpSet<OBJ>::fpSet() : root(1), last_version(1), sizes{0, 0} {
}


template <class OBJ>
pds::version_t pds::fpSet<OBJ>::insert(const OBJ& obj, pds::version_t version){

    return insert_impl(obj, version);
}


template <class OBJ>
pds::version_t pds::fpSet<OBJ>::insert(OBJ&& obj, pds::version_t version){

    return insert_impl(std::move(obj), version);
}


template <class OBJ>
template <typename T>
pds::version_t pds::fpSet<OBJ>::insert_impl(T&& obj, pds::version_t version){

    if(version == default_version)
        version = last_version;

    if(version == MasterVersion)
        throw pds::VersionZeroIllegal("Version 0 is not valid for insert");

    PDS_THROW_IF_VERSION_NOT_EXIST("fpSet::insert_impl", version, last_version);

    if(contains(obj, version))
        throw pds::ObjectAlreadyExist(
                "fpSet::insert: Version " + std::to_string(version) + " already contains this object" 
            );

    pds::version_t new_version = last_version + 1;

    pds::fpSetTracker<OBJ> tracker(root, version);

    // Inserting a new version to the tree:
    while(tracker.not_null()){

        tracker[new_version] = *tracker;

        if(obj < tracker.obj()){

            tracker.add_right_map(new_version);
            tracker = tracker.left();
        }
        else{
            tracker.add_left_map(new_version);
            tracker = tracker.right();
        }
    }

    pds::fpSetTracker<OBJ> track_master(root, MasterVersion);

    while(track_master.not_null()){

        if(obj < track_master.obj()){

            track_master = track_master.left();
        }
        else if(track_master.obj() < obj){

            track_master = track_master.right();
        }
        else{
            tracker[new_version] = *track_master;
            tracker.set_left_at(new_version) = nullptr;
            tracker.set_right_at(new_version) = nullptr;
            break;
        }
    }

    if(track_master.null()){

        track_master[MasterVersion] = std::make_shared<pds::fpFatNode<OBJ>>(std::forward<T>(obj), new_version);

        ++sizes[MasterVersion];
        tracker[new_version] = *track_master;
    }

    // push the size of the new version
    sizes.push_back(sizes[version] + 1);

    return (last_version = new_version);
}


template <class OBJ>
pds::version_t pds::fpSet<OBJ>::remove(const OBJ& obj, pds::version_t version){

    if(version == default_version)
        version = last_version;

    if(version == MasterVersion)
        throw pds::VersionZeroIllegal("Version 0 is not valid for remove");

    PDS_THROW_IF_VERSION_NOT_EXIST("fpSet::remove", version, last_version);

    if(!contains(obj, version))
        throw pds::ObjectNotExist(
            "pds::fpSet::remove: Attempting to remove an object from Version "
            + std::to_string(version) + ". But the object is not exists for this Version" 
        );


    pds::version_t new_version = last_version + 1;
    pds::fpSetTracker<OBJ> tracker(root, version);

    // Inserting a new version to the tree:
    while(tracker.not_null()){

        if(obj < tracker.obj()){

            tracker[new_version] = *tracker;
            tracker.add_right_map(new_version);
            tracker = tracker.left();
        }
        else if(tracker.obj() < obj){

            tracker[new_version] = *tracker;
            tracker.add_left_map(new_version);
            tracker = tracker.right();
        }
        else break;
    }

    pds::fpSetTracker<OBJ> to_remove = tracker;

    if(to_remove.left_null() && to_remove.right_null()){

        to_remove[new_version] = nullptr;
    }
    else if(to_remove.left_null() || to_remove.right_null()){

        if(to_remove.left_null())
            tracker = tracker.right();
        else
            tracker = tracker.left();

        to_remove[new_version] = *tracker;
        to_remove.add_left_map(new_version);
        to_remove.add_right_map(new_version);
    }
    else{
        pds::fpSetTracker<OBJ> track_to_leaf = tracker;
        track_to_leaf = track_to_leaf.right();

        if(track_to_leaf.left_null()){

            to_remove[new_version] = *track_to_leaf;
            track_to_leaf.add_left_map(new_version);
            track_to_leaf.add_right_map(new_version);

            tracker = tracker.left();
            to_remove.set_left_at(new_version) = *tracker;
            tracker.add_left_map(new_version);
            tracker.add_right_map(new_version);
        }
        else{

/***************  ILLUSTRATION  **************************

          +++ version +++    |   +++ new_version +++
                             |
      to_remove:[6]          |           [11]
               /   \        ==>>        /   \
            [4]     [16]     |       [4]     [16]
                   /         |              /
               [11]          |          [13]
                   \         |
                    [13]     |

***********************************************************/

            while(track_to_leaf.left_not_null()){

                track_to_leaf[new_version] = *track_to_leaf;
                track_to_leaf.add_right_map(new_version);
                track_to_leaf = track_to_leaf.left();
            }
            to_remove[new_version] = *track_to_leaf;

            if(track_to_leaf.right_not_null()){

                track_to_leaf[new_version] = track_to_leaf.get_right();
                track_to_leaf.add_left_map(new_version);

                track_to_leaf = track_to_leaf.right();
                track_to_leaf.add_left_map(new_version);
                track_to_leaf.add_right_map(new_version);
            }
            else{
                track_to_leaf[new_version] = nullptr;
            }

            to_remove.set_right_at(new_version) = tracker.get_right();
            to_remove.set_left_at(new_version) = tracker.get_left();
            tracker = tracker.left();
            tracker.add_left_map(new_version);
            tracker.add_right_map(new_version);
        }
    }

    // push the size of the new version
    sizes.push_back(sizes[version] - 1);

    return (last_version = new_version);
}


template <class OBJ>
pds::version_t pds::fpSet<OBJ>::remove(OBJ&& obj, pds::version_t version){

    return remove(std::as_const(obj), version);
}


template <class OBJ>
bool pds::fpSet<OBJ>::contains(const OBJ& obj, pds::version_t version){

    PDS_THROW_IF_VERSION_NOT_EXIST("fpSet::contains", version, last_version);

    pds::fpSetTracker<OBJ> tracker(root, version);

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


template <class OBJ>
std::vector<OBJ> pds::fpSet<OBJ>::to_vector(const pds::version_t version) {

    PDS_THROW_IF_VERSION_NOT_EXIST("fpSet::to_vector", version, last_version);

    std::vector<OBJ> obj_vec;

    /*** Inorder Stack Traversal ***/
    std::stack<pds::fpSetTracker<OBJ>> trav_stack;
    pds::fpSetTracker<OBJ> tracker(root, version);

    while(tracker.not_null() || !trav_stack.empty()){

        while(tracker.not_null()){

            trav_stack.push(tracker);
            tracker = tracker.left();
        }
        tracker = trav_stack.top();
        trav_stack.pop();

        obj_vec.push_back(tracker.obj());
        tracker = tracker.right();
    }
    return obj_vec;
}


template <class OBJ>
pds::version_t pds::fpSet<OBJ>::size(pds::version_t version) const noexcept {

    try{
        return sizes.at(version);
    }
    catch(const std::out_of_range&){

        return 0;
    }
}


template <class OBJ>
pds::version_t pds::fpSet<OBJ>::curr_version() const noexcept {

    return last_version;
}


template <class OBJ>
void pds::fpSet<OBJ>::print(pds::version_t version){

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