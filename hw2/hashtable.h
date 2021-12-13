#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <iostream>
#include <stdlib.h>
#include "../hw1/main_folder/entry.h"
#define SIZE 100

class Hashtable {
    entry_list** buckets;
    int size;
    int counter;
    public:
        unsigned long hash_function(char* str,int size);
        Hashtable();
        ErrorCode insert(entry *,int);
        ErrorCode print();
        entry* search(word *);
        ErrorCode rehash();
};



#endif