%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "obj.h"
  #include "mem.h"
  int yyparse();
  int yylex();
  int yyerror(char *s);
%}

%debug
%define api.value.type {void *}

%token ENTIER
%left DOUBLEPT
%left PLUS MOINS
%left FOIS DIVISE

%token OUVRIR
%token FERMER

%token EQUAL
%token PTCOMMA

%token VAR
%token STRING

%token COMMA

%start ROOT

%%

ROOT: statements

/* Instrucions */
statements
  : statement PTCOMMA statements
  | %empty                              { printf("[***  EOP  ***]\n"); }


statement
  : affectation                         { }
  | expr                                { }

affectation
  : var_dst EQUAL expr                  { $$ = OBJ_Affect($1, $3); }

call
  : var_src OUVRIR expr_list FERMER     { $$ = OBJ_Call($1, $3); }

/* Manipulation des objets (TODO)*/
expr_list
: expr                                  { $$ = $1; /*  TODO */}
  | expr_list COMMA expr                { $$ = $1; }

expr
  : var_src                             { $$ = $1;}
  | expr DOUBLEPT expr                  { $$ = OBJ_Add($1, $3); printf("LISTE : TODO\n"); }
  | expr PLUS expr                      { $$ = OBJ_Add($1, $3); }
  | expr MOINS expr                     { $$ = OBJ_Add($1, $3); }
  | expr FOIS expr                      { $$ = OBJ_Add($1, $3); }
  | expr DIVISE expr                    { $$ = OBJ_Add($1, $3); }
  | OUVRIR expr FERMER                  { $$ = $2; }

var_src
  : VAR                                 { $$ = MEM_GetObj($1); if ($$ == NULL){printf("USR# undefined varable : %s", (char *)$1); exit(1);}}
  | ENTIER                              { $$ = OBJ_Create(OBJ_INT, $1, NULL); }
  | STRING                              { $$ = OBJ_Create(OBJ_STR, $1, NULL); }
  | call

var_dst
  : VAR                                 { $$ = MEM_GetOrCreateObj(OBJ_STR, "undefined value", $1); }


%%

int yyerror(char *s) {
    printf("yyerror : %s\n",s);
    return 0;
}
