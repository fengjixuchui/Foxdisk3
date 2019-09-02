#include <stdio.h>
#include "BMP.h"				

#include "Vesa.h"
#include "Graphic.h"
#include "Font.h"

#include "Common.h"
#include "Painter.h"
#include "MainMenu.h"
#include "INT1ch.h"
#include "BootMenu.h"
#include "DISKDRV.h"
#include "DISKAPP.h"

#include "Foxdisk.h"
#include "iSetup.h"
#include "Setup.h"
#include "Global.h"
#include "iMem.h"

unsigned int isFirSetup(FOXPARA *para);
void updateSetup(FOXPARA *para);
int upgradeFox(FOXPARA *para);
void updateMBR(FOXPARA *para,unsigned int x,unsigned int y);
void updateFox(FOXPARA *para,unsigned int x,unsigned int y);
void addWallPaper(FOXPARA *para,unsigned int winx,unsigned int winy,unsigned int flag);
int savFoxKeyData(FOXPARA *para,OS *my_os,unsigned int flag);
void loadNewOS(FOXPARA *para,unsigned int x,unsigned int y);

/* �ж��Ƿ��һ�ΰ�װ */
/*ͨ�����������ж��Ƿ��һ��װ: */
/*    1 MBR��sigλ�õı�־'LUOB',ʹ��Setup.h�еĽṹ */
/*    2 para������Ҳ��ͬ���ı�־ */
/*    3 Data��ʼ������ 'iXLZ' ��־*/
/* ����:  0:�ǵ�һ�ΰ�װ ������ǵ�һ�ΰ�װ�򷵻ص�ǰ�汾�� 0xffff��ʾ����������� */
unsigned int isFirSetup(FOXPARA *para)
{
	BootPk mbr;
	unsigned char buff[512];
	unsigned int err=0;
	unsigned int ver=0;
	
	if(linear_ReadWDK(0, 1, (unsigned char *)&mbr))
		err=1;
	
	if( (mbr.sig[0]=='L') && (mbr.sig[1]=='U') && (mbr.sig[2]=='O') && (mbr.sig[3]=='B') )
		err=1;
	
	imemset(buff,0,512);
	if(linear_ReadWDK(para->lba_ipara, 1, buff))
		err=1;
	
	if((buff[0]=='L') && (buff[1]=='U') && (buff[2]=='O') && (buff[3]=='B'))
		err=1;
	
	imemset(buff,0,512);
	if(linear_ReadWDK(para->lba_ifox, 1, buff))
		err=1;
	
	if((buff[0]=='i') && (buff[1]=='X') && (buff[2]=='L') && (buff[3]=='Z'))
		err=1;
	
	ver=(unsigned int)buff[5] + (((unsigned int)buff[6])<<8);
	if(err)
	{ 
		if( (ver<0x9999) &&(ver>=0x0102))			/*��2008�꿪ʼ�ĵ�һ���汾��Ϊ0102���������� */
			err = ver;
		else
			err=0xffff;
		}
	
	return err;
}

/* ���³��� */
void updateSetup(FOXPARA *para)
{
	unsigned char *updateSetup_str[]={
		"��װ����--���³���",				/* 0 */
		"�����Ѱ�װ�İ汾�ϵ�(�Ѱ�װ�汾:%s,��ǰ�汾:%s),",/* 1 */
		"��ϣ��������(Y/N)?: %c", /* 2 */
		"�����ɹ�,�����������°汾��Foxdisk��^^", /* 3 */
		"����ʧ��,Ӳ��ĳЩ�����޷�д��!", /* 4 */
		"��⵽Foxdisk�Ѿ���װ����,��ϣ��:", /* 5 */
		"1 ������ϵͳ", /* 6 */
		"2 ��ӱ�ֽ", /* 7 */
		"3 ���ı�ֽ", /* 8 */		
		"4 ��ԭFoxdisk����������(����û�г��ֽ���������,����!!!!)",  /* 9 */
		"5 ���°�װ����",/* 10 */
		"��ѡ��(0-5,ѡ0��ֱ�ӻس����˳���ǰ����): %d",  /* 11 */
		"��л����ʹ��,�ټ�!", /* 12 */
		"��ѡ����%d,��������Ӧ�Ĵ������,�����������...", /* 13 */
		"����:%s  �ڲ��汾��:%s" /* 14 */
	};
	unsigned int i,j,cur_ver=iFoxVer,winx=320,winy=220,sel=0xffff;
	
	char VerStr0[13],VerStr1[13],ack=0;
	SvgaDAC pal[256];
	
	imemset(pal,0,sizeof(SvgaDAC)*256);
	initDisplay(0x105,pal);				/* ��ʼ��ͼ����ʾ */

	imemcpy(pal,sys_pal,sizeof(SvgaDAC)*256);
	imemset(VerStr1,0,13);
	sprintf(VerStr1,"%.2x.%.2x",(cur_ver>>8)&0xff,cur_ver&0xff);
	iFacePic(BOTTOM_FACE,RIGHT_FACE);
	iMessage(WHITE);			
	SvgaSmoothOn(pal);
	
	setCur(winx+1,winy-100+1);
	c_printf(DEEPBLUE,updateSetup_str[14],ifoxdisk,VerStr1);
	setCur(winx,winy-100);
	c_printf(WHITE,updateSetup_str[14],ifoxdisk,VerStr1);
	iWindowOut(winx,winy,updateSetup_str[0],10);

	InitIntRes(CLOCK);
	enable1CH();
		
	if(para->ver<cur_ver)
	{
		imemset(VerStr0,0,13);
		sprintf(VerStr0,"%.2x.%.2x",((para->ver)>>8)&0xff,(para->ver)&0xff);
		
		setCur(winx+8+16,winy+40+24);
		c_printf(WHITE,updateSetup_str[1],VerStr0,VerStr1);
		ack=0;
		while((ack!='Y') && (ack!='y') && (ack!='N') && (ack!='n'))
		{
			setCur(winx+8,winy+40+24*2);
			c_scanf(DEEPBLUE,WHITE,updateSetup_str[2],&ack);
		}
		if((ack=='Y') || (ack=='y'))
		{
			disable1CH();							/* �ر�ʱ���ж� */
			RelIntRes(CLOCK);
			setCur(winx+8,winy+40+24*3);
			if(upgradeFox(para))
				c_printf(WHITE,updateSetup_str[4]);
			else
				c_printf(WHITE,updateSetup_str[3]);
			InitIntRes(CLOCK);				/* ���´�ʱ���ж� */
			enable1CH();
			asm xor ax,ax
			asm int 0x16	
		}
	}
	winx=320;winy=220;
	rectblock(winx+8,winy+40,winx+8+(66<<3),winy+40+9*24+16,GROUND1);
	setCur(winx+8,winy+40);
	c_printf(SURFACE1,updateSetup_str[5]);
	
	for(i=0;i<5;i++)
	{
		setCur(winx+8+8*4,winy+40+24*(1+i));
		c_printf(SURFACE1,updateSetup_str[6+i]);
	}
	
	while(sel>5)
	{
		setCur(winx+8,winy+40+24*6);
		c_scanf(DEEPBLUE,WHITE,updateSetup_str[11],&sel);
	}
	setCur(winx+8,winy+40+24*8);
	if(sel==0)
	{
		c_printf(WHITE,updateSetup_str[12]);
		SvgaSmoothOff(pal);
		disable1CH();							/* �ر�ʱ���ж� */
		RelIntRes(CLOCK);				/* �ر�ʱ���ж�,ʹ��Ȩ��ת�������ӳ��� */
		return;
	}
	else
		c_printf(WHITE,updateSetup_str[13],sel);
	asm xor ax,ax
	asm int 0x16
	rectblock(winx+8,winy+40,winx+8+(66<<3),winy+40+9*24+16,GROUND1);
	disable1CH();							/* �ر�ʱ���ж� */
	RelIntRes(CLOCK);				/* �ر�ʱ���ж�,ʹ��Ȩ��ת�������ӳ��� */
	switch(sel)
	{
		case 4:				/* ����ΪFoxdisk MBR*/
			updateMBR(para,winx,winy);
			break;
		case 5:				/* ���´��� */
			updateFox(para,winx,winy);
			break;			
		case 2:				/* ��ӱ�ֽ */
			addWallPaper(para,winx,winy,0);
			break;
		case 3:				/* ���ı�ֽ */
			addWallPaper(para,winx,winy,1);
			break;
		case 1:				/* ������ϵͳ */
			loadNewOS(para,winx,winy);
			break;
		default:
			break;
	}
	SvgaSmoothOff(pal);
}

/* ����MBR */
void updateMBR(FOXPARA *para,unsigned int x,unsigned int y)
{
	unsigned char *updatembr_str[]={
		"����ԭFoxdisk����������,���Ժ�...",
		"  ����Foxdisk�İ汾...",
		"  ����Foxdisk�����ݿ�...",
		"  ����Foxdisk�Ĵ���...",
		"  ��ԭFoxdiskר����������...",
		"�����ɹ�,��л����ʹ��!",
		"����ʧ��,���̳��ֻ���!"
		"��Ǹ,�Ѿ���װ�İ汾�Ƚϸ�,������ʹ�õͰ汾������а�װ!",
	};
	unsigned int i;
	for(i=0;i<5;i++)
	{
		setCur(x+8,y+40+24*i);
		c_printf(WHITE,updatembr_str[i]);
		iDelay(200);
	}
	setCur(x+8,y+40+24*6);
	if(para->ver<=iFoxVer)
	{
		if(Setup(para->lba_ifox,SETUPMBR))
			c_printf(WHITE,updatembr_str[6]);
		else
			c_printf(WHITE,updatembr_str[5]);
	}
	else c_printf(WHITE,updatembr_str[7]);
	asm xor ax,ax
	asm int 0x16
}
/* ���´��� */
void updateFox(FOXPARA *para,unsigned int x,unsigned int y)
{
	unsigned char *updatefox_str[]={
		"�����°�װFoxdisk�Ĵ��벿��,���Ժ�...",
		"  ����Foxdisk�İ汾...",
		"  ����Foxdisk�����ݿ�...",
		"  ����Foxdisk�Ĵ���...",
		"  ��ԭFoxdiskר����������...",
		"�����ɹ�,��л����ʹ��!",
		"����ʧ��,���̳��ֻ���!",
		"��Ǹ,�Ѿ���װ�İ汾�Ƚϸ�,������ʹ�õͰ汾������а�װ!",
	};
	unsigned int i;
	for(i=0;i<5;i++)
	{
		setCur(x+8,y+40+24*i);
		c_printf(WHITE,updatefox_str[i]);
		iDelay(200);
	}
	setCur(x+8,y+40+24*6);
	if(para->ver<=iFoxVer)
	{
		if(Setup(para->lba_ifox,SETUPFOX))
			c_printf(WHITE,updatefox_str[6]);
		else
			c_printf(WHITE,updatefox_str[5]);
	}
	else
		c_printf(WHITE,updatefox_str[7]);
	asm xor ax,ax
	asm int 0x16
}
/* ��ӱ�ֽ ����±�ֽ */
/* flag=0:��ӱ�ֽ ,flag=1:���±�ֽ */
void addWallPaper(FOXPARA *para,unsigned int winx,unsigned int winy,unsigned int flag)
{
	unsigned char *addwp_str[]={
		"�밴��ָ���޸��Լ��ı�ֽ��:",															/* 0 */
		"���Ժ�,���ڴӵ�ǰĿ¼�л�ȡ�ɰ�װ�ı�ֽ...",							/* 1 */
		"    ��Ǹ,��ǰĿ¼û�з��ֿɰ�װ�ı�ֽ,��ȷ����Ҫ��װ�ı�ֽ", /* 2 */
		"�밲װ�������ͬһĿ¼��! ��������˳���ǰ����...", /* 3 */
		"��ǰĿ¼�·���Ҫ��ı�ֽ��(1024x768 256ɫ BMPͼ):", /* 4 */
		"%s",												/* 5 */
		"�Ѿ���װ�ı�ֽ��: %d",			/* 6 */
		"����װ��%d�ű�ֽ",					/* 7 */
		"��������Ҫ��װ�ı�ֽ����: %s",	/* 8 */
		"�Ѿ���װ�����ű�ֽ,�޷��ٰ�װ��!",			/* 9 */
		"��װʧ��!,�ļ��������������ļ�������Ҫ��,����Ҫ������(Y/N)?: %c", /* 10 */
		"���ѳɹ���װ��ֽ,������װ��һ����(Y/N)?: %c", /* 11 */
		"  �Ѱ�װ��ֽ����: %d.", /* 12 */
		"���浱ǰ����......",   /* 13 */
		"�ɹ�!��л����ʹ��^^",/* 14 */
		"�ܱ�Ǹ,����ʧ��!",   /* 15 */
		"��Ǹ,����û�а�װ����ֽ,�޷�ʹ���������!",  /* 16 */
		"��������Ҫ�滻�ı�ֽ���(1--%d):",  /* 17 */
		"%d",   /* 18 */
		"�ѳɹ��滻,��Ҫ����������һ����(Y/N)?: %c"  /* 19 */
		};
	unsigned char bmparray[12][13];			/* ��ŵ�ǰ���з���Ҫ���BMP�� */
	int bmp_count=0,i,j;
	char ack=0,bmpname[13];
	imemset(bmparray,0x0,12*13);
	
	
	if((para->wpaper_num==0) && (flag==1))
	{
		setCur(winx+8,winy+40+24*3);
		c_printf(WHITE,addwp_str[16]);
		asm xor ax,ax
		asm int 0x16
		return;
	}
	InitIntRes(CLOCK);
	enable1CH();
	
	setCur(winx+8,winy+40);
	c_printf(SURFACE1,addwp_str[0]);
	setCur(winx+8,winy+40+24);
	c_printf(WHITE,addwp_str[1]);
	bmp_count=dirWallPaper(bmparray);
	rectblock(winx+8,winy+40+24,winx+8+(66<<3),winy+40+24+16,GROUND1);
	if(bmp_count )
	{
		setCur(winx+8,winy+40+24);
		c_printf(SURFACE1,addwp_str[4]);
		for(j=0;j<3;j++)
			for(i=0;i<4;i++)
			{
				setCur(winx+8+4*8+i*13*8,winy+40+24*(2+j));
				c_printf(SURFACE1,addwp_str[5],bmparray[j*4+i]);
			}
		if(flag==0)
		{
			i=para->wpaper_num;
			ack='y';
			while((ack=='Y') || (ack=='y'))
			{
				rectblock(winx+8,winy+40+24*5,winx+8+(66<<3),winy+40+24*9+16,GROUND1);
				setCur(winx+8,winy+40+24*5);
				c_printf(WHITE,addwp_str[6],para->wpaper_num);
				setCur(winx+8,winy+40+24*6);
				if(i<6)
					c_printf(WHITE,addwp_str[7],i+1);
				if(i>=6)
				{
					rectblock(winx+8,winy+40+24*6,winx+8+(66<<3),winy+40+24*6+16,GROUND1);
					setCur(winx+8,winy+40+24*7);
					c_printf(WHITE,addwp_str[9]);
					asm xor ax,ax
					asm int 0x16
					break;	
				}
				imemset(bmpname,0,13);
				setCur(winx+8,winy+40+24*7);
				c_scanf(DEEPBLUE,WHITE,addwp_str[8],bmpname);
				if(savWallPaper(para->iwapaper,i,bmpname))
				{		/* ��װʧ�� */
					ack=0;
					while((ack!='Y') && (ack!='y') && (ack!='N') && (ack!='n'))
					{
						setCur(winx+8,winy+40+24*8);
						c_scanf(DEEPBLUE,WHITE,addwp_str[10],&ack);
					}
					if((ack=='n') || (ack=='N'))
						break;
				}
				else
				{			/* ��װ�ɹ� */
					ack=0;
					while((ack!='Y') && (ack!='y') && (ack!='N') && (ack!='n'))
					{
						setCur(winx+8,winy+40+24*8);
						c_scanf(DEEPBLUE,WHITE,addwp_str[11],&ack);
					}
					++i;
					++para->wpaper_num;
					if((ack=='n') || (ack=='N'))
						break;
				}
			}  
		}
		else
		{
			ack='y';
			while((ack=='Y') || (ack=='y'))
			{
				rectblock(winx+8,winy+40+24*5,winx+8+(66<<3),winy+40+24*9+16,GROUND1);
				setCur(winx+8,winy+40+24*5);
				c_printf(WHITE,addwp_str[6],para->wpaper_num);
				setCur(winx+8,winy+40+24*6);
				c_printf(WHITE,addwp_str[17],para->wpaper_num);
				i=0;
				while((i<1) || (i>para->wpaper_num))
				{
					setCur(winx+8+(c_strlen(addwp_str[17])<<3),winy+40+24*6);
					c_scanf(DEEPBLUE,WHITE,addwp_str[18],&i);
				}
				imemset(bmpname,0,13);
				setCur(winx+8,winy+40+24*7);
				c_scanf(DEEPBLUE,WHITE,addwp_str[8],bmpname);
				if(savWallPaper(para->iwapaper,i-1,bmpname))
				{		/* ��װʧ�� */
					ack=0;
					while((ack!='Y') && (ack!='y') && (ack!='N') && (ack!='n'))
					{
						setCur(winx+8,winy+40+24*8);
						c_scanf(DEEPBLUE,WHITE,addwp_str[10],&ack);
					}
					if((ack=='n') || (ack=='N'))
						break;
				}
				else
				{			/* ��װ�ɹ� */
					ack=0;
					while((ack!='Y') && (ack!='y') && (ack!='N') && (ack!='n'))
					{
						setCur(winx+8,winy+40+24*8);
						c_scanf(DEEPBLUE,WHITE,addwp_str[19],&ack);
					}
					if((ack=='n') || (ack=='N'))
						break;
				}
			}
		}
		rectblock(winx+8,winy+40+24*8,winx+8+(66<<3),winy+40+24*8+16,GROUND1);
		setCur(winx+8,winy+40+24*8);
		c_printf(WHITE,addwp_str[12],para->wpaper_num);
		setCur(winx+8,winy+40+24*9);
		c_printf(WHITE,addwp_str[13],para->wpaper_num);
		iDelay(500);
		if(savFoxKeyData(para,NULL,SAVEKEYPARA))
			c_printf(WHITE,addwp_str[15]);
		else
			c_printf(WHITE,addwp_str[14]);
		asm xor ax,ax
		asm int 0x16
	}
	else
	{
		setCur(winx+8,winy+40+24*2);
		c_printf(WHITE,addwp_str[2]);
		setCur(winx+8,winy+40+24*3);
		c_printf(WHITE,addwp_str[3]);
		asm xor ax,ax
		asm int 0x16
	} 
	 
	
	disable1CH();							/* �ر�ʱ���ж� */
	RelIntRes(CLOCK);				/* �ر�ʱ���ж�,ʹ��Ȩ��ת�������ӳ��� */   
}  

/* ������ϵͳ */
void loadNewOS(FOXPARA *para,unsigned int x,unsigned int y)
{
	unsigned char *loados_str[]={
		"����ϵͳ:����ϵͳ����Ϣ��ӵ�Foxdisk�����ݿ�",			/* 0 */
		"Foxdisk���ݿ�������ϵͳ��Ϣ:",/* 1 */
		"ϵͳ%d:%s",	/* 2 */
		"��ǰϵͳ�Ѿ��������ݿ�����,������Ҫ����!",					/* 3 */
		"��ǰ��ϵͳ�����ݿ����Ѿ�����,������:",				/* 4 */
		" 1 ʹ�ûָ�Foxdisk�Ĺ���1 �ָ���������(ǿ�ҽ���!)",				/* 5 */
		" 2 ��Ȼ�������ز��滻(������!)",										/* 6 */
		"��ѡ��(1/2):%d",	/* 7 */
		"лл����ʹ��,����������Foxdiskѡ����1�ָ���������.",/* 8 */
		"����ǰϵͳ¼�뵽Foxdisk�����ݿ���(�滻ϵͳ%d:%s):",  /* 9 */
		"�������µ�ϵͳ����:%s",  /* 10 */
		"¼��ɹ�,��л����ʹ��!",	/* 11 */
		"¼��ʧ��,���Foxdisk��Ӳ�������л���!", /* 12 */
		"���ݿ���ֻ�ܴ洢�ĸ�ϵͳ��Ϣ,���Ҫ������ϵͳ,ֻ���滻����һ��.",		/*13*/
		"��ǰϵͳ��ϵͳ%d:%s�ķ�������ͬ,�������滻���ϵͳ.",/*14*/
		"��Ҫ�滻�ĸ�ϵͳ?(1--4): %d", /*15*/
		"�˲�����һ���ķ���,��ȷ��Ҫ�滻��(Y/N)?: %c", /*16*/
		"�����ɹ�,��л����ʹ��^^",   /* 17 */
		"����ʧ��,���Foxdisk��Ӳ�������л���!",/*18*/
		"    ��л����ʹ��^^",/* 19*/
		
	};
	OS os[4];
	BootPk mbr;
	unsigned char buff[512],*ptr,os_str[13];
	unsigned int i,j,iLoadLen,flag=0,uGet=0,nos;
	int key_os=-1;
	char ack=0;
	
	InitIntRes(CLOCK);
	enable1CH();
	
	setCur(x+8,y+40);
	imemset(buff,0,512);
	if(linear_ReadWDK(para->lba_ios, 1, buff))
	{
		c_printf(WHITE,"Access HD Error!");
		disable1CH();							/* �ر�ʱ���ж� */
				RelIntRes(CLOCK);	
		return;
	}
	imemcpy(os,buff,sizeof(OS)*4);				/* ȡ��ǰϵͳ��Ϣ */
	c_printf(SURFACE1,loados_str[0]);
	setCur(x+8,y+40+24);
	c_printf(SURFACE1,loados_str[1]);
	for(i=0;i<4;i++)
	{
		if(i<2)
			setCur(x+8+(23<<3)*i+4*8,y+40+24*2);
		else
			setCur(x+8+(23<<3)*(i-2)+4*8,y+40+24*3);
		if(os[i].os_valid)
			c_printf(SURFACE1,loados_str[2],(unsigned int)(os[i].bar_index+1),os[i].os_name);
	}
	imemset(&mbr,0,512);
	setCur(x+8,y+40+24*4);
	if(linear_ReadWDK(0, 1, &mbr))					/* ��ȡ��ǰMBR����Ϣ��mbr */
	{
		c_printf(WHITE,"Access HD Error!");
		disable1CH();							/* �ر�ʱ���ж� */
		RelIntRes(CLOCK);	
		return;
	}
	ptr=(unsigned char *)(&mbr);
	iLoadLen = FP_OFFS(end_load)-FP_OFFS(beg_load);
	for(i=0;i<4;i++)												/* �Ƚϵ�ǰ���������ݿ��еķ����Ƿ�����ͬ�� */
	{
		imemset(buff,0,512);
		if(linear_ReadWDK(os[i].os_mbr, 1, buff))					/* ��ȡos����Ϣ��buff */
		{
			c_printf(WHITE,"Access HD Error!");
			disable1CH();							/* �ر�ʱ���ж� */
			RelIntRes(CLOCK);	
			return;
		}
		//if(!imemcmp(ptr+iLoadLen,buff+iLoadLen,512-iLoadLen))
		if(!imemcmp(ptr+0x1be,buff+0x1be,64))
		{
			flag=1;
			key_os=(int)i;
			break;
		}
	}	
	if(flag)
	{
		if(!imemcmp(mbr.sig,"LUOB",4))			/* ����ƥ��,�ҵ�ǰMBRΪ foxdisk mbr*/
		{
			c_printf(WHITE,loados_str[3]);
			asm xor ax,ax
			asm int 0x16
			disable1CH();							/* �ر�ʱ���ж� */
			RelIntRes(CLOCK);				
			return;
		}
		else
		{
			for(i=0;i<3;i++)
			{
				setCur(x+8,y+40+24*(4+i));
				c_printf(WHITE,loados_str[4+i]);
			}
			while((uGet!=1) && (uGet!=2))
			{
				setCur(x+8,y+40+24*7);
				c_scanf(DEEPBLUE,WHITE,loados_str[7],&uGet);
			}
			if(uGet==1)
			{
				setCur(x+8,y+40+24*8);
				c_printf(WHITE,loados_str[8]);
				asm xor ax,ax
				asm int 0x16
				disable1CH();							/* �ر�ʱ���ж� */
				RelIntRes(CLOCK);				
				return;
			}
		}	
	}
	/* Ѱ�ҿ��Լ���Ŀ�λ */
	rectblock(x+8,y+40+24*4,x+8+(66<<3),y+40+24*9+16,GROUND1);
	flag=0;
	for(i=0; i<4; i++)
	{
		imemset(buff,0,512);
		if(linear_ReadWDK(os[i].os_mbr, 1, buff))					/* ��ȡos����Ϣ��buff */
		{
			c_printf(WHITE,"Access HD Error!");
			disable1CH();							/* �ر�ʱ���ж� */
			RelIntRes(CLOCK);	
			return;
		}
		if(!imemcmp(buff,"Sun0",4))
		{
			unsigned char temp[64];
			imemset(temp,0,64);
			if(!imemcmp(buff+0x1be,temp,64))		/* �������Ƿ�Ϊ0 */
			{
				flag=1;
				break;
			}
		}
		
	}
	/* ���flag==1,��os[i]Ϊ��λ���� uGet==2ʱ key_os�д�����滻��ϵͳ */
	if((flag)||(uGet==2))
	{
		if(uGet==2)
			nos=key_os;
		else
			nos=i;			/* �����ҵ��Ŀ�λ */
		setCur(x+8,y+40+24*4);
		c_printf(WHITE,loados_str[9],os[nos].bar_index+1,os[nos].os_name);
		imemset(os_str,'\0',13);
		while(os_str[0]=='\0')
		{
			setCur(x+8,y+40+24*5);
			c_scanf(DEEPBLUE,WHITE,loados_str[10],os_str);
		}
		/*Ϊ����ʾһ��,�Խ��յ����ַ��������Ҷ���Ĵ��� 12�ֽ� */
		j=c_strlen(os_str);
		for(i=0;i<j;i++)
			os_str[11-i]=os_str[j-1-i];
		for(i=0;i<(12-j);i++)
			os_str[i]=' ';
		os_str[12]='\0';
		imemcpy(os[nos].os_name,os_str,13);		/* �����û��趨 */
		os[nos].os_valid=1;
		if(imemcmp(mbr.sig,"LUOB",4))				/* mbr������foxdiskmbr*/
			imemcpy(buff,ptr,512);
		else
			imemcpy(buff+iLoadLen,ptr+iLoadLen,512-iLoadLen);		/* ֻ���ط����� */
		/* �����趨 */
		setCur(x+8,y+40+24*6);
		if( linear_WriteWDK(os[nos].os_mbr, 1, buff) || savFoxKeyData(para,os,SAVEKEYOS))
			c_printf(WHITE,loados_str[12]);
		else
			c_printf(WHITE,loados_str[11]);	
	}
	else
	{
		setCur(x+8,y+40+24*4);
		c_printf(WHITE,loados_str[13]);
		setCur(x+8,y+40+24*5);
		if(key_os!=-1)
			c_printf(RED,loados_str[14],(unsigned int)(os[key_os].bar_index+1),os[key_os].os_name);
		i=0;
		while((i<1) || (i>4))
		{
			setCur(x+8,y+40+24*6);
			c_scanf(DEEPBLUE,WHITE,loados_str[15],&i);
		}
		for(j=0;j<4;j++)
			if((unsigned int)(os[j].bar_index+1)==i)
				nos=j;			/* �ҵ�Ҫ�滻��ϵͳ */
		while((ack!='Y') && (ack!='y') && (ack!='N') && (ack!='n'))
		{
			setCur(x+8,y+40+24*7);
			c_scanf(DEEPBLUE,WHITE,loados_str[16],&ack);
		}
		if((ack=='Y') || (ack=='y'))
		{
			/* ��ʼ�滻 */
			imemset(os_str,'\0',13);
			while(os_str[0]=='\0')
			{
				setCur(x+8,y+40+24*8);
				c_scanf(DEEPBLUE,WHITE,loados_str[10],os_str);
			}
			/*Ϊ����ʾһ��,�Խ��յ����ַ��������Ҷ���Ĵ��� 12�ֽ� */
			j=c_strlen(os_str);
			for(i=0;i<j;i++)
				os_str[11-i]=os_str[j-1-i];
			for(i=0;i<(12-j);i++)
				os_str[i]=' ';
			os_str[12]='\0';
			imemcpy(os[nos].os_name,os_str,13);		/* �����û��趨 */
			os[nos].os_valid=1;
			imemset(buff,0,512);
			linear_ReadWDK(os[nos].os_mbr, 1, buff);
			if(imemcmp(mbr.sig,"LUOB",4))				/* mbr������foxdiskmbr*/
				imemcpy(buff,ptr,512);
			else
				imemcpy(buff+iLoadLen,ptr+iLoadLen,512-iLoadLen);		/* ֻ���ط����� */
			/* �����趨 */
			setCur(x+8,y+40+24*9);
			if( linear_WriteWDK(os[nos].os_mbr, 1, buff) || savFoxKeyData(para,os,SAVEKEYOS))
				c_printf(WHITE,loados_str[18]);
			else
				c_printf(WHITE,loados_str[17]);	
		}
		else
		{
			setCur(x+8,y+40+24*8);
			c_printf(WHITE,loados_str[19]);
		}
	}
	disable1CH();							/* �ر�ʱ���ж� */
	RelIntRes(CLOCK);	
	asm xor ax,ax
	asm int 0x16
}

/* ����ؼ����� */
/*����0: ����ɹ� ����1:����ʧ�� */
int savFoxKeyData(FOXPARA *para,OS *my_os,unsigned int flag)
{
	FOXPARA old_para;
	OS old_os[4];
	unsigned char buff[512];

	if(flag&SAVEKEYPARA)	
	{
		imemset(buff,0,512);
		if(linear_ReadWDK(para->lba_ipara, 1, buff))
			return 0x01;
		imemcpy(&old_para,buff+0x10,sizeof(FOXPARA));
		if(imemcmp(para,&old_para,sizeof(FOXPARA)))		/* �ж��Ƿ���ͬ */
		{
			imemcpy(buff+0x10,para,sizeof(FOXPARA));			/* ��ͬ��׼������ */
			if(linear_WriteWDK(para->lba_ipara, 1, buff))
				return 0x01;
		}	
	}
	if(flag&SAVEKEYOS)	
	{
		imemset(buff,0,512);
		if(linear_ReadWDK(para->lba_ios, 1, buff))
			return 0x01;
		imemcpy(old_os,buff,sizeof(OS)*4);
		if(imemcmp(my_os,old_os,sizeof(OS)*4))		/* �ж��Ƿ���ͬ */
		{
			imemcpy(buff,my_os,sizeof(OS)*4);			/* ��ͬ��׼������ */
			if(linear_WriteWDK(para->lba_ios, 1, buff))
				return 0x01;
		}	
	}
	
	return 0;
	
}   
/* �汾��Ǩ������ */
/*����:0��ʾ�汾�����ɹ�,�����ʾʧ�� */
/* Ϊ�����汾�䲻ͬ�����ṩһ��Ǩ��ƽ̨ */
int upgradeFox(FOXPARA *para)
{     
	/* ������Ϊ���ݿ�����ͬ��,����Ҫת���������������������ʱ����������� */
	if(iFoxVer>=para->ver)
	{   
		return Setup(para->lba_ifox,SETUPFOX|SETUPMBR);
	}  
	return 0; 
}     
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
			
      
      
      
      
      
      
      
      
      
      
			
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      