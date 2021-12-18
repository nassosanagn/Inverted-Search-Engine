#ifndef BKTREE_H
#define BKTREE_H

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include "../../hw2/q_hashtable.h"
#include "entry.h"

using namespace std;

//Domh poy leitoyrgei ws komvos sto dentro-eyrethrio
class treeNode{

    private:

        //Le3h apo entry_list
        entry* myEntry;
        //
        int diff;
        //Deikths se epomeno struct gia thn dhmioyrgeia listas gia to idio epipedo
        treeNode* nextNode;
        //Deikths sto prwto paidi
        treeNode* childNode;
    
    public:
        //Contructor
        treeNode(entry* entry, int tmpDiff){
            myEntry = new ::entry(entry->getword());
            // myString = new char[strlen(str) + 1];
            // strcpy(myString, str);

            diff = tmpDiff;

            nextNode = NULL;
            childNode = NULL;
        }

        // Getters - Setters
        char* getString() const { return myEntry->getword(); }
        word* getWord() const { return myEntry->getmyword(); }
        entry* getEntry() const { return myEntry; }

        int getDiff() const { return diff; }

        treeNode* getnextNode() const { return nextNode; }
        treeNode* getChildNode() const { return childNode; }

        void setChildNode(treeNode* tempNode, int qid) {
            this->childNode = tempNode;
            this->childNode->getEntry()->getpayload()->payload_insert(qid);
        }
        void setNextNode(treeNode* tempNode, int qid) { 
            this->nextNode = tempNode;
            this->nextNode->getEntry()->getpayload()->payload_insert(qid);
         }

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
        ErrorCode insertTree(entry* entry, char* cmpWord, treeNode* tempNode, MatchType matchtype, int qid);
        ErrorCode build_entry_index(const entry_list* el, MatchType type, Index* ix, int qid);

        int HammingDistance(const char* word1, const char* word2);
        int EditDistance(char* a, int na, char* b, int nb);

        //Getter
        treeNode* getRoot() const { return root; }
        MatchType getmatchtype() const { return matchtype; }

        void setmatchtype(MatchType tmp){ matchtype = tmp;}
        /* Testing Functions*/
        void printTree();

        //O deikths result deixnei se le3eis poy moiazoyn me thn le3h sto prwto orisma
        ErrorCode lookup_entry_index(const word* w, Index* ix, int threshold, MatchType m_type,query_Hashtable* Q_hash,int current_doc,payload_list* q_result);

        ErrorCode insertWord(word* W, Index* ix, MatchType mt,int qid);
};


class BKList_node{

    private:
        treeNode* mynode;
        BKList_node* nextnode;
        int threshold;
    public:

        BKList_node(treeNode* input,int thr)
        {
            // cout<<"zzzzzzz"<<endl;
            nextnode = NULL; 
            mynode=input;
            threshold = thr;
            // cout<<"vvvvvvvv"<<endl;
        }

        treeNode* getnode() const { return mynode;}
        int get_threshold() const { return threshold;}
        BKList_node* getnext() const { return nextnode;}
        void add_node(treeNode*,int);
        void destroy_list(){if (nextnode) nextnode->destroy_list(); delete this;}
};

class BKList{
    private:
        BKList_node* first;
    public:
    BKList(treeNode* input,int thr){
        first = new BKList_node(input,thr);
    }
    treeNode* popfirst(int* threshold);
    BKList_node* getfirst() const { return first;}
    void add_node(treeNode* input,int threshold){
        if (first==NULL)
        {
            first = new BKList_node(input,threshold);
        }
        else
        {
            first->add_node(input,threshold);
        }
    }

    void destroy_list(){if (first) first->destroy_list();}
    
};

#endif