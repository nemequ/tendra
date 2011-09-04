
/*
 * Automatically generated by lexi version 2.0
 */

#include "c-lexi_lexer.h"



	#include <shared/error.h>

	#include "c-lexer.h"

#include <assert.h>
int c_lexi_readchar(struct c_lexi_state *state) {
	if (state->buffer_index) {
		return c_lexi_pop(state);
	}

	return c_lexi_getchar();
}
void c_lexi_push(struct c_lexi_state *state, const int c) {
	assert(state);
	assert(state->buffer_index < sizeof state->buffer / sizeof *state->buffer);
	state->buffer[state->buffer_index++] = c;
}

int c_lexi_pop(struct c_lexi_state *state) {
	assert(state);
	assert(state->buffer_index > 0);
	return state->buffer[--state->buffer_index];
}

void c_lexi_flush(struct c_lexi_state *state) {
	state->buffer_index = 0;
}


/* LOOKUP TABLE */

typedef unsigned char lookup_type;
static lookup_type lookup_tab[] = {
	   0,    0,    0,    0,    0,    0,    0,    0,    0,  0x1,  0x1,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
	   0,    0,    0,    0,    0,    0,    0,    0,  0x1,    0,    0,    0, 
	   0,  0x4,    0,    0,    0,    0,    0,    0,    0, 0x28,    0,    0, 
	0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,    0,    0, 
	   0,    0,    0,    0,    0, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 
	0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 
	0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,    0,    0,    0,    0, 0x62, 
	   0, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 
	0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 0x7a, 
	0x7a, 0x7a, 0x7a,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
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

int c_lexi_group(enum c_lexi_groups group, int c) {
	return lookup_tab[c] & group;
}


/* PRE-PASS ANALYSERS */

void c_lexi_init(struct c_lexi_state *state) {
	state->zone = c_lexi_read_token;
	state->buffer_index = 0;
}
/* ZONES PASS ANALYSER PROTOTYPES*/

static int c_lexi_read_token_act(struct c_lexi_state *state);
static void c_lexi_read_token_singleline_comment(struct c_lexi_state *state);
static void c_lexi_read_token_bracketed_comment(struct c_lexi_state *state);
/* MAIN PASS ANALYSERS */


/* MAIN PASS ANALYSER for act */
static int
c_lexi_read_token_act(struct c_lexi_state *state)
{
	start: {
		int c0 = c_lexi_readchar(state);
		switch (c0) {
		case LEXI_EOF: {
				return C_TOK_ACT_HEOF;
			}

		case '@': {
				int c1 = c_lexi_readchar(state);
				switch (c1) {
				case '$': {
						int c2 = c_lexi_readchar(state);
						if (c_lexi_group(c_lexi_group_identstart, c2)) {
							/* ACTION <support_read_id_global> */
							{
								int ZT1;

       ZT1=c_lexer_support_read_id(c2, C_TOK_ACT_HBASIC, c_lexi_group_identbody);
								return ZT1;
							}
							/* END ACTION <support_read_id_global> */
							goto start;	/* leaf */
						}
						c_lexi_push(state, c2);
					}
					break;

				case '=': {
						int c2 = c_lexi_readchar(state);
						if (c_lexi_group(c_lexi_group_identstart, c2)) {
							/* ACTION <support_read_id_act> */
							{
								int ZT1;

       ZT1=c_lexer_support_read_id(c2, C_TOK_ACT_HMODIFIABLE, c_lexi_group_act_identbody);
								return ZT1;
							}
							/* END ACTION <support_read_id_act> */
							goto start;	/* leaf */
						}
						c_lexi_push(state, c2);
					}
					break;

				case '&': {
						int c2 = c_lexi_readchar(state);
						if (c_lexi_group(c_lexi_group_identstart, c2)) {
							/* ACTION <support_read_id_act> */
							{
								int ZT1;

       ZT1=c_lexer_support_read_id(c2, C_TOK_ACT_HREFERENCE, c_lexi_group_act_identbody);
								return ZT1;
							}
							/* END ACTION <support_read_id_act> */
							goto start;	/* leaf */
						}
						c_lexi_push(state, c2);
					}
					break;

				case ':': {
						int c2 = c_lexi_readchar(state);
						if (c_lexi_group(c_lexi_group_identstart, c2)) {
							/* ACTION <support_read_id_act> */
							{
								int ZT1;

       ZT1=c_lexer_support_read_id(c2, C_TOK_ACT_HLABEL, c_lexi_group_act_identbody);
								return ZT1;
							}
							/* END ACTION <support_read_id_act> */
							goto start;	/* leaf */
						}
						c_lexi_push(state, c2);
					}
					break;

				case '>': {
						return C_TOK_ACT_HADVANCE;
					}

				case '.': {
						return C_TOK_ACT_HTERMINAL;
					}

				case '!': {
						return C_TOK_ACT_HEXCEPTION;
					}

				case '@': {
						return C_TOK_ACT_HAT;
					}

				case '}': {
						state->zone = c_lexi_read_token;
						return C_TOK_ACT_HCODEEND;
					}

				}
				if (c_lexi_group(c_lexi_group_identstart, c1)) {
					/* ACTION <support_read_id_act> */
					{
						int ZT1;

       ZT1=c_lexer_support_read_id(c1, C_TOK_ACT_HIDENTIFIER, c_lexi_group_act_identbody);
						return ZT1;
					}
					/* END ACTION <support_read_id_act> */
					goto start;	/* leaf */
				}
				c_lexi_push(state, c1);
			}
			break;

		}

		/* DEFAULT */
		/* ACTION <act_read_string> */
		{
			int ZT1;

       ZT1=c_lexer_act_read_string(c0);
			return ZT1;
		}
		/* END ACTION <act_read_string> */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for singleline_comment */
static void
c_lexi_read_token_singleline_comment(struct c_lexi_state *state)
{
	start: {
		int c0 = c_lexi_readchar(state);
		switch (c0) {
		case LEXI_EOF: {
				/* ACTION <eof_in_comment> */
				{

	error_posn(ERROR_FATAL, istream_name(&c_lexer_stream->istream), (int) istream_line(&c_lexer_stream->istream),
		"end of file in comment");
				}
				/* END ACTION <eof_in_comment> */
				goto start;	/* leaf */
			}

		case '\n': {
				return;
			}

		}

		/* DEFAULT */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for bracketed_comment */
static void
c_lexi_read_token_bracketed_comment(struct c_lexi_state *state)
{
	start: {
		int c0 = c_lexi_readchar(state);
		switch (c0) {
		case LEXI_EOF: {
				/* ACTION <eof_in_comment> */
				{

	error_posn(ERROR_FATAL, istream_name(&c_lexer_stream->istream), (int) istream_line(&c_lexer_stream->istream),
		"end of file in comment");
				}
				/* END ACTION <eof_in_comment> */
				goto start;	/* leaf */
			}

		case '*': {
				int c1 = c_lexi_readchar(state);
				if (c1 == '/') {
					return;
				}
				c_lexi_push(state, c1);
			}
			break;

		}

		/* DEFAULT */
		goto start; /* DEFAULT */
	}
}

/* MAIN PASS ANALYSER for the global zone */
int
c_lexi_read_token(struct c_lexi_state *state)
{
	if (state->zone != c_lexi_read_token)
		return state->zone(state);
	start: {
		int c0 = c_lexi_readchar(state);
		if (c_lexi_group(c_lexi_group_white, c0)) goto start;
		switch (c0) {
		case '@': {
				int c1 = c_lexi_readchar(state);
				if (c1 == '{') {
					state->zone = c_lexi_read_token_act;
					return C_TOK_ACT_HCODESTART;
				}
				c_lexi_push(state, c1);
			}
			break;

		case '/': {
				int c1 = c_lexi_readchar(state);
				switch (c1) {
				case '/': {
						c_lexi_read_token_singleline_comment(state);
						goto start;	/* pure function */
					}

				case '*': {
						c_lexi_read_token_bracketed_comment(state);
						goto start;	/* pure function */
					}

				}
				c_lexi_push(state, c1);
			}
			break;

		case '-': {
				int c1 = c_lexi_readchar(state);
				if (c1 == '>') {
					return C_TOK_ARROW;
				}
				c_lexi_push(state, c1);
			}
			break;

		case ')': {
				return C_TOK_CLOSE_HTUPLE;
			}

		case '(': {
				return C_TOK_OPEN_HTUPLE;
			}

		case '>': {
				return C_TOK_END_HACTION;
			}

		case '<': {
				return C_TOK_BEGIN_HACTION;
			}

		case ';': {
				return C_TOK_TERMINATOR;
			}

		case '&': {
				return C_TOK_REFERENCE;
			}

		case ',': {
				return C_TOK_SEPARATOR;
			}

		case '=': {
				return C_TOK_DEFINE;
			}

		case ':': {
				return C_TOK_TYPEMARK;
			}

		case LEXI_EOF: {
				return C_TOK_EOF;
			}

		}
		if (c_lexi_group(c_lexi_group_builtindlmt, c0)) {
			int c1 = c_lexi_readchar(state);
			if (c_lexi_group(c_lexi_group_builtinstart, c1)) {
				/* ACTION <read_builtin> */
				{
					int ZT1;

       ZT1=c_lexer_read_builtin(c1);
					return ZT1;
				}
				/* END ACTION <read_builtin> */
				goto start;	/* leaf */
			}
			c_lexi_push(state, c1);
		} else if (c_lexi_group(c_lexi_group_identstart, c0)) {
			/* ACTION <support_read_id_global> */
			{
				int ZT1;

       ZT1=c_lexer_support_read_id(c0, C_TOK_C_HIDENTIFIER, c_lexi_group_identbody);
				return ZT1;
			}
			/* END ACTION <support_read_id_global> */
			goto start;	/* leaf */
		}

		/* DEFAULT */
		/* ACTION <unknown_token> */
		{
			int ZT1;

       ZT1=c_lexer_unknown_token(c0);
			return ZT1;
		}
		/* END ACTION <unknown_token> */
		goto start; /* DEFAULT */
	}
}


