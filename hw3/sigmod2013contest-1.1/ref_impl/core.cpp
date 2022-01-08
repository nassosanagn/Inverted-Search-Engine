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
#define NUM_THREADS 1
#define END_DOC 960
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////

query_Hashtable* Q_hash;
doc_list* D_list;
HammingIndex* ham_index;
Hashtable* hash_index;
EditBKTree* edit_index;
int flg = 1;
doc* D_tmp;

int br_flag;
int flag_q;

// thread data
class job_scheduler{
   public:
      job_list* j_list;
};

pthread_cond_t cond_nonempty;
pthread_mutex_t mutexD;
pthread_mutex_t mutexQ;
pthread_mutex_t mutexAR;
job_scheduler J_s;
pthread_cond_t cond_br;
pthread_mutex_t br_mutex;
pthread_t tids[NUM_THREADS];
pthread_barrier_t barrier;
//Thread functions

job_node* obtain() {
	job_node* data;
	pthread_mutex_lock(&br_mutex);
	while(J_s.j_list->get_counter() <= 0) {
		pthread_cond_wait(&cond_nonempty, &br_mutex);
	}
	data = J_s.j_list->job_pop();
	if(data == NULL){
		cout<<"ISISISSSSNUULULLUULU"<<endl;
	}
	if(data->getjtype() == BARRIER){
		br_flag = 1;
	}
	pthread_mutex_unlock(&br_mutex);
	return data;
}

void * consumer(void * ptr){
	int i = 0 ;
	while (1){
		// pthread_mutex_lock(&mutex);
		if(br_flag){
			// pthread_mutex_unlock(&mutex);
			pthread_barrier_wait(&barrier);
			// cout<<"EL BARRIER"<<endl;
			pthread_cond_signal(&cond_br);
			br_flag = 0;
		}
		job_node* data = obtain();
		if(data == NULL){
			cout<<"IMAGE OF A DOHG"<<endl;
		}
		cout<<data->getId()<<" "<<data->getjtype()<<endl;
		pthread_mutex_lock(&mutexD);
		if(data->getjtype() == QUERY) {
			query_hash_node* Q;
			Q = Q_hash->insert(data->getId(),data->getstr(),data->getmatch_dist());
			switch(data->getmatch_type()){
				case MT_HAMMING_DIST:
					for(unsigned int i=0;i<Q->get_word_count();i++){
						ham_index->insert(&(Q->get_word_arr()[i]),data->getId());
					}
					break;
				case MT_EDIT_DIST:
					for(unsigned int i=0;i<Q->get_word_count();i++){
						edit_index->getBKtree()->insertWord(&(Q->get_word_arr()[i]),edit_index->getBKtree(),data->getmatch_type(),data->getId());
					}
					break;
				case MT_EXACT_MATCH:
					char* Str = new char[strlen("small")+1];
					strcpy(Str, "small");
					entry* E = new entry(Str);
					for(unsigned int i=0;i<Q->get_word_count();i++){
						E->setword(&(Q->get_word_arr()[i]));
						hash_index->insert(E,data->getId());
					}
					delete[] Str;
					delete E;
					break;
			}
		}
		else if(data->getjtype() == DOCUMENT){
			// cout<<data->getstr()<<endl;
			word* myword = new word();
			payload_list* q_result = new payload_list();

			// Iterate on all active queries to compare them with this new document
			char * pch;

			char* Str = new char[strlen(data->getstr())+1];
			strcpy(Str,data->getstr());
			pch = strtok (Str," ");
			while (pch != NULL){
				myword->setword(pch);
				hash_index->search(myword,Q_hash,data->getId(),q_result);
				ham_index->lookup_hamming_index(myword, 1, MT_HAMMING_DIST,Q_hash,data->getId(),q_result);
				edit_index->getBKtree()->lookup_entry_index(myword,edit_index->getBKtree(), 1,MT_EDIT_DIST,Q_hash,data->getId(),q_result);
				pch = strtok (NULL, " ");
			}
			doc* D = new doc(data->getId());
			D->set_num_res(q_result->get_counter());
			// Add this result to the set of undelivered results
			doc* D_tt;
			D_tt = D_list->add_doc(D_list,D,q_result);
			if(flg==1){
				flg = 0;
				D_tmp = D_tt;
			}

			delete myword;
			delete[] Str;
			// delete D->get_query_ids();
			q_result->destroy_payload_list();
		}
		else if(data->getjtype() == END_QUERY){
			Q_hash->delete_query(data->getId());
		}
		// cout<<"BARRRIrRIRRIRRRIR id:" <<pthread_self()<<"DAta : "<<data->getId()<<" "<<data->getjtype()<<endl;
		pthread_mutex_unlock(&mutexD);

		// cout<<"adasdssss"<<endl;
		if(data && data->getId()==END_DOC && data->getjtype() == DOCUMENT){
			cout<<"HE:LLO"<<endl;
			break;
		}
		i++;
	}
	cout<<"EL END"<<endl;
	pthread_exit(0);
}

// void * producer(void * ptr){
//    int i = 0;
//    int *ret;
//    job_node* input_node;
//    while (1) {
//       place(&J_s, input_node);
//       i++;
//       pthread_cond_signal(&cond_nonempty);
//    }
//    return ret;
// 

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode InitializeIndex(){
    Q_hash = new query_Hashtable();
    if(Q_hash == NULL){
        return EC_FAIL;
    }
    D_list = new doc_list();
    if(D_list == NULL){
        return EC_FAIL;
    }
    ham_index = new HammingIndex();
    if(ham_index == NULL){
        return EC_FAIL;
    }
    hash_index = new Hashtable();
    if(hash_index == NULL){
        return EC_FAIL;
    }
    edit_index = new EditBKTree();
    if(edit_index == NULL){
        return EC_FAIL;
    }
	J_s.j_list = new job_list();

	pthread_mutex_init(&mutexD, 0);
	pthread_mutex_init(&mutexQ, 0);
	pthread_mutex_init(&mutexAR, 0);
	pthread_mutex_init(&br_mutex, 0);
	pthread_cond_init(&cond_nonempty, 0);
	pthread_cond_init(&cond_br, 0);
	pthread_barrier_init(&barrier,NULL,NUM_THREADS);
	br_flag = 0;
	flag_q = 0;
	char *message1 = new char[strlen("Thread 1")+1];
    strcpy(message1,"Thread 1"); 
	for (size_t i = 0; i < NUM_THREADS; i++)
	{
	    pthread_create( &tids[i], NULL, consumer, (void*) message1);
	}
	 
    return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode DestroyIndex(){
	
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
	pthread_mutex_lock(&br_mutex);
	J_s.j_list->job_insert(query_id,query_str,match_type,match_dist,QUERY);
   	pthread_mutex_unlock(&br_mutex);
	pthread_cond_signal(&cond_nonempty);
	cout<<"qu_id "<<query_id<<endl;
	// cout<<"Insert Query "<<query_id<<endl;

	// query_hash_node* Q;
	// Q = Q_hash->insert(query_id,query_str,match_dist);
	// switch(match_type){
    //     case MT_HAMMING_DIST:
	// 		for(unsigned int i=0;i<Q->get_word_count();i++){
	// 			ham_index->insert(&(Q->get_word_arr()[i]),query_id);
	// 		}
    //         break;
    //     case MT_EDIT_DIST:
	// 		for(unsigned int i=0;i<Q->get_word_count();i++){
	// 			edit_index->getBKtree()->insertWord(&(Q->get_word_arr()[i]),edit_index->getBKtree(),match_type,query_id);
	// 		}
    //         break;
    //     case MT_EXACT_MATCH:
	// 		char* Str = new char[strlen("small")+1];
	// 		strcpy(Str, "small");
	// 		entry* E = new entry(Str);
	// 		for(unsigned int i=0;i<Q->get_word_count();i++){
	// 			E->setword(&(Q->get_word_arr()[i]));
	// 			hash_index->insert(E,query_id);
	// 		}
	// 		delete[] Str;
	// 		delete E;
    //         break;
    // }
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode EndQuery(QueryID query_id){	
	pthread_mutex_lock(&br_mutex);
	J_s.j_list->job_insert(query_id,"query_str",MT_EXACT_MATCH,0,END_QUERY);
   	pthread_mutex_unlock(&br_mutex);
	pthread_cond_signal(&cond_nonempty);
	cout<<"qu_id "<<query_id<<endl;
	// pthread_mutex_lock(&mutexD);
	// cout<<"AAAAAAAAAAA"<<endl;
	// Q_hash->delete_query(query_id);
	// pthread_mutex_unlock(&mutexD);
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode MatchDocument(DocID doc_id, const char* doc_str)// for each document
{
	if(!flag_q){
		pthread_mutex_lock(&br_mutex);
		// cout<<"counter"<<J_s.j_list->get_counter()<<endl;
		J_s.j_list->job_insert(8008,doc_str,MT_EXACT_MATCH,0,BARRIER);
		// cout<<"counter after"<<J_s.j_list->get_counter()<<endl;
		pthread_mutex_unlock(&br_mutex);
		pthread_cond_signal(&cond_nonempty);
		flag_q = 1;
	}
	pthread_mutex_lock(&br_mutex);
	J_s.j_list->job_insert(doc_id,doc_str,MT_EXACT_MATCH,0,DOCUMENT);
	cout<<"doc id "<<doc_id<<endl;
   	pthread_mutex_unlock(&br_mutex);
	pthread_cond_signal(&cond_nonempty);
	// cout<<"Insert document "<<doc_id<<endl;
	// word* myword = new word();
	// payload_list* q_result = new payload_list();

	// // Iterate on all active queries to compare them with this new document
    // char * pch;

    // char* Str = new char[strlen(doc_str)+1];
    // strcpy(Str,doc_str);
    // pch = strtok (Str," ");

    // while (pch != NULL){
	// 	myword->setword(pch);

	// 	hash_index->search(myword,Q_hash,doc_id,q_result);

	// 	ham_index->lookup_hamming_index(myword, 1, MT_HAMMING_DIST,Q_hash,doc_id,q_result);

	// 	edit_index->getBKtree()->lookup_entry_index(myword,edit_index->getBKtree(), 1, MT_EDIT_DIST,Q_hash,doc_id,q_result);
    //     pch = strtok (NULL, " ");
    // }

	// doc* D = new doc(doc_id);
	// D->set_num_res(q_result->get_counter());
	// // D->set_query_ids(q_result,q_result->get_counter());

	// // Add this result to the set of undelivered results
	// doc* D_tt;
	// D_tt = D_list->add_doc(D_list,D,q_result);
	// if(flg==1){
	// 	flg = 0;
	// 	D_tmp = D_tt;
	// }

	// delete myword;
	// delete[] Str;
	// // delete D->get_query_ids();
	// q_result->destroy_payload_list();
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids)
{
	// Get the first undeliverd resuilt from "docs" and return it
	// J_s.j_list->print_list();
	cout<<"DASDASS"<<endl;
	if(flag_q){
		pthread_mutex_lock(&br_mutex);
		J_s.j_list->job_insert(1111,"barrier",MT_EXACT_MATCH,0,BARRIER);
		pthread_mutex_unlock(&br_mutex);
		pthread_cond_signal(&cond_nonempty);
		pthread_cond_wait(&cond_br, &mutexAR);
		flag_q = 0;
	}
	// sleep(1);
	// J_s.j_list->print_list();
	*p_doc_id=0; *p_num_res=0; *p_query_ids=0;
	// J_s.j_list->print_list();
	flg = 1;
	pthread_mutex_lock(&mutexD);
	cout<<"... "<<D_tmp->get_id()<<endl;

	*p_doc_id = D_tmp->get_id();
	*p_num_res=D_tmp->get_num_res();
	*p_query_ids = D_tmp->get_query_ids();
	// cout<<D_tmp->get_id()<<endl;
	if(D_tmp->get_id() == END_DOC){
		cout<<"LLOdasdas"<<endl;
		for (size_t i = 0; i < NUM_THREADS; i++){
		    pthread_join(tids[i],NULL);
		}
	}
	// cout<<"Get next avail res "<<D_tmp->get_id()<<endl;
	D_tmp = D_tmp->get_next();
	pthread_mutex_unlock(&mutexD);
	//flag close threads

	// for (size_t i = 0; i < NUM_THREADS; i++)
	// {
	//     pthread_join(tids[i],NULL);
	// }
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
