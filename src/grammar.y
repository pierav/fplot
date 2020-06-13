%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "obj.h"
  #include "mem.h"
  #include "fpcode.h"
  #include "prgm.h"
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
| %empty                                { /* *** EOP *** */   PRGM_InitEnd(); }


statement
  : affectation                         { }
  | expr                                { PRGM_InitAdd(FPC_Create(POP, 0));}

affectation
  : var_dst EQUAL expr                  { PRGM_InitAdd(FPC_Create(AFFECT, 0)); }

call
  : var_src OUVRIR expr_list FERMER     { PRGM_InitAdd(FPC_Create(CALL, 0)); }

expr_list
  : expr                                {/*  TODO */}
  | expr_list COMMA expr                { }

expr
  : var_src                             { }
  | expr DOUBLEPT expr                  { PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__ADD__)); printf("LISTE : TODO\n"); }
  | expr PLUS expr                      { PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__ADD__)); }
  | expr MOINS expr                     { PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__SUB__)); }
  | expr FOIS expr                      { PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__MUL__)); }
  | expr DIVISE expr                    { PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__FDV__)); }
  | OUVRIR expr FERMER                  { }

var_src
  : VAR                                 { PRGM_InitAdd(FPC_Create(PUSH_SRC_VAR, $1));}
  | ENTIER                              { PRGM_InitAdd(FPC_Create(PUSH_CST, /*CONST OBJ */OBJ_Create(OBJ_INT, $1, NULL))); }
  | STRING                              { PRGM_InitAdd(FPC_Create(PUSH_CST, /*CONST OBJ */OBJ_Create(OBJ_STR, $1, NULL))); }
  | call

var_dst
  : VAR                                 { PRGM_InitAdd(FPC_Create(PUSH_DST_VAR, $1)); }


%%

int yyerror(char *s) {
    printf("yyerror : %s\n",s);
    return 0;
}
