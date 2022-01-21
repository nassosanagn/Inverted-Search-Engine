#ifndef Q_HASHTABLE_H
#define Q_HASHTABLE_H

#include <iostream>
#include <stdlib.h>
#include "./Lists/entry.h"
#include "./q_satisfied.h"
#define Q_SIZE 100000

class query_hash_node {

    QueryID query_id;
    word word_arr[MAX_QUERY_WORDS];
    // int word_c[MAX_QUERY_WORDS];

	// unsigned int words_found;
    query_sat_node* query_arr;
    unsigned int match_dist;
    unsigned int word_count;
    // unsigned int curr_doc;
    int alive;
    query_hash_node* next;

    public:
    
        query_hash_node(QueryID qid,const char * str, unsigned int m_dist);

        /* Getters */
        QueryID get_id() const{ return query_id;}
        word* get_word_arr(){ return word_arr;}

        query_sat_list* get_q_sat_list(){ return q_sat_list;}

        int get_alive(){ return alive;}

        query_sat_node* get_arr(){ return query_arr;}

        unsigned int get_dist() const{ return match_dist;}
        unsigned int get_word_count() const{ return word_count;}
        // unsigned int get_word_found() const { return words_found;}
        // int* get_word_c() { return word_c;}
        // unsigned int get_curr_doc() { return curr_doc;}
        query_hash_node* get_next() const{ return next;}

        /* Setters */
        void set_next(query_hash_node *tmp){ next = tmp;}
        // void set_curr_doc(unsigned int tmp){ curr_doc = tmp;}
        void set_alive(){ alive = 0;}
        // void set_found(int x){ word_c[x] = 1; words_found++;}

        ErrorCode set_found(unsigned int doc_id, int x){           
            return this->get_q_sat_list()->update(doc_id,x);
        }
               
        void reset_val();
};

class query_hash_list{

    query_hash_node* first;

    public:

        ErrorCode delete_query(QueryID query_id);
        // ErrorCode print_list();
        //Diagrafei kathe komvo ths listas
        ErrorCode destroy_query_list(query_hash_list** el);

        //Prosthetei ena query sto telos ths listas
        query_hash_node* add_query(query_hash_list* el, QueryID qid,const char * str,unsigned int m_dist);
        //epistrefei to prwto query
        query_hash_node* get_first(const query_hash_list* el);

        query_hash_node* search_id(unsigned int id){
            query_hash_node* qn = first;
            while(qn!=NULL){
                if(qn->get_id() == id){
                    return qn;
                }
                qn = qn->get_next();
            }
            return NULL;
        }

        /* Getters */
        query_hash_node* getfirst() const{ return first;}

        /* Setters */
        void setfirst(query_hash_node* tmp){ first = tmp;}
};

class query_Hashtable {

    query_hash_list** buckets;
    int size;
    int counter;

    public:

        query_Hashtable();
        ~query_Hashtable();

        unsigned long hash_function(int id,int size_tmp);
        
        query_hash_node* insert(QueryID qid,const char * str,unsigned int m_dist);
        query_hash_node* search(QueryID qid);
        // ErrorCode print();
    
        ErrorCode add_one(word* myword, int qid, unsigned int current_doc,int thread_id);
        ErrorCode add_one_tree(word* myword, int qid, unsigned int current_doc, unsigned int threshold,int thread_id);         
        ErrorCode add_one_payload(payload_list* pl,word* w, int current_doc, int threshold, payload_list* q_result,int thread_id);

        ErrorCode delete_query(int qid);
};


#endif