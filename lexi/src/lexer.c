/* $Id$ */

/*
 * Copyright 2002-2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

/*
 * Automatically generated by lexi version 2.0
 */

#include "lexer.h"



#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <shared/error.h>
#include <shared/xalloc.h>

#include "lexer.h"
#include "syntax.h"
/*
	THE CURRENT LEXER STATE
*/
struct lexi_state lexer_state;

/*
    TOKEN BUFFER

    This buffer is used by read_token to hold the values of identifiers
    and strings.
*/
char token_buff [2000];
char *token_end = token_buff + sizeof(token_buff);
static char* token_current;
unsigned int number_buffer;

/*
    CURRENT TOKEN

    These variables are used by the parser to hold the current and former
    lexical tokens.
*/

int crt_lex_token;
int saved_lex_token;

#define lexi_unknown_token	lex_unknown

/*
    GET THE NEXT CHARACTER

    This routine reads the next character, either from the pending buffer
    or from the input file.
*/

static int
lexi_getchar(struct lexi_state* state)
{
    int c;
    c = fgetc(state->input);
    if (c == '\n')crt_line_no++;
    if (c == EOF) return LEXI_EOF;
    c &= 0xff;
    return c;
}

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

int lexi_readchar(struct lexi_state *state) {
	if (state->buffer_index) {
		return lexi_pop(state);
	}

	return lexi_getchar(state);
}
void lexi_push(struct lexi_state *state, const int c) {
	assert(state);
	assert(state->buffer_index < sizeof state->buffer / sizeof *state->buffer);
	state->buffer[state->buffer_index++] = c;
}

int lexi_pop(struct lexi_state *state) {
	assert(state);
	assert(state->buffer_index > 0);
	return state->buffer[--state->buffer_index];
}

void lexi_flush(struct lexi_state *state) {
	state->buffer_index = 0;
}


/* LOOKUP TABLE */

typedef uint8_t lookup_type;
static lookup_type lookup_tab[] = {
	   0,    0,    0,    0,    0,    0,    0,    0,    0, 0x10, 0x10,    0, 
	   0, 0x10,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0, 0x10,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,  0x1,    0,    0, 
	 0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,    0,    0, 
	   0,    0,    0,    0,    0,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb, 
	 0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb, 
	 0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,    0,    0,    0,    0,  0xb, 
	   0,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb, 
	 0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb, 
	 0xb,  0xb,  0xb,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0
};

bool lexi_group(enum lexi_groups group, int c) {
	return lookup_tab[c] & group;
}


#include <string.h>
int lexi_keyword(const char *identifier, int notfound) {
	if (0 == strcmp(identifier, "ACTION")) return lex_action_Hkw;
	if (0 == strcmp(identifier, "DEFAULT")) return lex_default;
	if (0 == strcmp(identifier, "GROUP")) return lex_group;
	if (0 == strcmp(identifier, "KEYWORD")) return lex_keyword;
	if (0 == strcmp(identifier, "MAPPING")) return lex_mapping;
	if (0 == strcmp(identifier, "TOKEN")) return lex_token;
	if (0 == strcmp(identifier, "TYPE")) return lex_type_Hkw;
	if (0 == strcmp(identifier, "ZONE")) return lex_zone;
	if (0 == strcmp(identifier, "white")) return lex_white;
	return notfound;
}
/* PRE-PASS ANALYSERS */

void lexi_init(struct lexi_state *state, FILE_P input) {
	state->zone = lexi_read_token;
	state->buffer_index = 0;
	state->input = input;
}
/* ZONES PASS ANALYSER PROTOTYPES*/

static int lexi_read_token_sididentifierzone(struct lexi_state *state);
static int lexi_read_token_identifierzone(struct lexi_state *state);
static int lexi_read_token_stringzone(struct lexi_state *state);
static void lexi_read_token_line_comment(struct lexi_state *state);
static void lexi_read_token_comment(struct lexi_state *state);
static int lexi_read_token_arg_char_nb_zone(struct lexi_state *state);
/* MAIN PASS ANALYSERS */


/* MAIN PASS ANALYSER for sididentifierzone */
static int
lexi_read_token_sididentifierzone(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (!lexi_group(lexi_group_alphanumhyphen, c0)) {
			lexi_push(state, c0);
			/* ACTION <finalize_token_buffer> */
			{

       	if(token_current==token_end) {
		error(ERROR_FATAL, "Buffer overflow: trailing 0");
	       *(token_end-1) = 0;		
	} else {
	       *token_current++ = 0;	
	}
			}
			/* END ACTION <finalize_token_buffer> */
			return lex_sid_Hidentifier;
		}

		/* DEFAULT */
		/* ACTION <push_token_buffer> */
		{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
		}
		/* END ACTION <push_token_buffer> */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for identifierzone */
static int
lexi_read_token_identifierzone(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (!lexi_group(lexi_group_alphanum, c0)) {
			lexi_push(state, c0);
			/* ACTION <finalize_token_buffer> */
			{

       	if(token_current==token_end) {
		error(ERROR_FATAL, "Buffer overflow: trailing 0");
	       *(token_end-1) = 0;		
	} else {
	       *token_current++ = 0;	
	}
			}
			/* END ACTION <finalize_token_buffer> */
			/* ACTION <token_keyword_match> */
			{
				int ZT1;

	ZT1 = lexi_keyword(token_buff, lex_identifier);
				return ZT1;
			}
			/* END ACTION <token_keyword_match> */
		}

		/* DEFAULT */
		/* ACTION <push_token_buffer> */
		{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
		}
		/* END ACTION <push_token_buffer> */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for stringzone */
static int
lexi_read_token_stringzone(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		switch (c0) {
		case '\n': {
				/* ACTION <E_newline_in_string> */
				{

	error(ERROR_SERIOUS, "Unexpected newline in string");
				}
				/* END ACTION <E_newline_in_string> */
				goto start;	/* leaf */
			}

		case LEXI_EOF: {
				/* ACTION <E_eof_in_string> */
				{

	error(ERROR_SERIOUS, "Unexpected eof in string");
				}
				/* END ACTION <E_eof_in_string> */
				goto start;	/* leaf */
			}

		case '\\': {
				int c1 = lexi_readchar(state);
				switch (c1) {
				case '\\': {
						/* ACTION <push_token_buffer> */
						{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
						}
						/* END ACTION <push_token_buffer> */
						/* ACTION <push_token_buffer> */
						{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
						}
						/* END ACTION <push_token_buffer> */
						goto start;	/* leaf */
					}

				case '"': {
						/* ACTION <push_token_buffer> */
						{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c1;
						}
						/* END ACTION <push_token_buffer> */
						goto start;	/* leaf */
					}

				}
				lexi_push(state, c1);
			}
			break;

		case '"': {
				/* ACTION <finalize_token_buffer> */
				{

       	if(token_current==token_end) {
		error(ERROR_FATAL, "Buffer overflow: trailing 0");
	       *(token_end-1) = 0;		
	} else {
	       *token_current++ = 0;	
	}
				}
				/* END ACTION <finalize_token_buffer> */
				return lex_string;
			}

		}

		/* DEFAULT */
		/* ACTION <push_token_buffer> */
		{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
		}
		/* END ACTION <push_token_buffer> */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for line_comment */
static void
lexi_read_token_line_comment(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (c0 == '\n') {
			return;
		}

		/* DEFAULT */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for comment */
static void
lexi_read_token_comment(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (c0 == '*') {
			int c1 = lexi_readchar(state);
			if (c1 == '/') {
				return;
			}
			lexi_push(state, c1);
		}

		/* DEFAULT */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for arg_char_nb_zone */
static int
lexi_read_token_arg_char_nb_zone(struct lexi_state *state)
{
	start: {
		int c0 = lexi_readchar(state);
		if (lexi_group(lexi_group_white, c0)) goto start;
		if (!lexi_group(lexi_group_digit, c0)) {
			lexi_push(state, c0);
			return lex_arg_Hchar_Hnb;
		}

		/* DEFAULT */
		/* ACTION <number_buffer_push_digit> */
		{

	number_buffer *= 10;
	number_buffer += c0 - '0';
		}
		/* END ACTION <number_buffer_push_digit> */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for the global zone */
int
lexi_read_token(struct lexi_state *state)
{
	if (state->zone != lexi_read_token)
		return state->zone(state);
	start: {
		int c0 = lexi_readchar(state);
		if (lexi_group(lexi_group_white, c0)) goto start;
		switch (c0) {
		case '!': {
				return lex_arg_Hignore;
			}

		case '"': {
				/* ACTION <init_token_buffer> */
				{

	token_current=token_buff;
				}
				/* END ACTION <init_token_buffer> */
				return lexi_read_token_stringzone(state);
			}

		case '/': {
				int c1 = lexi_readchar(state);
				switch (c1) {
				case '/': {
						lexi_read_token_line_comment(state);
						goto start;	/* pure function */
					}

				case '*': {
						lexi_read_token_comment(state);
						goto start;	/* pure function */
					}

				}
				lexi_push(state, c1);
			}
			break;

		case '#': {
				int c1 = lexi_readchar(state);
				switch (c1) {
				case '*': {
						return lex_arg_Hchar_Hstring;
					}

				case 'n': {
						return lex_arg_Hnb_Hof_Hchars;
					}

				}
				if (lexi_group(lexi_group_digit, c1)) {
					/* ACTION <number_buffer_init> */
					{

	number_buffer = 0;
					}
					/* END ACTION <number_buffer_init> */
					/* ACTION <number_buffer_push_digit> */
					{

	number_buffer *= 10;
	number_buffer += c1 - '0';
					}
					/* END ACTION <number_buffer_push_digit> */
					return lexi_read_token_arg_char_nb_zone(state);
				}
				lexi_push(state, c1);
			}
			break;

		case LEXI_EOF: {
				return lex_eof;
			}

		case ',': {
				return lex_comma;
			}

		case '$': {
				int c1 = lexi_readchar(state);
				if (c1 == '$') {
					return lex_nothing_Hmarker;
				}
				if (lexi_group(lexi_group_alpha, c1)) {
					/* ACTION <init_token_buffer> */
					{

	token_current=token_buff;
					}
					/* END ACTION <init_token_buffer> */
					/* ACTION <push_token_buffer> */
					{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c1;
					}
					/* END ACTION <push_token_buffer> */
					return lexi_read_token_sididentifierzone(state);
				}
				lexi_push(state, c1);
				return lex_arg_Hreturn_Hterminal;
			}

		case '[': {
				int c1 = lexi_readchar(state);
				if (c1 == '.') {
					int c2 = lexi_readchar(state);
					if (c2 == '.') {
						int c3 = lexi_readchar(state);
						if (c3 == '.') {
							int c4 = lexi_readchar(state);
							switch (c4) {
							case ')': {
									return lex_range_Hclosed_Hopen;
								}

							case ']': {
									return lex_range_Hclosed_Hclosed;
								}

							}
							lexi_push(state, c4);
						}
						lexi_push(state, c3);
					}
					lexi_push(state, c2);
				}
				lexi_push(state, c1);
			}
			break;

		case '.': {
				int c1 = lexi_readchar(state);
				if (c1 == '.') {
					int c2 = lexi_readchar(state);
					if (c2 == '.') {
						return lex_range;
					}
					lexi_push(state, c2);
				}
				lexi_push(state, c1);
			}
			break;

		case '+': {
				return lex_plus;
			}

		case '=': {
				return lex_equal;
			}

		case ';': {
				return lex_semicolon;
			}

		case '&': {
				return lex_reference;
			}

		case ':': {
				return lex_colon;
			}

		case '-': {
				int c1 = lexi_readchar(state);
				if (c1 == '>') {
					return lex_arrow;
				}
				lexi_push(state, c1);
			}
			break;

		case '}': {
				return lex_close_Hbrace;
			}

		case '{': {
				int c1 = lexi_readchar(state);
				switch (c1) {
				case '0': {
						int c2 = lexi_readchar(state);
						if (c2 == '-') {
							int c3 = lexi_readchar(state);
							if (c3 == '9') {
								int c4 = lexi_readchar(state);
								if (c4 == '}') {
									return lex_digit;
								}
								lexi_push(state, c4);
							}
							lexi_push(state, c3);
						}
						lexi_push(state, c2);
					}
					break;

				case 'a': {
						int c2 = lexi_readchar(state);
						if (c2 == '-') {
							int c3 = lexi_readchar(state);
							if (c3 == 'z') {
								int c4 = lexi_readchar(state);
								if (c4 == '}') {
									return lex_lower;
								}
								lexi_push(state, c4);
							}
							lexi_push(state, c3);
						}
						lexi_push(state, c2);
					}
					break;

				case 'A': {
						int c2 = lexi_readchar(state);
						if (c2 == '-') {
							int c3 = lexi_readchar(state);
							if (c3 == 'Z') {
								int c4 = lexi_readchar(state);
								if (c4 == '}') {
									return lex_upper;
								}
								lexi_push(state, c4);
							}
							lexi_push(state, c3);
						}
						lexi_push(state, c2);
					}
					break;

				}
				lexi_push(state, c1);
				return lex_open_Hbrace;
			}

		case '>': {
				return lex_end_Haction;
			}

		case '<': {
				return lex_begin_Haction;
			}

		case ')': {
				return lex_close;
			}

		case '(': {
				return lex_open;
			}

		}
		if (lexi_group(lexi_group_alpha, c0)) {
			/* ACTION <init_token_buffer> */
			{

	token_current=token_buff;
			}
			/* END ACTION <init_token_buffer> */
			/* ACTION <push_token_buffer> */
			{

       	if(token_current==token_end-1)
		error(ERROR_FATAL, "Buffer overflow");
	else 
	       *token_current++ = c0;
			}
			/* END ACTION <push_token_buffer> */
			return lexi_read_token_identifierzone(state);
		}

		/* DEFAULT */
		return lexi_unknown_token;
	}
}
