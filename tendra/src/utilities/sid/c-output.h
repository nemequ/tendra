/*
 * Copyright (c) 2002, The Tendra Project <http://www.ten15.org/>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *    		 Crown Copyright (c) 1997
 *    
 *    This TenDRA(r) Computer Program is subject to Copyright
 *    owned by the United Kingdom Secretary of State for Defence
 *    acting through the Defence Evaluation and Research Agency
 *    (DERA).  It is made available to Recipients with a
 *    royalty-free licence for its use, reproduction, transfer
 *    to other parties and amendment for any purpose not excluding
 *    product development provided that any such use et cetera
 *    shall be deemed to be acceptance of the following conditions:-
 *    
 *        (1) Its Recipients shall ensure that this Notice is
 *        reproduced upon any copies or amended versions of it;
 *    
 *        (2) Any amended version of it shall be clearly marked to
 *        show both the nature of and the organisation responsible
 *        for the relevant amendment or amendments;
 *    
 *        (3) Its onward transfer from a recipient to another
 *        party shall be deemed to be that party's acceptance of
 *        these conditions;
 *    
 *        (4) DERA gives no warranty or assurance as to its
 *        quality or suitability for any purpose and DERA accepts
 *        no liability whatsoever in relation to any use to which
 *        it may be put.
 *
 * $TenDRA$
*/


/*** c-output.h --- Output routines.
 *
 ** Author: Steve Folkes <smf@hermes.mod.uk>
 *
 *** Commentary:
 *
 * See the file "c-output.c" for more information.
 *
 *** Change Log:*/

/****************************************************************************/

#ifndef H_C_OUTPUT
#define H_C_OUTPUT

#include "os-interface.h"
#include "cstring.h"
#include "entry-list.h"
#include "grammar.h"
#include "key.h"
#include "ostream.h"
#include "c-out-info.h"

/*--------------------------------------------------------------------------*/

#define C_INDENT_STEP ((unsigned) 4)
#define C_INDENT_FOR_ERROR C_INDENT_STEP
#define C_INDENT_FOR_PARAM ((unsigned) 2)
#define C_INDENT_FOR_CASE ((unsigned) 2)
#define C_INDENT_FOR_LABEL ((unsigned) 2)

/*--------------------------------------------------------------------------*/

extern void			c_output_parser
	PROTO_S ((COutputInfoP, GrammarP));
extern void			c_output_header
	PROTO_S ((COutputInfoP, GrammarP));
extern void			c_output_location
	PROTO_S ((COutputInfoP, CStringP, unsigned));
extern void			c_output_key_message
	PROTO_S ((COutputInfoP, CStringP, KeyP, CStringP, unsigned));
extern unsigned			c_out_next_label
	PROTO_S ((void));
extern void			c_output_open
	PROTO_S ((COutputInfoP, unsigned));
extern void			c_output_close
	PROTO_S ((COutputInfoP, unsigned));

#endif /* !defined (H_C_OUTPUT) */

/*
 * Local variables(smf):
 * eval: (include::add-path-entry "../os-interface" "../library")
 * eval: (include::add-path-entry "../transforms" "../output" "../generated")
 * end:
**/
