#ifndef Q_SATISFIED_H
#define Q_SATISFIED_H

#include <iostream>
#include <stdlib.h>

#include "./sigmod2013contest-1.1/include/core.h"


class query_sat_node{
    
    int word_c[MAX_QUERY_WORDS];
    unsigned int doc_id;
    unsigned int words_found;

    query_sat_node* next;

    public:

        query_sat_node(unsigned int doc_id){
        
            words_found = 0;
            this->doc_id = doc_id;

            for (int i = 0; i< MAX_QUERY_WORDS; i++){
                word_c[i] = 0;
            }
            this->next = NULL;
        }

        int* get_word_c() { return word_c;}
        unsigned int get_doc_id() const{ return doc_id;}
        unsigned int get_word_found() const { return words_found;}
        query_sat_node* get_next() const{ return next;}

        void set_next(query_sat_node *tmp){ next = tmp;}
        void set_doc_id(unsigned int tmp){ doc_id = tmp;}


        void update_node(int x){
            this->word_c[x] = 1;
            this->words_found++;
        }
};


class query_sat_list {

    query_sat_node* head;
    query_sat_node* last;

    public:

        query_sat_list(){
            this->head = NULL;
            this->last = NULL;
        }

        ErrorCode update(unsigned int doc_id, int x){
            query_sat_node* qn = head;

            while(qn!=NULL){
                if(qn->get_doc_id() == doc_id){   // update that doc

                    qn->update_node(x);
                    return EC_SUCCESS;
                }
                qn = qn->get_next();
            }
            return EC_FAIL;
        }

        unsigned int get_word_found(unsigned int doc_id){
            query_sat_node* qn = head;

            while(qn!=NULL){
                if(qn->get_doc_id() == doc_id){   // update that doc

                   return qn->get_word_found();
                }
                qn = qn->get_next();
            }
            return 999;
        }

        int* get_word_c(unsigned int doc_id){
            query_sat_node* qn = head;

            while(qn!=NULL){
                if(qn->get_doc_id() == doc_id){   // update that doc

                   return qn->get_word_c();
                }
                qn = qn->get_next();
            }
            return NULL;
        }

        query_sat_node* query_sat_add(unsigned int doc_id){

            query_sat_node* new_node = new query_sat_node(doc_id);
            query_sat_node* current = this->get_last();

            if (current == NULL){
                this->set_first(new_node);
                this->set_last(new_node);
                return new_node;
            }

            current->set_next(new_node);
            this->set_last(new_node);
            return new_node;

        }

        void print_list(){
    
            if (head == NULL){
                return;
            }

            query_sat_node* current = this->get_first();

            while (current != NULL){
                cout << current->get_doc_id() << " ";
                current = current->get_next();
            }

            cout << endl;
        }
      
        query_sat_node* search_doc_id(unsigned int doc_id){
            query_sat_node* qn = head;
            while(qn!=NULL){
                if(qn->get_doc_id() == doc_id){
                    return qn;
                }
                qn = qn->get_next();
            }
            return NULL;
        }

        /* Getters */
        query_sat_node* get_first() const{ return head;}
        query_sat_node* get_last() const{ return last;}

        /* Setters */
        void set_first(query_sat_node* tmp){ head = tmp; }
        void set_last(query_sat_node* tmp){ last = tmp; }
};

#endif