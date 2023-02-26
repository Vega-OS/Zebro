/*
 *  @description: gen_x64.h
 *  @author: Ian Marco Moffett.
 */

#ifndef GEN_X64_H
#define GEN_X64_H

#include <stdint.h>
#include <stddef.h>
#include <state.h>
#include <frontend/ast.h>

typedef uint16_t reg_t;

void gen_x64_begin(struct zebro_state *state);

reg_t x64_gen(struct zebro_state *state, struct ast_node *node,
              reg_t reg, int parent_ast_top);

#endif
