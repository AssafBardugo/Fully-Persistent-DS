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

#include "fat_node.hpp"


namespace pds{

    /**
     * @brief default_version uses to be 'last_version' in insert & remove.
     */
    const pds::version_t default_version = std::numeric_limits<std::size_t>::max();

    /**
     * @anchor MasterVersion
     * @brief Defines an exceptional version that is intended for internal use.
     * 
     * @details 
     * What is it: Version 0 (to be called 'MasterVersion') is reserve for a unique mission, to save 
     *  the all T-nodes together. A new T-node will insert first to MasterVersion and then its 'std::shared_ptr' 
     *  will copied to store it also for its real version.
     * 
     * @remarks
     * Why its good: The uses of groups all T-nodes (@ref T-node VS fat-node) under one version are:
     * 
     *      A. It helps to insert or remove in BST time-search by determine if a new T-node 
     *          is already exist in other version and we can avoid its unnecessary duplicate.
     * 
     *      B. Avoiding self loops between versions. 
     *          For example: {insert(obj=7, v=1), insert(obj=9, v=2), remove(obj=7, v=3), insert(obj=7, v=4)}
     *          will create the loop [7]<--->[9] between versions 2 and 4.
     * 
     *      C. A simple memory managment. Thanks to the fact that we avoiding self loops, we can use 'std::shared_ptr'
     *          and T-node will free itself when the all fat-nodes who points on it will removes its copy.
     * 
     *      D. Finally, for easy insert & remove algorithms.
     */
    const pds::version_t master_version = 0;

    /**
     * @brief fully persistent set container for sorted UNIQUE objects.
     * 
     * @tparam OBJ The object type. SHOULD SUPPORT operator< and copy constructor.
     */
    template <class OBJ>
    class fpset{

        std::set<std::shared_ptr<pds::fat_node<OBJ>>> v_master;
        pds::node_table_t<pds::fat_node<OBJ>> root;
        pds::version_t last_version;

    public:
        fpset();

        /**
         * @brief Insert an object to store in 'version'.
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
         *      or 'version' specify and contains('obj', 'version') != 0 
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
         *      or 'version' specify and contains('obj', 'version') != 0 
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
         * 
         * @param obj object to remove.
         * 
         * @param v the version to remove from. if 'version'=default_version remove from last_version. 
         * @attention if contains('obj', 'version') == 0 exception will thrown.
         * 
         * @return pds::version_t of the new version. (option: also the version that 'obj' was removed from).
         * 
         * @note 
         * Time complexity: O(log(size()))
         */
        pds::version_t remove(const OBJ& obj, pds::version_t version = default_version);


        /**
         * @brief check if obj in the set.
         * 
         * @param obj the object to query for.
         * @param v if v=master_version search in all versions. else, search for version v.
         *  
         * @return 
         * if v not given: the first version that contains 'obj'. 0 if 'obj' has never insert.
         * if v specify: v if 'obj' contains. 0 else.
         * 
         * @note 
         * Time complexity: O(log(size()))
         */
        bool contains(const OBJ& obj, pds::version_t version = master_version) const;

        pds::version_t size(pds::version_t version = master_version) const;

        pds::version_t curr_version() const;

        /**
         * @brief get set of all objects sorted.
         * 
         * @param v if v=master_version get all objects from all versions. else, objects of version v.
         * @return std::set<T> of the objects sorted.
         * 
         * @note 
         * Time complexity: O(size(v))
         */
        std::set<OBJ> to_set(pds::version_t version = master_version) const;

    private:
        /**
         * INTENDED FOR DEBUGGING PURPOSES ONLY
         * @brief Ensure that 'version' has the all vec's objs
         * 
         * @param version version for fpset
         * @param vec a vector for OBJ elements
         * @return true if 'version' contain exactly the same objects as vec
         * @return false otherwise
         */
        bool equal(const pds::version_t version, std::vector<OBJ>&& vec) const;
    };
};

#endif /* FULLY_PERSISTENT_SET_HPP */