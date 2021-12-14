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
#include "../query.h"
#include "../doc.h"
#include "../payload.h"
#include "../hashtable.h"
#include "../hammingindex.h"
#include "../editDistBkTree.h"
using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////

// Computes edit distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb" 
int EditDistance(char* a, int na, char* b, int nb)
{
	int oo=0x7FFFFFFF;

	static int T[2][MAX_WORD_LENGTH+1];

	int ia, ib;

	int cur=0;
	ia=0;

	for(ib=0;ib<=nb;ib++)
		T[cur][ib]=ib;

	cur=1-cur;

	for(ia=1;ia<=na;ia++)
	{
		for(ib=0;ib<=nb;ib++)
			T[cur][ib]=oo;

		int ib_st=0;
		int ib_en=nb;

		if(ib_st==0)
		{
			ib=0;
			T[cur][ib]=ia;
			ib_st++;
		}

		for(ib=ib_st;ib<=ib_en;ib++)
		{
			int ret=oo;

			int d1=T[1-cur][ib]+1;
			int d2=T[cur][ib-1]+1;
			int d3=T[1-cur][ib-1]; if(a[ia-1]!=b[ib-1]) d3++;

			if(d1<ret) ret=d1;
			if(d2<ret) ret=d2;
			if(d3<ret) ret=d3;

			T[cur][ib]=ret;
		}

		cur=1-cur;
	}

	int ret=T[1-cur][nb];

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////

// Computes Hamming distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb" 
unsigned int HammingDistance(char* a, int na, char* b, int nb)
{
	int j, oo=0x7FFFFFFF;
	if(na!=nb) return oo;
	
	unsigned int num_mismatches=0;
	for(j=0;j<na;j++) if(a[j]!=b[j]) num_mismatches++;
	
	return num_mismatches;
}

///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all information related to an active query
struct Query
{
	QueryID query_id;
	char str[MAX_QUERY_LENGTH];
	MatchType match_type;
	unsigned int match_dist;
};

///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all query ID results associated with a dcoument
struct Document
{
	DocID doc_id;
	unsigned int num_res;
	QueryID* query_ids;
};

///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all currently active queries
vector<Query> queries;

query_list* Q_list;
doc_list* D_list;
HammingIndex* ham_index;
Hashtable* hash_index;
EditBKTree* edit_index;
int flg = 1;
doc* D_tmp;
// Keeps all currently available results that has not been returned yet
vector<Document> docs;

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode InitializeIndex(){

    Q_list = new query_list();
    D_list = new doc_list();
	ham_index = new HammingIndex();
	hash_index = new Hashtable();
	edit_index = new EditBKTree();
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode DestroyIndex(){return EC_SUCCESS;}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist)
{
	query* Q;
	// cout<<"START "<<query_id<<endl;
	Q = Q_list->add_query(Q_list,query_id, query_str, match_type, match_dist);
	switch(match_type){

        case MT_HAMMING_DIST:
			for(unsigned int i=0;i<Q->get_word_count();i++){
				ham_index->insert(&(Q->get_word_arr()[i]),query_id);
			}
            break;
        case MT_EDIT_DIST:
			for(unsigned int i=0;i<Q->get_word_count();i++){
				edit_index->getBKtree()->insertWord(&(Q->get_word_arr()[i]),edit_index->getBKtree(),match_type,query_id);
			}
            break;
        case MT_EXACT_MATCH:
			char* Str = new char[strlen("small")+1];
    		strcpy(Str,"small");
			entry* E = new entry(Str);
			for(unsigned int i=0;i<Q->get_word_count();i++){
				E->setword(&(Q->get_word_arr()[i]));
				hash_index->insert(E,query_id);
			}
            break;
    }
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode EndQuery(QueryID query_id)
{
	// Remove this query from the active query set
	Q_list->delete_query(query_id);
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode MatchDocument(DocID doc_id, const char* doc_str)
{
	// cout<<"test "<<doc_id<<endl;
	char cur_doc_str[MAX_DOC_LENGTH];
	strcpy(cur_doc_str, doc_str);

	int r_num = 0;

	entry_list* result= new entry_list();

	
	word* myword = new word();

	payload_list* q_result = new payload_list();
	// Iterate on all active queries to compare them with this new document
	query* Q = Q_list->getfirst();
	while(Q!=NULL){
		bool matching_query=true;
		for(int i=0;i<int(Q->get_word_count());i++){
			if(!matching_query)
				break;
			bool matching_word=false;
			int id=0;
			while(cur_doc_str[id] && !matching_word)
			{
				while(cur_doc_str[id]==' ') id++;
				if(!cur_doc_str[id]) break;
				char* dword=&cur_doc_str[id];

				int ld=id;
				while(cur_doc_str[id] && cur_doc_str[id]!=' ') id++;
				char dt=cur_doc_str[id];
				cur_doc_str[id]=0;

				ld=id-ld;

				if(Q->get_match_type()==MT_EXACT_MATCH)
				{
					if(strcmp(((Q->get_word_arr())[i]).getword(), dword)==0) matching_word=true;
				}
				else if(Q->get_match_type()==MT_HAMMING_DIST)
				{
					unsigned int num_mismatches=HammingDistance(((Q->get_word_arr())[i]).getword(), strlen(((Q->get_word_arr())[i]).getword()), dword, ld);
					if(num_mismatches<=Q->get_dist()) matching_word=true;
				}
				else if(Q->get_match_type()==MT_EDIT_DIST)
				{
					myword->setword(dword);
					result = NULL;
					result = new entry_list();
					
					if(doc_id == 3 && Q->get_id()==8 ){
						edit_index->getBKtree()->lookup_entry_index(myword,edit_index->getBKtree(),Q->get_dist(),result,MT_EDIT_DIST,1);
					}
					else{
						edit_index->getBKtree()->lookup_entry_index(myword,edit_index->getBKtree(),Q->get_dist(),result,MT_EDIT_DIST);
					}
					
					entry* e = result->search_word(&((Q->get_word_arr())[i]));
					if(doc_id == 3&&Q->get_id()==8 && i==2 && result->get_first(result)!=NULL){

						// edit_index->getBKtree()->printTree();
						result->print_list(result);
					}
					if(e != NULL){
						if(e->search_payload(Q->get_id())!=NULL){
							// if(result->get_first(result)!=NULL)
							// 	result->get_first(result)->getpayload()->print_list();
							// e->getpayload()->print_list();
							// if(doc_id == 15){
							// 	// cout<<"DAS "<<dword<<"   "<<((Q->get_word_arr())[i]).getword()<<" "<<Q->get_dist()<<endl;
							// 	cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA "<<dword<<" "<<((Q->get_word_arr())[i]).getword()<<" "<<Q->get_id()<<" "<<endl;
							// 	result->print_list(result);
							// }
							matching_word=true;
						}
					}

					// unsigned int edit_dist=EditDistance(((Q->get_word_arr())[i]).getword(), strlen(((Q->get_word_arr())[i]).getword()), dword, ld);
					// if(edit_dist<=Q->get_dist()) matching_word=true;
				}

				cur_doc_str[id]=dt;
			}

			if(!matching_word)
			{
				// if(doc_id == 15&&Q->get_match_type() ==MT_EDIT_DIST ){
				// 	cout<<"DASDADAS"<<endl;
				// 	cout<<Q->get_word_count()<<" "<<Q->get_id()<<endl;
				// }
				// This query has a word that does not match any word in the document
				matching_query=false;
			}
		}

		if(matching_query)
		{
			// This query matches the document
			q_result->payload_insert(Q->get_id());
			r_num++;
		}
		Q = Q->get_next();
	}
	doc* D;
	D = new doc(doc_id);
	D->set_num_res(r_num);
	D->set_query_ids(q_result,r_num);
	// Add this result to the set of undelivered results
	doc* D_tt;
	D_tt = D_list->add_doc(D_list,D,q_result);
	if(flg==1){
		flg = 0;
		// cout<<"DADAS"<<endl;
		D_tmp = D_tt;
	}
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids)
{
	// Get the first undeliverd resuilt from "docs" and return it
	*p_doc_id=0; *p_num_res=0; *p_query_ids=0;
	flg = 1;
	if(D_tmp==NULL){
		// cout<<"HEY SISTES"<<endl;
		return EC_NO_AVAIL_RES;
	}
	// cout<<"D_t "<<D_tmp->get_id()<<endl;

	// cout<<"get_num_res "<<D_tmp->get_num_res()<<endl;

	*p_doc_id = D_tmp->get_id();
	*p_num_res=D_tmp->get_num_res();
	*p_query_ids = D_tmp->get_query_ids();
	
	// cout<<"id "<<*p_doc_id<<" num "<<*p_num_res<<endl;
	// cout<<"doc id = "<<*p_doc_id<<" ";
	// for(int i = 0 ; i < *p_num_res;i++){
	// 	cout<<(*p_query_ids)[i]<<" ";
	// }
	// cout<<endl;
	// cout<<"das"<<endl;
	D_tmp = D_tmp->get_next();
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
