%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "obj.h"
  #include "mem.h"
  #include "fpcode.h"
  //#include "prgm.h"
  #include "ast.h"
  #include "ast_displayer.h"
  #include "ast_traversal.h"

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

ROOT: statements                        {
                                          printf("DONE0\n");
                                          AST_DISPLAY_Text($1, 0);
                                          printf("DONE1\n");
                                          AST_DISPLAY_DotF($1, "out.dot");
                                          printf("DONE2\n");
                                          PrgmCodePrint(AST_ToCodeRec($1));
                                        }

/* Instrucions */
statements
  : statement statements                { $$ = AST_NODE_STAT_Create($2/*next*/,$1); }
  | %empty                              { $$ = NULL;/* *** EOP *** */   }


statement
  : statement_affectation               { $$ = $1; }
  | statement_condition                 {  }
  | statement_expression                { $$ = AST_NODE_PCODE_Create(PC_Create(POP, (PC_ARG)0), $1, NULL);}
  | BEG statements END                  { $$ = $2; }

statement_expression
  : expr PTCOMMA                        { $$ = $1; }
  | PTCOMMA

statement_affectation
  : var_dst EQUAL expr PTCOMMA          { $$ = AST_NODE_PCODE_Create(PC_Create(AFFECT, (PC_ARG)0), $1, $3); }

statement_condition
  : IF OPAR expr CPAR statement                 { $$ = AST_NODE_IF_Create($3, $5, NULL); }
  | IF OPAR expr CPAR statement ELSE statement  { $$ = AST_NODE_IF_Create($3, $5, $7); }

call
  : var_src OPAR expr_list CPAR         { $$ = AST_NODE_PCODE_Create(PC_Create(CALL, (PC_ARG)0), $1, $3); }

expr_list
  : expr                                {/*  TODO */}
  | expr_list COMMA expr                { $$ = $1; }

expr
  : var_src                             { $$ = $1; }
  | expr DOUBLEPT expr                  { $$ = AST_NODE_PCODE_Create(PC_Create(APPLY_OBJ_FUNC, (PC_ARG)__ADD__), $1, $3); printf("LISTE : TODO\n"); }
  | expr PLUS expr                      { $$ = AST_NODE_PCODE_Create(PC_Create(APPLY_OBJ_FUNC, (PC_ARG)__ADD__), $1, $3); }
  | expr MOINS expr                     { $$ = AST_NODE_PCODE_Create(PC_Create(APPLY_OBJ_FUNC, (PC_ARG)__SUB__), $1, $3); }
  | expr FOIS expr                      { $$ = AST_NODE_PCODE_Create(PC_Create(APPLY_OBJ_FUNC, (PC_ARG)__MUL__), $1, $3); }
  | expr DIVISE expr                    { $$ = AST_NODE_PCODE_Create(PC_Create(APPLY_OBJ_FUNC, (PC_ARG)__FDV__), $1, $3); }
  | OPAR expr CPAR                      { $$ = $2; }

var_src
  : VAR                                 { $$ = AST_NODE_PCODE_Create(PC_Create(PUSH_SRC_VAR, (PC_ARG)(char *) $1), NULL, NULL);}
  | ENTIER                              { $$ = AST_NODE_PCODE_Create(PC_Create(PUSH_CST, (PC_ARG)OBJ_Create(OBJ_INT, $1, NULL)), NULL, NULL); }
  | STRING                              { $$ = AST_NODE_PCODE_Create(PC_Create(PUSH_CST, (PC_ARG)OBJ_Create(OBJ_STR, $1, NULL)), NULL, NULL); }
  | call

var_dst
  : VAR                                 { $$ = AST_NODE_PCODE_Create(PC_Create(PUSH_DST_VAR, (PC_ARG)(char *)$1), NULL, NULL); }


%%

int yyerror(char *s) {
    printf("yyerror : %s\n",s);
    return 0;
}
