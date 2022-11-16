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
     OR = 258,
     COMMA = 259,
     SEMICOLON = 260,
     LEFT_PAREN = 261,
     RIGHT_PAREN = 262,
     RIGHT_BRAC = 263,
     LEFT_BRAC = 264,
     ID = 265,
     NEGATION = 266,
     NUMBER = 267,
     FLUENT = 268,
     ACTION = 269,
     IF = 270,
     CAUSES = 271,
     EXECUTABLE = 272,
     IMPOSSIBLE = 273,
     DETERMINE = 274,
     AWAREOF = 275,
     OBSERVES = 276,
     ANNOUNCES = 277,
     INIT = 278,
     GOAL = 279,
     AGENT = 280,
     B = 281,
     C = 282,
     E = 283
   };
#endif
/* Tokens.  */
#define OR 258
#define COMMA 259
#define SEMICOLON 260
#define LEFT_PAREN 261
#define RIGHT_PAREN 262
#define RIGHT_BRAC 263
#define LEFT_BRAC 264
#define ID 265
#define NEGATION 266
#define NUMBER 267
#define FLUENT 268
#define ACTION 269
#define IF 270
#define CAUSES 271
#define EXECUTABLE 272
#define IMPOSSIBLE 273
#define DETERMINE 274
#define AWAREOF 275
#define OBSERVES 276
#define ANNOUNCES 277
#define INIT 278
#define GOAL 279
#define AGENT 280
#define B 281
#define C 282
#define E 283




/* Copy the first part of user declarations.  */
#line 2 "lcp.y"

#include "reader.h"

int yyerror(char *s);
int yylex(void);

string get_negation(const string*);
bool is_consistent(StringList,StringList);
//StringList2 get_negateFluentForm(StringList2);
StringList2 negate_or(StringList);
StringList2 negate_form(StringList2);
StringList2 join_SL2(StringList2, StringList2);
void printStringList(StringList);
void printStringList2(StringList2);

extern Reader reader;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
typedef union YYSTYPE
#line 21 "lcp.y"
{
  string*	str_val;
  StringList*  str_list; 
  StringList2* str_list2;
  Proposition* prop;
  PropositionList* prop_list;
  BFNode* bfnode;
  Nodes* init_nodes;

  MyAction* myaction;
  Actions* actions;
}
/* Line 193 of yacc.c.  */
#line 184 "lcp.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 197 "lcp.tab.c"

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   187

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  29
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNRULES -- Number of states.  */
#define YYNSTATES  168

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   283

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,    11,    12,    15,    19,    21,    26,
      28,    30,    34,    36,    38,    40,    42,    44,    48,    49,
      52,    56,    58,    62,    67,    75,    77,    81,    83,    85,
      89,    91,    93,    96,    98,   102,   104,   108,   112,   116,
     118,   123,   125,   129,   133,   134,   137,   138,   141,   142,
     145,   147,   154,   158,   162,   167,   171,   180,   189,   190,
     193,   197,   203,   208,   213,   218,   224,   230,   235,   237,
     239,   241,   243,   245,   247,   249,   251,   252,   255,   259,
     260,   263,   267,   268
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      30,     0,    -1,    -1,    31,    40,    54,    68,    70,    72,
      -1,    -1,    31,    32,    -1,    13,    35,     5,    -1,    36,
      -1,    36,     6,    39,     7,    -1,    33,    -1,    33,    -1,
      35,     4,    33,    -1,    10,    -1,    12,    -1,    36,    -1,
      37,    -1,    38,    -1,    39,     4,    38,    -1,    -1,    40,
      41,    -1,    14,    42,     5,    -1,    43,    -1,    42,     4,
      43,    -1,    36,     9,    44,     8,    -1,    36,     9,    44,
       8,     6,    46,     7,    -1,    45,    -1,    44,     4,    45,
      -1,    36,    -1,    47,    -1,    46,     4,    47,    -1,    36,
      -1,    33,    -1,    11,    33,    -1,    48,    -1,    49,     4,
      48,    -1,    48,    -1,    50,     4,    50,    -1,    50,     3,
      50,    -1,     6,    50,     7,    -1,    36,    -1,    36,     6,
      39,     7,    -1,    51,    -1,    52,     4,    51,    -1,    25,
      52,     5,    -1,    -1,    54,    53,    -1,    -1,    15,    49,
      -1,    -1,    15,    57,    -1,    50,    -1,    26,     6,    51,
       4,    57,     7,    -1,    57,     4,    57,    -1,    57,     3,
      57,    -1,     6,    11,    57,     7,    -1,     6,    57,     7,
      -1,    28,     6,     9,    52,     8,     4,    57,     7,    -1,
      27,     6,     9,    52,     8,     4,    57,     7,    -1,    -1,
      15,    50,    -1,    49,    55,     5,    -1,    43,    16,    49,
      56,     5,    -1,    17,    43,    56,     5,    -1,    43,    19,
      34,     5,    -1,    43,    22,    50,     5,    -1,    51,    20,
      43,    58,     5,    -1,    51,    21,    43,    58,     5,    -1,
      18,    43,    55,     5,    -1,    59,    -1,    60,    -1,    61,
      -1,    66,    -1,    62,    -1,    63,    -1,    65,    -1,    64,
      -1,    -1,    68,    67,    -1,    23,    57,     5,    -1,    -1,
      70,    69,    -1,    24,    57,     5,    -1,    -1,    72,    71,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   125,   125,   127,   145,   149,   155,   162,   166,   172,
     178,   183,   192,   199,   203,   210,   220,   224,   232,   236,
     242,   247,   253,   260,   267,   276,   281,   287,   292,   297,
     303,   310,   314,   320,   326,   332,   342,   360,   364,   372,
     376,   381,   386,   392,   398,   402,   412,   416,   423,   428,
     433,   439,   446,   454,   462,   469,   476,   483,   494,   498,
     505,   516,   529,   541,   553,   565,   578,   591,   603,   607,
     612,   617,   622,   627,   632,   637,   646,   649,   658,   665,
     671,   717,   724,   727
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OR", "COMMA", "SEMICOLON", "LEFT_PAREN",
  "RIGHT_PAREN", "RIGHT_BRAC", "LEFT_BRAC", "ID", "NEGATION", "NUMBER",
  "FLUENT", "ACTION", "IF", "CAUSES", "EXECUTABLE", "IMPOSSIBLE",
  "DETERMINE", "AWAREOF", "OBSERVES", "ANNOUNCES", "INIT", "GOAL", "AGENT",
  "B", "C", "E", "$accept", "input", "fluent_decls", "fluent_decl",
  "fluent", "fluent_det_list", "fluent_list", "id", "constant", "param",
  "param_list", "action_decls", "action_decl", "action_list", "action",
  "executors", "executor", "observers", "observer", "literal",
  "literal_list", "formula", "agent", "agent_list", "agent_decl",
  "agent_decls", "if_part", "if_part_bf", "belief_formula",
  "if_part_fluent", "static_law", "dynamic_law", "executability",
  "determine", "announcement", "awareness", "observance", "impossibility",
  "proposition", "domain", "init", "init_spec", "goal", "goal_spec", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    29,    30,    30,    31,    31,    32,    33,    33,    34,
      35,    35,    36,    37,    37,    38,    39,    39,    40,    40,
      41,    42,    42,    43,    43,    44,    44,    45,    46,    46,
      47,    48,    48,    49,    49,    50,    50,    50,    50,    51,
      51,    52,    52,    53,    54,    54,    55,    55,    56,    56,
      57,    57,    57,    57,    57,    57,    57,    57,    58,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    67,
      67,    67,    67,    67,    67,    67,    68,    68,    69,    70,
      70,    71,    72,    72
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     6,     0,     2,     3,     1,     4,     1,
       1,     3,     1,     1,     1,     1,     1,     3,     0,     2,
       3,     1,     3,     4,     7,     1,     3,     1,     1,     3,
       1,     1,     2,     1,     3,     1,     3,     3,     3,     1,
       4,     1,     3,     3,     0,     2,     0,     2,     0,     2,
       1,     6,     3,     3,     4,     3,     8,     8,     0,     2,
       3,     5,     4,     4,     4,     5,     5,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     2,     3,     0,
       2,     3,     0,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,    18,     1,     0,     5,    44,    12,    10,     0,
       7,     0,    19,    76,     0,     6,     0,     0,     0,    21,
       0,    45,    79,    11,    13,    14,    15,    16,     0,     0,
       0,    20,    39,    41,     0,     0,     0,     0,    31,     7,
       0,    33,    46,     0,    68,    69,    70,    72,    73,    75,
      74,    71,    77,    82,     0,     8,    27,     0,    25,    22,
       0,     0,    43,    32,    48,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     3,    17,     0,
      23,     0,    42,     0,     0,     0,     0,    48,     9,     0,
       0,    35,     0,    34,    47,    60,    58,    58,     0,     0,
       0,     0,    50,     0,     0,    83,    26,     0,    40,    49,
      62,    67,     8,     0,    63,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,    30,     0,    28,    61,    38,    37,    36,    59,
      65,    66,    31,     0,    55,     0,     0,     0,    53,    52,
      81,     0,    24,    54,     0,     0,     0,    29,     0,     0,
       0,    51,     0,     0,     0,     0,    57,    56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     5,    38,    89,     9,    10,    26,    27,
      28,     6,    12,    18,    19,    57,    58,   133,   134,    91,
      42,   102,    33,    34,    21,    13,    72,    84,   103,   120,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    22,
      76,    53,   105,    77
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -78
static const yytype_int16 yypact[] =
{
      20,    56,    82,   -78,    91,   -78,    89,   -78,   -78,    -1,
     149,    91,   -78,   132,    91,   -78,    66,   101,    24,   -78,
      91,   -78,    70,   -78,   -78,   -78,   -78,   -78,   118,    91,
      91,   -78,   150,   -78,    30,    91,    91,    91,   -78,    33,
      78,   -78,    32,   125,   -78,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78,   121,    66,   -78,   -78,    40,   -78,   -78,
      66,    91,   -78,   -78,   143,   144,    66,   137,    91,    73,
     137,   137,   155,    91,    91,     4,   -78,   139,   -78,    91,
     156,   119,   -78,     4,   159,   160,   120,    67,   -78,   161,
      73,   -78,   129,   -78,   157,   -78,   152,   152,    64,   162,
     163,   164,   126,   134,     4,   -78,   -78,    91,   -78,   146,
     -78,   -78,   131,   166,   -78,    54,    73,    73,   -78,    73,
     167,   168,     4,    54,    95,    91,   165,   169,     4,     4,
     -78,   138,   -78,   124,   -78,   -78,   -78,   171,   -78,   126,
     -78,   -78,   -78,   102,   -78,   172,    91,    91,   173,   -78,
     -78,    91,   -78,   -78,     4,    85,   111,   -78,   104,   175,
     176,   -78,     4,     4,   109,   114,   -78,   -78
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -78,   -78,   -78,   -78,    -2,   -78,   -78,   -11,   -78,   127,
     -53,   -78,   -78,   -78,   -14,   -78,   103,   -78,    34,     2,
     -30,   -52,   -21,     7,   -78,   -78,   122,    96,   -77,    87,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78,   -78
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -41
static const yytype_int16 yytable[] =
{
      17,    43,     8,    14,    15,    25,   109,    81,    40,    32,
      98,    39,    23,    86,     7,    35,    59,    92,    56,    17,
      -2,   124,    64,    65,    41,    17,    17,   131,    30,    31,
      99,   100,   101,    63,    61,    62,    70,    87,   115,    66,
      82,    94,    29,    25,    79,   143,   123,    71,    80,    25,
      32,   148,   149,   -39,   -39,    25,     3,   116,   117,    96,
      97,   136,    17,    17,   137,   138,    88,   139,    56,    41,
      98,    70,    93,    41,     7,   122,     7,   158,    24,    90,
       7,    35,    83,     7,    35,   164,   165,    36,    37,    61,
      99,   100,   101,   159,    67,     4,   132,    68,   128,   129,
      69,     7,   144,    11,   145,   128,   129,   128,   129,   153,
      29,   161,   128,   129,    32,    61,   166,   128,   129,   160,
     142,   167,    54,    54,    54,    55,   108,   112,   151,   116,
     117,   152,   116,   117,   118,    32,    32,   128,   129,   130,
     132,   128,   129,   150,    75,    73,    74,     7,    35,   128,
     129,   -40,   -40,   155,   156,    16,    60,    20,    83,    71,
      95,    70,   107,   104,   110,   111,   114,   119,   125,   126,
     127,   135,   140,   141,   146,   117,   154,   129,   147,   162,
     163,    78,   106,   113,   121,   157,     0,    85
};

static const yytype_int16 yycheck[] =
{
      11,    22,     4,     4,     5,    16,    83,    60,    22,    20,
       6,    22,    14,    66,    10,    11,    30,    69,    29,    30,
       0,    98,    36,    37,    22,    36,    37,   104,     4,     5,
      26,    27,    28,    35,     4,     5,     4,    67,    90,     6,
      61,    71,     9,    54,     4,   122,    98,    15,     8,    60,
      61,   128,   129,    20,    21,    66,     0,     3,     4,    73,
      74,     7,    73,    74,   116,   117,    68,   119,    79,    67,
       6,     4,    70,    71,    10,    11,    10,   154,    12,     6,
      10,    11,    15,    10,    11,   162,   163,    17,    18,     4,
      26,    27,    28,     8,    16,    13,   107,    19,     3,     4,
      22,    10,     7,    14,   125,     3,     4,     3,     4,     7,
       9,     7,     3,     4,   125,     4,     7,     3,     4,     8,
     122,     7,     4,     4,     4,     7,     7,     7,     4,     3,
       4,     7,     3,     4,     5,   146,   147,     3,     4,     5,
     151,     3,     4,     5,    23,    20,    21,    10,    11,     3,
       4,    20,    21,   146,   147,     6,     6,    25,    15,    15,
       5,     4,     6,    24,     5,     5,     5,    15,     6,     6,
       6,     5,     5,     5,     9,     4,     4,     4,     9,     4,
       4,    54,    79,    87,    97,   151,    -1,    65
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    30,    31,     0,    13,    32,    40,    10,    33,    35,
      36,    14,    41,    54,     4,     5,     6,    36,    42,    43,
      25,    53,    68,    33,    12,    36,    37,    38,    39,     9,
       4,     5,    36,    51,    52,    11,    17,    18,    33,    36,
      43,    48,    49,    51,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    70,     4,     7,    36,    44,    45,    43,
       6,     4,     5,    33,    43,    43,     6,    16,    19,    22,
       4,    15,    55,    20,    21,    23,    69,    72,    38,     4,
       8,    39,    51,    15,    56,    55,    39,    49,    33,    34,
       6,    48,    50,    48,    49,     5,    43,    43,     6,    26,
      27,    28,    50,    57,    24,    71,    45,     6,     7,    57,
       5,     5,     7,    56,     5,    50,     3,     4,     5,    15,
      58,    58,    11,    50,    57,     6,     6,     6,     3,     4,
       5,    57,    36,    46,    47,     5,     7,    50,    50,    50,
       5,     5,    33,    57,     7,    51,     9,     9,    57,    57,
       5,     4,     7,     7,     4,    52,    52,    47,    57,     8,
       8,     7,     4,     4,    57,    57,     7,     7
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
        case 3:
#line 132 "lcp.y"
    { 
    reader.m_fluents = *(yyvsp[(1) - (6)].str_list);
    reader.m_actions = *(yyvsp[(2) - (6)].actions);
    reader.m_agents = *(yyvsp[(3) - (6)].str_list);
    reader.m_propositions = *(yyvsp[(4) - (6)].prop_list);
    reader.k_init = *(yyvsp[(5) - (6)].init_nodes);
    reader.m_gd = *(yyvsp[(6) - (6)].init_nodes);
  ;}
    break;

  case 4:
#line 145 "lcp.y"
    {
    (yyval.str_list) = new StringList;
  ;}
    break;

  case 5:
#line 149 "lcp.y"
    {
    (yyvsp[(1) - (2)].str_list)->insert((yyvsp[(2) - (2)].str_list)->begin(),(yyvsp[(2) - (2)].str_list)->end());
    (yyval.str_list) = (yyvsp[(1) - (2)].str_list);
  ;}
    break;

  case 6:
#line 155 "lcp.y"
    {
    (yyval.str_list) = (yyvsp[(2) - (3)].str_list);
  ;}
    break;

  case 7:
#line 162 "lcp.y"
    {
    (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
  ;}
    break;

  case 8:
#line 167 "lcp.y"
    {
    (yyval.str_val) = new std::string(*(yyvsp[(1) - (4)].str_val) + "(" + *(yyvsp[(3) - (4)].str_val) + ")");
  ;}
    break;

  case 9:
#line 172 "lcp.y"
    {
    (yyval.str_list) = new StringList;
    (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
  ;}
    break;

  case 10:
#line 178 "lcp.y"
    {
    (yyval.str_list) = new StringList;
    (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
  ;}
    break;

  case 11:
#line 183 "lcp.y"
    {
    (yyval.str_list) = (yyvsp[(1) - (3)].str_list);
    (yyval.str_list)->insert(*(yyvsp[(3) - (3)].str_val));
  ;}
    break;

  case 12:
#line 192 "lcp.y"
    {
    (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
  ;}
    break;

  case 13:
#line 199 "lcp.y"
    {
    (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
  ;}
    break;

  case 14:
#line 203 "lcp.y"
    {
    (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
  ;}
    break;

  case 15:
#line 211 "lcp.y"
    {
    (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
  ;}
    break;

  case 16:
#line 220 "lcp.y"
    {
    (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
  ;}
    break;

  case 17:
#line 225 "lcp.y"
    {
    (yyval.str_val) = new string(*(yyvsp[(1) - (3)].str_val) + "," + *(yyvsp[(3) - (3)].str_val));
  ;}
    break;

  case 18:
#line 232 "lcp.y"
    {
    (yyval.actions) = new Actions;
  ;}
    break;

  case 19:
#line 236 "lcp.y"
    {
    (yyvsp[(1) - (2)].actions)->insert((yyvsp[(1) - (2)].actions)->end(), (yyvsp[(2) - (2)].actions)->begin(),(yyvsp[(2) - (2)].actions)->end());
    (yyval.actions) = (yyvsp[(1) - (2)].actions);
  ;}
    break;

  case 20:
#line 242 "lcp.y"
    {
    (yyval.actions) = (yyvsp[(2) - (3)].actions);
  ;}
    break;

  case 21:
#line 247 "lcp.y"
    {
    (yyval.actions) = new Actions;
    (yyval.actions)->insert((yyval.actions)->begin(),*(yyvsp[(1) - (1)].myaction));
    // $1->print();
  ;}
    break;

  case 22:
#line 253 "lcp.y"
    {
    (yyval.actions) = (yyvsp[(1) - (3)].actions);
    (yyval.actions)->insert((yyvsp[(1) - (3)].actions)->begin(), *(yyvsp[(3) - (3)].myaction));
    // $3->print();
  ;}
    break;

  case 23:
#line 260 "lcp.y"
    {
    // $$ = new string(*$1 + "[" + *$3 + "]");
    (yyval.myaction) = new MyAction;
    (yyval.myaction)->name = *(yyvsp[(1) - (4)].str_val);
    (yyval.myaction)->executors = *(yyvsp[(3) - (4)].str_list);
  ;}
    break;

  case 24:
#line 267 "lcp.y"
    {
    // $$ = new string(*$1 + "[" + *$3 + "]" + "(" + *$6 + ")");
    (yyval.myaction) = new MyAction;
    (yyval.myaction)->name = *(yyvsp[(1) - (7)].str_val);
    (yyval.myaction)->observers = *(yyvsp[(6) - (7)].str_list);
    (yyval.myaction)->executors = *(yyvsp[(3) - (7)].str_list);
  ;}
    break;

  case 25:
#line 276 "lcp.y"
    {
    (yyval.str_list) = new StringList;
    (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
  ;}
    break;

  case 26:
#line 281 "lcp.y"
    {
    (yyval.str_list) = (yyvsp[(1) - (3)].str_list);
    (yyval.str_list)->insert(*(yyvsp[(3) - (3)].str_val));
  ;}
    break;

  case 27:
#line 287 "lcp.y"
    {
    (yyval.str_val) = new string(*(yyvsp[(1) - (1)].str_val));
  ;}
    break;

  case 28:
#line 292 "lcp.y"
    {
    (yyval.str_list) = new StringList;
    (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
  ;}
    break;

  case 29:
#line 297 "lcp.y"
    {
    (yyval.str_list) = (yyvsp[(1) - (3)].str_list);
    (yyval.str_list)->insert(*(yyvsp[(3) - (3)].str_val));
  ;}
    break;

  case 30:
#line 303 "lcp.y"
    {
    (yyval.str_val) = new string(*(yyvsp[(1) - (1)].str_val));
  ;}
    break;

  case 31:
#line 310 "lcp.y"
    {
    (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
  ;}
    break;

  case 32:
#line 315 "lcp.y"
    {
    (yyval.str_val) = new std::string(*(yyvsp[(1) - (2)].str_val) + *(yyvsp[(2) - (2)].str_val));
  ;}
    break;

  case 33:
#line 321 "lcp.y"
    {
    (yyval.str_list) = new StringList;
    (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
  ;}
    break;

  case 34:
#line 326 "lcp.y"
    {
    (yyval.str_list) = (yyvsp[(1) - (3)].str_list);  
    (yyval.str_list)->insert(*(yyvsp[(3) - (3)].str_val));
  ;}
    break;

  case 35:
#line 332 "lcp.y"
    {
    StringList s1;

    (yyval.str_list2) = new StringList2;

    s1.insert(*(yyvsp[(1) - (1)].str_val));

    (yyval.str_list2)->insert(s1);
  ;}
    break;

  case 36:
#line 343 "lcp.y"
    {
    StringList2::iterator it1;
    StringList2::iterator it2;
    StringList ns;

    (yyval.str_list2) = new StringList2;

    for (it2 = (yyvsp[(1) - (3)].str_list2)->begin(); it2 != (yyvsp[(1) - (3)].str_list2)->end(); it2++) {
      for (it1 = (yyvsp[(3) - (3)].str_list2)->begin(); it1 != (yyvsp[(3) - (3)].str_list2)->end(); it1++){
      if (is_consistent(*it2,*it1)) {
      ns = *it2;
      ns.insert(it1->begin(),it1->end());
      (yyval.str_list2)->insert(ns);
      }
      }
    }  
  ;}
    break;

  case 37:
#line 360 "lcp.y"
    {
    (yyval.str_list2) = (yyvsp[(1) - (3)].str_list2);
    (yyval.str_list2)->insert((yyvsp[(3) - (3)].str_list2)->begin(),(yyvsp[(3) - (3)].str_list2)->end());
  ;}
    break;

  case 38:
#line 365 "lcp.y"
    {
    (yyval.str_list2) = (yyvsp[(2) - (3)].str_list2);
  ;}
    break;

  case 39:
#line 372 "lcp.y"
    {
    (yyval.str_val) = new string(*(yyvsp[(1) - (1)].str_val));
  ;}
    break;

  case 40:
#line 376 "lcp.y"
    {
    (yyval.str_val) = new string(*(yyvsp[(1) - (4)].str_val) + "(" + *(yyvsp[(3) - (4)].str_val) + ")");
  ;}
    break;

  case 41:
#line 381 "lcp.y"
    {
    (yyval.str_list) = new StringList;
    (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
  ;}
    break;

  case 42:
#line 386 "lcp.y"
    {
    (yyval.str_list) = (yyvsp[(1) - (3)].str_list);
    (yyval.str_list)->insert(*(yyvsp[(3) - (3)].str_val));
  ;}
    break;

  case 43:
#line 392 "lcp.y"
    {
    (yyval.str_list) = (yyvsp[(2) - (3)].str_list);
  ;}
    break;

  case 44:
#line 398 "lcp.y"
    {
    (yyval.str_list) = new StringList;
  ;}
    break;

  case 45:
#line 403 "lcp.y"
    {
    (yyvsp[(1) - (2)].str_list)->insert((yyvsp[(2) - (2)].str_list)->begin(),(yyvsp[(2) - (2)].str_list)->end());
    (yyval.str_list) = (yyvsp[(1) - (2)].str_list);
  ;}
    break;

  case 46:
#line 412 "lcp.y"
    {
    (yyval.str_list) = new StringList;
  ;}
    break;

  case 47:
#line 416 "lcp.y"
    {
    (yyval.str_list) = (yyvsp[(2) - (2)].str_list);
  ;}
    break;

  case 48:
#line 423 "lcp.y"
    {
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = BFEmpty;
  ;}
    break;

  case 49:
#line 428 "lcp.y"
    {
    (yyval.bfnode) = (yyvsp[(2) - (2)].bfnode);
  ;}
    break;

  case 50:
#line 433 "lcp.y"
    {  
      (yyval.bfnode) = new BFNode;
      (yyval.bfnode)->node_type = fluForm;
      (yyval.bfnode)->flu_form = *(yyvsp[(1) - (1)].str_list2);
  ;}
    break;

  case 51:
#line 439 "lcp.y"
    {
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = BForm;
    (yyval.bfnode)->agentPro = *(yyvsp[(3) - (6)].str_val);
    (yyval.bfnode)->bfnode1 = (yyvsp[(5) - (6)].bfnode);
  ;}
    break;

  case 52:
#line 446 "lcp.y"
    {
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = propForm;
    (yyval.bfnode)->bfnode1 = (yyvsp[(1) - (3)].bfnode);
    (yyval.bfnode)->bfnode2 = (yyvsp[(3) - (3)].bfnode);
    (yyval.bfnode)->bfOperator = BFAND;
  ;}
    break;

  case 53:
#line 454 "lcp.y"
    {
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = propForm;
    (yyval.bfnode)->bfnode1 = (yyvsp[(1) - (3)].bfnode);
    (yyval.bfnode)->bfnode2 = (yyvsp[(3) - (3)].bfnode);
    (yyval.bfnode)->bfOperator = BFOR;
  ;}
    break;

  case 54:
#line 462 "lcp.y"
    {
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = propForm;
    (yyval.bfnode)->bfnode1 = (yyvsp[(3) - (4)].bfnode);
    (yyval.bfnode)->bfOperator = BFNOT;
  ;}
    break;

  case 55:
#line 469 "lcp.y"
    {
      (yyval.bfnode) = new BFNode;
      (yyval.bfnode)->node_type = propForm;
      (yyval.bfnode)->bfnode1 = (yyvsp[(2) - (3)].bfnode);
      (yyval.bfnode)->bfOperator = NONE;
  ;}
    break;

  case 56:
#line 476 "lcp.y"
    {
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = EForm;
    (yyval.bfnode)->groupAgent = *(yyvsp[(4) - (8)].str_list);
    (yyval.bfnode)->bfnode1 = (yyvsp[(7) - (8)].bfnode);
  ;}
    break;

  case 57:
#line 483 "lcp.y"
    {
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = CForm;
    (yyval.bfnode)->groupAgent = *(yyvsp[(4) - (8)].str_list);
    (yyval.bfnode)->bfnode1 = (yyvsp[(7) - (8)].bfnode);
  ;}
    break;

  case 58:
#line 494 "lcp.y"
    {
    (yyval.str_list2) = new StringList2;
  ;}
    break;

  case 59:
#line 498 "lcp.y"
    {
    (yyval.str_list2) = (yyvsp[(2) - (2)].str_list2);
  ;}
    break;

  case 60:
#line 506 "lcp.y"
    {
    (yyval.prop) = new Proposition;
    (yyval.prop)->n_type = STATIC;
    (yyval.prop)->precond = *(yyvsp[(2) - (3)].str_list);
    (yyval.prop)->effect = *(yyvsp[(1) - (3)].str_list);
  ;}
    break;

  case 61:
#line 517 "lcp.y"
    {  
    (yyval.prop) = new Proposition;
    (yyval.prop)->n_type = DYNAMIC;
    // $$->act_name = $1;
    (yyval.prop)->act = *(yyvsp[(1) - (5)].myaction);
    (yyval.prop)->bel_form = *(yyvsp[(4) - (5)].bfnode);
    (yyval.prop)->effect = *(yyvsp[(3) - (5)].str_list);
  ;}
    break;

  case 62:
#line 530 "lcp.y"
    {
    (yyval.prop) = new Proposition;
    (yyval.prop)->n_type = EXECUTABILITY;
    // $$->act_name = *$2;
    (yyval.prop)->act = *(yyvsp[(2) - (4)].myaction);
    (yyval.prop)->bel_form = *(yyvsp[(3) - (4)].bfnode);
  ;}
    break;

  case 63:
#line 542 "lcp.y"
    {
    (yyval.prop) = new Proposition;
    (yyval.prop)->n_type = DETERMINATION;
    (yyval.prop)->act = *(yyvsp[(1) - (4)].myaction);
    // $$->act_name = *$1;
    (yyval.prop)->effect = *(yyvsp[(3) - (4)].str_list); 
  ;}
    break;

  case 64:
#line 554 "lcp.y"
    {
    (yyval.prop) = new Proposition;
    (yyval.prop)->n_type = ANNOUNCEMENT;
    // $$->act_name = *$1;
    (yyval.prop)->act = *(yyvsp[(1) - (4)].myaction);
    (yyval.prop)->flu_form = *(yyvsp[(3) - (4)].str_list2);
  ;}
    break;

  case 65:
#line 566 "lcp.y"
    {
    (yyval.prop) = new Proposition;
    (yyval.prop)->n_type = AWARENESS;
    // $$->act_name = *$3;
    (yyval.prop)->act = *(yyvsp[(3) - (5)].myaction);
    (yyval.prop)->agentPro = *(yyvsp[(1) - (5)].str_val);
    (yyval.prop)->flu_form = *(yyvsp[(4) - (5)].str_list2);
  ;}
    break;

  case 66:
#line 579 "lcp.y"
    {
    (yyval.prop) = new Proposition;
    (yyval.prop)->n_type = OBSERVANCE;
    // $$->act_name = *$3;				
    (yyval.prop)->act = *(yyvsp[(3) - (5)].myaction);				
    (yyval.prop)->agentPro = *(yyvsp[(1) - (5)].str_val);
    (yyval.prop)->flu_form = *(yyvsp[(4) - (5)].str_list2);
  ;}
    break;

  case 67:
#line 592 "lcp.y"
    {
    (yyval.prop) = new Proposition;
    (yyval.prop)->n_type = IMPOSSIBILITY;
    // $$->act_name = *$2;
    (yyval.prop)->act = *(yyvsp[(2) - (4)].myaction);
    (yyval.prop)->precond = *(yyvsp[(3) - (4)].str_list);
  ;}
    break;

  case 68:
#line 603 "lcp.y"
    {
    (yyval.prop) = (yyvsp[(1) - (1)].prop);
  ;}
    break;

  case 69:
#line 608 "lcp.y"
    {
    (yyval.prop) = (yyvsp[(1) - (1)].prop);
  ;}
    break;

  case 70:
#line 613 "lcp.y"
    {
    (yyval.prop) = (yyvsp[(1) - (1)].prop);
  ;}
    break;

  case 71:
#line 618 "lcp.y"
    {
    (yyval.prop) = (yyvsp[(1) - (1)].prop);
  ;}
    break;

  case 72:
#line 623 "lcp.y"
    {
    (yyval.prop) = (yyvsp[(1) - (1)].prop);
  ;}
    break;

  case 73:
#line 628 "lcp.y"
    {
    (yyval.prop) = (yyvsp[(1) - (1)].prop);
  ;}
    break;

  case 74:
#line 633 "lcp.y"
    {
    (yyval.prop) = (yyvsp[(1) - (1)].prop);
  ;}
    break;

  case 75:
#line 638 "lcp.y"
    {
    (yyval.prop) = (yyvsp[(1) - (1)].prop);
  ;}
    break;

  case 76:
#line 646 "lcp.y"
    {
    (yyval.prop_list) = new PropositionList;
  ;}
    break;

  case 77:
#line 650 "lcp.y"
    {
    (yyval.prop_list) = (yyvsp[(1) - (2)].prop_list);
    (yyvsp[(1) - (2)].prop_list)->push_back(*(yyvsp[(2) - (2)].prop));
  ;}
    break;

  case 78:
#line 659 "lcp.y"
    {
    (yyval.bfnode) = (yyvsp[(2) - (3)].bfnode);
  ;}
    break;

  case 79:
#line 665 "lcp.y"
    {
    (yyval.init_nodes) = new Nodes;
    //$$->insert(bfnode());
    //$$ = new StringList2;
    //$$->insert(StringList());
  ;}
    break;

  case 80:
#line 672 "lcp.y"
    {
    (yyval.init_nodes) = (yyvsp[(1) - (2)].init_nodes);
    (yyval.init_nodes)->push_back(*(yyvsp[(2) - (2)].bfnode));
  ;}
    break;

  case 81:
#line 718 "lcp.y"
    {
    (yyval.bfnode) = (yyvsp[(2) - (3)].bfnode);
  ;}
    break;

  case 82:
#line 724 "lcp.y"
    {
    (yyval.init_nodes) = new Nodes;
  ;}
    break;

  case 83:
#line 728 "lcp.y"
    {
    (yyval.init_nodes) = (yyvsp[(1) - (2)].init_nodes);
    (yyval.init_nodes)->push_back(*(yyvsp[(2) - (2)].bfnode));
  ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2227 "lcp.tab.c"
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


#line 733 "lcp.y"


int yyerror(string s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  exit(1);
  return 0;
}

int yyerror(char *s)
{
  return yyerror(string(s));
}

bool is_consistent(StringList sl1, StringList sl2)
{
  StringList::const_iterator it;
  string nl;

  for (it = sl2.begin(); it != sl2.end(); it++) {
	nl = get_negation(&(*it));
	if (sl1.find(nl) != sl1.end())
	  return false;
  }

  return true;
}

string get_negation(const string* s)
{
  string ns;

  if (s->substr(0,1) == NEGATION_SYMBOL) {
	return s->substr(1);
  }
  ns = NEGATION_SYMBOL;
  return ns.append(*s);
}

/*
StringList2 get_negateFluentForm(StringList2 input){
  
  StringList2 separate;
  StringList2 join;
  StringList2::iterator it1;
  StringList2::iterator it3;
  StringList2 negation;
  string temp;
  StringList::const_iterator it2;

  for(it1 = input.begin(); it1 != input.end(); it1++){
     if(it1->begin() == it1->end())
        join.insert(*it1);
     else
        separate.insert(*it1);
  }//for loop

  //Separate elements in separate
     for(it1 = separate.begin(); it1 != separate.end(); it1++){
        temp = get_negation(&(*(it1->begin())));    //possible pointer problem
        StringList tiep;
	tiep.insert(temp);
	negation.insert(tiep);
     }//for loop
  

  //Join elements in join with all elements in separate
  for(it3 = negation.begin(); it3 != negation.end(); it3++)
     for(it1 = join.begin(); it1 != join.end(); it1++)
        for(it2 = it1->begin(); it2 != it1->end(); it2++)
        {
           temp = get_negation(&(*it2));    //possible pointer problem
           StringList tiep;
           tiep.insert(temp);
           negation.insert(tiep);
	}
  
  return negation;
}
*/

//negate_or: input: String list = list of or. 
//             output: Stringlist 2 = list of and of negation

StringList2 negate_or(StringList input){
   
   StringList::iterator it;
   StringList2 output;
   string element;
   
   for(it = input.begin(); it != input.end(); it++){
      StringList temp;
      element = get_negation(&(*it));
      temp.insert(element);
      output.insert(temp);
   }
   //printStringList2(output);
   return output;
}


// or_2_stringlist2

//negate_and : input: stringlist2 = list of and of or
//		negate_or(each member of input) = a stringlist 2
//                -> n stringlist 2 -> stringlist 3
//                output = first member stirnglist 3 or second member of stringlist 3

StringList2 join_SL2(StringList2 input1, StringList2 input2){
  
  if(input2.size() == 0){
     return input1;
  }

  StringList2::iterator it1;
  StringList2::iterator it2;
  StringList ns;

  StringList2 output;

  for (it2 = input1.begin(); it2 != input1.end(); it2++) {
    for (it1 = input2.begin(); it1 != input2.end(); it1++){
      if (is_consistent(*it1,*it2)) {
	ns = *it2;
	ns.insert(it1->begin(),it1->end());
	output.insert(ns);
      }
    }
  }  
 
  return output;
   
}

StringList2 negate_form(StringList2 input){
   
  typedef set<StringList2> StringList3;
  StringList3 list3;
  StringList2::iterator it1;
  StringList2::iterator it2;
  StringList3::iterator it3;
  StringList ns;
  StringList2 temp;

  StringList2 output;

  //turn all the “or” statements to “and” statements
   for(it1 = input.begin(); it1 != input.end(); it1++){
      temp = negate_or(*it1);
      list3.insert(temp);
   }

   
   output = *list3.begin();
   for(it3 = ++list3.begin(); it3 != list3.end(); it3++){
      output = join_SL2(output, *it3); 
   }

   
   return output;
}

void printStringList(StringList in){
	StringList::iterator it1;
	cout << "[ " ;
        for(it1 = in.begin();it1!=in.end();it1++){
		cout << *it1 << " , ";   
	}
	cout << "] " ;
}

void printStringList2(StringList2 in){
	StringList2::iterator it1;
	cout << "[ "; 
        for(it1 = in.begin();it1!=in.end();it1++){
 		 
		printStringList(*it1);
		cout << " , ";   
	}
	cout << " ] " ;
}

//Planning as Logic

