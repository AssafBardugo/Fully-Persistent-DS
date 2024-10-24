#ifndef PERSISTENT_DATA_STRUCTURE_UNION_FIND_HPP
#define PERSISTENT_DATA_STRUCTURE_UNION_FIND_HPP

#include "FatNode.hpp"

class UnionFind{

    std::unordered_map<pds::version_t, pds::version_t> parent;  // Maps each version to its parent
    std::unordered_map<pds::version_t, pds::version_t> rank;    // Maps each version to its rank

public:
    // Adds a new element to the Union-Find structure, if it's not already present.
    void add(pds::version_t version){

        if(parent.find(version) == parent.end()){

            parent[version] = version;  // Each element is initially its own parent (root)
            rank[version] = 0;      // Rank is initially 0
        }
    }

    // Finds the representative (root) of the set containing 'version', with path compression.
    pds::version_t Find(pds::version_t version){

        if(parent.find(version) == parent.end()) 
            return version; // If version is not present

        if(parent[version] != version){

            parent[version] = Find(parent[version]); // Path compression
        }
        return parent[version];
    }

    // unites the sets containing 'version1' and 'version2'.
    void Union(pds::version_t version1, pds::version_t version2){

        add(version1);
        add(version2);
        pds::version_t root1 = Find(version1);
        pds::version_t root2 = Find(version2);

        if(root1 != root2){
            // Union by rank
            if(rank[root1] > rank[root2]){

                parent[root2] = root1;
            } 
            else if(rank[root1] < rank[root2]){

                parent[root1] = root2;
            } 
            else{
                parent[root2] = root1;
                rank[root1]++;
            }
        }
    }

    // Check if two elements are in the same set
    bool connected(pds::version_t version1, pds::version_t version2){

        return Find(version1) == Find(version2);
    }
};

#endif /* PERSISTENT_DATA_STRUCTURE_UNION_FIND_HPP */