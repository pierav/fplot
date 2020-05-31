#include "grammar.y.h"
#include "obj.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  yyparse();

  OBJ *x = OBJ_INT_Create(10, "x");
  OBJ_Print(x);
  OBJ *y = OBJ_INT_Create(100, "y");
  OBJ_Print(y);

  OBJ *xpy = OBJ_Add(x, y);
  OBJ_Print(xpy);
  printf("Hello world\n");

  return 0;
}
