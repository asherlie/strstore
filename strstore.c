#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "strstore.h"
/*
this is maybe applicable to varedit, for example
an easily indexable string storage that has some overhead
but you can narrow very quickly while typing
maybe it'll shift directories so to speak

cd 'a'
cd 's'
cd 'x'
<delete>
cd ..
cd 'h'
cd 'e'
cd 'r'


a tree might be better
each node has a _Bool flag for
word - whether or not a word
is completed at that node

we can also hae an array of pointers to each word node

*/

uint64_t bytes_allocated = 0;
void* _Malloc(int nbytes){
    bytes_allocated += nbytes;
    /*printf("b to %i\n", bytes_allocated);*/
    return malloc(nbytes);
}

void* _Calloc(int nbytes, int sz){
    bytes_allocated += nbytes*sz;
    return calloc(nbytes, sz);
}

void init_sn(struct st_node* sn, char c){
    sn->n_children = 177;
    sn->children = _Calloc(sizeof(struct st_node*), sn->n_children);
    sn->c = c;
}

void init_st(struct str_tree* st){
    /*st->n_roots = 50;*/
    /*root->complete being true represents ""*/
    st->root = _Malloc(sizeof(struct st_node));
    init_sn(st->root, 0);
    st->root->parent = NULL;
    /*st->roots = _Calloc(sizeof(struct st_node), st->n_roots);*/
}

struct st_node* lookup_char(struct st_node* sn, char c){
    return sn->children[(int)c/*-'a'*/];
}

// only called if does not exist at layer
struct st_node* insert_char(struct st_node* sn, char c){
    struct st_node* ret = (sn->children[(int)c/*-'a'*/] = _Malloc(sizeof(struct st_node)));
    // overwritten if complete
    ret->parent = sn;
    ret->complete = 0;
    /*assert(!ret);*/
    /*ret = _Malloc(siezof(str));*/
    init_sn(ret, c);
    return ret;
    /*if(sn->n_children == sn->cap)*/

}

/* found the bug :) it ignores instead of recurses when a char is found */
void insert_st(struct str_tree* st, char* str){
    struct st_node* sn = st->root;
    for(char* i = str; *i; ++i){
        if(!lookup_char(sn, *i)){
            // insert
            sn = insert_char(sn, *i); 
        }
        else{
            sn = lookup_char(sn, *i);
            /*insert_char(sn, *i);*/
        }
    }
    ++sn->complete;
}

void p_str(struct st_node* sn, int len){
    struct st_node* sni = sn;
    char buf[len+1];
    int idx = len-1;
    buf[len] = 0;
    while(sni){
        buf[idx--] = sni->c;
        sni = sni->parent;
    }
    puts(buf);
}

void p_full_node(struct st_node* sn, int len, char* prefix){
    if(sn->complete){
        printf("%i x ", sn->complete);
        if(prefix)printf("%s", prefix);
        p_str(sn, len);
        /*printf(" of length %i x %i\n", len, sn->complete);*/
    }
    if(!sn->children)return;
    for(int i = 0; i < sn->n_children; ++i){
        if(sn->children[i])p_full_node(sn->children[i], len+1, prefix);
    }
}

void p_tree(struct str_tree* st){
    /*p_node(st->root);*/
    p_full_node(st->root, 0, NULL);
}

struct st_node* filter_str(struct str_tree* st, char* str){
    struct st_node* sn = st->root;
    for(char* i = str; *i; ++i){
        if(lookup_char(sn, *i))sn = lookup_char(sn, *i);
        else return NULL;
    }
    return sn;
}

void free_sn(struct st_node* sn){
    if(!sn->children)return;
    for(int i = 0; i < sn->n_children; ++i){
        /*printf("ii%i\n", i);*/
        if(sn->children[i]){
            free_sn(sn->children[i]);
            free(sn->children[i]);
        }
    }
    /*puts("frei");*/
    free(sn->children);
}

void free_str(struct str_tree* st){
    free_sn(st->root);
    free(st->root);
}
