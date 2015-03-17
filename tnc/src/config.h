/* $Id$ */

/*
 * Copyright 2002-2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */


#ifndef CONFIG_H
#define CONFIG_H


/*
    INCLUDE THE BASIC SYSTEM HEADERS

    These headers are required in several places.  It is easiest to
    include them here.  These are the only system headers included.
    The program is ANSI compliant.  It uses the following objects :

	(a)  Types : FILE, size_t, va_list.

	(b)  Procedures or macros : exit, fclose, fgetc, fopen,
	     fprintf, fputc, fputs, fseek, ftell, getc, isprint,
	     malloc, printf, putchar, realloc, sprintf, strcmp,
	     strlen, strncmp, strncpy, va_end, va_start, vfprintf.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
    MACRO TO DEAL WITH PROCEDURE PROTOTYPES

    Procedure declarations are given by the macro procedure with is
    defined to either give the full prototype or the tradition declaration
    depending on the value of __STDC__.
*/


#endif /* CONFIG_H */
