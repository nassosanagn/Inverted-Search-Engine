
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
        delete ix->getword();
        delete ix;
    }
    return EC_SUCCESS;
}

//Synarthsh gia eyresh hamming distance 
int Index::HammingDistance(const char* word1, const char* word2){
        int j = strlen(word1)-strlen(word2);
        if (j < 0){j=-j;}
        for(int i=0;i<(strlen(word1) < strlen(word2) ? strlen(word1) : strlen(word2));i++){
            if(word1[strlen(word1)-i-1]!=word2[strlen(word2)-i-1]){
                j++;
            }
        }
        return j;
}

//EditDistance apo core.cpp
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

ErrorCode Index::build_entry_index(const entry_list* el, MatchType type, Index* ix){

    entry* currentEntry = el->getfirst();

    if (currentEntry != NULL){
        ix->root = new treeNode(currentEntry->getword(),0);
        currentEntry = currentEntry->getnext();
    }

    while (currentEntry != NULL){
        ix->insertTree(currentEntry->getword(), ix->getRoot()->getString(), ix->getRoot(),MT_HAMMING_DIST);
        currentEntry = currentEntry->getnext();
    }

    return EC_SUCCESS;
}

//Eisagwgh komvoy sto dentro
ErrorCode Index::insertTree(char* str, char* cmpWord, treeNode* tempNode, MatchType matchtype){
    setmatchtype(matchtype);
    int tempDiff;
    //Analoga to matchtype kalei kai thn katallhlh synarthsh metrhshs
    switch(matchtype){
        case MT_HAMMING_DIST:
            tempDiff = HammingDistance(str, cmpWord);
            break;
        case MT_EDIT_DIST:
            tempDiff = EditDistance(str,strlen(str),cmpWord,strlen(cmpWord));
            break;
    }

    cout << "H diafora otan h leksh einai: " << str << " me thn: " << cmpWord << " einai: " << tempDiff << endl;

    //An den yparxei allo paidi ths rizas 
    if (this->getRoot()->getChildNode() == NULL){                     // Only for the first time 
        getRoot()->setChildNode(new treeNode(str,tempDiff));
        cout << "Inserted help returning" << endl; 
        return EC_SUCCESS;
    }

    if (tempNode == this->getRoot()){                /* Start from the root's child */
        tempNode = this->getRoot()->getChildNode();
    }

    if ((tempDiff == tempNode->getDiff())) {        /* Go down on that node */

        if (tempNode->getChildNode() == NULL){
            switch(matchtype){
                case MT_HAMMING_DIST:
                    tempDiff = HammingDistance(str, tempNode->getString());
                    break;
                case MT_EDIT_DIST:
                    tempDiff = EditDistance(str,strlen(str),tempNode->getString(),strlen(tempNode->getString()));
                    break;
            }
            tempNode->setChildNode(new treeNode(str, tempDiff));
            cout << "Inserted " << str << " down after: " << tempNode->getString() << endl;
        }else{
            cout << "Paei katw sth leksh: " << tempNode->getString() << endl;
            insertTree(str, tempNode->getString(), tempNode->getChildNode(),matchtype);
        }

    }else{                              /* Go right on that node */

        if (tempNode->getnextNode() == NULL){
            tempNode->setNextNode(new treeNode(str, tempDiff));
            cout << "Inserted " << str << " right after: " << tempNode->getString() << endl;
        }else{
            cout << "Paei deksia sth leksh: " << tempNode->getString() << endl;
            insertTree(str, cmpWord, tempNode->getnextNode(),matchtype);
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
    cout <<"Parent "<< this->getString() << endl;
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
    this->getRoot()->print_all();
}

//TESTING FUNCTIONS OVER


//Prosthkh komvoy sthn lista
void BKList_node::add_node(treeNode* input){
    if (nextnode == NULL)
    {
        nextnode = new BKList_node(input);
    }
    else
    {
        nextnode->add_node(input);
    }
}

//Epistrfei ton prwto komvo kai ton diagrafei apo thn lista
treeNode* BKList::popfirst(){
    BKList_node* temp = first->getnext();
    treeNode* return_val=first->getnode();
    delete first;
    first = temp; 
    return return_val;
}


//Syanrthsh gia thn eyresh omoiwn le3ewn
ErrorCode Index::lookup_entry_index(const word* w, Index* ix, int threshold, entry_list* result){
    //Lista ypopshfiwn le3ewn
    BKList* cand_list = new BKList(new BKList_node(ix->getRoot()));
    //An den yparxei dentro
    if (ix->getRoot()==NULL)
    {
        return EC_FAIL;
    }
    //proswrinh le3h gia thn dimhoyrgia entry struct
    char *tmpStr = new char[strlen("tmp")+1];
    strcpy(tmpStr,"k");
    //entry gia thn dhmioyrgia listas le3ewn gia epistrofh
    entry* input_entry = new entry(tmpStr);
    //Oso den yparxoyn alles ypopshfies le3eis
    while (cand_list->getfirst()!=NULL){
        //Dexetai ton prwto komvo-le3h apo thn lista
        treeNode* current_candidate = cand_list->popfirst();
        //Ypologizei thn apostash
        int word_dis;
        switch(matchtype){
            case MT_HAMMING_DIST:
                word_dis = HammingDistance(w->getword(),current_candidate->getString());
                break;
            case MT_EDIT_DIST:
                word_dis = EditDistance(w->getword(),strlen(w->getword()),current_candidate->getString(),strlen(current_candidate->getString()));
                break;
        }
        //An einai omoio me thn le3h
        if (word_dis <= threshold)
        {
            input_entry->setword(current_candidate->getString());
            
            result->add_entry(result,input_entry);
        }
        //Ypologizei ta oria twn apostasewn gia na elegthoyn
        int left = word_dis - threshold;
        int right = word_dis + threshold;
        //Gia kathe paidi poy vrisketai endiamesa twn oriwn ta prosthetei san candidate
        treeNode* temp_tnode=current_candidate->getChildNode();
        while (temp_tnode!= NULL)
        {
            if (left<=temp_tnode->getDiff() && temp_tnode->getDiff()<=right)
            {
                cand_list->add_node(temp_tnode);
            }
            temp_tnode=temp_tnode->getnextNode();
        }
    }
    delete cand_list;
    delete input_entry;
    delete[] tmpStr; 
    return EC_SUCCESS;
}



int main(){




    
    char* tmpStr = new char[strlen("hell")];
    strcpy(tmpStr,"hell");
    entry* E = new entry(tmpStr);
    entry_list* El;

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

    char* tmpStr7 = new char[strlen("small")];
    strcpy(tmpStr7,"small");
    entry* E7 = new entry(tmpStr7);
    


    El->create_entry_list(&El);
    El->add_entry(El,E);
    El->add_entry(El,E1);
    El->add_entry(El,E2);
    El->add_entry(El,E3);
    El->add_entry(El,E4);
    El->add_entry(El,E5);
    El->add_entry(El,E6);
    El->add_entry(El,E7);


    

    Index* Index1 = new Index();
    Index1->build_entry_index(El,MT_HAMMING_DIST,Index1);




    //Testing print
    for (size_t i = 0; i < 10; i++)
    {
        cout<<endl;
    }
    Index1->printTree();

    //Testing lookup

    char testStr[]="henn";

    word myword(testStr);
    int threshold = 2;
    entry_list* result= new entry_list();

    Index1->lookup_entry_index(&myword,Index1,threshold,result);

    cout<<"print start \n";
    result->print_list(result);

    result->destroy_entry_list(&result);

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