#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "Font.h"
#include "Vesa.h"
#include "DISKDRV.h"
#include "DISKAPP.h"
/*===data seg flag==== */
extern char  gFoxdisk[];
extern unsigned char	gEndOfData[];
extern unsigned int iFoxVer;

extern unsigned char debug1[];
extern unsigned char debug2[];
/* ====================BootMenu.c==================== */
extern unsigned char *bootStr[];
extern unsigned long bmsg_color;
extern unsigned long bbg_color;
/* ====================MainMenu.c==================== */
#define HELP_LINES	24
extern unsigned char helpTitle[];
extern unsigned char *helpStr[];
extern unsigned char aboutTitle[];
#define SYSFUN_NUM 5							/* ϵͳ�˵��еĹ��ܲ˵���Ŀ */
extern unsigned char *sysFun_str[];
extern unsigned char *sysFun_Name[];
extern unsigned char *sysFun_Sort[];
extern unsigned char *sysFun_Pwd[];
extern unsigned char hideFun_str0[];
extern unsigned char hideFun_str1[];
extern unsigned char hideFun_str2[];
extern unsigned char hideFun_str3[];
extern unsigned char hideFun_str4[];
extern unsigned char hideFun_str5[];
extern unsigned char *about_str[];
extern unsigned char *exitSys_str[];
/* ====================SetPara.c==================== */
extern unsigned char *paramenu_str[];
extern unsigned char *cntdown_str[];
extern unsigned char *setdefos_str[];
extern unsigned char *setwallp_str[];
extern unsigned char *exitPara_str[];
/* ====================Fdisk.c==================== */
extern unsigned char *fdisk_str[];
#define PARTFUN_NUM 9
extern unsigned char *part_fun[];
extern unsigned char *creatpart_str[];
extern unsigned char *delPartn_str[];
extern unsigned char *chkpart_str[];
extern unsigned char *savepart_str[];
extern unsigned char setid_array[];
extern unsigned char *setid_str[];
extern char *g_SysName[];

/* ====================INT1ch==================== */
extern unsigned int seg_oldint;							/* ������ǰ���ж����� */
extern unsigned int ofs_oldint;
/* bit0=1����ʾʱ�ӵĿ��� bit1=1�򿪹����ʾ bit2=1�򿪵���ʱ����ʾ  */
extern unsigned int _sw_INT_;									/* �жϺ���ʹ�õĿ��� */
extern unsigned int i_int_count;									/* �ҵ��ж���ʹ�õļ����� */
extern unsigned char i_number[];			/* ��ǰ��ʾʹ�õ��ַ��� */
/* ʱ����ʾ�õ����ַ������� */
extern unsigned char i_when[];
extern unsigned int oldhour_min;
extern unsigned char old_sec;
extern unsigned int _hms_x,_hms_y;			/* ʱ����ʾ������ */
/* �����ʾ�õ��ı��� */
/*x_cur,y_cur��Ϊȫ�ֱ������� */
extern unsigned char curcolor;
extern unsigned char bg_curcolor;
extern unsigned char cur_flag;
/* ����ʱ�õ��ı��� */
#define CNTD_SWID 16*2
#define CNTD_SHEI 16*2
extern unsigned char cntd_str[];
extern unsigned char cov_pci[(CNTD_SWID+1)*(CNTD_SHEI+1)];
extern unsigned int cntdown_times;					/* ����ʱ����ʱ�� */
extern unsigned int time_rad;							/* ȫ�ֱ�������������*/
extern unsigned int cntdown_over;					/* ����ʱ��ɵı�־ 1��ʾ���*/
extern unsigned int _cntd_x,_cntd_y;
extern unsigned long cntd_color,cntd_bcolor;

/* ====================Painter.c==================== */  
extern unsigned int x_cur;    
extern unsigned int y_cur;    
extern unsigned char digits[];
extern unsigned char iFaceStr[];
extern unsigned char iFaceStr1[];
extern unsigned char iFaceStr2[];
extern unsigned char iFaceStr3[];
extern unsigned char iFaceStr4[];
extern unsigned char iFaceStr5[];
extern unsigned char ifoxdisk[];
extern unsigned char iflag[];
extern unsigned char wing[17][3];
/* ====================Font.c==================== */
extern unsigned long selfont;
extern struct	ascii_typ	ascii_table[ASCIIMAX];
extern int hz24_count;
extern struct hzk24_typ	*hzk24_table;	
extern struct	hzk24_typ	HZK24Ktable[];		/* define in HZK24K.h */
extern struct	hzk16_typ	hzk16_table[];		/* define in HZTABLE.h*/
extern int hzcount;
extern int hzk24kcount;

/* ====================Graphic.c==================== */
extern SvgaDAC sys_pal[256];

/* ====================Vesa.c==================== */ 
extern void (far *bankSwitch)(void);   			/* Direct bank switching function */
extern void (*setbank)(unsigned int bank);
extern signed int (*setpalette)(unsigned int start,unsigned int num,SvgaDAC far *palette);
extern signed int (*getpalette)(unsigned int start,unsigned int num,SvgaDAC far *palette);
extern void (*putpixel)(unsigned int x,unsigned int y,unsigned long color);
extern unsigned long	(*getpixel)(unsigned int x,unsigned int y);
extern VBEInfo vbeinfo;         							/* VESA ������Ϣ�� 00h���ܵ��� */
extern ModeInfoBlock modeinfo;   						/* VESA ģʽ��ϸ��Ϣ 01h���ܵ��� */
extern unsigned int xres,yres;         			/* Resolution of video mode used */
extern unsigned int bytesperline;      			/* Logical CRT scanline length */
extern unsigned char bytesperpixel;    			/* ÿ������ռ�ֽ��� */
extern unsigned int curBank;           			/* Current read/write bank */
extern unsigned int bankShift;         			/* Bank granularity adjust factor */
extern unsigned int oldMode;           			/* Old video mode number */
extern unsigned long	maxpage;								/* �����ʾҳ�� */
extern char far *screenPtr;            			/* Pointer to start of video memory */
extern unsigned int VRAMStartX,VRAMStartY; 	/* �߼���ʾ��ʼ */
extern unsigned int WriteMode;								/* ��ͼдģʽ */	

/* ====================DISKAPP.c==================== */
extern DiskDT    g_DTable[1];
extern INFO      g_Info;
extern DiskDRV  g_diskDrv;
/* ====================EndCode.c==================== */
extern void EndofCode(void);

/* ====================Loader.asm==================== */
extern	void beg_load(void);
extern	void end_load(void);

/* bootOS.c */
extern unsigned char bootos_str[];
extern unsigned char bootos_str1[];
extern unsigned char bootos_str2[];                            

#endif