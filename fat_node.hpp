#ifndef FAT_NODE_HPP
#define FAT_NODE_HPP

#include <cstddef>
#include <unordered_map>
#include <memory>

using version_id = std::size_t;

template <class T>
class fat_node{
    T obj;
    std::unordered_map<version_id, std::shared_ptr<fat_node<T>>> left;
    std::unordered_map<version_id, std::shared_ptr<fat_node<T>>> right;
public:
    /*
        If we give an order-relation on fat_node according to the insert-version of the object, 
        we can have duplicates of objects in the structure.
        And so we can define a List structure.
    */
    bool operator<(fat_node& other) const;
};


template <class T>
class fplist_node : public fat_node{

public:
    /*
        If we give an order-relation on fat_node according to the insert-version of the object, 
        we can have duplicates of objects in the structure.
        And so we can define a List structure.
    */
    bool operator<(fat_node& other) const;
};

#endif /* FAT_NODE_HPP */