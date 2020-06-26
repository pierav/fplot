%{
#include "grammar.y.h"
#include <stdio.h>

int oldstate;

%}
%x COMMENT


%option warn

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
equal       \=
doublept    \:
ptcomma     \;
comma       \,

/* Variables */
name        {LET}({LET}|{DEC})*

/* Chaines de caractères */
string 			\"[^\"]*

%option yylineno

%%

"if"     		{ECHO; return(IF); }
"elsif"			{ECHO; return(ELSIF); }
"else"   		{ECHO; return(ELSE); }
"for"				{ECHO;  return(FOR); }
"while"			{ECHO;  return(WHILE); }

"{"					{ECHO;  return(BEG); }
"}"					{ECHO;  return(END); }

"("    			{ECHO;  return(OPAR); }
")"					{ECHO;  return(CPAR); }

"=="				{ECHO;  return(EQ); }
"!="				{ECHO;  return(NE); }
"<="				{ECHO;  return(LE); }
"<" 				{ECHO;  return(LT); }
">="				{ECHO;  return(GE); }
">" 				{ECHO;  return(GT); }

{comment2}	{ /* ignore */ }

"/*"								{ oldstate = YY_START; BEGIN COMMENT; }
<COMMENT>"*/" 			{ BEGIN oldstate; }
<COMMENT>.|\n 			{ ; }
<COMMENT><<EOF>>		{ printf("unclosed comment"); }


{entier}    {
							ECHO;
							yylval = malloc(sizeof(int));
	 						*(int *)yylval = atoi(yytext);
							return(ENTIER);
						}
{string}		{ ECHO; return(STRING);}
{plus}      { ECHO; return(PLUS); }
{moins}     { ECHO; return(MOINS); }
{fois}      { ECHO; return(FOIS); }
{divise}    { ECHO; return(DIVISE); }


{equal}     { ECHO; return(EQUAL); }
{doublept}  { ECHO; return(DOUBLEPT); }
{ptcomma}   { ECHO; return(PTCOMMA); }
{name}      {ECHO;
				      yylval = strdup(yytext);
				      return(VAR);
            }
{comma}     { ECHO; return(COMMA); }


%%
