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


#ifndef USTRING_INCLUDED
#define USTRING_INCLUDED


/*
 *    UNSIGNED STRING HANDLING
 *
 *    These macros are used to convert the normal string operations to the
 *    corresponding unsigned string operations.  The portability assumption,
 *    that char * and unsigned char * are interchangeable in these operations
 *    is encapsulated in the tokens U2S and S2U.
 */

#if FS_TENDRA
#pragma TenDRA begin
#pragma TenDRA conversion analysis (pointer-pointer) off
#pragma token PROC (EXP : unsigned char * :) EXP : char * : U2S #
#pragma token PROC (EXP : char * :) EXP : unsigned char * : S2U #
#define U2S(S)		((char *) (S))
#define S2U(S)		((unsigned char *) (S))
#pragma TenDRA end
#else
#define U2S(S)		((char *) (S))
#define S2U(S)		((unsigned char *) (S))
#endif

#define ustrlit(S)		(S2U (S))
#define strlit(S)		(U2S (S))

#define ustrlen(S)		(strlen (U2S (S)))
#define ustrcpy(S, T)		(S2U (strcpy (U2S (S), U2S (T))))
#define ustrcpy_v(S, T)	(strcpy_v (U2S (S), U2S (T)))
#define ustrcmp(S, T)		(strcmp (U2S (S), U2S (T)))
#define ustreq(S, T)		(strcmp (U2S (S), U2S (T)) == 0)
#define ustrchr(S, C)		(S2U (strchr (U2S (S), (C))))
#define ustrrchr(S, C)	(S2U (strrchr (U2S (S), (C))))

#define xustr(N)		(S2U (xstr (N)))
#define xustrcpy(S)		(S2U (xstrcpy (U2S (S))))
#define xustrcat(S, T)	(S2U (xstrcat (U2S (S), U2S (T))))


#endif
