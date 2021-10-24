
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


int main(){
    
    char* tmpStr = new char[15];
    strcpy(tmpStr,"hell");

    char* tmpStr2 = new char[15];
    strcpy(tmpStr2,"help");

    char* tmpStr3 = new char[15];
    strcpy(tmpStr3,"fell");

    char* tmpStr4 = new char[15];
    strcpy(tmpStr4,"fall");

    char* tmpStr5 = new char[15];
    strcpy(tmpStr5,"small");

    char* tmpStr6 = new char[15];
    strcpy(tmpStr6,"felt");

    char* tmpStr7 = new char[15];
    strcpy(tmpStr7, "melt");

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
    return 0;
}