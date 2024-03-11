typedef struct procinfo {
    char name[16];
    int parent_id;
    enum procstate state;
} procinfo_t;