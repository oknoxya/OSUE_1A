#pragma once
/**
 * @file cmd.h
 * @author Noel Atzwanger (e12002121@student.tuwien.ac.at)
 * @brief CLI module for parsing commands and logging.
 * @date 2022-11-10
 * 
 */

/**
 * @brief holds the arguments parsed
 */
typedef struct {
    char* file1;        ///< input file 1
    char* file2;        ///< input file 2
    char* output;       ///< output file (NULL for stdout)
    int casesensitive;  ///< casesensitive
} args;

/**
 * @brief prints help message
 * 
 */
void help(void);

/**
 * @brief prints usage
 * 
 */
void usage(void);

/**
 * @brief parses the command line arguments
 * 
 * @param a     pointer to allocated args struct
 * @param argc  count of arguments
 * @param args  list of arguments
 * @return int  status code (0 ok, 1 help/usage, -1 error)
 */
int parse_args(args* a, int argc, char** args);

/**
 * @brief prints message of level error
 * 
 * @param message message to print
 */
void error(const char *fmt, ...);

/**
 * @brief prints message of level warning
 * 
 * @param message message to print
 */
void warning(const char *fmt, ...);

/**
 * @brief prints message of level info
 * 
 * @param message message to print
 */
void info(const char *fmt, ...);