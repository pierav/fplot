%{
#include "grammar.y.h"
#include <stdio.h>

%}

%option noyywrap
%option nounput

/* Classe des caractères */
DEC					[0-9]
HEX					[a-fA-F0-9]
LET					[a-zA-Z_]
BLANK 			[ \t\n]

/* Type de nombres */
entier			{DEC}+
exposant  	[eE][+-]?{entier}
reel   			{entier}("."{entier})?{exposant}?

/* Espaces et commentaires */
blanks      {BLANK}+
comment2		\/\/[^\n]*

/* Opérations */
plus        \+
moins       \-
fois        \*
divise      \/
ouvrir      \(
fermer      \)

equal       \=
doublept    \:
ptcomma     \;
name        {LET}({LET}|{DEC})*
comma       \,

%%

{blanks}    { /* ignore */ }
"/*" 				{ /* http://web.iitd.ac.in/~sumeet/flex__bison.pdf */
						  int c1 = 0, c2 = input();
						  for (;;) {
						    if (c2 == EOF)
						      break;
						    if (c1 == '*' && c2 == '/')
						      break;
						    c1 = c2;
						    c2 = input();
						  }
							/* ignore */
						}
{comment2}	{ /* ignore */ }

{entier}    { yylval = malloc(4); *(int *)yylval = atoi(yytext); return(ENTIER); }
{plus}      { return(PLUS); }
{moins}     { return(MOINS); }
{fois}      { return(FOIS); }
{divise}    { return(DIVISE); }
{ouvrir}    { return(OUVRIR); }
{fermer}    { return(FERMER); }

{equal}     { return(EQUAL); }
{doublept}  { return(DOUBLEPT); }
{ptcomma}   { return(PTCOMMA); }
{name}      {
				      yylval = malloc(strlen(yytext));
				      strncpy(yylval, yytext, strlen(yytext));
				      return(NAME);
            }
{comma}     { return(COMMA); }

%%
