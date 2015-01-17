/* $Id$ */

/*
 * Copyright 2011-2012, The TenDRA Project.
 * Copyright 1996, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */


#include <construct/exp.h>
#include <construct/shape.h>
#include <construct/tags.h>
#include <construct/is_worth.h>
#include <construct/reg_result.h>

#include <main/flags.h>

#include <refactor/optimise.h>

#include "assembler.h"
#include "tests.h"

#ifndef tdf3
#include "68k_globals.h"
#endif

bool
reused_parameter(exp e)
{
	bool reused = 0;
	exp def, ident_exp;
	def = son(e);
	if (name(def) == name_tag) {
		ident_exp = son(def);
		if (! isvar(ident_exp)) {
			/* This an obtain_tag of a parameter */
			if (name(son(ident_exp)) == formal_callee_tag) {
				reused = cur_proc_use_same_callees;
			}
			else {
				/* caller parameter */
				reused = cur_proc_has_tail_call;
			}
		}
	}
	return reused;
}

/*
    This routine returns 1 if the expression e can be put into a register.
    It has to have its visible flag false, and to be of a suitable shape.
*/

bool
regable(exp e)
{
	shape sha;
	char n;
	long sz;

	if (isvis(e)) {
		return 0;
	}

	sha = sh(son(e));
	n = name(sha);
	if (n == realhd || n == doublehd) {
		return 1;
	}

	sz = shape_size(sha);

	return n != cpdhd && n != nofhd && sz <= 32;
}


/*
    This routine returns 1 if e has no side effects.
*/

bool
no_side(exp e)
{
	int n = name(e);
	if (n == ident_tag) {
		return no_side(son(e)) && (no_side(bro(son(e))));
	}
	return is_a(n) || n == test_tag || n == ass_tag || n == testbit_tag;
}

    char n;

/*
    Can the expression e be pushed directly onto the stack when it is
    the parameter of a procedure?
*/

bool
push_arg(exp e)
{
	unsigned char n = name(e);

	if (is_a(n)) {
		return 1;
	}
	if (n == apply_tag || n == apply_general_tag) {
		return reg_result(sh(e));
	}
	if (n == ident_tag) {
		return push_arg(son(e)) && push_arg(bro(son(e)));
	}
	return 0;
}


#if 0

/*
    No longer used.
*/


#ifndef PTR_VOID_MIN
#define PTR_VOID_MIN	10
#endif

bool
is_ptr_void(shape sha)
{
	bool go;
	int ptrs = 0;
	exp t = son(sha);
	if (t == NULL) {
		return 0;
	}
	do {
		go = (last(t) ? 0 : 1);
		if (name(sh(t)) != ptrhd) {
			return 0;
		}
		ptrs++;
		t = bro(t);
	} while (go);
	if (ptrs < PTR_VOID_MIN) {
		return 0;
	}
#ifdef PTR_VOID_MAX
	if (ptrs > PTR_VOID_MAX) {
		return 0;
	}
#endif
	return 1;
}

#endif


/*
    This routine is designed to test whether a given shape is compound,
    and thus likely to cause problems when it is the parameter of a
    procedure.
*/

bool
cpd_param(shape sha)
{
	char n = name(sha);
	if (cconv != CCONV_HP || n == bitfhd) {
		long sz = shape_size(sha);
		if (sz <= 32) {
			return 0;
		}
	}
	return n == cpdhd || n == nofhd || n == bitfhd || n == s64hd ||
		n == u64hd;
}


/*
    This routine returns 1 if sha involves an array.
*/

bool
varsize(shape sha)
{
	return name(sha) == nofhd ? 1 : 0;
}

#if 0
Use is_signed macro instead

/*
    This routine returns 1 if the integer variety shape sha is signed
    and 0 otherwise.
*/

bool
issigned(shape sha)
{
	char n = name(sha);
	if (n == ucharhd || n == uwordhd || n == ulonghd) {
		return 0;
	}
	return 1;
}
#endif

int
check_anyway(exp e)
{
#ifndef tdf3
	return 0;
#else
	if (optim & OPTIM_INLINE_EXPS) {
		setismarked(e);
		if (no(e) > 2) {
			return 1;
		}
	}
	return 0;
#endif
}


/*
    IS IT WORTH EXTRACTING A CONSTANT?
*/

int
is_worth(exp c)
{
	unsigned char cnam = name(c);
	return (!is_o(cnam) && cnam != clear_tag) ||
		/* ignore simple things unless ... */
		(cnam == cont_tag && name(son(c)) == cont_tag &&
		 name(son(son(c))) == name_tag) ||
		(cnam == name_tag && isparam(son(c)) && !isvar(son(c)) &&
		 shape_size(sh(c)) <= 32 && name(sh(c)) != shrealhd);
}
