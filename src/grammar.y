%{
  #include <stdio.h>
  int yyparse();
  int yylex();
  int yyerror(char *s);
%}

%define api.value.type {void *}

%token ENTIER
%left DOUBLEPT
%left PLUS MOINS
%left FOIS DIVISE

%token OUVRIR
%token FERMER

%token EQUAL
%token PTCOMMA
%token NAME
%token COMMA

%start ROOT

%%

ROOT: statements

/* Instrucions */
statements
  : statement PTCOMMA statements
  | %empty  {printf("[***  EOP  ***]\n");}


statement
  : affectation         {printf("\n");}
  | expr                {printf("--><_= %d>\n", *(int *)$1);}

affectation
  : variable EQUAL expr {printf("--><%s = %d>", (char *)$1, *(int *)$3);}

call
  : variable OUVRIR expr_list FERMER {printf("--><call %s(%d)>",  (char *)$1, *(int *)$3);}

/* Manipulation des objets (TODO)*/
expr_list
  : expr
  | expr_list COMMA expr

expr
  : variable
  | expr DOUBLEPT expr {printf("<Liste from %d to %d>", *(int *)$1, *(int *)$3);}
  | expr PLUS expr { $$ = malloc(4); *((int *)$$) = *(int *)$1 + *(int *)$3; }
  | expr MOINS expr { $$ = malloc(4); *((int *)$$) = *(int *)$1 - *(int *)$3; }
  | expr FOIS expr {$$ = malloc(4); *((int *)$$) = *(int *)$1 * *(int *)$3;}
  | expr DIVISE expr {$$ = malloc(4); *((int *)$$) = *(int *)$1 / *(int *)$3;}
  | OUVRIR expr FERMER { $$ = $2; }

variable
  : NAME {printf("<NAME %s>", (char *)$1);}
  | ENTIER {printf("<INT %d>", *(int *)$1);}
  | call

%%

int yyerror(char *s) {
    printf("yyerror : %s\n",s);
    return 0;
}

int main(void) {
    yyparse();
    printf("[***  EOM  ***]\n");
    return 0;
}
