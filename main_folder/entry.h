#include <cstring>
#include <stdlib.h>
#include <iostream>
using namespace std;

typedef enum error_c
{
    SUCCESS = 0,
    FAIL = 1,
    OTHER = 2
}error_c;

class word {
    char* String;
    public:
        word(char* tmp){
            String = new char[strlen(tmp)]();
            strcpy(String,tmp);
        }
        ~word(){
            delete String;
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
        entry(char * tmp,void *pload = NULL){
            myString = new word(tmp);
            payload = pload;
            next = NULL;
        }
        ~entry(){
            delete myString;
        }
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
};

class entry_list{
    entry* current;
    entry* first;
    entry* last;
    public:
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

error_c create_entry(const word* w, entry** e);

error_c destroy_entry(entry **e);

error_c create_entry_list(entry_list** el);

unsigned int get_number_entries(const entry_list* el);

error_c add_entry(entry_list* el, const entry* e);

entry* get_first(const entry_list* el);

entry* get_next(const entry_list* el);

error_c destroy_entry_list(entry_list** el);