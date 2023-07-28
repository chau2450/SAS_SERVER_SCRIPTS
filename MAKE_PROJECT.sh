#!/bin/bash

create_project() {
    read -p "Enter the project name: " project_name

    read -p "Do you want to continue and create the project? (y/n): " confirm

    if [[ "$confirm" == "y" || "$confirm" == "Y" ]]; then

        mkdir "$project_name"
        cd "$project_name" || exit

        mkdir bin build obj src include test

        echo -e '#include <stdio.h>\n\nint main(int argc, char *argv[]) {\n    printf("Hello, World!\\n");\n    return 0;\n}' > src/main.c

        touch README.md

        echo -e "bin/\nobj/\n" > .gitignore

        echo -e "YOUR_LICENSE_TEXT" > LICENSE

        echo -e "CC=gcc\nCFLAGS=-Wall -Werror -static\n\nROOT_DIR=..\n\nall: \$(ROOT_DIR)/bin/main\n\n\$(ROOT_DIR)/bin/main: \$(ROOT_DIR)/obj/main.o\n\tmkdir -p \$(ROOT_DIR)/bin\n\t\$(CC) \$(CFLAGS) -o \$(ROOT_DIR)/bin/main \$(ROOT_DIR)/obj/main.o\n\n\$(ROOT_DIR)/obj/main.o: \$(ROOT_DIR)/src/main.c\n\tmkdir -p \$(ROOT_DIR)/obj\n\t\$(CC) \$(CFLAGS) -c -o \$(ROOT_DIR)/obj/main.o \$(ROOT_DIR)/src/main.c\n\nclean:\n\trm -rf \$(ROOT_DIR)/bin \$(ROOT_DIR)/obj" > build/Makefile

        echo "Project created successfully."
    else
        echo "Project creation cancelled."
    fi
}

create_project

cd build
echo "built project"
make
echo "binary created!!!!!"
echo "output: "
./../bin/main