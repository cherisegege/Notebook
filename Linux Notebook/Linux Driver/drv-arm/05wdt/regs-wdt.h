/*******************************
 * 4412芯片WDT外设的头文件
 * 头文件中应该包含寄存器的偏移(不要包含基地址)；
 * 头文件中还应该包含寄存器内部的位定义；
 * Author: zht
 * Date: 2016-11-13
 *******************************/
#ifndef __REGS_WDT_H
#define __REGS_WDT_H

/*********************
 * 定义寄存器的偏移
 *********************/
#define WTCON		0x0
#define WTDAT		0x4
#define WTCNT		0x8
#define WTCLRINT	0xC


/***********************
 * 定义寄存器内部的位
 ***********************/
#define WTCON_RSTEN		(1<<0)
#define WTCON_INTEN		(1<<2)
#define WTCON_WDTEN		(1<<5)
//例子: 使能reset信号(读-改-写)
//value = readl(vir_base+WTCON);
//value |= WTCON_RSTEN;
//writel(value, (vir_base+WTCON));
//例子：不使能reset信号
//value = readl(vir_base+WTCON);
//value &= ~WTCON_RSTEN;
//writel(value, (vir_base+WTCON);

#define WTCON_DIV_MASK	(3<<3)
#define WTCON_DIV16		(0<<3)
#define WTCON_DIV32		(1<<3)
#define WTCON_DIV64		(2<<3)
#define WTCON_DIV128	(3<<3)
//例子：将分频比设置为64
//value = readl(vir_base+WTCON);
//value &= ~WTCON_DIV_MASK;
//value |= WTCON_DIV64;
//writel(value, (vir_base+WTCON));

#define WTCON_PRESC_MASK	(0xFF<<8)
#define WTCON_PRESC(x)		((x)<<8)
//例子：将预分频设置为100
//value = readl(vir_base+WTCON);
//value &= ~WTCON_PRESC_MASK;
//value |= WTCON_PRESC(99);
//writel(value, (vir_base+WTCON));

#endif //__REGS_WDT_H

