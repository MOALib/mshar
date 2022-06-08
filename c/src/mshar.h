/**
 * @file mshar.h
 * @author MXPSQL
 * @brief MShar library in a header.
 * Header only (there is an exception) library for creating MShar Archives, a self extracting archive format based of shar.
 * @version 0 (unscheduled release)
 * @date 2022-05-12
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

#ifndef MXPSQL_MShar_H
/**
 * @brief Include guard flag
 * 
 */
#define MXPSQL_MShar_H

#if __STDC_VERSION__ >= 201112L

    #ifndef __STDC_WANT_LIB_EXT1__
        /* we want those bounds checked functions if available */

        /**
         * @brief We want bounds checked functions
         * 
         */
        #define __STDC_WANT_LIB_EXT1__ 1
    #endif

#endif

#if defined(__cplusplus) || defined(c_plusplus)
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <clocale>
#else
#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>
#endif

/* All these to be used for later things, right now no use. */
#if (defined(__linux__) || defined(linux) || defined(__linux))
    /**
     * @brief I liek linux. I use arch btw.
     * 
     */
    #define MXPSQL_MShar_OS_Linux
#endif

#if (defined(unix) || defined(__unix) || defined(__unix__))
    /**
     * @brief Unixfication of MShar
     * 
     */
    #define MXPSQL_MShar_OS_Unix
#endif

#if defined(__APPLE__)
    /**
     * @brief Classic 68000 MacOS
     * 
     */
    #define MXPSQL_MShar_OS_MacOS

    #if defined(__MACH__)
        /**
         * @brief New MacOS (PowerPC, x86, x86_64 and ARM)
         * 
         */
        #define MXPSQL_MShar_OS_MacOSX
    #endif

#endif



#if (defined(MXPSQL_MShar_OS_Unix) || defined(MXPSQL_MShar_OS_MacOSX) || defined(__CYGWIN__) || defined(MXPSQL_MShar_OS_Linux) || defined(__FreeBSD__))

    /* 
    #ifndef _BSD_SOURCE
        #define _BSD_SOURCE
    #endif

    #ifndef _GNU_SOURCE
        #define _GNU_SOURCE
    #endif

    #ifndef _POSIX_SOURCE
        #define _POSIX_SOURCE
    #endif

    #ifndef _XOPEN_SOURCE
        #define _XOPEN_SOURCE
    #endif

    #ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE
    #endif

#include <unistd.h> 
    */

   /**
    * @brief We like POSIX and SUS (Not sus, but Single Unix Specification)
    * 
    */
    #define MXPSQL_MShar_OS_POSIX_SUS

#endif

#ifndef __STDC__
#error "MShar requires an ANSI C compiler"
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


/* Use your own allocator here if you want to */
#ifndef MXPSQL_MShar_Malloc
/**
 * @brief Define this macro to use your own custom malloc functio for your own allocator
 * 
 */
#define MXPSQL_MShar_Malloc(size) malloc(size)
#endif

#ifndef MXPSQL_MShar_Calloc
/**
 * @brief Same thing with MXPSQL_MShar_Malloc
 * 
 * @see MXPSQL_MShar_Malloc
 */
#define MXPSQL_MShar_Calloc(count, size) calloc(count, size)
#endif

#ifndef MXPSQL_MShar_Realloc
/**
 * @brief Same thing with MXPSQL_MShar_Calloc
 * 
 * @see MXPSQL_MShar_Calloc
 */
#define MXPSQL_MShar_Realloc(ptr, size) realloc(ptr, size)
#endif

#ifndef MXPSQL_MShar_Free
/**
 * @brief Same thing with MXPSQL_MShar_Realloc
 * 
 * @see MXPSQL_MShar_Realloc
 */
#define MXPSQL_MShar_Free(ptr) free(ptr)
#endif




/**
 * Thanks https://github.com/skullchap/b64/blob/master/b64.h for the code.
 * It is licensed under the unlicense.
 * Modified to support binary files (NUL character present in windows binaries, preventing it from working properly)
 */
char* mkmshar_b64Encode(char *data, size_t inlen);

/**
 * @brief strnlen function for mkmshar if not compiled on posix platforms, uses strlen from string if compiled on posix platforms.
 * 
 * @param str the string to checl
 * @param maxlen the maximum length of the string
 * @return size_t the length of the string
 */
size_t mkmshar_strnlen(const char *str, size_t maxlen);

/**
 * @brief Back the int for mkmshar_snprintf if using C90, no use if the standard version is beyond C90.
 * 
 * @param x the integer to be converted
 * @param buf the buffer to be filled
 * @param size the size of the buffer
 * @param base the base of the integer
 * @param uppercase if true, the result will be uppercase
 * @return int the number of characters written
 */
int mkmshar_int_to_str(int x, char *buf, size_t size, int base, int uppercase);

/**
 * @brief A dumb (and potentially very dummy and gummy) version of vsnprintf that backs mkmshar_snprintf.  
 * It's quite dumb.
 * 
 * Do not use this function for portability reasons (that is that it does not implement everything like '%p').
 * If if you need more elaborate format flags, go use a C standards compliant version.
 * This is not a standards compliant version, I just need a dumb one for C90.
 * Just do not use this even if you don't need elaborate flags like '%p' or '%z'.
 * 
 * 
 * @note You need to start and end the va_list manually.
 * 
 * @warning Not portable, I told you.
 * 
 * Derived from https://codereview.stackexchange.com/questions/132860/my-own-snprintf-implementation-in-c
 * 
 * @param buf the buffer to be filled
 * @param size the size of the buffer
 * @param fmt the format string
 * @param ap the variadic arguments list
 * @return int the number of characters written
 */
int mkmshar_dumbvsnprintf(char *buf, size_t size, const char *fmt, va_list ap);

/**
 * @brief snprintf for mkmshar if using C90, uses snprintf from stdio if the standard version beyond C90. 
 * Not portable if C standard version is below C99.
 * 
 * 
 * @warning mkmshar_dumbvsnprintf is why this function is not portable if the standard version is below C99.
 * 
 * @param str the string to copy to
 * @param size how many bytes to copy
 * @param format the format string
 * @param ... the arguments to the format string
 * @return int the number of bytes copied, negative on failure
 * 
 * @see mkmshar_int_to_str
 * @see mkmshar_strnlen
 * @see mkmshar_dumbvsnprintf
 */
int mkmshar_snprintf(char *str, size_t size, const char *format, ...);

/**
 * @brief Make an MShar archive
 * 
 * @note You may want to save your current locale because it will be changed to "C". This function will try to set it back to the old locale, but just save it just in case it doesn't (this is a bug, please report it).
 * @warning This function's return value must be manually freed if not null
 * 
 * @param prescript the script to run before extraction. Put NULL if empty and do not put the filename, put the content of the script you want to run (putting the filename will just place the filename, not the content)
 * @param postscript the script to run after extraction. Put NULL if empty and do not put the filename, put the content of the script you want to run (putting the filename will just place the filename, not the content)
 * @param files the files to be archived
 * @param nfiles how many files to archive
 * @param ignorefileerrors Ignore file errors and continue, set to 0 to not ignore
 * @return char* the archive or NULL if there is a problem (memory allocation failures, file errors or too big base64)
 */
char* mkmshar(char* prescript, char* postscript, char** files, size_t nfiles, int ignorefileerrors);

/**
 * @brief A version of mkmshar, but on all error, return null. Internally uses mkmshar.
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
 * @brief A version of mkmshar, but on file operation error, ignores it.
 * 
 * Does not ignore sprintf and memory allocation errors though, this means it returns null on those kind of errors. Internally uses mkmshar.
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
    #if defined(__cplusplus) || defined(c_plusplus)
    using namespace std;
    #endif

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

    char *out = (char*) MXPSQL_MShar_Malloc(outlen + 1);
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

size_t mkmshar_strnlen(const char *s, size_t maxlen)
{

    #if defined(__cplusplus) || defined(c_plusplus)
    using namespace std;
    #endif

    #if ((__STDC_VERSION__ >= 201112L) && defined(__STDC_LIB_EXT1__))
        return strnlen_s(s, maxlen);
    #else
    size_t len = 0;
    while (len < maxlen && s[len] != '\0') {
        len++;
    }
    return len;
    #endif
}

int mkmshar_int_to_str(int x, char *buf, size_t size, int base, int uppercase) {

    #if defined(__cplusplus) || defined(c_plusplus)
    using namespace std;
    #endif

    static const int INT_TO_STR_DIGITS_L[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    static const int INT_TO_STR_DIGITS_U[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    int length = (int)ceil(log((double)x)/log((double)base));
    int r, i = 0;
    char c;

    if (size < ((size_t) length)) {
        x /= (int)pow(base, (float)(length - size));
        length = size;
    }

    do {
        if (((size_t) i) >= size) break;
        r = x % base;
        if (uppercase) {
            c = INT_TO_STR_DIGITS_U[r];
        } else {
            c = INT_TO_STR_DIGITS_L[r];
        }
        buf[length-i-1] = c;
        x /= base;
        i++;
    } while (x != 0);

    return i;
}

int mkmshar_dumbvsnprintf(char *str, size_t size, const char *format, va_list arg_list) {
    /* dumb snprintf implemenration */

    #if defined(__cplusplus) || defined(c_plusplus)
    using namespace std;
    #endif

    int chars_printed = 0;
    char c, *str_arg;
    int num, len, i;
    int uppercase = 0, base = 10;
    size_t max_size = size;

    for (i = 0; format[i] != 0; i++) {
        if ((max_size - chars_printed <= 0) && (size != 0)) {
            break;
        } else if (format[i] == '%') {
            i++;
            switch (format[i]) {
            case 'c':
                c = va_arg(arg_list, int);
                str[chars_printed++] = c;
                break;
            case '%':
                str[chars_printed++] = '%';
                break;
            case 's':
                str_arg = va_arg(arg_list, char *);
                len = mkmshar_strnlen(str_arg, max_size - chars_printed);
                strncpy(str+chars_printed, str_arg, len);
                chars_printed += len;
                break;
            case 'H':
                uppercase = 1;
                continue;
            case 'h':
                base = 16;
                continue;
            case 'd':
                num = va_arg(arg_list, int);
                len = mkmshar_int_to_str(num, str+chars_printed, max_size - chars_printed, base, uppercase);
                chars_printed += len;
                break;
            default:
                return -1;
            }
        } else {
            str[chars_printed++] = format[i];
        }
    }

    if (((size_t) chars_printed) == max_size) chars_printed--;
    str[chars_printed] = 0;

    return chars_printed;
}

int mkmshar_snprintf(char *str, size_t size, const char *format, ...)
{
    #if defined(__cplusplus) || defined(c_plusplus)
    using namespace std;
    #endif

    va_list arg_list;
    int chars_printed = 0;
    va_start(arg_list, format);

    /* check c standard version */
    #if ((!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)) || (defined(__STDC__) && !defined(__STDC_VERSION__)))
    chars_printed = mkmshar_dumbvsnprintf(str, size, format, arg_list);
    #else
    chars_printed = vsnprintf(str, size, format, arg_list);
    #endif

    va_end(arg_list);
    return chars_printed;
}

char* mkmshar(char* prescript, char* postscript, char** files, size_t nfiles, int ignorefileerrors){
    
    #if defined(__cplusplus) || defined(c_plusplus)
    using namespace std;
    #endif

    static const size_t initarcfilesize = (((sizeof(char*) + sizeof(size_t)) * 1024) + 1);
    const char* old_locale = setlocale(LC_ALL, NULL);
    size_t i;

    char* arcfile = (char*) MXPSQL_MShar_Calloc(initarcfilesize, initarcfilesize);

    setlocale(LC_ALL, "C");

    /* printf("dumb: %d", mkmshar_snprintf(NULL, 0, "dumb %s", "snprintf")); */

    if(files == NULL){
        MXPSQL_MShar_Free(arcfile);
        setlocale(LC_ALL, old_locale);
        return NULL;
    }

    if(!arcfile){
        MXPSQL_MShar_Free(arcfile);
        setlocale(LC_ALL, old_locale);
        return NULL;
    }

    {
        static const char* prestr = (char*)
"#!/bin/sh \n\
# This archive is created using MShar, MXPSQL's version of the Shell archiver\n\
# You need a unix bourne shell and the base64 command to extract this\n\
\n\
# shellcheck disable=SC2034 # GNU utilities\n\
\n\
TEKTONE=;\n\
DIRNAME=;\n\
POSIXLY_CORRECT=1; # Make this posix \n\
POSIX_ME_HARDER=1; # Make this posix \n\
TTk=\"$(find /bin /usr/bin /usr/local/bin . -name 'base64*' -type f 2> /dev/null | head -n 1)\";\n\n";

        static const char* prestr2 = (char*)
"printf \"This archive is created with MShar (MXPSQL's version of the Shell archiver)\\n\";\n\
\n\
if test -z \"$TTk\"; then\n\
    printf \"The base64 command is not found you loser. \\nIt is needed to extract the archive. \\nPlease make it available in PATH or install it.\\n\";\n\
    exit 1;\n\
fi\n\
\n\n\n";

        if(strlen(prestr) + strlen(prestr2) + strlen(arcfile) > initarcfilesize){
            arcfile = (char*) MXPSQL_MShar_Realloc(arcfile, strlen(prestr) + strlen(prestr2) + strlen(arcfile));
            if(arcfile == NULL){
                setlocale(LC_ALL, old_locale);
                return NULL;
            }
        }

        strcat(arcfile, prestr);
        strcat(arcfile, prestr2);
    }

    if(prescript != NULL){
        if(strlen(arcfile) + strlen(arcfile) > strlen(arcfile)){
            arcfile = (char*) MXPSQL_MShar_Realloc(arcfile, strlen(arcfile) + strlen(prescript) + 1);
            if(arcfile == NULL){
                setlocale(LC_ALL, old_locale);
                return NULL;
            }
        }
        strcat(arcfile, prescript);
    }

    for(i = 0; i < nfiles; i++){
        size_t fsize = 0;
        FILE* fptr = NULL;
        char* filecontent = NULL;

        if(files[i] == NULL) {
            if(ignorefileerrors != 0){
                continue;
            } else {
                MXPSQL_MShar_Free(arcfile);
                setlocale(LC_ALL, old_locale);
                return NULL;
            }
        }

        fptr = fopen(files[i], "rb");
        if(fptr == NULL){
            if(ignorefileerrors != 0){
                continue;
            }
            else{
                MXPSQL_MShar_Free(arcfile);
                setlocale(LC_ALL, old_locale);
                return NULL;
            }
        }

        if(ferror(fptr) != 0){
            if(ignorefileerrors != 0){
                continue;
            }
            else{
                fclose(fptr);
                MXPSQL_MShar_Free(arcfile);
                setlocale(LC_ALL, old_locale);
                return NULL;
            }
        }

        /* Read until the end and get the size as C++ does not need to implement SEEK_END */
        {
            long int ifsize = 0;

            while(fgetc(fptr) != EOF || ferror(fptr)){;} /* this may seem hacky and unsophisticated, but it is portable and sophisticated due to C++ not mandating to implement SEEK_END just like the comment before (no longer existing). 
            All you do is read until you reach EOF, then get the file size and return to beginning. 
            This will also return if an error occured.
            */

            if(ferror(fptr)){
                if(ignorefileerrors != 0){
                    continue;
                }
                else{
                    fclose(fptr);
                    MXPSQL_MShar_Free(arcfile);
                    setlocale(LC_ALL, old_locale);
                    return NULL;
                }
            }

            if(fseek(fptr, 0, SEEK_CUR) != 0){
                if(ignorefileerrors != 0){
                    continue;
                }
                else{
                    fclose(fptr);
                    MXPSQL_MShar_Free(arcfile);
                    setlocale(LC_ALL, old_locale);
                    return NULL;
                }
            }

            ifsize = ftell(fptr);
            if(ifsize < 0){
                if(ignorefileerrors != 0){
                    continue;
                }
                else{
                    fclose(fptr);
                    MXPSQL_MShar_Free(arcfile);
                    setlocale(LC_ALL, old_locale);
                    return NULL;
                }
            }
            else{
                fsize = (size_t) ifsize;
            }

            if(fseek(fptr, 0, SEEK_SET) != 0){
                if(ignorefileerrors != 0){
                    continue;
                }
                else{
                    fclose(fptr);
                    MXPSQL_MShar_Free(arcfile);
                    setlocale(LC_ALL, old_locale);
                    return NULL;
                }
            }

        }

        if(ferror(fptr) != 0){
            if(ignorefileerrors != 0){
                continue;
            }
            else{
                fclose(fptr);
                MXPSQL_MShar_Free(arcfile);
                setlocale(LC_ALL, old_locale);
                return NULL;
            }
        }

        filecontent = (char*) MXPSQL_MShar_Malloc(fsize);
        if(filecontent == NULL){
            MXPSQL_MShar_Free(arcfile);
            MXPSQL_MShar_Free(filecontent);
            fclose(fptr);
            setlocale(LC_ALL, old_locale);
            return NULL;
        }

        fread(filecontent, fsize, 1, fptr);
        fclose(fptr);

        /*  MXPSQL_MShar_Realloc if needed */
        {
            if(strlen(arcfile) + strlen(filecontent) > strlen(arcfile)){
                arcfile = (char*) MXPSQL_MShar_Realloc(arcfile, strlen(arcfile) + strlen(filecontent) + 1);
            }

            {
                char* fileblock = (char*) MXPSQL_MShar_Calloc(initarcfilesize, initarcfilesize);
                if(fileblock == NULL){
                    MXPSQL_MShar_Free(arcfile);
                    MXPSQL_MShar_Free(filecontent);
                    setlocale(LC_ALL, old_locale);
                    return NULL;
                }

                {
                    char* tektfmt = NULL;
                    char* tektsrc = NULL;

                    {
                        static const char* tektfmt_part1=(char*)"TEKTONE='";
                        static const char* tektfmt_partmid=(char*)"%s";
                        static const char* tektfmt_part2=(char*)"'\n";

                        size_t tektfmt_size = ((strlen(tektfmt_part1) + strlen(files[i]) + strlen(tektfmt_part2)) * sizeof(char*));

                        tektfmt = (char*) MXPSQL_MShar_Calloc(tektfmt_size, tektfmt_size);
                        tektsrc = (char* ) MXPSQL_MShar_Calloc(tektfmt_size, tektfmt_size);

                        if(tektfmt == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }

                        strcat(tektfmt, tektfmt_part1);
                        strcat(tektfmt, tektfmt_partmid);
                        strcat(tektfmt, tektfmt_part2);
                    }

                    /* tektsrc = (char*) MXPSQL_MShar_Calloc(initarcfilesize, initarcfilesize); */
                    if(tektsrc == NULL){
                        MXPSQL_MShar_Free(arcfile);
                        MXPSQL_MShar_Free(filecontent);
                        MXPSQL_MShar_Free(fileblock);
                        setlocale(LC_ALL, old_locale);
                        return NULL;
                    }

                    if(strlen(files[i]) + strlen(tektsrc) > strlen(fileblock)){
                        tektsrc = (char*) MXPSQL_MShar_Realloc(tektsrc, strlen(files[i]) + strlen(tektsrc) + 1);
                        if(tektsrc == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }
                    }

                    if(sprintf(tektsrc, tektfmt, files[i]) < 0){
                        MXPSQL_MShar_Free(arcfile);
                        MXPSQL_MShar_Free(filecontent);
                        MXPSQL_MShar_Free(fileblock);
                        MXPSQL_MShar_Free(tektsrc);
                        setlocale(LC_ALL, old_locale);
                        return NULL;
                    }
                    strcat(fileblock, tektsrc);

                    MXPSQL_MShar_Free(tektsrc);
                    MXPSQL_MShar_Free(tektfmt);
                }

                {
                    static char* dirnam = (char*) "\
DIRNAME=\"./$(dirname \"$TEKTONE\")\"\n\
mkdir \"$DIRNAME\" 2> /dev/null;\
\n";
                    if(strlen(dirnam) + strlen(fileblock) > strlen(fileblock)){
                        fileblock = (char*) MXPSQL_MShar_Realloc(fileblock, strlen(dirnam) + strlen(fileblock) + 1);
                        if(fileblock == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }
                    }

                    strcat(fileblock, dirnam);
                }

                {
                    static char* info = (char*) "printf \"x - %s\\n\" \"$TEKTONE\";\n";
                    if(strlen(info) + strlen(fileblock) > strlen(fileblock)){
                        fileblock = (char*) MXPSQL_MShar_Realloc(fileblock, strlen(info) + strlen(fileblock) + 1);
                        if(fileblock == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }
                    }

                    strcat(fileblock, info);
                }

                {
                    static char* marker = (char*) "#@EE\n";
                    if(strlen(marker) + strlen(fileblock) > strlen(fileblock)){
                        fileblock = (char*) MXPSQL_MShar_Realloc(fileblock, strlen(marker) + strlen(fileblock) + 1);
                        if(fileblock == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }
                    }

                    strcat(fileblock, marker);
                }

                {
                    size_t s = initarcfilesize;

                    char* basprintf = (char*) MXPSQL_MShar_Calloc(s, s);
                    char* fmt = NULL;
                    char* bas64 = NULL;
                    if(basprintf == NULL){
                        MXPSQL_MShar_Free(arcfile);
                        MXPSQL_MShar_Free(filecontent);
                        MXPSQL_MShar_Free(fileblock);
                        setlocale(LC_ALL, old_locale);
                        return NULL;
                    }

                    bas64 = mkmshar_b64Encode(filecontent, fsize);

                    if(bas64 == NULL){
                        bas64 = (char*) MXPSQL_MShar_Malloc(sizeof(char));
                        if(bas64 == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            MXPSQL_MShar_Free(basprintf);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }
                        strcat(bas64, "");
                    }

                    {
                        static const char* fmt1 = (char*) "printf '";
                        static const char* fmtmid = (char*) "%%s";
                        static const char* fmt2 = (char*) "' '%s' > \"./$TEKTONE\";\n";
                        size_t s1 = (sizeof(char*) * strlen(fmt1));

                        char* ss = (char*) MXPSQL_MShar_Calloc(s1, s1);
                        if(ss == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            MXPSQL_MShar_Free(basprintf);
                            MXPSQL_MShar_Free(bas64);
                            if(ss != NULL) MXPSQL_MShar_Free(ss);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }

                        strcat(ss, fmt1);

                        if(strlen(bas64) + s1 > s1){
                            s1 = strlen(bas64) + s1 + 1;
                            ss = (char*) MXPSQL_MShar_Realloc(ss, s1);
                            if(ss == NULL){
                                MXPSQL_MShar_Free(arcfile);
                                MXPSQL_MShar_Free(filecontent);
                                MXPSQL_MShar_Free(fileblock);
                                MXPSQL_MShar_Free(basprintf);
                                MXPSQL_MShar_Free(bas64);
                                if(ss != NULL) MXPSQL_MShar_Free(ss);
                                setlocale(LC_ALL, old_locale);
                                return NULL;
                            }
                        }

                        strcat(ss, bas64);

                        if(strlen(fmt2) + s1 > s1){
                            s1 = strlen(fmt2) + s1 + 1;
                            ss = (char*) MXPSQL_MShar_Realloc(ss, s1);
                            if(ss == NULL){
                                MXPSQL_MShar_Free(arcfile);
                                MXPSQL_MShar_Free(filecontent);
                                MXPSQL_MShar_Free(fileblock);
                                MXPSQL_MShar_Free(basprintf);
                                MXPSQL_MShar_Free(bas64);
                                if(ss != NULL) MXPSQL_MShar_Free(ss);
                                setlocale(LC_ALL, old_locale);
                                return NULL;
                            }
                        }

                        strcat(ss, fmt2);

                        MXPSQL_MShar_Free(ss);
                        ss = NULL;

                        if(s1 > s){
                            s = s1;
                            basprintf = (char*) MXPSQL_MShar_Realloc(basprintf, s);
                            if(basprintf == NULL){
                                MXPSQL_MShar_Free(arcfile);
                                MXPSQL_MShar_Free(filecontent);
                                MXPSQL_MShar_Free(fileblock);
                                MXPSQL_MShar_Free(bas64);
                                MXPSQL_MShar_Free(fmt);
                                if(basprintf != NULL) MXPSQL_MShar_Free(basprintf);
                                setlocale(LC_ALL, old_locale);
                                return NULL;
                            }
                        }

                        {
                            size_t fmtlen = strlen(fmt1) + strlen(fmtmid) + strlen(fmt2) + 1;
                            fmt = (char*) MXPSQL_MShar_Calloc(fmtlen, fmtlen);

                            if(fmt == NULL){
                                MXPSQL_MShar_Free(arcfile);
                                MXPSQL_MShar_Free(filecontent);
                                MXPSQL_MShar_Free(fileblock);
                                MXPSQL_MShar_Free(basprintf);
                                MXPSQL_MShar_Free(bas64);
                                if(fmt != NULL) MXPSQL_MShar_Free(fmt);
                                setlocale(LC_ALL, old_locale);
                                return NULL;
                            }

                            strcat(fmt, fmt1);
                            strcat(fmt, fmtmid);
                            strcat(fmt, fmt2);
                        }
                    }

                    /* {
                        size_t sizy;
                        size_t sizy2;
                        static const char* fmt1 = (char*) "printf '";
                        static const char* fmt2 = (char*) "' > \"./$TEKTONE\";\n";

                        bas64 = mkmshar_b64Encode(filecontent, fsize);

                        /\* printf("t: %s", bas64); *\/

                        if(bas64 == NULL){
                            bas64 = (char*) MXPSQL_MShar_Malloc(sizeof(char));
                            if(bas64 == NULL){
                                MXPSQL_MShar_Free(arcfile);
                                MXPSQL_MShar_Free(filecontent);
                                MXPSQL_MShar_Free(fileblock);
                                MXPSQL_MShar_Free(basprintf);
                                setlocale(LC_ALL, old_locale);
                                return NULL;
                            }
                            strcat(bas64, "");
                        }

                        sizy = ((strlen(fmt1) + sizeof(bas64) + sizeof(fmt2)) * sizeof(char*));
                        sizy2 = ((strlen(fmt1) + strlen("%s") + strlen(fmt2)) * sizeof(char*));

                        basprintf = (char*) MXPSQL_MShar_Calloc(sizy, sizy);
                        fmt = (char*) MXPSQL_MShar_Calloc(sizy2, sizy2);

                        if(basprintf == NULL || fmt == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            if(basprintf != NULL)MXPSQL_MShar_Free(basprintf);
                            if(fmt != NULL)MXPSQL_MShar_Free(fmt);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }

                        strcat(fmt, fmt1);
                        strcat(fmt, "%s");
                        strcat(fmt, fmt2);
                    } */

                    if(mkmshar_snprintf(basprintf, s, fmt, bas64) < 0){
                        MXPSQL_MShar_Free(arcfile);
                        MXPSQL_MShar_Free(filecontent);
                        MXPSQL_MShar_Free(fileblock);
                        MXPSQL_MShar_Free(basprintf);
                        MXPSQL_MShar_Free(bas64);
                        MXPSQL_MShar_Free(fmt);
                        setlocale(LC_ALL, old_locale);
                        return NULL;
                    }

                    if(strlen(basprintf) + strlen(fileblock) > strlen(fileblock)){
                        fileblock = (char*) MXPSQL_MShar_Realloc(fileblock, strlen(basprintf) + strlen(fileblock) + 1);
                        if(fileblock == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            MXPSQL_MShar_Free(basprintf);
                            MXPSQL_MShar_Free(bas64);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }
                    }

                    strcat(fileblock, basprintf);

                    MXPSQL_MShar_Free(bas64);
                    MXPSQL_MShar_Free(basprintf);
                    MXPSQL_MShar_Free(fmt);
                }

                {
                    static char* debas64tmp = (char*)
"tmp=$(mktemp);\n\
\"$TTk\" -d \"./$TEKTONE\" > \"$tmp\";\n\
mv \"$tmp\" \"./$TEKTONE\";\n\
tmp=;\
\n\n";
                    if(strlen(debas64tmp) + strlen(fileblock) > strlen(fileblock)){
                        fileblock = (char*) MXPSQL_MShar_Realloc(fileblock, strlen(debas64tmp) + strlen(fileblock) + 1);
                        if(fileblock == NULL){
                            MXPSQL_MShar_Free(arcfile);
                            MXPSQL_MShar_Free(filecontent);
                            MXPSQL_MShar_Free(fileblock);
                            setlocale(LC_ALL, old_locale);
                            return NULL;
                        }
                    }
                    strcat(fileblock, debas64tmp);
                }

                if(strlen(fileblock) + strlen(arcfile) > strlen(arcfile)){
                    arcfile = (char*) MXPSQL_MShar_Realloc(arcfile, strlen(arcfile) + strlen(fileblock) + 1);
                    if(fileblock == NULL){
                        MXPSQL_MShar_Free(arcfile);
                        MXPSQL_MShar_Free(filecontent);
                        MXPSQL_MShar_Free(fileblock);
                        setlocale(LC_ALL, old_locale);
                        return NULL;
                    }
                }
                strcat(arcfile, fileblock);
            }
        }

        MXPSQL_MShar_Free(filecontent);
    }

    if(postscript != NULL){
        if(strlen(arcfile) + strlen(postscript) > strlen(arcfile)){
            arcfile = (char*) MXPSQL_MShar_Realloc(arcfile, strlen(arcfile) + strlen(postscript) + 1);
            if(arcfile == NULL){
                if(arcfile != NULL) MXPSQL_MShar_Free(arcfile);
                setlocale(LC_ALL, old_locale);
                return NULL;
            }
        }
        strcat(arcfile, postscript);
    }

    {
        static char* poststr = (char*)
"\n\
# This is a shell archive lol, created with mshar (MXPSQL's version of the Shell archiver)\n\
POSIXLY_CORRECT=; # Unposix it as we Done\n\
POSIX_ME_HARDER=; # Unposix it as we Done\n\
exit 0;\
\n";

        if(strlen(poststr) + strlen(arcfile) > strlen(arcfile)){
            arcfile = (char*) MXPSQL_MShar_Realloc(arcfile, strlen(arcfile) + strlen(poststr) + 1);
            if(arcfile == NULL){
                if(arcfile != NULL) MXPSQL_MShar_Free(arcfile);
                setlocale(LC_ALL, old_locale);
                return NULL;
            }
        }

        strcat(arcfile, poststr);
    }

    setlocale(LC_ALL, old_locale);
    return (char*) arcfile;
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
