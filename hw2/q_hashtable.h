#ifndef Q_HASHTABLE_H
#define Q_HASHTABLE_H
#include <iostream>
#include <stdlib.h>
#include "../hw1/main_folder/entry.h"
#define Q_SIZE 100

class query_hash_node {
    QueryID query_id;
    word word_arr[MAX_QUERY_WORDS];
    int word_c[MAX_QUERY_WORDS];
	int words_found;
    unsigned int match_dist;
    unsigned int word_count;

    query_hash_node* next;

    public:
        query_hash_node(QueryID qid,const char * str, unsigned int m_dist);

        QueryID get_id() const{
            return query_id;
        }
        word* get_word_arr(){
            return word_arr;
        }
        unsigned int get_dist() const{
            return match_dist;
        }
        unsigned int get_word_count() const{
            return word_count;
        }

        int get_word_found() const {
            return words_found;
        }

        int* get_word_c() {
            return word_c;
        }




        query_hash_node* get_next() const{
            return next;
        }
        void set_next(query_hash_node *tmp){
            next = tmp;
        }

        void set_found(int x){
            word_c[x] = 1;
            words_found++;
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
                cout << current->get_id() << " ";
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

};

class query_Hashtable {
    query_hash_list** buckets;
    int size;
    int counter;
    public:
        unsigned long hash_function(int id,int size_tmp);
        query_Hashtable();
        ErrorCode insert(QueryID qid,const char * str,unsigned int m_dist);
        ErrorCode print();
        query_hash_node* search(QueryID qid);

        ErrorCode add_one(word* myword, int qid){
            
            query_hash_node* qNode;
            int func_out = hash_function(qid,size);
            qNode = buckets[func_out]->search_id(qid);           

            for(int i = 0; i < qNode->get_word_count(); i++){

                if (!strcmp(((qNode->get_word_arr())[i]).getword(),myword->getword()) ){
                    qNode->set_found(i);
                }
            }
            
            if (qNode->get_word_found() == qNode->get_word_count()){
                return EC_SUCCESS;
            }

            return EC_FAIL;
        }
};



#endif