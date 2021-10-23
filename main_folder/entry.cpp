#include "entry.h"
using namespace std;

ErrorCode entry::create_entry(const word* w, entry** e){
    (*e) = new entry(w->getword());
    if((*e)==NULL){
        return EC_FAIL;
    }
    return EC_SUCCESS;
}

ErrorCode entry::destroy_entry(entry **e){
    delete (*e);
    return EC_SUCCESS;
}

ErrorCode entry_list::create_entry_list(entry_list** el){
    (*el) = new entry_list;
    if((*el)==NULL){
        return EC_FAIL;
    }
    return EC_SUCCESS;
}

unsigned int entry_list::get_number_entries(const entry_list* el){
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

ErrorCode entry_list::add_entry(entry_list* el, const entry* e){
    if(el == NULL || e == NULL){
        return EC_FAIL;
    }
    entry* entry_n = new entry(e->getword(),e->getpayload());
    entry* entry_tmp = el->getfirst();

    if(el->getfirst() == NULL){
        el->setfirst(entry_n);
        el->setcurrent(entry_n);
        return EC_SUCCESS;
    }

    while(entry_tmp->getnext() != NULL){
        entry_tmp = entry_tmp->getnext();
    }
    
    entry_tmp->setnext(entry_n);
    return EC_SUCCESS;
}

entry* entry_list::get_first(const entry_list* el){
    return el->getfirst();
}

entry* entry_list::get_next(const entry_list* el){
    return el->getcurrent()->getnext();
}

ErrorCode entry_list::destroy_entry_list(entry_list** el){
    if((*el)==NULL){
        return EC_FAIL;
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
    return EC_SUCCESS;
}



