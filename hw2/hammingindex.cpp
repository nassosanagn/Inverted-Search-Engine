using namespace std;

#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "hammingindex.h"




int main(){
    HammingIndex myindex;

    char* tmpStr = new char[strlen("hell")];
    strcpy(tmpStr,"hell");
    word* myword = new word(tmpStr);

    char* tmpStr1 = new char[strlen("helr")];
    strcpy(tmpStr1,"helr");
    word* myword1 = new word(tmpStr1);

    char* tmpStr2 = new char[strlen("hellf")];
    strcpy(tmpStr2,"helldsaf");
    word* myword2 = new word(tmpStr2);

    myindex.insert(myword);
    myindex.insert(myword1);
    myindex.insert(myword2);
    myindex.printall();
    return 0;
}