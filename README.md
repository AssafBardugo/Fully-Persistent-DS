
# Fully Persistent Data Structures in C++


This project is a C++ template library implementing [Fully Persistent Data Structures](https://en.wikipedia.org/wiki/Persistent_data_structure) using the **Fat Node** method.

It enables efficient storage and retrieval of all previous versions of a data structure. 
The project is highly modular, using templates to support arbitrary data types and designed for real-world applications where persistent version tracking is essential.

### Key Features:
- **Fat Node Structure**: Efficiently tracks all versions of a data structure while minimizing memory overhead.
- **Fully Persistent**: Allows modifications and access to both old and current versions.

## Features

- **Efficient Version Control**: Easily access and work with different versions of a data structure.
- **Fast Querying**: Implements version management using a Binary Search Tree (BST) for efficient access.
- **Optimized Memory Usage**: Keeps only one copy of each object, regardless of the number of versions to which it has been added.
- **Copy and Move Semantics**: Supports both copy and move operations to ensure efficient resource handling in various scenarios.

### Supported Data Structures

- **Fully Persistent Set** (`fpset<T>`)  
Additional data structures like **list** and **string** are currently under development.

### Example

Here is a basic example of creating and working with a fully persistent set:

```cpp
#include <cassert>
#include <string>

#include "fpset.hpp"

int main() {

    pds::fpset<std::string> my_fpset; // Create a new fpset with first version
    // Version 1: {}

    assert(my_fpset.insert("a") == 2); // Create Version 2 by insert "a"
    // Version 1: {}
    // Version 2: {"a"}

    assert(my_fpset.insert("b") == 3); // Create Version 3 by insert "b" to last version
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}

    assert(my_fpset.insert("c", 2) == 4); // Insert "c" to version 2
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}
    // Version 4: {"a", "c"}

    assert(my_fpset.remove("c") == 5); // Remove "c" from last version
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}
    // Version 4: {"a", "c"}
    // Version 5: {"a"}

    assert(my_fpset.contains("c", 4) == true); // Version 4 stil contain "a"

    assert(my_fpset.remove("b", 3) == 6); // Create a new version from version 3 without "b"
    // Version 1: {}
    // Version 2: {"a"}
    // Version 3: {"a", "b"}
    // Version 4: {"a", "c"}
    // Version 5: {"a"}
    // Version 6: {"a"}

    assert(my_fpset.contains("b") == true); // "b" exist in some version
    
    assert(my_fpset.size(1) == 0); // Version 1 has no elements
    assert(my_fpset.size() == 3); // The size of all elements is 3: {"a", "b", "c"}

    // fpset can return a set of specific version
    assert(my_fpset.to_set(4) == std::set<std::string>({"a", "c"})); 
    assert(my_fpset.to_set() == std::set<std::string>({"a", "b", "c"})); // All elements

    return 0;
}
```

## Usage

To integrate the library into your project:
1. Download the files ans merge the [include](https://github.com/AssafBardugo/Fully-Persistent-DS/tree/main/include) folder with your include folder.
2. Include the data structure you to want to use:

```cpp
#include "fpset.hpp"

pds::fpset my_set;
```

#### Prerequisites

- **C++20**: Ensure you have C++20 installed.



## Documentation

To generate the documentation using **Doxygen**, ensure Doxygen is installed on your system and then run the following:

```bash
doxygen Doxyfile
```

This will generate the documentation in the `docs/` directory.

## Testing

To build and run the tests:

```bash
make test
./test
```

Ensure that the `tests/` directory contains all relevant unit tests for your data structures.

## Project Structure

```
.
├── include/
│   ├── internal/       
│   │   ├── fat_node.hpp        # Fat Node header for version tracking
│   │   └── fat_node_impl.hpp   # Fat Node implementation for version tracking
│   ├── in_develop/
│   │   ├── fplist/
│   │   │   ├── fplist.hpp
│   │   │   └── fplist_impl.hpp
│   │   └── fpstring.hpp
│   ├── fpset.hpp               # Header for fully persistent set
│   ├── fpset_impl.hpp          # Implementation of fully persistent set
│   ├── pds_excep.hpp           # Exception handling for the library
│   └── utils.hpp               # Utility functions for the library
├── tests/
│   ├── test_fpset.cpp          # Unit tests for the fpset class
│   ├── test_fplist.cpp         # Unit tests for the fplist class
│   └── test_fat_node.cpp       # Unit tests for the fat_node class
├── Makefile                    # Makefile for building the project
├── README.md                   # Project documentation
└── Doxyfile                    # Doxygen configuration file for generating docs
```

## Future Work

- Implement fully persistent data structures for **list**, **string**, and **priority_queue**.
- Explore optimizations for minimizing memory overhead even further.

## Contributing

Contributions are welcome! If you would like to contribute, please fork the repository, create a feature branch, and submit a pull request. Any improvements, optimizations, or bug fixes are greatly appreciated.
