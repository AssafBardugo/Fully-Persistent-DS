#include "fplist.hpp"

int main(){

    // TODO: methods had changed!
    // TODO: list can contain duplicates but there is no example for it yet.

    fplist<int> list; // v=0
    // v=0 {}

    list.insert(5); // v=1
    // v=0 {}
    // v=1 {5}

    list.insert(3); // v=2
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}

    list.remove(5, 2); // v=3
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}
    // v=3 {3}

    list.insert(7, 2); // v=4
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}
    // v=3 {3}
    // v=4 {5, 3, 7}

    list.insert(11); // v=5 (insert to last version)
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}
    // v=3 {3}
    // v=4 {5, 3, 7}
    // v=5 {5, 3, 7, 11}

    list.contains(11, 2); // return 0

    list.contains(11); // return 5 (first version)

    list.remove(3); // v=6 (remove from last version)
    // v=0 {}
    // v=1 {5}
    // v=2 {5, 3}
    // v=3 {3}
    // v=4 {5, 3, 7}
    // v=5 {5, 3, 7, 11}
    // v=6 {5, 7, 11}

    list.remove(3); // throw exception since v=6 is not contains 3

    list.contains(3); // return 2

    return 0;
}