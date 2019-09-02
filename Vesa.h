/*luobing 2008-10-13 ��ʼ��д*/
/*����һ������OS����DOS��ͨ�õ�ͼ�ο�,��ʼ��ǰ�ļƻ�.
  ��δ�����ƽ̨��ֲ,���д������i386������*/
#ifndef _VESA_H
#define _VESA_H

#define DIRECT_BANKING					/*�����Ƿ���ó����ṩ�Ļ�ҳ����*/
/*#define MULTI_PAGE							/*��ҳ����ʾ�ı��뿪��*/

#define INVLIADIN				0x01		/* ��ʼ��ͼ������ʱ��Ч��ڲ��� */
#define NOTSUPPORTVBE		0x02		/* ��֧��VBE */
#define NOTSUPPORTMODE  0x04		/* ��Ч��ʾģʽ */
#define LIMITVMEM				0x08		/* �Դ治�� */

/* WriteMode ���õĳ���*/
#define	PUT			0
#define XOR_PUT	1
#define AND_PUT	2
#define OR_PUT	3 


/*
#ifndef __FP__MACRO
#define __FP__MACRO
#define FP_SEGM(fp)         ((unsigned int)((unsigned long)((void far *)(fp)) >> 16))
#define FP_OFFS(fp)         ((unsigned int)(fp))
#endif
*/
/*VBE��Ϣ���ݽṹ����(VBE���ܵ���00H ����)*/
typedef struct{																/*�ܹ�512�ֽ�*/
	char VBESignature[4];								/*�ַ���"VESA"*/
	unsigned int VBEVersion;						/*VBE�汾��,BCD��*/
	char far *OEMStringPtr;							/*ָ��OEM���̵ı�־����ָ��*/
	unsigned long Capabilities;					/*��ʾ������*/
	unsigned int far *VideoModePtr;			/*ָ������ʾģʽ�б��ָ��*/
	unsigned int VRAMMemory;						/*��ʾ�ڴ��С,��λΪ64KB*/
	
	/*����ΪVESA VBE2.0�����϶���*/
	unsigned int OemSoftwareRev;				/*VBE�����OEM�޶��汾��,BCD��*/
	char far *OemVendorNamePtr;					/*ָ����ʾ�����쳧�̵��ַ���ָ��*/
	char far *OemProductNamePtr;				/*ָ����ʾ����Ʒ���Ƶ��ַ���ָ��*/
	char far *OemProductRevPtr;				  /*ָ����ʾ���޶��汾�Ż��Ʒ�ȼ����ַ���ָ��*/
	char reserved[222];									/*����*/
	
	char OEMData[256];									/*VESA2.0���ϰ汾����*/
}VBEInfo;

/*VBE�ض�ģʽ��Ϣ�����ݽṹ����(VBE���ܵ���01H����)*/
typedef struct																			/*�ܹ�256�ֽ�*/
{
	unsigned int ModeAttr; 										/* ģʽ������ */
	unsigned char WinAAttr;				 						/* ����A������ */
	unsigned char WinBAttr;										/* ����B������ */
	unsigned short WinGran; 									/* ��������,��KBΪ��λ */
	unsigned short WinSize; 									/* ���ڴ�С,��KBΪ��λ */
	unsigned short WinASeg; 									/* ����A��ʼ��ַ */
	unsigned short WinBSeg; 									/* ����B��ʼ��ַ */
	char far *BankFunc;												/* ��ҳ������� */
	unsigned int BytesPerScanline; 						/* ÿ��ˮƽɨ������ռ���ֽ��� */
	unsigned int XRes,YRes; 								  /* ˮƽ�ʹ�ֱ�ֱ��� */
	unsigned char XCharSize; 									/* �ַ��Ŀ�� */
	unsigned char YCharSize; 									/* �ַ��ĸ߶� */
	unsigned char NumberOfPlanes; 						/* λƽ��ĸ��� */
	unsigned char BitsPerPixel; 							/* ÿ���ص�λ�� */
	unsigned char NumberOfBanks;	 						/* CGA�߼�ɨ���߷����� */
	unsigned char MemoryModel; 								/* ��ʾ�ڴ�ģʽ */
	unsigned char BankSize; 									/* CGAÿ��ɨ���ߵĴ�С */
	unsigned char NumberOfImagePages; 				/* ��ͬʱ������������ͼ���� */
	unsigned char reserve1; 									/* Reserved */
	
	/*��ֱ��д��ɫģʽ�Ķ�������*/
	unsigned char RedMaskSize; 								/* ��ɫ��ռλ�� */
	unsigned char RedFieldPosition; 					/* ��ɫ�������Чλλ�� */
	unsigned char GreenMaskSize; 							/* ��ɫ��ռλ�� */
	unsigned char GreenFieldPosition; 				/* ��ɫ�������Чλλ�� */
	unsigned char BlueMaskSize; 							/* ��ɫ��ռλ�� */
	unsigned char BlueFieldPosition; 					/* ��ɫ�������Чλλ�� */
	unsigned char RsvdMaskSize; 							/* ����ɫ��ռλ�� */
	unsigned char RsvdFieldPosition; 					/* ����ɫ�������Чλλ�� */
	unsigned char DirectColorModeInfo; 				/* ֱ����ɫģʽ���� */
	
	/*����λVBE2.0�����϶���*/
	char far *PhyBasePtr;											/* ��ʹ�ô��֡����ʱΪָ������ַ��32λ�����ַ */
	unsigned long OffScreenMemOffset;					/* ֡������ַ��32λƫ����*/
	unsigned int OffScreenMemSize;						/* ���õġ���������ʾ������,��KBΪ��λ*/
	
	/*����ΪVBE3.0�����϶���*/
	unsigned int LinBytePerScanLine;					/* ���Ի�������ÿ��ɨ���ߵĳ��ȣ����ֽ�Ϊ��λ*/
	char BnkNumberOfImagePages;								/* ʹ�ô��ڹ���ʱ����ʾҳ����*/
	char LinNumberOfImagePages;								/* ʹ�ô�����Ի�����ʱ��ʾҳ����*/
	char LinRedMaskSize;											/* ʹ�ô�����Ի�����ʱ��ɫ��ռλ��*/
	char LinRedFieldPosition;									/* ʹ�ô�����Ի�����ʱ��ɫ�������Чλλ��*/
	char LinGreenMaskSize;										/* ʹ�ô�����Ի�����ʱ��ɫ��ռλ��*/
	char LinGreenFieldPosition;								/* ʹ�ô�����Ի�����ʱ��ɫ�������Чλλ��*/
	char LinBlueMaskSize;											/* ʹ�ô�����Ի�����ʱ��ɫ��ռλ��*/
	char LinBlueFieldPosition;								/* ʹ�ô�����Ի�����ʱ��ɫ�������Чλλ��*/
	char LinRsvMaskSize;											/* ʹ�ô�����Ի�����ʱ����ɫ��ռλ��*/
	char LinRsvFieldPosition;									/* ʹ�ô�����Ի�����ʱ����ɫ�������Чλλ��*/
	unsigned char reserve2[194]; 							/* ���� */
} ModeInfoBlock;

/* SVGA DAC��ɫ�����ݽṹ */
typedef struct	
{
	unsigned char b;										/*��ɫ*/
	unsigned char g;										/*��ɫ*/
	unsigned char r;										/*��ɫ*/
	unsigned char alpha;								/*����*/
}SvgaDAC;

extern VBEInfo vbeinfo;								 /*VESA ������Ϣ�� 00h���ܵ���*/
extern ModeInfoBlock modeinfo;				 /*VESA ģʽ��ϸ��Ϣ 01h���ܵ���*/
extern unsigned int xres,yres;         /* Resolution of video mode used */
extern unsigned int bytesperline;      /* Logical CRT scanline length */
extern unsigned char bytesperpixel;		 /* ÿ������ռ�ֽ��� */
extern unsigned int curBank;           /* Current read/write bank */
extern unsigned int bankShift;         /* Bank granularity adjust factor */
extern unsigned int oldMode;           /* Old video mode number */
extern char far *screenPtr;            /* Pointer to start of video memory */
extern void (far *bankSwitch)(void);   /* Direct bank switching function */

extern signed int getVBEInfo(VBEInfo far *vbeinfo);
extern signed int getModeInfo(unsigned int mode,ModeInfoBlock far *modeinfo);
extern void setmode(unsigned int mode);
extern unsigned int getVBEMode(void);

extern unsigned int initGraphics(unsigned int mode);
extern signed int (*setpalette)(unsigned int start,unsigned int num,SvgaDAC far *palette);
extern signed int (*getpalette)(unsigned int start,unsigned int num,SvgaDAC far *palette);
extern void	SetWriteMode(unsigned int writemode);
extern void (*putpixel)(unsigned int x,unsigned int y,unsigned long color);
extern unsigned long	(*getpixel)(unsigned int x,unsigned int y);
#endif