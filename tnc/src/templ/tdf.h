/* $Id$ */

/*
 * Copyright 2002-2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */


@use bytestream
@use tokdec_props
@use tokdef_props
@use tagdec_props
@use tagdef_props
@use al_tagdef_props
@use version_props
@special external
@special token
@special tokdec
@special tokdef
@special tagdec
@special tagdef
@special al_tagdef
@special version
@special token_defn
@special sortname
#ifndef TDF_H
#define TDF_H


/* AUTOMATICALLY GENERATED BY %ZX VERSION %ZV FROM TDF %VA.%VB */

void init_constructs(void);
sortname find_sort(char);

@loop sort
@if sort.basic
@if sort.special
long de_%SN_bits(void);
@else
node *de_%SN(void);
@endif
@endif
@end

#ifdef ENC_TYPES_H
@loop sort
@if sort.basic
@if sort.special
void enc_%SN_bits(bitstream *, int);
@else
@if sort.edge
void enc_%SN_bits(bitstream *, int);
@endif
@endif
@endif
@end
#endif /* ENC_TYPES_H */

@loop sort
@if sort.basic
@if !sort.special
node *read_%SN(long);
@endif
@endif
@end

#define VERSION_capsule		%48tMAGIC_NUMBER
#define VERSION_library		%48tMAGIC_LINK_NUMBER
#define VERSION_major		%48t%VA
#define VERSION_minor		%48t%VB

@loop sort
@if sort.unit
#define LINK_%SN		%48t"%SU"
@endif
@end
#define LINK_tld_props		%48t"tld"

@loop sort
@if sort.link
#define LINK_%SN		%48t"%SL"
@endif
@end

@special !sortname
@loop sort
@if sort.eq.sortname
@loop sort.cons
@if cons.eq.alignment_sort
#define SORT_alignment		%48t%CE
@else
#define SORT_%CN		%48t%CE
@endif
@end
#define SORT_foreign		%48tENC_foreign_sort
#define SORT_callees		%48t%1SM
#define SORT_error_code		%48t%2SM
#define SORT_sortname		%48t0
#define SORT_unknown		%48t%u
#define SORT_repeat		%48t(-%u)
#define SORT_option		%48t(-%u)
#define SORT_completion		%48t(-%u)
#define SORT_small_tdfint	%48t(-%u)
@loop sort
@if sort.builtin
#define SORT_%SN		%48t(-%u)
@endif
@end
#define SORT_max		%48t%SM
#define SORT_no			%48t%3SM
@endif
@end

@loop sort
@if sort.basic
@loop sort.cons
#define ENC_%27CN	%48t%CE
@end
@if sort.eq.shape
#define ENC_shape_none	%48t%1SM
@endif
@endif
@end


#endif /* TDF_H */
