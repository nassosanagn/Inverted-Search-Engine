#ifndef HAMMINGINDEX_H
#define HAMMINGINDEX_H

#define TREECOUNT MAX_WORD_LENGTH-MIN_WORD_LENGTH

using namespace std;

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include "../hw1/main_folder/BKTree.h"
#include "sigmod2013contest-1.1/include/core.h"

class HammingIndex{
    private:
        Index* mytrees;
    public:

        HammingIndex(){
            mytrees = new Index[TREECOUNT];
        }

        ~HammingIndex(){
            for(int i = 0; i < TREECOUNT; i++){
               mytrees[i].destroy_entry_index(mytrees[i].getRoot());
            }
        }

        ErrorCode insert(word* input,int qid){
            int treepos = strlen(input->getword())-MIN_WORD_LENGTH;
            // cout <<"Position is "<<treepos<<" "<<qid<<" "<<input->getword()<<endl;
            mytrees[treepos].insertWord(input,&(mytrees[treepos]),MT_HAMMING_DIST,qid);
            return EC_SUCCESS;
        }

        ErrorCode printall(){
            for (int i = 0; i < TREECOUNT; i++){
                cout << "Tree len " << i + 4<<endl;
                mytrees[i].printTree();
            }
            return EC_SUCCESS;
        }

        Index* getTree(){
            return mytrees;
        }

        ErrorCode lookup_hamming_index(const word* w, int threshold, MatchType m_type,query_Hashtable* Q_hash,int current_doc,payload_list* q_result){
            int word_len = strlen(w->getword());
            Index* target_tree = &(mytrees[word_len-MIN_WORD_LENGTH]);
            return target_tree->lookup_entry_index(w,target_tree,threshold,m_type,Q_hash,current_doc,q_result);
        }
};

#endif
