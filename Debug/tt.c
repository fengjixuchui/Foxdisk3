#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

typedef struct _WallPaper{
	unsigned long lba_addr;						/* �����Ӳ���еĵ�ַ */
	/*unsigned long size;						  /* ��ֽȱʡΪ1024*768*/
	unsigned char valid;
}iWallPaper;

typedef struct FoxdiskPara{
	signed int cntdown_time;						/* ����ʱʱ��,-1Ϊ��Ч */
	unsigned int cntdown_x;							/* ��ʾ����ʱ������ */
	unsigned int cntdown_y;
	signed int def_os;									/* Ĭ�Ͻ����ϵͳ,0��4 */
	unsigned int wpaper_num;						/* ��ֽ����Ŀ,���Ϊ0��wallpaper����Ϊ-1 */
	signed int wallpaper;								/* ��ֽ��ѡ�� -1��ʾ��Ч,�����ر�ֽ */
	unsigned long lba_ipara;						/* �������ŵ�lba��ַ �ɰ�װ�����ʼ�� */
	unsigned long lba_ios;							/* OS��Ϣ��ŵ�lba��ַ �ɰ�װ�����ʼ�� */	
	unsigned int ver;									  /* �汾��Ǩ����,��0x0100��ʼ */
	iWallPaper iwapaper[6];
	
	/*signed int modified;								/* �û��Ƿ������ϵͳ���� 0Ϊδ���� */
}FOXPARA;
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
void tt(FOXPARA *para)
{
	FOXPARA old_para={0xf0f0,0x55};
	unsigned char *now,*old,*tx;
	unsigned char *ptr,*ptr1;
	int i;
	
	now=(unsigned char *)para;
	old=(unsigned char *)(&old_para);
	ptr1=(unsigned char *)(&old_para);
	printf("\nluobing old[0]=%d\n",old[0]);
	printf("\nluobing ptr1[0]=%d\n",ptr1[0]);
	

	
	for(i=0;i<sizeof(FOXPARA);i++)
		old[i]=now[i];
	tx=(unsigned char *)(&old_para);	

	printf("old:");
	for(i=0;i<sizeof(FOXPARA)/3;i++)
		printf("%d,",old[i]);
	printf("\ntx:");
	for(i=0;i<sizeof(FOXPARA)/3;i++)
		printf("%d,",tx[i]);
		
	printf("\nnow:");
	for(i=0;i<sizeof(FOXPARA)/3;i++)
		printf("%d,",now[i]);	
}

void ttt(FOXPARA *para)
{
	FOXPARA old_para={0x0f0f,0x55};
	int i;
	unsigned char *cur,*old;
	unsigned char *ptr,*ptr1;
	
	ptr1=(unsigned char *)(&old_para);
	old=(unsigned char *)(&old_para);
	printf("\nluobing ptr1[0]=%d\n",ptr1[0]);
	printf("\nluobing old[0]=%d\n",old[0]);
	
	ptr=(unsigned char *)(para);
	for(i=0;i<sizeof(FOXPARA);i++)
		ptr1[i]=ptr[i];	
		
		ptr1[2]=0x22;
		cur=(unsigned char *)para;
		//old=(unsigned char *)(&old_para);
		printf("cur:");
		for(i=0;i<sizeof(FOXPARA)/2;i++)
			printf("%d,",cur[i]);
		printf("\nold:");
		for(i=0;i<sizeof(FOXPARA)/2;i++)
			printf("%d,",old[i]);
		printf("\nptr-para:");
		for(i=0;i<sizeof(FOXPARA)/2;i++)
			printf("%d,",ptr[i]);
		printf("\nptr1-old_para:");
		for(i=0;i<sizeof(FOXPARA)/2;i++)
			printf("%d,",ptr1[i]);
	
}
void tttt(FOXPARA *para)
{
	unsigned char *old,*ptr1;
	FOXPARA old_para={0xf0f0,0x55};
	
	old=(unsigned char *)(&old_para);
	ptr1=(unsigned char *)(&old_para);
	printf("\nluobing old[0]=%d\n",old[0]);
	printf("\nluobing ptr1[0]=%d\n",ptr1[0]);
}
void	main(void)
{
	FOXPARA para={30,750,660,1,6,5};
	
	tt(&para);
	
	tttt(&para);
	ttt(&para);
}