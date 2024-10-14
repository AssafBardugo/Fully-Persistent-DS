CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Werror -Iinclude

HEADERS = include/internal/nodes_table.hpp \
          include/internal/fat_node.hpp \
          include/fpset.hpp \
          include/fpset_impl.hpp \
          include/excep.hpp \
          include/utils.hpp	\
          tests/pds_test.hpp

TESTS_SRCS = tests/test_nodes_table.cpp \
			 tests/test_fat_node.cpp \
			 tests/test_fpset.cpp \
			 tests/main.cpp

TESTS_OBJS = $(TESTS_SRCS:tests/%.cpp=build/%.o)


all: 
	./build.sh --memcheck

test: $(TESTS_OBJS)
	$(CXX) $^ -o $@

build/%.o: tests/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f build/*.o test

.PHONY: all test clean