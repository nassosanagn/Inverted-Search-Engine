
using namespace std;

#ifndef BKTREE_H
#define BKTREE_H

#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "entry.h"


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

        /* Testing Functions */
        void print_children();
        void print_all();
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

        /* Testing Functions*/
        void printTree();

        bool lookup_entry_index(const word* w, BKTree* ix, int threshold, entry_list* result);
};


class BKList_node{

    private:
        treeNode* mynode;
        BKList_node* nextnode;
    public:
        BKList_node(treeNode* input){ mynode=input;}


        treeNode* getnode() const { return mynode;}
        BKList_node* getnext() const { return nextnode;}
        void add_node(treeNode*);
        void destroy_list(){if (nextnode) nextnode->destroy_list(); delete this;}
};

class BKList{
    private:
        BKList_node* first;
    public:
    BKList(BKList_node* input){first = input;}
    treeNode* popfirst();
    BKList_node* getfirst() const { return first;}
    void add_node(treeNode* input){
        if (first==NULL)
        {
            first = new BKList_node(input);
        }
        else
        {
            first->add_node(input);
        }
    }

    void destroy_list(){if (first) first->destroy_list();}
    
};

#endif