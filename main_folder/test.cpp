#include "acutest.h"
#include "BKTree.h"
#include <iostream>
using namespace std;

void test_create_entry(void){
    entry * e_test;
    char tmp[] = "wordtest";
    word * word_test = new word(tmp);
    ErrorCode err;
    err = e_test->create_entry(word_test,&e_test);

    TEST_CHECK_(!strcmp(e_test->getword(),word_test->getword()),"Wrong word input");
    delete word_test;
} 

void test_destroy_entry(void){
    entry * e_test;
    char tmp[] = "wordtest";
    word * word_test = new word(tmp);
    ErrorCode err;
    err = e_test->create_entry(word_test,&e_test);
    err = e_test->destroy_entry(&e_test);

    TEST_CHECK_(e_test==NULL,"Wrong word input");
    delete word_test;
}

void test_create_entry_list(void){
    entry_list * el_test;
    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    TEST_CHECK_(err == EC_SUCCESS,"Wrong memory allocation");
}

void test_get_number_entries(void){
    entry_list * el_test;

    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    entry * e_test;

    char* tmp;
    tmp = new char[strlen("first") ];
    strcpy(tmp,"first");

    e_test = new entry(tmp);
    el_test->add_entry(el_test,e_test);

    delete[] tmp;
    tmp = new char[strlen("second") ];
    strcpy(tmp,"second"); 

    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    delete[] tmp;
    tmp = new char[strlen("third") ];
    strcpy(tmp,"third");

    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    TEST_CHECK_(el_test->get_number_entries(el_test)==3,"Wrong number of entries");
    delete[] tmp;
}

void test_add_entry(void){
    entry_list * el_test;

    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    entry * e_test;

    char* tmp;
    tmp = new char[strlen("first") ];
    strcpy(tmp,"first");

    e_test = new entry(tmp);
    el_test->add_entry(el_test,e_test);

    delete[] tmp;
    tmp = new char[strlen("second") ];
    strcpy(tmp,"second");

    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    delete[] tmp;
    tmp = new char[strlen("third") ];
    strcpy(tmp,"third"); 

    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    TEST_CHECK_(el_test->get_number_entries(el_test)==3,"Wrong number of entries");
    
    
    e_test = el_test->getfirst();
    int i = 0;
    while(e_test!=NULL){
        switch(i){
            case 0:
                TEST_CASE("first");
                TEST_CHECK_(!strcmp(e_test->getword(),"first"),"Wrong word");
                break;
            case 1:
                TEST_CASE("second");
                TEST_CHECK_(!strcmp(e_test->getword(),"second"),"Wrong word");
                break;
            case 2:
                TEST_CASE("second");
                TEST_CHECK_(!strcmp(e_test->getword(),"third"),"Wrong word");
                break;
        }
        i++;
        e_test = e_test->getnext();
    }
    delete[] tmp;
}

void test_get_first(void){
    entry_list * el_test;

    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    entry * e_test;

    char* tmp;
    tmp = new char[strlen("first") ];
    strcpy(tmp,"first"); 

    e_test = new entry(tmp);
    el_test->add_entry(el_test,e_test);

    delete[] tmp;
    tmp = new char[strlen("second") ];
    strcpy(tmp,"second"); 

    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    delete[] tmp;
    tmp = new char[strlen("third") ];
    strcpy(tmp,"third");
    
    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    TEST_CHECK_(!strcmp(el_test->get_first(el_test)->getword(),"first"),"Wrong first entry");
    delete[] tmp;
}

void test_get_next(void){
    entry_list * el_test;

    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    entry * e_test;

    entry * e_test2;

    char* tmp;
    tmp = new char[strlen("first") ];
    strcpy(tmp,"first"); 

    e_test = new entry(tmp);
    e_test2 = new entry(tmp);
    
    el_test->add_entry(el_test,e_test);

    delete[] tmp;
    tmp = new char[strlen("second") ];
    strcpy(tmp,"second");

    e_test2->setword(tmp);
    el_test->add_entry(el_test,e_test2);

    delete[] tmp;
    tmp = new char[strlen("third") ];
    strcpy(tmp,"third");

    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    TEST_CHECK_(!strcmp(el_test->get_next(el_test,e_test2)->getword(),"third"),"Wrong first entry");
    delete[] tmp;
}

void test_destroy_entry_list(void){
    entry_list * el_test;

    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    entry * e_test;

    char* tmp;
    tmp = new char[strlen("first") ];
    strcpy(tmp,"first");

    e_test = new entry(tmp);
    el_test->add_entry(el_test,e_test);

    delete[] tmp;
    tmp = new char[strlen("second") ];
    strcpy(tmp,"second");

    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    delete[] tmp;
    tmp = new char[strlen("third") ];
    strcpy(tmp,"third");

    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    el_test->destroy_entry_list(&el_test);
    TEST_CHECK_(el_test->get_number_entries(el_test) == 0,"Not empty entry list");
    TEST_CHECK_(el_test == NULL,"Wrong first entry");
    delete[] tmp;
}

void test_build_entry_index(void){
    entry_list* El;
    
    char* tmpStr = new char[strlen("hell")];
    strcpy(tmpStr,"hell");
    entry* E = new entry(tmpStr);

    char* tmpStr1 = new char[strlen("help")];
    strcpy(tmpStr1,"help");
    entry* E1 = new entry(tmpStr1);


    char* tmpStr2 = new char[strlen("fall")];
    strcpy(tmpStr2,"fall");
    entry* E2 = new entry(tmpStr2);


    char* tmpStr3 = new char[strlen("felt")];
    strcpy(tmpStr3,"felt");
    entry* E3 = new entry(tmpStr3);


    char* tmpStr4 = new char[strlen("fell")];
    strcpy(tmpStr4,"fell");
    entry* E4 = new entry(tmpStr4);


    char* tmpStr5 = new char[strlen("small")];
    strcpy(tmpStr5,"small");
    entry* E5 = new entry(tmpStr5);


    char* tmpStr6 = new char[strlen("melt")];
    strcpy(tmpStr6,"melt");
    entry* E6 = new entry(tmpStr6);



    El->create_entry_list(&El);
    El->add_entry(El,E);
    El->add_entry(El,E1);
    El->add_entry(El,E2);
    El->add_entry(El,E3);
    El->add_entry(El,E4);
    El->add_entry(El,E5);
    El->add_entry(El,E6);


    Index* Index1 = new Index();

    TEST_CHECK_(Index1->build_entry_index(El,MT_HAMMING_DIST,Index1) == EC_SUCCESS,"Not empty entry list");

    delete[] tmpStr;
    delete[] tmpStr1;
    delete[] tmpStr2;
    delete[] tmpStr3;
    delete[] tmpStr4;
    delete[] tmpStr5;
    delete[] tmpStr6;
    delete E;
    delete E1;
    delete E2;
    delete E3;
    delete E4;
    delete E5;
    delete E6;
    El->destroy_entry_list(&El);
    delete El;
    delete Index1;
}

void test_lookup_entry_index(void){
    entry_list* El;
    
    char* tmpStr = new char[strlen("hell")];
    strcpy(tmpStr,"hell");
    entry* E = new entry(tmpStr);

    char* tmpStr1 = new char[strlen("help")];
    strcpy(tmpStr1,"help");
    entry* E1 = new entry(tmpStr1);


    char* tmpStr2 = new char[strlen("fall")];
    strcpy(tmpStr2,"fall");
    entry* E2 = new entry(tmpStr2);


    char* tmpStr3 = new char[strlen("felt")];
    strcpy(tmpStr3,"felt");
    entry* E3 = new entry(tmpStr3);


    char* tmpStr4 = new char[strlen("fell")];
    strcpy(tmpStr4,"fell");
    entry* E4 = new entry(tmpStr4);


    char* tmpStr5 = new char[strlen("small")];
    strcpy(tmpStr5,"small");
    entry* E5 = new entry(tmpStr5);


    char* tmpStr6 = new char[strlen("melt")];
    strcpy(tmpStr6,"melt");
    entry* E6 = new entry(tmpStr6);



    El->create_entry_list(&El);
    El->add_entry(El,E);
    El->add_entry(El,E1);
    El->add_entry(El,E2);
    El->add_entry(El,E3);
    El->add_entry(El,E4);
    El->add_entry(El,E5);
    El->add_entry(El,E6);


    Index* Index1 = new Index();
    TEST_CHECK_(Index1->build_entry_index(El,MT_HAMMING_DIST,Index1) == EC_SUCCESS,"Not empty entry list");



    char testStr[]="henn";

    word myword(testStr);
    int threshold = 2;
    entry_list* result= new entry_list();

    Index1->lookup_entry_index(&myword,Index1,threshold,result);

    char* tmpStr8 = new char[strlen("hell")];
    strcpy(tmpStr8,"hell");
    entry* E8 = new entry(tmpStr8);

    char* tmpStr9 = new char[strlen("help")];
    strcpy(tmpStr9,"help");
    entry* E9 = new entry(tmpStr9);

    entry_list* El_tmp;
    El_tmp->create_entry_list(&El_tmp);
    El_tmp->add_entry(El_tmp,E8);
    El_tmp->add_entry(El_tmp,E9);

    TEST_CHECK_( El_tmp->list_similarity(El_tmp,result) == EC_SUCCESS,"Not empty entry list");


    result->destroy_entry_list(&result);
    delete[] tmpStr;
    delete[] tmpStr1;
    delete[] tmpStr2;
    delete[] tmpStr3;
    delete[] tmpStr4;
    delete[] tmpStr5;
    delete[] tmpStr6;
    delete E;
    delete E1;
    delete E2;
    delete E3;
    delete E4;
    delete E5;
    delete E6;
    El->destroy_entry_list(&El);
    delete El;
    delete result;
    delete Index1;
}

void test_destroy_entry_index(void){
    entry_list* El;
    
    char* tmpStr = new char[strlen("hell")];
    strcpy(tmpStr,"hell");
    entry* E = new entry(tmpStr);

    char* tmpStr1 = new char[strlen("help")];
    strcpy(tmpStr1,"help");
    entry* E1 = new entry(tmpStr1);


    char* tmpStr2 = new char[strlen("fall")];
    strcpy(tmpStr2,"fall");
    entry* E2 = new entry(tmpStr2);


    char* tmpStr3 = new char[strlen("felt")];
    strcpy(tmpStr3,"felt");
    entry* E3 = new entry(tmpStr3);


    char* tmpStr4 = new char[strlen("fell")];
    strcpy(tmpStr4,"fell");
    entry* E4 = new entry(tmpStr4);


    char* tmpStr5 = new char[strlen("small")];
    strcpy(tmpStr5,"small");
    entry* E5 = new entry(tmpStr5);


    char* tmpStr6 = new char[strlen("melt")];
    strcpy(tmpStr6,"melt");
    entry* E6 = new entry(tmpStr6);



    El->create_entry_list(&El);
    El->add_entry(El,E);
    El->add_entry(El,E1);
    El->add_entry(El,E2);
    El->add_entry(El,E3);
    El->add_entry(El,E4);
    El->add_entry(El,E5);
    El->add_entry(El,E6);


    Index* Index1 = new Index();
    TEST_CHECK_(Index1->build_entry_index(El,MT_HAMMING_DIST,Index1) == EC_SUCCESS,"Not empty entry list");
    TEST_CHECK_(Index1->destroy_entry_index(Index1->getRoot()) == EC_SUCCESS,"Not empty entry list");



    delete[] tmpStr;
    delete[] tmpStr1;
    delete[] tmpStr2;
    delete[] tmpStr3;
    delete[] tmpStr4;
    delete[] tmpStr5;
    delete[] tmpStr6;
    delete E;
    delete E1;
    delete E2;
    delete E3;
    delete E4;
    delete E5;
    delete E6;
    El->destroy_entry_list(&El);
    delete El;
}

TEST_LIST = {
    { "test_create_entry", test_create_entry },
    { "test_destroy_entry", test_destroy_entry },
    { "test_create_entry_list", test_create_entry_list },
    { "test_get_number_entries", test_get_number_entries },
    { "test_add_entry", test_add_entry },
    { "test_get_first", test_get_first },
    { "test_get_next", test_get_next },
    { "test_destroy_entry_list", test_destroy_entry_list },
    { "test_build_entry_index", test_build_entry_index },
    { "test_lookup_entry_index", test_lookup_entry_index },
    { "test_destroy_entry_index", test_destroy_entry_index},
    { NULL, NULL }
};