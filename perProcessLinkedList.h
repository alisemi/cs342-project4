typedef struct per_process_entry per_process_entry;
typedef struct process_node process_node;

process_node** getPHead();

void p_printList();

void p_printListToFile( char* filename);

void p_insertFirst(int key, per_process_entry data);

process_node* p_deleteFirst();

bool s_isEmpty();

process_node** p_find( int key);

process_node* p_delete(int key);

int p_length();

void p_sort();

void p_destroyList();

