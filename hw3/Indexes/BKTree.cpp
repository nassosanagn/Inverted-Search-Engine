using namespace std;

#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "BKTree.h"

//constructor gia to Index
Index::Index(){ 
    root = NULL;
}

//destructor
Index::~Index(){
    //kalei synarthsh diagrafhs kathe komvoy toy dentroy
    destroy_entry_index(root); 
}

//Diagrafei anadromika oloys toys komvoys toy dentroy
ErrorCode Index::destroy_entry_index(treeNode* ix){
    if (ix != NULL){
        //Gia kathe komvo sto idio epipedo
        destroy_entry_index(ix->getnextNode());
        //Gia kathe paidi toy
        destroy_entry_index(ix->getChildNode());
        //Diagrafei ton komvo
        delete ix->getEntry();
        delete ix;
    }
    return EC_SUCCESS;
}

//Synarthsh gia eyresh hamming distance 

int Index::HammingDistance(const char* word1, const char* word2){
    
    int j = strlen(word1)-strlen(word2);
    if (j < 0){j=-j;}
    for(size_t i = 0;i<(strlen(word1) < strlen(word2) ? strlen(word1) : strlen(word2));i++){
        if(word1[strlen(word1)-i-1]!=word2[strlen(word2)-i-1]){
            j++;
        }
    }
    return j;
}

// int Index::EditDistance(char *str1,int str1Len, char *str2, int str2Len) {
//   int row, column;
//   int array[str2Len + 1][str1Len + 1];
//   array[0][0] = 0;
//   for (row = 1; row <= str2Len; row++)
//     array[row][0] = array[row - 1][0] + 1;
//   for (column = 1; column <= str1Len; column++)
//     array[0][column] = array[0][column - 1] + 1;
//   for (row = 1; row <= str2Len; row++) {
//     for (column = 1; column <= str1Len; column++) {
//       int upper_cell = array[row - 1][column] + 1;
//       int left_cell = array[row][column - 1] + 1;
//       int diagonal_cell = array[row - 1][column - 1];
//       if (str1[column - 1] != str2[row - 1]) {
//         diagonal_cell++;
//       }
//       if (upper_cell < left_cell) {
//         if (upper_cell < diagonal_cell) {
//           array[row][column] = upper_cell;
//         } else {
//           array[row][column] = diagonal_cell;
//         }
//       } else {
//         if (left_cell < diagonal_cell) {
//           array[row][column] = left_cell;
//         } else {
//           array[row][column] = diagonal_cell;
//         }
//       }
//     }
//   }

//   return array[str2Len][str1Len];
// }

int Index::EditDistance(char* a, int na, char* b, int nb)
{
	int oo=0x7FFFFFFF;

	static int T[2][MAX_WORD_LENGTH+1];

	int ia, ib;

	int cur=0;
	ia=0;

	for(ib=0;ib<=nb;ib++)
		T[cur][ib]=ib;

	cur=1-cur;

	for(ia=1;ia<=na;ia++)
	{
		for(ib=0;ib<=nb;ib++)
			T[cur][ib]=oo;

		int ib_st=0;
		int ib_en=nb;

		if(ib_st==0)
		{
			ib=0;
			T[cur][ib]=ia;
			ib_st++;
		}

		for(ib=ib_st;ib<=ib_en;ib++)
		{
			int ret=oo;

			int d1=T[1-cur][ib]+1;
			int d2=T[cur][ib-1]+1;
			int d3=T[1-cur][ib-1]; if(a[ia-1]!=b[ib-1]) d3++;

			if(d1<ret) ret=d1;
			if(d2<ret) ret=d2;
			if(d3<ret) ret=d3;

			T[cur][ib]=ret;
		}

		cur=1-cur;
	}
	int ret=T[1-cur][nb];
	return ret;
}

ErrorCode Index::build_entry_index(const entry_list* el, MatchType type, Index* ix, int qid){

    entry* currentEntry = el->getfirst();

    if (currentEntry != NULL){
        ix->root = new treeNode(currentEntry,0);
        currentEntry = currentEntry->getnext();
    }

    while (currentEntry != NULL){
        ix->insertTree(currentEntry, ix->getRoot()->getString(), ix->getRoot(),MT_HAMMING_DIST,qid);
        currentEntry = currentEntry->getnext();
    }

    return EC_SUCCESS;
}

ErrorCode Index::insertWord(word* W, Index* ix, MatchType mt, int qid){

    entry* tempentry = new entry(W->getword());
    if(ix->getRoot() == NULL){
        ix->root = new treeNode(tempentry,0);
        ix->root->getEntry()->getpayload()->payload_insert(qid);
        return EC_SUCCESS;
    }
    ix->insertTree(tempentry,ix->getRoot()->getString(), ix->getRoot(),mt, qid);
        
    return EC_SUCCESS;
    
    
}

//Eisagwgh komvoy sto dentro
ErrorCode Index::insertTree(entry* entry, char* cmpWord, treeNode* tempNode, MatchType matchtype, int qid){
    setmatchtype(matchtype);
    int tempDiff = 0;
    char* str = entry->getword();
    if(tempNode == root){
        if(!strcmp(str,tempNode->getString())){
            tempNode->getEntry()->getpayload()->payload_insert(qid);
            return EC_FAIL;
        }
    }
    //Analoga to matchtype kalei kai thn katallhlh synarthsh metrhshs
    switch(matchtype){
        case MT_HAMMING_DIST:
            tempDiff = HammingDistance(str, cmpWord);
            break;
        case MT_EDIT_DIST:
            tempDiff = EditDistance(str, strlen(str),cmpWord, strlen(cmpWord));
            break;
        default:
            break;
    }
    
    // An den yparxei allo paidi ths rizas 
    if (this->getRoot()->getChildNode() == NULL){                     // Only for the first time 
        getRoot()->setChildNode(new treeNode(entry, tempDiff),qid);
        return EC_SUCCESS;
    }

    if (tempNode == this->getRoot()){                /* Start from the root's child */
        tempNode = this->getRoot()->getChildNode();
    }

    if ((tempDiff == tempNode->getDiff())) {        /* Go down on that node */
        if(!strcmp(str,tempNode->getString())){
            tempNode->getEntry()->getpayload()->payload_insert(qid);
            return EC_FAIL;
        }
        if (tempNode->getChildNode() == NULL){
            switch(matchtype){
                case MT_HAMMING_DIST:
                    tempDiff = HammingDistance(str, tempNode->getString());
                    break;
                case MT_EDIT_DIST:
                    tempDiff = EditDistance(str,strlen(str),tempNode->getString(),strlen(tempNode->getString()));
                    break;
                default:
                    break;
            }
            tempNode->setChildNode(new treeNode(entry, tempDiff),qid);
        }else{
            if(insertTree(entry, tempNode->getString(), tempNode->getChildNode(),matchtype,qid)==EC_FAIL){
                return EC_FAIL;
            }
        }
    }else{                              /* Go right on that node */
        if (tempNode->getnextNode() == NULL){
            tempNode->setNextNode(new treeNode(entry, tempDiff),qid);
        }else{
            if(insertTree(entry, cmpWord, tempNode->getnextNode(),matchtype,qid)==EC_FAIL){
                return EC_FAIL;
            }
        }
    }

    return EC_SUCCESS;
}


//TESTING FUNCTIONS

//Ektypwsh kathe komvoy
void treeNode::print_all(){
    this->print_children();
    treeNode* next = this->getnextNode();
    treeNode* child = this->getChildNode();
    if (next!= NULL) next->print_all();
    if (child!= NULL) child->print_all();
}

//Ektypwsh paidiwn
void treeNode::print_children(){
    treeNode* tempNode = this->getChildNode();
    cout <<"Parent "<< this->getString()/* << " with payload "*/;
    // this->getEntry()->getpayload()->print_list();
    cout<< endl;
    if (tempNode == NULL)
    {
        cout<<endl;
        return;
    }
    cout <<"Children ";
    while (tempNode!= NULL)
    {
        cout << tempNode->getString() <<" "<< tempNode->getDiff()<<"  ";
        tempNode = tempNode->getnextNode();
    }
    cout << endl;
    
}

void Index::printTree(){
    if (this->getRoot()!=NULL)
    {
        this->getRoot()->print_all();
    }
    
}

//TESTING FUNCTIONS OVER


//Prosthkh komvoy sthn lista
void BKList_node::add_node(treeNode* input,int threshold){
    if (nextnode == NULL)
    {
        nextnode = new BKList_node(input,threshold);
    }
    else
    {
        nextnode->add_node(input,threshold);
    }
}

//Epistrfei ton prwto komvo kai ton diagrafei apo thn lista
treeNode* BKList::popfirst(int* threshold){
    BKList_node* temp = first->getnext();
    treeNode* return_val=first->getnode();
    (*threshold) = first->get_threshold();
    delete first;
    first = temp; 
    return return_val;
}

// Syanrthsh gia thn eyresh omoiwn le3ewn

ErrorCode Index::lookup_entry_index(const word* w, Index* ix, int threshold, MatchType m_type,query_Hashtable* Q_hash,int current_doc,payload_list* q_result){
    
    pthread_mutex_lock(&mutexqhash);

    //Lista ypopshfiwn le3ewn
    treeNode* root = ix->getRoot();
    if (root==NULL)
    {   
        pthread_mutex_unlock(&mutexqhash);
        return EC_FAIL;
    }
    // BKList_node* blns = new BKList_node(root,threshold);
    BKList* cand_list = new BKList(root,threshold);
    //An den yparxei dentro
    //proswrinh le3h gia thn dimhoyrgia entry struct
    char *tmpStr = new char[strlen("tmp")+1];
    strcpy(tmpStr,"k");

    //entry gia thn dhmioyrgia listas le3ewn gia epistrofh
    entry* input_entry = new entry(tmpStr);

    // Oso den yparxoyn alles ypopshfies le3eis
    while (cand_list->getfirst()!=NULL){
        //Dexetai ton prwto komvo-le3h apo thn lista
        treeNode* current_candidate = cand_list->popfirst(&threshold);
        //Ypologizei thn apostash
        int word_dis = 0;
        switch(m_type){
            case MT_HAMMING_DIST:
                word_dis = HammingDistance(w->getword(),current_candidate->getString());
                break;
            case MT_EDIT_DIST:
                word_dis = EditDistance(w->getword(),strlen(w->getword()),current_candidate->getString(),strlen(current_candidate->getString()));
                break;
            default:
                break;
        }
        //An einai omoio me thn le3h

        for(int i = threshold;i<=3;i++){
            if (word_dis <= i)
            {
                input_entry->setword(current_candidate->getString());
                input_entry->setpayload(current_candidate->getEntry()->getpayload());
                
                Q_hash->add_one_payload(current_candidate->getEntry()->getpayload(),current_candidate->getWord(),current_doc,i,q_result);
                // pthread_mutex_unlock(&mutexqhash);
                //add_entry(hashtable,input_entry,-1); sto hashmap
                //an to query exei megalytero h iso match dist apo to i tote kanei insert
                break;
            }
        }

        //Ypologizei ta oria twn apostasewn gia na elegthoyn
        //Gia kathe paidi poy vrisketai endiamesa twn oriwn ta prosthetei san candidate
        treeNode* temp_tnode=current_candidate->getChildNode();
        while (temp_tnode!= NULL)
        {
            for(int i = threshold;i<=3;i++){
                int left = word_dis - i;
                int right = word_dis + i;
                if (left<=temp_tnode->getDiff() && temp_tnode->getDiff()<=right)
                {
                    cand_list->add_node(temp_tnode,i);
                    break;
                }
            }
            temp_tnode=temp_tnode->getnextNode();
        }
    }

    delete cand_list;
    delete input_entry;
    delete[] tmpStr;

    pthread_mutex_unlock(&mutexqhash);
    return EC_SUCCESS;
}