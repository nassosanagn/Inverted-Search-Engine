
using namespace std;

#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "tree.h"

BKTree::BKTree(char* myStr){ 
    root = new treeNode(myStr); 
}

BKTree::~BKTree(){ 
    destroyTree(root); 
}

bool BKTree::destroyTree(treeNode* tempNode){

    if (tempNode != NULL){
        destroyTree(tempNode->getRight());
        destroyTree(tempNode->getLeft());
        delete tempNode;
    }

    return true;
}

int main(){
    
    char* tmpStr = new char[15];
    strcpy(tmpStr,"nassosAnagn");

    BKTree* BKTree1 = new BKTree(tmpStr);
    BKTree1->getRoot()->getString();

    cout << "To string einai " << BKTree1->getRoot()->getString() << endl;
    return 0;
}