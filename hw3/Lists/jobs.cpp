#include "jobs.h"

/* ---------------------------------------- job_list functions ---------------------------------------- */

job_list::job_list(){
    counter = 0;
    this->head = NULL;
    this->last = NULL;
}

job_list::~job_list(){
    destroy_job_list();
}

ErrorCode job_list::destroy_job_list(){

    if (head == NULL){
        return EC_FAIL;
    }

    job_node* current = head;
    job_node* next = NULL;

    while (current != NULL){

        next = current->getNext();

        delete current;
        current = next;
    }

    head = NULL;
    return EC_SUCCESS;
}

ErrorCode job_list::print_list(){
    
    if (head == NULL){
        return EC_FAIL;
    }

    job_node* current = this->getFirst();

    while (current != NULL){
        cout << current->getId() << " ";
        current = current->getNext();
    }

    cout << endl;
    return EC_SUCCESS;
}


ErrorCode job_list::job_insert(QueryID tmpId,const char* tmpstr,MatchType tmpmtp,unsigned int tmpmd){

    
    job_node* new_node = new job_node(tmpId,tmpstr,tmpmtp,tmpmd);
    job_node* current = this->getLast();

    if (current == NULL){
        this->setFirst(new_node);
        this->setLast(new_node);
        return EC_SUCCESS;
    }

    current->setNext(new_node);
    this->setLast(new_node);
    return EC_SUCCESS;

}

ErrorCode job_list::job_insert_asc(QueryID tmpId,const char* tmpstr,MatchType tmpmtp,unsigned int tmpmd){

    job_node *new_node = new job_node(tmpId,tmpstr,tmpmtp,tmpmd);
    job_node *current = this->head;
    job_node *prev = NULL;

    // job_node* new_node = new job_node(tmpId);
    // job_node* current = this->getFirst();
    if (current == NULL){
        this->setFirst(new_node);
        counter++;
        return EC_SUCCESS;
    }
    while (current != NULL) {
        if (current->getId() > tmpId) {
            if (prev == NULL) {
                this->head = new_node;
                new_node->setNext(current);
                counter++;
                return EC_SUCCESS;
            }
            new_node->setNext(current);
            prev->setNext(new_node);
            counter++;
            return EC_SUCCESS;
        }
            prev = current;
            current = current->getNext();
    }
    prev->setNext(new_node);
    counter++;
    return EC_SUCCESS;
}