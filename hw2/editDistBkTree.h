
#ifndef EDIT_DIST_BK_TREE_H
#define EDIT_DIST_BK_TREE_H

using namespace std;
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include "../hw1/main_folder/BKTree.h"
#include "sigmod2013contest-1.1/include/core.h"


class EditBKTree{
    

    Index* editBk;
    public:
        Index* getBKtree() const { return editBk; }
        EditBKTree(){
            editBk = new Index();
        }
};

#endif