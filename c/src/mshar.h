/**
 * @file mshar.h
 * @author MXPSQL
 * @brief MShar library in a header.
 * Header only (there is an exception) library for creating MShar Archives, a self extracting archive format based of shar.
 * @version 0 (unscheduled release)
 * @date 2022-05-12
 * 
 * @details
 * Some are taken from b64 and the apple snprintf. Copyright notice of the snprintf.
 * 
 * Copyright (c) 2013, NLnet Labs. All rights reserved.
 *
 * This software is open source.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * Neither the name of the NLNET LABS nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * This Library also may consume a lot of memory and may cause a memory leak (sometimes will fail trying to archive large file).
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
#include <cerrno>
#else
#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>
#include <errno.h>
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
/* #error "MShar requires an ANSI C compiler" */
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
 * @brief add padding to string 
 * @details
 * Derived from https://opensource.apple.com/source/network_cmds/network_cmds-511/unbound/compat/snprintf.c
 * 
 * Copyright notice is at top
*/
static void print_pad(char** at, size_t* left, int* ret, char p, int num);

/**
 * @brief print %s
 * 
 * @details
 * Derived from https://opensource.apple.com/source/network_cmds/network_cmds-511/unbound/compat/snprintf.c
 * 
 * Copyright notice is at top 
*/
static void print_str(char** at, size_t* left, int* ret, char* s, int minw, int precision, int prgiven, int minus);

/** 
 * @brief print %c 
 * @details
 * Derived from https://opensource.apple.com/source/network_cmds/network_cmds-511/unbound/compat/snprintf.c
 * 
 * Copyright notice is at top
 */
static void print_char(char** at, size_t* left, int* ret, int c, int minw, int minus);

/**
 * @brief A dumb (and potentially very dummy and gummy) version of vsnprintf that backs mkmshar_snprintf.  
 * It's quite dumb.
 * 
 * Do not use this function for portability reasons (that is that it does not implement everything like '%p').
 * If if you need more elaborate format flags, go use a C standards compliant version.
 * This is not a standards compliant version, I just need a dumb one for C90.
 * Just do not use this even if you don't need elaborate flags like '%p' or '%z', the list of supported flags are listed below.
 * 
 * @param str string buffer for result. result will be null terminated.
 * @param size size of the buffer. null is put inside buffer.
 * @param format printf format string.
 * @param arg '...' arguments to print.
 * @returns number of characters. a null is printed after this.
 * @return number of bytes that would have been written
 *	   if the buffer had been large enough.
 * 
 * @note You need to start and end the va_list manually.
 * 
 * @warning Not portable, I told you.
 * 
 * @details
 * Originally derived from https://codereview.stackexchange.com/questions/132860/my-own-snprintf-implementation-in-c
 * 
 * Now derived from https://opensource.apple.com/source/network_cmds/network_cmds-511/unbound/compat/snprintf.c
 * 
 * Copyright notice is at top
 * 
 * supported format specifiers:
 * 	%s, %c, %n, %m.
 * 	and %%.
 * 
 * Yeah, a small subset of the standard format specifiers, how cool and portable is that. Not cool and portable.
 * 
 * @param buf the buffer to be filled
 * @param size the size of the buffer
 * @param fmt the format string
 * @param ap the variadic arguments list
 * @return int the number of characters written
 */
int mkmshar_dumbvsnprintf(char* str, size_t size, const char* format, va_list arg);

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
 * 
 * Also you can't ignore memory allocation errors, those realloc spams are needed.
 * 
 * @warning This function's return value must be manually freed if not null.
 * 
 * Sometimes this function can fail if you try to archive big files (like the executable itself).
 * 
 * @param prescript the script to run before extraction. Put NULL if empty and do not put the filename, put the content of the script you want to run (putting the filename will just place the filename, not the content)
 * @param postscript the script to run after extraction. Put NULL if empty and do not put the filename, put the content of the script you want to run (putting the filename will just place the filename, not the content)
 * @param files the files to be archived, retuning null will set the errno to EDOM and return NULL
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

static void
print_pad(char** at, size_t* left, int* ret, char p, int num)
{
	while(num--) {
		if(*left > 1) {
			*(*at)++ = p;
			(*left)--;
		}
		(*ret)++;
	}
}

static void
spool_str(char** at, size_t* left, int* ret, const char* buf, int len)
{
	int i;
	for(i=0; i<len; i++) {
		if(*left > 1) {
			*(*at)++ = buf[i];
			(*left)--;
		}
		(*ret)++;
	}
}

static void
print_str(char** at, size_t* left, int* ret, char* s,
	int minw, int precision, int prgiven, int minus)
{
	int w;
	/* with prec: no more than x characters from this string, stop at 0 */
	if(prgiven)
		w = mkmshar_strnlen(s, precision);
	else	w = (int)strlen(s); /* up to the nul */
	if(w < minw && !minus)
		print_pad(at, left, ret, ' ', minw - w);
	spool_str(at, left, ret, s, w);
	if(w < minw && minus)
		print_pad(at, left, ret, ' ', minw - w);
}

static void
print_char(char** at, size_t* left, int* ret, int c,
	int minw, int minus)
{
	if(1 < minw && !minus)
		print_pad(at, left, ret, ' ', minw - 1);
	print_pad(at, left, ret, c, 1);
	if(1 < minw && minus)
		print_pad(at, left, ret, ' ', minw - 1);
}

int mkmshar_dumbvsnprintf(char* str, size_t size, const char* format, va_list arg)
{
	char* at = str;
	size_t left = size;
	int ret = 0;
	const char* fmt = format;
	int conv, minw, precision, prgiven, minus;
	while(*fmt) {
		/* copy string before % */
		while(*fmt && *fmt!='%') {
			if(left > 1) {
				*at++ = *fmt++;
				left--;
			} else fmt++;
			ret++;
		}
		
		/* see if we are at end */
		if(!*fmt) break;

		/* fetch next argument % designation from format string */
		fmt++; /* skip the '%' */

		/********************************/
		/* get the argument designation */
		/********************************/
		/* we must do this vararg stuff inside this function for
		 * portability.  Hence, get_designation, and print_designation
		 * are not their own functions. */

		/* printout designation:
		 * conversion specifier: x, d, u, s, c, n, m, p
		 * flags: # not supported
		 *        0 zeropad (on the left)
		 *	  - left adjust (right by default)
		 *	  ' ' printspace for positive number (in - position).
		 *	  + alwayssign
		 * fieldwidth: [1-9][0-9]* minimum field width.
		 * 	if this is * then type int next argument specifies the minwidth.
		 * 	if this is negative, the - flag is set (with positive width).
		 * precision: period[digits]*, %.2x.
		 * 	if this is * then type int next argument specifies the precision.
		 *	just '.' or negative value means precision=0.
		 *		this is mindigits to print for d, i, u, x
		 *		this is aftercomma digits for f
		 *		this is max number significant digits for g
		 *		maxnumber characters to be printed for s
		 * length: 0-none (int), 1-l (long), 2-ll (long long)
		 * 	notsupported: hh (char), h (short), L (long double), q, j, z, t
		 * Does not support %m$ and *m$ argument designation as array indices.
		 * Does not support %#x
		 *
		 */
		minw = 0;
		precision = 1;
		prgiven = 0;
		minus = 0;

		/* field width */
		if(*fmt == '*') {
			fmt++; /* skip char */
			minw = va_arg(arg, int);
			if(minw < 0) {
				minus = 1;
				minw = -minw;
			}
		} else while(*fmt >= '0' && *fmt <= '9') {
			minw = minw*10 + (*fmt++)-'0';
		}

		/* precision */
		if(*fmt == '.') {
			fmt++; /* skip period */
			prgiven = 1;
			precision = 0;
			if(*fmt == '*') {
				fmt++; /* skip char */
				precision = va_arg(arg, int);
				if(precision < 0)
					precision = 0;
			} else while(*fmt >= '0' && *fmt <= '9') {
				precision = precision*10 + (*fmt++)-'0';
			}
		}

		/* get the conversion */
		if(!*fmt) conv = 0;
		else	conv = *fmt++;

		/***********************************/
		/* print that argument designation */
		/***********************************/
		switch(conv) {
		case 's':
			print_str(&at, &left, &ret, va_arg(arg, char*),
				minw, precision, prgiven, minus);
			break;
		case 'c':
			print_char(&at, &left, &ret, va_arg(arg, int),
				minw, minus);
			break;
		case 'n':
			*va_arg(arg, int*) = ret;
			break;
		case 'm':
			print_str(&at, &left, &ret, strerror(errno),
				minw, precision, prgiven, minus);
			break;
		case '%':
			print_pad(&at, &left, &ret, '%', 1);
			break;
		/* unknown */
		default:
		case 0: break;
		}
	}

	/* zero terminate */
	if(left > 0)
		*at = 0;
	return ret;
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
        errno = EDOM;
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

            while(fgetc(fptr) != EOF || ferror(fptr)){;} /* this may seem hacky, convoluted and unsophisticated, but it is portable and sophisticated due to C++ not mandating to implement SEEK_END just like the comment before (no longer existing). 
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

                    if(tektsrc != NULL) MXPSQL_MShar_Free(tektsrc);
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
                    size_t s = 0;

                    /* char* basprintf = (char*) MXPSQL_MShar_Calloc(s, s); */
                    char* basprintf = NULL;
                    char* fmt = NULL;
                    char* bas64 = NULL;

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

                            size_t fmtlen = strlen(fmt1) + strlen(fmtmid) + strlen(fmt2) + 1;
                            fmt = (char*) MXPSQL_MShar_Calloc(fmtlen, fmtlen);

                            if(fmt == NULL){
                                MXPSQL_MShar_Free(arcfile);
                                MXPSQL_MShar_Free(filecontent);
                                MXPSQL_MShar_Free(fileblock);
                                if(basprintf != NULL) MXPSQL_MShar_Free(basprintf);
                                MXPSQL_MShar_Free(bas64);
                                if(fmt != NULL) MXPSQL_MShar_Free(fmt);
                                setlocale(LC_ALL, old_locale);
                                return NULL;
                            }

                            strcat(fmt, fmt1);
                            strcat(fmt, fmtmid);
                            strcat(fmt, fmt2);
                    }

                    s = mkmshar_snprintf(NULL, 0, fmt, bas64);
                    basprintf = (char*) MXPSQL_MShar_Calloc(s, s);
                    if(basprintf == NULL){
                        MXPSQL_MShar_Free(arcfile);
                        MXPSQL_MShar_Free(fmt);
                        MXPSQL_MShar_Free(filecontent);
                        MXPSQL_MShar_Free(fileblock);
                        setlocale(LC_ALL, old_locale);
                        return NULL;
                    }

                    /* {
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
                    } */

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
