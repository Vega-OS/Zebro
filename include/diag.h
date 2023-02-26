/*
 *  @description: diag.h
 *  @author: Ian Marco Moffett.
 */

#ifndef DIAG_H
#define DIAG_H

#include <state.h>
#include <stdarg.h>

void diag_err(struct zebro_state *state, const char *fmt, ...);
void diag_warning(struct zebro_state *state, const char *fmt, ...);

#endif
