/*
 *  @description: Zebro main.
 *  @author: Ian Marco Moffett.
 */

#include <stdio.h>
#include <version.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <util.h>
#include <state.h>
#include <frontend/ast.h>
#include <frontend/symbol.h>
#include <frontend/parser.h>
#include <sysdeps/mem.h>

static const char help_msg[] = {
  "ZebroCompiler v" VERSION "\n"
  "--help: Bring up this message\n"
};

static inline void help(void)
{
  printf(help_msg);
  exit(1);
}

static void compile(struct zebro_state *state, const char *filename)
{
  extern const char* __progname;
  state->fp = fopen(filename, "r");
  state->current_filename = filename;
  
  /* Create a temp file for the assembly to go */
  state->out_fp = fopen("zebro-out.asm", "w");

  if (state->fp == NULL)
  {
    printf("%s: \033[0;31merror: \033[39mFailed to open \"%s\"\n",
           __progname, filename
    );
    exit(1);
  }

  if (state->out_fp == NULL)
  {
    printf("%s: \033[0;31merror: \033[39mFailed to open "
           "/tmp/zebro-out.asm\n", __progname
    );
    
    perror("fopen");
    exit(1);
  }
  
  parse(state);
  fclose(state->fp);

  fclose(state->out_fp);

  zebro_cleanup(state);
  memset(state, 0, sizeof(struct zebro_state));  /* Reset all state */
}

static void handle_args(struct zebro_state *state, int argc, char **argv)
{
  for (int i = 1; i < argc; ++i)
  {
    if (strcmp(argv[i], "--help") == 0)
    {
      help();
    }
    else if (argv[i][0] != '-')
    {
      compile(state, argv[i]);
    }
    else
    {
      help();
    }
  }
}


void zebro_cleanup(struct zebro_state *state)
{
  symtbl_cleanup(state);
  ast_cleanup_nodes();
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Usage: %s <files>\n", argv[0]);
    printf("Please pass --help for more info\n");
    return 1;
  }

  struct zebro_state state = {0};
  state.line = 1;
  state.col = 1;
  
  handle_args(&state, argc, argv); 
  return 0;
}
