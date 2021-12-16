#ifndef Q_HASHTABLE_H
#define Q_HASHTABLE_H
#include <iostream>
#include <stdlib.h>
#include "../hw1/main_folder/entry.h"
#define Q_SIZE 100

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
};

class query_hash_node {
    QueryID query_id;
    word word_arr[MAX_QUERY_WORDS];
	unsigned int match_dist;
    unsigned int word_count;
    query_hash_node* next;

    public:
        query_hash_node(QueryID qid,const char * str,unsigned int m_dist);

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
        query_hash_node* get_next() const{
            return next;
        }
        void set_next(query_hash_node *tmp){
            next = tmp;
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
        
        query_hash_node* search_id(int id);

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

#endif