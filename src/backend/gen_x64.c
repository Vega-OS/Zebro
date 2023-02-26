/*
 *  @description: x86_64 code generator.
 *  @author: Ian Marco Moffett.
 */

#include <backend/gen_x64.h>
#include <frontend/symbol.h>
#include <stdio.h>

static void prologue(struct zebro_state *state)
{
  fputs(
      ";; Generated by the BetterC compiler.\n"
      ";; Copyright (c) 2023, Ian Marco Moffett and the Vega team\n"
      ";; Licensed under the BSD 3-Clause\n"
      ";; See LICENSE at https://github.com/Vega-OS/Zebro/\n\n"
      "bits 64\n\n",
      state->out_fp
  );
}

static void func_prologue(struct zebro_state *state, size_t sym_id)
{


  struct symbol* sym = &state->symtbl[sym_id];
  if (sym->linkage == L_EXTERNAL)
  {
    fprintf(state->out_fp, "global $%s\n", sym->name);
  }

  fprintf(state->out_fp, "$%s:\n"
                         "\tpush rbp\n"
                         "\tmov rbp, rsp\n", sym->name
  );
}


reg_t x64_gen(struct zebro_state *state, struct ast_node *node,
              reg_t reg, int parent_ast_top)
{
  reg_t leftreg, rightleft;

  switch (node->op)
  {
    case AST_FUNC:
      func_prologue(state, node->left->id);
      break;
  }
}

void gen_x64_begin(struct zebro_state *state)
{
  prologue(state);
}
