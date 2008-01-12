#ifndef _YTAB_H
#define _YTAB_H

# define Y_SCAN 257
# define Y_SCANF 258
# define Y_FSCAN 259
# define Y_FSCANF 260
# define Y_OSESC 261
# define Y_APPEND 262
# define Y_ALLAPPEND 263
# define Y_ALLREDIR 264
# define Y_GSREDIR 265
# define Y_ALLPIPE 266
# define D_D 267
# define D_PEEK 268
# define Y_NEWLINE 269
# define Y_CONSTANT 270
# define Y_IDENT 271
# define Y_WHILE 272
# define Y_IF 273
# define Y_ELSE 274
# define Y_FOR 275
# define Y_BREAK 276
# define Y_NEXT 277
# define Y_SWITCH 278
# define Y_CASE 279
# define Y_DEFAULT 280
# define Y_RETURN 281
# define Y_GOTO 282
# define Y_PROCEDURE 283
# define Y_BEGIN 284
# define Y_END 285
# define Y_BOOL 286
# define Y_INT 287
# define Y_REAL 288
# define Y_STRING 289
# define Y_FILE 290
# define Y_STRUCT 291
# define Y_GCUR 292
# define Y_IMCUR 293
# define Y_UKEY 294
# define Y_PSET 295
# define YOP_AOADD 296
# define YOP_AOSUB 297
# define YOP_AOMUL 298
# define YOP_AODIV 299
# define YOP_AOCAT 300
# define YOP_OR 301
# define YOP_AND 302
# define YOP_EQ 303
# define YOP_NE 304
# define YOP_LE 305
# define YOP_GE 306
# define YOP_CONCAT 307
# define YOP_NOT 308
# define UMINUS 309
# define YOP_POW 310

#ifndef YYSTYPE
#define YYSTYPE int
#endif

/* ytab.c */
extern YYSTYPE yylval;
extern int dobkg;		/* flag bkg execution */
extern int inarglist;		/* set by parser when in argument list */
extern int parenlevel;		/* nesting level of parens */
extern char *ifseen;		/* Processing an IF statement? */
extern int yydebug;		/* print each parser state if set */
extern FILE *yyin;		/* where parser reads from */
extern int yyparse( void );

/* lexicon.c */
extern	int _lexmodes;		/* enable lexical mode switching */
extern void lexinit( void );

#endif	/* _YTAB_H */
