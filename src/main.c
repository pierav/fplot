#include "grammar.y.h"
#include "obj.h"
#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

int main(void) {
  setvbuf(stdout, NULL, _IONBF, 0);

  yyparse();
  MEM_Print();

  // MEM_Tu();

  printf("Hello world\n");

  // MEM_Tu();

  return 0;
}
