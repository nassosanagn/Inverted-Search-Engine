#include "hashtable.h"
// #include <bits/stdc++.h>
using namespace std;
  
    
int editDistDP(string str1, string str2, int m, int n){
    int len1 = str1.length();
    int len2 = str2.length();
    int edit_arr[len1 + 1][len2 + 1];
    int t = 0;
    for (int i = 0; i < len1+1; i++) {
        for (int j = 0; j < len2+1; j++) {
            if (i == 0){
                edit_arr[i][j] = j;
            }
            else if (j == 0){
                edit_arr[i][j] = i;
            }
            else if (str1[i - 1] == str2[j - 1]){
                edit_arr[i][j] = edit_arr[i - 1][j - 1];
            }
            else{
                t = (edit_arr[i - 1][j] < edit_arr[i - 1][j - 1] ? edit_arr[i - 1][j] : edit_arr[i - 1][j - 1]);
                edit_arr[i][j] = 1 + (edit_arr[i][j - 1] < t? edit_arr[i][j - 1] : t);
            }
        }
    }
  
    return edit_arr[len1][len2];
}

unsigned long Hashtable::hash_function(char* str,int size_tmp){
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return i % size_tmp;
}

Hashtable::Hashtable(){
    size = SIZE;
    counter = 0 ;
    buckets = new entry_list*[size];
    for(int i =0;i<size;i++){
        buckets[i] = new entry_list();
    }
}

ErrorCode Hashtable::insert(entry* entry_tmp,int id){
    int func_out = hash_function(entry_tmp->getword(),size);
    entry* e = buckets[func_out]->search_word(entry_tmp->getmyword());
    if(e == NULL){
        buckets[func_out]->add_entry(buckets[func_out],entry_tmp,id);
        counter++;
    }
    else{
        e->getpayload()->payload_insert(id);
    }
    return EC_SUCCESS;
}

entry* Hashtable::search(word *W){
    int func_out = hash_function(W->getword(),size);
    return buckets[func_out]->search_word(W);
}

ErrorCode Hashtable::print(){
    for(int i=0;i<size;i++){
        cout<<"bucket with id "<<i<<" ";
        buckets[i]->print_list(buckets[i]);
        cout<<endl<<endl;
    }
    return EC_SUCCESS;
}

ErrorCode Hashtable::rehash(){
    int new_size = size * 2;
    int new_count = 0;
    entry_list** new_buckets;
    new_buckets = new entry_list*[new_size];
    for(int i =0;i<new_size;i++){
        new_buckets[i] = new entry_list();
    }
    for(int i = 0;i<size;i++){
        entry* e = buckets[i]->getfirst();
        while(e!=NULL){
            int func_out = hash_function(e->getword(),new_size);
            if(new_buckets[func_out]->getfirst()==NULL){
                new_count++;
            }
            new_buckets[func_out]->add_entry(new_buckets[func_out],e,-1);
            e = e->getnext();
        }
    }
    counter = new_count;
    size = new_size;
    buckets = new_buckets;
    return EC_SUCCESS;
}

// int main(){

//     Hashtable* hash;
//     hash = new Hashtable();
//     hash->create_hashtable();

//     char* tmpStr = new char[strlen("hell")];
//     strcpy(tmpStr,"hell");
//     entry* E = new entry(tmpStr);

//     char* tmpStr1 = new char[strlen("help")];
//     strcpy(tmpStr1,"help");
//     entry* E1 = new entry(tmpStr1);

//     char* tmpStr2 = new char[strlen("fall")];
//     strcpy(tmpStr2,"fall");
//     entry* E2 = new entry(tmpStr2);

//     char* tmpStr3 = new char[strlen("felt")];
//     strcpy(tmpStr3,"felt");
//     entry* E3 = new entry(tmpStr3);

//     char* tmpStr4 = new char[strlen("fell")];
//     strcpy(tmpStr4,"fell");
//     entry* E4 = new entry(tmpStr4);

//     char* tmpStr5 = new char[strlen("small")];
//     strcpy(tmpStr5,"small");
//     entry* E5 = new entry(tmpStr5);

//     char* tmpStr6 = new char[strlen("melt")];
//     strcpy(tmpStr6,"melt");
//     entry* E6 = new entry(tmpStr6);

//     hash->insert(E,0);
//     hash->insert(E1,1);
//     hash->insert(E2,2);
//     hash->insert(E3,3);
//     hash->insert(E4,4);
//     hash->insert(E5,5);
//     hash->insert(E6,6);
//     hash->print();
//     return 0;
// }