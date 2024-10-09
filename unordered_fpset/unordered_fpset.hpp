/**
 * @file unordered_fpset.hpp
 * @author Assaf Bardugo (https://github.com/AssafBardugo)
 * 
 * @brief fully persistent unordered set container
 * @version 0.1
 * @date 2024-10-09
 */

/**
 * implemantation:
 * 
 * 
 */

#ifndef FULLY_PERSISTENT_UNORDERED_SET_HPP
#define FULLY_PERSISTENT_UNORDERED_SET_HPP

#include <cstddef>
#include <cstdbool>
#include <unordered_set>

using version_id = std::size_t;
version_id default_v;

template <class T>
class unordered_fpset{

public:
    /**
     * @brief Construct an empty fully-persistent unordered set container.
     *  The initial version is 0. and version 0 will always be empty.
     */
    unordered_fpset();

    /**
     * @brief insert obj to version v.
     * 
     * @param obj object to insert.
     * 
     * @param v the version to insert to. if v=default_v insert to last version.
     * @attention if contains('obj', 'v') != 0 exception will thrown.
     * 
     * @return version_id of the new version.
     */
    version_id insert(T obj, version_id v = default_v);


    /**
     * @brief remove obj from the unordered_set with version v.
     * 
     * @param obj object to remove.
     * 
     * @param v the version to remove from. if v=default_v remove from last version. 
     * @attention if contains('obj', 'v') == 0 exception will thrown.
     * 
     * @return version_id of the new version. (option: also the version that 'obj' was removed from).
     */
    version_id remove(T obj, version_id v = default_v);

    /**
     * @brief check if obj in the unordered_set.
     * 
     * @param obj the object to query for.
     * @param v if v=default_v search in all versions. else, search for version v.
     *  
     * @return 
     * if v not given: the first version that contains 'obj'. 0 if 'obj' has never insert.
     * if v specify: v if 'obj' contains. 0 else.
     */
    version_id contains(T obj, version_id v = default_v);

    /**
     * @brief get unordered_set of all objects.
     * 
     * @param v if v=default_v get all objects from all versions. else, objects of version v.
     * @return std::unordered_set<T> of the objects in the order they were entered.
     */
    std::unordered_set<T> get(version_id v = default_v);

    /**
     * @brief return the last version.
     */
    version_id curr_version();
};

#endif /* FULLY_PERSISTENT_UNORDERED_SET_HPP */