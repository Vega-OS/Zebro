/*
 *  @description: symbol.h
 *  @author: Ian Marco Moffett.
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <state.h>
#include <stddef.h>

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

size_t symtbl_push(struct zebro_state *state, const char *name,
                   sym_stype_t stype, sym_ptype_t ptype, sym_linkage_t linkage);

uint8_t is_defined(struct zebro_state *state, const char *name);


void symtbl_cleanup(struct zebro_state *state);

#endif
