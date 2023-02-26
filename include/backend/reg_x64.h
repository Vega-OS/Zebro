/*
 *  @description: reg_x64.h
 *  @author: Ian Marco Moffett.
 */

#ifndef REG_X64_H
#define REG_X64_H

#include <backend/gen_x64.h>


reg_t reg_load(struct zebro_state *state, ssize_t value);
reg_t reg_add(struct zebro_state *state, reg_t r1, reg_t r2);
reg_t reg_sub(struct zebro_state *state, reg_t r1, reg_t r2);
reg_t reg_mul(struct zebro_state *state, reg_t r1, reg_t r2);
reg_t reg_div(struct zebro_state *state, reg_t r1, reg_t r2);

void freeall_regs(void);
void free_reg(reg_t r);

reg_t alloc_reg(void);

extern const char *g_regs[4];
extern const char *g_bregs[4];

#endif
