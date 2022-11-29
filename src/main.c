/**
 * @file main.c
 * @author Noel Atzwanger (e12002121@student.tuwien.ac.at)
 * @brief Main program module
 * @date 2022-11-10
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "cli.h"
#include "diff.h"

int main(int argc, char** argv) {
    
    args arguments = {0};

    int status = parse_args(&arguments, argc, argv);
    // printf("file1:   %s\n", arguments.file1);
    // printf("file2:   %s\n", arguments.file2);
    // printf("output:  %s\n", arguments.output);
    // printf("caseins: %d\n", arguments.casesensitive);

    if (status == -1) {
        exit(EXIT_FAILURE);
    }
    else if (status == 1) {
        exit(EXIT_SUCCESS);
    }

    FILE* file1 = fopen(arguments.file1, "r");
    if (!file1) {
        error("Could not open inputfile (%s)!", arguments.file1);
        exit(EXIT_FAILURE);
    }
    
    FILE* file2 = fopen(arguments.file2, "r");
    if (!file2) {
        error("Could not open inputfile (%s)!", arguments.file2);
        exit(EXIT_FAILURE);
    }

    FILE* out;
    
    if (arguments.output == 0) {
        out = stdout;
    } else {
        out = fopen(arguments.output, "w");
    }
    if (!out) {
        error("Could not open outputfile (%s)!", arguments.output);
        exit(EXIT_FAILURE);
    }

    diff(arguments.casesensitive, file1, file2, out);
    status = EXIT_SUCCESS;

    if(fclose(file1)){
        error("Could not close inputfile (%s)!", arguments.file1);
        status = EXIT_FAILURE;
    }
    if(fclose(file2)){
        error("Could not close inputfile (%s)!", arguments.file2);
        status = EXIT_FAILURE;
    }

    if (arguments.output != 0) {
        if(fclose(out)) {
            error("Could not close outputfile (%s)!", arguments.output);
            status = EXIT_FAILURE;
        }
    }

    exit(status);
}