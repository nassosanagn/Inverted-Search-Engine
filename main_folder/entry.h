#ifndef ENTRY_H
#define ENTRY_H
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "core.h"
using namespace std;

//Struct word poy leitoyrgei ws String
class word {

    char* String;

    public:

        //Constructor - Destructor
        word(char* tmp);
        ~word();

        //Copy contructor
        word(const word &ww);

        //Setters - Getters
        void setword(const char *ww){
            if(String!=NULL){
                delete[] String;
            }
            String = new char[strlen(ww)+1];
            strcpy(String,ww);
        }
        char* getword() const{ 
            return String; 
        }
};

//Struct entry poy antistoixei se mia le3h
class entry {

    word* myString;
    void* payload;
    entry* next;

    public:

        //Constructor - Destructor
        entry(char * tmp,void *pload=NULL);
        ~entry();

        //Dhmioyrgia ths domhs
        ErrorCode create_entry(const word* w, entry** e);

        //Diagrafei thn domh
        ErrorCode destroy_entry(entry **e);

        //Copy constructor
        entry(const entry &ee);

        //Getters - Setters
        char* getword() const{
            return myString->getword();
        }
        void* getpayload() const{
            return payload;
        }
        entry* getnext() const{
            return next;
        }
        void setnext(entry *tmp){
            next = tmp;
        }
        void setword(char *tmp){
            myString->setword(tmp);
        }
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
        ErrorCode add_entry(entry_list* el, const entry* e);

        //epistrefei to prwto entry
        entry* get_first(const entry_list* el);

        //Epistrefei ton epomeno komvo apo ayton sto orisma
        entry* get_next(const entry_list* el,const entry* e);
        
        //Diagrafei kathe komvo ths listas
        ErrorCode destroy_entry_list(entry_list** el);
        
        //Ektypwnei kathe komvo
        ErrorCode print_list(entry_list *el);

        //Epistrefei success otan oi listes einai karivws idies
        ErrorCode list_similarity(entry_list *el1,entry_list *el2);
        
        //Getters - Setters
        entry* getfirst() const{
            return first;
        }
        entry* getlast() const{
            return last;
        }
        void setfirst(entry* tmp){
            first = tmp;
        }
        void setlast(entry* tmp){
            last = tmp;
        }
};

#endif



