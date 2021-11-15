
using namespace std;

#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "BKTree.h"

//constructor gia to BKTree
BKTree::BKTree(char* myStr){ 
    root = new treeNode(myStr,0);
}

//destructor
BKTree::~BKTree(){
    //kalei synarthsh diagrafhs kathe komvoy toy dentroy
    destroyTree(root); 
}

//Diagrafei anadromika oloys toys komvoys toy dentroy
ErrorCode BKTree::destroyTree(treeNode* tempNode){
    if (tempNode != NULL){
        //Gia kathe komvo sto idio epipedo
        destroyTree(tempNode->getnextNode());
        //Gia kathe paidi toy
        destroyTree(tempNode->getChildNode());
        //Diagrafei ton komvo
        delete[] tempNode->getString();
        delete tempNode;
    }
    return EC_SUCCESS;
}

//Synarthsh gia eyresh hamming distance 
int BKTree::HammingDistance(const char* word1, const char* word2){
        int j = 0;
        for(int i=0;i<(strlen(word1) > strlen(word2) ? strlen(word1) : strlen(word2));i++){
            if(word1[i]!=word2[i]){
                j++;
            }
        }
        return j;
}

//EditDistance apo core.cpp
int BKTree::EditDistance(char* a, int na, char* b, int nb)
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

//Eisagwgh komvoy sto dentro
ErrorCode BKTree::insertTree(char* str, char* cmpWord, treeNode* tempNode,MatchType matchtype){
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
        cout << "Inserted help" << endl; 
        return EC_SUCCESS;
    }

    if (tempNode == this->getRoot()){                /* Start from the root's child */
        tempNode = this->getRoot()->getChildNode();
    }

    if ((tempDiff == tempNode->getDiff())) {        /* Go down on that node */

        if (tempNode->getChildNode() == NULL){
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

void BKTree::printTree(){
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
ErrorCode BKTree::lookup_entry_index(const word* w, BKTree* ix, int threshold, entry_list* result){
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
        int word_dis = HammingDistance(w->getword(),current_candidate->getString());
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

    // // word* W = new word(tmpStr);
    // // entry* E = new entry(tmpStr);
    // // strcpy(tmpStr,"hell");
    // // entry* E1 = new entry(tmpStr);
    // // strcpy(tmpStr,"fall");
    // // entry* E2 = new entry(tmpStr);
    // // strcpy(tmpStr,"fell");
    // // entry* E3 = new entry(tmpStr);
    // // entry_list* El;
    // // El->create_entry_list(&El);
    // // El->add_entry(El,E);
    // // El->add_entry(El,E1);
    // // El->add_entry(El,E2);
    // // El->add_entry(El,E3);

    // // El->destroy_entry_list(&El);
    
    // // delete E;
    // // delete W;
    // // delete E1;
    // // delete E2;
    // // delete E3;
    // // delete[] tmpStr;
    // // delete El;

    // char* tmpStr = new char[15];
    // strcpy(tmpStr,"hell");
    char tmpStr[]="hell";

    // char* tmpStr2 = new char[15];
    // strcpy(tmpStr2,"help");
    char tmpStr2[]="help";
    
    // char* tmpStr3 = new char[15];
    // strcpy(tmpStr3,"fell");
    char tmpStr3[]="fell";

    // char* tmpStr4 = new char[15];
    // strcpy(tmpStr4,"fall");
    char tmpStr4[]="fall";

    // char* tmpStr5 = new char[15];
    // strcpy(tmpStr5,"small");
    char tmpStr5[]="small";

    // char* tmpStr6 = new char[15];
    // strcpy(tmpStr6,"felt");
    char tmpStr6[]="felt";

    // char* tmpStr7 = new char[15];
    // strcpy(tmpStr7, "melt");
    char tmpStr7[]="melt";

    BKTree* BKTree1 = new BKTree(tmpStr);
    
    BKTree1->insertTree(tmpStr2,BKTree1->getRoot()->getString(),BKTree1->getRoot(),MT_HAMMING_DIST);
    BKTree1->insertTree(tmpStr3,BKTree1->getRoot()->getString(),BKTree1->getRoot(),MT_HAMMING_DIST);
    BKTree1->insertTree(tmpStr4,BKTree1->getRoot()->getString(),BKTree1->getRoot(),MT_HAMMING_DIST);
    BKTree1->insertTree(tmpStr5,BKTree1->getRoot()->getString(),BKTree1->getRoot(),MT_HAMMING_DIST);
    cout << "-----------------------------" << endl;
    BKTree1->insertTree(tmpStr6,BKTree1->getRoot()->getString(),BKTree1->getRoot(),MT_HAMMING_DIST);
    cout << "------------------------>-----" << endl;
    BKTree1->insertTree(tmpStr7,BKTree1->getRoot()->getString(),BKTree1->getRoot(),MT_HAMMING_DIST);
    cout << "----------------------------" << endl;


    cout << "To string einai " << BKTree1->getRoot()->getString() << endl;

    //Testing print
    for (size_t i = 0; i < 10; i++)
    {
        cout<<endl;
    }
    BKTree1->printTree();

    //Testing lookup

    
    char testStr[]="henn";

    word myword(testStr);
    int threshold = 2;
    entry_list* result= new entry_list();

    BKTree1->lookup_entry_index(&myword,BKTree1,threshold,result);

    cout<<"print start \n";
    result->print_list(result);
    result->destroy_entry_list(&result);
    delete result;
    delete BKTree1;
    return 0;
}