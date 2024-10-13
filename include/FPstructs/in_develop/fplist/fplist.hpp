/**
 * @file fplist.hpp
 * @author Assaf Bardugo (https://github.com/AssafBardugo)
 * 
 * @brief fully persistent list container.
 * @version 0.1
 * @date 2024-10-09
 */

/**
 * implemantation:
 * 
 * master_version = set<shared_ptr<fat_node>>
 * 
 * root = unorderd_map<version_id, shared_ptr<fat_node>>
 * 
 * fat_node = {obj, 
 *              first_version,
 *              last_version,
 *              last_mapped,
 *              left=unorderd_map<version_id, shared_ptr<fat_node>>, 
 *              right=unorderd_map<version_id, shared_ptr<fat_node>>
 *             }
 */

#ifndef FULLY_PERSISTENT_LIST_HPP
#define FULLY_PERSISTENT_LIST_HPP

#include <cstddef>
#include <cstdbool>
#include <unordered_set>

using version_id = std::size_t;
version_id default_v;

/**
 * @brief 
 * 
 * @tparam T object type.
 * @attention 
 *      A. There is NO REQUIREMENTS for support any operator from type T.
 *      B. There is NO REQUIREMENTS for unique elements in the structure.
 * 
 * @note 
 * Space complexity: O(size())
 */
template <class T>
class fplist{

    
public:
    struct iterator{
    private:
        version_id insert_version;
    };

    /**
     * @brief Construct an empty fully-persistent unordered set container.
     *  The initial version is 0. and version 0 will always be empty.
     * @note 
     * Time complexity: O(1)
     * Space complexity: O(1)
     */
    fplist();

    /**
     * @brief insert obj to version v.
     * 
     * @param obj object to insert.
     * 
     * @param v the version to insert to. if v=default_v insert to last version.
     * @attention There is NO REQUIREMENTS for unique elements in the structure.
     * 
     * @return iterator to obj
     * 
     * @note 
     * Time complexity: O(log(size()))
     */
    fplist<T>::iterator insert(T obj, version_id v = default_v);


    version_id push_back(T obj, version_id v = default_v);

    version_id pop_back(T obj, version_id v = default_v);


    /**
     * @brief remove obj from the unordered_set with version v.
     * 
     * @param iter the iterator that was return from insert
     * 
     * @attention if contains('iter') == 0 exception will thrown.
     * 
     * @return version_id of the new version.
     * 
     * @note 
     * Time complexity: O(log(size()))
     */
    version_id remove(fplist::iterator iter);

    /**
     * @brief check if obj in the unordered_set.
     * 
     * @param obj the object to query for.
     * @param v if v=default_v search in all versions. else, search for version v.
     *  
     * @return 
     * if v not given: the first version that contains 'obj'. 0 if 'obj' has never insert.
     * if v specify: v if 'obj' contains. 0 else.
     * 
     * @note 
     * Time complexity: O(log(size()))
     */
    version_id contains(fplist::iterator, version_id v = default_v);

    /**
     * @brief get unordered_set of all objects.
     * 
     * @param v if v=default_v get all objects from all versions. else, objects of version v.
     * @return std::unordered_set<T> of the objects in the order they were entered.
     * 
     * @note 
     * Time complexity: O(size(v))
     */
    std::unordered_set<T> get(version_id v = default_v);

    /**
     * @brief return the last version.
     * 
     * @note 
     * Time complexity: O(1)
     */
    version_id curr_version();
};

#endif /* FULLY_PERSISTENT_LIST_HPP */