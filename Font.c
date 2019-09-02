/* Font.c:�����Ѿ��еĻ��ߡ����㺯����������ַ���ʾ�ĺ��� */

#include "Vesa.h"											/* ��Ӳ����ص�ͼ�δ����� */
#include "Graphic.h"									/* ���������Ӳ���޹ص�ͼ�κ��� */
#include "Font.h"
#include "Global.h"
#include "iMath.h"

void selFONT(unsigned long font);
void	putStr(unsigned int x,unsigned int y,unsigned char *s,unsigned long color,unsigned char mag);

void	putASCII(unsigned int x,unsigned int y,unsigned char code,unsigned long color,unsigned char mag);
void	putHZ12(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag);
void	putHZ16(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag);
void	putHZ24(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag);
void	putHZ40(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag);
void	putHZ48(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag);
void	(* putHZ)(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag);
void	selHZK24(char mode);
void	selHZK12(char mode);
void	selHZK16(char mode);
void	selHZK40(char mode);
void	selHZK48(char mode);
void (* selHZK)(char mode);


/*���ܣ���ʾASCII�ַ�*/
/*��ڲ�����x,y:����   code:ASCII��   color����ɫ*/
/*          mag:magnification�Ŵ���*/
/*���ڲ�������*/
void	putASCII(unsigned int x,unsigned int y,unsigned char code,unsigned long color,unsigned char mag)
{
	int	i,j,index,m,n;
	
	for(i=0;i<ASCIIMAX;i++)
		if(ascii_table[i].code == code)
			break;
	index = i;
	for(j=0;j<CHARHEIGHT;j++)
		for(i=0;i<CHARWIDTH;i++)
		{
			if(ascii_table[index].array[j] & (1<<(CHARWIDTH-i)))
				for(n=0;n<mag;n++)
					for(m=0;m<mag;m++)
						putpixel(x+mag*i+m,y+mag*j+n,color);	
		}
}	
/* ѡ����Ҫ��ʾ��24x24���ֿ� */
/*��ڲ���: mode: 's' 'k' 'h' 't' 'f'*/
/*���ڲ���: �� */
void	selHZK16(char mode)
{
	/* 16x16 ��ֻ��һ������ ����ʾ������ʹ��HZCOUNT�͹̶���HZHEIGHT HZWIDTH������*/
	return;
}
/*���ܣ���ʾ����16X16�ֿ�			*/
/*��ڲ�����x,y:����   code:����   color����ɫ*/
/*          mag:magnification�Ŵ���*/
/*���ڲ�������																*/
void	putHZ16(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag)
{
	int	i,j,index,m,n;
	unsigned int zimo;
		
	for(i=0;i<hzcount;i++)							/* Ϊ�ӿ���ʾ�ٶ�,��ȡ�ĺ�����Ŀ�Ѿ����� */
		if(hzk16_table[i].code == code)
			break;
			
	index = i;
	if(index == hzcount) return;
	for(j=0;j<HZHEIGHT;j++)
	{
		zimo=hzk16_table[index].array[j];
		asm mov ax,zimo;
		asm xchg	al,ah
		asm mov zimo,ax
		
		for(i=0;i<HZWIDTH;i++)
			if((zimo>>(HZWIDTH-i))&0x01)
				for(n=0;n<mag;n++)
					for(m=0;m<mag;m++)
						putpixel(x+mag*i+m,y+mag*j+n,color);	
	}
}

/*���ܣ���ʾ����24X24�ֿ�			*/
/*��ڲ�����x,y:����   code:����   color����ɫ*/
/*          mag:magnification�Ŵ���*/
/*���ڲ�������																*/
/*ע��,��ʾ����ǰ������ʹ��selHZK24ѡ����Ӧ����ʾ����*/
void	putHZ24(unsigned int x,unsigned int y,unsigned int code,unsigned long color,unsigned char mag)
{
	int	i,j,index,k,m,n;
	unsigned long zimo;

	/*1 Ѱ�Ҷ�Ӧ����*/
	for(i=0;i<hz24_count;i++)
		if(hzk24_table[i].code==code)
			break;
	index=i;
	if((index == hz24_count)||(hz24_count==-1) )return;

	/*2 ��ʾ*/
	for(j=0;j<HZ24HEIGHT;j++)
	{
		zimo=0;
		
		for(k=0;k<HZ24HEIGHT;k++)
		{
			if(((hzk24_table[index].array[(k*3)+(j>>3)])>>(7-(j%8)))&0x01)
				zimo|=1;
			zimo<<=1;	
		}
	
		for(i=0;i<HZ24WIDTH;i++)
			if(lintshr(HZ24WIDTH-i,zimo)&0x01)	/*if((zimo>>(HZ24WIDTH-i))&0x01)*/
				for(n=0;n<mag;n++)
					for(m=0;m<mag;m++)
						putpixel(x+mag*i+m,y+mag*j+n,color);
	}
}
/* ѡ����Ҫ��ʾ��24x24���ֿ� */
/*��ڲ���: mode: 's' 'k' 'h' 't' 'f'*/
/*���ڲ���: �� */
void	selHZK24(char mode)
{
	switch(mode)
	{
	#ifdef HZK24S_ACTIVE	/* ���� */
		case 's':
    case 'S':
          hz24_count=HZK24SCOUNT;
          hzk24_table=HZK24Stable;
          break;
  #endif
  #ifdef HZK24K_ACTIVE
    case 'k':
    case 'K':
          hz24_count=hzk24kcount;
          hzk24_table=HZK24Ktable;
          break;
  #endif
  #ifdef HZK24F_ACTIVE
    case 'f':
    case 'F':
          hz24_count=HZK24FCOUNT;
          hzk24_table=HZK24Ftable;
          break;
  #endif
  #ifdef HZK24T_ACTIVE
    case 't':
    case 'T':
          hz24_count=HZK24TCOUNT;
          hzk24_table=HZK24Ttable;
          break;
  #endif
  #ifdef HZK24H_ACTIVE
    case 'h':
    case 'H':
          hz24_count=HZK24HCOUNT;
          hzk24_table=HZK24Htable;
          break;
	#endif
	default:
		hz24_count=-1;  
		break;	
	}
}
/*    ��ʾ�ַ���	*/
/*��ڲ�����x,y:����   *s:�ַ���   color����ɫ	*/
/*          mag:magnification�Ŵ���*/
/*���ڲ�������	*/
/*ע��,��ʾ����ǰ������ʹ��selFONTѡ����Ӧ����ʾ����*/
void	putStr(unsigned int x,unsigned int y,unsigned char *s,unsigned long color,unsigned char mag)
{
	unsigned int hzwidth,hzheight;
	unsigned int charwidth,charheight;
	unsigned int i=0,hz=0;
	/* ����Ӣ���ַ�������ֻ��һ��,�����Ҫ��ӵĻ�,Ҫ�޸Ĵ˴� */
	switch(selfont&0xff)
	{
		/*case _HZ12:							/* 12x16 *//*
			selHZK=selHZK12;
			putHZ=putHZ12;
			hzwidth=hzheight=12;
			break;*/
		case _HZ16:							/* 16x16 */
			selHZK=selHZK16;
			putHZ=putHZ16;
			hzwidth=hzheight=16;
			break;
		case _HZ24:							/* 24x24 */
			hzwidth=hzheight=24;
			selHZK=selHZK24;
			putHZ=putHZ24;
			break;
		/*case _HZ40:    					/* 40x40 *//*
			hzwidth=40;
			selHZK=selHZK40;
			putHZ=putHZ40;
			break;
	  case _HZ48:							/* 48x48 *//*
	  	hzwidth=48;
	  	selHZK=selHZK48;
			putHZ=putHZ48;
	  	break;*/
	  default:
	  	break;
	}
	switch((selfont)&0x0000ff00)
	{
		case HZKK:
			selHZK('k');
			break;
		case HZKS:
			selHZK('s');
			break;
		case HZKF:
			selHZK('f');
			break;
		case HZKH:
			selHZK('h');
			break;
		default:
			break;
	}
	/* ��Ӣ�ĵĴ�����ʱ���ڴ�ʵ��,�Ժ�����Ӱ� */
	charwidth=8;					/* ʹ�� 8x16�ַ� */
	charheight=16;
	
	while(*(s+i))
	{
		if(s[i]<128)
		{
			putASCII(x,(y+(hzheight-charheight)*mag),s[i],color,mag);
			i++;
			x+=(charwidth*mag);		
		}
		else
		{
			hz=(unsigned int)(s[i+1]);
			hz<<=8;
			hz+=(unsigned int)(s[i]);
			putHZ(x,y,hz,color,mag);
			i+=2;
			x+=(hzwidth*mag);
		}
	}
}
/* ѡ����Ҫ��ʾ������(���ֺ�Ӣ��) */
/*��ڲ���: �� ͷ�ļ��к궨��˵��*/
/*���ڲ���: �� */
void selFONT(unsigned long font)
{
	selfont=0;							/* ����ǰ����ʾģʽȡ�� */
	selfont=font;
}
/* ���ص�ǰ����ģʽ */
unsigned long getFONT(void)
{
	return selfont;
}
