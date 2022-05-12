/**
 * @file mshar.h
 * @author MXPSQL
 * @brief MShar library in a header
 * @version 0 (rolling release)
 * @date 2022-05-12
 * 
 * @copyright Copyright (c) 2022 MXPSQL, under the MIT License
 * 
 * @mainpage ANSI C Implementation of MShar
 * 
 * This is the documentation about the ANSI C implementation of MShar.
 */

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

#ifndef MXPSQL_MShar_H
#define MXPSQL_MShar_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/**
 * Thanks https://github.com/skullchap/b64/blob/master/b64.h for the code.
 * It is licensed under the unlicense.
 */
char* mkmshar_b64Encode(char *data)
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

    ssize_t inlen = strlen(data);
    ssize_t outlen = ((((inlen) + 2) / 3) * 4);

    char *out = (char*) malloc(outlen + 1);
    char *p;
    ssize_t i;

    if (out == NULL) return NULL;
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


/**
 * @brief Make an MShar archive
 * @warning This function's return value must be manually freed if not null
 * 
 * @param prescript the script to run before extraction, put NULL if empty and do not put the filename, put the content of the script you want to run
 * @param postscript the script to run after extraction, put NULL if empty and do not put the filename, put the content of the script you want to run
 * @param files the files to be archived
 * @param nfiles how many files to archive
 * @return char* the archive or NULL if there is a problem
 */
char* mkmshar(char* prescript, char* postscript, char** files, size_t nfiles){
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
TTk=\"$(find /bin /usr/bin /usr/local/bin . -name base64* -type f 2> /dev/null | head -n 1)\";\n\
\n\n\n";

        if(strlen(prestr) + strlen(arcfile) > initarcfilesize){
            arcfile = (char*) realloc(arcfile, strlen(prestr) + strlen(arcfile));
            if(arcfile == NULL){
                return NULL;
            }
        }

        strcat(arcfile, prestr);
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
            free(arcfile);
            return NULL;
        }

        /* Read until the end and get the size as C++ does not need to implement SEEK_END */
        {
            while(fgetc(fptr) != EOF){;} /* this may seem hacky, but it is portable due to C++ not mandating to implement SEEK_END just like the comment before */
            fsize = ftell(fptr);
            fseek(fptr, 0, SEEK_SET);
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

                    sprintf(tektsrc, "TEKTONE='%s';\n", files[i]);
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

                    bas64 = mkmshar_b64Encode(filecontent);

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

                    sprintf(basprintf, fmt, bas64);

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

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif