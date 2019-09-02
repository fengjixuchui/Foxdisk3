#ifndef __INT1CH_H_
#define __INT1CH_H_


#define CLOCK    0x01
#define CURSOR   0x02
#define CNTDOWN  0x04
#define ENABLE(sw)		(_sw_INT_|=sw);
#define DISABLE(sw)		(_sw_INT_&= (~sw));

/* bit0=1����ʾʱ�ӵĿ��� bit1=1�򿪹����ʾ bit2=1�򿪵���ʱ����ʾ  */
extern unsigned int _hms_x,_hms_y;
extern void InitIntRes(unsigned int sw);
extern unsigned int enable1CH(void);
extern unsigned int disable1CH(void);
extern void RelIntRes(unsigned int sw);
extern void InitCntDown(unsigned int times,unsigned long strcorlor,unsigned long bgcolor,unsigned int x,unsigned int y);void InitCntDown(unsigned int times,unsigned long strcorlor,unsigned long bgcolor,unsigned int x,unsigned int y);
extern unsigned int getCountOver(void);
#endif