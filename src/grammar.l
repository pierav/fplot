%{
#include "grammar.y.h"
#include <stdio.h>

int oldstate;
int oldstatestr;

char buf[256];
char *s;

%}
%x F_COMMENT
%x F_STRING

%option warn

%option noyywrap
%option nounput
%option noinput

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
%option yylineno

%%

{comment2}					{ /* ignore */ }
"/*"								{ oldstate = YY_START; BEGIN F_COMMENT; printf("[COMM: "); }
<F_COMMENT>"*/" 		{ BEGIN oldstate; printf("]\n"); }
<F_COMMENT>.|\n 		{ ECHO; }
<F_COMMENT><<EOF>>	{ printf("unclosed comment"); }

"\"" 								{ oldstatestr = YY_START; BEGIN F_STRING; s = buf; }
<F_STRING>"\\n" 		{ *s++ = '\n'; }
<F_STRING>"\\t" 		{ *s++ = '\t'; }
<F_STRING>"\\\\" 		{ *s++ = '\"'; }
<F_STRING>"\"" 			{ *s   = '\0';
											BEGIN oldstatestr;
											yylval = OBJ_string_init(buf, strlen(buf));
											printf("[STR ");
											OBJ_FPrint(stdout, yylval);
											printf("]");
											return(STRING);
										}
<F_STRING>\n 				{ printf("invalid string"); exit(1); }
<F_STRING>. 				{ *s++ = *yytext; }

"if"     		{ECHO; return(IF); }
"elsif"			{ECHO; return(ELSIF); }
"else"   		{ECHO; return(ELSE); }
"for"				{ECHO; return(FOR); }
"while"			{ECHO; return(WHILE); }
"function"	{ECHO; return(FUNC);}
"return"		{ECHO; return(RETURN); }

"{"					{ECHO; return(BEG); }
"}"					{ECHO; return(END); }

"("    			{ECHO; return(OPAR); }
")"					{ECHO; return(CPAR); }

"=="				{ECHO; return(EQ); }
"!="				{ECHO; return(NE); }
"<="				{ECHO; return(LE); }
"<" 				{ECHO; return(LT); }
">="				{ECHO; return(GE); }
">" 				{ECHO; return(GT); }

{entier}    {
	 						yylval = OBJ_int_init(atoi(yytext));
							printf("[INT ");
							OBJ_FPrint(stdout, yylval);
							printf("]");
							return(ENTIER);
						}

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
