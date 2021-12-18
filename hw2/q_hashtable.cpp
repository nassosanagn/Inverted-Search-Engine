#include "q_hashtable.h"
// #include <bits/stdc++.h>
using namespace std;
  
unsigned long query_Hashtable::hash_function(int id,int size_tmp){
    return id % size_tmp;
}

query_Hashtable::query_Hashtable(){
    size = Q_SIZE;
    counter = 0 ;
    buckets = new query_hash_list*[size];
    for(int i =0;i<size;i++){
        buckets[i] = new query_hash_list();
    }
}

ErrorCode query_Hashtable::insert(QueryID qid,const char * str,unsigned int m_dist){
    int func_out = hash_function(qid,size);
    buckets[func_out]->add_query(buckets[func_out],qid,str,m_dist);
    counter++;
    return EC_SUCCESS;
}

query_hash_node* query_Hashtable::search(QueryID qid){
    int func_out = hash_function(qid,size);
    return buckets[func_out]->search_id(qid);
}

ErrorCode query_Hashtable::print(){
    for(int i=0;i<size;i++){
        cout<<"bucket with id "<<i<<" ";
        buckets[i]->print_list();
        cout<<endl;
    }
    return EC_SUCCESS;
}

query_hash_node::query_hash_node(QueryID qid,const char * str,unsigned int m_dist){
    query_id = qid;
    word_count = 0;
    alive = 1;
    words_found = 0;
    curr_doc = 0;
    char * pch;
    char* Str = new char[strlen(str)+1];
    strcpy(Str,str);
    pch = strtok (Str," ");
    int i = 0;
    while (pch != NULL)
    {
        word_count++;
        word_arr[i].setword(pch);
        word_c[i] = 0;
        pch = strtok (NULL, " ");
        i++;
    }
    match_dist = m_dist;
    next = NULL;
}


//Dhmioyrgei to query list
ErrorCode query_hash_list::create_query_list(query_hash_list** el){
    (*el) = new query_hash_list;
    (*el)->setfirst(NULL);
    if((*el)==NULL){
        return EC_FAIL;
    }
    return EC_SUCCESS;
}


//Prosthetei ena query sto telos ths listas
query_hash_node* query_hash_list::add_query(query_hash_list* el, QueryID qid,const char * str,unsigned int m_dist){
    if(el == NULL){
        return NULL;
    }
    //Dhmioyrgei ton komvo poy tha prosthethei
    query_hash_node* query_n = new query_hash_node(qid,str,m_dist);
    query_hash_node* query_tmp = el->getfirst();
    //An den yparxei allos komvos
    if(el->getfirst() == NULL){
        el->setfirst(query_n);
        return query_n;
    }
    //Phgainei sto telos ths listas
    while(query_tmp->get_next() != NULL){
        query_tmp = query_tmp->get_next();
    }
    //Prosthetei ton komvo san epomeno
    query_tmp->set_next(query_n);
    return query_n;
}

query_hash_node* query_hash_list::get_first(const query_hash_list* el){
    return el->getfirst();
}

//Diagrafei kathe komvo ths listas
ErrorCode query_hash_list::destroy_query_list(query_hash_list** el){
    if((*el)==NULL){
        return EC_FAIL;
    }
    query_hash_node* curr = (*el)->getfirst();
    query_hash_node* next = NULL;
    //Diatrexei thn lista kai diagrafei toys komvoys
    while(curr != NULL){
        next = curr->get_next();

        delete curr;

        curr = next;
    }
    delete (*el);
    (*el) = NULL;
    return EC_SUCCESS;
}

query_hash_node* query_hash_list::search_id(unsigned int id){
    query_hash_node* qn = first;
    while(qn!=NULL){
        if(qn->get_id() == id){
            return qn;
        }
        qn = qn->get_next();
    }
    return NULL;
}

ErrorCode query_hash_list::delete_query(QueryID query_id){
    query_hash_node* temp = first;

    if (temp != NULL && temp->get_id() == query_id){
        first = temp->get_next();
        delete temp;
        return EC_SUCCESS;
    }
    
    while(temp->get_next()!=NULL){
        if(temp->get_next()->get_id()==query_id){
            query_hash_node* tmp2 = temp->get_next()->get_next();
            delete temp->get_next();
            temp->set_next(tmp2);
        }
        temp = temp->get_next();
    }
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