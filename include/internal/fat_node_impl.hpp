#ifndef FAT_NODE_IMPLEMENTATION_HPP
#define FAT_NODE_IMPLEMENTATION_HPP

#include "fat_node.hpp"
#include "excep.hpp"

using namespace pds;

template <class OBJ>
fat_node<OBJ>::fat_node(const OBJ& obj, const version_t new_version) 

    : obj(obj), left(node_table(new_version)), right(node_table(new_version)) {

}

template <class OBJ>
fat_node<OBJ>::fat_node(OBJ&& obj, const version_t new_version) 

    : obj(std::move(obj)), left(node_table(new_version)), right(node_table(new_version)) {

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
node_table<FN>::node_table(const version_t create_version) 

    : table{{create_version, nullptr}} {
}

template <class FN>
std::shared_ptr<FN>& node_table<FN>::operator[](const version_t key){

    return table[key];
}

template <class FN>
std::shared_ptr<FN>& node_table<FN>::at(const version_t key){

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
            "node_table::at: std::out_of_range::what(): " + std::string(e.what())
        );
    }
}

template <class FN>
void node_table<FN>::map(const std::vector<pds::map_cow>& to_push){

    cow_stack.insert(cow_stack.end(), to_push.begin(), to_push.end());
}


#endif /* FAT_NODE_IMPLEMENTATION_HPP */