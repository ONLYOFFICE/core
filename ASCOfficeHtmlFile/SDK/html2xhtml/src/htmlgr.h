/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_DOCTYPE = 258,
     TOK_COMMENT = 259,
     TOK_BAD_COMMENT = 260,
     TOK_STAG_INI = 261,
     TOK_ETAG = 262,
     TOK_CDATA = 263,
     TOK_ATT_NAME = 264,
     TOK_ATT_NAMECHAR = 265,
     TOK_ATT_VALUE = 266,
     TOK_EREF = 267,
     TOK_CREF = 268,
     TOK_CDATA_SEC = 269,
     TOK_XMLPI_INI = 270,
     TOK_STAG_END = 271,
     TOK_EMPTYTAG_END = 272,
     TOK_ATT_EQ = 273,
     TOK_XMLPI_END = 274,
     TOK_WHITESPACE = 275,
     TOK_BOUNDARY = 276,
     TOK_PARAM_HEADER = 277,
     TOK_PARAM_CONTENT = 278
   };
#endif
/* Tokens.  */
#define TOK_DOCTYPE 258
#define TOK_COMMENT 259
#define TOK_BAD_COMMENT 260
#define TOK_STAG_INI 261
#define TOK_ETAG 262
#define TOK_CDATA 263
#define TOK_ATT_NAME 264
#define TOK_ATT_NAMECHAR 265
#define TOK_ATT_VALUE 266
#define TOK_EREF 267
#define TOK_CREF 268
#define TOK_CDATA_SEC 269
#define TOK_XMLPI_INI 270
#define TOK_STAG_END 271
#define TOK_EMPTYTAG_END 272
#define TOK_ATT_EQ 273
#define TOK_XMLPI_END 274
#define TOK_WHITESPACE 275
#define TOK_BOUNDARY 276
#define TOK_PARAM_HEADER 277
#define TOK_PARAM_CONTENT 278




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 46 "htmlgr.y"
{
  int  ent;
  char *cad;
}
/* Line 1489 of yacc.c.  */
#line 100 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

