
using namespace std;

#include <iostream>
#include <cstring>
#include <stdlib.h>

class treeNode{

    private:
        char* myString;
        int diff;
        treeNode* nextNode;
        treeNode* childNode;
    
    public:
        treeNode(char* str, int tmpDiff){

            myString = new char[strlen(str) + 1];
            strcpy(myString, str);

            diff = tmpDiff;

            nextNode = NULL;
            childNode = NULL;
        }

        /* Getters */
        char* getString() const { return myString; }
        int getDiff() const { return diff; }
        treeNode* getnextNode() const { return nextNode; }
        treeNode* getChildNode() const { return childNode; }

        /* Setters */
        void setChildNode(treeNode* tempNode) { this->childNode = tempNode; }
        void setNextNode(treeNode* tempNode) { this->nextNode = tempNode; }
};

class BKTree{

    private:
        treeNode* root;

    public:
        
        BKTree(char* myStr);        /* Constructor */
        ~BKTree();                  /* Destructor */

        bool destroyTree(treeNode* tempNode);               /* Destroy the tree by visiting and destroying each node */
        bool insertTree(char* str, char* cmpWord, treeNode* tempNode);

        int charDiff(const char* word1, const char* word2);

        treeNode* getRoot() const { return root; }
};