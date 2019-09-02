/* my new int 1ch */
/* �ڱ��ļ��ڲ���ʹ�ø߲�ε���c_printf c_scanf�Ⱥ���,���������
   ֻ��ʹ��putStr���߸��ײ����ʾ���� */
#include "DISKDRV.h"													/* ��Ҫ�������е�һЩ�궨�� */
#include "Vesa.h"														/* ʹ����ײ����ʾ����,�ӿ촦���ٶ� */
#include "Graphic.h"
#include "Font.h"
#include "Painter.h"
#include "INT1ch.h"
#include "Global.h"

void InitIntRes(unsigned int sw);
void RelIntRes(unsigned int sw);
void InitIntRes(unsigned int sw);
void InitCntDown(unsigned int times,unsigned long strcorlor,unsigned long bgcolor,unsigned int x,unsigned int y);


unsigned int getCountOver(void);

/* ��ʼ��������Դ */
void InitIntRes(unsigned int sw)
{
	ENABLE(sw);
	switch(sw)
	{
		case CLOCK  :
			_hms_x=maxX-95;					/* ������ʾʱ�ӵ�λ�� */
			_hms_y=maxY-35;
			break;
		case CURSOR :
			bg_curcolor=DEEPBLUE;
			curcolor=YELLOW;
			cur_flag=1;
			break;
		case CNTDOWN:
			break;
		default:
			break;
	}	
}
/* ��ʼ������ʱ�ĸ�����Դ */
void InitCntDown(unsigned int times,unsigned long strcorlor,unsigned long bgcolor,unsigned int x,unsigned int y)
{
	unsigned int i,j;
	_cntd_x=x;					/* ����λ�� */
	_cntd_y=y;
	cntd_color=strcorlor;
	cntd_bcolor=bgcolor;
	cntdown_times=times;
	cntdown_over=0;
	time_rad=0;
	for(j=0;j<CNTD_SHEI+1;j++)				/* 0~15 */
		for(i=0;i<CNTD_SWID+1;i++)
			cov_pci[j*(CNTD_SWID+1)+i]=(unsigned char)(getpixel(i+x,j+y));
}
/* ����ʱ��ɵı�־ */
unsigned int getCountOver(void)
{
	return cntdown_over;
}

/* �ڹرո�����Ҫʱ���жϴ�����¼���,��Ҫ�ͷ���Ӧ��Դ*/
void RelIntRes(unsigned int sw)
{
	DISABLE(sw);
	switch(sw)
	{
		case CLOCK  :
			break;
		case CURSOR :
			line(x_cur,y_cur,x_cur,y_cur+15,(unsigned long)(bg_curcolor));
			break;
		case CNTDOWN:
			break;
		default:
			break;
	}
}

/* ����ʱ�Ĵ��� */
void _int_countDown(void)
{
	if((_sw_INT_&CNTDOWN) && (!cntdown_over))
	{
		asm cli
		if(time_rad<18)
			++time_rad;
		else
		{
			unsigned int i,j;
			unsigned long font;
			font=getFONT();
			time_rad=1;
			
			/* ����ϴ���ʾ */
			for(j=0;j<CNTD_SHEI+1;j++)				/* 0~15 */
				for(i=0;i<CNTD_SWID+1;i++)
					putpixel(_cntd_x+i,_cntd_y+j,(unsigned long)(cov_pci[j*(CNTD_SWID+1)+i]));
			/* ��ʾ��ǰ����ʱ�� */
			i=cntdown_times/10;				/* ʮλ�� */
			j=(cntdown_times%10);			/* ��λ�� */
			cntd_str[0]=i_number[i];
			cntd_str[1]=i_number[j];
			selFONT(ASC0|_ASC0|HZKK|_HZ16);	
			putStr(_cntd_x+1,_cntd_y+1,cntd_str,cntd_bcolor,2);
			putStr(_cntd_x,_cntd_y,cntd_str,cntd_color,2);
			if(cntdown_times==0)cntdown_over=1;
			--cntdown_times;
			selFONT(font);
			asm sti
		}
	}
}
void _int_Clock(void)
{
	if(_sw_INT_&CLOCK)
	{
		unsigned int temp;
		unsigned char temp1;
		unsigned long font;
		font=getFONT();
		asm cli
		asm mov ax,0x0200
		asm int 0x1a
		//asm mov ch,dh				/* ������ lbdebug*/
		asm jc clock_pass
		asm mov temp,cx
		asm mov temp1,dh
		if((oldhour_min!=temp) || (old_sec!=temp1))
		{
			oldhour_min=temp;
			old_sec=temp1;
			/* �Ѿ���ȡ�˵�ǰʱ��,������ʾ�� */
			i_when[0]=i_number[(oldhour_min>>12) &0x0f];
			i_when[1]=i_number[(oldhour_min>>8) &0x0f];
			i_when[3]=i_number[(oldhour_min>>4) &0x0f];
			i_when[4]=i_number[oldhour_min &0x0f];
			i_when[6]=i_number[(old_sec>>4) &0x0f];
			i_when[7]=i_number[old_sec&0x0f];
			i_when[8]='\0';
			rectblock(_hms_x,_hms_y,_hms_x+95-17,_hms_y+35-9,GROUND1);
			selFONT(ASC0|_ASC0|HZKK|_HZ16);				/* ��Ϊһ����Ȥ������,������ѡ����^^*/
			putStr(_hms_x+6,_hms_y+5,i_when,WHITE,1);
			selFONT(font);
		}
		clock_pass:
		asm sti
	}
}
void _int_Cursor(void)
{
	#define onesecond 9							/*1/2��ļ���������Ϊ18������*/
	if(i_int_count<onesecond)
		{
			++i_int_count;
			return;
		}
	i_int_count=0;
	if(_sw_INT_&CURSOR)	
	{
		if(cur_flag)			/* ��ʾ */
		{
			cur_flag=0;
			line(x_cur,y_cur,x_cur,y_cur+15,(unsigned long)(curcolor));
		}
		else							/* ������ʾ */
		{
			cur_flag=1;
			line(x_cur,y_cur,x_cur,y_cur+15,(unsigned long)(bg_curcolor));
		}
	}
}
