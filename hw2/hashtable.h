#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <iostream>
#include <stdlib.h>

#include "../hw1/main_folder/entry.h"
#include "q_hashtable.h"

#define SIZE 10000

class Hashtable {
    entry_list** buckets;
    int size;
    int counter;
    public:
        unsigned long hash_function(char*,int);
        Hashtable();
        ~Hashtable();

        ErrorCode insert(entry *,int);
        ErrorCode print();
        ErrorCode search(word *W,query_Hashtable* Q_hash,int current_doc,payload_list* q_result);
        ErrorCode rehash();
};



#endif