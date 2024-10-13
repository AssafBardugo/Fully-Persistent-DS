#ifndef PERSISTENT_DATA_STRUCTURE_FAT_NODE_IMPLEMENTATION_HPP
#define PERSISTENT_DATA_STRUCTURE_FAT_NODE_IMPLEMENTATION_HPP

#include "fat_node.hpp"
#include "excep.hpp"

using namespace pds;

template <class OBJ>
fat_node<OBJ>::fat_node(const OBJ& obj, const version_t new_version) 

    : obj(obj), left(nodes_table(new_version)), right(nodes_table(new_version)) {

}

template <class OBJ>
fat_node<OBJ>::fat_node(OBJ&& obj, const version_t new_version) 

    : obj(std::move(obj)), left(nodes_table(new_version)), right(nodes_table(new_version)) {

}

template <class OBJ>
bool fat_node<OBJ>::operator<(const fat_node& other) const {
    return obj < other.obj;
}

template <class OBJ>
bool fat_node<OBJ>::operator<(const OBJ& other_obj) const {
    return obj < other_obj;
}

template <class OBJ>
bool fat_node<OBJ>::operator>(const OBJ& other_obj) const {
    return other_obj < obj;
}


/*** Implementation of helper classes ***/
template <class FN>
nodes_table<FN>::nodes_table(const version_t create_version) 

    : table{{create_version, nullptr}} {
}

template <class FN>
std::shared_ptr<FN>& nodes_table<FN>::operator[](const version_t key){

    return table[key];
}

template <class FN>
std::shared_ptr<FN>& nodes_table<FN>::at(const version_t key){

    try{
        for(map_cow& new_map : cow_stack)
            table[new_map.new_version] = table[new_map.old_version];

        if(table.at(key) != nullptr){

            table.at(key)->left.map(cow_stack);
            table.at(key)->right.map(cow_stack);
        }

        cow_stack.clear();
        return table.at(key);
    }
    catch(const std::out_of_range& e){

        throw VersionOutOfRange(
            "nodes_table::at: std::out_of_range::what(): " + std::string(e.what())
        );
    }
}

template <class FN>
void nodes_table<FN>::map(const std::vector<pds::map_cow>& to_push){

    cow_stack.insert(cow_stack.end(), to_push.begin(), to_push.end());
}


#endif /* PERSISTENT_DATA_STRUCTURE_FAT_NODE_IMPLEMENTATION_HPP */