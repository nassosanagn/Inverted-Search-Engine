
using namespace std;

#include <iostream>
#include <cstring>
#include <stdlib.h>

class treeNode{

    private:
        char* myString;
        treeNode* right;
        treeNode* left;
    
    public:
        treeNode(char* str){

            myString = new char[strlen(str) + 1];
            strcpy(myString, str);

            right = NULL;
            left = NULL;
        }

        char* getString() const { return myString; }
        treeNode* getRight() const { return right; }
        treeNode* getLeft() const { return left; }
};

class BKTree{

    private:
        treeNode* root;

    public:
        
        BKTree(char* myStr);        /* Constructor */
        ~BKTree();                  /* Destructor */

        bool destroyTree(treeNode* tempNode);       /* Destroy the tree by visiting and destroying each node */

        treeNode* getRoot() const { return root; }
};