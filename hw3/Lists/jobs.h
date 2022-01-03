#ifndef JOBS_H
#define JOBS_H

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include "../sigmod2013contest-1.1/include/core.h"

using namespace std;

class job_node {

    QueryID id;
    char* str;
    MatchType match_type;
    unsigned int match_dist;
    job_node* next;

    public:

        /* Constructor - Destructor */
        job_node(QueryID tmpId,const char* tmpstr,MatchType tmpmtp,unsigned int tmpmd) {
            this->id = tmpId;
            this->str = new char[strlen(tmpstr)+1];
			strcpy(this->str, tmpstr);
            this->match_type = tmpmtp;
            this->match_dist = tmpmd;
            this->next = NULL;
        }

        /* Getters */
        int getId() const    { return this->id; }
        char* getstr() const    { return this->str; }
        MatchType getmatch_type() const    { return this->match_type; }
        unsigned int getmatch_dist() const    { return this->match_dist; }
        job_node* getNext() const { return this->next; }
   
        /* Setters */
        void setNext(job_node* tmp) { this->next = tmp; }
        void setId(int tmpId) { this->id = tmpId; }
};

class job_list{

    job_node* head;
    job_node* last;
    int counter;

    public:
        
        job_list();
        ~job_list();

        ErrorCode print_list();
        ErrorCode job_insert(QueryID tmpId,const char* tmpstr,MatchType tmpmtp,unsigned int tmpmd);
        ErrorCode job_insert_asc(QueryID tmpId,const char* tmpstr,MatchType tmpmtp,unsigned int tmpmd);
        ErrorCode destroy_job_list();

        ErrorCode search_id(int id){
            job_node* n = head;
            while(n!=NULL){
                if(n->getId() == id){
                    return EC_SUCCESS;
                }
                n = n->getNext();
            }
            return EC_FAIL;
        }

        /* Getters */
        job_node* getFirst() const{ return head;}
        job_node* getLast() const{ return last;}
        int get_counter() const{return counter;}

        /* Setters */
        void setFirst(job_node* tmp){ head = tmp; }
        void setLast(job_node* tmp){ last = tmp; }
        
};

#endif