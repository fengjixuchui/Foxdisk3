/* bootOS.c */
#include "Vesa.h"
#include "Graphic.h"
#include "Font.h"

#include "Common.h"
#include "Painter.h"
#include "INT1ch.h"

#include "DISKDRV.h"
#include "DISKAPP.h"

#include "Global.h"
#include "iMem.h"
void bootOS(OS *os,int nos,unsigned long bg,unsigned long fore,SvgaDAC *pal);
void bootAbExit(void);
int canBootOS(OS *os,int nos);

void bootOS(OS *os,int nos,unsigned long bg,unsigned long fore,SvgaDAC *pal)
{
	unsigned int i,iLoadLen,winx=380,winy=320;
	char password[13];
	unsigned char buffer[512];
	unsigned char mbr0[512];
	unsigned char far *ptr=(unsigned char far *)0x00007c00;
	if(os[nos].pwd_valid)			/* ������ϵͳ���� */
	{
		enable1CH();
		rectblock(winx,winy,winx+(30<<3),winy+24*4,bg);
		rectangle(winx,winy,winx+(30<<3),winy+32,fore);
		rectangle(winx,winy+32,winx+(30<<3),winy+24*4,fore);
		Pic_Wing(winx+8,winy+7,bg,fore,0);
		setCur(winx+8+8+16,winy+8);
		c_printf(fore,bootos_str,os[nos].os_name);
		setCur(winx+8,winy+40);
		c_printf(fore,bootos_str1);
		while(1)
		{
			setCur(winx+8+8*8,winy+40+24);
			for(i=0;i<13;i++)password[i]=0;
			c_scanf(fore,bg,bootos_str2,password);
			if(!(c_strncmp(os[nos].pwd,password,c_strlen(os[nos].pwd))||(c_strlen(os[nos].pwd)!=c_strlen(password))))
				break;				/* ����һ�� */	
		}
		disable1CH();
	}
	/* ����ϵͳ��Ϣ�������� */
	/* 1 ��ȡ��ǰϵͳ��Ϣ */
	iLoadLen = FP_OFFS(end_load)-FP_OFFS(beg_load);
	for(i=0;i<512;i++)buffer[i]=0;
	for(i=0;i<512;i++)mbr0[i]=0;
	if(linear_ReadWDK(os[nos].os_mbr, 1, buffer))
		bootAbExit();						/* ������������ */

	if(linear_ReadWDK(0, 1, mbr0))
		bootAbExit();						/* ������������ */
	/* 2 �Ƚ�Ҫ������ϵͳ�͵�ǰmbr��ϵͳ�������Ƿ�һ��*/
	if(imemcmp(buffer+iLoadLen,mbr0+iLoadLen,512-iLoadLen))			/* ��ȥ����������ȫ���Ƚ� */
	{/* ����ͬ�� Ҫ������ϵͳ ����Ϣд��MBR0*/
		imemcpy(mbr0+iLoadLen,buffer+iLoadLen,512-iLoadLen -2);
		if(linear_WriteWDK(0, 1, mbr0))
			bootAbExit();	
	}
	
	if(linear_ReadWDK(os[nos].os_mbr, 1, buffer))
		bootAbExit();						/* ������������ */
	/*3 lbdebug*/
	/*
	{
		int m,n;
		rectblock(0,0,1024,16*16,GROUND1);
		for(m=0;m<16;m++)
			for(n=0;n<32;n++)
			{
				setCur(n*32,m*16);
				c_printf(WHITE,debug1,buffer[m*32+n]);
			}
		setCur(0,16*16);
		c_printf(YELLOW,debug2,os[nos].os_mbr);
		c_printf(YELLOW,debug2,os[3].os_mbr+1);
		c_printf(YELLOW,debug2,iLoadLen);
		asm xor ax,ax
		asm int 0x16
	}*/
	if((buffer[0]=='S') && (buffer[1]=='u') && (buffer[2]=='n') && (buffer[3]=='0'))
		if(linear_ReadWDK(os[3].os_mbr+1, 1, buffer))		/* ���û����������,���ñ���΢���������� */
			bootAbExit();						/* ������������ */
	imemcpy(buffer+iLoadLen,mbr0+iLoadLen,512-iLoadLen-2);
	/*4 lbdebug*/
	/*
	{
		int m,n;
		rectblock(0,0,1024,16*16,GROUND1);
		for(m=0;m<16;m++)
			for(n=0;n<32;n++)
			{
				setCur(n*32,m*16);
				c_printf(WHITE,debug1,buffer[m*32+n]);
			}
		setCur(0,16*16);
		c_printf(YELLOW,debug2,os[nos].os_mbr);
		c_printf(YELLOW,debug2,os[3].os_mbr+1);
		c_printf(YELLOW,debug2,iLoadLen);
		asm xor ax,ax
		asm int 0x16
	}*/
	SvgaSmoothOff(pal);				/* ��ʼ������ʾ */
	/* 3 ֱ������,����ǰϵͳ������Ϣ������0x7c00�� */ 
	for(i=0;i<512;i++)
		ptr[i]=buffer[i];
	asm{ 
  	sti 
  	mov ax,0003h
  	int 10h
  	xor	ax,ax
  	mov	bx,ax
  	mov	cx,ax
  	mov	dx,ax
  	mov	bp,ax
  	mov	si,ax
  	mov	di,ax
  	mov	ds,ax
  	mov	es,ax
  	mov	SP,7c00h
  	db 0eah     				/*jmp 0000:7c00h*/
  	dw 7c00h
  	dw 0000h
	}
	
}
/* �ж�ϵͳ�Ƿ��ܹ����� */
/*��ڲ���:osϵͳ��Ϣ�� nos(0~4):ϵͳ��� */
/*���ڲ���: 0:�������� ��0:��������,������DISKAPP.h�����Ĵ��� */
int canBootOS(OS *os,int nos)
{
	/* ϵͳ�ܹ�����,ͨ�������ж� */
	/* 1 �Ƿ�����Ч������ */
	/* 2 �Ƿ��л����(�����)*/
	/* 3 �Ƿ�������MBR,���û�еĻ�,����������Foxdisk�Դ���΢���������� */
	/* ��ȡ��ǰӲ�̵ķ��� */
	PARTN partnArr[MAX_PARTN_ELEM];				/* �洢��ǰϵͳ�ķ������� */
	int err=0;
	CreateDiskParaTable();
	err=Get_Disk_Info(CUR_DK,partnArr,os[nos].os_mbr,TRUE);
	if(os[nos].os_valid==0)err|=0x80;
	if(g_Info.fd.used<2)err|=0x80;			/* ռ�ô˱�־80Ϊ���Լ��ı�־:����û�л��� */
	
	return err;
}
void bootAbExit(void)
{
}