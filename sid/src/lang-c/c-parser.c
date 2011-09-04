/*
 * Automatically generated from the files:
 *	c-parser.sid
 * and
 *	c-parser.act
 * by:
 *	sid
 */

/* BEGINNING OF HEADER */

#line 121 "c-parser.act"


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

	#include <assert.h>

	#include <shared/check.h>
	#include <shared/error.h>

	#include "c-parser.h"
	#include "../adt/action.h"
	#include "c-code.h"
	#include "c-out-info.h"
	#include "../adt/basic.h"
	#include "../adt/entry.h"
	#include "../adt/type.h"
	#include "../adt/types.h"

	#define ERROR_TERMINAL (C_TOK_ERROR)
	#define CURRENT_TERMINAL c_lexer_get_terminal (c_current_stream)
	#define ADVANCE_LEXER c_lexer_next_token (c_current_stream)
	#define SAVE_LEXER(x) (c_lexer_save_terminal (c_current_stream, (CTokenT) (x)))
	#define RESTORE_LEXER (c_lexer_restore_terminal (c_current_stream))

	/* typedefs for the maps section */
	typedef CCodeT *CCodeP;

	static NStringT    c_prefix_names [CPFX_NUM_PREFIXES];
	static BoolT       c_inited_prefix_names = FALSE;
	static CPrefixT    c_current_prefix;
	static EntryT     *c_current_entry;
	static TypeTupleT  c_saved_type;
	static TypeTupleT  c_current_type;
	static BoolT       c_propagating_error = FALSE;

	CLexerStreamT *c_current_stream;
	COutputInfoT  *c_current_out_info;
	TableT        *c_current_table;


	static void err_expected(const char *item) {
		error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
			"expected %s", item);
	}

	static void err_unknown(const char *item, NStringT *name) {
		error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
			"unknown %s '%S'", item, (void *) name);
	}

	static void err_mult(const char *verb, const char *item, NStringT *name) {
		error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
			"multiple %s operations defined for %s '%S'", verb, item, (void *) name);
	}

	static void err_formal_clash(const char *verb, const char *item, KeyT *key, TypeTupleT *a, TypeTupleT *b) {
		error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
			"%s operation formal clash for %s '%K' [parameter = %Y, result = %Y]",
			verb, item, (void *) key, (void *) a, (void *) b);

	}

	static void err_clash(const char *verb, const char *field, const char *item, KeyT *key, TypeTupleT *a) {
		error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
			"%s operation %s clash (or missing name) for %s '%K' [%s = %Y]",
			verb, field, item, (void *) key, field, (void *) a);
	}

	static void err_field_clash(const char *field, const char *item, KeyT *key, TypeTupleT *a) {
		error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
			"%s clash (or missing name) for %s '%K' [%s = %Y]",
			field, item, (void *) key, field, (void *) a);
	}

	static void err_mismatch(const char *field, const char *verb, const char *item, KeyT *key, TypeTupleT *a, TypeTupleT *b) {
		error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
			"%s type mismatch for %s operation on %s '%K' [%Y should be %Y]",
			field, verb, item, (void *) key, (void *) a, (void *) b);
	}

	static void err_field_mismatch(const char *field, KeyT *key, TypeTupleT *a, TypeTupleT *b) {
		error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
			"%s type mismatch for action '%K' [%Y should be %Y]",
			field, (void *) key, (void *) a, (void *) b);
	}

#line 156 "tmp.c-parser.c"


#ifndef ERROR_TERMINAL
#error "-s no-numeric-terminals given and ERROR_TERMINAL is not defined"
#endif

/* BEGINNING OF FUNCTION DECLARATIONS */

static void ZR137(void);
static void ZR177(void);
static void ZR118(CCodeP);
static void ZR184(void);
static void ZR112(CCodeP *);
static void ZR161(void);
static void ZR130(void);
static void ZR164(void);
static void ZR193(void);
static void ZR152(void);
static void ZR191(void);
static void ZR172(void);
static void ZR120(void);
static void ZR114(CCodeP);
static void ZR122(void);
extern void c_parse_grammar(void);
static void ZR155(void);
static void ZR157(void);
static void ZR139(void);
static void ZR179(void);
static void ZR218(void);
static void ZR147(void);
static void ZR221(void);
static void ZR186(void);
static void ZR128(void);
static void ZR145(void);
static void ZR162(void);
static void ZR170(void);
static void ZR124(void);

/* BEGINNING OF STATIC VARIABLES */

static BoolT ZI0;

/* BEGINNING OF FUNCTION DEFINITIONS */

static void
ZR137(void)
{
  ZL2_137:;
    switch (CURRENT_TERMINAL) {
      case (C_TOK_C_HIDENTIFIER):
	{
	    ZR139 ();
	    /* BEGINNING OF INLINE: c-parse-grammar::persistent-list */
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    } else {
		goto ZL2_137;
	    }
	    /* END OF INLINE: c-parse-grammar::persistent-list */
	}
	/*UNREACHED*/
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR177(void)
{
  ZL2_177:;
    switch (CURRENT_TERMINAL) {
      case (C_TOK_SID_HIDENTIFIER): case (C_TOK_C_HIDENTIFIER):
	{
	    ZR179 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    /* BEGINNING OF ACTION: skip-recover */
	    {
#line 1177 "c-parser.act"

		c_propagating_error = FALSE;
	
#line 248 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: skip-recover */
	    /* BEGINNING OF INLINE: c-parse-grammar::result-assign-list */
	    goto ZL2_177;
	    /* END OF INLINE: c-parse-grammar::result-assign-list */
	}
	/*UNREACHED*/
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR118(CCodeP ZI111)
{
  ZL2_118:;
    switch (CURRENT_TERMINAL) {
      case (C_TOK_ACT_HAT): case (C_TOK_ACT_HEXCEPTION): case (C_TOK_ACT_HTERMINAL): case (C_TOK_ACT_HADVANCE):
      case (C_TOK_ACT_HBASIC): case (C_TOK_ACT_HLABEL): case (C_TOK_ACT_HREFERENCE): case (C_TOK_ACT_HIDENTIFIER):
      case (C_TOK_ACT_HMODIFIABLE): case (C_TOK_ACT_HCODESTRING): case (C_TOK_ACT_HEOF):
	{
	    ZR114 (ZI111);
	    /* BEGINNING OF INLINE: c-parse-grammar::code-block::code-body */
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    } else {
		goto ZL2_118;
	    }
	    /* END OF INLINE: c-parse-grammar::code-block::code-body */
	}
	/*UNREACHED*/
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR184(void)
{
  ZL2_184:;
    switch (CURRENT_TERMINAL) {
      case (C_TOK_SID_HIDENTIFIER): case (C_TOK_C_HIDENTIFIER):
	{
	    ZR186 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    /* BEGINNING OF ACTION: skip-recover */
	    {
#line 1177 "c-parser.act"

		c_propagating_error = FALSE;
	
#line 316 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: skip-recover */
	    /* BEGINNING OF INLINE: c-parse-grammar::terminal-list */
	    goto ZL2_184;
	    /* END OF INLINE: c-parse-grammar::terminal-list */
	}
	/*UNREACHED*/
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR112(CCodeP *ZO111)
{
    CCodeP ZI111;

    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case (C_TOK_ACT_HCODESTART):
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: ccode-init */
	{
#line 807 "c-parser.act"

		(ZI111) = c_code_create(istream_name(&c_lexer_stream->istream), 
			istream_line(&c_lexer_stream->istream));
	
#line 358 "tmp.c-parser.c"
	}
	/* END OF ACTION: ccode-init */
	ZR118 (ZI111);
	switch (CURRENT_TERMINAL) {
	  case (C_TOK_ACT_HCODEEND):
	    break;
	  case (ERROR_TERMINAL):
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
  ZL0:;
    *ZO111 = ZI111;
}

static void
ZR161(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case (C_TOK_ARROW):
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    {
	/* BEGINNING OF ACTION: expected-arrow */
	{
#line 902 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("'->'");
		}
	
#line 407 "tmp.c-parser.c"
	}
	/* END OF ACTION: expected-arrow */
    }
}

static void
ZR130(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	NStringT ZI132;

	/* BEGINNING OF INLINE: c-parse-grammar::identifier */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_C_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI132, c_lexer_string_value(c_current_stream));
	
#line 433 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      case (C_TOK_SID_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: SID-IDENTIFIER */
		    {
#line 247 "c-parser.act"

		nstring_assign(&ZI132, c_lexer_string_value(c_current_stream));
	
#line 447 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: SID-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      default:
		goto ZL1;
	    }
	}
	/* END OF INLINE: c-parse-grammar::identifier */
	/* BEGINNING OF ACTION: set-map */
	{
#line 345 "c-parser.act"

		c_current_entry = table_get_entry(c_current_table, (&ZI132));
		if (c_current_entry == NULL) {
			err_unknown("identifier", (&ZI132));
		} else if (entry_get_mapping(c_current_entry)) {
			error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
				"attempt to remap identifier '%S'", (void *) (&ZI132));
			c_current_entry = NULL;
		} else {
			switch (entry_type(c_current_entry)) EXHAUSTIVE {
			case ET_NAME:
			case ET_ACTION:
			case ET_NON_LOCAL:
				error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
					"cannot map identifier '%S' [it must be a rule, a basic or a type]", (void *) (&ZI132));
				c_current_entry = NULL;
				break;
			case ET_RENAME:
			case ET_PREDICATE:
				/* UNREACHED */
				break;
			case ET_TYPE:
			case ET_RULE:
			case ET_BASIC:
				break;
			}
		}
		nstring_destroy(&(ZI132));
	
#line 490 "tmp.c-parser.c"
	}
	/* END OF ACTION: set-map */
	ZR161 ();
	/* BEGINNING OF INLINE: 134 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		NStringT ZI135;

		switch (CURRENT_TERMINAL) {
		  case (C_TOK_C_HIDENTIFIER):
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI135, c_lexer_string_value(c_current_stream));
	
#line 511 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    break;
		  default:
		    goto ZL4;
		}
		ADVANCE_LEXER;
		/* BEGINNING OF ACTION: x-set-map */
		{
#line 376 "c-parser.act"

		if (c_current_entry) {
			entry_set_mapping(c_current_entry, &(ZI135));
		} else {
			nstring_destroy(&(ZI135));
		}
	
#line 529 "tmp.c-parser.c"
		}
		/* END OF ACTION: x-set-map */
		ZR221 ();
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
	    }
	    goto ZL3;
	  ZL4:;
	    {
		/* BEGINNING OF ACTION: expected-c-identifier */
		{
#line 878 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("C identifier");
		}
	
#line 549 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-c-identifier */
		/* BEGINNING OF ACTION: skip-to-end-of-map */
		{
#line 1021 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HASSIGNMENTS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HPARAM_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HRESULT_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTERMINALS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HACTIONS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			} else if (CURRENT_TERMINAL == (C_TOK_CODE)) {
				c_code_deallocate(c_lexer_code_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 582 "tmp.c-parser.c"
		}
		/* END OF ACTION: skip-to-end-of-map */
	    }
	  ZL3:;
	}
	/* END OF INLINE: 134 */
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR164(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	NStringT ZI108;

	/* BEGINNING OF INLINE: c-parse-grammar::identifier */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_C_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 616 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      case (C_TOK_SID_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: SID-IDENTIFIER */
		    {
#line 247 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 630 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: SID-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      default:
		goto ZL1;
	    }
	}
	/* END OF INLINE: c-parse-grammar::identifier */
	/* BEGINNING OF ACTION: assign */
	{
#line 443 "c-parser.act"

		c_current_entry = table_get_type(c_current_table, (&ZI108));
		if (c_current_entry == NULL) {
			error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
				"assignment operation defined for unknown type '%S'", (void *) (&ZI108));
		} else if (type_get_assign_code(entry_get_type(c_current_entry))) {
			err_mult("assignment", "type", (&ZI108));
			c_current_entry = NULL;
		}
		nstring_destroy(&(ZI108));
	
#line 655 "tmp.c-parser.c"
	}
	/* END OF ACTION: assign */
	ZR145 ();
	ZR124 ();
	/* BEGINNING OF INLINE: 167 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		CCodeP ZI168;

		ZR112 (&ZI168);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
		/* BEGINNING OF ACTION: x-assign */
		{
#line 492 "c-parser.act"

		if (c_current_entry) {
			BoolT  errored = FALSE;
			KeyT  *key     = entry_key (c_current_entry);
			TypeTupleT tmp;

			types_init(&tmp);
			types_add_type_entry(&tmp, c_current_entry, FALSE);

			if (!types_disjoint_names(&c_saved_type)) {
				err_clash("assignment", "parameter", "type", key, &c_saved_type);
				errored = TRUE;
			}

			if (!types_fillin_types (&c_saved_type, &tmp)) {
				err_mismatch("parameter", "assignment", "type", key, &tmp, &c_saved_type);
				errored = TRUE;
			}

			if (!types_disjoint_names (&c_current_type)) {
				err_clash("assignment", "result", "type", key, &c_saved_type);
				errored = TRUE;
			}

			if (!types_fillin_types (&c_current_type, &tmp)) {
				err_mismatch("result", "assignment", "type", key, &tmp, &c_saved_type);
				errored = TRUE;
			}

			if (types_intersect (&c_saved_type, &c_current_type)) {
				err_formal_clash("assignment", "type", key, &c_saved_type, &c_current_type);
				errored = TRUE;
			}

			types_destroy(&tmp);
			if (errored) {
				types_destroy(&c_saved_type);
				types_destroy(&c_current_type);
				c_code_deallocate((ZI168));
				c_current_entry = NULL;
			} else {
			TypeT *type;

				type = entry_get_type (c_current_entry);
				c_code_check((ZI168), FALSE, FALSE, &c_saved_type, &c_current_type, c_current_table);
				type_set_assign_code(type,  (ZI168));
			}
		} else {
			types_destroy(&c_saved_type);
			types_destroy(&c_current_type);
			c_code_deallocate((ZI168));
		}
	
#line 730 "tmp.c-parser.c"
		}
		/* END OF ACTION: x-assign */
		ZR221 ();
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
	    }
	    goto ZL3;
	  ZL4:;
	    {
		/* BEGINNING OF ACTION: expected-code */
		{
#line 926 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("code block");
		}
	
#line 750 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-code */
		/* BEGINNING OF ACTION: skip-to-end-of-assignment */
		{
#line 1049 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HPARAM_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HRESULT_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTERMINALS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HACTIONS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			} else if (CURRENT_TERMINAL == (C_TOK_CODE)) {
				c_code_deallocate(c_lexer_code_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 782 "tmp.c-parser.c"
		}
		/* END OF ACTION: skip-to-end-of-assignment */
	    }
	  ZL3:;
	}
	/* END OF INLINE: 167 */
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR193(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case (C_TOK_BEGIN_HACTION):
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 195 */
	{
	    {
		NStringT ZI108;

		/* BEGINNING OF INLINE: c-parse-grammar::identifier */
		{
		    switch (CURRENT_TERMINAL) {
		      case (C_TOK_C_HIDENTIFIER):
			{
			    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
			    {
#line 243 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 826 "tmp.c-parser.c"
			    }
			    /* END OF EXTRACT: C-IDENTIFIER */
			    ADVANCE_LEXER;
			}
			break;
		      case (C_TOK_SID_HIDENTIFIER):
			{
			    /* BEGINNING OF EXTRACT: SID-IDENTIFIER */
			    {
#line 247 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 840 "tmp.c-parser.c"
			    }
			    /* END OF EXTRACT: SID-IDENTIFIER */
			    ADVANCE_LEXER;
			}
			break;
		      default:
			goto ZL3;
		    }
		}
		/* END OF INLINE: c-parse-grammar::identifier */
		/* BEGINNING OF ACTION: set-action */
		{
#line 722 "c-parser.act"

		c_current_entry = table_get_action(c_current_table, (&ZI108));
		if (c_current_entry == NULL) {
			error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
				"definition for unknown action '%S'", (void *) (&ZI108));
		} else {
			ActionT *action;

			action = entry_get_action(c_current_entry);
			if (action_get_code(action)) {
				error_posn(ERROR_SERIOUS, istream_name(&c_lexer_stream->istream), (int) istream_line(&c_lexer_stream->istream),
					"multiple definitions for action '%S'", (void *) (&ZI108));
				c_current_entry = NULL;
			}
		}
		nstring_destroy(&(ZI108));
	
#line 871 "tmp.c-parser.c"
		}
		/* END OF ACTION: set-action */
		/* BEGINNING OF INLINE: 196 */
		{
		    {
			switch (CURRENT_TERMINAL) {
			  case (C_TOK_END_HACTION):
			    break;
			  default:
			    goto ZL6;
			}
			ADVANCE_LEXER;
		    }
		    goto ZL5;
		  ZL6:;
		    {
			/* BEGINNING OF ACTION: expected-end-action */
			{
#line 914 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("'>'");
		}
	
#line 896 "tmp.c-parser.c"
			}
			/* END OF ACTION: expected-end-action */
		    }
		  ZL5:;
		}
		/* END OF INLINE: 196 */
		ZR145 ();
		ZR124 ();
		/* BEGINNING OF INLINE: 198 */
		{
		    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL3;
		    }
		    {
			CCodeP ZI168;

			ZR112 (&ZI168);
			if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			    RESTORE_LEXER;
			    goto ZL8;
			}
			/* BEGINNING OF ACTION: x-set-action */
			{
#line 777 "c-parser.act"

		if (c_current_entry) {
			ActionT    *action  = entry_get_action(c_current_entry);
			TypeTupleT *param   = action_param(action);
			TypeTupleT *result  = action_result(action);
			BoolT       errored = FALSE;
			KeyT       *key     = entry_key(c_current_entry);

			if (!types_disjoint_names(&c_saved_type)) {
				err_field_clash("parameter", "action", key, &c_saved_type);
				errored = TRUE;
			}

			if (!types_fillin_types(&c_saved_type, param)) {
				err_field_mismatch("parameter", key, &c_saved_type, param);
				errored = TRUE;
			}

			if (!types_disjoint_names(&c_current_type)) {
				err_field_clash("result", "action", key, &c_current_type);
				errored = TRUE;
			}

			if (!types_fillin_types(&c_current_type, result)) {
				err_field_mismatch("result", key, &c_saved_type, result);
				errored = TRUE;
			}

			if (types_intersect(&c_saved_type, &c_current_type)) {
				error_posn(ERROR_SERIOUS, istream_name(&c_lexer_stream->istream), (int) istream_line(&c_lexer_stream->istream),
					"formal clash for action '%K' [parameter = %Y, result = %Y]",
					(void *) key, (void *) &c_saved_type, (void *) &c_current_type);
				errored = TRUE;
			}

			if (errored) {
				types_destroy(&c_saved_type);
				types_destroy(&c_current_type);
				c_code_deallocate((ZI168));
				c_current_entry = NULL;
			} else {
				c_code_check((ZI168), TRUE, FALSE, &c_saved_type, &c_current_type, c_current_table);
				types_propogate_mutations(param, &c_saved_type);
				action_set_code(action,  (ZI168));
			}
		} else {
			types_destroy(&c_saved_type);
			types_destroy(&c_current_type);
			c_code_deallocate((ZI168));
		}
	
#line 973 "tmp.c-parser.c"
			}
			/* END OF ACTION: x-set-action */
			ZR221 ();
			if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			    RESTORE_LEXER;
			    goto ZL8;
			}
		    }
		    goto ZL7;
		  ZL8:;
		    {
			/* BEGINNING OF ACTION: expected-code */
			{
#line 926 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("code block");
		}
	
#line 993 "tmp.c-parser.c"
			}
			/* END OF ACTION: expected-code */
			/* BEGINNING OF ACTION: skip-to-end-of-action */
			{
#line 1152 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER)
			|| CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			} else if (CURRENT_TERMINAL == (C_TOK_CODE)) {
				c_code_deallocate(c_lexer_code_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 1022 "tmp.c-parser.c"
			}
			/* END OF ACTION: skip-to-end-of-action */
		    }
		  ZL7:;
		}
		/* END OF INLINE: 198 */
	    }
	    goto ZL2;
	  ZL3:;
	    {
		/* BEGINNING OF ACTION: expected-identifier */
		{
#line 872 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("identifier");
		}
	
#line 1041 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-identifier */
		/* BEGINNING OF ACTION: skip-to-end-of-action */
		{
#line 1152 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER)
			|| CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			} else if (CURRENT_TERMINAL == (C_TOK_CODE)) {
				c_code_deallocate(c_lexer_code_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 1070 "tmp.c-parser.c"
		}
		/* END OF ACTION: skip-to-end-of-action */
	    }
	  ZL2:;
	}
	/* END OF INLINE: 195 */
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR152(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
  ZL2_152:;
    {
	ZR147 ();
	/* BEGINNING OF INLINE: 154 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		/* BEGINNING OF ACTION: is-close-tuple-or-skipped-or-eof */
		{
#line 1180 "c-parser.act"

		(ZI0) = (CURRENT_TERMINAL == (C_TOK_CLOSE_HTUPLE)
			|| CURRENT_TERMINAL == (C_TOK_EOF)
			|| c_propagating_error);
	
#line 1108 "tmp.c-parser.c"
		}
		/* END OF ACTION: is-close-tuple-or-skipped-or-eof */
		if (!ZI0)
		    goto ZL5;
		goto ZL3;
	    }
	    /*UNREACHED*/
	  ZL5:;
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_SEPARATOR):
		{
		    ADVANCE_LEXER;
		    /* BEGINNING OF INLINE: c-parse-grammar::function-type-defn::tuple-defn-list-1 */
		    goto ZL2_152;
		    /* END OF INLINE: c-parse-grammar::function-type-defn::tuple-defn-list-1 */
		}
		/*UNREACHED*/
	      default:
		goto ZL4;
	    }
	    /*UNREACHED*/
	  ZL4:;
	    {
		/* BEGINNING OF ACTION: expected-separator */
		{
#line 884 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("','");
		}
	
#line 1140 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-separator */
		/* BEGINNING OF INLINE: c-parse-grammar::function-type-defn::tuple-defn-list-1 */
		goto ZL2_152;
		/* END OF INLINE: c-parse-grammar::function-type-defn::tuple-defn-list-1 */
	    }
	    /*UNREACHED*/
	  ZL3:;
	}
	/* END OF INLINE: 154 */
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR191(void)
{
  ZL2_191:;
    switch (CURRENT_TERMINAL) {
      case (C_TOK_BEGIN_HACTION):
	{
	    ZR193 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    /* BEGINNING OF ACTION: skip-recover */
	    {
#line 1177 "c-parser.act"

		c_propagating_error = FALSE;
	
#line 1176 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: skip-recover */
	    /* BEGINNING OF INLINE: c-parse-grammar::action-list */
	    goto ZL2_191;
	    /* END OF INLINE: c-parse-grammar::action-list */
	}
	/*UNREACHED*/
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR172(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	NStringT ZI108;

	/* BEGINNING OF INLINE: c-parse-grammar::identifier */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_C_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 1215 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      case (C_TOK_SID_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: SID-IDENTIFIER */
		    {
#line 247 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 1229 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: SID-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      default:
		goto ZL1;
	    }
	}
	/* END OF INLINE: c-parse-grammar::identifier */
	/* BEGINNING OF ACTION: passign */
	{
#line 509 "c-parser.act"

		c_current_entry = table_get_type(c_current_table, (&ZI108));
		if (c_current_entry == NULL) {
			error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
				"parameter assignment operation defined for unknown type '%S'", (void *) (&ZI108));
		} else if (type_get_param_assign_code(entry_get_type(c_current_entry))) {
			err_mult("parameter assignment", "type", (&ZI108));
			c_current_entry = NULL;
		}
		nstring_destroy(&(ZI108));
	
#line 1254 "tmp.c-parser.c"
	}
	/* END OF ACTION: passign */
	ZR145 ();
	ZR124 ();
	/* BEGINNING OF INLINE: 175 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		CCodeP ZI168;

		ZR112 (&ZI168);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
		/* BEGINNING OF ACTION: x-passign */
		{
#line 557 "c-parser.act"

		if (c_current_entry) {
			BoolT  errored = FALSE;
			KeyT  *key     = entry_key(c_current_entry);
			TypeTupleT tmp;

			types_init(&tmp);
			types_add_type_entry(&tmp, c_current_entry, FALSE);
			if (!types_disjoint_names(&c_saved_type)) {
				err_clash("parameter assignment", "parameter", "type", key, &c_saved_type);
				errored = TRUE;
			}

			if (!types_fillin_types(&c_saved_type, &tmp)) {
				err_mismatch("parameter", "parameter assignment", "type", key, &c_saved_type, &tmp);
				errored = TRUE;
			}

			if (!types_disjoint_names(&c_current_type)) {
				err_clash("parameter assignment", "result", "type", key, &c_saved_type);
				errored = TRUE;
			}

			if (!types_fillin_types(&c_current_type, &tmp)) {
				err_mismatch("result type", "parameter assignment", "type", key, &c_saved_type, &tmp);
				errored = TRUE;
			}

			if (types_intersect(&c_saved_type, &c_current_type)) {
				err_formal_clash("parameter assignment", "type", key, &c_saved_type, &c_current_type);
				errored = TRUE;
			}

			types_destroy(&tmp);
			if (errored) {
				types_destroy(&c_saved_type);
				types_destroy(&c_current_type);
				c_code_deallocate((ZI168));
				c_current_entry = NULL;
			} else {
				TypeT *type = entry_get_type(c_current_entry);

				c_code_check((ZI168), FALSE, TRUE, &c_saved_type,
					&c_current_type, c_current_table);
				type_set_param_assign_code(type,  (ZI168));
			}
		} else {
			types_destroy(&c_saved_type);
			types_destroy(&c_current_type);
			c_code_deallocate((ZI168));
		}
	
#line 1328 "tmp.c-parser.c"
		}
		/* END OF ACTION: x-passign */
		ZR221 ();
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
	    }
	    goto ZL3;
	  ZL4:;
	    {
		/* BEGINNING OF ACTION: expected-code */
		{
#line 926 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("code block");
		}
	
#line 1348 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-code */
		/* BEGINNING OF ACTION: skip-to-end-of-param-assign */
		{
#line 1076 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HRESULT_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTERMINALS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HACTIONS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER)
				|| CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			} else if (CURRENT_TERMINAL == (C_TOK_CODE)) {
				c_code_deallocate(c_lexer_code_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 1380 "tmp.c-parser.c"
		}
		/* END OF ACTION: skip-to-end-of-param-assign */
	    }
	  ZL3:;
	}
	/* END OF INLINE: 175 */
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR120(void)
{
  ZL2_120:;
    switch (CURRENT_TERMINAL) {
      case (C_TOK_SID_HIDENTIFIER): case (C_TOK_C_HIDENTIFIER):
	{
	    ZR122 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    /* BEGINNING OF ACTION: skip-recover */
	    {
#line 1177 "c-parser.act"

		c_propagating_error = FALSE;
	
#line 1412 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: skip-recover */
	    /* BEGINNING OF INLINE: c-parse-grammar::prefix-list */
	    goto ZL2_120;
	    /* END OF INLINE: c-parse-grammar::prefix-list */
	}
	/*UNREACHED*/
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR114(CCodeP ZI111)
{
    switch (CURRENT_TERMINAL) {
      case (C_TOK_ACT_HADVANCE):
	{
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-advance */
	    {
#line 826 "c-parser.act"

		c_code_append_advance((ZI111));
	
#line 1444 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-advance */
	}
	break;
      case (C_TOK_ACT_HAT):
	{
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-at */
	    {
#line 813 "c-parser.act"

		NStringT ns;
		nstring_copy_cstring(&ns, "@");	/* TODO append '@' to code buffer? */
		c_code_append_string((ZI111), &ns);	/* TODO really append_label()? */
	
#line 1460 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-at */
	}
	break;
      case (C_TOK_ACT_HBASIC):
	{
	    NStringT ZI116;

	    /* BEGINNING OF EXTRACT: ACT-BASIC */
	    {
#line 255 "c-parser.act"

		nstring_assign(&ZI116, c_lexer_string_value(c_current_stream));
	
#line 1475 "tmp.c-parser.c"
	    }
	    /* END OF EXTRACT: ACT-BASIC */
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-basic */
	    {
#line 830 "c-parser.act"

		c_code_append_basic((ZI111), &(ZI116));
	
#line 1485 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-basic */
	}
	break;
      case (C_TOK_ACT_HCODESTRING):
	{
	    NStringT ZI117;

	    /* BEGINNING OF EXTRACT: ACT-CODESTRING */
	    {
#line 275 "c-parser.act"

		nstring_assign(&ZI117, c_lexer_string_value(c_current_stream));
	
#line 1500 "tmp.c-parser.c"
	    }
	    /* END OF EXTRACT: ACT-CODESTRING */
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-codestring */
	    {
#line 850 "c-parser.act"

		assert(!nstring_contains(&(ZI117), '@'));	/* XXX '@'? */
		c_code_append_string((ZI111), &(ZI117));
	
#line 1511 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-codestring */
	}
	break;
      case (C_TOK_ACT_HEOF):
	{
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: E-eof-in-code-block */
	    {
#line 857 "c-parser.act"

		error_posn(ERROR_SERIOUS, istream_name(&c_lexer_stream->istream), (int) istream_line(&c_lexer_stream->istream),
			"end of file in C code block");
	
#line 1526 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: E-eof-in-code-block */
	}
	break;
      case (C_TOK_ACT_HEXCEPTION):
	{
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-exception */
	    {
#line 818 "c-parser.act"

		c_code_append_exception((ZI111));
	
#line 1540 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-exception */
	}
	break;
      case (C_TOK_ACT_HIDENTIFIER):
	{
	    NStringT ZI116;

	    /* BEGINNING OF EXTRACT: ACT-IDENTIFIER */
	    {
#line 267 "c-parser.act"

		nstring_assign(&ZI116, c_lexer_string_value(c_current_stream));
	
#line 1555 "tmp.c-parser.c"
	    }
	    /* END OF EXTRACT: ACT-IDENTIFIER */
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-identifier */
	    {
#line 842 "c-parser.act"

		c_code_append_identifier((ZI111), &(ZI116));
	
#line 1565 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-identifier */
	}
	break;
      case (C_TOK_ACT_HLABEL):
	{
	    NStringT ZI116;

	    /* BEGINNING OF EXTRACT: ACT-LABEL */
	    {
#line 259 "c-parser.act"

		nstring_assign(&ZI116, c_lexer_string_value(c_current_stream));
	
#line 1580 "tmp.c-parser.c"
	    }
	    /* END OF EXTRACT: ACT-LABEL */
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-label */
	    {
#line 834 "c-parser.act"

		c_code_append_label((ZI111), &(ZI116));
	
#line 1590 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-label */
	}
	break;
      case (C_TOK_ACT_HMODIFIABLE):
	{
	    NStringT ZI116;

	    /* BEGINNING OF EXTRACT: ACT-MODIFIABLE */
	    {
#line 271 "c-parser.act"

		nstring_assign(&ZI116, c_lexer_string_value(c_current_stream));
	
#line 1605 "tmp.c-parser.c"
	    }
	    /* END OF EXTRACT: ACT-MODIFIABLE */
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-modifiable */
	    {
#line 846 "c-parser.act"

		c_code_append_modifiable((ZI111), &(ZI116));
	
#line 1615 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-modifiable */
	}
	break;
      case (C_TOK_ACT_HREFERENCE):
	{
	    NStringT ZI116;

	    /* BEGINNING OF EXTRACT: ACT-REFERENCE */
	    {
#line 263 "c-parser.act"

		nstring_assign(&ZI116, c_lexer_string_value(c_current_stream));
	
#line 1630 "tmp.c-parser.c"
	    }
	    /* END OF EXTRACT: ACT-REFERENCE */
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-reference */
	    {
#line 838 "c-parser.act"

		c_code_append_reference((ZI111), &(ZI116));
	
#line 1640 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-reference */
	}
	break;
      case (C_TOK_ACT_HTERMINAL):
	{
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: append-ccode-terminal */
	    {
#line 822 "c-parser.act"

		c_code_append_terminal((ZI111));
	
#line 1654 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: append-ccode-terminal */
	}
	break;
      case (ERROR_TERMINAL):
	return;
      default:
	goto ZL1;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR122(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	NStringT ZI123;

	/* BEGINNING OF INLINE: c-parse-grammar::identifier */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_C_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI123, c_lexer_string_value(c_current_stream));
	
#line 1690 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      case (C_TOK_SID_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: SID-IDENTIFIER */
		    {
#line 247 "c-parser.act"

		nstring_assign(&ZI123, c_lexer_string_value(c_current_stream));
	
#line 1704 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: SID-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      default:
		goto ZL1;
	    }
	}
	/* END OF INLINE: c-parse-grammar::identifier */
	/* BEGINNING OF ACTION: set-prefix */
	{
#line 299 "c-parser.act"

		int prefix;

		if (!c_inited_prefix_names) {
			nstring_copy_cstring(&c_prefix_names[CPFX_TYPE], "type");
			nstring_copy_cstring(&c_prefix_names[CPFX_FN], "function");
			nstring_copy_cstring(&c_prefix_names[CPFX_IN], "input");
			nstring_copy_cstring(&c_prefix_names[CPFX_OUT], "output");
			nstring_copy_cstring(&c_prefix_names[CPFX_LABEL], "label");
			nstring_copy_cstring(&c_prefix_names[CPFX_TERMINAL], "terminal");
			nstring_copy_cstring(&c_prefix_names[CPFX_PERSISTENTS], "persistents");
			c_inited_prefix_names = TRUE;
		}

		for (prefix = 0; prefix < CPFX_NUM_PREFIXES; prefix++) {
			if (nstring_ci_equal((&ZI123), &c_prefix_names[prefix])) {
				break;
			}
		}

		c_current_prefix = (CPrefixT) prefix;
		if (c_current_prefix == CPFX_NUM_PREFIXES) {
			err_unknown("prefix type", (&ZI123));
		}
		nstring_destroy(&(ZI123));
	
#line 1744 "tmp.c-parser.c"
	}
	/* END OF ACTION: set-prefix */
	ZR124 ();
	/* BEGINNING OF INLINE: 125 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		NStringT ZI126;

		switch (CURRENT_TERMINAL) {
		  case (C_TOK_C_HIDENTIFIER):
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI126, c_lexer_string_value(c_current_stream));
	
#line 1765 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    break;
		  default:
		    goto ZL4;
		}
		ADVANCE_LEXER;
		/* BEGINNING OF ACTION: x-set-prefix */
		{
#line 313 "c-parser.act"

		if (c_current_prefix == CPFX_NUM_PREFIXES) {
			nstring_destroy(&(ZI126));
		} else {
			NStringT *prefix = c_out_info_prefix(c_current_out_info, c_current_prefix);

			nstring_destroy(prefix);
			nstring_assign(prefix, &(ZI126));
		}
	
#line 1786 "tmp.c-parser.c"
		}
		/* END OF ACTION: x-set-prefix */
		ZR221 ();
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
	    }
	    goto ZL3;
	  ZL4:;
	    {
		/* BEGINNING OF ACTION: expected-c-identifier */
		{
#line 878 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("C identifier");
		}
	
#line 1806 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-c-identifier */
		/* BEGINNING OF ACTION: skip-to-end-of-prefix */
		{
#line 992 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HMAPS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTERMINALS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HASSIGNMENTS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HPARAM_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HRESULT_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HACTIONS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			} else if (CURRENT_TERMINAL == (C_TOK_CODE)) {
				c_code_deallocate(c_lexer_code_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 1840 "tmp.c-parser.c"
		}
		/* END OF ACTION: skip-to-end-of-prefix */
	    }
	  ZL3:;
	}
	/* END OF INLINE: 125 */
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

void
c_parse_grammar(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	/* BEGINNING OF INLINE: 200 */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_BLT_HPREFIXES):
		{
		    ADVANCE_LEXER;
		    ZR120 ();
		    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 200 */
	/* BEGINNING OF INLINE: 201 */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_BLT_HPERSISTENTS):
		{
		    ADVANCE_LEXER;
		    ZR137 ();
		    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 201 */
	/* BEGINNING OF INLINE: 202 */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_BLT_HMAPS):
		{
		    ADVANCE_LEXER;
		    ZR128 ();
		    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 202 */
	/* BEGINNING OF INLINE: 203 */
	{
	    {
		switch (CURRENT_TERMINAL) {
		  case (C_TOK_BLT_HHEADER):
		    break;
		  default:
		    goto ZL6;
		}
		ADVANCE_LEXER;
	    }
	    goto ZL5;
	  ZL6:;
	    {
		/* BEGINNING OF ACTION: expected-blt-header */
		{
#line 932 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("'%header%'");
		}
	
#line 1937 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-blt-header */
	    }
	  ZL5:;
	}
	/* END OF INLINE: 203 */
	/* BEGINNING OF INLINE: 204 */
	{
	    {
		CCodeP ZI205;

		ZR112 (&ZI205);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL8;
		}
		/* BEGINNING OF ACTION: set-header1 */
		{
#line 388 "c-parser.act"

		c_code_check((ZI205), FALSE, FALSE, NULL, NULL, c_current_table);
		c_out_info_set_header1(c_current_out_info, (ZI205));
	
#line 1961 "tmp.c-parser.c"
		}
		/* END OF ACTION: set-header1 */
	    }
	    goto ZL7;
	  ZL8:;
	    {
		/* BEGINNING OF ACTION: expected-code */
		{
#line 926 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("code block");
		}
	
#line 1976 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-code */
	    }
	  ZL7:;
	}
	/* END OF INLINE: 204 */
	ZR218 ();
	/* BEGINNING OF INLINE: 207 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		CCodeP ZI208;

		ZR112 (&ZI208);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL10;
		}
		/* BEGINNING OF ACTION: set-header2 */
		{
#line 393 "c-parser.act"

		c_code_check((ZI208), FALSE, FALSE, NULL, NULL, c_current_table);
		c_out_info_set_header2(c_current_out_info, (ZI208));
	
#line 2005 "tmp.c-parser.c"
		}
		/* END OF ACTION: set-header2 */
	    }
	    goto ZL9;
	  ZL10:;
	    {
		/* BEGINNING OF ACTION: expected-code */
		{
#line 926 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("code block");
		}
	
#line 2020 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-code */
	    }
	  ZL9:;
	}
	/* END OF INLINE: 207 */
	ZR221 ();
	/* BEGINNING OF INLINE: 210 */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_BLT_HASSIGNMENTS):
		{
		    ADVANCE_LEXER;
		    ZR162 ();
		    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      case (ERROR_TERMINAL):
		RESTORE_LEXER;
		goto ZL1;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 210 */
	/* BEGINNING OF INLINE: 211 */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_BLT_HPARAM_HASSIGN):
		{
		    ADVANCE_LEXER;
		    ZR170 ();
		    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 211 */
	/* BEGINNING OF INLINE: 212 */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_BLT_HRESULT_HASSIGN):
		{
		    ADVANCE_LEXER;
		    ZR177 ();
		    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 212 */
	/* BEGINNING OF INLINE: 213 */
	{
	    {
		switch (CURRENT_TERMINAL) {
		  case (C_TOK_BLT_HTERMINALS):
		    break;
		  default:
		    goto ZL15;
		}
		ADVANCE_LEXER;
	    }
	    goto ZL14;
	  ZL15:;
	    {
		/* BEGINNING OF ACTION: expected-blt-terminals */
		{
#line 938 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("'%terminals%'");
		}
	
#line 2107 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-blt-terminals */
	    }
	  ZL14:;
	}
	/* END OF INLINE: 213 */
	ZR184 ();
	/* BEGINNING OF INLINE: 214 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		switch (CURRENT_TERMINAL) {
		  case (C_TOK_BLT_HACTIONS):
		    break;
		  default:
		    goto ZL17;
		}
		ADVANCE_LEXER;
	    }
	    goto ZL16;
	  ZL17:;
	    {
		/* BEGINNING OF ACTION: expected-blt-actions */
		{
#line 944 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("'%actions%'");
		}
	
#line 2141 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-blt-actions */
	    }
	  ZL16:;
	}
	/* END OF INLINE: 214 */
	ZR191 ();
	/* BEGINNING OF INLINE: 215 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		switch (CURRENT_TERMINAL) {
		  case (C_TOK_BLT_HTRAILER):
		    break;
		  default:
		    goto ZL19;
		}
		ADVANCE_LEXER;
	    }
	    goto ZL18;
	  ZL19:;
	    {
		/* BEGINNING OF ACTION: expected-blt-trailer */
		{
#line 950 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("'%trailer%'");
		}
	
#line 2175 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-blt-trailer */
	    }
	  ZL18:;
	}
	/* END OF INLINE: 215 */
	/* BEGINNING OF INLINE: 216 */
	{
	    {
		CCodeP ZI217;

		ZR112 (&ZI217);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL21;
		}
		/* BEGINNING OF ACTION: set-trailer1 */
		{
#line 792 "c-parser.act"

		c_code_check((ZI217), FALSE, FALSE, NULL, NULL, c_current_table);
		c_out_info_set_trailer1 (c_current_out_info, (ZI217));
	
#line 2199 "tmp.c-parser.c"
		}
		/* END OF ACTION: set-trailer1 */
	    }
	    goto ZL20;
	  ZL21:;
	    {
		/* BEGINNING OF ACTION: expected-code */
		{
#line 926 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("code block");
		}
	
#line 2214 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-code */
	    }
	  ZL20:;
	}
	/* END OF INLINE: 216 */
	ZR218 ();
	/* BEGINNING OF INLINE: 219 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		CCodeP ZI220;

		ZR112 (&ZI220);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL23;
		}
		/* BEGINNING OF ACTION: set-trailer2 */
		{
#line 797 "c-parser.act"

		c_code_check((ZI220), FALSE, FALSE, NULL, NULL, c_current_table);
		c_out_info_set_trailer2(c_current_out_info, (ZI220));
	
#line 2243 "tmp.c-parser.c"
		}
		/* END OF ACTION: set-trailer2 */
	    }
	    goto ZL22;
	  ZL23:;
	    {
		/* BEGINNING OF ACTION: expected-code */
		{
#line 926 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("code block");
		}
	
#line 2258 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-code */
	    }
	  ZL22:;
	}
	/* END OF INLINE: 219 */
	ZR221 ();
	/* BEGINNING OF INLINE: 222 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		switch (CURRENT_TERMINAL) {
		  case (C_TOK_EOF):
		    break;
		  default:
		    goto ZL25;
		}
		ADVANCE_LEXER;
	    }
	    goto ZL24;
	  ZL25:;
	    {
		/* BEGINNING OF ACTION: expected-eof */
		{
#line 956 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("end of file");
		}
	
#line 2292 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-eof */
	    }
	  ZL24:;
	}
	/* END OF INLINE: 222 */
    }
    return;
  ZL1:;
    {
	/* BEGINNING OF ACTION: unhandled-syntax-error */
	{
#line 866 "c-parser.act"

		UNREACHED;
	
#line 2309 "tmp.c-parser.c"
	}
	/* END OF ACTION: unhandled-syntax-error */
    }
}

static void
ZR155(void)
{
    switch (CURRENT_TERMINAL) {
      case (C_TOK_C_HIDENTIFIER):
	{
	    ZR152 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	}
	break;
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR157(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	/* BEGINNING OF ACTION: init-tuple */
	{
#line 413 "c-parser.act"

		types_init(&c_current_type);
	
#line 2352 "tmp.c-parser.c"
	}
	/* END OF ACTION: init-tuple */
	/* BEGINNING OF INLINE: 159 */
	{
	    {
		switch (CURRENT_TERMINAL) {
		  case (C_TOK_OPEN_HTUPLE):
		    break;
		  default:
		    goto ZL3;
		}
		ADVANCE_LEXER;
	    }
	    goto ZL2;
	  ZL3:;
	    {
		/* BEGINNING OF ACTION: expected-open-tuple */
		{
#line 890 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("'('");
		}
	
#line 2377 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-open-tuple */
	    }
	  ZL2:;
	}
	/* END OF INLINE: 159 */
	ZR155 ();
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	    RESTORE_LEXER;
	    goto ZL1;
	}
	/* BEGINNING OF ACTION: skip-recover */
	{
#line 1177 "c-parser.act"

		c_propagating_error = FALSE;
	
#line 2395 "tmp.c-parser.c"
	}
	/* END OF ACTION: skip-recover */
	/* BEGINNING OF INLINE: 160 */
	{
	    {
		switch (CURRENT_TERMINAL) {
		  case (C_TOK_CLOSE_HTUPLE):
		    break;
		  default:
		    goto ZL5;
		}
		ADVANCE_LEXER;
	    }
	    goto ZL4;
	  ZL5:;
	    {
		/* BEGINNING OF ACTION: expected-close-tuple */
		{
#line 896 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("')'");
		}
	
#line 2420 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-close-tuple */
	    }
	  ZL4:;
	}
	/* END OF INLINE: 160 */
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR139(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	NStringT ZI126;
	NStringT ZI143;

	/* BEGINNING OF INLINE: 141 */
	{
	    {
		switch (CURRENT_TERMINAL) {
		  case (C_TOK_C_HIDENTIFIER):
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI126, c_lexer_string_value(c_current_stream));
	
#line 2455 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    break;
		  default:
		    goto ZL3;
		}
		ADVANCE_LEXER;
	    }
	    goto ZL2;
	  ZL3:;
	    {
		/* BEGINNING OF ACTION: empty-string */
		{
#line 336 "c-parser.act"

		nstring_init(&(ZI126));
	
#line 2473 "tmp.c-parser.c"
		}
		/* END OF ACTION: empty-string */
		/* BEGINNING OF ACTION: expected-c-identifier */
		{
#line 878 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("C identifier");
		}
	
#line 2484 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-c-identifier */
	    }
	  ZL2:;
	}
	/* END OF INLINE: 141 */
	switch (CURRENT_TERMINAL) {
	  case (C_TOK_TYPEMARK):
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 142 */
	{
	    {
		switch (CURRENT_TERMINAL) {
		  case (C_TOK_C_HIDENTIFIER):
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI143, c_lexer_string_value(c_current_stream));
	
#line 2509 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    break;
		  default:
		    goto ZL5;
		}
		ADVANCE_LEXER;
	    }
	    goto ZL4;
	  ZL5:;
	    {
		/* BEGINNING OF ACTION: empty-string */
		{
#line 336 "c-parser.act"

		nstring_init(&(ZI143));
	
#line 2527 "tmp.c-parser.c"
		}
		/* END OF ACTION: empty-string */
		/* BEGINNING OF ACTION: expected-identifier */
		{
#line 872 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("identifier");
		}
	
#line 2538 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-identifier */
	    }
	  ZL4:;
	}
	/* END OF INLINE: 142 */
	/* BEGINNING OF ACTION: set-persistent */
	{
#line 323 "c-parser.act"

		if(persistent_list_find(c_out_info_persistents(c_current_out_info), (&ZI126)) == NULL) {
			PersistentT *p;

			p = persistent_create(&(ZI126), &(ZI143));
			persistent_list_append(c_out_info_persistents(c_current_out_info), p);
		} else {
			nstring_destroy(&(ZI126));
			nstring_destroy(&(ZI143));
			/* TODO error out E_c_persistent_variable_name_conflict((&ZI126))) ;*/
		}
	
#line 2560 "tmp.c-parser.c"
	}
	/* END OF ACTION: set-persistent */
	ZR221 ();
	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	    RESTORE_LEXER;
	    goto ZL1;
	}
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR179(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	NStringT ZI108;

	/* BEGINNING OF INLINE: c-parse-grammar::identifier */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_C_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 2595 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      case (C_TOK_SID_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: SID-IDENTIFIER */
		    {
#line 247 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 2609 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: SID-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      default:
		goto ZL1;
	    }
	}
	/* END OF INLINE: c-parse-grammar::identifier */
	/* BEGINNING OF ACTION: rassign */
	{
#line 574 "c-parser.act"

		c_current_entry = table_get_type(c_current_table, (&ZI108));
		if (c_current_entry == NULL) {
			error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
				"result assignment operation defined for unknown type '%S'", (void *) (&ZI108));
		} else if (type_get_result_assign_code(entry_get_type(c_current_entry))) {
			err_mult("result assignment", "type", (&ZI108));
			c_current_entry = NULL;
		}
		nstring_destroy(&(ZI108));
	
#line 2634 "tmp.c-parser.c"
	}
	/* END OF ACTION: rassign */
	ZR145 ();
	ZR124 ();
	/* BEGINNING OF INLINE: 182 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		CCodeP ZI168;

		ZR112 (&ZI168);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
		/* BEGINNING OF ACTION: x-rassign */
		{
#line 622 "c-parser.act"

		if (c_current_entry) {
			BoolT  errored = FALSE;
			KeyT  *key     = entry_key (c_current_entry);
			TypeTupleT tmp;

			types_init (&tmp);
			types_add_type_entry (&tmp, c_current_entry, FALSE);
			if (!types_disjoint_names(&c_saved_type)) {
				err_clash("result essignment", "parameter", "type", key, &c_saved_type);
				errored = TRUE;
			}

			if (!types_fillin_types(&c_saved_type, &tmp)) {
				err_mismatch("parameter", "result assignment", "type", key, &c_saved_type, &tmp);
				errored = TRUE;
			}

			if (!types_disjoint_names(&c_current_type)) {
				err_clash("result assignment", "result", "type", key, &c_saved_type);
				errored = TRUE;
			}

			if (!types_fillin_types(&c_current_type, &tmp)) {
				err_mismatch("result", "result assignment", "type", key, &c_saved_type, &tmp);
				errored = TRUE;
			}

			if (types_intersect(&c_saved_type, &c_current_type)) {
				err_formal_clash("result assignment", "type", key, &c_saved_type, &c_current_type);
				errored = TRUE;
			}

			types_destroy (&tmp);
			if (errored) {
				types_destroy(&c_saved_type);
				types_destroy(&c_current_type);
				c_code_deallocate((ZI168));
				c_current_entry = NULL;
			} else {
				TypeT *type = entry_get_type(c_current_entry);

				c_code_check((ZI168), FALSE, FALSE, &c_saved_type,
					&c_current_type, c_current_table);
				type_set_result_assign_code (type,  (ZI168));
			}
		} else {
			types_destroy(&c_saved_type);
			types_destroy(&c_current_type);
			c_code_deallocate((ZI168));
		}
	
#line 2708 "tmp.c-parser.c"
		}
		/* END OF ACTION: x-rassign */
		ZR221 ();
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
	    }
	    goto ZL3;
	  ZL4:;
	    {
		/* BEGINNING OF ACTION: expected-code */
		{
#line 926 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("code block");
		}
	
#line 2728 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-code */
		/* BEGINNING OF ACTION: skip-to-end-of-result-assign */
		{
#line 1103 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTERMINALS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HACTIONS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			} else if (CURRENT_TERMINAL == (C_TOK_CODE)) {
				c_code_deallocate(c_lexer_code_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 2758 "tmp.c-parser.c"
		}
		/* END OF ACTION: skip-to-end-of-result-assign */
	    }
	  ZL3:;
	}
	/* END OF INLINE: 182 */
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR218(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case (C_TOK_SEPARATOR):
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    {
	/* BEGINNING OF ACTION: expected-separator */
	{
#line 884 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("','");
		}
	
#line 2798 "tmp.c-parser.c"
	}
	/* END OF ACTION: expected-separator */
    }
}

static void
ZR147(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	NStringT ZI126;

	switch (CURRENT_TERMINAL) {
	  case (C_TOK_C_HIDENTIFIER):
	    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
	    {
#line 243 "c-parser.act"

		nstring_assign(&ZI126, c_lexer_string_value(c_current_stream));
	
#line 2821 "tmp.c-parser.c"
	    }
	    /* END OF EXTRACT: C-IDENTIFIER */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 149 */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_TYPEMARK):
		{
		    ADVANCE_LEXER;
		    /* BEGINNING OF INLINE: 150 */
		    {
			{
			    NStringT ZI143;

			    /* BEGINNING OF INLINE: c-parse-grammar::identifier */
			    {
				switch (CURRENT_TERMINAL) {
				  case (C_TOK_C_HIDENTIFIER):
				    {
					/* BEGINNING OF EXTRACT: C-IDENTIFIER */
					{
#line 243 "c-parser.act"

		nstring_assign(&ZI143, c_lexer_string_value(c_current_stream));
	
#line 2851 "tmp.c-parser.c"
					}
					/* END OF EXTRACT: C-IDENTIFIER */
					ADVANCE_LEXER;
				    }
				    break;
				  case (C_TOK_SID_HIDENTIFIER):
				    {
					/* BEGINNING OF EXTRACT: SID-IDENTIFIER */
					{
#line 247 "c-parser.act"

		nstring_assign(&ZI143, c_lexer_string_value(c_current_stream));
	
#line 2865 "tmp.c-parser.c"
					}
					/* END OF EXTRACT: SID-IDENTIFIER */
					ADVANCE_LEXER;
				    }
				    break;
				  default:
				    goto ZL4;
				}
			    }
			    /* END OF INLINE: c-parse-grammar::identifier */
			    /* BEGINNING OF INLINE: 151 */
			    {
				switch (CURRENT_TERMINAL) {
				  case (C_TOK_REFERENCE):
				    {
					ADVANCE_LEXER;
					/* BEGINNING OF ACTION: tuple-ref-type */
					{
#line 425 "c-parser.act"

		if (!types_add_typed_name(&c_current_type, c_current_table, &(ZI126),
			(&ZI143), TRUE)) {
			error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
				"type '%S' hasn't been declared", (void *) (&ZI143));
		}
		nstring_destroy(&(ZI143));
	
#line 2893 "tmp.c-parser.c"
					}
					/* END OF ACTION: tuple-ref-type */
				    }
				    break;
				  default:
				    {
					/* BEGINNING OF ACTION: tuple-type */
					{
#line 416 "c-parser.act"

		if (!types_add_typed_name(&c_current_type, c_current_table, &(ZI126),
			(&ZI143), FALSE)) {
			error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
				"type '%S' hasn't been declared", (void *) (&ZI143));
		}
		nstring_destroy(&(ZI143));
	
#line 2911 "tmp.c-parser.c"
					}
					/* END OF ACTION: tuple-type */
				    }
				    break;
				}
			    }
			    /* END OF INLINE: 151 */
			}
			goto ZL3;
		      ZL4:;
			{
			    /* BEGINNING OF ACTION: expected-identifier */
			    {
#line 872 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("identifier");
		}
	
#line 2931 "tmp.c-parser.c"
			    }
			    /* END OF ACTION: expected-identifier */
			    /* BEGINNING OF ACTION: destroy-string */
			    {
#line 959 "c-parser.act"

		nstring_destroy(&(ZI126));
	
#line 2940 "tmp.c-parser.c"
			    }
			    /* END OF ACTION: destroy-string */
			    /* BEGINNING OF ACTION: skip-to-end-of-tuple-defn */
			    {
#line 963 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_DEFINE)
			&& CURRENT_TERMINAL != (C_TOK_CODE)
			&& CURRENT_TERMINAL != (C_TOK_SEPARATOR)
			&& CURRENT_TERMINAL != (C_TOK_CLOSE_HTUPLE)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HPARAM_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HRESULT_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTERMINALS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HACTIONS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 2974 "tmp.c-parser.c"
			    }
			    /* END OF ACTION: skip-to-end-of-tuple-defn */
			}
		      ZL3:;
		    }
		    /* END OF INLINE: 150 */
		}
		break;
	      default:
		{
		    /* BEGINNING OF ACTION: tuple-name */
		    {
#line 434 "c-parser.act"

		types_add_name(&c_current_type, c_current_table, &(ZI126), FALSE);
	
#line 2991 "tmp.c-parser.c"
		    }
		    /* END OF ACTION: tuple-name */
		}
		break;
	    }
	}
	/* END OF INLINE: 149 */
    }
    return;
  ZL1:;
    {
	/* BEGINNING OF ACTION: expected-identifier */
	{
#line 872 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("identifier");
		}
	
#line 3011 "tmp.c-parser.c"
	}
	/* END OF ACTION: expected-identifier */
	/* BEGINNING OF ACTION: skip-to-end-of-tuple-defn */
	{
#line 963 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_DEFINE)
			&& CURRENT_TERMINAL != (C_TOK_CODE)
			&& CURRENT_TERMINAL != (C_TOK_SEPARATOR)
			&& CURRENT_TERMINAL != (C_TOK_CLOSE_HTUPLE)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HPARAM_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HRESULT_HASSIGN)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTERMINALS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HACTIONS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 3045 "tmp.c-parser.c"
	}
	/* END OF ACTION: skip-to-end-of-tuple-defn */
    }
}

static void
ZR221(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case (C_TOK_TERMINATOR):
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    {
	/* BEGINNING OF ACTION: expected-terminator */
	{
#line 908 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("';'");
		}
	
#line 3077 "tmp.c-parser.c"
	}
	/* END OF ACTION: expected-terminator */
    }
}

static void
ZR186(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	NStringT ZI108;

	/* BEGINNING OF INLINE: c-parse-grammar::identifier */
	{
	    switch (CURRENT_TERMINAL) {
	      case (C_TOK_C_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: C-IDENTIFIER */
		    {
#line 243 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 3103 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: C-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      case (C_TOK_SID_HIDENTIFIER):
		{
		    /* BEGINNING OF EXTRACT: SID-IDENTIFIER */
		    {
#line 247 "c-parser.act"

		nstring_assign(&ZI108, c_lexer_string_value(c_current_stream));
	
#line 3117 "tmp.c-parser.c"
		    }
		    /* END OF EXTRACT: SID-IDENTIFIER */
		    ADVANCE_LEXER;
		}
		break;
	      default:
		goto ZL1;
	    }
	}
	/* END OF INLINE: c-parse-grammar::identifier */
	/* BEGINNING OF ACTION: set-terminal */
	{
#line 644 "c-parser.act"

		c_current_entry = table_get_basic(c_current_table, (&ZI108));
		if (c_current_entry == NULL) {
			error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
				"extraction operation defined for unknown terminal '%S'", (void *) (&ZI108));
		} else {
			BasicT * basic = entry_get_basic(c_current_entry);

			if (basic_get_result_code(basic)) {
				err_mult("extraction", "terminal", (&ZI108));
				c_current_entry = NULL;
			} else if (types_equal_zero_tuple(basic_result (basic))) {
				error_posn(ERROR_SERIOUS, c_lexer_stream_name(c_current_stream), (int) c_lexer_stream_line(c_current_stream),
					"extraction operation defined for terminal '%S' that returns nothing", (void *) (&ZI108));
				c_current_entry = NULL;
			}
		}
		nstring_destroy(&(ZI108));
	
#line 3150 "tmp.c-parser.c"
	}
	/* END OF ACTION: set-terminal */
	ZR145 ();
	ZR124 ();
	/* BEGINNING OF INLINE: 189 */
	{
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    {
		CCodeP ZI168;

		ZR112 (&ZI168);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
		/* BEGINNING OF ACTION: x-set-terminal */
		{
#line 702 "c-parser.act"

		if (c_current_entry) {
			BasicT     *basic   = entry_get_basic(c_current_entry);
			TypeTupleT *result  = basic_result(basic);
			BoolT       errored = FALSE;
			KeyT       *key     = entry_key(c_current_entry);

			if (!types_disjoint_names(&c_saved_type)) {
				err_clash("extraction", "parameter", "terminal", key, &c_saved_type);
				errored = TRUE;
			}

			if (!types_equal_zero_tuple(&c_saved_type)) {
				error_posn(ERROR_SERIOUS, istream_name(&c_lexer_stream->istream), (int) istream_line(&c_lexer_stream->istream),
					"parameter type mismatch for extraction operation on "
					"terminal '%K' [%Y should be ()]",
					(void *) key, (void *) &c_saved_type);
				errored = TRUE;
			}

			if (!types_disjoint_names(&c_current_type)) {
				err_clash("extraction", "result", "terminal", key, &c_saved_type);
				errored = TRUE;
			}

			if (!types_fillin_types(&c_current_type, result)) {
				err_mismatch("result", "extraction", "terminal", key, &c_current_type, result);
				errored = TRUE;
			}

			if (types_intersect(&c_saved_type, &c_current_type)) {
				err_formal_clash("extraction", "terminal", key, &c_saved_type, &c_current_type);
				errored = TRUE;
			}

			if (errored) {
				types_destroy(&c_saved_type);
				types_destroy(&c_current_type);
				c_code_deallocate((ZI168));
				c_current_entry = NULL;
			} else {
				types_destroy(&c_saved_type);
				c_code_check((ZI168), FALSE, FALSE, NULL, &c_current_type, c_current_table);
				basic_set_result_code(basic,  (ZI168));
			}
		} else {
			types_destroy(&c_saved_type);
			types_destroy(&c_current_type);
			c_code_deallocate((ZI168));
		}
	
#line 3223 "tmp.c-parser.c"
		}
		/* END OF ACTION: x-set-terminal */
		ZR221 ();
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		    RESTORE_LEXER;
		    goto ZL4;
		}
	    }
	    goto ZL3;
	  ZL4:;
	    {
		/* BEGINNING OF ACTION: expected-code */
		{
#line 926 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("code block");
		}
	
#line 3243 "tmp.c-parser.c"
		}
		/* END OF ACTION: expected-code */
		/* BEGINNING OF ACTION: skip-to-end-of-terminal */
		{
#line 1128 "c-parser.act"

		while (CURRENT_TERMINAL != (C_TOK_EOF)
			&& CURRENT_TERMINAL != (C_TOK_TERMINATOR)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HACTIONS)
			&& CURRENT_TERMINAL != (C_TOK_BLT_HTRAILER)) {
			if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
				nstring_destroy(c_lexer_string_value(c_current_stream));
			} else if (CURRENT_TERMINAL == (C_TOK_CODE)) {
				c_code_deallocate(c_lexer_code_value(c_current_stream));
			}
			ADVANCE_LEXER;
		}

		if (CURRENT_TERMINAL == (C_TOK_SID_HIDENTIFIER) || CURRENT_TERMINAL == (C_TOK_C_HIDENTIFIER)) {
			nstring_destroy(c_lexer_string_value(c_current_stream));
		}

		if (CURRENT_TERMINAL != (C_TOK_EOF)) {
			ADVANCE_LEXER;
		}

		c_propagating_error = TRUE;
	
#line 3272 "tmp.c-parser.c"
		}
		/* END OF ACTION: skip-to-end-of-terminal */
	    }
	  ZL3:;
	}
	/* END OF INLINE: 189 */
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR128(void)
{
  ZL2_128:;
    switch (CURRENT_TERMINAL) {
      case (C_TOK_SID_HIDENTIFIER): case (C_TOK_C_HIDENTIFIER):
	{
	    ZR130 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    /* BEGINNING OF ACTION: skip-recover */
	    {
#line 1177 "c-parser.act"

		c_propagating_error = FALSE;
	
#line 3304 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: skip-recover */
	    /* BEGINNING OF INLINE: c-parse-grammar::map-list */
	    goto ZL2_128;
	    /* END OF INLINE: c-parse-grammar::map-list */
	}
	/*UNREACHED*/
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR145(void)
{
    switch (CURRENT_TERMINAL) {
      case (C_TOK_TYPEMARK):
	{
	    ADVANCE_LEXER;
	    ZR157 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    /* BEGINNING OF ACTION: save-tuple */
	    {
#line 404 "c-parser.act"

		types_assign(&c_saved_type, &c_current_type);
	
#line 3341 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: save-tuple */
	    ZR161 ();
	    ZR157 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	}
	break;
      default:
	{
	    /* BEGINNING OF ACTION: null-type */
	    {
#line 409 "c-parser.act"

		types_init(&c_saved_type);
		types_init(&c_current_type);
	
#line 3361 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: null-type */
	}
	break;
      case (ERROR_TERMINAL):
	return;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR162(void)
{
  ZL2_162:;
    switch (CURRENT_TERMINAL) {
      case (C_TOK_SID_HIDENTIFIER): case (C_TOK_C_HIDENTIFIER):
	{
	    ZR164 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    /* BEGINNING OF ACTION: skip-recover */
	    {
#line 1177 "c-parser.act"

		c_propagating_error = FALSE;
	
#line 3393 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: skip-recover */
	    /* BEGINNING OF INLINE: c-parse-grammar::assignment-list */
	    goto ZL2_162;
	    /* END OF INLINE: c-parse-grammar::assignment-list */
	}
	/*UNREACHED*/
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR170(void)
{
  ZL2_170:;
    switch (CURRENT_TERMINAL) {
      case (C_TOK_SID_HIDENTIFIER): case (C_TOK_C_HIDENTIFIER):
	{
	    ZR172 ();
	    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	    /* BEGINNING OF ACTION: skip-recover */
	    {
#line 1177 "c-parser.act"

		c_propagating_error = FALSE;
	
#line 3430 "tmp.c-parser.c"
	    }
	    /* END OF ACTION: skip-recover */
	    /* BEGINNING OF INLINE: c-parse-grammar::param-assign-list */
	    goto ZL2_170;
	    /* END OF INLINE: c-parse-grammar::param-assign-list */
	}
	/*UNREACHED*/
      case (ERROR_TERMINAL):
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER ((ERROR_TERMINAL));
    return;
}

static void
ZR124(void)
{
    if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case (C_TOK_DEFINE):
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    {
	/* BEGINNING OF ACTION: expected-define */
	{
#line 920 "c-parser.act"

		if (!c_propagating_error) {
			err_expected("'='");
		}
	
#line 3475 "tmp.c-parser.c"
	}
	/* END OF ACTION: expected-define */
    }
}

/* BEGINNING OF TRAILER */

#line 1187 "c-parser.act"


#line 3486 "tmp.c-parser.c"

/* END OF FILE */
