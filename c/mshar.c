/**
 * @file mshar.c
 * @author MXPSQL
 * @brief Main MShar Command line program
 * @version 0
 * @date 2022-06-04
 * 
 * @copyright 
 * 
 * MIT License
 * 
 * Copyright (c) 2022 MXPSQL
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#include "src/mshar.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

int main(int argc, char* argv[]){
    char* pre_script = NULL;
    char* post_script = NULL;
    char** files = NULL;
    FILE* f = NULL;
    char* script = NULL;

    /*
        usage: mshar [pre execution script] [post execution script] file1 file2 file3 file4 file5 file6 file7 file8 file9 ... > archive
        the [pre execution script] and the [post execution script] can be replaced with - for no script
     */

    if(argc < 3){
        fprintf(stderr, "usage: %s [pre execution script] [post execution script] file1 file2 file3 file4 file5 file6 file7 file8 file9 ... > archive\n", argv[0]);
        fprintf(stderr, "Put - for [pre execution script] and [post execution script] to not use a script\n");
        return EXIT_FAILURE;
    }

    pre_script = argv[1];
    post_script = argv[2];

    if(strcmp(pre_script, "-") == 0){
        pre_script = NULL;
    }
    else{
        size_t fsize;
        f = fopen(pre_script, "rb");
        if(f == NULL){
            fprintf(stderr, "Could not open pre script file %s\n", pre_script);
            return EXIT_FAILURE;
        }
        {
            while(fgetc(f) != EOF){;}
            fsize = ftell(f);
            fseek(f, 0, SEEK_SET);
        }
        pre_script = (char*) malloc(fsize + 1);
        fread(pre_script, fsize, 1, f);
        pre_script[fsize] = '\0';
        fclose(f);
    }

    if(strcmp(post_script, "-") == 0){
        post_script = NULL;
    }
    else{
        size_t fsize;
        f = fopen(post_script, "rb");
        if(f == NULL){
            fprintf(stderr, "Could not open post script file %s\n", post_script);
            return EXIT_FAILURE;
        }
        {
            while(fgetc(f) != EOF){;}
            fsize = ftell(f);
            fseek(f, 0, SEEK_SET);
        }
        post_script = (char*) malloc(fsize + 1);
        fread(post_script, fsize, 1, f);
        post_script[fsize] = '\0';
        fclose(f);
    }

    {
        int i;
        files = (char**) malloc(sizeof(char*) * (argc - 3));
        for(i = 3; i < argc; i++){
            files[i - 3] = argv[i];
        }
    }

    script = mkmshar_s(pre_script, post_script, files, argc - 3);
    if(script == NULL){
        fprintf(stderr, "Error creating script: %s\n", strerror(errno));
        /* perror("Error creating script"); */
        free(script);
        return EXIT_FAILURE;
    }
    else{
        fprintf(stdout, "%s", script);
        fflush(stdout);
        free(script);
    }

    return EXIT_SUCCESS;
}
