#ifndef FULLY_PERSISTENT_SET_HPP
#define FULLY_PERSISTENT_SET_HPP

#include <cstddef>
#include <cstdbool>
#include <set>

using version_id = std::size_t;
version_id default_v;

template <class T>
class fpset{

public:
    fpset();

    version_id insert(T obj, version_id v = default_v);

    version_id remove(T obj, version_id v = default_v);

    bool contains(T obj, version_id v = default_v);

    std::set<T> get(version_id v = default_v);
};



#endif /* FULLY_PERSISTENT_SET_HPP */