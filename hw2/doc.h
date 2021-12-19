#ifndef DOC_H
#define DOC_H
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "sigmod2013contest-1.1/include/core.h"
#include "../hw1/main_folder/entry.h"
using namespace std;

class doc {
    DocID doc_id;
    unsigned int num_res;
    QueryID* query_ids;
    doc* next;

    public:
        doc(DocID did);

        DocID get_id() const{
            return doc_id;
        }
        unsigned int get_num_res(){
            return num_res;
        }
        QueryID* get_query_ids() const{
            return query_ids;
        }
        doc* get_next() const{
            return next;
        }
        void set_num_res(int tmp){
            num_res = tmp;
        }
        void set_query_ids(payload_list* tmp,int num){
            query_ids = new QueryID[num];
            payload_node* P = tmp->getFirst();
            int i = 0;
            while(P!=NULL){
                query_ids[i] = P->getId();
                i++;
                P = P->getNext();
            }
        }
        void set_next(doc *tmp){
            next = tmp;
        }
};

class doc_list{
    doc* first;
    doc* last;
    public:

        //Dhmioyrgei thn doc list
        ErrorCode create_doc_list(doc_list** el);

        //Prosthetei ena doc sto telos ths listas
        doc* add_doc(doc_list* el, DocID did);

        doc* add_doc(doc_list* el, doc* D_temp,payload_list* pl);
        //epistrefei to prwto doc
        doc* get_first(const doc_list* el);
        
        //Diagrafei kathe komvo ths listas
        ErrorCode destroy_doc_list(doc_list** el);
        
        //Getters - Setters
        doc* getfirst() const{
            return first;
        }

        void setfirst(doc* tmp){
            first = tmp;
        }
        doc* getlast() const{
            return last;
        }

        void setlast(doc* tmp){
            last = tmp;
        }

};

#endif



