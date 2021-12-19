#include "acutest.h"
#include "sigmod2013contest-1.1/include/core.h"
#include "query.h"
#include "doc.h"
#include "payload.h"
#include "hashtable.h"
#include "hammingindex.h"
#include "editDistBkTree.h"
#include "q_hashtable.h"
#include <iostream>
using namespace std;


void test_InitializeIndex(void){
    TEST_CHECK_(InitializeIndex() == EC_SUCCESS,"Unallocated memory at InitializeIndex");
}

void test_StartQuery(void){
    TEST_CHECK_(InitializeIndex() == EC_SUCCESS,"Unallocated memory at InitializeIndex");
    char* query_str = new char[strlen("first_word second_word third_word")+1];
    strcpy(query_str,"first_word second_word third_word");
    
    TEST_CHECK_(StartQuery(1,query_str,MT_EXACT_MATCH,0) == EC_SUCCESS,"Unallocated memory at StartQuery on exact match");
    TEST_CHECK_(check_hash_insert(1) == EC_SUCCESS,"Cannot find query id 1 at query hashtable");

    TEST_CHECK_(StartQuery(2,query_str,MT_HAMMING_DIST,1) == EC_SUCCESS,"Unallocated memory at StartQuery on hamming dist");
    TEST_CHECK_(check_hash_insert(2) == EC_SUCCESS,"Cannot find query id 2 at query hashtable");

    TEST_CHECK_(StartQuery(3,query_str,MT_EDIT_DIST,2) == EC_SUCCESS,"Unallocated memory at StartQuery on edit dist");
    TEST_CHECK_(check_hash_insert(3) == EC_SUCCESS,"Cannot find query id 3 at query hashtable");
}

void test_EndQuery(void){
    TEST_CHECK_(InitializeIndex() == EC_SUCCESS,"Unallocated memory at InitializeIndex");
    char* query_str = new char[strlen("first_word second_word third_word")+1];
    strcpy(query_str,"first_word second_word third_word");
    
    TEST_CHECK_(StartQuery(1,query_str,MT_EXACT_MATCH,0) == EC_SUCCESS,"Unallocated memory at StartQuery on exact match");
    TEST_CHECK_(check_hash_insert(1) == EC_SUCCESS,"Cannot find query id 1 at query hashtable");

    TEST_CHECK_(StartQuery(2,query_str,MT_HAMMING_DIST,1) == EC_SUCCESS,"Unallocated memory at StartQuery on hamming dist");
    TEST_CHECK_(check_hash_insert(2) == EC_SUCCESS,"Cannot find query id 2 at query hashtable");

    TEST_CHECK_(StartQuery(3,query_str,MT_EDIT_DIST,2) == EC_SUCCESS,"Unallocated memory at StartQuery on edit dist");
    TEST_CHECK_(check_hash_insert(3) == EC_SUCCESS,"Cannot find query id 3 at query hashtable");

    TEST_CHECK_(EndQuery(1) == EC_SUCCESS,"Error at end query for id 3");
    TEST_CHECK_(check_hash_del(1) == EC_SUCCESS,"Error at hash delete: There is still a query with id 3");

    TEST_CHECK_(EndQuery(2) == EC_SUCCESS,"Error at end query for id 3");
    TEST_CHECK_(check_hash_del(2) == EC_SUCCESS,"Error at hash delete: There is still a query with id 3");

    TEST_CHECK_(EndQuery(3) == EC_SUCCESS,"Error at end query for id 3");
    TEST_CHECK_(check_hash_del(3) == EC_SUCCESS,"Error at hash delete: There is still a query with id 3");
} 

void test_MatchDocument(void){
    TEST_CHECK_(InitializeIndex() == EC_SUCCESS,"Unallocated memory at InitializeIndex");
    char* query_str = new char[strlen("first_word")+1];
    strcpy(query_str,"first_word");
    
    char* query_str2 = new char[strlen("second_word")+1];
    strcpy(query_str2,"second");

    char* query_str3 = new char[strlen("first_word third_word")+1];
    strcpy(query_str3,"first_word third_word");

    TEST_CHECK_(StartQuery(1,query_str,MT_EXACT_MATCH,0) == EC_SUCCESS,"Unallocated memory at StartQuery on exact match");
    TEST_CHECK_(check_hash_insert(1) == EC_SUCCESS,"Cannot find query id 1 at query hashtable");

    TEST_CHECK_(StartQuery(2,query_str,MT_HAMMING_DIST,1) == EC_SUCCESS,"Unallocated memory at StartQuery on hamming dist");
    TEST_CHECK_(check_hash_insert(2) == EC_SUCCESS,"Cannot find query id 2 at query hashtable");

    TEST_CHECK_(StartQuery(3,query_str2,MT_EDIT_DIST,2) == EC_SUCCESS,"Unallocated memory at StartQuery on edit dist");
    TEST_CHECK_(check_hash_insert(3) == EC_SUCCESS,"Cannot find query id 3 at query hashtable");

    TEST_CHECK_(MatchDocument(1,query_str3) == EC_SUCCESS,"Error at match document");

    unsigned int doc_id=0;
    unsigned int num_res=0;
    unsigned int* query_ids=0;

    TEST_CHECK_(GetNextAvailRes(&doc_id,&num_res,&query_ids) == EC_SUCCESS,"Error at GetNextAvailRes");
    //Sto document 1 prepei na toy antistoixoyn mono ta queries 1 kai 2 kathws sto 3 einai diaforetikh le3h me dif > 2
    TEST_CHECK_(doc_id==1,"Not the same document with had insert");
    TEST_CHECK_(num_res==2,"Not the correct amount of result queries");
    for(int i=0;i<2;i++){
        TEST_CHECK_(query_ids[i] == i+1,"Wrong query result");
    }
}

void test_query_hash_node(void){
    char* query_str = new char[strlen("first_word")+1];
    strcpy(query_str,"first_word");
    query_hash_node* Q = new query_hash_node(1,query_str,1);
    TEST_CHECK_(Q!=NULL,"Wrong initialization");
    TEST_CHECK_(Q->get_alive()==1,"Wrong initialization");
}

void test_query_hash_list(void){
    query_hash_list* ql;
    ql = new query_hash_list();
    TEST_CHECK_(ql != NULL,"Wrong initialization");

    char* query_str = new char[strlen("first_word")+1];
    strcpy(query_str,"first_word");
    ql->add_query(ql,1,query_str,1);
    TEST_CHECK_(ql->search_id(1) != NULL,"Cannot find id 1");
}

void test_query_Hashtable(void){
    query_Hashtable* q_h = new query_Hashtable();
    char* query_str = new char[strlen("first_word")+1];
    strcpy(query_str,"first_word");
    TEST_CHECK_(q_h->insert(1,query_str,1) != NULL,"Wrong insert for id 1");
    TEST_CHECK_(q_h->search(1) != NULL,"Cannot find query id 1 ");

    TEST_CHECK_(q_h->delete_query(1) == EC_SUCCESS,"Cannot delete query id 1 ");

    query_hash_node* qn;
    qn = q_h->search(1);
    TEST_CHECK_(qn->get_alive() == 0,"found deleted query with id 1 ");
}

void test_query_Hashtable_payload(void){
    query_Hashtable* q_h = new query_Hashtable();
    query_hash_node* Q;
    char* query_str = new char[strlen("first_word")+1];
    strcpy(query_str,"first_word");
    TEST_CHECK_(q_h->insert(1,query_str,1) != NULL,"Wrong insert for id 1");
    TEST_CHECK_((Q=q_h->search(1)) != NULL,"Cannot find query id 1 ");

    word* W = new word(query_str);
    TEST_CHECK_(q_h->add_one(W,1,1) == EC_SUCCESS,"Cannot change data at query with id 1 ");
}
void test_payload_node(void){
    payload_node* pn = new payload_node(1); 
}



TEST_LIST = {
    { "test_InitializeIndex", test_InitializeIndex},
    { "test_StartQuery", test_StartQuery},
    { "test_EndQuery", test_EndQuery},
    { "test_MatchDocument", test_MatchDocument},
    { "test_query_hash_node", test_query_hash_node},
    { "test_query_hash_list", test_query_hash_list},
    { "test_query_Hashtable", test_query_Hashtable},
    { "test_query_Hashtable_payload", test_query_Hashtable_payload},
    { "test_payload_node", test_payload_node},
    { NULL, NULL }
};