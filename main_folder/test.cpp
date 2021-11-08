#include "acutest.h"
#include "entry.h"
#include <iostream>
using namespace std;

void test_create_entry(void){
    entry * e_test;
    char tmp[] = "wordtest";
    word * word_test = new word(tmp);
    ErrorCode err;
    err = e_test->create_entry(word_test,&e_test);

    TEST_CHECK_(!strcmp(e_test->getword(),word_test->getword()),"Wrong word input");
} 

void test_destroy_entry(void){
    entry * e_test;
    char tmp[] = "wordtest";
    word * word_test = new word(tmp);
    ErrorCode err;
    err = e_test->create_entry(word_test,&e_test);
    err = e_test->destroy_entry(&e_test);

    TEST_CHECK_(e_test==NULL,"Wrong word input");
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
    tmp = strdup("first");

    e_test = new entry(tmp);
    el_test->add_entry(el_test,e_test);

    tmp = strdup("second");
    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    tmp = strdup("third");
    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    TEST_CHECK_(el_test->get_number_entries(el_test)==3,"Wrong number of entries");
}

void test_add_entry(void){
    entry_list * el_test;

    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    entry * e_test;

    char* tmp;
    tmp = strdup("first");

    e_test = new entry(tmp);
    el_test->add_entry(el_test,e_test);

    tmp = strdup("second");
    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    tmp = strdup("third");
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
}

void test_get_first(void){
    entry_list * el_test;

    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    entry * e_test;

    char* tmp;
    tmp = strdup("first");

    e_test = new entry(tmp);
    el_test->add_entry(el_test,e_test);

    tmp = strdup("second");
    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    tmp = strdup("third");
    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    TEST_CHECK_(!strcmp(el_test->get_first(el_test)->getword(),"first"),"Wrong first entry");
}

void test_get_next(void){
    entry_list * el_test;

    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    entry * e_test;

    entry * e_test2;

    char* tmp;
    tmp = strdup("first");

    e_test = new entry(tmp);
    e_test2 = new entry(tmp);
    
    el_test->add_entry(el_test,e_test);

    tmp = strdup("second");
    e_test2->setword(tmp);
    el_test->add_entry(el_test,e_test2);

    tmp = strdup("third");
    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    TEST_CHECK_(!strcmp(el_test->get_next(el_test,e_test2)->getword(),"third"),"Wrong first entry");
}

void test_destroy_entry_list(void){
    entry_list * el_test;

    ErrorCode err;
    err = el_test->create_entry_list(&el_test);

    entry * e_test;

    char* tmp;
    tmp = strdup("first");

    e_test = new entry(tmp);
    el_test->add_entry(el_test,e_test);

    tmp = strdup("second");
    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    tmp = strdup("third");
    e_test->setword(tmp);
    el_test->add_entry(el_test,e_test);

    el_test->destroy_entry_list(&el_test);
    TEST_CHECK_(el_test == NULL,"Wrong first entry");
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
    { NULL, NULL }
};