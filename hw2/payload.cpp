#include "payload.h"

/* ---------------------------------------- payload_list functions ---------------------------------------- */

payload_list::payload_list(){
    this->head = NULL;
}

payload_list::~payload_list(){
    destroy_payload_list();
}

ErrorCode payload_list::destroy_payload_list(){

    if (head == NULL){
        return EC_FAIL;
    }

    payload_node* current = head;
    payload_node* next = NULL;

    while (current != NULL){

        next = current->getNext();

        delete current;
        current = next;
    }

    head = NULL;
    return EC_SUCCESS;
}

ErrorCode payload_list::print_list(){
    
    if (head == NULL){
        return EC_FAIL;
    }

    payload_node* current = this->getFirst();

    while (current != NULL){
        cout << current->getId() << " ";
        current = current->getNext();
    }

    cout << endl;
    return EC_SUCCESS;
}

ErrorCode payload_list::(int id){
    payload_node* n = head;
    while(n!=NULL){
        if(n->getId() == id){
            return EC_SUCCESS;
        }
        n = n->getNext();
    }
    return EC_FAIL;
}


ErrorCode payload_list::payload_insert(int tmpId){

    
    payload_node* new_node = new payload_node(tmpId);
    payload_node* current = this->getFirst();

    if (current == NULL){
        this->setFirst(new_node);
        return EC_SUCCESS;
    }

    while (current->getNext() != NULL){
        current = current->getNext();
    }

    current->setNext(new_node);
    return EC_SUCCESS;
}

ErrorCode payload_list::payload_insert_asc(int tmpId){

    
    payload_node* new_node = new payload_node(tmpId);
    payload_node* current = this->getFirst();

    if (current == NULL){
        this->setFirst(new_node);
        return EC_SUCCESS;
    }

    while (current->getNext()!=NULL &&current->getNext()->getId() < tmpId){
        current = current->getNext();
    }
    new_node->setNext(current->getNext());
    current->setNext(new_node);

    return EC_SUCCESS;
}