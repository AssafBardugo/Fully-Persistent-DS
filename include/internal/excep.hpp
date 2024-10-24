#ifndef PERSISTENT_DATA_STRUCTURE_EXCEPTIONS_HPP
#define PERSISTENT_DATA_STRUCTURE_EXCEPTIONS_HPP

#include "utils.hpp"

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
                - pds::fpset::insert_impl
                - pds::fpset::remove
                - pds::fpset::contains
                - pds::fpset::to_vector

                - pds::pset::contains
                - pds::pset::to_vector
                - pds::pset::print
                
                - pds::fat_node_tracker - almost all func
        */
    public:
        VersionNotExist(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class VersionZeroIllegal : public pdsExcept{
        /*
            thrown by:
                - pds::fpset::insert_impl
        */
    public:
        VersionZeroIllegal(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class ObjectAlreadyExist : public pdsExcept{
        /*
            thrown by:
                - pds::pset::insert_impl
                - pds::fpset::insert_impl
        */
    public:
        ObjectAlreadyExist(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class ObjectNotExist : public pdsExcept{
        /*
            thrown by:
                - pds::pset::remove
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