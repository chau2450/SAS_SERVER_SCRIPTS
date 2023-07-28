
#include "traverse_dirs.h"
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

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
            fprintf(output_file, "%*s[%s]\n", depth, "", entry->d_name);
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

                fprintf(output_file, "%s|%s|%lld|%s|%s\n", entry->d_name, path, (long long)st.st_size,
                        pwd ? pwd->pw_name : "", grp ? grp->gr_name : "");
            }

            if (S_ISDIR(st.st_mode)) {
                write_file_info(path, output_file, exclusion_list);
            }
        }
    }
    closedir(dir);
}
