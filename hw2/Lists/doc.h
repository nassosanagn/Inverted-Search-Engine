#ifndef DOC_H
#define DOC_H
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "../sigmod2013contest-1.1/include/core.h"
#include "./entry.h"
using namespace std;

class doc {
    DocID doc_id;
    unsigned int num_res; 
    QueryID* query_ids;
    doc* next;

    public:

        doc(DocID did);

        /* Getters */
        DocID get_id() const{ return doc_id;}
        unsigned int get_num_res(){ return num_res;}
        QueryID* get_query_ids() const{ return query_ids;}
        doc* get_next() const{ return next;}

        /* Setters */
        void set_num_res(int tmp){num_res = tmp;}
        void set_query_ids(payload_list* tmp,int num);
        void set_next(doc *tmp){ next = tmp;}
};

class doc_list{
    doc* first;
    doc* last;

    public:

        //Dhmioyrgei thn doc list
        ErrorCode create_doc_list(doc_list** el);

        //Prosthetei ena doc sto telos ths listas
        doc* add_doc(doc_list* el, DocID did);
        doc* add_doc(doc_list* el, doc* D_temp, payload_list* pl);

        //epistrefei to prwto doc
        doc* get_first(const doc_list* el);
        doc* get_last(){ return last;};
        
        //Diagrafei kathe komvo ths listas
        ErrorCode destroy_doc_list(doc_list** el);
        
        doc* getfirst() const{ return first;}
        //Getters - Setters
        void setfirst(doc* tmp){ first = tmp;}
        void set_last(doc* tmp){ last = tmp;}

};

#endif



