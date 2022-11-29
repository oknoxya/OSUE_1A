/**
 * @file cmd.c
 * @author Noel Atzwanger (e12002121@student.tuwien.ac.at)
 * @date 2022-11-10
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include "cli.h"

/**
 * @brief tries to create the specified file and reports the success.
 * 
 * @param filepath file to create
 * @return int success as a parse_args status
 */
static int check_and_create(char* filepath) {
    FILE* fd = fopen(filepath, "w");

    if (fd) {
        if(fclose(fd)) { // generally a very bad sign (so just stop)
            error("Failed creating output file (%s)!", filepath);
            return -1;
        } else {
            return 0;
        }
    } else {
        error("Failed creating output file (%s)!", filepath);
        return -1;
    }
    return -1; // just in case
}

/**
 * @brief checks if the arguments are sane. (VONURABLE TO TOCTOU)
 * 
 * @param a arguments to check
 * @return int status like parse_args
 */
static int check_args(args* a) {
    if (access(a->file1, R_OK) != 0) {
        error("The first input file (%s) cannot be read from!", a->file1);
        return -1;
    }
    if (access(a->file2, R_OK) != 0) {
        error("The second input file (%s) cannot be read from!", a->file2);
        return -1;
    }
    if (a->output != 0) {
        if (access(a->output, F_OK) == 0) {
            if (access(a->output, W_OK) == 0) {
                warning("The output file (%s) already exists an will be overwritten.", a->output);
            } else {
                error("The output file (%s) already exists and cannot be written to!", a->output);
                return -1;
            }
        } else {
            return check_and_create(a->output);
        }
    }
    return 0;
}

void error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fputs("mydiff error: ", stderr);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
}

void warning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fputs("mydiff warning: ", stderr);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
}

void info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void help(void) {
    printf("mydiff v1.0.2\n");
    printf("\n");

    usage();

    printf("\n");
    printf("optional arguments:\n");
    printf("    -h          print usage and exit\n");
    printf("    -i          case insensitive\n");
    printf("    -o [OUT]    output to OUT instead of stdout\n\n");

    printf("positional arguments\n");
    printf("    file1       input file 1\n");
    printf("    file2       input file 2\n");
}

void usage(void) {
    printf("mydiff [-h] [-i] [-o OUT] file1 file2\n");
}

int parse_args(args* a, int argc, char** argv) {
    
    if (argc <= 1) {
        usage();
        return 1;
    }
    
    char* pattern = ":hio:";
    int c = 0;

    a->file1 = 0;
    a->file2 = 0;
    a->output = 0;
    a->casesensitive = 1;

    while ((c = getopt(argc, argv, pattern)) != -1) {
        switch (c) {
            case 'h': 
                help();
                return 1;
                break;
            case 'i':
                a->casesensitive = 0;
                break;
            case 'o':
                a->output = optarg;
                break;
            case ':':
                error("The option '%c' does not have an argument!", c);
                return -1;
                break;
            case '?':
                error("Unknown option '%c' encountered!", optopt);
                return -1;
                break;
            default:
                error("getopt returned unexpected value (%d)!", c);
                return -1;
                break;
        }
    }

    if ( (argc - optind) != 2) {
        error("2 positional arguments are required %d where given!", (argc - optind));
        return -1;
    }

    a->file1 = argv[optind];
    a->file2 = argv[optind+1];

    return check_args(a);
}