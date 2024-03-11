enum proc_info_state { IUNUSED, IUSED, ISLEEPING, IRUNNABLE, IRUNNING, IZOMBIE };


typedef struct procinfo {
    char name[16];
    int parent_id;
    enum proc_info_state state;
} procinfo_t;