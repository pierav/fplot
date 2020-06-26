%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "obj.h"
  #include "mem.h"
  #include "fpcode.h"
  //#include "prgm.h"
  #include "ast_displayer.h"
  #include "ast_traversal.h"

  int yyparse(AST_NODE **root);
  int yyerror(AST_NODE **root, char *s);
  int yylex();
%}
%code requires {
  #include "ast.h"
}


%debug
%define api.value.type {void *}

%parse-param {AST_NODE **root}

// union /!\


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

ROOT: statements                        {
                                          *root = $1;
                                        }
/* Instrucions */
statements
  : statement statements                { $$ = AST_NODE_STAT_Create($2/*next*/,$1); }
  | %empty                              { $$ = NULL;/* *** EOP *** */   }


statement
  : statement_affectation               { $$ = $1; }
  | statement_condition                 { $$ = $1; }
  | statement_while                     { $$ = $1; }
  | statement_expression                { $$ = AST_NODE_PCODE_Create(PC_CreatePop(), $1, NULL);}
  | BEG statements END                  { $$ = $2; }

statement_expression
  : expr PTCOMMA                        { $$ = $1; }
  | PTCOMMA

statement_affectation
  : var_dst EQUAL expr PTCOMMA          { $$ = AST_NODE_PCODE_Create(PC_CreateAffect(), $1, $3); }

statement_condition
  : IF OPAR expr CPAR statement                 { $$ = AST_NODE_IF_Create($3, $5, NULL); }
  | IF OPAR expr CPAR statement ELSE statement  { $$ = AST_NODE_IF_Create($3, $5, $7); }

statement_while
  : WHILE OPAR expr CPAR statement      { $$ = AST_NODE_WHILE_Create($3, $5); }

call
  : var_src OPAR expr_list CPAR         { $$ = AST_NODE_PCODE_Create(PC_Create(CALL, (PC_ARG)0UL), $1, $3); }

expr_list
  : expr                                {/*  TODO */}
  | expr_list COMMA expr                { $$ = $1; }

expr
  : var_src                             { $$ = $1; }
  | expr DOUBLEPT expr                  { $$ = AST_NODE_PCODE_Create(PC_CreateApply(__ADD__), $1, $3); printf("LISTE : TODO\n"); }
  | expr PLUS expr                      { $$ = AST_NODE_PCODE_Create(PC_CreateApply(__ADD__), $1, $3); }
  | expr MOINS expr                     { $$ = AST_NODE_PCODE_Create(PC_CreateApply(__SUB__), $1, $3); }
  | expr FOIS expr                      { $$ = AST_NODE_PCODE_Create(PC_CreateApply(__MUL__), $1, $3); }
  | expr DIVISE expr                    { $$ = AST_NODE_PCODE_Create(PC_CreateApply(__FDV__), $1, $3); }
  | OPAR expr CPAR                      { $$ = $2; }

var_src
  : VAR                                 { $$ = AST_NODE_PCODE_Create(PC_CreatePushSrc((char *)$1), NULL, NULL);}
  | ENTIER                              { $$ = AST_NODE_PCODE_Create(PC_CreatePushCst(OBJ_Create(OBJ_INT, $1, NULL)), NULL, NULL); }
  | STRING                              { $$ = AST_NODE_PCODE_Create(PC_CreatePushCst(OBJ_Create(OBJ_STR, $1, NULL)), NULL, NULL); }
  | call

var_dst
  : VAR                                 { $$ = AST_NODE_PCODE_Create(PC_CreatePushDst((char *)$1), NULL, NULL); }


%%

int yyerror(AST_NODE **root, char *s){
    printf("yyerror : %s\n tree : %p\n", s, root);
    return 0;
}
