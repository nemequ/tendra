/*
 * Copyright (c) 2002-2005 The TenDRA Project <http://www.tendra.org/>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of The TenDRA Project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific, prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */
/*
					 Crown Copyright (c) 1997

	This TenDRA(r) Computer Program is subject to Copyright
	owned by the United Kingdom Secretary of State for Defence
	acting through the Defence Evaluation and Research Agency
	(DERA).  It is made available to Recipients with a
	royalty-free licence for its use, reproduction, transfer
	to other parties and amendment for any purpose not excluding
	product development provided that any such use et cetera
	shall be deemed to be acceptance of the following conditions:-

		(1) Its Recipients shall ensure that this Notice is
		reproduced upon any copies or amended versions of it;

		(2) Any amended version of it shall be clearly marked to
		show both the nature of and the organisation responsible
		for the relevant amendment or amendments;

		(3) Its onward transfer from a recipient to another
		party shall be deemed to be that party's acceptance of
		these conditions;

		(4) DERA gives no warranty or assurance as to its
		quality or suitability for any purpose and DERA accepts
		no liability whatsoever in relation to any use to which
		it may be put.
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "output.h"

#include <shared/error.h>

void
output_indent(FILE *out, unsigned int depth)
{
	while(depth --> 0)
		fputc('\t', out);
}


int
output_comment_file(enum output_comment com, FILE* output, FILE* input)
{
    char buf[BUFSIZ];
	char *start;
	char *middle;
	char *end;

	switch(com) {
	case OUTPUT_COMMENT_C90:
	case OUTPUT_COMMENT_C99:
		start  = "/*";
		middle = " *";
		end    = " */";
		break;
	}

    fprintf(output, "%s \n%s ", start, middle);
    while(1) {
        buf[BUFSIZ - 1] = 'x';

        if(!fgets(buf, sizeof buf, input)) {
            break;
        }

        if(strstr(buf, end)) {
            return 0;
        }

        fputs(buf, output);

        if(!(buf[BUFSIZ - 1] == '\0' && buf[BUFSIZ - 2] != '\n')) {
            fprintf(output, "%s ", middle);
        }
    }
    fprintf(output, "\n%s\n\n", end);

    return 1;
}

void
output_generated_by_lexi(enum output_comment com, FILE* out)
{
	assert(com == OUTPUT_COMMENT_C90 || com == OUTPUT_COMMENT_C99);

	fprintf(out, "\n/*"
	             "\n * Automatically generated by %s version %s"
	             "\n */\n\n", progname, progvers);
}

