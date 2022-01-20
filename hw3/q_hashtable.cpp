#include "q_hashtable.h"
// #include <bits/stdc++.h>
using namespace std;


/* ---------------------------------------------------------------- query_hash_node functions ---------------------------------------------------------------- */

query_hash_node::query_hash_node(QueryID qid,const char * str, unsigned int m_dist){
    query_id = qid;
    word_count = 0;
    alive = 1;
    words_found = 0;
    curr_doc = 0;
    char *pch;
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

    delete []Str;
}

void query_hash_node::reset_val(){
    for(unsigned int i = 0; i < word_count; i++){
        word_c[i] = 0;
    }
    words_found = 0;
}


/* ---------------------------------------------------------------- query_hash_list functions ---------------------------------------------------------------- */

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

ErrorCode query_hash_list::print_list(){
    
    if (first == NULL){
        return EC_FAIL;
    }
    query_hash_node* current = first;
    while (current != NULL){
        cout << "ID = "<<current->get_id() << " get_word_count: "<<current->get_word_count()<<" words found: "<<current->get_word_found()<<" CURR "<<current->get_curr_doc()<<" ";
        for(unsigned int i = 0 ;i<current->get_word_count();i++){
            cout<<(current->get_word_c())[i]<<" ";
        }
        current = current->get_next();
    }
    cout << endl;
    return EC_SUCCESS;
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

/* ---------------------------------------------------------------- query_Hashtable functions ---------------------------------------------------------------- */

query_Hashtable::query_Hashtable(){
    size = Q_SIZE;
    counter = 0 ;
    buckets = new query_hash_list*[size];
    for(int i =0;i<size;i++){
        buckets[i] = new query_hash_list();
    }
}

query_Hashtable::~query_Hashtable(){
  
    // for(int i = 0; i < size;i++){
    //     buckets[i]->destroy_query_list(&buckets[i]);
    //     // delete []buckets[i];
    // }
    // delete buckets;
}

query_hash_node* query_Hashtable::insert(QueryID qid,const char * str,unsigned int m_dist){
    int func_out = hash_function(qid,size);
    query_hash_node* qhn = buckets[func_out]->add_query(buckets[func_out],qid,str,m_dist);
    counter++;
    return qhn;
}

query_hash_node* query_Hashtable::search(QueryID qid){
    int func_out = hash_function(qid,size);
    return buckets[func_out]->search_id(qid);
}

unsigned long query_Hashtable::hash_function(int id,int size_tmp){
    return id % size_tmp;
}

ErrorCode query_Hashtable::print(){
    for(int i=0;i<size;i++){
        cout<<"bucket with id "<<i<<" ";
        buckets[i]->print_list();
        cout<<endl;
    }
    return EC_SUCCESS;
}

ErrorCode query_Hashtable::add_one(word* myword, int qid, unsigned int current_doc){
            
    query_hash_node* qNode;
    int func_out = hash_function(qid,size);
    qNode = buckets[func_out]->search_id(qid);           
    if(qNode->get_alive() == 0){
        return EC_FAIL;
    }
    if(qNode->get_curr_doc() != current_doc){
        qNode->set_curr_doc(current_doc);
        qNode->reset_val();
    }

    if (qNode->get_word_found() == qNode->get_word_count()){
        return EC_FAIL;
    }

    for(unsigned int i = 0; i < qNode->get_word_count(); i++){
        if ((!strcmp(((qNode->get_word_arr())[i]).getword(),myword->getword())) && ((qNode->get_word_c())[i] == 0)){
            qNode->set_found(i);
        }
    }
    
    if (qNode->get_word_found() == qNode->get_word_count()){
        return EC_SUCCESS;
    }

    return EC_FAIL;
}

ErrorCode query_Hashtable::add_one_tree(word* myword, int qid, unsigned int current_doc, unsigned int threshold){
    query_hash_node* qNode;
    int func_out = hash_function(qid,size);
    qNode = buckets[func_out]->search_id(qid);           
    if(qNode->get_alive() == 0){
        return EC_FAIL;
    }
    if(qNode->get_curr_doc() != current_doc){
        qNode->set_curr_doc(current_doc);
        qNode->reset_val();
    }
    
    if (qNode->get_word_found() == qNode->get_word_count()){
        return EC_FAIL;
    }

    if(qNode->get_dist() < threshold){
        return EC_FAIL;
    }
    for(unsigned int i = 0; i < qNode->get_word_count(); i++){
        if ((!strcmp(((qNode->get_word_arr())[i]).getword(),myword->getword()) ) && ((qNode->get_word_c())[i] == 0)){
            qNode->set_found(i);
        }
    }
    if (qNode->get_word_found() == qNode->get_word_count()){
        return EC_SUCCESS;
    }

    return EC_FAIL;
}

ErrorCode query_Hashtable::add_one_payload(payload_list* pl,word* w,int current_doc,int threshold,payload_list* q_result){
    payload_node* pn;
    pn = pl->getFirst();
    while(pn!=NULL){
        if(add_one_tree(w,pn->getId(),current_doc,threshold) == EC_SUCCESS){
            q_result->payload_insert_asc(pn->getId());
        }
        pn = pn->getNext();
    }

    return EC_SUCCESS;
}

ErrorCode query_Hashtable::delete_query(int qid){
    query_hash_node* qNode;
    int func_out = hash_function(qid,size);
    qNode = buckets[func_out]->search_id(qid);
    qNode->set_alive();

    return EC_SUCCESS;
}
/* ------------------------------------------------------------------------------------------------------------------------------------------------------------- */
