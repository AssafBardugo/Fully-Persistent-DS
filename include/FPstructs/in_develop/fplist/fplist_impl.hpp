template <class OBJ>
class fplist_node : public fat_node<OBJ>{

public:
    /*
        If we give an order-relation on fat_node according to the insert-version of the object, 
        we can have duplicates of objects in the structure.
        And so we can define a List structure.
    */
    bool operator<(fat_node& other) const;
};