#include "entry.h"
using namespace std;


word::word(char* tmp){
    String = new char[strlen(tmp)+1];
    strcpy(String,tmp);
    // String = strdup(tmp);
}

word::~word(){
    delete[] String;
}

word::word(const word &ww){
    delete[] this->String;
    String = new char[strlen(ww.String)+1];
    strcpy(String,ww.String);
    // this->String = strdup(ww.String);
}

entry::entry(const entry &ee){
    myString->setword(ee.getword());
    payload = ee.getpayload();
    next = ee.getnext();
}

entry::entry(char * tmp,void *pload){
    myString = new word(tmp);
    payload = pload;
    next = NULL;
}

entry::~entry(){
    delete myString;
}

ErrorCode entry::create_entry(const word* w, entry** e){
    (*e) = new entry(w->getword());
    if((*e)==NULL){
        return EC_FAIL;
    }
    return EC_SUCCESS;
}

ErrorCode entry::destroy_entry(entry **e){
    delete (*e);
    *e = NULL;
    return EC_SUCCESS;
}

ErrorCode entry_list::create_entry_list(entry_list** el){
    (*el) = new entry_list;
    if((*el)==NULL){
        return EC_FAIL;
    }
    return EC_SUCCESS;
}
ErrorCode entry_list::print_list(entry_list *el){
    if(el == NULL){
        return EC_FAIL;
    }
    entry* entry_tmp = el->getfirst();
    while(entry_tmp != NULL){
        cout<<entry_tmp->getword()<<" ";
        entry_tmp = entry_tmp->getnext();
    }
    cout<<endl;
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

entry* entry_list::get_next(const entry_list* el,const entry * e){
    entry* curr = el->getfirst();
    while(curr != NULL){
        if(!strcmp(curr->getword(),e->getword())){
            return curr->getnext();
        }
        curr = curr->getnext();
    } 
    return NULL;
}

ErrorCode entry_list::list_similarity(entry_list* el1,entry_list* el2){
    entry* curr1 = el1->getfirst();
    entry* curr2 = el2->getfirst();
    while(curr1!=NULL && curr2!=NULL){
        if(strcmp(curr1->getword(),curr2->getword())){
            return EC_FAIL;
        }
        curr1 = curr1->getnext();
        curr2 = curr2->getnext();
    }
    return EC_SUCCESS;
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


