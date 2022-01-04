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
#include <vector>
#include <pthread.h>
#include "../Lists/jobs.h"
#include "../Lists/doc.h"
#include "../Lists/payload.h"

#include "../Indexes//hashtable.h"
#include "../Indexes/hammingindex.h"
#include "../Indexes//editDistBkTree.h"

#include "../q_hashtable.h"
#define NUM_THREADS 2

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



// thread data
class job_scheduler{
   public:
      job_list* j_list;
};

pthread_cond_t cond_nonempty;
pthread_mutex_t mutex;
job_scheduler J_s;
pthread_cond_t cond_nonfull;
pthread_mutex_t write_mutex;
pthread_t tids[NUM_THREADS];

//Thread functions

job_node* obtain() {
   job_node* data;
   while (J_s.j_list->get_counter() <= 0) {
      pthread_cond_wait(&cond_nonempty, &mutex);
   }
	data = J_s.j_list->job_pop();
	pthread_mutex_unlock(&mutex);
   return data;
}

void * consumer(void * ptr){
	int i = 0 ;
   while (i<50) {
      	job_node* data = obtain();
      	// pthread_mutex_lock(&write_mutex);
		cout<<"DAta : "<<data->getId()<<" "<<data->getjtype()<<endl;
      	// pthread_mutex_unlock(&write_mutex);
		i++;
   }
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

	pthread_mutex_init(&mutex, 0);
	pthread_mutex_init(&write_mutex, 0);
	pthread_cond_init(&cond_nonempty, 0);
	pthread_cond_init(&cond_nonfull, 0);
	
	int ret;
	char *message1 = new char[strlen("Thread 1")+1];
    strcpy(message1,"Thread 1"); 
	for (size_t i = 0; i < 1; i++)
	{
	    ret = pthread_create( &tids[i], NULL, consumer, (void*) message1);
		// cout <<"Ret "<<ret<<endl;
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
	pthread_mutex_lock(&mutex);
	J_s.j_list->job_insert(query_id,query_str,match_type,match_dist,QUERY);
   	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond_nonempty);

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

ErrorCode EndQuery(QueryID query_id)
{
	Q_hash->delete_query(query_id);
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode MatchDocument(DocID doc_id, const char* doc_str)// for each document
{
	pthread_mutex_lock(&mutex);
	J_s.j_list->job_insert(doc_id,doc_str,MT_EXACT_MATCH,0,DOCUMENT);
   	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond_nonempty);
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
	// *p_doc_id=0; *p_num_res=0; *p_query_ids=0;
	// J_s.j_list->print_list();
	// flg = 1;

	// *p_doc_id = D_tmp->get_id();
	// *p_num_res=D_tmp->get_num_res();
	// *p_query_ids = D_tmp->get_query_ids();
	
	// D_tmp = D_tmp->get_next();
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
