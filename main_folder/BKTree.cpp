
using namespace std;

#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "BKTree.h"

BKTree::BKTree(char* myStr){ 
    root = new treeNode(myStr,0);
}

BKTree::~BKTree(){ 
    destroyTree(root); 
}


bool BKTree::destroyTree(treeNode* tempNode){

    if (tempNode != NULL){
        delete[] tempNode->getString();
        destroyTree(tempNode->getnextNode());
        destroyTree(tempNode->getChildNode());
        delete tempNode;
    }

    return true;
}

int BKTree::charDiff(const char* word1, const char* word2){
        int j = 0;
        for(int i=0;i<strlen(word1);i++){
            if(word1[i]!=word2[i]){
                j++;
            }
        }
        return j;
}

bool BKTree::insertTree(char* str, char* cmpWord, treeNode* tempNode){

    int tempDiff = charDiff(str, cmpWord);

    cout << "H diafora otan h leksh einai: " << str << " me thn: " << cmpWord << " einai: " << tempDiff << endl;

    if (this->getRoot()->getChildNode() == NULL){                     // Only for the first time 
        getRoot()->setChildNode(new treeNode(str,tempDiff));
        cout << "Inserted help" << endl; 
        return true;
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
            insertTree(str, tempNode->getString(), tempNode->getChildNode());
        }

    }else{                              /* Go right on that node */

        if (tempNode->getnextNode() == NULL){
            tempNode->setNextNode(new treeNode(str, tempDiff));
            cout << "Inserted " << str << " right after: " << tempNode->getString() << endl;
        }else{
            cout << "Paei deksia sth leksh: " << tempNode->getString() << endl;
            insertTree(str, cmpWord, tempNode->getnextNode());
        }
    }

    return true;
}


//TESTING FUNCTIONS

void treeNode::print_all(){
this->print_children();

treeNode* next = this->getnextNode();
treeNode* child= this->getChildNode();
if (next!= NULL) next->print_all();
if (child!= NULL) child->print_all();
}
void treeNode::print_children(){
    treeNode* tempNode = this->getChildNode();
    cout <<"Parent "<< this->getString() << endl;
    if (tempNode == NULL)
    {
        cout<<endl;
    }
    else
    {
        cout <<"Children ";
        while (tempNode!= NULL)
        {
            cout << tempNode->getString() <<" "<< tempNode->getDiff()<<"  ";
            tempNode = tempNode->getnextNode();
        }
        cout << endl;
    }
    
    
}

void BKTree::printTree(){

    this->getRoot()->print_all();
}

//TESTING FUNCTIONS OVER



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




treeNode* BKList::popfirst(){
    BKList_node* temp = first->getnext();
    treeNode* return_val=first->getnode();
    delete first;
    first = temp; 
    return return_val;
}



bool BKTree::lookup_entry_index(const word* w, BKTree* ix, int threshold, entry_list* result){
    BKList* cand_list = new BKList(new BKList_node(root));
    // cand_list->create_entry_list(&cand_list);
    // entry* root_entry=new entry(root->getString());
    // cand_list->add_entry(cand_list,root_entry);
    if (root==NULL)
    {
        return false;
    }
    // char *tmpStr = strdup("k");
    char *tmpStr = new char[strlen("k")+1];
    strcpy(tmpStr,"k");
    entry* input_entry = new entry(tmpStr);
    while (cand_list->getfirst()!=NULL){
        //remove first candidate from cand_list

        treeNode* current_candidate = cand_list->popfirst();
        int distance = charDiff(w->getword(),current_candidate->getString());

        if (distance <= threshold)
        {
            // entry* input_entry = new entry(current_candidate->getString());
            // cout << input_entry->getword()<< "\n";
            input_entry->setword(current_candidate->getString());
            
            result->add_entry(result,input_entry);
            // result->print_list();
        }
        int low_bound = distance - threshold;
        int up_bound = distance + threshold;
        treeNode* temp_tnode=current_candidate->getChildNode();
        while (temp_tnode!= NULL)
        {
            if (low_bound<=temp_tnode->getDiff() && temp_tnode->getDiff()<=up_bound)
            {
                cand_list->add_node(temp_tnode);
            }
            temp_tnode=temp_tnode->getnextNode();
        }
    }
    delete cand_list;
    delete input_entry;
    delete[] tmpStr; 
    return true;
}



int main(){

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
    
    BKTree1->insertTree(tmpStr2,BKTree1->getRoot()->getString(),BKTree1->getRoot());
    BKTree1->insertTree(tmpStr3,BKTree1->getRoot()->getString(),BKTree1->getRoot());
    BKTree1->insertTree(tmpStr4,BKTree1->getRoot()->getString(),BKTree1->getRoot());
    BKTree1->insertTree(tmpStr5,BKTree1->getRoot()->getString(),BKTree1->getRoot());
    cout << "-----------------------------" << endl;
    BKTree1->insertTree(tmpStr6,BKTree1->getRoot()->getString(),BKTree1->getRoot());
    cout << "------------------------>-----" << endl;
    BKTree1->insertTree(tmpStr7,BKTree1->getRoot()->getString(),BKTree1->getRoot());
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