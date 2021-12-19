#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <iostream>
#include <stdlib.h>
#include "./sigmod2013contest-1.1/include/core.h"

using namespace std;

class payload_node {

    int id;
    payload_node* next;

    public:

        /* Constructor - Destructor */
        payload_node(int tmpId) {
            this->id = tmpId; 
            this->next = NULL;
        }

        /* Getters */
        int getId() const    { return this->id; }
        payload_node* getNext() const { return this->next; }
   
        /* Setters */
        void setNext(payload_node* tmp) { this->next = tmp; }
        void setId(int tmpId) { this->id = tmpId; }
};

class payload_list{

    payload_node* head;

    public:
        
        payload_list();
        ~payload_list();

        //Ektypwnei kathe komvo
        ErrorCode print_list();
        ErrorCode payload_insert(int id);
        ErrorCode destroy_payload_list();
        
        ErrorCode search_id(int id);

        //Getters - Setters
        payload_node* getFirst() const{ return head;}

        void setFirst(payload_node* tmp){ head = tmp; }
        
};

#endif