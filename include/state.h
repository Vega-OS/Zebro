/*
 *  @description: Compiler state machine.
 *  @author: Ian Marco Moffett.
 */

#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdio.h>


/*
 *  @current_filename: Current file of file being compiled.
 *  @fp: File pointer.
 *  @out_fp: File pointer for output file.
 *  @line: Current line.
 *  @col: Current col.
 *  @symtbl: Symbol table.
 *  @symtbl_size: Symbol table size *in entries*.
 *  @putback: Lexer specific field.
 *  @pub: 1 if next symbol is to be public.
 */

struct zebro_state
{
  const char *current_filename;
  FILE *fp;
  FILE *out_fp;
  size_t line;
  size_t col;

  struct symbol *symtbl;
  size_t symtbl_size;

  /* Lexer specific fields */
  char putback;
  char *last_str;

  /* Parser specific fields */
  uint8_t pub : 1;
};

#endif
