#include "query.h"
using namespace std;

//contructor

query::query(QueryID qid,char * str,MatchType m_type,unsigned int m_dist){
    query_id = qid;
    char * pch;
    pch = strtok (str," ");
    int i = 0;
    while (pch != NULL)
    {
        word_arr[i].setword(pch);
        pch = strtok (NULL, " ");
        i++;
    }
    match_type = m_type;
    match_dist = m_dist;
    next = NULL;
}


//Dhmioyrgei to query list
ErrorCode query_list::create_query_list(query_list** el){
    (*el) = new query_list;
    (*el)->setfirst(NULL);
    if((*el)==NULL){
        return EC_FAIL;
    }
    return EC_SUCCESS;
}


//Prosthetei ena query sto telos ths listas
ErrorCode query_list::add_query(query_list* el, QueryID qid,char * str,MatchType m_type,unsigned int m_dist){
    if(el == NULL){
        return EC_FAIL;
    }
    //Dhmioyrgei ton komvo poy tha prosthethei
    query* query_n = new query(qid,str,m_type,m_dist);
    query* query_tmp = el->getfirst();
    //An den yparxei allos komvos
    if(el->getfirst() == NULL){
        el->setfirst(query_n);
        return EC_SUCCESS;
    }
    //Phgainei sto telos ths listas
    while(query_tmp->get_next() != NULL){
        query_tmp = query_tmp->get_next();
    }
    //Prosthetei ton komvo san epomeno
    query_tmp->set_next(query_n);
    return EC_SUCCESS;
}

query* query_list::get_first(const query_list* el){
    return el->getfirst();
}

//Diagrafei kathe komvo ths listas
ErrorCode query_list::destroy_query_list(query_list** el){
    if((*el)==NULL){
        return EC_FAIL;
    }
    query* curr = (*el)->getfirst();
    query* next = NULL;
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

int main(){
    char* tmpStr = new char[strlen("hell")];
    strcpy(tmpStr,"hell");
    query* Q = new query(3,tmpStr,MT_HAMMING_DIST,2);
    cout<< Q->get_id()<<endl;
    cout<< Q->get_match_type()<<endl;
    cout<< Q->get_dist()<<endl;
    word *W = Q->get_word_arr();
    for(int i = 0 ; i < 5 ; i++){
        cout<<W[i].getword();
    }
    cout<<endl;
    return 0;
}