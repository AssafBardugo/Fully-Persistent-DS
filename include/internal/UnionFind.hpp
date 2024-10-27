#ifndef PERSISTENT_DATA_STRUCTURE_UNION_FIND_HPP
#define PERSISTENT_DATA_STRUCTURE_UNION_FIND_HPP

#include "Utils.hpp"

namespace pds{

    class UnionFind{

        std::unordered_map<std::size_t, std::size_t> parent;
        std::unordered_map<std::size_t, std::size_t> size;

    public:
        std::size_t Find(std::size_t version){

            if(parent.find(version) == parent.end()) 
                throw pds::VersionNotExist(
                    "UnionFind::Find: Version " + std::to_string(version) + " has no parent"
                );

            if(parent[version] != version){

                parent[version] = Find(parent[version]);
            }
            return parent[version];
        }

        void Union(std::size_t version1, std::size_t version2){

            std::size_t root1 = Find(version1);
            std::size_t root2 = Find(version2);

            if(root1 != root2){

                if(size[root1] <= size[root2]){

                    parent[root1] = root2;
                    size[root2] += size[root1];
                }
                else{
                    // We shouldn't get here since size[root1] is always 1 in our fpSetTracker
                    assert(false); 
                    parent[root2] = root1;
                    size[root1] += size[root2];
                }
            }
        }

        void add(std::size_t version){

            parent[version] = version;
            size[version] = 1;
        }
    };
};

#endif /* PERSISTENT_DATA_STRUCTURE_UNION_FIND_HPP */