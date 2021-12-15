struct st_node{
    char c;
    /* number of strings that finish here */
    int complete;
    int n_children;
    /*struct st_node* next;*/
    // could just do proof of concept with lowcase chars, 26 children
    struct st_node* parent;
    struct st_node** children;
};

struct str_tree{
    /*int n_roots;*/
    /*struct st_node** roots;*/
    struct st_node* root;
};

void init_st(struct str_tree* st);
void free_str(struct str_tree* st);

void insert_st(struct str_tree* st, char* str);
void p_full_node(struct st_node* sn, int len, char* prefix);
void p_tree(struct str_tree* st);
struct st_node* filter_str(struct str_tree* st, char* str);
