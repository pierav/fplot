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

%token EQUAL

%token ENTIER
%left DOUBLEPT
%left PLUS MOINS
%left FOIS DIVISE

%token OPAR CPAR
%token BEG END
%token IF ELSIF ELSE
%token FOR WHILE

%token COMMA
%token PTCOMMA

%token VAR
%token STRING


%start ROOT

%%

ROOT: statements

/* Instrucions */
statements
  : statement statements
  | %empty                                { /* *** EOP *** */   PRGM_InitEnd(); }


statement
  : statement_affectation                         { }
  | statement_condition                           { printf("--end COND\n"); }
  | statement_expression                          { PRGM_InitAdd(FPC_Create(POP, 0));}
  | BEG statements END

statement_expression
  : expr PTCOMMA
  | PTCOMMA

statement_affectation
  : var_dst EQUAL expr PTCOMMA                  { $$ = PRGM_InitAdd(FPC_Create(AFFECT, 0)); }

statement_condition
  : IF OPAR expr CPAR statement                 { $$ = PRGM_InitAdd(FPC_Create(CONDITIONAL_JUMP, ((struct PRGM_NODE*)$5)->index)); printf("--if\n"); }
  | IF OPAR expr CPAR statement ELSE statement  { $$ = PRGM_InitAdd(FPC_Create(CONDITIONAL_JUMP, 0)); printf("--if-else\n"); }

call
  : var_src OPAR expr_list CPAR         { $$ = PRGM_InitAdd(FPC_Create(CALL, 0)); }

expr_list
  : expr                                {/*  TODO */}
  | expr_list COMMA expr                { }

expr
  : var_src                             { }
  | expr DOUBLEPT expr                  { $$ = PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__ADD__)); printf("LISTE : TODO\n"); }
  | expr PLUS expr                      { $$ = PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__ADD__)); }
  | expr MOINS expr                     { $$ = PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__SUB__)); }
  | expr FOIS expr                      { $$ = PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__MUL__)); }
  | expr DIVISE expr                    { $$ = PRGM_InitAdd(FPC_Create(APPLY_OBJ_FUNC, (void *)__FDV__)); }
  | OPAR expr CPAR                      { }

var_src
  : VAR                                 { $$ = PRGM_InitAdd(FPC_Create(PUSH_SRC_VAR, $1));}
  | ENTIER                              { $$ = PRGM_InitAdd(FPC_Create(PUSH_CST, /*CONST OBJ */OBJ_Create(OBJ_INT, $1, NULL))); }
  | STRING                              { $$ = PRGM_InitAdd(FPC_Create(PUSH_CST, /*CONST OBJ */OBJ_Create(OBJ_STR, $1, NULL))); }
  | call

var_dst
  : VAR                                 { PRGM_InitAdd(FPC_Create(PUSH_DST_VAR, $1)); }


%%

int yyerror(char *s) {
    printf("yyerror : %s\n",s);
    return 0;
}
