/**
 * @file fpset.hpp
 * @author Assaf Bardugo (https://github.com/AssafBardugo)
 * 
 * @brief fully persistent set container.
 * @version 0.1
 * @date 2024-10-09
 */

/**
 * implemantation:
 * 
 * master_version = set<shared_ptr<fat_node>>
 * 
 * root = unorderd_map<pds::version_id, shared_ptr<fat_node>>
 * 
 * fat_node = {obj, 
 *              first_version,
 *              last_version,
 *              last_mapped,
 *              left=unorderd_map<pds::version_id, shared_ptr<fat_node>>, 
 *              right=unorderd_map<pds::version_id, shared_ptr<fat_node>>
 *             }
 */

#ifndef FULLY_PERSISTENT_SET_HPP
#define FULLY_PERSISTENT_SET_HPP

#include <cstddef>
#include <cstdbool>
#include <set>
#include "utils.hpp"


/**
 * @brief fully persistent set container for sorted objects.
 * 
 * @tparam T obj type. SHOULD SUPPORT operator<.
 */
template <class T>
class fpset{

public:
    fpset();

    /**
     * @brief insert obj to version v.
     * 
     * @param obj object to insert.
     * 
     * @param v the version to insert to. if v=default_v insert to last version.
     * @attention if contains('obj', 'v') != 0 an exception will be thrown.
     * 
     * @return pds::version_id of the new version.
     * 
     * @note 
     * Time complexity: O(log(size()))
     */
    pds::version_id insert(T obj, pds::version_id v = default_v);


    /**
     * @brief remove obj from the set with version v.
     * 
     * @param obj object to remove.
     * 
     * @param v the version to remove from. if v=default_v remove from last version. 
     * @attention if contains('obj', 'v') == 0 exception will thrown.
     * 
     * @return pds::version_id of the new version. (option: also the version that 'obj' was removed from).
     * 
     * @note 
     * Time complexity: O(log(size()))
     */
    pds::version_id remove(T obj, pds::version_id v = default_v);


    /**
     * @brief check if obj in the set.
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
    bool contains(T obj, pds::version_id v = default_v);


    /**
     * @brief get set of all objects sorted.
     * 
     * @param v if v=default_v get all objects from all versions. else, objects of version v.
     * @return std::set<T> of the objects sorted.
     * 
     * @note 
     * Time complexity: O(size(v))
     */
    std::set<T> get(pds::version_id v = default_v);
};



#endif /* FULLY_PERSISTENT_SET_HPP */