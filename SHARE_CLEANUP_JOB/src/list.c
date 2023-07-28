#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* create_node(char* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = strdup(data);
    new_node->next = NULL;
    return new_node;
}

void append_node(Node** head, char* data) {
    Node* new_node = create_node(data);
    new_node->next = *head;
    *head = new_node;
}

void free_list(Node* head) {
    Node* tmp;
    while (head != NULL) {
       tmp = head;
       head = head->next;
       free(tmp->data);
       free(tmp);
    }
}

int is_excluded(Node* head, char* directory) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->data, directory) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}


void load_exclusions(const char* filename, Node** head) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file");
        return;
    }

    char* line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        append_node(head, line);
    }

    free(line);
    fclose(file);
}
