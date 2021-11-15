using namespace std;

#ifndef BKTREE_H
#define BKTREE_H

#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "entry.h"

//Domh poy leitoyrgei ws komvos sto dentro-eyrethrio
class treeNode{

    private:

        //Le3h apo entry_list
        word* myString;
        //
        int diff;
        //Deikths se epomeno struct gia thn dhmioyrgeia listas gia to idio epipedo
        treeNode* nextNode;
        //Deikths sto prwto paidi
        treeNode* childNode;
    
    public:
        //Contructor
        treeNode(char* str, int tmpDiff){
            myString = new word(str);
            // myString = new char[strlen(str) + 1];
            // strcpy(myString, str);

            diff = tmpDiff;

            nextNode = NULL;
            childNode = NULL;
        }

        //Getters - Setters
        char* getString() const { return myString->getword(); }
        word* getword() const { return myString; }
        int getDiff() const { return diff; }
        treeNode* getnextNode() const { return nextNode; }
        treeNode* getChildNode() const { return childNode; }

        void setChildNode(treeNode* tempNode) { this->childNode = tempNode; }
        void setNextNode(treeNode* tempNode) { this->nextNode = tempNode; }

        /* Testing Functions */
        void print_children();
        void print_all();
};

class Index{

    private:
        MatchType matchtype;
        treeNode* root;

    public:
        
        Index();        /* Constructor */
        ~Index();                  /* Destructor */

        ErrorCode destroy_entry_index(treeNode* tempNode);               /* Destroy the tree by visiting and destroying each node */
        ErrorCode insertTree(char* str, char* cmpWord, treeNode* tempNode,MatchType matchtype);
        ErrorCode build_entry_index(const entry_list* el, MatchType type, Index* ix);

        int HammingDistance(const char* word1, const char* word2);
        int EditDistance(char* a, int na, char* b, int nb);

        //Getter
        treeNode* getRoot() const { return root; }
        MatchType getmatchtype() const { return matchtype; }

        void setmatchtype(MatchType tmp){ matchtype = tmp;}
        /* Testing Functions*/
        void printTree();

        //O deikths result deixnei se le3eis poy moiazoyn me thn le3h sto prwto orisma
        ErrorCode lookup_entry_index(const word* w, Index* ix, int threshold, entry_list* result);
};


class BKList_node{

    private:
        treeNode* mynode;
        BKList_node* nextnode;
    public:

        BKList_node(treeNode* input){nextnode = NULL; mynode=input;}

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