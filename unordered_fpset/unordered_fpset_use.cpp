#include "unordered_fpset.hpp"

int main(){

    unordered_fpset<int> unset; // v=0
    // v=0 {}

    unset.insert(5); // v=1
    // v=0 {}
    // v=1 {5}

    unset.insert(3); // v=2
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}

    unset.remove(5, 2); // v=3
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}
    // v=3 {3}

    unset.insert(7, 2); // v=4
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}
    // v=3 {3}
    // v=4 {5, 3, 7}

    unset.insert(11); // v=5 (insert to last version)
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}
    // v=3 {3}
    // v=4 {5, 3, 7}
    // v=5 {5, 3, 7, 11}

    unset.contains(11, 2); // return 0

    unset.contains(11); // return 5 (first version)

    unset.remove(3); // v=6 (remove from last version)
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}
    // v=3 {3}
    // v=4 {5, 3, 7}
    // v=5 {5, 3, 7, 11}
    // v=6 {5, 7, 11}

    unset.remove(3); // throw exception since v=6 is not contains 3

    unset.contains(3); // return 2

    return 0;
}