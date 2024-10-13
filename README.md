
# Fully Persistent Data Structures Library in C++

## Overview

This project is a C++ template library implementing Fully Persistent Data Structures (FPDS) using the **Fat Node** method. It enables efficient storage and retrieval of all previous versions of a data structure. This allows for operations such as **undo/redo**, **version control**, and **time-travel queries** for databases. The project is highly modular, using templates to support arbitrary data types and designed for real-world applications where persistent version tracking is essential.

### Key Features:
- **Fat Node Structure**: Efficiently tracks all versions of a data structure while minimizing memory overhead.
- **Persistent Operations**: Non-destructive updates (insert, delete, modify) that preserve all prior versions.
- **Fully Persistent**: Allows modifications and access to both old and current versions.
- **Applications**: Version control, editing systems (undo/redo), and time-travel queries in databases.
- **Extensible Design**: Ready for extension to other persistent data structures beyond the set.

## Features

- **Generic Template Classes**: Supports arbitrary types via templates.
- **Efficient Version Control**: Easily access and work with different versions of a data structure.
- **Fast Querying**: Implements version management using a Binary Search Tree (BST) for efficient access.
- **Optimized Memory Usage**: Tracks only the changes between versions, reducing the memory footprint.
- **Copy and Move Semantics**: Supports both copy and move operations to ensure efficient resource handling in various scenarios.

## Supported Data Structures

- **Fully Persistent Set** (`fpset<T>`)  
Additional data structures like stacks and queues may be added in future versions.

## Installation

### Prerequisites

- **GCC**: Ensure you have GCC installed (version 9.0 or later recommended).
- **MSYS2** (for Windows users): Install MSYS2 to provide the required build environment.

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/persistent-structures-c.git
   ```

2. Navigate to the project directory:
   ```bash
   cd persistent-structures-c
   ```

3. Build the project using `make`:
   ```bash
   make
   ```

## Usage

To integrate the library into your project, include the header files in your source code:

```cpp
#include "fpset.h"
#include "fat_node.h"
```

### Example

Here is an example of creating and working with a fully persistent set:

```cpp
#include "fpset.h"
#include <iostream>

int main() {
    pds::fpset<int> set;

    set.insert(1); // Insert element 1
    set.insert(2); // Insert element 2

    auto version1 = set.get_version(); // Save the first version
    set.insert(3); // Insert element 3 in the current version

    auto version2 = set.get_version(); // Save the second version

    // Access the different versions
    std::cout << "Version 1 contains 1: " << version1.contains(1) << "
"; // Outputs: true
    std::cout << "Version 2 contains 3: " << version2.contains(3) << "
"; // Outputs: true

    return 0;
}
```

## Documentation

To generate the documentation using **Doxygen**, ensure Doxygen is installed on your system and then run the following:

```bash
doxygen Doxyfile
```

This will generate the documentation in the `docs/` directory.

## Testing

Unit tests are provided using the **Unity** testing framework. To build and run the tests:

```bash
make test
./test
```

Ensure that the `tests/` directory contains all relevant unit tests for your data structures.

## Project Structure

```
.
├── include/
│   ├── fat_node.h       # Fat Node implementation for tracking versions
│   ├── fpset.h          # Fully Persistent Set template class
├── src/
│   └── main.cpp         # Example usage of the library
├── tests/
│   └── test_fpset.cpp   # Unit tests for the fpset class
├── Makefile             # Makefile for building the project
├── README.md            # Project documentation
└── Doxyfile             # Doxygen configuration file for generating docs
```

## Future Work

- Implement additional fully persistent data structures such as **queues**, **stacks**, and **maps**.
- Explore optimizations for minimizing memory overhead even further.
- Add support for persistent algorithms, allowing operations over different versions of data structures.

## Contributing

Contributions are welcome! If you would like to contribute, please fork the repository, create a feature branch, and submit a pull request. Any improvements, optimizations, or bug fixes are greatly appreciated.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
