#ifndef TRAVERSE_DIRS_H
#define TRAVERSE_DIRS_H

#include "list.h"
#include <stdio.h>

void write_directory_tree(char* dir_path, FILE* output_file, Node* exclusion_list, int depth);
void write_file_info(char* dir_path, FILE* output_file, Node* exclusion_list);

#endif
