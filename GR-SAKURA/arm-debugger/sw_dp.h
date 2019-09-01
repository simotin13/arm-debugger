#ifndef __SW_DP_H__
#define __SW_DP_H__

#define REG_R_IDCODE		(0x00)
#define REG_W_ABORT		(0x00)
#define REG_RW_CTRL_STAT	(0x04)

extern void init_sw_port(void);
extern void init_swd(void);
extern void select_swd(void);
extern int32_t read_dp_reg(uint8_t addr, uint32_t *pVal);
extern int32_t write_dp_reg(uint8_t addr, uint32_t val);

#endif	// __SW_DP_H__