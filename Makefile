CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Werror -Iinclude

HEADERS = include/internal/nodes_table.hpp \
          include/internal/fat_node.hpp \
          include/fpset.hpp \
          include/fpset_impl.hpp \
          include/excep.hpp \
          include/utils.hpp	\
          tests/tools_tests.hpp

all: test_nodes_table test_fat_node test_fpset

test_nodes_table: test_nodes_table.o
	$(CXX) $^ -o $@

test_fat_node: test_fat_node.o
	$(CXX) $^ -o $@

test_fpset: test_fpset.o
	$(CXX) $^ -o $@

test_nodes_table.o: tests/test_nodes_table.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test_fat_node.o: tests/test_fat_node.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test_fpset.o: tests/test_fpset.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o test_nodes_table test_fat_node test_fpset

.PHONY: all clean