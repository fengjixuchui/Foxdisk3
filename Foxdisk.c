#include "Vesa.h"
#include "Graphic.h"
#include "Font.h"

#include "Common.h"
#include "Painter.h"
#include "SetPara.h"
#include "MainMenu.h"
#include "BootMenu.h"

#include "DISKDRV.h"
#include "DISKAPP.h"
#include "Foxdisk.h"

void foxdisk(void);
unsigned int initEntry(FOXPARA *para,OS *my_os,unsigned long lba_para);
void abnormalExit(unsigned int err);
int SavFoxPara(FOXPARA *para);
int SavOSMsg(FOXPARA *para,OS *os);
int i_memcmp(void *cs,void *ct,unsigned int count);
void	BootEntry(void);

/* boot�������foxdisk�����׶ε���� */
void	BootEntry(void)
{
	foxdisk();
}

/* ִ�г��� */
/*��Ҫ����: �ṩ�����׶εĴ���: ��ʾ�����׶κ�������׶� */
void foxdisk(void)
{
	FOXPARA para;
	OS my_os[4];
	iWallPaper iwapaper[5];					/* ��ֻ�а�װ������ܸı����Ŀֵ,���Ե����г���Ϊֻ���������� */
	unsigned long LBA_iPara=0;							/* �������λ��,���ŵ���-10 ����519Ϊ���ŵ���,�������ʼλ��Ϊ510*/
	unsigned int err=0;
	/* ��һ��,��ָ���Ĵ洢λ�û�ȡ������Ϣ */
	CreateDiskParaTable();									/* ����������,Ԥ������Ӳ�̷��� */
	LBA_iPara=CHS_to_Sect(CUR_DK->lgeo.maxCyl+1,0,1,CUR_DK);
	err=initEntry(&para,my_os,LBA_iPara);
	if(err)
		abnormalExit(err);
	/*lbdebug_init(&para,my_os);*/
	/* �ڶ���,�����׶���ʾ */
	bootMeun(&para,my_os);				/* ���û�а�'F2'�Ļ�,��ֱ�ӽ��뵱ǰϵͳ */

	/* ������,������׶� */
	SysRun(my_os,&para);
}
/* ��ڱ����ĳ�ʼ�� */
/*��ڲ���: FOXPARA -foxdisk�����Ĵ�ŵ�ַ */
/*���ڲ���: 0:��ȡ�ɹ� ��0:����,���ִ����ͷ�ļ��еĶ��� */
unsigned int initEntry(FOXPARA *para,OS *my_os,unsigned long lba_para)
{
	unsigned char buffer[512],*ptr;
	int i;

	for(i=0;i<512;i++)buffer[i]=0;
	/* ��Ӳ��ָ��λ�û�ȡ��Ϣ */
	if ( linear_ReadWDK(lba_para, 1, buffer) != 0 )					/* ��ָ��λ�ö�ȡ��Ϣ*/
		return PARALBAERR;
	if(!((buffer[0]=='L') && (buffer[1]=='U') && (buffer[2]=='O') && (buffer[3]=='B') ))
		return PARALBAERR;						/* �ж�Ԥ�ŵı�־ 'LUOB' */
	/* 0x10��ʼ��ŵ���para����Ϣ */	
	ptr=(unsigned char *)(para);
	for(i=0; i<sizeof(FOXPARA); i++)
		ptr[i]=buffer[i+0x10];
	if(para->lba_ipara!=lba_para)
		return PARAADRERR;
	/* ��ȡOS����Ϣ */
	ptr=(unsigned char *)(my_os);
	if( linear_ReadWDK(para->lba_ios, 1, buffer) != 0 )				/* ��ȡϵͳ��Ϣ */
		return OSMSGERR;
	for(i=0;i<sizeof(OS)*4;i++)
		ptr[i]=buffer[i];

	return 0;
}


/* ��������� */
/*���ڲ���: 0:����ɹ� 0x01:����ʧ��*/
int SavFoxPara(FOXPARA *para) 
{
	unsigned char buff[512],*ptr;
	int i;
	if( linear_ReadWDK(para->lba_ipara, 1, buff) != 0 )
		return 1;
	if(i_memcmp(buff+0x10,para,sizeof(FOXPARA)))		/* �Ƚ��Ƿ���ͬ */
	{
		ptr=(unsigned char *)(para);
		for(i=0;i<sizeof(FOXPARA);i++)
			buff[0x10+i]=ptr[i];
		if(linear_WriteWDK(para->lba_ipara, 1, buff) != 0)
			return 1;
	}
	return 0;
}
/* �����ĸ�ϵͳ����Ϣ */
/*���ڲ���: 0:����ɹ� 0x01:����ʧ��*/
int SavOSMsg(FOXPARA *para,OS *os) 
{
	unsigned char buff[512],*ptr;
	int i;
	if( linear_ReadWDK(para->lba_ios, 1, buff) != 0 )	/* ��ȡ��Ϣ */
		return 1;
	if(i_memcmp(buff,os,(sizeof(OS)<<2)))		/* �Ƚ�4��ϵͳ��Ϣ�Ƿ���ͬ */
	{
		ptr=(unsigned char *)(os);
		for(i=0;i<(sizeof(OS)<<2);i++)
			buff[i]=ptr[i];
		if(linear_WriteWDK(para->lba_ios, 1, buff) != 0)
			return 1;
	}
	return 0;
}

/* �ڴ�Ƚ� */
/*���ڲ���: <0 cs<ct   =0 cs=ct   >1 cs>ct*/
int i_memcmp(void *cs,void *ct,unsigned int count)
{
	unsigned char *su1,*su2;
	int res=0;
	for(su1=cs,su2=ct;count>0;++su1,++su2,count--)
		if((res=*su1-*su2)!=0)
			break;
	return res;
}  

/* ��������³���������˳� */
void abnormalExit(unsigned int err)
{
  asm {
  	jmp	idata_over
			db 'Foxdisk', 0x27,'s parameter files lost!',00h
  		db 'What you can do:',00h
  		db '1 Reinstall foxdisk.',00h
      db '2 Regret.',00h
      db '3 Vituperation.',00h            		
      db '4 Crying.',00h            		
      db '5 Never use foxdisk again.',00h            		
      db 'I choose 1,what about you?^^',00h                
  }
  idata_over:
  
}  
  
  