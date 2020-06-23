#include "fpcode.h"
#include "grammar.y.h"
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "mem_prgm.h"

#include "ast.h"
#include "ast_displayer.h"
#include "ast_traversal.h"

AST_NODE *BISON_Parse(void) {
  AST_NODE *root = NULL;

  yydebug = 0;
  int ret = yyparse(&root);

  assert(ret == 0);
  assert(root);
  return root;
}

int main(void) {
  // auto flush stdout
  setvbuf(stdout, NULL, _IONBF, 0);

  // parse file
  AST_NODE *root = BISON_Parse();
  printf("DONE0\n");

  AST_DISPLAY_Text(root, 0);
  printf("DONE1\n");

  AST_DISPLAY_DotF(root, "out.dot");
  printf("DONE2\n");

  size_t size;
  PCODE **prgm = AST_ComputePrgm(root, &size);
  MEMPRGM_Init(prgm, size);
  printf("DONE3\n");

  MEMPRGM_Print();
  printf("DONE4\n");

  // Outdated
  // PRGM_Print();

  // Execution

  // for (FPCODE *fpc = PRGM_GetNext(); fpc != NULL; fpc = PRGM_GetNext())
  //  FPC_RunFpcode(fpc);

  // State of PO
  MEM_Print();
  PC_PrintStack();

  // Free
  // PRGM_Free();

  return 0;
}
