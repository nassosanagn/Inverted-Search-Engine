#ifndef Q_HASHTABLE_H
#define Q_HASHTABLE_H
#include <iostream>
#include <stdlib.h>
#include "../hw1/main_folder/entry.h"
#define Q_SIZE 100000

class query_hash_node {
    QueryID query_id;
    word word_arr[MAX_QUERY_WORDS];
    int word_c[MAX_QUERY_WORDS];
	unsigned int words_found;
    unsigned int match_dist;
    unsigned int word_count;
    unsigned int curr_doc;
    int alive;
    query_hash_node* next;

    public:
        query_hash_node(QueryID qid,const char * str, unsigned int m_dist);

        QueryID get_id() const{
            return query_id;
        }
        word* get_word_arr(){
            return word_arr;
        }
        int get_alive(){
            return alive;
        }
        unsigned int get_dist() const{
            return match_dist;
        }
        unsigned int get_word_count() const{
            return word_count;
        }

        unsigned int get_word_found() const {
            return words_found;
        }

        int* get_word_c() {
            return word_c;
        }
        unsigned int get_curr_doc() {
            return curr_doc;
        }

        query_hash_node* get_next() const{
            return next;
        }
        void set_next(query_hash_node *tmp){
            next = tmp;
        }
        void set_curr_doc(unsigned int tmp){
            curr_doc = tmp;
        }
        void set_alive(){
            alive = 0;
        }
        void set_found(int x){
            word_c[x] = 1;
            words_found++;
        }
        void reset_val(){
            for(unsigned int i = 0; i < word_count; i++){
                word_c[i] = 0;
            }
            words_found = 0;
        }
};

class query_hash_list{
    query_hash_node* first;

    public:

        //Dhmioyrgei thn query list
        ErrorCode create_query_list(query_hash_list** el);

        ErrorCode delete_query(QueryID query_id);
        //Prosthetei ena query sto telos ths listas
        query_hash_node* add_query(query_hash_list* el, QueryID qid,const char * str,unsigned int m_dist);

        //epistrefei to prwto query
        query_hash_node* get_first(const query_hash_list* el);
        
        //Diagrafei kathe komvo ths listas
        ErrorCode destroy_query_list(query_hash_list** el);
        
        query_hash_node* search_id(unsigned int id);

        ErrorCode print_list(){
    
            if (first == NULL){
                return EC_FAIL;
            }
            query_hash_node* current = first;
            while (current != NULL){
                cout << "ID = "<<current->get_id() << " get_word_count: "<<current->get_word_count()<<" words found: "<<current->get_word_found()<<" CURR "<<current->get_curr_doc()<<" ";
                for(unsigned int i = 0 ;i<current->get_word_count();i++){
                    cout<<(current->get_word_c())[i]<<" ";
                }
                current = current->get_next();
            }
            cout << endl;
            return EC_SUCCESS;
        }
        //Getters - Setters
        query_hash_node* getfirst() const{
            return first;
        }

        void setfirst(query_hash_node* tmp){
            first = tmp;
        }
        // void delete_query(int id){
        //     query_hash_node *current = first;
        //     query_hash_node *previous = NULL;
        //     while (current != NULL) {
        //         if (current->get_id() == id) {
        //             if (previous == NULL) {
        //                 first = current->get_next();
        //             } else {
        //                 previous->set_next(current->get_next());
        //             }
        //             // delete current;
        //             break;
        //         }
        //         previous = current;
        //         current = current->get_next();
        //     }
        // }
};

class query_Hashtable {
    query_hash_list** buckets;
    int size;
    int counter;
    public:
        unsigned long hash_function(int id,int size_tmp);
        query_Hashtable();
        query_hash_node* insert(QueryID qid,const char * str,unsigned int m_dist);
        ErrorCode print();
        query_hash_node* search(QueryID qid);
    
        ErrorCode add_one(word* myword, int qid, unsigned int current_doc){
            
            query_hash_node* qNode;
            int func_out = hash_function(qid,size);
            qNode = buckets[func_out]->search_id(qid);           
            if(qNode->get_alive() == 0){
                return EC_FAIL;
            }
            if(qNode->get_curr_doc() != current_doc){
                qNode->set_curr_doc(current_doc);
                qNode->reset_val();
            }

            if (qNode->get_word_found() == qNode->get_word_count()){
                return EC_FAIL;
            }

            for(unsigned int i = 0; i < qNode->get_word_count(); i++){
                if ((!strcmp(((qNode->get_word_arr())[i]).getword(),myword->getword()) ) && ((qNode->get_word_c())[i] == 0)){
                    qNode->set_found(i);
                }
            }
            
            if (qNode->get_word_found() == qNode->get_word_count()){
                return EC_SUCCESS;
            }

            return EC_FAIL;
        }

        ErrorCode add_one_tree(word* myword, int qid, unsigned int current_doc, unsigned int threshold){
            query_hash_node* qNode;
            int func_out = hash_function(qid,size);
            qNode = buckets[func_out]->search_id(qid);           
            if(qNode->get_alive() == 0){
                return EC_FAIL;
            }
            if(qNode->get_curr_doc() != current_doc){
                qNode->set_curr_doc(current_doc);
                qNode->reset_val();
            }
            
            if (qNode->get_word_found() == qNode->get_word_count()){
                return EC_FAIL;
            }

            if(qNode->get_dist() < threshold){
                return EC_FAIL;
            }
            for(unsigned int i = 0; i < qNode->get_word_count(); i++){
                if ((!strcmp(((qNode->get_word_arr())[i]).getword(),myword->getword()) ) && ((qNode->get_word_c())[i] == 0)){
                    qNode->set_found(i);
                }
            }
            if (qNode->get_word_found() == qNode->get_word_count()){
                return EC_SUCCESS;
            }

            return EC_FAIL;
        }


        ErrorCode add_one_payload(payload_list* pl,word* w,int current_doc,int threshold,payload_list* q_result){
            payload_node* pn;
            pn = pl->getFirst();
            while(pn!=NULL){
                if(add_one_tree(w,pn->getId(),current_doc,threshold) == EC_SUCCESS){
                    q_result->payload_insert_asc(pn->getId());
                }
                pn = pn->getNext();
            }

            return EC_SUCCESS;
        }

        ErrorCode delete_query(int qid){
            query_hash_node* qNode;
            int func_out = hash_function(qid,size);
            qNode = buckets[func_out]->search_id(qid);
            qNode->set_alive();

            return EC_SUCCESS;
        }
};



#endif