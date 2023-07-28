#ifndef LIST_H
#define LIST_H

typedef struct Node {
    char* data;
    struct Node* next;
} Node;

Node* create_node(char* data);
void append_node(Node** head, char* data);
void free_list(Node* head);
int is_excluded(Node* head, char* directory);
void load_exclusions(const char* filename, Node** head);


#endif
