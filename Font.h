#ifndef _FONT_H
#define _FONT_H

#define		ASCIIMAX		256
#define   CHARWIDTH  	8
#define   CHARHEIGHT  16
#define 	HZWIDTH			16
#define 	HZHEIGHT		16
#define 	HZ24WIDTH		24
#define 	HZ24HEIGHT	24

//#define 	HZK24S_ACTIVE
#define 	HZK24K_ACTIVE
//#define 	HZK24F_ACTIVE
//#define 	HZK24H_ACTIVE


#ifndef HZK24TYPE_DEFINE
#define HZK24TYPE_DEFINE
struct	hzk24_typ{         /*  ������ģ�ṹ��  */
  unsigned int code;
  unsigned char array[3*24];
};
#endif

#ifndef HZK16TYPE_DEFINE
#define HZK16TYPE_DEFINE
struct	hzk16_typ{         /*  ������ģ�ṹ��  */
  unsigned int code;
  unsigned int array[16];
};
#endif
struct	ascii_typ{				/* ASCII����ģ */
	unsigned char code;
	unsigned array[16];
};
	
//extern int hz24_count;
//extern struct hzk24_typ	*hzk24_table;
//extern unsigned long selfont;									/* �����ѡ��ȫ�ֱ��� */
/* ����Ϊ�˱����ĺ��� */
/* [][][][]  [][][][]    [][][][] [][][][] */
/* Ӣ������  Ӣ�Ĵ�С    �������� ���Ĵ�С */
#define ASC0 			0x01000000					/* ȱʡ��ASCII�ַ���Ҳ��һֱ���õ� */
#define _ASC0			0x00010000					/* 8x16 ��С��Ӣ���ַ� */	
#define HZKK			0x00000100					/* ���ֿ� ���� */
#define HZKS			0x00000200					/* ���ֿ� ���� */
#define HZKF			0x00000400					/* ���ֿ� ���� */
#define HZKH			0x00000800					/* ���ֿ� ���� */
/*#define _HZ12			0x00000001					/* ���ִ�С 12x16 */		
#define _HZ16			0x00000002					/* ���ִ�С 16x16 */	
#define _HZ24			0x00000004					/* ���ִ�С 24x24 */	
/*#define _HZ40			0x00000008					/* ���ִ�С 40x40 */	
/*#define _HZ48			0x00000010					/* ���ִ�С 48x48 */	

extern void	putStr(unsigned int x,unsigned int y,unsigned char *s,unsigned long color,unsigned char mag);
extern void selFONT(unsigned long font);
extern unsigned long getFONT(void);

extern void	(* putHZ)(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag);
extern void	putASCII(unsigned int x,unsigned int y,unsigned char code,unsigned long color,unsigned char mag);
extern void	putHZ16(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag);
extern void	putHZ24(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag);
extern void	selHZK24(char mode);
#endif