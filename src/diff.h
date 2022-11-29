#pragma once
/**
 * @file diff.h
 * @author Noel Atzwanger (e12002121@student.tuwien.ac.at)
 * @brief Module for diffing files
 * @date 2022-11-11
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Diffs two files (with or without casesensetivity) and outputs the results.
 * 
 * @param casesensitive is the output case sensitive or not
 * @param file1         input file 1
 * @param file2         input file 2
 * @param out           output file
 * @return int          accumulated number of differences
 */
int diff(int casesensitive, FILE* file1, FILE* file2, FILE* out);
