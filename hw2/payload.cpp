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

int main(){

    payload_list* myList = new payload_list();

    myList->payload_insert(5);
    myList->payload_insert(8);

    myList->payload_insert(7);
    myList->payload_insert(5);

    myList->print_list();

}