/*
** $Id: llimits.h,v 1.26 2001/02/23 17:28:12 roberto Exp roberto $
** Limits, basic types, and some other `installation-dependent' definitions
** See Copyright Notice in lua.h
*/

#ifndef llimits_h
#define llimits_h


#include <limits.h>
#include <stddef.h>


#include "lua.h"


/*
** try to find number of bits in an integer
*/
#ifndef BITS_INT
/* avoid overflows in comparison */
#if INT_MAX-20 < 32760
#define	BITS_INT	16
#else
#if INT_MAX > 2147483640L
/* machine has at least 32 bits */
#define BITS_INT	32
#else
#error "you must define BITS_INT with number of bits in an integer"
#endif
#endif
#endif


/* function to convert a lua_Number to a string */
#ifndef NUMBER_FMT
#define NUMBER_FMT	l_s("%.16g")		/* LUA_NUMBER */
#endif
#ifndef lua_number2str
#define lua_number2str(s,n)	sprintf((s), NUMBER_FMT, (n))
#endif

/* function to convert a string to a lua_Number */
#ifndef lua_str2number
#define lua_str2number(s,p)	strtod((s), (p))
#endif


/* macro to control type of literal strings */
#ifndef l_s
#define l_s(x)	x
#endif

/* macro to control type of literal chars */
#ifndef l_c
#define l_c(x)	x
#endif


/*
** the following types define integer types for values that may not
** fit in a `small int' (16 bits), but may waste space in a
** `large long' (64 bits). The current definitions should work in
** any machine, but may not be optimal.
*/

/* an unsigned integer to hold hash values */
typedef unsigned int lu_hash;
/* its signed equivalent */
typedef int ls_hash;

/* an unsigned integer big enough to count the total memory used by Lua */
typedef unsigned long lu_mem;

/* an integer big enough to count the number of strings in use */
typedef long ls_nstr;


/* chars used as small naturals (so that `char' is reserved for characteres) */
typedef unsigned char lu_byte;


/* macro to `unsign' a character */
#ifndef uchar
#define uchar(c)	((unsigned char)(c))
#endif


#define MAX_SIZET	((size_t)(~(size_t)0)-2)


#define MAX_INT (INT_MAX-2)  /* maximum value of an int (-2 for safety) */

/*
** conversion of pointer to integer
** this is for hashing only; there is no problem if the integer
** cannot hold the whole pointer value
** (the shift removes bits that are usually 0 because of alignment)
*/
#define IntPoint(p)  ((((lu_hash)(p)) >> 4) ^ (lu_hash)(p))



#define MINPOWER2       4       /* minimum size for `growing' vectors */



#ifndef DEFAULT_STACK_SIZE
#define DEFAULT_STACK_SIZE      1024
#endif



/* type to ensure maximum alignment */
union L_Umaxalign { double d; void *s; long l; };



/*
** type for virtual-machine instructions
** must be an unsigned with (at least) 4 bytes (see details in lopcodes.h)
** For a very small machine, you may change that to 2 bytes (and adjust
** the following limits accordingly)
*/
typedef unsigned long Instruction;


/*
** size and position of opcode arguments.
** For an instruction with 2 bytes, size is 16, and size_b can be 5
** (accordingly, size_u will be 10, and size_a will be 5)
*/
#define SIZE_INSTRUCTION        32
#define SIZE_B          8

#define SIZE_OP         6
#define SIZE_U          (SIZE_INSTRUCTION-SIZE_OP)
#define POS_U           SIZE_OP
#define POS_B           SIZE_OP
#define SIZE_A          (SIZE_INSTRUCTION-(SIZE_OP+SIZE_B))
#define POS_A           (SIZE_OP+SIZE_B)


/*
** limits for opcode arguments.
** we use (signed) int to manipulate most arguments,
** so they must fit in BITS_INT-1 bits (-1 for sign)
*/
#if SIZE_U < BITS_INT-1
#define MAXARG_U        ((1<<SIZE_U)-1)
#define MAXARG_S        (MAXARG_U>>1)		/* `S' is signed */
#else
#define MAXARG_U        MAX_INT
#define MAXARG_S        MAX_INT
#endif

#if SIZE_A < BITS_INT-1
#define MAXARG_A        ((1<<SIZE_A)-1)
#else
#define MAXARG_A        MAX_INT
#endif

#if SIZE_B < BITS_INT-1
#define MAXARG_B        ((1<<SIZE_B)-1)
#else
#define MAXARG_B        MAX_INT
#endif


/* maximum stack size in a function */
#ifndef MAXSTACK
#define MAXSTACK	250
#endif

#if MAXSTACK > MAXARG_B
#undef MAXSTACK
#define MAXSTACK	MAXARG_B
#endif


/* maximum number of local variables */
#ifndef MAXLOCALS
#define MAXLOCALS 200           /* arbitrary limit (<MAXSTACK) */
#endif
#if MAXLOCALS>=MAXSTACK
#undef MAXLOCALS
#define MAXLOCALS	(MAXSTACK-1)
#endif


/* maximum number of upvalues */
#ifndef MAXUPVALUES
#define MAXUPVALUES 32          /* arbitrary limit (<=MAXARG_B) */
#endif
#if MAXUPVALUES>MAXARG_B
#undef MAXUPVALUES
#define MAXUPVALUES	MAXARG_B
#endif


/* maximum number of variables in the left side of an assignment */
#ifndef MAXVARSLH
#define MAXVARSLH 100           /* arbitrary limit (<MULT_RET) */
#endif
#if MAXVARSLH>=MULT_RET
#undef MAXVARSLH
#define MAXVARSLH	(MULT_RET-1)
#endif


/* maximum number of parameters in a function */
#ifndef MAXPARAMS
#define MAXPARAMS 100           /* arbitrary limit (<MAXLOCALS) */
#endif
#if MAXPARAMS>=MAXLOCALS
#undef MAXPARAMS
#define MAXPARAMS	(MAXLOCALS-1)
#endif


/* number of list items to accumulate before a SETLIST instruction */
#define LFIELDS_PER_FLUSH	64
#if LFIELDS_PER_FLUSH>(MAXSTACK/4)
#undef LFIELDS_PER_FLUSH
#define LFIELDS_PER_FLUSH	(MAXSTACK/4)
#endif

/* number of record items to accumulate before a SETMAP instruction */
/* (each item counts 2 elements on the stack: an index and a value) */
#define RFIELDS_PER_FLUSH	(LFIELDS_PER_FLUSH/2)


/* maximum lookback to find a real constant (for code generation) */
#ifndef LOOKBACKNUMS
#define LOOKBACKNUMS    20      /* arbitrary constant */
#endif


#endif
