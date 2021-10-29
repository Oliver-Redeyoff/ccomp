/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     CONSTANT = 259,
     STRING_LITERAL = 260,
     LE_OP = 261,
     GE_OP = 262,
     EQ_OP = 263,
     NE_OP = 264,
     EXTERN = 265,
     AUTO = 266,
     INT = 267,
     VOID = 268,
     FUNCTION = 269,
     APPLY = 270,
     LEAF = 271,
     IF = 272,
     ELSE = 273,
     WHILE = 274,
     CONTINUE = 275,
     BREAK = 276,
     RETURN = 277
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define CONSTANT 259
#define STRING_LITERAL 260
#define LE_OP 261
#define GE_OP 262
#define EQ_OP 263
#define NE_OP 264
#define EXTERN 265
#define AUTO 266
#define INT 267
#define VOID 268
#define FUNCTION 269
#define APPLY 270
#define LEAF 271
#define IF 272
#define ELSE 273
#define WHILE 274
#define CONTINUE 275
#define BREAK 276
#define RETURN 277




/* Copy the first part of user declarations.  */
#line 1 "C.y"

#include "nodes.h"
#define YYSTYPE NODE*
#define YYDEBUG 1
extern TOKEN *int_token, *void_token, *function_token, *lasttok;
NODE *ans;
int yylex();
int yyerror(char *s);


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 160 "C.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   380

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  107
/* YYNRULES -- Number of states.  */
#define YYNSTATES  171

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   277

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,     2,     2,    32,    26,     2,
      23,    24,    27,    28,    25,    29,     2,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    36,
      33,    35,    34,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    15,    17,    21,
      26,    28,    32,    34,    37,    39,    41,    43,    45,    47,
      49,    53,    57,    61,    63,    67,    71,    73,    77,    81,
      85,    89,    91,    95,    99,   101,   105,   107,   111,   114,
     116,   120,   122,   125,   127,   130,   132,   136,   138,   142,
     144,   146,   148,   150,   152,   155,   157,   159,   163,   168,
     173,   177,   179,   182,   184,   188,   191,   194,   196,   198,
     202,   204,   206,   209,   213,   216,   220,   224,   229,   231,
     233,   235,   237,   239,   242,   246,   250,   255,   257,   260,
     262,   265,   267,   270,   276,   284,   290,   293,   296,   299,
     303,   305,   308,   310,   312,   317,   321,   325
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      40,     0,    -1,    74,    -1,     3,    -1,     4,    -1,     5,
      -1,    23,    51,    24,    -1,    41,    -1,    42,    23,    24,
      -1,    42,    23,    43,    24,    -1,    50,    -1,    43,    25,
      50,    -1,    42,    -1,    45,    44,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    30,    -1,    44,    -1,    46,
      27,    44,    -1,    46,    31,    44,    -1,    46,    32,    44,
      -1,    46,    -1,    47,    28,    46,    -1,    47,    29,    46,
      -1,    47,    -1,    48,    33,    47,    -1,    48,    34,    47,
      -1,    48,     6,    47,    -1,    48,     7,    47,    -1,    48,
      -1,    49,     8,    48,    -1,    49,     9,    48,    -1,    49,
      -1,    44,    35,    50,    -1,    50,    -1,    51,    25,    50,
      -1,    53,    36,    -1,    76,    -1,    53,    54,    36,    -1,
      56,    -1,    56,    53,    -1,    57,    -1,    57,    53,    -1,
      55,    -1,    54,    25,    55,    -1,    58,    -1,    58,    35,
      50,    -1,    10,    -1,    11,    -1,    13,    -1,    12,    -1,
      14,    -1,    60,    59,    -1,    59,    -1,     3,    -1,    23,
      58,    24,    -1,    59,    23,    61,    24,    -1,    59,    23,
      63,    24,    -1,    59,    23,    24,    -1,    27,    -1,    27,
      60,    -1,    62,    -1,    61,    25,    62,    -1,    53,    58,
      -1,    53,    64,    -1,    53,    -1,     3,    -1,    63,    25,
       3,    -1,    60,    -1,    65,    -1,    60,    65,    -1,    23,
      64,    24,    -1,    23,    24,    -1,    23,    61,    24,    -1,
      65,    23,    24,    -1,    65,    23,    61,    24,    -1,    67,
      -1,    70,    -1,    71,    -1,    72,    -1,    73,    -1,    37,
      38,    -1,    37,    69,    38,    -1,    37,    68,    38,    -1,
      37,    68,    69,    38,    -1,    52,    -1,    68,    52,    -1,
      66,    -1,    69,    66,    -1,    36,    -1,    51,    36,    -1,
      17,    23,    51,    24,    66,    -1,    17,    23,    51,    24,
      66,    18,    66,    -1,    19,    23,    51,    24,    66,    -1,
      20,    36,    -1,    21,    36,    -1,    22,    36,    -1,    22,
      51,    36,    -1,    75,    -1,    74,    75,    -1,    76,    -1,
      52,    -1,    53,    58,    68,    67,    -1,    53,    58,    67,
      -1,    58,    68,    67,    -1,    58,    67,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    23,    23,    27,    28,    29,    30,    34,    35,    36,
      41,    42,    47,    48,    52,    53,    54,    55,    56,    60,
      61,    63,    65,    70,    71,    73,    78,    79,    81,    83,
      85,    90,    91,    93,    98,    99,   104,   105,   109,   110,
     111,   116,   117,   119,   120,   124,   125,   129,   130,   134,
     135,   139,   140,   141,   145,   146,   150,   151,   152,   153,
     154,   158,   159,   163,   164,   168,   169,   170,   174,   175,
     181,   182,   183,   187,   188,   189,   190,   191,   195,   196,
     197,   198,   199,   203,   204,   205,   206,   210,   211,   215,
     216,   220,   221,   225,   226,   232,   236,   237,   238,   239,
     243,   244,   248,   249,   253,   255,   257,   259
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "CONSTANT",
  "STRING_LITERAL", "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "EXTERN", "AUTO",
  "INT", "VOID", "FUNCTION", "APPLY", "LEAF", "IF", "ELSE", "WHILE",
  "CONTINUE", "BREAK", "RETURN", "'('", "')'", "','", "'&'", "'*'", "'+'",
  "'-'", "'!'", "'/'", "'%'", "'<'", "'>'", "'='", "';'", "'{'", "'}'",
  "$accept", "goal", "primary_expression", "postfix_expression",
  "argument_expression_list", "unary_expression", "unary_operator",
  "multiplicative_expression", "additive_expression",
  "relational_expression", "equality_expression", "assignment_expression",
  "expression", "declaration", "declaration_specifiers",
  "init_declarator_list", "init_declarator", "storage_class_specifier",
  "type_specifier", "declarator", "direct_declarator", "pointer",
  "parameter_list", "parameter_declaration", "identifier_list",
  "abstract_declarator", "direct_abstract_declarator", "statement",
  "compound_statement", "declaration_list", "statement_list",
  "expression_statement", "selection_statement", "iteration_statement",
  "jump_statement", "translation_unit", "external_declaration",
  "function_definition", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    40,    41,    44,    38,    42,    43,    45,
      33,    47,    37,    60,    62,    61,    59,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    41,    41,    41,    41,    42,    42,    42,
      43,    43,    44,    44,    45,    45,    45,    45,    45,    46,
      46,    46,    46,    47,    47,    47,    48,    48,    48,    48,
      48,    49,    49,    49,    50,    50,    51,    51,    52,    52,
      52,    53,    53,    53,    53,    54,    54,    55,    55,    56,
      56,    57,    57,    57,    58,    58,    59,    59,    59,    59,
      59,    60,    60,    61,    61,    62,    62,    62,    63,    63,
      64,    64,    64,    65,    65,    65,    65,    65,    66,    66,
      66,    66,    66,    67,    67,    67,    67,    68,    68,    69,
      69,    70,    70,    71,    71,    72,    73,    73,    73,    73,
      74,    74,    75,    75,    76,    76,    76,    76
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     3,     1,     3,     4,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     1,     3,     2,     1,
       3,     1,     2,     1,     2,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     2,     1,     1,     3,     4,     4,
       3,     1,     2,     1,     3,     2,     2,     1,     1,     3,
       1,     1,     2,     3,     2,     3,     3,     4,     1,     1,
       1,     1,     1,     2,     3,     3,     4,     1,     2,     1,
       2,     1,     2,     5,     7,     5,     2,     2,     2,     3,
       1,     2,     1,     1,     4,     3,     3,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    56,    49,    50,    52,    51,    53,     0,    61,     0,
     103,     0,    41,    43,     0,    55,     0,     2,   100,    39,
       0,    62,     1,    38,     0,    45,    47,    42,    44,     0,
      87,   107,     0,    39,     0,    54,   101,    57,     0,    40,
       0,   105,     0,     3,     4,     5,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,    18,    91,    83,     7,
      12,    19,     0,    23,    26,    31,    34,    36,     0,    89,
      78,     0,     0,    79,    80,    81,    82,    88,   106,    68,
      60,    67,     0,    63,     0,    46,    47,     3,     0,    15,
      48,   104,     0,     0,    96,    97,    98,     0,     0,     0,
       0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    85,     0,    84,    90,     0,
      65,    70,    66,    71,    58,     0,    59,     0,     0,     0,
      99,     6,     8,     0,    10,    35,    20,    21,    22,    19,
      24,    25,    29,    30,    27,    28,    32,    33,    37,    86,
      74,     0,     0,    72,     0,    64,    69,     0,     0,     9,
       0,    75,    73,    76,     0,    93,    95,    11,    77,     0,
      94
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     9,    59,    60,   133,    61,    62,    63,    64,    65,
      66,    67,    68,    30,    11,    24,    25,    12,    13,    14,
      15,    16,    82,    83,    84,   122,   123,    69,    70,    32,
      72,    73,    74,    75,    76,    17,    18,    33
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -112
static const yytype_int16 yypact[] =
{
     351,  -112,  -112,  -112,  -112,  -112,  -112,    22,    -8,    24,
    -112,    10,   210,   210,    17,    19,    13,   351,  -112,  -112,
       8,  -112,  -112,  -112,   -15,  -112,    71,  -112,  -112,   124,
    -112,  -112,    17,  -112,   356,    19,  -112,  -112,    22,  -112,
     301,  -112,    17,   258,  -112,  -112,    30,    35,    57,    64,
     273,   312,  -112,    -8,  -112,  -112,  -112,  -112,  -112,  -112,
      37,    44,   301,    93,   -11,     5,   109,  -112,    61,  -112,
    -112,   153,   181,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,    32,   115,  -112,   144,  -112,    67,  -112,   301,  -112,
    -112,  -112,   301,   301,  -112,  -112,  -112,    63,   163,   284,
     301,  -112,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,  -112,  -112,   209,  -112,  -112,   333,
    -112,    73,  -112,    82,  -112,   210,  -112,   107,   191,   219,
    -112,  -112,  -112,   224,  -112,  -112,  -112,  -112,  -112,  -112,
      93,    93,   -11,   -11,   -11,   -11,     5,     5,  -112,  -112,
    -112,   226,    88,    82,   182,  -112,  -112,   237,   237,  -112,
     301,  -112,  -112,  -112,   228,    96,  -112,  -112,  -112,   237,
    -112
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -112,  -112,  -112,  -112,  -112,   -39,  -112,    43,    23,   122,
    -112,   -38,   -36,     9,   -12,  -112,    83,  -112,  -112,    -4,
     -10,    -3,  -111,    -2,  -112,     7,    34,   -68,    77,    66,
     100,  -112,  -112,  -112,  -112,  -112,   142,    87
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -57
static const yytype_int16 yytable[] =
{
      27,    28,    90,    20,   118,    21,    35,    26,   151,    10,
      38,   107,   108,     1,    97,    98,     1,   105,   106,     8,
       1,    39,    81,   101,    22,     1,    10,     2,     3,     4,
       5,     6,    37,     7,    86,     1,     7,     8,   109,   110,
       7,    77,    34,   164,     8,     7,    23,    20,   118,     8,
      21,    77,    98,    92,    29,   119,   128,   129,    93,     8,
      99,   134,   135,   136,   137,   138,   139,   139,   139,   139,
     139,   139,   139,   139,     1,   148,     1,   120,   121,   100,
      77,     2,     3,     4,     5,     6,   113,    19,   113,   165,
     166,    31,    42,    94,     7,    71,   119,   114,     8,   130,
      95,   170,    40,    41,    19,   154,    40,    81,    29,    78,
     156,    35,   162,    81,   169,    20,   121,   111,   112,    91,
     102,    85,   167,   155,   103,   104,   152,    43,    44,    45,
     142,   143,   144,   145,     2,     3,     4,     5,     6,   124,
     125,    46,    81,    47,    48,    49,    50,    51,   140,   141,
      52,    53,    54,    55,    56,   153,    43,    44,    45,    36,
      57,    29,    58,     2,     3,     4,     5,     6,   126,   127,
      46,   116,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,    55,    56,    87,    44,    45,   131,   113,    57,
      29,   115,     2,     3,     4,     5,     6,     0,    46,     0,
      47,    48,    49,    50,    88,     0,   163,    52,    89,    54,
      55,    56,    87,    44,    45,   157,   113,    57,    29,   117,
       2,     3,     4,     5,     6,     0,    46,     0,    47,    48,
      49,    50,    88,   146,   147,    52,    89,    54,    55,    56,
      87,    44,    45,   158,   113,    57,    29,   149,   159,   160,
     161,   125,   168,   125,    46,     0,    47,    48,    49,    50,
      88,   -56,     0,    52,    89,    54,    55,    56,   -56,   -56,
     -56,   -56,   -56,    57,    29,     0,    87,    44,    45,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    44,    45,
       0,     0,     0,     0,     0,   -56,    88,     0,     0,    52,
      89,    54,    55,    56,    87,    44,    45,    88,   132,    96,
      52,    89,    54,    55,    56,    43,    44,    45,     0,     0,
       0,     0,     0,     0,    88,     0,     0,    52,    89,    54,
      55,    56,     0,     0,     0,    51,     1,     0,    52,    53,
      54,    55,    56,     2,     3,     4,     5,     6,     0,     0,
       0,     0,     0,     0,     1,     0,   119,   150,     0,    79,
       8,     2,     3,     4,     5,     6,     2,     3,     4,     5,
       6,     0,     0,     0,     7,     0,     0,     0,     8,     0,
      80
};

static const yytype_int16 yycheck[] =
{
      12,    13,    40,     7,    72,     8,    16,    11,   119,     0,
      25,     6,     7,     3,    50,    51,     3,    28,    29,    27,
       3,    36,    34,    62,     0,     3,    17,    10,    11,    12,
      13,    14,    24,    23,    38,     3,    23,    27,    33,    34,
      23,    32,    23,   154,    27,    23,    36,    51,   116,    27,
      53,    42,    88,    23,    37,    23,    92,    93,    23,    27,
      23,    99,   100,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,     3,   113,     3,    81,    81,    35,
      71,    10,    11,    12,    13,    14,    25,     0,    25,   157,
     158,    14,    26,    36,    23,    29,    23,    36,    27,    36,
      36,   169,    35,    26,    17,    23,    35,   119,    37,    32,
       3,   121,    24,   125,    18,   119,   119,     8,     9,    42,
      27,    38,   160,   125,    31,    32,   119,     3,     4,     5,
     107,   108,   109,   110,    10,    11,    12,    13,    14,    24,
      25,    17,   154,    19,    20,    21,    22,    23,   105,   106,
      26,    27,    28,    29,    30,   121,     3,     4,     5,    17,
      36,    37,    38,    10,    11,    12,    13,    14,    24,    25,
      17,    71,    19,    20,    21,    22,    23,    -1,    -1,    26,
      27,    28,    29,    30,     3,     4,     5,    24,    25,    36,
      37,    38,    10,    11,    12,    13,    14,    -1,    17,    -1,
      19,    20,    21,    22,    23,    -1,    24,    26,    27,    28,
      29,    30,     3,     4,     5,    24,    25,    36,    37,    38,
      10,    11,    12,    13,    14,    -1,    17,    -1,    19,    20,
      21,    22,    23,   111,   112,    26,    27,    28,    29,    30,
       3,     4,     5,    24,    25,    36,    37,    38,    24,    25,
      24,    25,    24,    25,    17,    -1,    19,    20,    21,    22,
      23,     3,    -1,    26,    27,    28,    29,    30,    10,    11,
      12,    13,    14,    36,    37,    -1,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,    -1,    -1,    -1,    -1,    37,    23,    -1,    -1,    26,
      27,    28,    29,    30,     3,     4,     5,    23,    24,    36,
      26,    27,    28,    29,    30,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    -1,    -1,    23,     3,    -1,    26,    27,
      28,    29,    30,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    23,    24,    -1,     3,
      27,    10,    11,    12,    13,    14,    10,    11,    12,    13,
      14,    -1,    -1,    -1,    23,    -1,    -1,    -1,    27,    -1,
      24
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    10,    11,    12,    13,    14,    23,    27,    40,
      52,    53,    56,    57,    58,    59,    60,    74,    75,    76,
      58,    60,     0,    36,    54,    55,    58,    53,    53,    37,
      52,    67,    68,    76,    23,    59,    75,    24,    25,    36,
      35,    67,    68,     3,     4,     5,    17,    19,    20,    21,
      22,    23,    26,    27,    28,    29,    30,    36,    38,    41,
      42,    44,    45,    46,    47,    48,    49,    50,    51,    66,
      67,    68,    69,    70,    71,    72,    73,    52,    67,     3,
      24,    53,    61,    62,    63,    55,    58,     3,    23,    27,
      50,    67,    23,    23,    36,    36,    36,    51,    51,    23,
      35,    44,    27,    31,    32,    28,    29,     6,     7,    33,
      34,     8,     9,    25,    36,    38,    69,    38,    66,    23,
      58,    60,    64,    65,    24,    25,    24,    25,    51,    51,
      36,    24,    24,    43,    50,    50,    44,    44,    44,    44,
      46,    46,    47,    47,    47,    47,    48,    48,    50,    38,
      24,    61,    64,    65,    23,    62,     3,    24,    24,    24,
      25,    24,    24,    24,    61,    66,    66,    50,    24,    18,
      66
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 23 "C.y"
    { ans = (yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 3:
#line 27 "C.y"
    { (yyval) = make_leaf(lasttok); ;}
    break;

  case 4:
#line 28 "C.y"
    { (yyval) = make_leaf(lasttok); ;}
    break;

  case 5:
#line 29 "C.y"
    { (yyval) = make_leaf(lasttok); ;}
    break;

  case 6:
#line 30 "C.y"
    { (yyval) = (yyvsp[(2) - (3)]); ;}
    break;

  case 7:
#line 34 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 8:
#line 35 "C.y"
    { (yyval) = make_node(APPLY, (yyvsp[(1) - (3)]), NULL); ;}
    break;

  case 9:
#line 36 "C.y"
    {
				          (yyval) = make_node(APPLY, (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 10:
#line 41 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 11:
#line 42 "C.y"
    {
          (yyval) = make_node(',', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 12:
#line 47 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 13:
#line 48 "C.y"
    { (yyval) = make_node((int)(yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]), NULL); ;}
    break;

  case 14:
#line 52 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 15:
#line 53 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 16:
#line 54 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 17:
#line 55 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 18:
#line 56 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 19:
#line 60 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 20:
#line 61 "C.y"
    {
                                          (yyval) = make_node('*', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 21:
#line 63 "C.y"
    {
                                          (yyval) = make_node('/', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 22:
#line 65 "C.y"
    {
                                          (yyval) = make_node('%', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 23:
#line 70 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 24:
#line 71 "C.y"
    {
                                          (yyval) = make_node('+', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 25:
#line 73 "C.y"
    {
                                          (yyval) = make_node('-', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 26:
#line 78 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 27:
#line 79 "C.y"
    {
                                          (yyval) = make_node('<', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 28:
#line 81 "C.y"
    {
                                          (yyval) = make_node('>', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 29:
#line 83 "C.y"
    {
                                          (yyval) = make_node(LE_OP, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 30:
#line 85 "C.y"
    {
                                          (yyval) = make_node(GE_OP, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 31:
#line 90 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 32:
#line 91 "C.y"
    {
                                          (yyval) = make_node(EQ_OP, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 33:
#line 93 "C.y"
    {
                                          (yyval) = make_node(NE_OP, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 34:
#line 98 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 35:
#line 99 "C.y"
    {
                                          (yyval) = make_node('=', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 36:
#line 104 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 37:
#line 105 "C.y"
    { (yyval) = make_node(',', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 38:
#line 109 "C.y"
    { (yyval) = (yyvsp[(1) - (2)]); ;}
    break;

  case 39:
#line 110 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 40:
#line 111 "C.y"
    {
                                                  (yyval) = make_node('~', (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])); ;}
    break;

  case 41:
#line 116 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 42:
#line 117 "C.y"
    { 
                                                  (yyval) = make_node('~', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 43:
#line 119 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 44:
#line 120 "C.y"
    { (yyval) = make_node('~', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 45:
#line 124 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 46:
#line 125 "C.y"
    { (yyval) = make_node(',', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 47:
#line 129 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 48:
#line 130 "C.y"
    { (yyval) = make_node('=', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 49:
#line 134 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 50:
#line 135 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 51:
#line 139 "C.y"
    { (yyval) = make_leaf(void_token); ;}
    break;

  case 52:
#line 140 "C.y"
    { (yyval) = make_leaf(int_token); ;}
    break;

  case 53:
#line 141 "C.y"
    { (yyval) = make_leaf(function_token); ;}
    break;

  case 54:
#line 145 "C.y"
    { (yyval) = make_node('~', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 55:
#line 146 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 56:
#line 150 "C.y"
    { (yyval) = make_leaf(lasttok); ;}
    break;

  case 57:
#line 151 "C.y"
    { (yyval) = (yyvsp[(2) - (3)]); ;}
    break;

  case 58:
#line 152 "C.y"
    { (yyval) = make_node('F', (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 59:
#line 153 "C.y"
    { (yyval) = make_node('F', (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 60:
#line 154 "C.y"
    { (yyval) = make_node('F', (yyvsp[(1) - (3)]), NULL); ;}
    break;

  case 61:
#line 158 "C.y"
    { (yyval) = (NODE*)1; ;}
    break;

  case 62:
#line 159 "C.y"
    { (yyval) = (NODE*)((int)(yyvsp[(2) - (2)])+1); ;}
    break;

  case 63:
#line 163 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 64:
#line 164 "C.y"
    { (yyval) = make_node(',', (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 65:
#line 168 "C.y"
    { (yyval) = make_node('~', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 66:
#line 169 "C.y"
    { (yyval) = make_node('~', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 67:
#line 170 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 68:
#line 174 "C.y"
    { (yyval) = make_leaf(lasttok); ;}
    break;

  case 69:
#line 175 "C.y"
    {
                                          (yyval) = make_node(',', (yyvsp[(1) - (3)]),
                                                              make_leaf(lasttok)); ;}
    break;

  case 70:
#line 181 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 71:
#line 182 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 72:
#line 183 "C.y"
    { (yyval) = make_node('G', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 73:
#line 187 "C.y"
    { (yyval) = (yyvsp[(2) - (3)]); ;}
    break;

  case 74:
#line 188 "C.y"
    { (yyval) = NULL; ;}
    break;

  case 75:
#line 189 "C.y"
    { (yyval) = (yyvsp[(2) - (3)]); ;}
    break;

  case 76:
#line 190 "C.y"
    { (yyval) = make_node(APPLY, (yyvsp[(1) - (3)]), NULL); ;}
    break;

  case 77:
#line 191 "C.y"
    { (yyval) = make_node(APPLY, (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 78:
#line 195 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 79:
#line 196 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 80:
#line 197 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 81:
#line 198 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 82:
#line 199 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 83:
#line 203 "C.y"
    { (yyval) = NULL; ;}
    break;

  case 84:
#line 204 "C.y"
    { (yyval) = (yyvsp[(2) - (3)]); ;}
    break;

  case 85:
#line 205 "C.y"
    { (yyval) = (yyvsp[(2) - (3)]); ;}
    break;

  case 86:
#line 206 "C.y"
    { (yyval) = make_node(';', (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 87:
#line 210 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 88:
#line 211 "C.y"
    { (yyval) = make_node(';', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 89:
#line 215 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 90:
#line 216 "C.y"
    { (yyval) = make_node(';', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 91:
#line 220 "C.y"
    { (yyval) = NULL; ;}
    break;

  case 92:
#line 221 "C.y"
    { (yyval) = (yyvsp[(1) - (2)]); ;}
    break;

  case 93:
#line 225 "C.y"
    { (yyval) = make_node(IF, (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 94:
#line 227 "C.y"
    { (yyval) = make_node(IF, (yyvsp[(3) - (7)]),
                                                        make_node(ELSE, (yyvsp[(5) - (7)]), (yyvsp[(7) - (7)]))); ;}
    break;

  case 95:
#line 232 "C.y"
    { (yyval) = make_node(WHILE, (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 96:
#line 236 "C.y"
    { (yyval) = make_node(CONTINUE, NULL, NULL); ;}
    break;

  case 97:
#line 237 "C.y"
    { (yyval) = make_node(BREAK, NULL, NULL); ;}
    break;

  case 98:
#line 238 "C.y"
    { (yyval) = make_node(RETURN, NULL, NULL); ;}
    break;

  case 99:
#line 239 "C.y"
    { (yyval) = make_node(RETURN, (yyvsp[(2) - (3)]), NULL); ;}
    break;

  case 100:
#line 243 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 101:
#line 244 "C.y"
    { (yyval) = make_node('~', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]));;}
    break;

  case 102:
#line 248 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 103:
#line 249 "C.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 104:
#line 253 "C.y"
    {
          (yyval) = make_node('D', make_node('d', (yyvsp[(1) - (4)]), make_node('e', (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))), (yyvsp[(4) - (4)])); ;}
    break;

  case 105:
#line 255 "C.y"
    {
          (yyval) = make_node('D', make_node('d', (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])), (yyvsp[(3) - (3)])); ;}
    break;

  case 106:
#line 257 "C.y"
    {
          (yyval) = make_node('D', make_node('d', (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])), (yyvsp[(3) - (3)])); ;}
    break;

  case 107:
#line 259 "C.y"
    { (yyval) = make_node('D', (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2117 "C.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 261 "C.y"

#include <stdio.h>

extern char yytext[];
extern int column;

int yyerror(char *s)
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}


