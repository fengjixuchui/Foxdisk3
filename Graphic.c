/* Graphic.c:�ṩӲ���޹ص�ͼ�κ��� */

#include "Vesa.h"
#include "Graphic.h"
#include "iMath.h"

void setBackGround(unsigned long color);
unsigned int initDisplay(unsigned int mode,SvgaDAC far *palette);
void	line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned long color);
void	rectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned long color);
void	rectblock(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned long color);
void	circle(unsigned int centerx,unsigned int centery,unsigned int radius,unsigned long color);
void cirblock(unsigned int cenx,unsigned int ceny,unsigned int radius,unsigned long color);
void	iDelay(unsigned long millsec);
void SvgaSmoothOn(SvgaDAC *pal);
void SvgaSmoothOff(SvgaDAC *pal);

#if ANOTHER_AR
void	cirx(unsigned int centerx,unsigned int centery,unsigned int radius,unsigned long color,unsigned char quadrant);
void	rectarc(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned long color);
#endif

unsigned int initDisplay(unsigned int mode,SvgaDAC far *palette)
{
	unsigned int err=0;
	err=initGraphics(mode);
	if(err)
		return err;
	if(modeinfo.BitsPerPixel==8)					/* 256ɫģʽ */
		setpalette(0,256,palette);					/* ��ʼ����ɫ�Ĵ��� */
	return err;
}

void setBackGround(unsigned long color)
{
	unsigned int x,y;
	
	for(y=0;y<yres;y++)								/*ע��,�����Ժ��򻭵�,������ʾ��ԭ��*/
		for(x=0;x<xres;x++)
			putpixel(x,y,color);
}
/* ���뼶��ʱ*/
void	iDelay(unsigned long millsec)
{
	unsigned long microsec;
	unsigned int high,low;
	
	microsec=lintmul(millsec,1000);
	/*microsec=millsec*1000;*/
	high=(unsigned int)((microsec>>16)&0xffff);
	low=(unsigned int)(microsec&0xffff);
	
	asm{
		mov	dx,low
		mov	cx,high
		mov	ah,86h
		int 15h	
	}
}

/* ͼ���� */
void SvgaSmoothOn(SvgaDAC *pal)
{
	SvgaDAC temp[256];
	int i,j;
	unsigned char maxdac=0;
	for(i=0;i<256;i++)				/* ��ʼ�� */
	{
		temp[i].b=0;
		temp[i].g=0;
		temp[i].r=0;
		if(pal[i].b>maxdac)maxdac=pal[i].b;
		if(pal[i].g>maxdac)maxdac=pal[i].g;
		if(pal[i].r>maxdac)maxdac=pal[i].r;
	}
	
	for(j=0;j<64;j++)
	{
		for(i=0;i<256;i++)
		{
			if(pal[i].b>=maxdac) ++temp[i].b;
			if(pal[i].g>=maxdac) ++temp[i].g;
			if(pal[i].r>=maxdac) ++temp[i].r;
		}
		iDelay(8);
		setpalette(0,256,temp);
		if(!--maxdac)break;
	}
}
/* ͼ�󵭳� */
void SvgaSmoothOff(SvgaDAC *pal)
{
	SvgaDAC temp[256];
	int i,j;
	
	for(i=0;i<256;i++)
	{
		temp[i].b=pal[i].b;
		temp[i].g=pal[i].g;
		temp[i].r=pal[i].r;
	}
	for(j=0;j<64;j++)
	{
		for(i=0;i<256;i++)
		{
			if(temp[i].b>0)--temp[i].b;
			if(temp[i].g>0)--temp[i].g;
			if(temp[i].r>0)--temp[i].r;
		}
		iDelay(10);
		setpalette(0,256,temp);
	}
}
/*======================����ĺ���Ϊ��������Ļ�ͼ����,��Ӳ�����������======================*/
/*    					���ߺ���    			 */
/*��ڲ���: (x1,y1)->(x2,y2),color */
/*���ڲ���: ��                     */
void	line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned long color)
{
	/*ʹ��Bresenham�㷨*/
	signed int d,dx,dy,dx2,dy2,dxy;
	signed int xinc,yinc;

	
	/* ����׼�� */
	dx=(signed int)((x2>x1)?(x2-x1):(x1-x2));
	dx2=dx<<1;
	dy=(signed int)((y2>y1)?(y2-y1):(y1-y2));
	dy2=dy<<1;
	xinc=(x2>x1)?1:((x2==x1)?0:(-1));
	yinc=(y2>y1)?1:((y2==y1)?0:(-1));
	putpixel(x1,y1,color);
	if(dx>=dy){
		d=dy2-dx;
		dxy=dy2-dx2;
		while(dx--){
			if(d<=0)d+=dy2;
			else{
				d+=dxy;
				y1+=yinc;
			}
			if(((x1+xinc)<=maxX)&&(y1<=maxY))			/* �Գ�����Ļ��Χ�ĵ㲻����ʾ */
				putpixel(x1+=xinc,y1,color);
		}
	}
	else{
		d=dx2-dy;
		dxy=dx2-dy2;
		while(dy--){
			if(d<=0)d+=dx2;
			else{
				d+=dxy;
				x1+=xinc;
			}
			if((x1<=maxX)&&((y1+yinc)<=maxY))			/* �Գ�����Ļ��Χ�ĵ㲻����ʾ */
				putpixel(x1,y1+=yinc,color);
		}
	}
}
/*    					���κ���    			 */
/*��ڲ���: (x1,y1)->(x2,y2),color */
/*���ڲ���: ��                     */
void	rectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned long color)
{
	unsigned int left,top,right,bottom;
	unsigned int i;
	/* ����λ��*/
	left=(x1<x2)?x1:x2;
	right=(x1>x2)?x1:x2;
	top=(y1<y2)?y1:y2;
	bottom=(y1>y2)?y1:y2;
	right=(right<maxX)?right:maxX;
	bottom=(bottom<maxY)?bottom:maxY;				/* ����������Ļ�ĵ� */
	
	for(i=left;i<right;i++)putpixel(i,top,color);
	for(i=top;i<bottom;i++)putpixel(right,i,color);
	for(i=right;i>left;i--)putpixel(i,bottom,color);
	for(i=bottom;i>top;i--)putpixel(left,i,color);
}
/*    				���ο麯��    			 */
/*��ڲ���: (x1,y1)->(x2,y2),color */
/*���ڲ���: ��                     */
void	rectblock(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned long color)
{
	unsigned int left,top,right,bottom;
	unsigned int x,y;
	/* ����λ��*/
	left=(x1<x2)?x1:x2;
	right=(x1>x2)?x1:x2;
	top=(y1<y2)?y1:y2;
	bottom=(y1>y2)?y1:y2;
	right=(right<maxX)?right:maxX;
	bottom=(bottom<maxY)?bottom:maxY;				/* ����������Ļ�ĵ� */
	
	for(y=top;y<=bottom;y++)
		for(x=left;x<=right;x++)
			putpixel(x,y,color);
	
}
/*    				��Բ����      			 */
/*��ڲ���: (centerx,centery)Բ��  */
/*          radius�뾶 color��ɫ   */
/*���ڲ���: ��                     */
/*˵��: ����û���ж��Ƿ񳬳���Ļ��Χ*/
void	circle(unsigned int centerx,unsigned int centery,unsigned int radius,unsigned long color)
{
	signed int x,y,d;
	if(!radius)return;
	y=(signed int)(radius);
	d=3-((signed int)(radius)<<1);
	
	/*��xor_putģʽ��,�������µ�ѭ������x=0��ʱ��ִ����ָͬ������*/
	/*��ʹ�ô����������һ��,�����޷��ﵽҪ��*/
	putpixel(centerx,centery+y,color);				/*0,-1*/
	putpixel(centerx,centery-y,color);				/*0,1*/
	putpixel(centerx+y,centery,color);
	putpixel(centerx-y,centery,color);
	
	for(x=1;x<y;x++)
	{
		putpixel(centerx+x,centery+y,color);
		putpixel(centerx+x,centery-y,color);
		putpixel(centerx-x,centery-y,color);
		putpixel(centerx-x,centery+y,color);
		putpixel(centerx+y,centery+x,color);
		putpixel(centerx+y,centery-x,color);
		putpixel(centerx-y,centery-x,color);
		putpixel(centerx-y,centery+x,color);
		if(d<0)d+=((x<<2)+6);						/*ȡ���ҷ���Ϊ԰�ϵ�*/
		else d+=(((x-(y--))<<2)+10);				/*ȡ���·���Ϊ԰�ϵ�*/
	}
	/* x=y ʱ��Ҳ������ظ�д�����*/
	putpixel(centerx+y,centery+y,color);
	putpixel(centerx+y,centery-y,color);
	putpixel(centerx-y,centery-y,color);
	putpixel(centerx-y,centery+y,color);
	
}
/*    				��Բ�麯��      		 */
/*��ڲ���: (centerx,centery)Բ��  */
/*          radius�뾶 color��ɫ   */
/*���ڲ���: ��                     */
/*˵��: ����û���ж��Ƿ񳬳���Ļ��Χ*/
/*�㷨����: ��YֵΪ����,x����С��ʼѰ��,����ͬ������ɫ���־,��*/
/*          ���ֵ��ʼѰ��,����ͬ������ɫ���־.����λ�����*/
void cirblock(unsigned int cenx,unsigned int ceny,unsigned int radius,unsigned long color)
{
	unsigned int minx,maxx,maxy;
	unsigned int lx,rx,x,y,i;
	
	
	circle(cenx,ceny,radius,color);
	minx=cenx-radius;
	maxx=cenx+radius;
	y=ceny-radius+1;				/* ȥ����һ�еĴ��� */
	maxy=ceny+radius;
	
	while(y<maxy)
	{
		for(x=minx;x<maxx;x++)
		{
			if(getpixel(x,y)==color)
			{
				lx=x;
				break;
			}
		}
		for(x=maxx;x>minx;x--)
		{
			if(getpixel(x,y)==color)
			{
				rx=x;
				break;
			}
		}
		for(i=lx+1;i<rx;i++)			/*ע�����ģʽ�µ����*/
			putpixel(i,y,color);
		y++;
	}
	
}
#if ANOTHER_AR
/*�㷨��James D.Foley�����������ͼ��ѧԭ��ʵ�� C����������*/
/*������ʹ�������Բ�ĺ���,�����ݸ���дģʽ*/

/* �����������޵�Բ�� */
void	cirx(unsigned int centerx,unsigned int centery,unsigned int radius,unsigned long color,unsigned char quadrant)
{
	signed int x,y,d;
	if((quadrant==0)||(quadrant>4))return;			/*1~4����*/
	if(!radius)return;
	y=(signed int)(radius);
	d=3-((signed int)(radius)<<1);
	/*printf("cirx1\n");
	printf("x0=%d,y0=%d,rad=%d,quadrant=%d\n",centerx,centery,radius,quadrant);*/
	/*Ϊ���ݸ���дģʽ����*/
	/*x=0ʱ���׷��*/
	switch(quadrant)
	{
		case	1:
				putpixel(centerx+radius,centery,color);		
			break;
		case 2:
				/*printf("2 x0,y0-rad\n");*/
				putpixel(centerx,centery-radius,color);
				/*printf("2 x0-rad,y0\n");*/
				putpixel(centerx-radius,centery,color);
				/*printf("2 over\n");*/
			break;
		case 3:
				putpixel(centerx,centery+radius,color);
			break;
		default:
			break;
	}
	/*printf("cirx2\n");*/
	for(x=1;x<y;x++)
	{
		switch(quadrant)
		{
			case	1:
					putpixel(centerx+x,centery-y,color);
					putpixel(centerx+y,centery-x,color);		
				break;
			case 2:
					putpixel(centerx-x,centery-y,color);
					putpixel(centerx-y,centery-x,color);
				break;
			case 3:
					putpixel(centerx-y,centery+x,color);	
					putpixel(centerx-x,centery+y,color);
				break;
			case 4:
					putpixel(centerx+x,centery+y,color);
					putpixel(centerx+y,centery+x,color);
				break;
		}
		if(d<0)d+=((x<<2)+6);						/*ȡ���ҷ���Ϊ԰�ϵ�*/
		else d+=(((x-y--)<<2)+10);				/*ȡ���·���Ϊ԰�ϵ�*/
	}
	/* x=yʱ��׷�� */
		/*printf("cirx3\n");*/

	switch(quadrant)
	{
		case	1:
				putpixel(centerx+y,centery-y,color);	
			break;
		case 2:
				putpixel(centerx-y,centery-y,color);
			break;
		case 3:
				putpixel(centerx-y,centery+y,color);	
			break;
		case 4:
				putpixel(centerx+y,centery+y,color);
			break;
	}
}
/* �ĸ���ΪԲ���ľ��� */
void	rectarc(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned long color)
{
	unsigned int left,top,right,bottom;
	unsigned int x0,y0,x,y,radius,minx,miny,maxx,maxy;
	/* ����λ��*/
	minx=(x1<x2)?x1:x2;
	maxx=(x1>x2)?x1:x2;
	miny=(y1<y2)?y1:y2;
	maxy=(y1>y2)?y1:y2;
	x=((maxx-minx)>>3);
	y=((maxy-miny)>>3);
	radius=(x<y)?x:y;
	if(radius<3)return;
	
	left=minx+radius;
	right=maxx-radius;
	top=miny+radius;
	bottom=maxy-radius;
	/*������ߺ͵�һ����Բ*/
	
	y=miny;
	for(x=left+1;x<=right;x++)
		putpixel(x,y,color);
	x0=right;
	y0=top;
	cirx(x0,y0,radius,color,1);
	/*�ұߵ��ߺ͵�������Բ*/
	x=maxx;
	for(y=top+1;y<=bottom;y++)
		putpixel(x,y,color);
	x0=right;
	y0=bottom;
	cirx(x0,y0,radius,color,4);
	/*�±ߵ��ߺ͵�������Բ*/
	y=maxy;
	/*
	for(x=left+1;x<=right;x++)
		putpixel(x,y,color);*/
	for(x=right;x>=left+1;x--)
		putpixel(x,y,color);
	x0=left;
	y0=bottom;
	cirx(x0,y0,radius,color,3);
	/*��ߵ��ߺ͵ڶ�����Բ*/
	x=minx;
	/*
	for(y=top+1;y<=bottom;y++)
		putpixel(x,y,color);*/
	for(y=bottom;y>=top+1;y--)
		putpixel(x,y,color);
	x0=left;
	y0=top;
	cirx(x0,y0,radius,color,2);
}




#endif