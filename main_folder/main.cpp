#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "entry.h"
using namespace std;


int main(int argc, char* argv[]){
    char* wordtmp;
    char wordt[] = "nadme";
    char wordt1[] = "11111";
    char wordt2[] = "na222222dme";

    entry* E;
    entry* E1;
    entry* E2;

    word* W = new word(wordt);
    word* W1 = new word(wordt1);
    word* W2 = new word(wordt2);


    create_entry(W,&E);
    create_entry(W1,&E1);
    create_entry(W2,&E2);

    entry_list* el;
    create_entry_list(&el);
    cout<<"A"<<endl;
    cout<<get_number_entries(el)<<endl;
    cout<<"B"<<endl;

    add_entry(el,E);
    add_entry(el,E1);
    add_entry(el,E2);

    cout<<get_number_entries(el)<<endl;
    cout<<"C"<<endl;
    
    destroy_entry_list(&el);
    cout<<get_number_entries(el)<<endl;
    cout<<"B"<<endl;

    return 0;
}