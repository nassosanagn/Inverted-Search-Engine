#include "entry.h"
using namespace std;

enum ::error_code create_entry(const word* w, entry** e){
    (*e) = new entry(w->getword());
    if((*e)==NULL){
        return FAIL;
    }
    return SUCCESS;
}

enum ::error_code destroy_entry(entry **e){
    delete (*e);
    return SUCCESS;
}

enum ::error_code create_entry_list(entry_list** el){
    (*el) = new entry_list;
    if((*el)==NULL){
        return FAIL;
    }
    return SUCCESS;
}

unsigned int get_number_entries(const entry_list* el){
    unsigned int counter = 0;
    if(el == NULL){
        return 0;
    }
    entry* curr = el->getfirst();
    while (curr != NULL) 
    { 
        counter++; 
        curr = curr->getnext();
    } 
    return counter; 
}

enum ::error_code add_entry(entry_list* el, const entry* e){
    if(el == NULL || e == NULL){
        return FAIL;
    }
    entry* entry_n = new entry(e->getword(),e->getpayload());
    entry* entry_tmp = el->getfirst();

    if(el->getfirst() == NULL){
        el->setfirst(entry_n);
        el->setcurrent(entry_n);
        return SUCCESS;
    }

    while(entry_tmp->getnext() != NULL){
        entry_tmp = entry_tmp->getnext();
    }
    
    entry_tmp->setnext(entry_n);
    return SUCCESS;
}

entry* get_first(const entry_list* el){
    return el->getfirst();
}

entry* get_next(const entry_list* el){
    return el->getcurrent()->getnext();
}

enum ::error_code destroy_entry_list(entry_list** el){
    if((*el)==NULL){
        return FAIL;
    }
    entry* curr = (*el)->getfirst();
    entry* next = NULL;
    while(curr != NULL){
        next = curr->getnext();
        delete curr;
        curr = next;
    }
    delete (*el);
    (*el) = NULL;
    return SUCCESS;
}



