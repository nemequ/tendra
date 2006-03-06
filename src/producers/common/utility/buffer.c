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


#include "config.h"
#if FS_STDARG
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include "c_types.h"
#include "error.h"
#include "buffer.h"
#include "ustring.h"
#include "xalloc.h"


/*
    FREE A BUFFER

    This routine frees the contents of the buffer bf.
*/

void free_buffer
    PROTO_N ( ( bf ) )
    PROTO_T ( BUFFER *bf )
{
    xfree_nof ( bf->start ) ;
    bf->start = NULL ;
    bf->posn = NULL ;
    bf->end = NULL ;
    return ;
}


/*
    CLEAR A BUFFER

    This routine sets the current position of the buffer bf to the start
    of the buffer and sets the buffer file to f.  It returns bf.
*/

BUFFER *clear_buffer
    PROTO_N ( ( bf, f ) )
    PROTO_T ( BUFFER *bf X FILE *f )
{
    bf->posn = bf->start ;
    bf->file = f ;
    return ( bf ) ;
}


/*
    OUTPUT THE CONTENTS OF A BUFFER

    This routine outputs the content of the buffer bf to the associated
    file and resets the position to the start of the buffer.  If fl is
    true then the file is flushed.
*/

void output_buffer
    PROTO_N ( ( bf, fl ) )
    PROTO_T ( BUFFER *bf X int fl )
{
    FILE *f = bf->file ;
    if ( f ) {
	string s = bf->start ;
	size_t n = ( size_t ) ( bf->posn - s ) ;
	if ( n ) {
	    IGNORE fwrite ( ( gen_ptr ) s, sizeof ( character ), n, f ) ;
	    if ( fl ) IGNORE fflush_v ( f ) ;
	    bf->posn = s ;
	}
    }
    return ;
}


/*
    EXTEND A BUFFER

    This routine extends the buffer bf.  s gives a pointer into the buffer,
    the corresponding position in the extended buffer is returned.
*/

string extend_buffer
    PROTO_N ( ( bf, s ) )
    PROTO_T ( BUFFER *bf X string s )
{
    string p = bf->start ;
    gen_size m = ( gen_size ) ( s - p ) ;
    gen_size n = ( gen_size ) ( bf->end - p ) + 500 ;
    p = xrealloc_nof ( p, character, n + 12 ) ;
    bf->start = p ;
    bf->end = p + n ;
    return ( p + m ) ;
}


/*
    MAKE SPACE IN A BUFFER

    This routine makes space for at least m + 1 characters in the buffer
    bf following the position s.  It returns the corresponding position
    in the extended buffer.
*/

string stretch_buffer
    PROTO_N ( ( bf, s, m ) )
    PROTO_T ( BUFFER *bf X string s X gen_size m )
{
    gen_size n = ( gen_size ) ( bf->end - s ) ;
    while ( m >= n ) {
	s = extend_buffer ( bf, s ) ;
	n = ( gen_size ) ( bf->end - s ) ;
    }
    return ( s ) ;
}


/*
    ADD A CHARACTER TO A BUFFER

    This routine adds the character c to the buffer bf.  Note that bf will
    not necessarily be null terminated after this routine.
*/

void bfputc
    PROTO_N ( ( bf, c ) )
    PROTO_T ( BUFFER *bf X int c )
{
    string p = bf->posn ;
    if ( p == bf->end ) p = extend_buffer ( bf, p ) ;
    *p = ( character ) c ;
    bf->posn = p + 1 ;
    return ;
}


/*
    ADD A STRING TO A BUFFER

    This routine adds the string s to the buffer bf.  Note that this
    guarantees that bf will be null terminated.
*/

void bfputs
    PROTO_N ( ( bf, s ) )
    PROTO_T ( BUFFER *bf X string s )
{
    gen_size m = ( gen_size ) ustrlen ( s ) ;
    string p = stretch_buffer ( bf, bf->posn, m ) ;
    ustrcpy_v ( p, s ) ;
    bf->posn = p + m ;
    return ;
}


/*
    ADD A FORMATTED STRING TO A BUFFER

    This routine adds the string s to the buffer bf, using printf-like
    format characters to print the extra arguments.  Note that this
    guarantees that bf will be null terminated.
*/

void bfprintf
    PROTO_V ( ( BUFFER *bf, CONST char *s, ... ) ) /* VARARGS */
{
    char c ;
    string p ;
    gen_size m ;
    va_list args ;
#if FS_STDARG
    va_start ( args, s ) ;
#else
    BUFFER *bf ;
    CONST char *s ;
    va_start ( args ) ;
    bf = va_arg ( args, BUFFER * ) ;
    s = va_arg ( args, CONST char * ) ;
#endif
    m = ( gen_size ) strlen ( s ) ;
    p = stretch_buffer ( bf, bf->posn, m ) ;

    /* Scan through format string */
    while ( c = *( s++ ), c != 0 ) {
	if ( c == '%' ) {
	    int ext = 0 ;
	    c = *( s++ ) ;
	    if ( c == 'l' ) {
		c = *( s++ ) ;
		ext = 1 ;
	    }
	    switch ( c ) {
		case 'c' : {
		    /* '%c' -> character (passed as int) */
		    int ac = va_arg ( args, int ) ;
		    p = stretch_buffer ( bf, p, 1 ) ;
		    *( p++ ) = ( character ) ac ;
		    break ;
		}
		case 'd' : {
		    p = stretch_buffer ( bf, p, ( gen_size ) 50 ) ;
		    if ( ext ) {
			/* '%ld' -> long */
			long al = va_arg ( args, long ) ;
			sprintf_v ( strlit ( p ), "%ld", al ) ;
		    } else {
			/* '%d' -> int */
			int ad = va_arg ( args, int ) ;
			sprintf_v ( strlit ( p ), "%d", ad ) ;
		    }
		    p = p + ustrlen ( p ) ;
		    break ;
		}
		case 's' : {
		    /* '%s' -> string */
		    string as = va_arg ( args, string ) ;
		    if ( as ) {
			m = ( gen_size ) ustrlen ( as ) ;
			p = stretch_buffer ( bf, p, m ) ;
			ustrcpy_v ( p, as ) ;
			p += m ;
		    }
		    break ;
		}
		case 'u' : {
		    p = stretch_buffer ( bf, p, ( gen_size ) 50 ) ;
		    if ( ext ) {
			/* '%lu' -> unsigned long */
			unsigned long al = va_arg ( args, unsigned long ) ;
			sprintf_v ( strlit ( p ), "%lu", al ) ;
		    } else {
			/* '%u' -> unsigned */
			unsigned au = va_arg ( args, unsigned ) ;
			sprintf_v ( strlit ( p ), "%u", au ) ;
		    }
		    p = p + ustrlen ( p ) ;
		    break ;
		}
		case 'x' : {
		    /* '%x' -> char * */
		    char *ax = va_arg ( args, char * ) ;
		    if ( ax ) {
			m = ( gen_size ) strlen ( ax ) ;
			p = stretch_buffer ( bf, p, m ) ;
			ustrcpy_v ( p, ustrlit ( ax ) ) ;
			p += m ;
		    }
		    break ;
		}
		case '%' : {
		    /* '%%' -> percent */
		    p = stretch_buffer ( bf, p, 1 ) ;
		    *( p++ ) = ( character ) c ;
		    break ;
		}
		default : {
		    /* Other characters */
		    FAIL ( Unknown format character ) ;
		    s = "<ERROR>" ;
		    break ;
		}
	    }
	} else {
	    p = stretch_buffer ( bf, p, 1 ) ;
	    *( p++ ) = ( character ) c ;
	}
    }
    *p = 0 ;
    bf->posn = p ;
    va_end ( args ) ;
    return ;
}


/*
    READ A STRING FROM A BUFFER

    This routine reads at most n characters from the buffer bf into the
    string s.  It returns the number of characters read.
*/

gen_size bfread
    PROTO_N ( ( bf, s, n ) )
    PROTO_T ( BUFFER *bf X string s X gen_size n )
{
    string p = bf->posn ;
    gen_size m = ( gen_size ) ( bf->end - p ) ;
    if ( m > n ) m = n ;
    if ( m ) {
	xumemcpy ( s, p, m ) ;
	bf->posn = p + m ;
    }
    return ( m ) ;
}
