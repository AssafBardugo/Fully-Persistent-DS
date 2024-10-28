/**
 * @file Utils.hpp
 * @author Assaf Bardugo (https://github.com/AssafBardugo)
 * 
 * @brief utilities for pds library
 * @version 0.1
 * @date 2024-10-28
 */
#ifndef PERSISTENT_DATA_STRUCTURE_UTILITYS_HPP
#define PERSISTENT_DATA_STRUCTURE_UTILITYS_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdbool>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Excep.hpp"


#define PDS_THROW_IF_NULL_TRACKER(func_name, vrs)                   \
            do{ if(!ptr->table.at(vrs))                             \
                    throw pds::NullTracker("fat_node_tracker::"     \
            + std::string(func_name) + ": table.at(" + std::to_string(vrs) + ") is nullptr"); } while(0)

#define PDS_THROW_IF_TRACK_VERSION_NOT_EXIST(func_name, vrs)            \
            do{ try{ PDS_THROW_IF_NULL_TRACKER(func_name, vrs); }       \
                catch(const std::out_of_range&){                        \
                    throw pds::VersionNotExist("fat_node_tracker::"     \
            + std::string(func_name) + ": Version " + std::to_string(vrs) + " is not exist"); } } while(0)

#define PDS_THROW_IF_VERSION_NOT_EXIST(func_name, vrs, last_vrs)        \
            do{ if(vrs > last_vrs)                                      \
                    throw pds::VersionNotExist(std::string(func_name)   \
            + ": Version " + std::to_string(vrs) + " is not exist"); } while(0)


#define PRINT_GREEN(str)  "\033[32m" << str << "\033[0m"

// TOMOVE
namespace pds{

    /**
     * @anchor pds::version_t
     * @brief A version type for all pds library.
     */
    using version_t = std::size_t;

    /**
     * @anchor MasterVersion
     * @brief Defines an exceptional version that is intended for internal use.
     * 
     * @details 
     * What is it: Version 0 (to be called 'MasterVersion') is reserve for a unique mission, to save 
     *  the all T-nodes together. A new T-node will insert first to MasterVersion and then its 'std::shared_ptr' 
     *  will copied to store it also for its real version.
     * 
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
    const version_t MasterVersion = 0;
};

#endif /* PERSISTENT_DATA_STRUCTURE_UTILITYS_HPP */