#include <string.h>
#include <ctype.h>
#include "diff.h"
#include "cli.h"

#define DEF_LINE_BUFF 64

/**
 * @brief dynamically growing string
 */
typedef struct {
    char* buff; ///< current buffer of size dyn_str.size
    int len;    ///< lenght of the cuffent string
    int size;   ///< size of the current buffer
} dyn_str;

/**
 * @brief adds a character to the dynamic string
 * 
 * @param s string
 * @param c character to add
 */
static void dstr_addc(dyn_str* s, char c) {
    if ((s->len+1) < s->size) {
        s->buff[s->len] = c;
        s->len += 1;
    } else {
        s->buff = realloc(s->buff, (s->size)*2);
        if (s->buff == NULL) {
            error("Could not expand string (aborting)!");
            exit(EXIT_FAILURE); // OOM, nothing better to do
        }
        s->size *= 2;
        s->buff[s->len] = c;
        s->len += 1;
    }
}

/**
 * @brief makes a new dynamic string
 * 
 * @param size          starting size of buffer
 * @return dyn_str*     pointer to string
 */
static dyn_str* dstr_new(int size) {
    dyn_str* s = malloc(sizeof(dyn_str));
    if (s == NULL) {
        error("Cannot aquire new memory for dyn_str (aborting)!");
        exit(EXIT_FAILURE);
    }
    s->size = size;
    s->len = 0;
    s->buff = malloc(size);
    if (s->buff == NULL) {
        error("Cannot aquire new memory for dyn_str buffer (aborting)!");
        exit(EXIT_FAILURE);
    }
    return s;
}

/**
 * @brief shrinkes the string to 0 length (buffer stayes untouched)
 * 
 * @param s string to shrink
 */
static void dstr_shrink(dyn_str* s) {
    s->len = 0;
}

/**
 * @brief frees the dynamic string
 * 
 * @param s string
 */
static void dstr_free(dyn_str* s) {
    free(s->buff);
    free(s);
}

/**
 * @brief Get the next line of the stream
 * 
 * @param buff      destination for line
 * @param stream    source stream
 */
static void get_line(dyn_str* buff, FILE* stream) {
    dstr_shrink(buff);

    int c = fgetc(stream);
    while((c != EOF) && (c != '\n')) {
        dstr_addc(buff, c);
        c = fgetc(stream);
    }
}

/**
 * @brief compare strings case sensitivly (only up to the size of the smaller one)
 * 
 * @param line1 string 1
 * @param line2 string 2
 * @return int  number of differences
 */
static int comp_case(dyn_str* line1, dyn_str* line2) {
    int N = (line1->len) <= (line2->len) ? line1->len : line2->len;
    int c = 0;
    int l, r;

    for (int i = 0; i < N; i++) {
        l = line1->buff[i];
        r = line2->buff[i];
        
        if (l != r) {
            c++;
        }
    }

    return c;
}

/**
 * @brief compare strings case insensitivly (only up to the size of the smaller one)
 * 
 * @param line1 string 1
 * @param line2 string 2
 * @return int  number of differences
 */
static int comp_ncase(dyn_str* line1, dyn_str* line2) {
    int N = (line1->len) <= (line2->len) ? line1->len : line2->len;
    int c = 0;
    int l, r;

    for (int i = 0; i < N; i++) {
        l = line1->buff[i];
        r = line2->buff[i];

        l = tolower(l);
        r = tolower(r);

        if (l != r) {
            c++;
        }
    }

    return c;
}

int diff(int casesensitive, FILE* file1, FILE* file2, FILE* out) {
    int count = 0;

    dyn_str* line1 = dstr_new(DEF_LINE_BUFF);
    dyn_str* line2 = dstr_new(DEF_LINE_BUFF);

    int i = 0;
    int d = 0;
    while (!feof(file1) && !feof(file2)) {
        get_line(line1, file1);
        get_line(line2, file2);
        i++;

        if (casesensitive) {
            d = comp_case(line1, line2);
        } else {
            d = comp_ncase(line1, line2);
        }

        if (d != 0) {
            fprintf(out, "Line: %d, characters: %d\n", i, d);
            count++;
        }
    }

    dstr_free(line1);
    dstr_free(line2);

    return count;
}