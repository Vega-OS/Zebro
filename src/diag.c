/*
 *  @description: Diagnosis stuff.
 *  @author: Ian Marco Moffett.
 */

#include <diag.h>
#include <stdio.h>

static void diag_common(struct zebro_state *state, const char *fmt, va_list args) {
  vprintf(fmt, args);
  printf("%s:%lld:%lld\n", state->current_filename, state->line, state->col);
}


void diag_err(struct zebro_state *state, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  printf("\033[0;31merror:\033[39m ");
  diag_common(state, fmt, ap);
  va_end(ap);
}

void diag_warning(struct zebro_state *state, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  printf("\033[0;35mwarning:\033[39m ");
  diag_common(state, fmt, ap);
  va_end(ap);
}
