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

    class VersionZero : public pdsExcept {
        /*
            thrown by:
                - pds::fat_node C'tor
        */
    public:
        VersionZero(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class VersionAlreadyExist : public pdsExcept {
        /*
            thrown by:
        */
    public:
        VersionAlreadyExist(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class VersionOutOfRange : public pdsExcept {
        /*
            thrown by:
                - pds::fpset::insert_impl
                - pds::fpset::remove
                - pds::fpset::contains
                - pds::node_table::at
        */
    public:
        VersionOutOfRange(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class VersionHasNullNext : public pdsExcept {
        /*
            thrown by:
                - pds::fpset::insert
        */
    public:
        VersionHasNullNext(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class ObjectAlreadyExist : public pdsExcept{
        /*
            thrown by:
                - pds::fpset::insert_impl
        */
    public:
        ObjectAlreadyExist(std::string&& m) : pdsExcept(std::move(m)){}
    };

    class ObjectNotExist : public pdsExcept{
        /*
            thrown by:
                - pds::fpset::remove
        */
    public:
        ObjectNotExist(std::string&& m) : pdsExcept(std::move(m)){}
    };
    
};

#endif /* PERSISTENT_DATA_STRUCTURE_EXCEPTIONS_HPP */