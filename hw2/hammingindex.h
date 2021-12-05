using namespace std;

#ifndef HAMMINGINDEX_H
#define HAMMINGINDEX_H

#define TREECOUNT MAX_WORD_LENGTH-MIN_WORD_LENGTH

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include "../hw1/main_folder/BKTree.h"
#include "sigmod2013contest-1.1/include/core.h"


class HammingIndex{
    private:
    public:
        Index mytrees[TREECOUNT];

        ErrorCode insert(word* input){
            int treepos = strlen(input->getword())-MIN_WORD_LENGTH;
            cout <<"Length is "<<treepos<<endl;
            mytrees[treepos].insertWord(input,&(mytrees[treepos]),MT_HAMMING_DIST);
            return EC_SUCCESS;
        }

        ErrorCode printall(){
            for (int i = 0; i < TREECOUNT; i++)
            {
                cout << "Tree len " << i + 4<<endl;
                mytrees[i].printTree();
            }
            

            return EC_SUCCESS;
        }

        HammingIndex()
        {
            // cout << mytrees[1].getRoot() <<endl;
            // entry_list* El;
    
            // char* tmpStr = new char[strlen("hell")];
            // strcpy(tmpStr,"hell");
            // entry* E = new entry(tmpStr);

            // char* tmpStr1 = new char[strlen("help")];
            // strcpy(tmpStr1,"help");
            // entry* E1 = new entry(tmpStr1); 

            // El->create_entry_list(&El);
            // El->add_entry(El,E);
            // El->add_entry(El,E1);

            // El->print_list(El);
            // word* myword = new word(tmpStr);
            // word* myword1 = new word(tmpStr1);
            // mytrees[1].insertWord(myword,&(mytrees[1]),MT_HAMMING_DIST);
            // mytrees[1].insertWord(myword1,&(mytrees[1]),MT_HAMMING_DIST);
            // mytrees[1].printTree();
        }


        
};



#endif
