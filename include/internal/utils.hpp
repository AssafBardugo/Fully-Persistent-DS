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


#define PDS_THROW_IF_NULL_TRACKER(func_name, vrs)   \
            do{ if(!ptr->table.at(vrs))             \
                    throw pds::NullTracker(         \
            std::string(func_name) + ": table.at(" + std::to_string(vrs) + ") is nullptr"); } while(0)

#define PDS_THROW_IF_VERSION_NOT_EXIST(func_name, vrs, last_vrs)    \
            do{ if(vrs > last_vrs)                                  \
                    throw pds::VersionNotExist("pds::pset::" +      \
            std::string(func_name) + ": Version " + std::to_string(vrs) + " is not exist"); } while(0)


#endif /* PERSISTENT_DATA_STRUCTURE_UTILITYS_HPP */