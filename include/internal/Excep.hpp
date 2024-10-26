#ifndef PERSISTENT_DATA_STRUCTURE_EXCEPTIONS_HPP
#define PERSISTENT_DATA_STRUCTURE_EXCEPTIONS_HPP

#include "Utils.hpp"

namespace pds{

    class pdsExcept : public std::exception {
        /*
            Base class for pds exceptions.
        */
        std::string message;
    public:
        pdsExcept(std::string&& m) : message(std::move(m)){}

        const char* what() const noexcept override {
            return message.c_str();
        }
    };


    class VersionNotExist : public pdsExcept {
        /*
            thrown by:
                - pds::fpSet::insert_impl
                - pds::fpSet::remove
                - pds::fpSet::contains
                - pds::fpSet::to_vector
                - pds::fpSet::print

                - pds::pSet::contains
                - pds::pSet::to_vector
                - pds::pSet::print
                
                - pds::fat_node_tracker - almost all func
                - pds::UnionFind::Find
        */
    public:
        VersionNotExist(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class VersionZeroIllegal : public pdsExcept{
        /*
            thrown by:
                - pds::fpSet::insert_impl
                - pds::fpSet::remove
        */
    public:
        VersionZeroIllegal(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class ObjectAlreadyExist : public pdsExcept{
        /*
            thrown by:
                - pds::pSet::insert_impl
                - pds::fpSet::insert_impl
        */
    public:
        ObjectAlreadyExist(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class ObjectNotExist : public pdsExcept{
        /*
            thrown by:
                - pds::pSet::remove
                - pds::fpSet::remove
        */
    public:
        ObjectNotExist(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class NullTracker : public pdsExcept{
        /*
            thrown by:
                - pds::fat_node_tracker
        */
    public:
        NullTracker(std::string&& m) : pdsExcept(std::move(m)){}
    };
};

#endif /* PERSISTENT_DATA_STRUCTURE_EXCEPTIONS_HPP */