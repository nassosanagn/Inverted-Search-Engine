
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
        destroyTree(tempNode->getRight());
        destroyTree(tempNode->getLeft());
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

bool BKTree::insertTree(char* str, treeNode* tempNode){

    int tempDiff = charDiff(str, tempNode->getString());

    if (tempDiff == tempNode->diff){    /* Go down on that node */

        if (tempNode->childNode == NULL){
            tempNode->childNode = new treeNode(tempNode->childNode->getString(), tempDiff);
        }else{
            insertTree(str, tempNode->childNode);
        }

    }else{                              /* Go right on that node */

        if (tempNode->nextNode == NULL){
            tempNode->nextNode = new treeNode(str, tempDiff);
        }else{
            insertTree(str, tempNode->nextNode);
        }
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