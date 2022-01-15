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

#define NUM_THREADS 80 
#define END_DOC 960
using namespace std;

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
QueryID end_flg = 0;

pthread_cond_t cond_nonempty;
pthread_mutex_t mutex_end;
pthread_mutex_t mutexD;
pthread_mutex_t mutexQ;
pthread_mutex_t mutexAR;
pthread_mutex_t mutexAR2;
job_scheduler J_s;
pthread_cond_t cond_end;
pthread_cond_t cond_br;
pthread_cond_t cond_br2;
pthread_cond_t cond_brt;
pthread_mutex_t br_mutex;
pthread_t tids[NUM_THREADS];

pthread_barrier_t barrier;
pthread_barrier_t barrier2;
pthread_barrier_t barrier3;
//Thread functions

job_node* obtain() {
	job_node* data;
	pthread_mutex_lock(&br_mutex);
	
	while(J_s.j_list->get_counter() <= 0) {					// perimenei na mpei ena job sth lista
		pthread_cond_wait(&cond_nonempty, &br_mutex);
	}
	data = J_s.j_list->job_pop();						// pairnei to 1o stoixeio ths listas

	if(data == NULL){
		cout<<"ISISISSSSNUULULLUULU"<<endl;
	}

	if (data->getjtype() == BARRIER){		

		if (data->getId() == 8008){										/* if id == 8008 it's a barrier after a  document? */
			cout << endl << endl << "ABAB EDWWWW" << endl << endl;
			br_flag = 1;
		}else if (data->getId() == 1111){								/* if id == 1111 it's a barrier after Getnextavailres */
			cout << "Ftanei to thread: " << pthread_self() << endl;
			br_flag = 2;
		}else if (data->getId() == 2222){								/* if id == 2222 it's the ending barrier */
			cout << "estww miaaa" << endl;
			br_flag = 3;
		}
	}

	pthread_mutex_unlock(&br_mutex);
	return data;
}
void * consumer(void * ptr){		// consumer tha trexei kathe thread
	int i = 0 ;
	while (1){

		// pthread_mutex_lock(&mutex);
		if(br_flag){
			
			// cout << "top of consumer" << endl;
			// pthread_mutex_unlock(&mutex);
			// perimenoun ola ta threads
			if (br_flag == 1){
				// cout<<"COND: br_flag == 1"<<endl;
				pthread_barrier_wait(&barrier);	
			}
			
			if (br_flag == 2){
				cout<<"COND: br_flag == 2"<<endl;
				pthread_barrier_wait(&barrier2);
				cout << "after br_flag == 2"<<endl;
				pthread_cond_signal(&cond_br);
				}
			else if (br_flag == 3){
				// pthread_cond_signal(&cond_br2);
				cout << "bgainei to threadddddddd" << endl;
				break;
			}
			if (br_flag != 3)
				br_flag = 0;
		}
		
		job_node* data = obtain();
		if(data->getjtype()==BARRIER){
			continue;
		}

		if(br_flag){
			cout<<"AAA1"<<endl;
			// pthread_mutex_unlock(&mutex);
					// perimenoun ola ta threads
			if (br_flag == 1){
				cout<<"EYEEYE111111111111111111111111111"<<endl;
				// sleep(1);
				//data = query 10
				pthread_barrier_wait(&barrier);

			}
			if (br_flag == 2){
				cout<<"iuouououo1"<<endl;
				pthread_barrier_wait(&barrier2);
				pthread_cond_signal(&cond_br);
			
			}else if (br_flag == 3){
				// pthread_cond_signal(&cond_br2);
				cout << "bgainei to threadddddddd1" << endl;
				break;
			}
			if (br_flag != 3)
				br_flag = 0;
		}

		if(data == NULL){
			cout<<"IMAGE OF A DOHG"<<endl;
		}

		// cout<< "obtain: " << data->getId()<<" "<<data->getjtype()<<endl;

		pthread_mutex_lock(&mutexD);
		// cout << ""
		// J_s.j_list->print_list();
		if(data->getjtype() == QUERY) {
			cout <<"Thread "<<pthread_self()<<" inserting query " << data->getId()<<endl; 
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
			if(end_flg == data->getId()){
				cout<<"SIGNAL TO "<<data->getId()<<endl;
				pthread_cond_signal(&cond_end);
			}
			cout <<"Thread "<<pthread_self()<<" done with query " << data->getId()<<endl; 

		}else if(data->getjtype() == DOCUMENT){
			// int* jobid = (int*) ptr;
			// cout<<"Job "<< pthread_self() <<" parsing document "<<data->getId() <<endl;
			word* myword = new word();
			payload_list* q_result = new payload_list();

			// Iterate on all active queries to compare them with this new document
			char * pch;

			char* Str = new char[strlen(data->getstr())+1];
			strcpy(Str,data->getstr());
			pch = strtok (Str," ");
			while (pch != NULL){
				myword->setword(pch);
				// cout << " prin apo to searchh " << endl;
				hash_index->search(myword,Q_hash,data->getId(),q_result);
				ham_index->lookup_hamming_index(myword, 1, MT_HAMMING_DIST,Q_hash,data->getId(),q_result);
				edit_index->getBKtree()->lookup_entry_index(myword,edit_index->getBKtree(), 1,MT_EDIT_DIST,Q_hash,data->getId(),q_result);
				// cout << " meta apo to searchh " << endl;
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
			// cout<<"Job "<< pthread_self() <<" done parsing document "<<data->getId() <<endl << endl;
		}
		else if(data->getjtype() == END_QUERY){
			// cout<<"Job END"<< pthread_self()<<"parsing end queyryr "<<data->getId() <<endl;
			if(Q_hash->search(data->getId())==NULL){
				end_flg = data->getId();
				pthread_mutex_unlock(&mutexD);
				cout << "WAIT TO "<<data->getId()<<endl;
				// cout<<"DA"<<endl;
				pthread_cond_wait(&cond_end, &mutex_end);
				pthread_mutex_lock(&mutexD);
			}
			cout<<"DASSDA"<<endl;
			Q_hash->delete_query(data->getId());
			// cout<<"Job END"<< pthread_self()<<" done parsing  end queyryr "<<data->getId() <<endl;

		}


		// cout<<"BARRRIrRIRRIRRRIR id:" <<pthread_self()<<"DAta : "<<data->getId()<<" "<<data->getjtype()<<endl;

		// if (J_s.j_list->getFirst() != NULL)
		// 	J_s.j_list->print_list();
		pthread_mutex_unlock(&mutexD);
		// cout << "done done parsing document " << data->getId() <<endl;
		
		if (data && data->getId()==END_DOC && data->getjtype() == DOCUMENT){
			cout<<"HE:LLO"<<endl;
			break;
		}
		i++;
	}	
	pthread_barrier_wait(&barrier3);			// perimenoun ola ta threads
	// sleep(1);
	cout<<"EL END"<<endl;

	pthread_cond_signal(&cond_br2);
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
	pthread_mutex_init(&mutexAR2, 0);
	pthread_mutex_init(&mutex_end, 0);
	pthread_mutex_init(&br_mutex, 0);
	pthread_cond_init(&cond_end, 0);
	pthread_cond_init(&cond_nonempty, 0);
	pthread_cond_init(&cond_br, 0);
	pthread_cond_init(&cond_br2, 0);
	pthread_cond_init(&cond_brt, 0);
	pthread_barrier_init(&barrier,NULL,NUM_THREADS);
	pthread_barrier_init(&barrier2,NULL,NUM_THREADS);
	pthread_barrier_init(&barrier3,NULL,NUM_THREADS);
	br_flag = 0;
	flag_q = 0;
	int* k = new int();
	*k = 0;
	for (int i = 0; i < NUM_THREADS; i++)
	{
	    pthread_create( &tids[i], NULL, consumer, (void*) k);
		(k)++;
	}
	 
    return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode DestroyIndex(){
	
	cout << "KALEITAIIII" << endl;
	if(flag_q){

		cout << "MPHKE EDWWW DestroyIndex" << endl;

		pthread_mutex_lock(&br_mutex);
		J_s.j_list->job_insert(2222,"barrier",MT_EXACT_MATCH,0,BARRIER);
		pthread_mutex_unlock(&br_mutex);

		pthread_cond_signal(&cond_nonempty);
		pthread_cond_wait(&cond_br2, &mutexAR2);
		flag_q = 0;
	}

	cout << "BGAINEIIIIII" << endl;
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
	J_s.j_list->job_insert(query_id,query_str,match_type,match_dist,QUERY);		// bazw 1 query sto job list
   	pthread_mutex_unlock(&br_mutex);

	pthread_cond_signal(&cond_nonempty);
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
	J_s.j_list->job_insert(doc_id,doc_str,MT_EXACT_MATCH,0,DOCUMENT); 	// bazei to docuent sto job lst
	// cout<<"doc id "<<doc_id<<endl;
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
	cout<<"Getnextavailres"<<endl;
	// J_s.j_list->print_list();
	if(flag_q){

		cout << "MPHKE EDWWW GetNextAvailRes" << endl;

		pthread_mutex_lock(&br_mutex);
		J_s.j_list->job_insert(1111,"barrier",MT_EXACT_MATCH,0,BARRIER);
		pthread_mutex_unlock(&br_mutex);

		pthread_cond_signal(&cond_nonempty);

		pthread_cond_wait(&cond_br, &mutexAR);
		flag_q = 0;
	}
	cout<<"AFTER Barrier getnextavailres"<<endl;
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
	return EC_SUCCESS;
}

// otan to programma trexei den ginetai pote barrier == 2 => flag_q != 1 sto GetNextAvailRes

///////////////////////////////////////////////////////////////////////////////////////////////
