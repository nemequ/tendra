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


/*** rule-names.c --- Recompute alternative names.
 *
 ** Author: Steve Folkes <smf@hermes.mod.uk>
 *
 *** Commentary:
 *
 * This file implements the functions that recompute the names defined in each
 * alternative of a rule (including the exception handler alternative).
 *
 *** Change Log:*/

/****************************************************************************/

#include "rule.h"
#include "action.h"
#include "basic.h"
#include "name.h"
#include "type.h"
#include "types.h"

/*--------------------------------------------------------------------------*/

static void
rule_recompute_alt_names_2 PROTO_N ((alt, predicate_id))
			   PROTO_T (AltP   alt X
				    EntryP predicate_id)
{
    TypeTupleP names = alt_names (alt);
    ItemP      item;

    types_destroy (names);
    types_init (names);
    for (item = alt_item_head (alt); item; item = item_next (item)) {
	types_add_new_names (names, item_result (item), predicate_id);
    }
}

static void
rule_recompute_alt_names_1 PROTO_N ((rule, predicate_id))
			   PROTO_T (RuleP  rule X
				    EntryP predicate_id)
{
    AltP alt;

    if ((alt = rule_get_handler (rule)) != NIL (AltP)) {
	rule_recompute_alt_names_2 (alt, predicate_id);
    }
    for (alt = rule_alt_head (rule); alt; alt = alt_next (alt)) {
	rule_recompute_alt_names_2 (alt, predicate_id);
    }
}

/*--------------------------------------------------------------------------*/

void
rule_recompute_alt_names PROTO_N ((entry, gclosure))
			 PROTO_T (EntryP   entry X
				  GenericP gclosure)
{
    if (entry_is_rule (entry)) {
	RuleP  rule         = entry_get_rule (entry);
	EntryP predicate_id = (EntryP) gclosure;

	rule_recompute_alt_names_1 (rule, predicate_id);
    }
}

/*
 * Local variables(smf):
 * eval: (include::add-path-entry "../os-interface" "../library")
 * eval: (include::add-path-entry "../generated")
 * end:
**/
