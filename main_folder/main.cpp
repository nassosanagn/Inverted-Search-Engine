#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

int char_dif(const char* word1, const char* word2){
    int j = 0;
    for(int i=0;i<strlen(word1);i++){
        if(word1[i]!=word2[i]){
            j++;
        }
    }
    return j;
}

int main(int argc, char* argv[]){
    char tmp1[] = "hell";
    char tmp2[] = "fall";
    cout << char_dif(tmp1, tmp2)<<endl;
    return 0;
}