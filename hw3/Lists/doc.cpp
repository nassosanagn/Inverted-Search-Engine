#include "doc.h"
using namespace std;

//contructor

doc::doc(DocID did){
    doc_id = did;
    next = NULL;
    num_res = 0;
    query_ids = NULL;
}

void doc::set_query_ids(payload_list* tmp,int num){
            
    query_ids = new QueryID[num];
    payload_node* P = tmp->getFirst();
    int i = 0;
    while(P!=NULL){
        query_ids[i] = P->getId();
        i++;
        P = P->getNext();
    }
}

/* ---------------------------------------------------------------- doc_list functions ---------------------------------------------------------------- */

//Dhmioyrgei to doc list
ErrorCode doc_list::create_doc_list(doc_list** el){
    (*el) = new doc_list;
    (*el)->setfirst(NULL);
    if((*el)==NULL){
        return EC_FAIL;
    }
    return EC_SUCCESS;
}


//Prosthetei ena doc sto telos ths listas
doc* doc_list::add_doc(doc_list* el, DocID did){
    if(el == NULL){
        return NULL;
    }
    //Dhmioyrgei ton komvo poy tha prosthethei
    doc* doc_n = new doc(did);
    doc* doc_tmp = el->get_last();
    //An den yparxei allos komvos
    if(el->getfirst() == NULL){
        el->setfirst(doc_n);
        el->set_last(doc_n);
        return doc_n;
    }
    //Prosthetei ton komvo san epomeno
    doc_tmp->set_next(doc_n);
    this->set_last(doc_n);
    return doc_n;
}

doc* doc_list::add_doc(doc_list* el, doc* D_temp, payload_list* pl){
    if (el == NULL){
        return NULL;
    }
    //Dhmioyrgei ton komvo poy tha prosthethei
    doc* doc_n = new doc(D_temp->get_id());
    doc_n->set_num_res(D_temp->get_num_res());
    doc_n->set_query_ids(pl,D_temp->get_num_res());

    doc* doc_tmp = el->get_last();
    //An den yparxei allos komvos
    if(el->getfirst() == NULL){
        el->setfirst(doc_n);
        el->set_last(doc_n);
        return doc_n;
    }

    doc_tmp->set_next(doc_n);
    this->set_last(doc_n);

    return doc_n;
} 


doc* doc_list::get_first(const doc_list* el){
    return el->getfirst();
}


//Diagrafei kathe komvo ths listas
ErrorCode doc_list::destroy_doc_list(doc_list** el){
    if((*el)==NULL){
        return EC_FAIL;
    }
    doc* curr = (*el)->getfirst();
    doc* next = NULL;
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
