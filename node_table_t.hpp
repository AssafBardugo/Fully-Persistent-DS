#ifndef NEXT_TABLE_TYPE_HPP
#define NEXT_TABLE_TYPE_HPP

#include "utils.hpp"

namespace pds{

    struct version_range{
        pds::version_t start;
        pds::version_t end;
        // TODO: need to provide a hash function and operator==
    };

    template <class FN>
    class node_table_t{
        std::unordered_map<pds::version_range, std::shared_ptr<FN>> table;

    };
};


#endif /* NEXT_TABLE_TYPE_HPP */