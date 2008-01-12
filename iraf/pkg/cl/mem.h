/*
 * MEM.H -- Define the dictionary, the stack, indices of various kinds, 
 * and ways of converting the indices into true address pointers.
 *
 * Structures that live within the dictionary may use pointers to
 * point at other structures (such as the task and parameter chains) but
 * things that simply point AT the dictionary and that move around are indices
 * into what appears to be the array of unsigned integers called dictionary.
 * This is to facilitate putting things of disparate types into the array.
 */
#ifndef _MEM_H
#define _MEM_H

/* bytes per int;
 * typically used when putting things in the dictionary like strings, operands
 * and codeentries. also, the pc must be advanced in ints.
 *
 * N.B. it is FUNDAMENTALLY ASSUMED throughout that an int is large enough to
 * hold a pointer to an int. Further, although casts are used carefully as
 * much as possible and so a good compiler will do much of the work,
 * it is also pretty much taken for granted that all pointers are the
 * same size, in particular that (char *) is the same size as (unsigned *).
 */

#define	BPI	(sizeof (memel))
#define	btoi(x)	((int)((((x)+BPI-1)/BPI))) /* avoid promotion to unsigned */
#define dtoi(x) ((int)(sizeof(double))/(sizeof(memel))*x)

/* the dictionary starts at the top of the system break and grows as needed.
 * if this is hard to do on your os, declare it as a genuine array and
 *   forever fix the value of maxd by initializing them in their declarations
 *   in compile.c. see machdep.c.
 */


extern memel *dictionary;    /* base of the dictionary; never moves */

/* ----------
 * convert a dictionary index into a structure pointer.
 * also, dereference a pointer to a dictionary index.
 */

#define	reference(sname,index)	((struct sname *) (&dictionary[index]))
#define	dereference(ptr) \
(((unsigned long)(char *)(ptr) - (unsigned long)(char *)(dictionary))/BPI)

/* ----------
 * Generic push/pop memory routines.  Can be used to push/pop any integer type
 * argument regardless of size, so long as it fits in a memel.
 */
#define push(v)		pushmem((memel)v)
#define ppush(v)	ppushmem((memel)v)
#define pop		popmem

/* ----------
 * convert a dictionary index into a genuine address; type will be 
 * the type of dictionary.
 */

#define	daddr(x)			(&dictionary[x])

/* ----------
 * maxd: smallest d. index that is out of range and will give mem fault if
 *	referenced. commonly referred to as the "system break".
 * topd: next d. index available for use, ie, it is the smallest d. index
 *	not in use.
 * pachead: dictionary index of most recently added package.
 * parhead: 		   "			    pfile.
 * envhead:                "                        environment.
 */

extern int maxd;
extern int topd;
extern int pachead;
extern int parhead;
extern int envhead;

/* ----------
 * these are indices into the stack defined in stack.c.
 * topcs: the smallest index into stack[], ie, the "top" index of the control
 *	stack since it grows downwards, that has been used.
 * topos: the largest index into stack[], ie, the top of the operand stack
 *	since it grows upwards, that has been used.
 * pc: at compile time, this is the stack[] index at which the next codeentry
 *	may be compiled; at run time, it is the program counter and points
 *	to the next codeentry to be run	(it is bumped before the "execute"
 *	cycle begins. see run()).
 * basos: not used at compile time, but when compilation ends and runtime
 *	begins, it is set to pc and thus serves as the base of the operand
 *	stack as everything below it will be compiled code. when compiling
 *	starts again, this, and pc, are set to zero to forcibly clear the
 *	operand stack.
 */

extern memel stack[];		/* space for the stacks			*/
extern int topcs;		/* top of control stack			*/
extern int topos;		/* top of operand stack			*/
extern int basos;		/* base of operand stack		*/
extern int pc;			/* program counter			*/

/* ----------
 * reference a codeentry in stack at x.
 */
#define	coderef(x)	((struct codeentry *)&stack[x])

/* main.c */
extern memel cl_dictbuf[];	/* static dictionary area */
extern char *memneed( int );	/* insures enough core, returns start	*/
/* compile.c */
#ifdef USE_STDARG
extern int compile ( int, ... );
#else
extern int compile ();
#endif
extern char *comdstr( const char * );	/* compile string at topd, return start	*/
extern void catdstr ( char *, const char * );
/* gram.c */
extern void caseset ( memel *, int );
/* stack.c */
extern memel popmem ( void );
extern void pushmem ( memel );
extern void ppushmem ( memel );

#endif	/* _MEM_H */
