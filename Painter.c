/* Painter.c:�����Ѿ��еĻ��ߡ����㺯���������ͼ����Ҫ������Ԫ�� */
#include "Vesa.h"											/* ��Ӳ����ص�ͼ�δ����� */
#include "Graphic.h"									/* ���������Ӳ���޹ص�ͼ�κ��� */
#include "Font.h"
#include "Common.h"
#include "Painter.h"
#include "INT1ch.h"
#include "Global.h"
#include "iMath.h"

void gear(unsigned int cenx,unsigned int ceny,unsigned int radius);
unsigned int	c_strlen(unsigned char *s);
unsigned int	c_strlen_ascii(unsigned char *s);
int c_strncmp(unsigned char *str1,unsigned char *str2,unsigned int count);
void	navigationBar(unsigned int x,unsigned int y,unsigned long font,unsigned char *s,unsigned long color,unsigned char mag);
void chgNavbar(unsigned int x,unsigned int y,unsigned long font,unsigned char *s,unsigned long color,unsigned char mag);
void iFacePic(unsigned int ibottom,unsigned int iright);
void	iFasceStr(unsigned char turn,unsigned long color);
void	iFaceClr(unsigned int ibottom);
void iMessage(unsigned long color);
void	iItem(unsigned int x,unsigned int y,unsigned char *s,unsigned long color);
void iItemSel(unsigned int x,unsigned int y,unsigned char *s,unsigned long color);

void	iWindowOut(unsigned int x,unsigned int y,unsigned char *wstr,unsigned int lines);
void Pic_Wing(unsigned int x,unsigned int y,unsigned int color_in,unsigned int color_out,unsigned char direct);
void iScrollBar(unsigned int winx,unsigned int winy,unsigned int num_line,unsigned int lines);
void	iWindowSlide(unsigned int winx,unsigned int winy,unsigned int minline,unsigned int maxline,unsigned char up_down);

void _i2asc(unsigned int num,int base,unsigned char *out);
void _l2asc(unsigned long num,int base,unsigned char *out);
void c_printf(unsigned long color, const char *format,...);
void c_scanf(unsigned long bg_color,unsigned long color,const char *format,...);
void chgLine(unsigned int x,unsigned int y,unsigned int num_line,unsigned long color);
unsigned long getKbin(unsigned long bg_color,unsigned long color,unsigned int num_base,unsigned char *out);
void setCur(unsigned int x,unsigned int y);
unsigned int getXCur(void);
unsigned int getYCur(void);



/* ������������ */
/*��ڲ���: ��  */
/*���ڲ���: ��  */
/*��ע: ���ʹ��˫��8x16 ��Ӣ���ַ� ��ʼ y=250,dy=80 */
/*      ���ʹ��24x24���� ��ʼy=250,dy=64*/
void iFacePic(unsigned int ibottom,unsigned int iright)
{
	/* �²����16�ַ���*/
	unsigned int i,j,m,n;
		
	circle(160,160,72+5,BLACK);	
	circle(160,160,72+4,LIGHTSHADOW1);	
	circle(160,160,72+3,SURFACE1);
	circle(160,160,72+2,SURFACE1);
	circle(160,160,72+1,DARKSHADOW1);	
	circle(160,160,72,DARKPROJECT1);
	
	circle(260,112,54+5,BLACK);	
	circle(260,112,54+4,LIGHTSHADOW1);	
	circle(260,112,54+3,SURFACE1);
	circle(260,112,54+2,SURFACE1);
	circle(260,112,54+1,DARKSHADOW1);	
	circle(260,112,54,DARKPROJECT1);

	
	rectblock(200,101,236,200,GROUND1);			/*13�ַ�*16 =208 208-8*/
	rectblock(233,112,320,174,GROUND1);			/*112+54=166 */	
	putpixel(202,100,DARKPROJECT1);
	putpixel(202,99,SURFACE1);
	line(203,98,203,96,SURFACE1);						/*΢��*/
	gear(160,160,64);
	gear(260,112,48);
	rectblock(220,200,225,207,GROUND1);
	line(220,200,220,maxY-ibottom,DARKPROJECT1);
	line(220-1,200+4,220-1,maxY-ibottom-1,DARKSHADOW1);
	line(220,maxY-ibottom,0,maxY-ibottom,DARKPROJECT1);
	line(220-2,maxY-ibottom-1,0,maxY-ibottom-1,DARKSHADOW1);
	line(220-2,200+5,220-2,maxY-ibottom-1-40,SURFACE1);				/* ����߶�Ϊ40*/
	line(220-3,200+6,220-3,maxY-ibottom-1-40-1,SURFACE1);
	line(220-4,200+7,220-4,maxY-ibottom-1-40-2,LIGHTSHADOW1);
	line(220-5,200+8+7,220-5,maxY-ibottom-1-40-3,BLACK);
	line(220-5,maxY-ibottom-1-40-3,0,maxY-ibottom-1-40-3,BLACK);
	line(220-4,maxY-ibottom-1-40-2,0,maxY-ibottom-1-40-2,LIGHTSHADOW1);
	rectblock(0,maxY-ibottom-1-40-1,220-2,maxY-ibottom-2,SURFACE1);
	
	line(maxX-iright,0,maxX-iright,112,DARKPROJECT1);
	line(260+54,112,maxX-iright,112,DARKPROJECT1);
	line(260+54+1,112-1,maxX-iright-1,112-1,DARKSHADOW1);
	line(260+54+2,112-2,maxX-iright-2,112-2,SURFACE1);
	line(260+54+3,112-3,maxX-iright-2,112-3,SURFACE1);
	line(260+54+4,112-4,maxX-iright-32-1,112-4,LIGHTSHADOW1);			/* �ұ߿��32�������������ַ�����*/
	line(260+54+5,112-5,maxX-iright-32-2,112-5,BLACK);
	line(maxX-iright-32-2,112-5,maxX-iright-32-2,0,BLACK);
	line(maxX-iright-32-1,112-4,maxX-iright-32-1,0,LIGHTSHADOW1);	
	rectblock(maxX-iright-32,112-4,maxX-iright-2,0,SURFACE1);
	line(maxX-iright-1,112-1,maxX-iright-1,0,DARKSHADOW1);
	
	rectblock(0,0,maxX-iright-32-2-1,112-(54+5)-1,MGROUND1);
	rectblock(maxX-iright-32-2-1,112-5-1,260+54+6,112-(54+5)-1,MGROUND1);
	/* ����������� */
	for(j=52;j<=106;j++)
	{
		for(i=320;i>112;i--)
			if(getpixel(i,j)==BLACK)break;
			line(i+1,j,320,j,MGROUND1);
	}
	/*miny=52 maxy=237
	maxx=260*/
	for(j=52;j<=237;j++)
	{
		for(i=0;i<260;i++)
			if(getpixel(i,j)==BLACK)break;
			line(0,j,i-1,j,MGROUND1);
	}
	rectblock(0,238,220-6,maxY-ibottom-1-40-4,MGROUND1);
	/*maxx=220-6  maxy=237
	  miny=210*/
	for(j=216;j<=237;j++)
	{
		for(i=220-6;i>160;i--)
			if(getpixel(i,j)==BLACK)break;
			line(i+1,j,220-6,j,MGROUND1);
	}
	/*miny=83  maxy=94
	maxx=210*/
	for(j=83;j<=94;j++)
	{
		i=210;
		while(getpixel(i,j)!=BLACK)
		{
			i--;
		}
		m=i;
		i-=1;
		while(getpixel(i,j)!=BLACK)
		{
			i--;
		}
		line(i+1,j,m-1,j,MGROUND1);
	}
	iFasceStr(0,WHITE);
	
}
/* ��ʾ���������ַ� */
/*��ڲ���: turn:0 ��ʾ�����ַ� 1,2,3,4,5: ��ʾ��x���ַ��� */
/*          color�ַ�������ɫ */
/*���ڲ���: ��*/
void	iFasceStr(unsigned char turn,unsigned long color)
{
	unsigned int m,n,len,j;
	unsigned int len1,len2,len3,len4;
	m=c_strlen(iFaceStr1);						/* ������ */
	n=c_strlen_ascii(iFaceStr1);			/* ascii������ */
	len1=(n<<3)+(m-n)*12;
	m=c_strlen(iFaceStr2);						/* ������ */
	n=c_strlen_ascii(iFaceStr2);			/* ascii������ */
	len2=(n<<3)+(m-n)*12;
	m=c_strlen(iFaceStr3);						/* ������ */
	n=c_strlen_ascii(iFaceStr3);			/* ascii������ */
	len3=(n<<3)+(m-n)*12;
	m=c_strlen(iFaceStr4);						/* ������ */
	n=c_strlen_ascii(iFaceStr4);			/* ascii������ */
	len4=(n<<3)+(m-n)*12;
	m=c_strlen(iFaceStr);					/* �ܹ����ַ���Ŀ */
	n=c_strlen_ascii(iFaceStr);		/* ascii���ַ���Ŀ*/
	len=((n+3)<<3)+((m-n)*12);		/* �ұ�Ԥ��3��ASCII�ַ� */
	selFONT(ASC0|_ASC0|HZKK|_HZ24);
	switch(turn)
	{
		case 0:
			/* ��ʾ iFaceStr */
			line(maxX-len,maxY-48,maxX,maxY-48,WHITE);	/* ����Ԥ����������ַ��߶�,����Ԥ����������ַ��߶�*/
			cirx(maxX-len,maxY-48+16,16,WHITE,2);
			line(maxX-len-16,maxY-48+16,maxX-len-16,maxY,WHITE);
			putStr(maxX-len,maxY-48+12,iFaceStr,color,1);
			j=8+3;				/* 00.00.00 8���ַ�,�ұ�Ԥ���������ַ�*/
			line(maxX-((j+1)<<3),maxY-48+12,maxX-(2<<3),maxY-48+12,DARKPROJECT1);
			line(maxX-(2<<3),maxY-48+12,maxX-(2<<3),maxY-8,LIGHTSHADOW1);
			line(maxX-(2<<3),maxY-8,maxX-((j+1)<<3),maxY-8,LIGHTSHADOW1);
			line(maxX-((j+1)<<3),maxY-7,maxX-((j+1)<<3),maxY-48+12,DARKPROJECT1);
			rectblock(maxX-((j+1)<<3)+1,maxY-48+12+1,maxX-(2<<3)-1,maxY-8-1,GROUND1);
		break;
		case 1:
			putStr(maxX-len,maxY-48+12,iFaceStr1,color,1);
			break;
		case 2:
			putStr(maxX-len+len1,maxY-48+12,iFaceStr2,color,1);
			break;
		case 3:
			putStr(maxX-len+len1+len2,maxY-48+12,iFaceStr3,color,1);
			break;
		case 4:
			putStr(maxX-len+len1+len2+len3,maxY-48+12,iFaceStr4,color,1);
			break;
		case 5:
			putStr(maxX-len+len1+len2+len3+len4,maxY-48+12,iFaceStr5,color,1);
			break;
		default:
			break;
		
	}
}
/* ���ϵͳ�˵�λ�õ�ͼ��Ϊ��һ���˵���׼��*/
/*��ڲ���: ibottom: ��ײ���λ��,��iFacePic�е���Ӧ����������ͬ */
/*���ڲ���: ��*/
void	iFaceClr(unsigned int ibottom)
{
	rectblock(0,238,220-6,maxY-ibottom-1-40-4,MGROUND1); 
	line(220-5,238,220-5,maxY-ibottom-1-40-4,BLACK);
	line(220-4,238,220-4,maxY-ibottom-1-40-4,LIGHTSHADOW1);
	line(220-3,238,220-3,maxY-ibottom-1-40-4,SURFACE1);
	line(220-2,238,220-2,maxY-ibottom-1-40-4,SURFACE1);
	line(220-1,238,220-1,maxY-ibottom-1-40-4,DARKSHADOW1);
	line(220,238,220,maxY-ibottom-1-40-4,DARKPROJECT1);
	rectblock(220+1,238,220+1+30,maxY-ibottom-1-40-4,GROUND1);  /*ע��,�������ֲ��ô���30����*/
}
/* ��ʾ������Ϣ */
/*��ڲ���: color��ɫ*/
/*���ڲ���: ��*/
void iMessage(unsigned long color)
{
	if(color!=GROUND1)
	{
		selFONT(ASC0|_ASC0|HZKK|_HZ24);
		putStr(400-1,300,ifoxdisk,LIGHTSHADOW1,5);
		putStr(400+1,300,ifoxdisk,BLACK,5);
		putStr(400,300+1,ifoxdisk,SURFACE1,5);
		putStr(400,300,ifoxdisk,color,5);
		putStr(530-1,424,iflag,LIGHTSHADOW1,1);
		putStr(530+1,424,iflag,BLACK,1);
		putStr(530,424,iflag,color,1);
	}   
	else
	{
		rectblock(400-1,300,400+5*8*12,424+24,GROUND1);
	}
}
/* ���ֵĻ��� */
/*��ڲ���: cenx,ceny:���ֵ�Բ�� radius:��������Χ�뾶 */
/*���ڲ���: �� */
void gear(unsigned int cenx,unsigned int ceny,unsigned int radius)
{
	/*sin45(o) ~ 45/64    */
	/*sin22.5(o) ~ 49/128 */
	/*cos22.5(o) ~ 59/64  */
	#define out_color	BLACK
	#define clr_color bkGColor
	#define outB_color	LIGHTGRAY				/* ����Χ������ɫ */
	#define midB1_color	MLIGSHADOW1			/* ����Ȧ��ɫ */
	#define midB2_color	DARKBLACK				/* ��Ȧ��ɫ 	*/
	#define inB_color		LIGHTBLACK		  /* ����Ȧ��ɫ */
	#define inBl_color	WHITE
	
	unsigned int r4_1,r8_1,r8_7,r4_1_1,r8_1_1,r8_7_1;
	unsigned int rev1,rev2,minx,maxx,maxy,x,y,lx,rx,i,flag;
	rev1=(radius-(radius>>2));
	r4_1=((radius*45)>>6);			/* rsin45   */
	r8_1=((radius*49)>>7);			/* rsin22.5 */
	r8_7=((radius*59)>>6);			/* rcos22.5 */
	
	r4_1_1=((rev1*45)>>6);			/* rsin45   */
	//r4_1_1=((rev1*91)>>7);
	r8_1_1=((rev1*49)>>7);			/* rsin22.5 */
	r8_7_1=((rev1*59)>>6);			/* rcos22.5 */
	
	/*1 8���� */
	line(cenx,ceny-radius,cenx,ceny+radius,out_color);
	line(cenx+r8_1,ceny-r8_7,cenx-r8_1,ceny+r8_7,out_color);
	line(cenx+r4_1,ceny-r4_1,cenx-r4_1,ceny+r4_1,out_color);
	line(cenx+r8_7,ceny-r8_1,cenx-r8_7,ceny+r8_1,out_color);
	line(cenx+radius,ceny,cenx-radius,ceny,out_color);	
	line(cenx+r8_7,ceny+r8_1,cenx-r8_7,ceny-r8_1,out_color);
	line(cenx+r4_1,ceny+r4_1,cenx-r4_1,ceny-r4_1,out_color);
	line(cenx+r8_1,ceny+r8_7,cenx-r8_1,ceny-r8_7,out_color);
	/*2 ��ΧԲ */
	circle(cenx,ceny,radius,out_color);
	circle(cenx,ceny,rev1,out_color);
	
	/*3 ��� */
	line(cenx,ceny-rev1,cenx,ceny+rev1,clr_color);
	line(cenx+rev1,ceny,cenx-rev1,ceny,clr_color);	
	/*4 ����һ������ */
	minx=cenx-r8_1;
	maxx=cenx;
	maxy=ceny-(r8_7_1);
	y=ceny-radius;
	while(y<maxy)
	{
		if(y<=(ceny-r8_7))
		{
			for(x=maxx;x>minx;x--)
			{
				if(getpixel(x,y)==out_color)
				{
					rx=x;
					break;
				}
			}
			for(x=minx;x<maxx;x++)
			{
				if(getpixel(x,y)==out_color)
				{
					lx=x;
					break;
				}
			}
		}
		else
		{
			for(x=minx;x<maxx;x++)
			{
				if(getpixel(x,y)==out_color)
				{
					lx=x;
					break;
				}
			}
			for(x=maxx;x>minx;x--)
			{
				if(getpixel(x,y)==out_color)
				{
					rx=x;
					break;
				}
			}
		}
		for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel(i,y,outB_color);
		for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel((cenx<<1)-i,(ceny<<1)-y,outB_color);
		y++;
	}
	/* �ڶ�������*/
	minx=cenx+r8_1_1;
	maxx=cenx+r4_1;
	maxy=ceny-r4_1_1;
	y=ceny-r8_7;
	while(y<maxy)
	{
		if(y>(ceny-r4_1))
		{
			for(x=maxx;x>minx;x--)
			{
				if(getpixel(x,y)==out_color)
				{
					rx=x;
					break;
				}
			}
			for(x=minx+1;x<maxx;x++)
			{
				if(getpixel(x,y)==out_color)
				{
					lx=x;
					break;
				}
			}
		}
		else
		{
			for(x=minx;x<maxx;x++)
			{
				if(getpixel(x,y)==out_color)
				{
					lx=x;
					break;
				}
			}
			for(x=maxx;x>minx;x--)
			{
				if(getpixel(x,y)==out_color)
				{
					rx=x;
					break;
				}
			}
		}
		for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel(i,y,outB_color);
		for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel((cenx<<1)-i,(ceny<<1)-y,outB_color);
		y++;
	}
	lx=minx;
	rx=maxx;
	y=ceny-r4_1-1;
	for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel(i,y,outB_color);
	for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
		putpixel((cenx<<1)-i,(ceny<<1)-y,outB_color);
	
	line(minx,ceny-r4_1-1,maxx,ceny-r4_1-1,outB_color);
	/* ����������*/
	minx=cenx+r8_7_1;
	maxx=cenx+radius;
	maxy=ceny+1;
	y=ceny-r8_1;
	while(y<maxy)
	{
		for(x=maxx;x>minx;x--)
		{
			if(getpixel(x,y)==out_color)
			{
				rx=x;
				break;
			}
		}
		for(x=minx+1;x<maxx;x++)
		{
			if(getpixel(x,y)==out_color)
			{
				lx=x;
				break;
			}
		}
			
		for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel(i,y,outB_color);
		for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel((cenx<<1)-i,(ceny<<1)-y,outB_color);
		y++;
	}
	
	/* ���ĸ�����*/
	minx=cenx+r4_1_1;
	maxx=cenx+r8_7;
	maxy=ceny+r4_1;
	y=ceny+r8_1_1;
	while(y<maxy)
	{
		for(x=minx+1;x<maxx;x++)
		{
			if(getpixel(x,y)==out_color)
			{
				lx=x;
				break;
			}
		}
		for(x=maxx;x>minx;x--)
		{
			if(getpixel(x,y)==out_color)
			{
				rx=x;
				break;
			}
		}
		for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel(i,y,outB_color);
		for(i=lx;i<=rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel((cenx<<1)-i,(ceny<<1)-y,outB_color);
		y++;
	}
	/*5 ��ʼ��Բ */
	circle(cenx,ceny,radius,clr_color);

	cirblock(cenx,ceny,rev1,midB1_color);
	cirblock(cenx,ceny,rev1-(rev1>>3),midB2_color);
	cirblock(cenx,ceny,(rev1-(rev1>>3))>>1,inB_color);
	circle(cenx,ceny,((rev1-(rev1>>3))>>1)-1,midB2_color);
	rev2=((rev1-(rev1>>3))>>3);
	cirblock(cenx,ceny-(rev2<<1)-(rev2<<2),rev2,inBl_color);
	cirblock(cenx+(rev2<<1)+(rev2<<2),ceny,rev2,inBl_color);
	cirblock(cenx,ceny+(rev2<<1)+(rev2<<2),rev2,inBl_color);
	cirblock(cenx-(rev2<<1)-(rev2<<2),ceny,rev2,inBl_color);

}
/* ���������� */
/*��ڲ���: x,y:���� font:���� s:�ַ��� color:��ɫ mag:����*/
/*���ڲ���: �� */
/*��ע: ��iFacePic�����µĵ�����ʼλ��Ϊy=250 */
void	navigationBar(unsigned int x,unsigned int y,unsigned long font,unsigned char *s,unsigned long color,unsigned char mag)
{
	/* �ṩ��������ѡ��,Ϊ�˱�����ʾ����,���ڴ˺����д���*/
	/* ��ʾʱ�����Ҹ�����һ���ַ����,���¿����1/4�߶� */
	/* Ϊ����ʾЧ��,��iFacePic����ʹ�� ������ӰΪ220-6����*/
	unsigned int charhei,charwid;				/* �ַ��ĸ߶ȺͿ�� */
	unsigned int height,length;					/* ��ʾ�ַ����Ŀ�ĳ��Ϳ�*/
	
	switch(font&0x0f)
	{
		case _HZ16:		
			charwid=8*mag;
			charhei=16*mag;
				break;
		case _HZ24:
			charwid=12*mag;
			charhei=24*mag;
				break;
		default:
			break;
	}
	length=((c_strlen(s)+2)*charwid);
	height=(charhei>>1)+charhei;
	line(x,y,(x+4+charwid+length),y,BLACK);
	line(x,y+1,(x+2+charwid+length),y+1,LIGHTSHADOW1);
	rectblock(x,y+2,(x+2+charwid+length),y+2+(charhei>>2),SURFACE1);
	line(x,y+3+(charhei>>2),x+length+1,y+3+(charhei>>2),DARKSHADOW1);
	line(x,y+4+(charhei>>2),x+length+1,y+4+(charhei>>2),BLACK);
	rectblock(x,y+5+(charhei>>2),x+length,y+5+(charhei>>2)+height,MGROUND1);
	line(x,y+5+(charhei>>2)+height+1,x+length+1,y+5+(charhei>>2)+height+1,BLACK);
	line(x,y+5+(charhei>>2)+height+2,x+length+2,y+5+(charhei>>2)+height+2,LIGHTSHADOW1);
	rectblock(x,y+5+(charhei>>2)+height+3,(x+2+charwid+length),y+5+(charhei>>2)+height+3+(charhei>>2),SURFACE1);
	line(x,y+(charhei>>1)+height+9,(x+3+charwid+length),y+(charhei>>1)+height+9,DARKSHADOW1);
	line(x,y+(charhei>>1)+height+10,(x+4+charwid+length),y+(charhei>>1)+height+10,BLACK);
	
	line(x+length+1,y+5+(charhei>>2),x+length+1,y+5+(charhei>>2)+height,BLACK);
	line(x+length+2,y+3+(charhei>>2),x+length+2,y+6+(charhei>>2)+height,LIGHTSHADOW1);
	rectblock(x+length+3,y+3+(charhei>>2),x+length+3+charwid,y+7+(charhei>>2)+height,SURFACE1);
	line(x+length+charwid+3,y+1,x+length+charwid+3,y+(charhei>>1)+height+9,DARKSHADOW1);
	line(x+length+charwid+4,y+1,x+length+charwid+4,y+(charhei>>1)+height+10,BLACK);
	line(0,y+(charhei>>1)+height+11,220-6,y+(charhei>>1)+height+11,DARKPROJECT1);
	line(0,y+(charhei>>1)+height+12,220-6,y+(charhei>>1)+height+12,DARKPROJECT1);  
	
	selFONT(font);
	putStr(x+charwid+1,y+(charhei>>1)+5,s,LIGHTGRAY,mag);
	putStr(x+charwid,y+(charhei>>1)+4,s,color,mag);

}
/* ��������ѡ����ɫ�任 */
/*��ڲ���: x,y����,font:���� s:�ַ��� */
/*          color:��8λΪ�任�����ɫ  ��8λΪ��ʼ��ɫ mag:���屶��*/
/*���ڲ���: �� */
/*��ע:     ��ɫ�ɸ��� ���ֹ�ͬ��ʾ ,���õ�ʱ��ע���*/
void chgNavbar(unsigned int x,unsigned int y,unsigned long font,unsigned char *s,unsigned long color,unsigned char mag)
{
	/* �ṩ��������ѡ��,Ϊ�˱�����ʾ����,���ڴ˺����д���*/
	/* ��ʾʱ�����Ҹ�����һ���ַ����,���¿����1/4�߶� */
	unsigned int charhei,charwid;				/* �ַ��ĸ߶ȺͿ�� */
	unsigned int height,length;					/* ��ʾ�ַ����Ŀ�ĳ��Ϳ�*/
	unsigned int i,j;
	unsigned long src_color,dst_color;
	
	src_color=(color&0xff);
	dst_color=(lintshr(8,color)&0xff);
	/*dst_color=((color>>8)&0xff);*/
	
	switch(font&0x0f)
	{
		case _HZ16:		
			charwid=8*mag;
			charhei=16*mag;
				break;
		case _HZ24:
			charwid=12*mag;
			charhei=24*mag;
				break;
		default:
			break;
	}
	length=((c_strlen(s)+2)*charwid);
	height=(charhei>>1)+charhei;
	for(j=y+5+(charhei>>2);j<=y+5+(charhei>>2)+height;j++)
		for(i=x;i<=x+length;i++)
			if(getpixel(i,j)==src_color)
				putpixel(i,j,dst_color);
}

/* �˵������ */
/*��ڲ���: x,y:����  s:�ַ��� color:��ɫ */
/*���ڲ���: �� */
/*��ע: �̶�Ϊ16x16������ʾ */
/*      ÿ���˵���߶�Ϊ 32*/
void	iItem(unsigned int x,unsigned int y,unsigned char *s,unsigned long color)
{
	unsigned length,height;
	
	/*Բ����ѡ��� ����̶�Ϊ16x16���� �����ַ�*/
	length=((c_strlen(s)+2)<<3);		/* ���ҹ�Ԥ�������ַ��� */
	height=16+16;										/* ���¹�Ԥ��1���ַ��� */
	rectarc(x,y,x+length,y+height,WHITE);
	selFONT(ASC0|_ASC0|HZKK|_HZ16);
	putStr(x+8,y+8,s,color,1);
}

/* �˵����ѡ����ɫ�任 */
/*��ڲ���: x,y����, s:�ַ��� */
/*          color:��8λΪ�任�����ɫ  ��8λΪ��ʼ��ɫ */
/*���ڲ���: �� */
/*��ע:     ��ɫ�ɸ��� ���ֹ�ͬ��ʾ ,���õ�ʱ��ע���*/
void iItemSel(unsigned int x,unsigned int y,unsigned char *s,unsigned long color)
{
	unsigned long src_color;
	unsigned long dst_color;
	unsigned int startx,maxx,i,j;
	startx=x+8;
	maxx=x+((c_strlen(s)+2)<<3);
	src_color=(color&0x0ff);
	dst_color=(lintshr(8,color)&0xff);
	/*dst_color=((color>>8)&0xff);*/
	for(j=y+1;j<(y+16+16);j++)
		for(i=startx;i<maxx;i++)
			if(getpixel(i,j)==src_color)
				putpixel(i,j,dst_color);
	for(j=y+1;j<(y+16+16);j++)
		for(i=startx;i>x;i--)
			if(getpixel(i,j)==src_color)
				putpixel(i,j,dst_color);
	
}
/* ������� */
/*��ڲ���: x,y:���� wstr:�������ַ� lines:׼����ʾ������*/
/*���ڲ���: ��*/
/*��ע �༭��:(x+1,y+33,x+570-25,y+height-1)*/
/*     �ַ�����ӡ��ʼ:(x+8,y+40)*/
void	iWindowOut(unsigned int x,unsigned int y,unsigned char *wstr,unsigned int lines)
{
	/*33��10�� �����ʹ��� �����10�У�*/
	/*��ȹ̶�Ϊ571*/
	unsigned int height,i,j,sx,sy;
	if(lines>10)
		height=(10*24+8+16+16);
	else
		height=(lines*24+8+16+16);
	rectangle(x,y,x+570,y+16+16,WHITE);
	rectblock(x+1,y+1,x+570-1,y+16+16-1,SURFACE1);
	selFONT(ASC0|_ASC0|HZKK|_HZ16);
	Pic_Wing(x+9,y+7,DARKPROJECT1,BLACK,0);
	Pic_Wing(x+8,y+7,DARKPROJECT1,WHITE,0);
	putStr(x+8+24,y+8,wstr,WHITE,1);
	
	rectangle(x,y+16+16,x+570,y+height,WHITE);
	rectangle(x+570-24,y+height-24,x+570,y+height,WHITE);
	rectblock(x+570-24+1,y+height-24+1,x+570-1,y+height-1,SURFACE1);
	sx=x+570-24+4;
	sy=y+height-24+4;
	for(j=16;j>0;j-=2)
	{
			for(i=0;i<j;i++)
				putpixel(sx+i,sy,WHITE);
			sx++;
			sy+=2;
	}
	rectangle(x+570-24,y+16+16,x+570,y+16+16+24,WHITE);
	rectblock(x+570-24+1,y+16+16+1,x+570-1,y+16+16+24-1,SURFACE1);
	sx=x+570-24+4;
	sy=y+16+16+24-4;
	for(j=16;j>0;j-=2)
	{
			for(i=0;i<j;i++)
				putpixel(sx+i,sy,WHITE);
			sx++;
			sy-=2;
	}
	line(x+570-24,y+16+16+24,x+570-24,y+height-24,WHITE);
	rectblock(x+1,y+33,x+570-25,y+height-1,GROUND1);
	rectblock(x+570-24+1,y+16+16+24+1,x+570-1,y+height-24-1,GROUND1);
	
	if(lines<=10)
	{
		rectblock(x+570-24+2,y+58,x+570-2,y+height-26,SURFACE1);
		/* x+570-24+2=x+548*/
		line(x+548,y+58,x+568-1,y+58,WHITE);
		line(x+568,y+58,x+568,y+height-26,DARKPROJECT1);
		line(x+567,y+height-26,x+548,y+height-26,DARKPROJECT1);
		line(x+548,y+height-27,x+548,y+59,WHITE);
	}
	else
	{
		iScrollBar(x,y,0,lines);
	}
	/*Scroll Bar: x+570-24+1+1,x+570-1-1  y+58,y+height-26*/
	//
	//line(x+570-24+2,y+58,x+570-2-1,y+58,WHITE);
	//line(x+570-24+2,y+58,x+570-24+2,y+height-86,WHITE);
	//line(x+570-2,y+58,)
	
}
/* �������Ļ��� */
/*��ڲ���; winx,winy:���ڵ���ʼ���� num_line:�к�(0~lines-1),lines:������ */
/*���ڲ���: �� */
void iScrollBar(unsigned int winx,unsigned int winy,unsigned int num_line,unsigned int lines)
{
	/* 173=10*24+8+16+16-26-58 -24+1 */
	/* �߶� 24 ���21 */
	unsigned int startx,starty;
	unsigned long dividend,divisor;			/*�����������*/
	if(lines<=10)
		return;
	if(num_line>=lines)
		return;
	startx=winx+548;
	starty=winy+58;
	dividend=lintmul(173,lintshl(10,num_line));
	/*dividend=((unsigned long)(num_line)<<10)*173;*/
	divisor=(lines-1);
	starty+=lintshr(10,lintdiv(divisor,dividend));
	/*starty+=((dividend/divisor)>>10);*/
	rectblock(winx+548,winy+58,winx+568,winy+((lines>10)?10:lines)*24+8+16+16-24-1,GROUND1);
	rectblock(startx,starty,startx+20,starty+23,SURFACE1);
	line(startx,starty,startx+20-1,starty,WHITE);
	line(startx+20,starty,startx+20,starty+23,DARKPROJECT1);
	line(startx+20-1,starty+23,startx,starty+23,DARKPROJECT1);
	line(startx,starty+22,startx,starty+1,WHITE);
	//asm mov ax,3
	//asm int 0x10
	//printf("winy=%d ",winy+58);
	//printf("starty=%d",starty);
	//getch();
}
/* ���ڱ༭������Ϣ��ʾ�Ļ��� */
/*��ڲ���: winx,winy:���ڵ���ʼ����,minline,maxline:�����ķ�Χ(��0��ʼ���е�,0~9)
            up_down:���ϻ��������»� 0:���� 1:���� */
/*���ڲ���: �� */
void	iWindowSlide(unsigned int winx,unsigned int winy,unsigned int minline,unsigned int maxline,unsigned char up_down)
{
	unsigned startx,starty,i,j;
	unsigned char buffer[538];
	if(up_down==1)
	{
		startx=winx+8;			/* ��Ե�ǰiWindow����ʼλ�� */
		starty=winy+40+(minline*24);
		
		for(j=winy+40+(maxline-1)*24+16;j>=starty;j--)
		{
			for(i=0;i<538;i++)
				buffer[i]=getpixel(startx+i,j);
			for(i=0;i<538;i++)
				putpixel(startx+i,j+24,buffer[i]);
		}
		rectblock(startx,starty,startx+537,starty+16,GROUND1);
	}
	else if(up_down==0)
	{
		for(j=winy+40+(minline+1)*24;j<=winy+40+(maxline*24)+16;j++)
		{
			for(i=0;i<538;i++)
				buffer[i]=getpixel(winx+8+i,j);
			for(i=0;i<538;i++)
				putpixel(winx+8+i,j-24,buffer[i]);
		}
		rectblock(winx+8,winy+40+(maxline*24),winx+8+537,winy+40+(maxline*24)+16,GROUND1);
	}
	
}

void Pic_Wing(unsigned int x,unsigned int y,unsigned int color_in,unsigned int color_out,unsigned char direct)
{
	unsigned int i,j;
	unsigned char buffer[18];
	unsigned char flag,firp,endp;
	
	for(i=0;i<17;i++)
	{
		if(direct==0)			//��ͨ���򣬳������չ��
		{
			for(j=0;j<8;j++) buffer[j]=(((wing[i][0])>>(7-j))&0x01);						//��ʼ��
			for(j=8;j<16;j++)buffer[j]=(((wing[i][1])>>(8+7-j))&0x01);
			buffer[16]=(((wing[i][2])>>7)&0x01);
			buffer[17]=(((wing[i][2])>>6)&0x01);
		}
		else if(direct==1)//�������չ��
		{
			buffer[0]=(((wing[i][2])>>6)&0x01);
			buffer[1]=(((wing[i][2])>>7)&0x01);
			for(j=2;j<2+8;j++) buffer[j]=(((wing[i][1])>>(j-2))&0x01);
			for(j=2+8;j<2+8+8;j++) buffer[j]=(((wing[i][0])>>(j-2-8))&0x01);
		}
		else if(direct==2)		//����£�����չ��
		{
			for(j=0;j<8;j++) buffer[j]=(((wing[16-i][0])>>(7-j))&0x01);						//��ʼ��
			for(j=8;j<16;j++)buffer[j]=(((wing[16-i][1])>>(8+7-j))&0x01);
			buffer[16]=(((wing[16-i][2])>>7)&0x01);
			buffer[17]=(((wing[16-i][2])>>6)&0x01);
		}
		else if(direct==3)//����£�����չ��
		{
			buffer[0]=(((wing[16-i][2])>>6)&0x01);
			buffer[1]=(((wing[16-i][2])>>7)&0x01);
			for(j=2;j<2+8;j++) buffer[j]=(((wing[16-i][1])>>(j-2))&0x01);
			for(j=2+8;j<2+8+8;j++) buffer[j]=(((wing[16-i][0])>>(j-2-8))&0x01);
		}
		flag=0;
		for(j=0;j<18;j++)																							//��Ѱͼ�����
		{
			if((buffer[j]==1)&&(flag==0))
			{
				flag=1;
				firp=j;
			}
			else if((buffer[j]==1)&&(flag>=1))
			{
				flag++;
				endp=j;
			}
		}
		if(endp==(firp+1))
			flag=1;
		if(flag==1)
		{
			for(j=0;j<17;j++)
				if(buffer[j]==0x01)buffer[j]=0x03;										//��Χ���ص�			
		}
		else if(flag>=1)
		{
			
			for(j=0;j<18;j++)
				if(buffer[j]==0x01)buffer[j]=0x03;										//��Χ���ص�	
			for(j=firp+1;j<endp;j++)
				if(buffer[j]==0x00)buffer[j]=0x02;										//�ڲ����ص�
		}
		for(j=0;j<18;j++)
		{
			switch(buffer[j])
			{
				case 0x02:
					putpixel(x+j,y+i,color_in);
					break;
				case 0x03:
					putpixel(x+j,y+i,color_out);
					break;
				default:
					break;
			}
		}
	}
	
}
/* ��ȡ�ַ����������ַ��ĸ���,�����������ַ� */
unsigned int	c_strlen(unsigned char *s)
{
	unsigned int	i = 0;
	while(*s++) i++;
	return	i;
}

/* ��ȡ�ַ�����ASCII��ĸ��� */
unsigned int	c_strlen_ascii(unsigned char *s)
{
	unsigned int	i = 0;
	/*while((*s++)<0xa0)i++;*/
	while(*s)
	{
		if(*s<0xa0)i++;
		s+=1;
	}		
	return	i;
}
/* �Ƚ������ַ����Ƿ���ͬ */
/*<0 str1С��str2   =0 str1����str2  >0 str1����str2 */
int c_strncmp(unsigned char *str1,unsigned char *str2,unsigned int count)
{
	int res=0;
	while(count){
		if((res=*str1-*str2++)!=0 || !(*str1++))
			break;
		count--;
	}
	return res;
}
/* ͼ�η�ʽ��printf���� */
/*��ڲ���: color:��ɫ format:�ɱ���� */
/*���ڲ���: �� */
/*��ע: �����˿ɱ������C����ʵ��,ͨ��һ���(������stdarg.h)������ȡ������ջ�Ĳ��� 
        ��ת���Ļ�������������Ŀ�����������ʵ�ֵ�ƽ̨���治��Ҫ��ô�ϸ�ļ��,
        Ϊ�˼�����,������ʵ��ԭ�����������Ĵ���
        ����,�ڳ�������������x_cur,y_cur��������,�������Ҫʵ�ֻ��еĻ�,ͨ�����ָ����ʵ��*/
void c_printf(unsigned long color, const char *format,...)
{
	/*��ǰ����ʾƽ̨�������ʾ35������,���ַ������ó���67��*/
	/*��֧�� "%d %ld %x %lx %c %C %s���ָ�ʽ"*/
	char buffer[100],buffer1[11];								/* Ԥ��������100�ַ����� */
	char ch;
	char *ptr,*ptr1;
	unsigned char flag=0;
	unsigned int len,i;
	i_va_list argptr;
	
	i_va_start(argptr,format);
	for (ptr=buffer ; *format ; ++format) 
	{
		if (*format != '%') 
		{
			*ptr++ = *format;
			continue;
		}
		++format;					/* �˵� %*/
		if((*format=='l')||(*format=='L'))
		{
			flag=1;
			++format;
		}
		switch(*format)
		{
			case 'c':
			case 'C':
				*ptr++ = (unsigned char) i_va_arg(argptr, int);
				break;
			case 's':
			case 'S':
				ptr1 = i_va_arg(argptr, char *);
				len=c_strlen(ptr1);
				for (i = 0; i < len; ++i)
					*ptr++ = *ptr1++;
					break;
			case 'x':
			case 'X':
				if(flag)
				{
					flag=0;
					_l2asc(i_va_arg(argptr, unsigned long),16,buffer1);
				}
				else
					_i2asc(i_va_arg(argptr, unsigned int),16,buffer1);
				ptr1=buffer1;
				len=c_strlen(ptr1);
				for (i = 0; i < len; ++i)
					*ptr++ = *ptr1++;
					break;
			case 'd':
			case 'D':
				if(flag)
				{
					flag=0;
					_l2asc(i_va_arg(argptr, unsigned long),10,buffer1);
				}
				else
					_i2asc(i_va_arg(argptr, unsigned int),10,buffer1);
				ptr1=buffer1;
				len=c_strlen(ptr1);
				for (i = 0; i < len; ++i)
					*ptr++ = *ptr1++;
					break;
			default:
				*ptr++=*format;
				break;
				
		}
	}
	*ptr='\0';
	i_va_end(argptr);
	if(c_strlen(buffer)>66)return;
	selFONT(ASC0|_ASC0|HZKK|_HZ16);
	putStr(x_cur,y_cur,buffer,color,1);
	x_cur+=(c_strlen(buffer)*8);
}
/* ͼ�η�ʽ��scanf���� */
/*��ڲ���: bg_color:�����ʱ��ı�����ɫ color:�ַ���ɫ format:�ɱ���� */
/*���ڲ���: �� */
/*��ע: %d %ld %x %lx %c %C %s %p  %(����)s  %(����)p ���ָ�ʽ (���ֲ���ʮ���ƣ������Ϊ9)
				���û�м�������ȱʡ��Ϊ�����Խ���12���ַ�
        ע������������Ե�ַ���͵ķ�ʽ����.����%p��ʾ��������,��ʾ��ʱ����ʾ'*'
        ������%4s %5p֮��Ĳ��������Լ��������ӵ�,Ϊ���ṩһ��ȷ����ȡ�ַ��ķ�ʽ */
void c_scanf(unsigned long bg_color,unsigned long color,const char *format,...)
{
	/*��ǰ����ʾƽ̨�������ʾ35������,���ַ������ó���71��*/
	/*��֧�� "%d %ld %x %lx %c %C %s %p  %(����)s  %(����)p ���ָ�ʽ"*/
	char buffer[100];								/* Ԥ��������100�ַ����� */
	unsigned long in_long;
	char *ptr,*ptr1;
	unsigned char flag=0;
	unsigned int code,len,i,lens=0;
	i_va_list argptr;
	
	i_va_start(argptr,format);
	selFONT(ASC0|_ASC0|HZKK|_HZ16);
	for (; *format ; ++format) 
	{
		if (*format != '%') 
		{	
			code=((unsigned int)(format[0])&0xff);	
			if(code<128)			/* Ӣ���ַ� */
				putASCII(x_cur,y_cur,*format,color,1);
			else
				{
					code=((unsigned int)(format[1])&0xff);
					code<<=8;
					code+=((unsigned int)(format[0])&0xff);
					putHZ(x_cur,y_cur,code,color,1);
					x_cur+=8;
					++format;
				}
			x_cur+=8;
			continue;
		}
		++format;					/* �˵� %*/
		if((*format=='l')||(*format=='L'))
		{
			flag=1;
			++format;
		}
		if((*format>'0')&&(*format<'9'))
		{
			lens=*format-'0';
			++format;
		}
		switch(*format)
		{
			case 'c':
			case 'C':
				ptr = i_va_arg(argptr, char *);
				getKbin(bg_color,color,0x0100,buffer);
				ptr[0]=buffer[0];
				break;
			case 's':
			case 'S':
				ptr = i_va_arg(argptr, char *);
				if(lens!=0)
					getKbin(bg_color,color,(lens<<8),buffer);
				else
					getKbin(bg_color,color,0x0c00,buffer);				/* �������12���ַ� */
				ptr1=buffer;
				len=c_strlen(ptr1);
				for (i = 0; i < len; ++i)
					*ptr++ = *ptr1++;
				*ptr='\0';
				lens=0;
					break;
			case 'x':
			case 'X':
				if(flag)
				{
					flag=0;
					ptr = i_va_arg(argptr, char *);
					in_long=getKbin(bg_color,color,0x0810,buffer);
					ptr[0]=(in_long&0xff);
					ptr[1]=(lintshr(8,in_long)&0xff);
					ptr[2]=(lintshr(16,in_long)&0xff);
					ptr[3]=(lintshr(24,in_long)&0xff);
					/*
					ptr[1]=((in_long>>8)&0xff);
					ptr[2]=((in_long>>16)&0xff);
					ptr[3]=((in_long>>24)&0xff);*/
				}
				else
				{
					ptr = i_va_arg(argptr, char *);
					in_long=getKbin(bg_color,color,0x0410,buffer);
					ptr[0]=(in_long&0xff);
					ptr[1]=(lintshr(8,in_long)&0xff);
					/*ptr[1]=((in_long>>8)&0xff);*/
				}
					break;
			case 'd':
			case 'D':
				if(flag)
				{
					flag=0;
					ptr = i_va_arg(argptr, char *);
					in_long=getKbin(bg_color,color,0x0a0a,buffer);
					ptr[0]=(in_long&0xff);
					ptr[1]=(lintshr(8,in_long)&0xff);
					ptr[2]=(lintshr(16,in_long)&0xff);
					ptr[3]=(lintshr(24,in_long)&0xff);
					/*
					ptr[1]=((in_long>>8)&0xff);
					ptr[2]=((in_long>>16)&0xff);
					ptr[3]=((in_long>>24)&0xff);*/
				}
				else
				{
					ptr = i_va_arg(argptr, char *);
					in_long=getKbin(bg_color,color,0x050a,buffer);
					ptr[0]=(in_long&0xff);
					ptr[1]=(lintshr(8,in_long)&0xff);
					/*ptr[1]=((in_long>>8)&0xff);*/
				}
					break;
			case 'p':
			case 'P':
				ptr = i_va_arg(argptr, char *);
				if(lens!=0)
					getKbin(bg_color,color,((lens<<8)|0x01),buffer);
				else
					getKbin(bg_color,color,0x0C01,buffer);				/* �������12���ַ� */
				ptr1=buffer;
				len=c_strlen(ptr1);
				for (i = 0; i < len; ++i)
					*ptr++ = *ptr1++;
				*ptr='\0';
				lens=0;
					break;
			default:
				putASCII(x_cur,y_cur,*format,color,1);
				x_cur+=8;
				break;
				
		}
	}
}
/* �༭�����е�ѡ����ɫ�任 */
/*��ڲ���: x,y����,           */
/*          color:��8λΪ�任�����ɫ  ��8λΪ��ʼ��ɫ mag:���屶��*/
/*���ڲ���: �� */
/*��ע:     ��ɫ�ɸ��� ���ֹ�ͬ��ʾ ,���õ�ʱ��ע���*/
void chgLine(unsigned int x,unsigned int y,unsigned int num_line,unsigned long color)
{

	unsigned int height,length;					/* ��ʾ�ַ����Ŀ�ĳ��Ϳ�*/
	unsigned int i,j;
	unsigned long src_color,dst_color;
	
	if(num_line>9)return;										/* ���ֻ��ʾ10�� */
	src_color=(color&0xff);
	dst_color=(lintshr(8,color)&0xff);
	/*dst_color=((color>>8)&0xff);*/
	

	length=(33<<4);						/* һ�е���󳤶� */
	height=4+16+4;						/* ѡ������ʾ��� */
	
	for(j=y+40+num_line*24-4;j<y+40+num_line*24-4+height;j++)
		for(i=x+8;i<x+8+length;i++)
			if(getpixel(i,j)==src_color)
				putpixel(i,j,dst_color);
}
/* ��������ת��ΪASCII�ַ��� */
/*��ڲ���: num:��Ҫת������ base:����,2~16Ϊ��Ч���� out:ת������ַ��� */
/*���ڲ���: �� */
void _i2asc(unsigned int num,int base,unsigned char *out)
{
	/*const unsigned char *digits="0123456789ABCDEF";*/
	unsigned char outstr[17];
	unsigned int i,j,shang;
	
	if(num==0)
	{
		out[0]='0';
		out[1]='\0';
		return;
	}
	shang=num;
	i=0;
	outstr[0]='\0';
	if(base!=16)
	{
		while(shang!=0)
		{
			asm xor dx,dx
			asm mov ax,shang
			asm div base
			asm mov shang,ax											/* �� */
			outstr[(++i)]=digits[_DX];

		}
	}
	else
	{
		while(shang!=0)
		{
			outstr[(++i)]=digits[(shang&0x0f)];
			shang=lintshr(4,shang);
			/*shang>>=4;*/
		}
	}
	for(j=0;j<=i;j++)
		out[j]=outstr[i-j];
}
/* ����������ת��ΪASCII�ַ��� */
/*��ڲ���: num:��Ҫת������ base:����,2~16Ϊ��Ч���� out:ת������ַ��� */
/*���ڲ���: �� */
void _l2asc(unsigned long num,int base,unsigned char *out)
{
	/*const unsigned char *digits="0123456789ABCDEF";*/
	unsigned char outstr[33];
	unsigned int i,j;
	unsigned long shang;
	
	if(num==0)
	{
		out[0]='0';
		out[1]='\0';
		return;
	}
	shang=num;
	i=0;
	outstr[0]='\0';
	if(base!=16)
	{
		while(shang!=0)
		{
			outstr[(++i)]=digits[lintmod(base,shang)];
			shang=lintdiv(base,shang);
			/*outstr[(++i)]=digits[shang%base];
			shang=(shang/base);									/* �� */	
		}
	}
	else
	{
		while(shang!=0)
		{
			outstr[(++i)]=digits[(shang&0x0f)];
			shang=lintshr(4,shang);
			/*shang>>=4;*/
		}
	}
	for(j=0;j<=i;j++)
		out[j]=outstr[i-j];	
}

/* ��ȡ�û��ĸ������� */
/*��ڲ���: ����Ϊ:����ɫ �ַ���ɫ �ɽ��ܵ��ַ����������� ����ַ���*/            
/*���ڲ���: �����ȡ���������򷵻���ֵ��������ַ��򷵻�0 */
unsigned long getKbin(unsigned long bg_color,unsigned long color,unsigned int num_base,unsigned char *out)
{
	/*num:��ȡ���ַ�����,base:0��ȡ�ַ��� 1��ȡ�����ַ���  10��ȡ10������ 16��ȡ16������*/	
	unsigned char buffer[32];
	unsigned int number,base,cur_num,i,num_chars;
	unsigned char chin=0;
	unsigned long retval=0;
	number=((num_base>>8)&0xff);
	base=(num_base&0xff);
	selFONT(ASC0|_ASC0|HZKK|_HZ16);
	rectblock(x_cur,y_cur,x_cur+number*8,y_cur+16-1,bg_color);
	
	InitIntRes(CURSOR);			/* �򿪹�����ʾ */
	if(base==16)base=22;
	if(base>1)
	{
		cur_num=0;
		while(chin!='\r')					/*enter �س�*/
		{
			asm xor ax,ax
			asm int 0x16
			asm mov chin,al
			if((chin==0x08)&&cur_num>0)				/*1 ���˼��Ĵ��� */
			{
				--cur_num;
				buffer[cur_num]=0;
				x_cur-=8;						/* ������ */
				rectblock(x_cur,y_cur,x_cur+8,y_cur+16-1,bg_color);		/* ������ǰ��ʾ */	
			}
			/*2 �ж��Ƿ��Ƿ���Ҫ�����,��������������ȡ��һ�ַ� */
			for(i=0;i<=base;i++)
				if(digits[i]==chin)			/* ����Ҫ�� */
				{
					/*2 �ж��Ƿ��Ѿ��������� */
					if(cur_num==(number))
					{
						buffer[cur_num-1]=((i>0xf)?(i-6):i);
						rectblock(x_cur-8,y_cur,x_cur-1,y_cur+16-1,bg_color);			/*�����ǰ��ʾ*/
						putASCII(x_cur-8,y_cur,chin,color,1);						/* ��ʾ�õ������ַ� */
					}
					else
					{
						buffer[cur_num]=((i>0xf)?(i-6):i);
						++cur_num;
						x_cur+=8;
						rectblock(x_cur-8,y_cur,x_cur,y_cur+16-1,bg_color);
						putASCII(x_cur-8,y_cur,chin,color,1);					/*��ʾ��ǰ�ַ�*/
					}
				}
			
		}
		/* ����Ϊֹ,buffer�а����˵õ�������,cur_numΪ�õ��ĸ���*/
		if(base==22)base=16;
		for(i=0;i<cur_num;i++)
			retval=lintmul(retval,base)+(unsigned long)(buffer[i]);
		/*	retval=retval*base+buffer[i];	*/
	}
	else
	{
		cur_num=0;
		num_chars=c_strlen(digits);
		while(chin!='\r')
		{
			asm xor ax,ax
			asm int 0x16
			asm mov chin,al
			if((chin==0x08)&&(cur_num>0))				/*1 ���˼��Ĵ��� */
			{
				--cur_num;
				out[cur_num]='\0';
				/* ������ǰ��ʾ	*/
				x_cur-=8;								/* ������ */
				rectblock(x_cur,y_cur,x_cur+8,y_cur+16-1,bg_color);
									
			}
			for(i=0;i<num_chars;i++)
				if(chin==digits[i])
				{
					/*2 �ж��Ƿ��Ѿ��������� */
					if(cur_num==(number))
					{
						out[cur_num-1]=chin;
						/*�����ǰ��ʾ*/
						rectblock(x_cur-8,y_cur,x_cur-1,y_cur+16-1,bg_color);
						
						if(base==0)
							putASCII(x_cur-8,y_cur,chin,color,1);	/*��ʾ�õ������ַ�*/
						else
							putASCII(x_cur-8,y_cur,'*',color,1);
						
					}
					else
					{
						out[cur_num]=chin;
						++cur_num;
						x_cur+=8;
						rectblock(x_cur-8,y_cur,x_cur,y_cur+16-1,bg_color);
						if(base==0)
							putASCII(x_cur-8,y_cur,chin,color,1);	/*��ʾ��ǰ�ַ�*/
						else
							putASCII(x_cur-8,y_cur,'*',color,1);
						
					}
				}
		}
		out[cur_num]='\0';
	}
	RelIntRes(CURSOR);
	return retval;
}
/* ��괦���ϵ�к��� */
void setCur(unsigned int x,unsigned int y)
{
	x_cur=x;
	y_cur=y;
}
unsigned int getXCur(void)
{
	return x_cur;
}
unsigned int getYCur(void)
{
	return y_cur;
}

/*      ASCII��ת��Ϊʮ��������                 */
/*��ڲ�����  ��Ҫת����ascii                   */      
/*���ڲ�����  AL=ת���ַ�                       */
/*     
unsigned char asc2hex (unsigned char ch)
{
	asm{
		mov	al,ch
		or  al,20h            /*��дתСд*//*
 		sub al,'0'
 		cmp al,9
 		jbe  athover          
 		sub al,'a'-'0'-10
	}
	athover: 
	return(_AL);
}*/

/* ʮ��������ת��Ϊ��Ӧ��ASCII��         */
/*��ڲ�����  ch:Ҫת��������(0~f)       */
/*���ڲ�����  ��Ӧ��ASCII��              */
/*unsigned char hex2asc(unsigned char ch) 
{
	asm{
		mov al,ch
		and al,0fh
    add al,30h
    cmp al,39h
    jbe htoasc1
    add al,7
	}
	htoasc1:
	return(_AL);
}  
*/