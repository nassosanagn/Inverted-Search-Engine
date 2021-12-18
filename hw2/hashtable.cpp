#include "hashtable.h"
// #include <bits/stdc++.h>
using namespace std;
  
    


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

ErrorCode Hashtable::search(word *W,query_Hashtable* Q_hash,int current_doc,payload_list* q_result){
    int func_out = hash_function(W->getword(),size);
    entry* e = buckets[func_out]->search_word(W);
    if(e){
        payload_node* pNode = e->getpayload()->getFirst();

        while(pNode != NULL){
            if (Q_hash->add_one(e->getmyword(), pNode->getId(),current_doc) == EC_SUCCESS){
                q_result->payload_insert_asc(pNode->getId());
            }
            pNode = pNode->getNext();
        }
    }
    return EC_SUCCESS;
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