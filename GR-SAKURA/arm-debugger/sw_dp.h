#ifndef __SW_DP_H__
#define __SW_DP_H__

#define REG_TYPE_DP		(0x00)
#define REG_TYPE_AP		(0x01)

#define REG_R_IDCODE		(0x00)
#define REG_W_ABORT		(0x00)
#define REG_RW_CTRL_STAT	(0x04)
#define REG_RW_SELECT		(0x08)

#define REG_AP_BANK_F_CSW	(0x00)
#define REG_AP_BANK_F_IDR	(0x0C)

extern void init_sw_port(void);
extern void init_swd(void);
extern void select_swd(void);
extern int32_t read_reg(uint8_t type, uint8_t addr, uint32_t *pVal);
extern int32_t write_reg(uint8_t type, uint8_t addr, uint32_t val);

#endif	// __SW_DP_H__