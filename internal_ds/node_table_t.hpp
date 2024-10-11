#ifndef NEXT_TABLE_TYPE_HPP
#define NEXT_TABLE_TYPE_HPP

#include "utils.hpp"

namespace pds{

    struct version_range{
        pds::version_t start;
        pds::version_t end;
        // TODO: need to provide a hash function

        bool operator==(const version_range& other) const {
            return start == other.start;
        }
    };

    template <class FN>
    class node_table_t{
        std::unordered_map<pds::version_range, std::shared_ptr<FN>> table;

    public:
        std::shared_ptr<FN>& operator[](const pds::version_t key);
    };
};


#endif /* NEXT_TABLE_TYPE_HPP */