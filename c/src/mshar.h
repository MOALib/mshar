/**
 * @file mshar.h
 * @author MXPSQL
 * @brief MShar library in a header
 * @version 0 (rolling release)
 * @date 2022-05-12
 * 
 * @copyright 
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
 * 
 * @mainpage ANSI C Implementation of MShar
 * 
 * This is the documentation about the ANSI C implementation of MShar.
 */

#ifndef MXPSQL_MShar_H
#define MXPSQL_MShar_H

#if defined(__cplusplus) || defined(c_plusplus)
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#else
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/**
 * Thanks https://github.com/skullchap/b64/blob/master/b64.h for the code.
 * It is licensed under the unlicense.
 * Modified to support binary files (NUL character present in windows binaries, preventing it from working properly)
 */
char* mkmshar_b64Encode(char *data, size_t inlen);

/**
 * @brief Make an MShar archive
 * @warning This function's return value must be manually freed if not null
 * 
 * @param prescript the script to run before extraction, put NULL if empty and do not put the filename, put the content of the script you want to run
 * @param postscript the script to run after extraction, put NULL if empty and do not put the filename, put the content of the script you want to run
 * @param files the files to be archived
 * @param nfiles how many files to archive
 * @param ignorefileerrors Ignore file errors and continue, set to 0 to not ignore
 * @return char* the archive or NULL if there is a problem
 */
char* mkmshar(char* prescript, char* postscript, char** files, size_t nfiles, int ignorefileerrors);

/**
 * @brief A version of mkmshar, but on error, return null. Internally uses mkmshar
 * 
 * @param prescript script to be executed before extraction, put NULL if empty and do not put the filename, put the content of the script you want to run
 * @param postscript script to be executed after extraction, put NULL if empty and do not put the filename, put the content of the script you want to run
 * @param files the path to files
 * @param nfiles the number of files
 * @return char* the archive or NULL if there is a problem 
 * 
 * @see mkmshar
 */
char* mkmshar_x(char* prescript, char* postscript, char** files, size_t nfiles);

/**
 * @brief A version of mkmshar, but on error, ignores it. Internally uses mkmshar
 * 
 * @param prescript the script to be executed before extraction, put NULL if empty and do not put the filename, put the content of the script you want to run
 * @param postscript the script to be executed after extraction, put NULL if empty and do not put the filename, put the content of the script you want to run
 * @param files the path to the files
 * @param nfiles the number of files
 * @return char* the archive or NULL if there is a problem
 * 
 * @see mkmshar
 */
char* mkmshar_s(char* prescript, char* postscript, char** files, size_t nfiles);





#ifndef MXPSQL_MShar_NO_IMPL_4_LANG_BINDING /* define this if you want a custom implementation or you need to write a binding */

char* mkmshar_b64Encode(char *data, size_t inlen)
{
    static const char b64e[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'};

    size_t outlen = ((((inlen) + 2) / 3) * 4);

    char *out = (char*) malloc(outlen + 1);
    char *p;
    size_t i;

    if (out == NULL) {
        return NULL;
    }
    out[outlen] = '\0';
    p = out;

    for (i = 0; i < inlen - 2; i += 3)
    {
        *p++ = b64e[(data[i] >> 2) & 0x3F];
        *p++ = b64e[((data[i] & 0x3) << 4) | ((data[i + 1] & 0xF0) >> 4)];
        *p++ = b64e[((data[i + 1] & 0xF) << 2) | ((data[i + 2] & 0xC0) >> 6)];
        *p++ = b64e[data[i + 2] & 0x3F];
    }

    if (i < inlen)
    {
        *p++ = b64e[(data[i] >> 2) & 0x3F];
        if (i == (inlen - 1))
        {
            *p++ = b64e[((data[i] & 0x3) << 4)];
            *p++ = '=';
        }
        else
        {
            *p++ = b64e[((data[i] & 0x3) << 4) | ((data[i + 1] & 0xF0) >> 4)];
            *p++ = b64e[((data[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    return out;
}


char* mkmshar(char* prescript, char* postscript, char** files, size_t nfiles, int ignorefileerrors){
    register const size_t initarcfilesize = ((sizeof(char*)) * 1024);
    size_t i;

    char* arcfile = (char*) calloc(initarcfilesize, initarcfilesize);

    if(!arcfile){
        free(arcfile);
        return NULL;
    }

    {
        static const char* prestr = 
"#!/bin/sh \n\
# This archive is created using MShar, MXPSQL's version of the Shell archiver\
# You need a unix bourne shell and the base64 command to extract this\n\
TEKTONE=;\n\
TTk=\"$(find /bin /usr/bin /usr/local/bin . -name base64* -type f 2> /dev/null | head -n 1)\";\n\n";

        static const char* prestr2 =
"echo \"This archive is created with MShar (MXPSQL's version of the Shell archiver)\";\n\
\n\
if test -z \"$TTk\"; then\n\
    printf \"The base64 command is not found you loser. \nIt is needed to extract the archive. \nPlease make it available in PATH or install it.\";\n\
    exit 1;\n\
fi\n\
\n\n\n";

        if(strlen(prestr) + strlen(prestr2) + strlen(arcfile) > initarcfilesize){
            arcfile = (char*) realloc(arcfile, strlen(prestr) + strlen(prestr2) + strlen(arcfile));
            if(arcfile == NULL){
                return NULL;
            }
        }

        strcat(arcfile, prestr);
        strcat(arcfile, prestr2);
    }

    if(prescript != NULL){
        if(strlen(arcfile) + strlen(arcfile) > strlen(arcfile)){
            arcfile = (char*) realloc(arcfile, strlen(arcfile) + strlen(prescript) + 1);
            if(arcfile == NULL){
                return NULL;
            }
        }
        strcat(arcfile, prescript);
    }

    for(i = 0; i < nfiles; i++){
        size_t fsize = 0;
        FILE* fptr = fopen(files[i], "rb");
        char* filecontent = NULL;
        if(fptr == NULL){
            if(ignorefileerrors != 0){
                continue;
            }
            else{
                free(arcfile);
                return NULL;
            }
        }

        if(ferror(fptr)){
            if(ignorefileerrors != 0){
                continue;
            }
            else{
                fclose(fptr);
                free(arcfile);
                return NULL;
            }
        }

        /* Read until the end and get the size as C++ does not need to implement SEEK_END */
        {
            while(fgetc(fptr) != EOF){;} /* this may seem hacky and unsophisticated, but it is portable and sophisticated due to C++ not mandating to implement SEEK_END just like the comment before. 
            All you do is read until you reach EOF, then get the file size and return to beginning. */
            if(fseek(fptr, 0, SEEK_CUR) != 0){
                if(ignorefileerrors != 0){
                    continue;
                }
                else{
                    fclose(fptr);
                    free(arcfile);
                    return NULL;
                }
            }
            fsize = ftell(fptr);
            if(fseek(fptr, 0, SEEK_SET) != 0){
                if(ignorefileerrors != 0){
                    continue;
                }
                else{
                    fclose(fptr);
                    free(arcfile);
                    return NULL;
                }
            }

        }

        if(ferror(fptr)){
            if(ignorefileerrors != 0){
                continue;
            }
            else{
                fclose(fptr);
                free(arcfile);
                return NULL;
            }
        }

        filecontent = (char*) malloc(fsize);
        if(filecontent == NULL){
            free(arcfile);
            free(filecontent);
            fclose(fptr);
            return NULL;
        }

        fread(filecontent, fsize, 1, fptr);
        fclose(fptr);

        /*  realloc if needed */
        {
            if(strlen(arcfile) + strlen(filecontent) > strlen(arcfile)){
                arcfile = (char*) realloc(arcfile, strlen(arcfile) + strlen(filecontent) + 1);
            }

            {
                char* fileblock = (char*) calloc(initarcfilesize, initarcfilesize);
                if(fileblock == NULL){
                    free(arcfile);
                    free(filecontent);
                    return NULL;
                }

                {
                    char* tektsrc = (char*) calloc(initarcfilesize, initarcfilesize);
                    if(tektsrc == NULL){
                        free(arcfile);
                        free(filecontent);
                        free(fileblock);
                        return NULL;
                    }

                    if(strlen(files[i]) + strlen(tektsrc) > strlen(fileblock)){
                        tektsrc = (char*) realloc(tektsrc, strlen(files[i]) + strlen(tektsrc) + 1);
                        if(tektsrc == NULL){
                            free(arcfile);
                            free(filecontent);
                            free(fileblock);
                            return NULL;
                        }
                    }

                    if(sprintf(tektsrc, "TEKTONE='%s';\n", files[i]) < 0){
                        free(arcfile);
                        free(filecontent);
                        free(fileblock);
                        free(tektsrc);
                        return NULL;
                    }
                    strcat(fileblock, tektsrc);
                    free(tektsrc);
                }

                {
                    char* dirnam = "mkdir ./$(dirname \"$TEKTONE\") 2> /dev/null;\n";
                    if(strlen(dirnam) + strlen(fileblock) > strlen(fileblock)){
                        fileblock = (char*) realloc(fileblock, strlen(dirnam) + strlen(fileblock) + 1);
                        if(fileblock == NULL){
                            free(arcfile);
                            free(filecontent);
                            free(fileblock);
                            return NULL;
                        }
                    }

                    strcat(fileblock, dirnam);
                }

                {
                    char* info = "echo \"x - $TEKTONE\";\n";
                    if(strlen(info) + strlen(fileblock) > strlen(fileblock)){
                        fileblock = (char*) realloc(fileblock, strlen(info) + strlen(fileblock) + 1);
                        if(fileblock == NULL){
                            free(arcfile);
                            free(filecontent);
                            free(fileblock);
                            return NULL;
                        }
                    }

                    strcat(fileblock, info);
                }

                {
                    char* basprintf = (char*) calloc(initarcfilesize, initarcfilesize);
                    const char* fmt = "printf '%s' > \"./$TEKTONE\";\n";
                    char* bas64 = NULL;
                    if(basprintf == NULL){
                        free(arcfile);
                        free(filecontent);
                        free(fileblock);
                        return NULL;
                    }

                    bas64 = mkmshar_b64Encode(filecontent, fsize);

                    if(bas64 == NULL){
                        bas64 = (char*) malloc(sizeof(char));
                        if(bas64 == NULL){
                            free(arcfile);
                            free(filecontent);
                            free(fileblock);
                            free(basprintf);
                            return NULL;
                        }
                        strcat(bas64, "");
                    }

                    if(sprintf(basprintf, fmt, bas64) < 0){
                        free(arcfile);
                        free(filecontent);
                        free(fileblock);
                        free(basprintf);
                        free(bas64);
                        return NULL;
                    }

                    if(strlen(basprintf) + strlen(fileblock) > strlen(fileblock)){
                        fileblock = (char*) realloc(fileblock, strlen(basprintf) + strlen(fileblock) + 1);
                        if(fileblock == NULL){
                            free(arcfile);
                            free(filecontent);
                            free(fileblock);
                            free(basprintf);
                            return NULL;
                        }
                    }

                    strcat(fileblock, basprintf);

                    free(bas64);
                }

                {
                    char* debas64tmp = 
"tmp=$(mktemp);\n\
\"$TTk\" -d \"./$TEKTONE\" > \"$tmp\";\n\
mv \"$tmp\" \"./$TEKTONE\";\n\
tmp=;\
\n\n";
                    if(strlen(debas64tmp) + strlen(fileblock) > strlen(fileblock)){
                        fileblock = (char*) realloc(fileblock, strlen(debas64tmp) + strlen(fileblock) + 1);
                        if(fileblock == NULL){
                            free(arcfile);
                            free(filecontent);
                            free(fileblock);
                            return NULL;
                        }
                    }
                    strcat(fileblock, debas64tmp);
                }

                if(strlen(fileblock) + strlen(arcfile) > strlen(arcfile)){
                    arcfile = (char*) realloc(arcfile, strlen(arcfile) + strlen(fileblock) + 1);
                    if(fileblock == NULL){
                        free(arcfile);
                        free(filecontent);
                        free(fileblock);
                        return NULL;
                    }
                }
                strcat(arcfile, fileblock);
            }
        }

        free(filecontent);
    }

    if(postscript != NULL){
        if(strlen(arcfile) + strlen(postscript) > strlen(arcfile)){
            arcfile = (char*) realloc(arcfile, strlen(arcfile) + strlen(postscript) + 1);
            if(arcfile == NULL){
                free(arcfile);
                return NULL;
            }
        }
        strcat(arcfile, postscript);
    }

    {
        char* poststr = 
"\n\
# This is a shell archive lol, created with mshar (MXPSQL's version of the Shell archiver)\n\
exit 0;";

        if(strlen(poststr) + strlen(arcfile) > strlen(arcfile)){
            arcfile = (char*) realloc(arcfile, strlen(arcfile) + strlen(poststr) + 1);
            if(arcfile == NULL){
                free(arcfile);
                return NULL;
            }
        }

        strcat(arcfile, poststr);
    }

    return arcfile;
}

char* mkmshar_x(char* prescript, char* postscript, char** files, size_t nfiles){
    return mkmshar(prescript, postscript, files, nfiles, 0);
}

char* mkmshar_s(char* prescript, char* postscript, char** files, size_t nfiles){
    return mkmshar(prescript, postscript, files, nfiles, 1);
}

#endif

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif