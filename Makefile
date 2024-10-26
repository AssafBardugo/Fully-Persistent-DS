CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Werror -Iinclude

HEADERS = include/fpSet.hpp \
		  include/pSet.hpp \
		  include/internal/fpSetTracker.hpp \
		  include/internal/pSetTracker.hpp \
          include/internal/fpFatNode.hpp \
		  include/internal/pFatNode.hpp \
          include/internal/Excep.hpp \
          include/internal/Utils.hpp \
		  include/internal/UnionFind.hpp \
          tests/pds_test.h

TESTS_SRCS = tests/test_fpSet.cpp \
			 tests/test_pSet.cpp \
			 tests/main.cpp

TESTS_OBJS = $(TESTS_SRCS:tests/%.cpp=build/%.o)


all: test
	./test

test: $(TESTS_OBJS)
	$(CXX) $^ -o $@

build/%.o: tests/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f build/*.o test

.PHONY: all test clean