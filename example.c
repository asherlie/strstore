#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "strstore.h"

extern uint64_t bytes_allocated;

int main(int a, char** b){
    struct str_tree st;
    struct st_node* sn;
    FILE* fp;
    int br;
    size_t sz = 0;
    char* word = NULL;
    if(a < 2)return 1;
    fp = fopen(b[1], "r");
    init_st(&st);
    while((br = getline(&word, &sz, fp)) != EOF){
        word[--br] = 0;
        insert_st(&st, word);
    }
    free(word);
    word = NULL;
    fclose(fp);
    /*p_tree(&st);*/
    printf("used %li bytes total\n", bytes_allocated);
    while((br = getline(&word, &sz, stdin))){
        if(*word == '\\')break;
        word[--br] = 0;
        if(!(sn = filter_str(&st, word)))continue;
        p_full_node(sn, 0, word);
    }
    free(word);
    free_str(&st);
}
