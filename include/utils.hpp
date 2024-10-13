#ifndef UTILITYS_HPP
#define UTILITYS_HPP

#include <cassert>
#include <cstdbool>
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>


namespace pds{
    
    using version_t = std::size_t;


    template <class T>
    void print_span(std::span<T> v_arr){

        std::cout << "{";
        if(!v_arr.empty()){
        	std::copy(v_arr.begin(), v_arr.end() - 1, std::ostream_iterator<T>(std::cout, ", "));
			std::cout << v_arr.back();
        }
        std::cout << "}" << std::endl;
    }


    /****** for checking if type T has an 'operator<<' ******/

    // Primary template for the trait, defaults to false
    template<typename, typename = void>
    struct has_output_operator 
        : std::false_type {
    };

    // Specialization for types that have operator<<
    template<typename T>
    struct has_output_operator<T, std::void_t<decltype(std::cout << std::declval<T>())>> 
        : std::true_type {
    };
    /********************************************************/
};

#endif /* UTILITYS_HPP */