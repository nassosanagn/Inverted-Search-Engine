#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <iostream>
#include <stdlib.h>

#include "../Lists/entry.h"
#include "../q_hashtable.h"

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
        ErrorCode rehash();

        ErrorCode search(word *W,query_Hashtable* Q_hash,int current_doc,payload_list* q_result){
            int func_out = hash_function(W->getword(),size);
            entry* e = buckets[func_out]->search_word(W);
            if(e){
                payload_node* pNode = e->getpayload()->getFirst();

                while(pNode != NULL){
                    if (Q_hash->add_one(e->getmyword(), pNode->getId(),current_doc) == EC_SUCCESS){
                        q_result->payload_insert_asc(pNode->getId());
                    }
                    pNode = pNode->getNext();
                }
            }
            return EC_SUCCESS;
        }
};



#endif