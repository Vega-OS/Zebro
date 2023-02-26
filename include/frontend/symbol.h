/*
 *  @description: symbol.h
 *  @author: Ian Marco Moffett.
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <state.h>

/*
 *  Structural types.
 */

typedef enum
{
  S_FUNCTION
} sym_stype_t;

/*
 *  Primitive types.
 */

typedef enum
{
  P_U8
} sym_ptype_t;

typedef enum
{
  L_INTERNAL,
  L_EXTERNAL
} sym_linkage_t;

struct symbol
{
  char *name;
  sym_stype_t stype;
  sym_ptype_t ptype;
  sym_linkage_t linkage;
};

void symtbl_push(struct zebro_state *state, const char *name,
                 sym_stype_t stype, sym_ptype_t ptype, sym_linkage_t linkage);

void symtbl_cleanup(struct zebro_state *state);

#endif
