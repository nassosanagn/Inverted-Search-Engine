/*
 * core.cpp version 1.0
 * Copyright (c) 2013 KAUST - InfoCloud Group (All Rights Reserved)
 * Author: Amin Allam
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "../include/core.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include "../Lists/jobs.h"
#include "../Lists/doc.h"
#include "../Lists/payload.h"
#include "../Indexes/hashtable.h"
#include "../Indexes/hammingindex.h"
#include "../Indexes/editDistBkTree.h"

#include "../q_hashtable.h"


#define END_DOC 960
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////

query_Hashtable* Q_hash;
doc_list* D_list;
HammingIndex* ham_index;
Hashtable* hash_index;
EditBKTree* edit_index;

doc* D_tmp;

int status[NUM_THREADS];
int id_assign[NUM_THREADS];

void print_status();
int realid(int thread_self);

int br_type;
int flag = 1;
int flag_q;

// thread data
class job_scheduler{
   public:
      job_list* j_list;
};

QueryID end_flg[3];

pthread_cond_t cond_nonempty;
pthread_mutex_t mutex_end[3];
pthread_mutex_t mutexD;
pthread_mutex_t mutexq;
pthread_mutex_t mutexAR;
pthread_mutex_t mutexAR2;
pthread_mutex_t mutexcout;
pthread_mutex_t mutexif;

pthread_mutex_t mutexqhash;
pthread_mutex_t mutexedit;
pthread_mutex_t mutexhamm;
pthread_mutex_t mutexexact;
pthread_mutex_t mutexdoc;

job_scheduler J_s;
pthread_cond_t cond_end[3];
pthread_cond_t cond_q;
pthread_cond_t cond_br;
pthread_cond_t cond_br2;

pthread_mutex_t br_mutex;
pthread_t tids[NUM_THREADS];

pthread_barrier_t barrier;
pthread_barrier_t barrier2;
pthread_barrier_t barrier3;


ErrorCode match_doc(job_node* data){
	
	word* myword = new word();
	payload_list* q_result = new payload_list();

	char* Str = new char[strlen(data->getstr())+1];
	strcpy(Str,data->getstr());

	char* rest = NULL;
	char* pch = strtok_r (Str," ",&rest);

	int thread_id = realid(pthread_self());

	pthread_mutex_lock(&mutexqhash);
	cout <<"Job "<< realid(pthread_self()) <<" parsing document..  "<<data->getId() <<endl;
	while (pch != NULL){
		myword->setword(pch);
		hash_index->search(myword,Q_hash,data->getId(),q_result,thread_id);
		ham_index->lookup_hamming_index(myword, 1, MT_HAMMING_DIST,Q_hash,data->getId(),q_result,thread_id);
		edit_index->getBKtree()->lookup_entry_index(myword,edit_index->getBKtree(), 1,MT_EDIT_DIST,Q_hash,data->getId(),q_result,thread_id);
		pch = strtok_r(NULL, " ",&rest);
	}
	cout <<"Job "<< realid(pthread_self()) <<" done parsing document..  "<<data->getId() <<endl;
	pthread_mutex_unlock(&mutexqhash);

	pthread_mutex_lock(&mutexdoc);

	doc* D = new doc(data->getId());
	D->set_num_res(q_result->get_counter());

	// Add this result to the set of undelivered results
	doc* D_tt;
	D_tt = D_list->add_doc(D_list,D,q_result);

	if (flag == 1){
		flag = 0;
		D_tmp = D_tt;
	}

	delete myword;
	delete[] Str;
	q_result->destroy_payload_list();

	pthread_mutex_unlock(&mutexdoc);

	return EC_SUCCESS;
}


ErrorCode start_q(job_node* data){
	// cout <<"Thread "<< realid(pthread_self()) <<" parsing query..  "<<data->getId() <<endl;

	pthread_mutex_lock(&mutexqhash);
	query_hash_node* Q;
	Q = Q_hash->insert(data->getId(),data->getstr(),data->getmatch_dist());
	pthread_mutex_unlock(&mutexqhash);

	switch(data->getmatch_type()){

		case MT_HAMMING_DIST:
			pthread_mutex_lock(&mutexhamm);
			for(unsigned int i=0;i<Q->get_word_count();i++){
				ham_index->insert(&(Q->get_word_arr()[i]),data->getId());
			}
			pthread_mutex_unlock(&mutexhamm);
			break;

		case MT_EDIT_DIST:
			pthread_mutex_lock(&mutexedit);
			for(unsigned int i=0;i<Q->get_word_count();i++){
				edit_index->getBKtree()->insertWord(&(Q->get_word_arr()[i]),edit_index->getBKtree(),data->getmatch_type(),data->getId());
			}
			pthread_mutex_unlock(&mutexedit);
			break;

		case MT_EXACT_MATCH:
			pthread_mutex_lock(&mutexexact);
			char* Str = new char[strlen("small")+1];
			strcpy(Str, "small");
			entry* E = new entry(Str);

			for (unsigned int i = 0; i < Q->get_word_count(); i++){
				E->setword(&(Q->get_word_arr()[i]));
				hash_index->insert(E,data->getId());
			}
			delete[] Str;
			delete E;
			pthread_mutex_unlock(&mutexexact);
			break;
			
	}
	for (int i = 0; i < 3;i++){
		if(end_flg[i] == data->getId()){
			pthread_cond_signal(&(cond_end[i]));
		}
	}
	// cout <<"Thread "<< realid(pthread_self()) <<" done parsing query..  "<<data->getId() <<endl;

	return EC_SUCCESS;
}

ErrorCode end_q(job_node* data){

	pthread_mutex_lock(&mutexqhash);

	if (Q_hash->search(data->getId()) == NULL){
		int i;
		for(i = 0; i < 3; i++){
			if(end_flg[i] == 0){
				end_flg[i] = data->getId();
				break;
			}
		}

		pthread_mutex_unlock(&mutexqhash);
		pthread_cond_wait(&(cond_end[i]), &(mutex_end[i]));

		end_flg[i] = 0;
		pthread_mutex_lock(&mutexqhash);
	}
	
	Q_hash->delete_query(data->getId());
	pthread_mutex_unlock(&mutexqhash);
	return EC_SUCCESS;
}

job_node* obtain(){

	job_node* data;
	pthread_mutex_lock(&br_mutex);

	while(J_s.j_list->get_counter() <= 0) {					// perimenei na mpei ena job sth lista
		
		if(br_type == 2 || br_type == 1 || br_type == 3){
			pthread_mutex_unlock(&br_mutex);
			return NULL;
		}

		pthread_cond_wait(&cond_nonempty, &br_mutex);

		if(br_type == 1 || br_type == 2|| br_type == 3){
			pthread_mutex_unlock(&br_mutex);
			return NULL;
		}
	}	

	data = J_s.j_list->job_pop();   

	if (data->getjtype() == BARRIER){

		if (data->getId() == 8008){
			br_type = 1;

		}else if (data->getId() == 1111){
			br_type = 2;
	
		}else if (data->getId() == 2222){
			br_type = 3;
		}
	}

	pthread_mutex_unlock(&br_mutex);
	return data;
}

void * consumer(void * ptr){		// consumer tha trexei kathe thread

	while(true){
		
		if (br_type){
			
			if (br_type == 1){
			
				pthread_cond_signal(&cond_nonempty);

				pthread_barrier_wait(&barrier);

				pthread_mutex_lock(&mutexif);
				if (br_type == 1)
					br_type = 0;
				pthread_mutex_unlock(&mutexif);

				if (pthread_self() == tids[0])
					pthread_cond_signal(&cond_q);
			}
			else if (br_type == 2){
				
				status[realid(pthread_self())] = 1;

				pthread_cond_signal(&cond_nonempty);
				pthread_barrier_wait(&barrier2);

				pthread_mutex_lock(&mutexif);
				if(br_type == 2){
					br_type = 0;
				}
				pthread_mutex_unlock(&mutexif);

				status[realid(pthread_self())] = 0;
				if (pthread_self() == tids[0]){
					pthread_cond_signal(&cond_br);
				}

			}else if (br_type == 3){
				pthread_cond_signal(&cond_nonempty);
				break;
			}
		}

		job_node* data = obtain();

		if ((data == NULL) || (data->getjtype() == BARRIER)){
		 	continue;
		}

		if (br_type){
			if (br_type == 1){
				
				if(data->getjtype()==QUERY){
					start_q(data);
					continue;
				
				}else if(data->getjtype()==END_QUERY){
					end_q(data);
					continue;
				}
				pthread_cond_signal(&cond_nonempty);
				pthread_barrier_wait(&barrier);

				pthread_mutex_lock(&mutexif);
				if (br_type == 1)
					br_type = 0;
				pthread_mutex_unlock(&mutexif);

				if (pthread_self() == tids[0])
					pthread_cond_signal(&cond_q);
			
			}else if (br_type == 2){

				if(data->getjtype()==DOCUMENT){
					match_doc(data);
					continue;
				}

				status[realid(pthread_self())] = 1;

				pthread_cond_signal(&cond_nonempty);
				pthread_barrier_wait(&barrier2);

				pthread_mutex_lock(&mutexif);
				if(br_type == 2){
					br_type = 0;
				}
				pthread_mutex_unlock(&mutexif);

				status[realid(pthread_self())] = 0;
				if (pthread_self() == tids[0])
					pthread_cond_signal(&cond_br);
			
			}else if (br_type == 3){
				pthread_cond_signal(&cond_nonempty);
				break;
			}
		}

		if(data->getjtype() == QUERY) {
			start_q(data);
		
		}else if(data->getjtype() == DOCUMENT){
			match_doc(data);
		
		}else if(data->getjtype() == END_QUERY){
			end_q(data);
		}
		
		if (data && data->getId()==END_DOC && data->getjtype() == DOCUMENT){
			break;
		}
	}
	pthread_barrier_wait(&barrier3);			// perimenoun ola ta threads
	pthread_cond_signal(&cond_br2);

	pthread_exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode InitializeIndex(){
	cout<<"num _ htraeads = "<<NUM_THREADS<<endl;
    Q_hash = new query_Hashtable();
    if (Q_hash == NULL){
        return EC_FAIL;
    }

    D_list = new doc_list();
    if (D_list == NULL){
        return EC_FAIL;
    }

    ham_index = new HammingIndex();
    if (ham_index == NULL){
        return EC_FAIL;
    }

    hash_index = new Hashtable();
    if (hash_index == NULL){
        return EC_FAIL;
    }

    edit_index = new EditBKTree();
    if(edit_index == NULL){
        return EC_FAIL;
    }

	J_s.j_list = new job_list();

	pthread_mutex_init(&mutexif, 0);
	pthread_mutex_init(&mutexD, 0);
	pthread_mutex_init(&mutexq, 0);
	pthread_mutex_init(&mutexAR, 0);
	pthread_mutex_init(&mutexAR2, 0);
	pthread_mutex_init(&mutexcout, 0);
	pthread_mutex_init(&br_mutex, 0);

	pthread_mutex_init(&mutexqhash, 0);
	pthread_mutex_init(&mutexedit, 0);
	pthread_mutex_init(&mutexhamm, 0);
	pthread_mutex_init(&mutexexact, 0);
	pthread_mutex_init(&mutexdoc, 0);

	for(int i =0;i<3;i++){
		end_flg[i] = 0;
		pthread_cond_init(&(cond_end[i]), 0);
		pthread_mutex_init(&(mutex_end[i]), 0);
	}

	pthread_cond_init(&cond_nonempty, 0);
	pthread_cond_init(&cond_q, 0);
	pthread_cond_init(&cond_br, 0);
	pthread_cond_init(&cond_br2, 0);

	pthread_barrier_init(&barrier,NULL,NUM_THREADS);
	pthread_barrier_init(&barrier2,NULL,NUM_THREADS);
	pthread_barrier_init(&barrier3,NULL,NUM_THREADS);

	br_type = 0;
	flag_q = 0;
	int* k = new int();
	*k = 0;
	for (int i = 0; i < NUM_THREADS; i++)
	{
	    pthread_create( &tids[i], NULL, consumer, (void*) k);
		id_assign[i] = tids[i];
		status[i]=0;
		(k)++;
	}
	 
    return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode DestroyIndex(){
	
	if(flag_q){

		J_s.j_list->job_insert(2222,"barrier",MT_EXACT_MATCH,0,BARRIER);

		pthread_cond_signal(&cond_nonempty);
		pthread_cond_wait(&cond_br2, &mutexAR2);
		flag_q = 0;
	}

	D_list->destroy_doc_list(&D_list);

	delete Q_hash;
	delete ham_index;
	delete hash_index;
	delete edit_index;

	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode check_hash_insert(QueryID qid){
	query_hash_node* Q = Q_hash->search(qid);
	if(Q==NULL){
		return EC_FAIL;
	}
	return EC_SUCCESS;
}

ErrorCode check_hash_del(QueryID qid){
	query_hash_node* Q = Q_hash->search(qid);
	if(Q->get_alive() == 0){
		return EC_SUCCESS;
	}
	return EC_FAIL;
}


ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist)
{
	J_s.j_list->job_insert(query_id,query_str,match_type,match_dist,QUERY);		// bazw 1 query sto job list
	pthread_cond_signal(&cond_nonempty);

	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode EndQuery(QueryID query_id){	

	
	J_s.j_list->job_insert(query_id,"query_str",MT_EXACT_MATCH,0,END_QUERY);
	pthread_cond_signal(&cond_nonempty);
	
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode MatchDocument(DocID doc_id, const char* doc_str)// for each document
{
	if(!flag_q){

		J_s.j_list->job_insert(8008,doc_str,MT_EXACT_MATCH,0,BARRIER);
	
		pthread_cond_signal(&cond_nonempty);
		pthread_cond_wait(&cond_q, &mutexq);
		flag_q = 1;
	}

	J_s.j_list->job_insert(doc_id,doc_str,MT_EXACT_MATCH,0,DOCUMENT); 	// bazei to docuent sto job lst
	pthread_cond_signal(&cond_nonempty);

	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids)
{

	if (flag_q){
	
		J_s.j_list->job_insert(1111,"barrier",MT_EXACT_MATCH,0,BARRIER);

		pthread_cond_signal(&cond_nonempty);
		pthread_cond_wait(&cond_br, &mutexAR);
		
		// J_s.j_list->print_list();
		flag_q = 0;
	}
	cout <<"GetNextAvailRes "<< realid(pthread_self()) << endl;

	pthread_mutex_lock(&mutexdoc);

	*p_doc_id=0; *p_num_res=0; *p_query_ids=0; flag = 1;

	*p_doc_id = D_tmp->get_id();
	*p_num_res=D_tmp->get_num_res();
	*p_query_ids = D_tmp->get_query_ids();
	
	if (D_tmp->get_id() == END_DOC){
	
		for (size_t i = 0; i < NUM_THREADS; i++){
		    pthread_join(tids[i],NULL);
		}
	}

	D_tmp = D_tmp->get_next();
	pthread_mutex_unlock(&mutexdoc);
	return EC_SUCCESS;
}

/* ------------------------------------------ Functions ------------------------------------------ */

/* Used for debugging */
void print_status(){
	for (int i = 0; i < NUM_THREADS; i++)
	{
		cout <<"[" <<i <<"-"<<status[i]<<"] ";
	}
	cout << endl;
}

int realid(int thread_self){
	for (int thread = 0; thread < NUM_THREADS; thread++){
		if (thread_self == id_assign[thread])
			return thread;
	}
	return -99999;
}