/*
 *  @description: Symbol management.
 *  @author: Ian Marco Moffett.
 */

#include <frontend/symbol.h>
#include <sysdeps/mem.h>
#include <string.h>


void symtbl_push(struct zebro_state *state, const char *name,
                 sym_stype_t stype, sym_ptype_t ptype, sym_linkage_t linkage)
{
  if (state->symtbl == NULL)
  {
    state->symtbl = zebro_malloc(sizeof(struct symbol));
  }
  
  state->symtbl[state->symtbl_size].name = strdup(name);
  state->symtbl[state->symtbl_size].stype = stype;
  state->symtbl[state->symtbl_size].ptype = ptype;
  state->symtbl[state->symtbl_size].stype = stype;
  state->symtbl[state->symtbl_size].linkage = linkage;
}


void symtbl_cleanup(struct zebro_state *state)
{
  if (state->symtbl == NULL)
  {
    return;
  }

  for (size_t i = 0; i < state->symtbl_size; ++i)
  {
    zebro_free(state->symtbl[i].name);
  }

  zebro_free(state->symtbl);
  state->symtbl = NULL;
  state->symtbl_size = 0;
}
