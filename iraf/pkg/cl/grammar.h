/*
 * GRAMMAR.H -- Include stuff for parser and other grammar-related routines.
 */
#ifndef _GRAMMAR_H
#define _GRAMMAR_H

#include "param.h"

/* fieldcvt() takes the p_xxx parameter field spec and replaces it with 
 *   one of these field_name letters. this makes testing and using fields much
 *   faster for paramget(), paramset(), etc.
 * the letter is the first letter of the field, or the second if ambiguous.
 * FN_NULL is to test when field came back from fieldcvt() unspecified.
 *   or when calling paramset() or paramget() and you want the "worth" field.
 * The aliases for p_value all use FN_VALUE. see fieldcvt() in gram.c.
 */

#define	FN_NAME		'N'
#define	FN_TYPE		'T'
#define	FN_MODE		'O'
#define	FN_VALUE	'V'
#define	FN_LENGTH	'L'
#define	FN_MIN		'I'
#define	FN_MAX		'A'
#define	FN_PROMPT	'P'
#define FN_XTYPE	'X'		/* Extended type (list, gcur, etc) */
#define	FN_NULL		'\0'

/* possible return values from keyword(), in gram.c. 
 */
#define	KWBAD		(-1)	/* keyword not found			*/
#define	KWAMBIG		(-2)	/* keyword ambiguous			*/

/* magic constants.
 */
#define	CL_EOF		(-2)	/* integer value of EOF in language	*/
#define	CL_EOFSTR	"-2"	/* string equivalent of the above	*/
#define	PBRACE		1000	/* start brace level in procedure	*/

#define	NOLOG		0	/* do not save command block in logfile	*/
#define	LOG		1	/* save command block in logfile	*/

/* Constants determining how the parser is being called.		*/
#define PARSE_PARAMS	0	/* Parsing parameters at beginning.	*/
#define PARSE_BODY	1	/* Parsing body of script.		*/
#define PARSE_FREE	2	/* Not a procedure script.		*/

/* Command/compute mode status package.  The lexical mode may be set
 * explicitly for a particular command input stream.  While in command
 * mode (the default), the sequence #{ at the beginning of a line causes
 * compute mode to be permanently set for that stream (e.g., in a comment
 * at the head of a script file).  We use an otherwise unused bit in the
 * stdio file descriptor flag word to record whether or not compute mode
 * is set on a stream.
 */
#define	_LEXBIT			0100000
#define	lex_setcpumode(fp)	((fp)->_fflags |= _LEXBIT)
#define	lex_clrcpumode(fp)	((fp)->_fflags &= ~_LEXBIT)
#define	lex_cpumodeset(fp)	((fp)->_fflags & _LEXBIT)

/* global.c */
extern int parse_state;			/* What are we parsing?	*/
extern int proc_script;			/* In a procedure script? */
extern struct pfile *parse_pfile;	/* Where parsed params are added. */

/* gram.c */
extern int yeof;		/* set by yywrap when it sees eof. */
extern int pipetable[];		/* pipe stack (pipecodes) */
extern int nextpipe;		/* pipe stack pointer (next index) */
extern char truestr[];
extern char falsestr[];
extern char nullstr[];
extern char undefval[];
extern const char *indefstr;
extern const char *indeflc;
extern const char *eofstr;
extern const char *eoflc;
extern const char *epsilonstr;
extern const char *errorstr;
extern char *err_cmdblk;	/* Pointer where error detected */
extern void delpipes ( int );
extern void breakout ( const char *, char **, char **, char **, char ** );
extern int yywrap ( void );
extern int addconst ( const char *, int );
extern int crackident ( const char * );
extern void rerun( void );
extern void p_position( void );
extern void sexa_to_index ( double, int *, int * );
extern void yyerror ( const char * );
extern void loopincr ( void );
extern void loopdecr( void );
extern void setswitch ( void );
extern int in_switch( void );
extern void unsetigoto ( int );
extern void setigoto ( int );
extern int make_imloop ( int, int );
extern void intrfunc ( const char *, int );
extern char *addpipe( void );
extern char *getpipe( void );
extern struct label *getlabel ( struct operand * );
extern struct label *setlabel ( struct operand * );
extern int y_typedef ( const char * );

/* decl.c */
extern int procscript ( FILE * );
extern int skip_to ( FILE *, const char * );
extern void proc_params ( int );
extern void do_arrayinit ( struct param *, int, int );
extern void do_option ( struct param *, struct operand *, struct operand * );
extern void do_scalarinit ( struct param *, int );
extern int get_dim ( const char * );
extern int getlimits ( const char *, int, int *, int * );
extern struct param *initparam ( struct operand *, int, int, int );

#endif	/* _GRAMMAR_H */
