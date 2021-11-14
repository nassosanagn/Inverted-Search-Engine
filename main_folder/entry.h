#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "core.h"
using namespace std;

class word {
    char* String;
    public:
        word(char* tmp);
        ~word();
        word(const word &ww);
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

class entry {
    word* myString;
    void* payload;
    entry* next;

    public:
        entry(char * tmp,void *pload=NULL);
        ~entry();
        ErrorCode create_entry(const word* w, entry** e);
        ErrorCode destroy_entry(entry **e);
        entry(const entry &ee);
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

class entry_list{
    entry* current;
    entry* first;
    entry* last;
    public:
        ErrorCode create_entry_list(entry_list** el);

        unsigned int get_number_entries(const entry_list* el);
        
        ErrorCode add_entry(entry_list* el, const entry* e);

        entry* get_first(const entry_list* el);

        entry* get_next(const entry_list* el,const entry* e);
        
        ErrorCode destroy_entry_list(entry_list** el);
        
        ErrorCode print_list(entry_list *el);

        ErrorCode list_similarity(entry_list *el1,entry_list *el2);

        entry* getfirst() const{
            return first;
        }
        entry* getcurrent() const{
            return current;
        }
        void setfirst(entry* tmp){
            first = tmp;
        }
        void setcurrent(entry* tmp){
            current = tmp;
        }
};







