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
        cout << current->getId() << " "<<current->getjtype()<<"  ";
        current = current->getNext();
    }

    cout << endl;
    return EC_SUCCESS;
}

job_node* job_list::job_pop(){
    if(head == NULL){
        return NULL;
    }
    counter--;
    job_node* temp = head->getNext();
    job_node* return_val = head;
    head = temp;
    if(temp==NULL){
        last = NULL;
    }
    return return_val;
}

job_node* job_list::job_insert(QueryID tmpId,const char* tmpstr,MatchType tmpmtp,unsigned int tmpmd,JobType tmpjtype){
    
    job_node* new_node = new job_node(tmpId,tmpstr,tmpmtp,tmpmd,tmpjtype);
    job_node* current = this->getLast();

    if (current == NULL){
        this->setFirst(new_node);
        this->setLast(new_node);
        counter = 1;
        return new_node;
    }
    current->setNext(new_node);
    this->setLast(new_node);
    counter++;
    return new_node;

}

ErrorCode job_list::job_insert_asc(QueryID tmpId,const char* tmpstr,MatchType tmpmtp,unsigned int tmpmd,JobType tmpjtype){

    job_node *new_node = new job_node(tmpId,tmpstr,tmpmtp,tmpmd,tmpjtype);
    job_node *current = this->head;
    job_node *prev = NULL;

    // job_node* new_node = new job_node(tmpId);
    // job_node* current = this->getFirst();
    if (current == NULL){
        counter = 1;
        this->setFirst(new_node);
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


// int main(){
//     job_list* jl = new job_list();
//     jl->job_insert(8008,"query_str",MT_EXACT_MATCH,1,BARRIER);
//     jl->job_insert(9999,"query_str",MT_EXACT_MATCH,1,BARRIER);
//     jl->print_list();
//     job_node* jb;
//     jb = jl->job_pop();
//     cout<<jb->getId()<<endl;
//     jb = jl->job_pop();
//     cout<<jb->getId()<<endl;   
//     if(jl->getFirst() == NULL){
//         cout << "is bnull"<<endl;
//     }
//     jl->job_insert(111,"query_str",MT_EXACT_MATCH,1,BARRIER);
//     if(jl->getFirst() == NULL){
//         cout << "is bnull??"<<endl;
//     }
//     jl->print_list();
//     return 1;
// }