%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "obj.h"
  #include "mem.h"
  #include "fpcode.h"
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
  | expr                                { FPC_RunFpcode(FPC_Create(POP, 0));}

affectation
  : var_dst EQUAL expr                  { FPC_RunFpcode(FPC_Create(AFFECT, 0)); }

call
  : var_src OUVRIR expr_list FERMER     { FPC_RunFpcode(FPC_Create(CALL, 0)); }

expr_list
  : expr                                {/*  TODO */}
  | expr_list COMMA expr                { }

expr
  : var_src                             { }
  | expr DOUBLEPT expr                  { FPC_RunFpcode(FPC_Create(APPLY_OBJ_FUNC, (void *)__ADD__)); printf("LISTE : TODO\n"); }
  | expr PLUS expr                      { FPC_RunFpcode(FPC_Create(APPLY_OBJ_FUNC, (void *)__ADD__)); }
  | expr MOINS expr                     { FPC_RunFpcode(FPC_Create(APPLY_OBJ_FUNC, (void *)__SUB__)); }
  | expr FOIS expr                      { FPC_RunFpcode(FPC_Create(APPLY_OBJ_FUNC, (void *)__MUL__)); }
  | expr DIVISE expr                    { FPC_RunFpcode(FPC_Create(APPLY_OBJ_FUNC, (void *)__FDV__)); }
  | OUVRIR expr FERMER                  { }

var_src
  : VAR                                 { FPC_RunFpcode(FPC_Create(PUSH_SRC_VAR, $1));}
  | ENTIER                              { FPC_RunFpcode(FPC_Create(PUSH_CST, /*CONST OBJ */OBJ_Create(OBJ_INT, $1, NULL))); }
  | STRING                              { FPC_RunFpcode(FPC_Create(PUSH_CST, /*CONST OBJ */OBJ_Create(OBJ_STR, $1, NULL))); }
  | call

var_dst
  : VAR                                 { FPC_RunFpcode(FPC_Create(PUSH_DST_VAR, $1)); }


%%

int yyerror(char *s) {
    printf("yyerror : %s\n",s);
    return 0;
}
