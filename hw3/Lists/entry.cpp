#include "entry.h"
using namespace std;

/* ---------------------------------------------------------------- word functions ---------------------------------------------------------------- */

word::word(char* tmp){
    if(tmp==NULL){
        String = NULL;
        return;
    }
    String = new char[strlen(tmp)+1];
    strcpy(String,tmp);
}

word::~word(){
    delete[] String;
}

void word::setword(const char *ww){
    if(String!=NULL){
        delete[] String;
    }
    String = new char[strlen(ww)+1];
    strcpy(String,ww);
}

/* ---------------------------------------------------------------- entry functions ---------------------------------------------------------------- */

//contructor
entry::entry(char * tmp){
    myString = new word(tmp);
    list = new payload_list();
    next = NULL;
}
//destructor
entry::~entry(){
    delete myString;
}

//Dhmioyrgei neo entry
ErrorCode entry::create_entry(const word* w, entry** e){
    (*e) = new entry(w->getword());
    if((*e)==NULL){
        return EC_FAIL;
    }
    return EC_SUCCESS;
}

payload_node* entry::search_payload(int id){
    payload_node* pn = list->getFirst();
    while(pn!=NULL){
        if(pn->getId()==id){
            return pn;
        }
        pn = pn->getNext();
    }
    return NULL;
}

//diagrafei entry
ErrorCode entry::destroy_entry(entry **e){
    delete (*e);
    *e = NULL;
    return EC_SUCCESS;
}

void entry::setpayload(payload_list* pl){
    payload_node* pn = pl->getFirst();
    while(pn!=NULL){
        list->payload_insert(pn->getId());
        pn = pn->getNext();
    }
}


/* ---------------------------------------------------------------- entry_list functions ---------------------------------------------------------------- */

//Dhmioyrgei to entry list
ErrorCode entry_list::create_entry_list(entry_list** el){
    (*el) = new entry_list;
    (*el)->setfirst(NULL);
    if((*el)==NULL){
        return EC_FAIL;
    }
    return EC_SUCCESS;
}

//Ektypwnei kathe komvo ths listas
ErrorCode entry_list::print_list(entry_list *el){
    if(el == NULL){
        return EC_FAIL;
    }
    //Diatrexei toys komvoys ths listas kai toys ektypwnei
    entry* entry_tmp = el->getfirst();
    while(entry_tmp != NULL){
        cout<<entry_tmp->getword()<<" ";
        // entry_tmp->getpayload()->print_list();
        // cout<<endl;
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
    //Diatrexei toys komvoys ths listas kai au3anei counter
    entry* curr = el->getfirst();
    while (curr != NULL) 
    { 
        counter++; 
        curr = curr->getnext();
    } 
    return counter; 
}

//Prosthetei ena entry sto telos ths listas
ErrorCode entry_list::add_entry(entry_list* el, const entry* e,int id){
    if(el == NULL || e == NULL){
        return EC_FAIL;
    }
    //Dhmioyrgei ton komvo poy tha prosthethei
    entry* entry_n = new entry(e->getword());
    if(id == -1){
        payload_node* pn = e->getpayload()->getFirst();
        while(pn!=NULL){
            entry_n->getpayload()->payload_insert(pn->getId());
            pn = pn->getNext();
        }
    }
    else
        entry_n->getpayload()->payload_insert(id);
    //An den yparxei allos komvos
    if(el->getfirst() == NULL){
        el->setfirst(entry_n);
        el->setlast(entry_n);
        return EC_SUCCESS;
    }
    el->getlast()->setnext(entry_n);
    el->setlast(entry_n);
    return EC_SUCCESS;
}

entry* entry_list::get_first(const entry_list* el){
    return el->getfirst();
}

entry* entry_list::get_next(const entry_list* el,const entry * e){
    entry* curr = el->getfirst();
    while(curr != NULL){
        //Vriskei thn le3h kleidi kai epistrefei ton epomeno
        if(!strcmp(curr->getword(),e->getword())){
            return curr->getnext();
        }
        curr = curr->getnext();
    } 
    return NULL;
}

//Diatrexei thn lista gia na dei an einai akrivws to idio
ErrorCode entry_list::list_similarity(entry_list* el1,entry_list* el2){
    entry* curr1 = el1->getfirst();
    entry* curr2 = el2->getfirst();
    while(curr1!=NULL && curr2!=NULL){
        //An vrei diaforetikh le3h termatizei 
        if(strcmp(curr1->getword(),curr2->getword())){
            return EC_FAIL;
        }
        curr1 = curr1->getnext();
        curr2 = curr2->getnext();
    }
    return EC_SUCCESS;
}

//Diagrafei kathe komvo ths listas
ErrorCode entry_list::destroy_entry_list(entry_list** el){
    if((*el)==NULL){
        return EC_FAIL;
    }
    entry* curr = (*el)->getfirst();
    entry* next = NULL;
    //Diatrexei thn lista kai diagrafei toys komvoys
    while(curr != NULL){
        next = curr->getnext();

        delete curr;

        curr = next;
    }
    delete (*el);
    (*el) = NULL;
    return EC_SUCCESS;
}


ErrorCode entry_list::destroy_entrys(entry_list** el){
    if((*el)==NULL){
        return EC_FAIL;
    }
    entry* curr = (*el)->getfirst();
    entry* next = NULL;
    //Diatrexei thn lista kai diagrafei toys komvoys
    while(curr != NULL){
        next = curr->getnext();

        delete curr;

        curr = next;
    }
    return EC_SUCCESS;
}

/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------- */