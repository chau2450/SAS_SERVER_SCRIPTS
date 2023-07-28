
#include "traverse_dirs.h"
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <stdlib.h> 

char* get_parent_owner(const char* file_path) {
    struct stat st;
    struct passwd *pwd;
    char *parent_owner = NULL;
    char path_copy[1024];
    strncpy(path_copy, file_path, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';

    char* last_slash = strrchr(path_copy, '/');
    if (last_slash == path_copy) {
        path_copy[0] = '/';
        path_copy[1] = '\0';
    } else if (last_slash) {
        *last_slash = '\0';
    }

    if (stat(path_copy, &st) == -1)
        return NULL;

    pwd = getpwuid(st.st_uid);

    if (pwd && pwd->pw_name) {
        parent_owner = strdup(pwd->pw_name);
    }

    return parent_owner;
}

char* get_parent_group(const char* file_path) {
    struct stat st;
    struct group *grp;
    char *parent_group = NULL;
    char path_copy[1024];
    strncpy(path_copy, file_path, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';

    char* last_slash = strrchr(path_copy, '/');
    if (last_slash == path_copy) {
        path_copy[0] = '/';
        path_copy[1] = '\0';
    } else if (last_slash) {
        *last_slash = '\0';
    }

    if (stat(path_copy, &st) == -1)
        return NULL;

    grp = getgrgid(st.st_gid);

    if (grp && grp->gr_name) {
        parent_group = strdup(grp->gr_name);
    } 

    return parent_group;
}


void write_directory_tree(char* dir_path, FILE* output_file, Node* exclusion_list, int depth) {
    DIR* dir;
    struct dirent* entry;
    struct stat st;

    if (!(dir = opendir(dir_path)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        
        if (stat(path, &st) == -1)
            continue;

        if (S_ISDIR(st.st_mode) && !is_excluded(exclusion_list, path)) {
            fprintf(output_file, "%*s[%s]\n", depth, "", entry->d_name);
            write_directory_tree(path, output_file, exclusion_list, depth + 2);
        } else{
            fprintf(output_file, "%*s-%s\n", depth, "", entry->d_name);
        }
    }
    closedir(dir);
}

void write_file_info(char* dir_path, FILE* output_file, Node* exclusion_list) {
    DIR* dir;
    struct dirent* entry;
    struct stat st;
    struct passwd *pwd;
    struct group *grp;

    if (!(dir = opendir(dir_path)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        

        if (stat(path, &st) == -1)
            continue;

        if (!is_excluded(exclusion_list, path)) {
            if (S_ISREG(st.st_mode)) {
                pwd = getpwuid(st.st_uid);
                grp = getgrgid(st.st_gid);

                char* parent_owner = get_parent_owner(path);
                char* parent_group = get_parent_group(path);

                fprintf(output_file, "%s|%s|%lld|%s|%s|%s|%s\n", entry->d_name, path, (long long)st.st_size,
                        pwd ? pwd->pw_name : "", grp ? grp->gr_name : "", 
                        parent_owner ? parent_owner : "", parent_group ? parent_group : "");

                
                
                free(parent_owner);
                free(parent_group);
            }

            if (S_ISDIR(st.st_mode)) {
                write_file_info(path, output_file, exclusion_list);
            }
        }
    }
    closedir(dir);
}
