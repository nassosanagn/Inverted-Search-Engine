#ifndef QUERY_H
#define QUERY_H
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "sigmod2013contest-1.1/include/core.h"
#include "../hw1/main_folder/entry.h"
using namespace std;

class query {
    QueryID query_id;
    word word_arr[MAX_QUERY_WORDS];
	MatchType match_type;
	unsigned int match_dist;
    unsigned int word_count;
    query* next;

    public:
        query(QueryID qid,const char * str,MatchType m_type,unsigned int m_dist);

        QueryID get_id() const{
            return query_id;
        }
        word* get_word_arr(){
            return word_arr;
        }
        MatchType get_match_type() const{
            return match_type;
        }
        unsigned int get_dist() const{
            return match_dist;
        }
        unsigned int get_word_count() const{
            return word_count;
        }
        query* get_next() const{
            return next;
        }
        void set_next(query *tmp){
            next = tmp;
        }
};

class query_list{
    query* first;

    public:

        //Dhmioyrgei thn query list
        ErrorCode create_query_list(query_list** el);

        ErrorCode delete_query(QueryID query_id);
        //Prosthetei ena query sto telos ths listas
        query* add_query(query_list* el, QueryID qid,const char * str,MatchType m_type,unsigned int m_dist);

        //epistrefei to prwto query
        query* get_first(const query_list* el);
        
        //Diagrafei kathe komvo ths listas
        ErrorCode destroy_query_list(query_list** el);
        
        ErrorCode print_list(){
    
            if (first == NULL){
                return EC_FAIL;
            }

            query* current = first;

            while (current != NULL){
                cout << current->get_id() << " ";
                current = current->get_next();
            }

            cout << endl;
            return EC_SUCCESS;
        }
        //Getters - Setters
        query* getfirst() const{
            return first;
        }

        void setfirst(query* tmp){
            first = tmp;
        }

};

#endif



