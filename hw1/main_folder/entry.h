#ifndef ENTRY_H
#define ENTRY_H

#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "core.h"
#include "../../hw2/payload.h"

using namespace std;

//Struct word poy leitoyrgei ws String
class word {

    char* String;

    public:
        //Constructor - Destructor
        word(char* tmp=NULL);
        ~word();

        /* Getters */
        char* getword() const{ return String; }

        /* Setters */
        void setword(const char *ww);
};

//Struct entry poy antistoixei se mia le3h
class entry {

    word* myString;
    payload_list* list;
    entry* next;

    public:

        //Constructor - Destructor
        entry(char * tmp);
        ~entry();

        //Dhmioyrgia ths domhs
        ErrorCode create_entry(const word* w, entry** e);
        ErrorCode destroy_entry(entry **e);
        payload_node* search_payload(int id);

        /* Getters */
        char* getword() const    { return myString->getword(); }
        word* getmyword() const {return myString;}
        payload_list* getpayload() const { return list; }
        entry* getnext() const   { return next; }
        
        /* Setters */
        void setnext(entry *tmp) { next = tmp; }
        void setword(char *tmp)  { myString->setword(tmp); }
        void setword(word *W)    {myString->setword(W->getword()); }
        void setpayload(payload_list* pl);
};

//Lista apo komvoys entries
class entry_list{

    //Deixnei ston prwto kai teleytaio komvo
    entry* first;
    entry* last;

    public:
        //Dhmioyrgei thn entry list
        ErrorCode create_entry_list(entry_list** el);

        //Epistrefei ton arithmo komvwn
        unsigned int get_number_entries(const entry_list* el);
        
        //Prosthetei ena entry sto telos ths listas
        ErrorCode add_entry(entry_list* el, const entry* e,int id);

        //Epistrefei ton epomeno komvo apo ayton sto orisma
        entry* get_next(const entry_list* el,const entry* e);
        
        //Diagrafei kathe komvo ths listas
        ErrorCode destroy_entry_list(entry_list** el);
        
        ErrorCode destroy_entrys(entry_list** el);
        //Ektypwnei kathe komvo
        ErrorCode print_list(entry_list *el);

        //Epistrefei success otan oi listes einai karivws idies
        ErrorCode list_similarity(entry_list *el1,entry_list *el2);
        
        entry* search_word(word* W){
            entry * e = first;
            while(e!=NULL){
                if(!strcmp(e->getword(),W->getword())){
                    return e;
                }
                e = e->getnext();
            }
            return NULL;
        }

        //Getters - Setters
        entry* getlast() const{ return last; }
        entry* getfirst() const{ return first; }
        entry* get_first(const entry_list* el);

        void setfirst(entry* tmp){ first = tmp; }
        void setlast(entry* tmp){ last = tmp; }
};
#endif



