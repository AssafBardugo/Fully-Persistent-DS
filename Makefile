all: clean test
	./test

HEADERS = include/fpset.hpp \
			include/internal/excep.hpp \
			include/internal/utils.hpp \
			include/internal/FatNode.hpp \
			include/internal/UnionFind.hpp \
			include/internal/fptracker.hpp

test: tests/test_fpset.cpp $(HEADERS)
	g++ -std=c++23 -Wall -Wextra -Werror -Iinclude tests/test_fpset.cpp -o test

clean:
	rm -f tests/test_fpset.o test