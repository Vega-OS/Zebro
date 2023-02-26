/*
 *  @description: Zebro lexer.
 *  @author: Ian Marco Moffett.
 */

#include <lexer.h>
#include <stdio.h>
#include <diag.h>
#include <stdlib.h>
#include <string.h>
#include <sysdeps/mem.h>

#define IS_LETTER(ch) \
  ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))

#define SHOULD_SKIP(ch) \
  (ch == '\r' || ch == '\n' || ch == '\f' || ch == ' ')

static char *scanstr(char ch, struct zebro_state *state)
{
  char *buf = zebro_malloc(sizeof(char));

  if (buf == NULL)
  {
    printf("ZEBRO INTERNAL ERROR: Could not allocate memory in %s:%s\n",
           __FILE__, __func__
    );

    exit(1);
  }

  size_t buf_idx = 0;
  uint8_t is_looping = 1;

  while (1)
  {
    buf[buf_idx++] = ch;
    buf = zebro_realloc(buf, sizeof(char) * (buf_idx + 1));
    ch = fgetc(state->fp);
    
    if (!IS_LETTER(ch) && ch != '_')
    {
      break;
    }

    ++state->col;
  }

  buf[buf_idx++] = '\0';
  return buf;
}

static void checkstr(char *str, struct zebro_state *state,
                     struct token *token_out)
{

  token_out->line = state->line;
  token_out->col = state->col;

  if (strcmp(str, "pub") == 0)
  {
    token_out->type = TT_PUB;
  }
  else if (strcmp(str, "proc") == 0)
  {
    token_out->type = TT_PROC;
  }
  else
  {
    diag_err(state, "Unrecognized keyword \"%s\" found while scanning\n", str);
    zebro_free(str);
    exit(1);
  }
}

uint8_t scan(struct zebro_state *state, struct token *token_out)
{
  char ch = fgetc(state->fp);
  while (SHOULD_SKIP(ch) && ch != EOF)
  {
    ch = fgetc(state->fp);
  }

  if (ch == EOF)
  {
    return 0;
  }
  
  if (IS_LETTER(ch))    // Assume keyword for now if lexeme is a letter.
  {
    char *str = scanstr(ch, state);
    checkstr(str, state, token_out);
    zebro_free(str);
  }
  else
  {
    diag_err(state, "Unrecognized token '%c' found while scanning\n", ch);
    exit(1);
  }

  ++state->col;
  return 1;
}
