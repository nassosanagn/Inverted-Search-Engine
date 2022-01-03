#include "acutest.h"
#include "q_hashtable.h"
#include "Indexes/BKTree.h"
#include "Indexes/hammingindex.h"
#include "Indexes/editDistBkTree.h"
#include "Indexes/hashtable.h"
#include "Lists/doc.h"
#include "Lists/entry.h"
#include "Lists/payload.h"
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
    strcpy(query_str2,"second_word");

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
    for(unsigned int i=0;i<2;i++){
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
void test_payload_list(void){
    payload_list* pl = new payload_list();
    TEST_CHECK_(pl->payload_insert(1) == EC_SUCCESS,"Cannot insert payload with id 1");
    TEST_CHECK_(pl->search_id(1) == EC_SUCCESS,"Cannot find payload with inserted before");
    TEST_CHECK_(pl->payload_insert_asc(2) == EC_SUCCESS,"Cannot insert payload with id 1");
    TEST_CHECK_(pl->search_id(2) == EC_SUCCESS,"Cannot find payload with inserted before");
}
void test_doc(void){
    payload_list* pl = new payload_list();
    TEST_CHECK_(pl->payload_insert(1) == EC_SUCCESS,"Cannot insert payload with id 1");
    TEST_CHECK_(pl->search_id(1) == EC_SUCCESS,"Cannot find payload with inserted before");
    TEST_CHECK_(pl->payload_insert_asc(2) == EC_SUCCESS,"Cannot insert payload with id 1");
    TEST_CHECK_(pl->search_id(2) == EC_SUCCESS,"Cannot find payload with inserted before");
    doc* D = new doc(1);
    D->set_query_ids(pl,2);
    for(unsigned int i = 0 ; i < 2;i++){
        TEST_CHECK_((D->get_query_ids())[i] == i+1,"Cannot find query id we inserted");
    }
    
}
void test_doc_list(void){
    doc_list* dl = new doc_list();
	doc* D;
	D = new doc(1);

    payload_list* pl = new payload_list();

    TEST_CHECK_(pl->payload_insert(1) == EC_SUCCESS,"Cannot insert payload with id 1");
    TEST_CHECK_(pl->search_id(1) == EC_SUCCESS,"Cannot find payload with inserted before");
    TEST_CHECK_(pl->payload_insert_asc(2) == EC_SUCCESS,"Cannot insert payload with id 1");
    TEST_CHECK_(pl->search_id(2) == EC_SUCCESS,"Cannot find payload with inserted before");


    TEST_CHECK_(dl->add_doc(dl,D,pl) != NULL,"Error at allocating doc");
    TEST_CHECK_(dl->add_doc(dl,1) != NULL,"Error at allocating doc");
    
}

void test_hashtable(void){
    TEST_CHECK_(InitializeIndex() == EC_SUCCESS,"Unallocated memory at InitializeIndex");
    char* query_str = new char[strlen("first_word")+1];
    strcpy(query_str,"first_word");
    
    char* query_str2 = new char[strlen("second_word")+1];
    strcpy(query_str2,"second_word");

    char* query_str3 = new char[strlen("first_word second_word")+1];
    strcpy(query_str3,"third_word");

    char* query_str4 = new char[strlen("first_word second_word")+1];
    strcpy(query_str4,"first_word second_word");

    TEST_CHECK_(StartQuery(1,query_str,MT_EXACT_MATCH,0) == EC_SUCCESS,"Unallocated memory at StartQuery on exact match");
    TEST_CHECK_(check_hash_insert(1) == EC_SUCCESS,"Cannot find query id 1 at query hashtable");

    TEST_CHECK_(StartQuery(2,query_str2,MT_EXACT_MATCH,0) == EC_SUCCESS,"Unallocated memory at StartQuery on exact match");
    TEST_CHECK_(check_hash_insert(2) == EC_SUCCESS,"Cannot find query id 2 at query hashtable");

    TEST_CHECK_(StartQuery(3,query_str3,MT_EXACT_MATCH,0) == EC_SUCCESS,"Unallocated memory at StartQuery on exact match");
    TEST_CHECK_(check_hash_insert(3) == EC_SUCCESS,"Cannot find query id 2 at query hashtable");


    TEST_CHECK_(MatchDocument(1,query_str4) == EC_SUCCESS,"Error at match document");

    unsigned int doc_id=0;
    unsigned int num_res=0;
    unsigned int* query_ids=0;

    TEST_CHECK_(GetNextAvailRes(&doc_id,&num_res,&query_ids) == EC_SUCCESS,"Error at GetNextAvailRes");
    //Sto document 1 prepei na toy antistoixoyn mono ta queries 1 kai 2 kathws sto 3 einai diaforetikh le3h me dif > 2
    TEST_CHECK_(doc_id==1,"Not the same document with had insert");
    TEST_CHECK_(num_res==2,"Not the correct amount of result queries");
    TEST_CHECK_(query_ids[0] == 1,"Wrong query result");
    TEST_CHECK_(query_ids[1] == 2,"Wrong query result");
}

void test_hammingindex(void){

    TEST_CHECK_(InitializeIndex() == EC_SUCCESS,"Unallocated memory at InitializeIndex");
    char* query_str = new char[strlen("first_wzrd")+1];
    strcpy(query_str,"first_wzrd");
    
    char* query_str2 = new char[strlen("first_zxcv")+1];
    strcpy(query_str2,"first_zxcv");

    char* query_str3 = new char[strlen("second_wor")+1];
    strcpy(query_str3,"second_wor");

    char* query_str4 = new char[strlen("first_word second_word")+1];
    strcpy(query_str4,"first_word second_word");

    TEST_CHECK_(StartQuery(1,query_str,MT_HAMMING_DIST,1) == EC_SUCCESS,"Unallocated memory at StartQuery on hamming dist");
    TEST_CHECK_(check_hash_insert(1) == EC_SUCCESS,"Cannot find query id 1 at query hashtable");

    TEST_CHECK_(StartQuery(2,query_str2,MT_HAMMING_DIST,2) == EC_SUCCESS,"Unallocated memory at StartQuery on hamming dist");
    TEST_CHECK_(check_hash_insert(2) == EC_SUCCESS,"Cannot find query id 2 at query hashtable");

    TEST_CHECK_(StartQuery(3,query_str3,MT_HAMMING_DIST,2) == EC_SUCCESS,"Unallocated memory at StartQuery on hamming dist");
    TEST_CHECK_(check_hash_insert(3) == EC_SUCCESS,"Cannot find query id 3 at query hashtable");


    TEST_CHECK_(MatchDocument(1,query_str4) == EC_SUCCESS,"Error at match document");

    unsigned int doc_id=0;
    unsigned int num_res=0;
    unsigned int* query_ids=0;

    TEST_CHECK_(GetNextAvailRes(&doc_id,&num_res,&query_ids) == EC_SUCCESS,"Error at GetNextAvailRes");
    //Sto document 1 prepei na toy antistoixoyn mono ta queries 1 kai 2 kathws sto 3 einai diaforetikh le3h me dif > 2
    TEST_CHECK_(doc_id==1,"Not the same document with had insert");
    TEST_CHECK_(num_res==1,"Not the correct amount of result queries");
    TEST_CHECK_(query_ids[0] == 1,"Wrong query result");
}

void test_editindex(void){
    TEST_CHECK_(InitializeIndex() == EC_SUCCESS,"Unallocated memory at InitializeIndex");
    char* query_str = new char[strlen("first_wor")+1];
    strcpy(query_str,"first_wor");
    
    char* query_str2 = new char[strlen("first_zxcvq")+1];
    strcpy(query_str2,"first_zxcvq");

    char* query_str3 = new char[strlen("second_wd")+1];
    strcpy(query_str3,"second_wd");

    char* query_str4 = new char[strlen("first_word second_word")+1];
    strcpy(query_str4,"first_word second_word");

    TEST_CHECK_(StartQuery(1,query_str,MT_EDIT_DIST,1) == EC_SUCCESS,"Unallocated memory at StartQuery on edit dist");
    TEST_CHECK_(check_hash_insert(1) == EC_SUCCESS,"Cannot find query id 1 at query hashtable");

    TEST_CHECK_(StartQuery(2,query_str2,MT_EDIT_DIST,2) == EC_SUCCESS,"Unallocated memory at StartQuery on edit dist");
    TEST_CHECK_(check_hash_insert(2) == EC_SUCCESS,"Cannot find query id 2 at query hashtable");

    TEST_CHECK_(StartQuery(3,query_str3,MT_EDIT_DIST,2) == EC_SUCCESS,"Unallocated memory at StartQuery on edit dist");
    TEST_CHECK_(check_hash_insert(3) == EC_SUCCESS,"Cannot find query id 3 at query hashtable");

    TEST_CHECK_(MatchDocument(1,query_str4) == EC_SUCCESS,"Error at match document");

    unsigned int doc_id=0;
    unsigned int num_res=0;
    unsigned int* query_ids=0;

    TEST_CHECK_(GetNextAvailRes(&doc_id,&num_res,&query_ids) == EC_SUCCESS,"Error at GetNextAvailRes");
    //Sto document 1 prepei na toy antistoixoyn mono ta queries 1 kai 2 kathws sto 3 einai diaforetikh le3h me dif > 2
    TEST_CHECK_(doc_id==1,"Not the same document with had insert");
    TEST_CHECK_(num_res==2,"Not the correct amount of result queries");
    TEST_CHECK_(query_ids[0] == 1,"Wrong query result");
    TEST_CHECK_(query_ids[1] == 3,"Wrong query result");
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
    { "test_payload_node", test_payload_list},
    { "test_doc", test_doc},
    { "test_doc_list", test_doc_list},
    { "test_hashtable", test_hashtable},
    { "test_hammingindex", test_hammingindex},
    { "test_editindex", test_editindex},
    { NULL, NULL }
};