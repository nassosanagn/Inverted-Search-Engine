#include <cstdlib>
#include <cstring>
#include <iostream>
#include "BKTree.h"
using namespace std;

int main(){
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
    Index1->build_entry_index(El,MT_HAMMING_DIST,Index1);

    cout<<"Print Tree:"<<endl<<endl;
    Index1->printTree();

    char testStr[]="henn";

    word myword(testStr);
    int threshold = 2;
    entry_list* result= new entry_list();

    Index1->lookup_entry_index(&myword,Index1,threshold,result);

    cout<<"Result from lookup with word: "<<testStr<<" and threshhold : "<<threshold<<endl;

    result->print_list(result);

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
    return 0;
}