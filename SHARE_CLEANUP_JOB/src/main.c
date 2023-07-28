#include <pthread.h>
#include "traverse_dirs.h"
#include "list.h"
#include <stdio.h>

typedef struct {
    char* dir_path;
    FILE* output_file;
    Node* exclusion_list;
} ThreadData;

void* thread_write_directory_tree(void* args) {
    ThreadData* data = (ThreadData*)args;
    write_directory_tree(data->dir_path, data->output_file, data->exclusion_list, 0);
    return NULL;
}

void* thread_write_file_info(void* args) {
    ThreadData* data = (ThreadData*)args;
    fprintf(data->output_file, "file_name|file_path|file_size|file_owner|group_owner|parent_dir_owner|parent_dir_group_owner\n");
    write_file_info(data->dir_path, data->output_file, data->exclusion_list);
    return NULL;
}

int main(int argc, char* argv[]) {
    if(argc != 5) {
        printf("Incorrect number of arguments supplied\n");
        printf("Usage: %s <Directory Start> <Tree Diagram> <File Info> <Exclusion List>\n", argv[0]);
        return 1;
    }

    Node* exclusion_list = NULL;
    load_exclusions(argv[4], &exclusion_list);
    

    ThreadData data1 = {argv[1], fopen(argv[2], "w"), exclusion_list};
    ThreadData data2 = {argv[1], fopen(argv[3], "w"), exclusion_list};

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_write_directory_tree, &data1);
    pthread_create(&thread2, NULL, thread_write_file_info, &data2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    fclose(data1.output_file);
    fclose(data2.output_file);

    printf("Process Complete!!!!!\n");

    free_list(exclusion_list);

    return 0;
}
