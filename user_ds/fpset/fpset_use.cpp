#include "fpset.hpp"

int main(){

    fpset<int> set; // v=0
    // v=0 {}

    set.insert(5); // v=1
    // v=0 {}
    // v=1 {5}

    set.insert(3); // v=2
    // v=0 {}
    // v=1 {5}
    // v=2 {3, 5}

    set.remove(5, 2); // v=3
    // v=0 {}
    // v=1 {5}
    // v=2 {3, 5}
    // v=3 {3}

    set.insert(7, 2); // v=4
    // v=0 {}
    // v=1 {5}
    // v=2 {3, 5}
    // v=3 {3}
    // v=4 {3, 5, 7}

    set.insert(11); // v=5 (insert to last version)
    // v=0 {}
    // v=1 {5}
    // v=2 {3, 5}
    // v=3 {3}
    // v=4 {3, 5, 7}
    // v=5 {3, 5, 7, 11}

    set.contains(11, 2); // return 0

    set.contains(11); // return 5 (first version)

    set.insert(1, 4); // v=6
    // v=0 {}
    // v=1 {5}
    // v=2 {3, 5}
    // v=3 {3}
    // v=4 {3, 5, 7}
    // v=5 {3, 5, 7, 11}
    // v=6 {1, 3, 5, 7}

    set.remove(3, 5); // v=7
    // v=0 {}
    // v=1 {5}
    // v=2 {3, 5}
    // v=3 {3}
    // v=4 {3, 5, 7}
    // v=5 {3, 5, 7, 11}
    // v=6 {1, 3, 5, 7}
    // v=7 {5, 7, 11}

    set.insert(3); // v=8
    // v=0 {}
    // v=1 {5}
    // v=2 {3, 5}
    // v=3 {3}
    // v=4 {3, 5, 7}
    // v=5 {3, 5, 7, 11}
    // v=6 {1, 3, 5, 7}
    // v=7 {5, 7, 11}
    // v=8 {3, 5, 7, 11}

    return 0;
}