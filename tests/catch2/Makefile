CATCH_MAIN = catch2/catch_main.o

all: clean test
	./test

test: test_pset.cpp $(CATCH_MAIN) pset.hpp internal/excep.hpp internal/utils.hpp internal/fat_node.hpp internal/fat_node_tracker.hpp
	g++ -std=c++23 -Wall -Wextra -Werror -Icatch2 test_pset.cpp -o test $(CATCH_MAIN)

$(CATCH_MAIN): catch2/catch_main.cpp
	g++ -std=c++23 -Wall -Icatch2 -c catch2/catch_main.cpp -o $@

clean:
	rm -f test