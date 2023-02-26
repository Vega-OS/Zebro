/*
 *  @description: x86_64 register operations.
 *  @author: Ian Marco Moffett.
 */

#include <backend/reg_x64.h>

const char *g_rregs[4] = {"r8", "r9", "r10", "r11"};
const char *g_bregs[4] = {"r8b", "r9b", "r10b", "r11b"};
static uint8_t regbmp = 0xFF;

reg_t alloc_reg(void)
{
  for (int i = 0; i < 4; ++i)
  {
    if (regbmp & (1 << i))
    {
      regbmp &= ~(1 << i);
      return i;
    }
  }

  return -1;
}

void freeall_regs(void)
{
  regbmp = 0xFF;
}

void free_reg(reg_t r)
{
  regbmp |= (1 << r);
}

reg_t reg_load(struct zebro_state *state, int64_t value) {
  reg_t r = alloc_reg();
  fprintf(state->out_fp, "\tmov %s, %d\n", g_rregs[r], value);
  return r;
}

reg_t reg_add(struct zebro_state *state, reg_t r1, reg_t r2)
{
  fprintf(state->out_fp, "\tadd %s, %s\n", g_rregs[r1], g_rregs[r2]);
  free_reg(r2);
  return r1;
}

reg_t reg_sub(struct zebro_state *state, reg_t r1, reg_t r2)
{
  fprintf(state->out_fp, "\tsub %s, %s\n", g_rregs[r1], g_rregs[r2]);
  free_reg(r2);
  return r1;
}

reg_t reg_mul(struct zebro_state *state, reg_t r1, reg_t r2)
{
  fprintf(state->out_fp, "\timul %s, %s\n", g_rregs[r1], g_rregs[r2]);
  free_reg(r2);
  return r1;
}

reg_t reg_div(struct zebro_state *state, reg_t r1, reg_t r2)
{
  fprintf(state->out_fp, "\tmov rax, %s\n", g_rregs[r1]);
  fprintf(state->out_fp, "\tcqo\n");
  fprintf(state->out_fp, "\tidiv %s\n", g_rregs[r2]);
  fprintf(state->out_fp, "\tmov %s, rax\n", g_rregs[r1]);
  free_reg(r2);
  return r1;
}
