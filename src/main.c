#include "fpcode.h"
#include "grammar.y.h"
#include "prgm.h"

#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

int main(void) {
  // auto flush stdout
  setvbuf(stdout, NULL, _IONBF, 0);

  // parse file
  yyparse();
  PRGM_Print();

  // Execution

  for (FPCODE *fpc = PRGM_GetNext(); fpc != NULL; fpc = PRGM_GetNext())
    FPC_RunFpcode(fpc);

  // State of PO
  MEM_Print();
  FPC_PrintStack();

  // Free
  // PRGM_Free();

  return 0;
}
