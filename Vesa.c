/*luobing 2008-10-13 ��ʼ��д*/
#include "Vesa.h"
#include "Global.h"
#include "iMath.h"

signed int getVBEInfo(VBEInfo far *vbeinfo);
signed int getModeInfo(unsigned int mode,ModeInfoBlock far *modeinfo);
void setmode(unsigned int mode);
unsigned int getVBEMode(void);
void setbankA(unsigned int bank);
void setbankAB(unsigned int bank);
unsigned int SetLogicWidth(unsigned int pixels);
signed intSetShowBegin(unsigned int x,unsigned int y);
signed int setpaletteVBE(unsigned int start,unsigned int num,SvgaDAC far *palette);
signed int getpaletteVBE(unsigned int start,unsigned int num,SvgaDAC far *palette);
signed int setpaletteVGA(unsigned int start,unsigned int num,SvgaDAC far *palette);
signed int getpaletteVGA(unsigned int start,unsigned int num,SvgaDAC far *palette);
void	SetWriteMode(unsigned int writemode);
void	putpixel256(unsigned int x,unsigned int y,unsigned long color);
unsigned long	getpixel256(unsigned int x,unsigned int y);

unsigned int initGraphics(unsigned int mode);


/* 		���û���ʱ����Ҫ������ģʽ 	*/
/*��ڲ���: writemode:дģʽ      */
/*���ڲ���: ��										*/
/*˵��������дģʽ						    */
unsigned int initGraphics(unsigned int mode)
{
	unsigned int err=0;
	unsigned long i,j;
	
	if(mode<0x100)																	/*1 �ж���ڲ���*/
	{
		err|=INVLIADIN;
		return	err;
	}
	if(!getVBEInfo(&vbeinfo))												/*2 ��ȡ VBE��Ϣ */
	{
		err|=NOTSUPPORTVBE;
		return err;
	}
	if(!getModeInfo(mode,&modeinfo))								/*3 ��ȡ��ǰģʽ����Ϣ */
	{
		err|=NOTSUPPORTMODE;
		return err;
	}
	if(!(modeinfo.ModeAttr&0x01))										/*4 �ж���ʾ�ڴ��Ƿ��㹻 */
	{
		err|=LIMITVMEM;
		return err;
	}
	oldMode=getVBEMode();														/*5 ���浱ǰ��ʾģʽ,�˳�ʱ��ԭ */
	setmode(mode);																	/*6 ����SVGAģʽ */
	xres=modeinfo.XRes;															/*7 ��ʼ��ȫ�ֱ���*/
	yres=modeinfo.YRes;
	bankSwitch=modeinfo.BankFunc;
	bytesperline=modeinfo.BytesPerScanline;
	bytesperpixel=((modeinfo.BitsPerPixel+7)>>3);
	bankShift=0;
	while ((unsigned)(64 >> bankShift) != modeinfo.WinGran)
		bankShift++;
	curBank=0;
	
	j=(unsigned long)(vbeinfo.VRAMMemory<<16);
	i=lintmul((unsigned long)bytesperline,(unsigned long)modeinfo.NumberOfPlanes);
	i=lintmul(i,(unsigned long)yres);
	maxpage=lintdiv(i,j);
	//maxpage=((unsigned long)(vbeinfo.VRAMMemory<<16))/((unsigned long)bytesperline*modeinfo.NumberOfPlanes*yres);
	
	VRAMStartX=VRAMStartY=0;
	if(modeinfo.WinBAttr&0x01)
		setbank=setbankAB;
	else  setbank=setbankA;
	
	if(vbeinfo.VBEVersion>=0x0200)
		setpalette=setpaletteVBE;
	else
		setpalette=setpaletteVGA;	
	getpalette=getpaletteVGA;							/*see vbe3.0 4f09 01���ܷ�ǿ����ִ�е�,������Ҫʹ���������*/
	
	switch(modeinfo.BitsPerPixel)
	{
		case 8:
			putpixel=putpixel256;
			getpixel=getpixel256;
				break;
		/* ������ʾ�ڴ˼������ ---luobing */
		default:
			break;
	}
	screenPtr=(char far *)( ((unsigned long)0xA000)<<16 | 0);
	SetWriteMode(PUT);
	return err;
}

/* 		���û���ʱ����Ҫ������ģʽ 	*/
/*��ڲ���: writemode:дģʽ      */
/*���ڲ���: ��										*/
/*˵��������дģʽ						    */
void	SetWriteMode(unsigned int writemode)
{
	WriteMode=writemode;
	return;
}
/*          256ɫд�㺯��           */
/*��ڲ���: x,y����  color:��ɫ���� */
/*���ڲ���: ��											*/
/*˵����														*/
void	putpixel256(unsigned int x,unsigned int y,unsigned long color)
{
	asm{
		les	di,screenPtr												/*es:di ָ��ǰλ��*/
		mov	ax,y
	#ifdef MULTI_PAGE
		add	ax,VRAMStartY
	#endif
		mov	bx,x
	#ifdef MULTI_PAGE	
		add	bx,VRAMStartX												/*Ϊ��ҳ����ʾ��д*/
	#endif
		mul	word ptr	bytesperline
		add	ax,bx
		adc	dx,0
		add	di,ax
		/*��ǰ�����dx��������Ҫ�任��ҳ��*/ 
		cmp	dx,curBank
		je	short	_PUT_
		mov	curBank,dx
	}
	setbank(_DX);														/*��ҳ��δ��ʱ���������ҳ��任����,��ԭ�����Ĵ���*/
	_PUT_:asm{
		mov	al,byte ptr color
		mov	bx,WriteMode											/*����дģʽ*/
		cmp	bx,0
		jne	short XORPUT
		mov	ES:[di],al
		jmp	short	exit
	}
	XORPUT:asm{
		cmp	bx,1
		jne	short ANDPUT
		xor	ES:[di],al
		jmp	short	exit
	}
	ANDPUT:asm{
		cmp	bx,2
		jne	short ORPUT
		and	ES:[di],al
		jmp	short exit
	}
	ORPUT:asm or ES:[di],al
	exit:
}
/*          256ɫȡ�㺯��           */
/*��ڲ���: x,y����  							  */
/*���ڲ���: ��ɫ����								*/
/*˵����														*/
unsigned long	getpixel256(unsigned int x,unsigned int y)
{
	asm{
		les	di,screenPtr
		mov	ax,y
		add	ax,VRAMStartY
		mov	bx,x
		add	bx,VRAMStartX
		mul	word ptr bytesperline
		add	ax,bx
		adc	dx,0
		add	di,ax
		/*��ǰ�����dx��������Ҫ�任��ҳ��*/ 
		cmp	dx,curBank
		je	short GET
		mov	curBank,dx
	}
	setbank(_DX);														/*��ҳ��δ��ʱ���������ҳ��任����,��ԭ�����Ĵ���*/
	GET:
	asm mov	al,ES:[di]
	return(_AL);
}
/*==========================����Ϊ��Ӳ����ϲ������������(int 10h)=====================================*/
/*=====��ѯVESA Super VGA��Ϣ=====*/
/*��ڲ���: ��                    */
/*���ڲ���: 1:���óɹ�  0:����ʧ��*/
/*˵��������int10h  4f00h ����    */
signed int getVBEInfo(VBEInfo far *vbeinfo)
{
  asm{
    les di,vbeinfo        /*vbeinfo�Ķ�ַ��es,ƫ����di*/      
    /*mov dx,seg vbeinfo*/
    /*mov ES,dx*/
    /*lea di,vbeinfo*/
    mov ax,4f00h
    int 10h
  }
  return(_AX==0x4f);
}

/*===== ��ѯ�ض���ʾģʽ��Ϣ =====*/
/*��ڲ���: ��ʾģʽ              */
/*���ڲ���: 1:���óɹ�  0:����ʧ��*/
/*˵��������int10h  4f01h ����    */
signed int getModeInfo(unsigned int mode,ModeInfoBlock far *modeinfo)
{
  asm{
    /*
    mov dx,seg modeinfo
    mov ES,dx
    lea di,modeinfo */
    les di,modeinfo
    mov cx,mode
    mov ax,4f01h
    int 10h
  }
  return(_AX==0x4f);
}

/*=====  ����ѡ������ʾģʽ  =====*/
/*��ڲ���: ��ʾģʽ              */
/*���ڲ���: ��                    */
/*˵����    ��                    */
void setmode(unsigned int mode)
{
  asm{
    cmp mode,100h
    jae vesamode
    /*�������vesaģʽ,��ʹ��int 10h��0�Ź���*/
    xor ax,ax
    mov al,byte ptr mode
    int 10h
    jmp short end
  }
  vesamode:asm{
    /*�����vesaģʽ,��ʹ��int 10h��4f02h����*/
    mov ax,4f02h
    mov bx,mode
    int 10h
  }
  end:
}

/*===== ��ȡVesa SVGA��ʾģʽ=====*/
/*��ڲ���: ��                    */
/*���ڲ���: ��ʾģʽ              */
/*˵����													*/
unsigned int getVBEMode(void)
{
  asm{
    mov ax,4f03h
    int 10h
    and bx,3fffh
  }
  return(_BX);
}
/*=====   �����µĶ�дҳ��   =====*/
/*��ڲ���: bank                  */
/*���ڲ���: ��                    */
/*˵����ͨ����ʾģʽ��Ϣ�������ĸ�����
ȫ�ֱ���curBank,bankShift,bankSwitch()�����Ѿ�׼����*/
void setbankA(unsigned int bank)
{
	/*
  if(curBank==bank)return;
  curBank=bank;*/
  asm{
    push  ds
    mov cx,bankShift
    mov dx,bank
    shl dx,cl
    xor bx,bx
  }
  #ifdef DIRECT_BANKING
    bankSwitch();
  #else
  asm{
    mov ax,4f05h
    int 10h
  }
  #endif
  asm pop ds
}
void setbankAB(unsigned int bank)
{
	/*
  if(curBank==bank)return;
  curBank=bank;*/
  asm{
    push  ds
    mov cx,bankShift
    mov dx,bank
    shl dx,cl
  }
  #ifdef DIRECT_BANKING
    asm push dx
    asm xor bx,bx
    bankSwitch();
    asm pop dx
    asm mov bx,1
    bankSwitch();
  #else
  asm{
    push ds
    xor bx,bx
    mov ax,4f05h
    int 10h
    pop ds
    mov bx,1
    mov ax,4f05h
    int 10h
  }
  #endif
  asm pop ds
}
/*=====     �����߼�����     =====*/
/*��ڲ���: pixels:ÿ�е�������Ŀ */
/*���ڲ���: ʵ�ʵ�ÿ��������      */
/*˵��: 4f06h����,ȷ��bytesperpixel�Ѿ�����ʼ��*/
unsigned int SetLogicWidth(unsigned int pixels)
{
  asm{
    mov cx,pixels
    xor bl,bl
    mov ax,04f06h
    int 10h
    /*cx=ÿ��ʵ�ʵ�������*/
    xor ax,ax
    mov al,bytesperpixel
    mul cx
    mov bytesperline,ax         /*���¸���ÿ���ֽ���*/ 
  }
  return(_CX);
}

/*===== ������ʾ�ڴ���ʼ��ַ =====*/
/*��ڲ���: (x,y)��ʾ�ڴ���ʼ����,������Ϊ��λ */
/*���ڲ���: 1:�ɹ� 0:ʧ��         */
/*˵��: 4f07h����                 */
signed intSetShowBegin(unsigned int x,unsigned int y)
{
  if((vbeinfo.VBEVersion>>8)>=2){
    asm{
      mov bl,0
      /* ��ѯVBE��Ϣ,�Ƿ���Ҫ�ȴ���ʾ����ֱ��ɨ */
      mov ax,word ptr vbeinfo.Capabilities
      test ax,4
      jz  short SetIt
      or  bl,080h
      jmp short SetIt
    }
  }
  else{     /* ��VGA����,�ȴ���ʾ����ֱ��ɨ */
    asm mov dx,03dah      /* ��Ƶ״̬�˿� */
    WaitDE:asm{           /* �ȴ���ʾ������ */
      in  al,dx
      test al,1
      jnz short WaitDE
    }
    WaitVR:asm{           /* �ȴ���ʾ����ֱ��ɨ */
      in  al,dx
      test  al,8
      jz  short WaitVR
      xor bl,bl
    }
  }
  /* ��VGA����,���ȴ���ʾ����ֱ��ɨ */
  SetIt:asm{
    mov cx,x
    mov dx,y
    mov ax,4f07h
    int 10h
  }
  if(_AX==0x4f){
    VRAMStartX=x;
    VRAMStartY=y;
    return 1;
  }
  return 0;
}

/*=====     ����DAC��ɫ��    =====*/
/*��ڲ���: palette:ָ��256ɫ��ɫ�����ݵ�Զָ��*/
/*          start:��ʼ��ɫ�� num:Ҫ���õ���ɫ��*/
/*���ڲ���: 1:�ɹ� 0:ʧ��                      */
/*˵����4f09h 00���� VBE2.0����                */
signed int setpaletteVBE(unsigned int start,unsigned int num,SvgaDAC far *palette)
{
  asm{
    mov bl,0
    /*��ѯVBE��Ϣ,�Ƿ���Ҫ�ȴ���ʾ����ֱ��ɨ/*/
    mov ax,word ptr vbeinfo.Capabilities
    test ax,4
    jz short NoWaitVR
    or bl,80h
  }
  NoWaitVR:asm{
    mov ax,04f09h
    mov dx,start
    /*����DI*/
    les di,palette
    mov cx,dx
    shl cx,2
    add di,cx
    mov cx,num
    int 10h
  }
  return(_AX==0x4f);
}
/*=====     ��ȡDAC��ɫ��    =====*/
/*��ڲ���: palette:ָ��256ɫ��ɫ�����ݵ�Զָ��*/
/*          start:��ʼ��ɫ�� num:Ҫ���õ���ɫ��*/
/*���ڲ���: 1:�ɹ� 0:ʧ��                      */
/*˵����4f09h 01���� VBE2.0����                */
signed int getpaletteVBE(unsigned int start,unsigned int num,SvgaDAC far *palette) 
{
  asm{
    mov bl,1
    /*��ѯVBE��Ϣ,�Ƿ���Ҫ�ȴ���ʾ����ֱ��ɨ/*/
    mov ax,word ptr vbeinfo.Capabilities
    test ax,4
    jz short NoWaitVR
    or bl,80h
  }
  NoWaitVR:asm{
    mov ax,04f09h
    mov dx,start
    /*����DI*/
    les di,palette
    mov cx,dx
    shl cx,2
    add di,cx
    mov cx,num
    int 10h
  }
  return(_AX==0x4f);
}

/*=====     ����DAC��ɫ��    =====*/
/*��ڲ���: palette:ָ��256ɫ��ɫ�����ݵ�Զָ��*/
/*          start:��ʼ��ɫ�� num:Ҫ���õ���ɫ��*/
/*���ڲ���: 1:�ɹ� 0:ʧ��                      */
/*˵����VGAͨ��IO�˿����õ�ɫ��                */
signed int setpaletteVGA(unsigned int start,unsigned int num,SvgaDAC far *palette)
{
	asm{
		push	ds
		lds	si,palette
		/*�ȴ�CRT��ֱ��ɨ*/
		/*����״̬�Ĵ���03dah����λ=1��ʾCRT���ڴ�ֱ��ɨ*/
		mov	dx,03dah
	}
	WaitNotVsync:asm{
		in	al,dx
		test	al,8
		jnz	short WaitNotVsync
	}
	WaitVsync:asm{
		in	al,dx
		test	al,8
		jz	short	WaitVsync
		mov	dx,3c8h
		mov	cx,num
		mov	ax,start
		out	dx,al
		inc	dx
		shl	ax,2
		add	si,ax
	}
	nextColor:asm{
		mov	al,[si+2]
		out	dx,al
		mov	al,[si+1]
		out	dx,ax
		mov	al,[si+0]
		out	dx,al
		add	si,4
		loop	short nextColor
		pop	ds
	}
	return 1;
}

/*=====     ��ȡDAC��ɫ��    =====*/
/*��ڲ���: palette:ָ��256ɫ��ɫ�����ݵ�Զָ��*/
/*          start:��ʼ��ɫ�� num:Ҫ���õ���ɫ��*/
/*���ڲ���: 1:�ɹ� 0:ʧ��                      */
/*˵����VGAͨ��IO�˿����õ�ɫ��                */

signed int getpaletteVGA(unsigned int start,unsigned int num,SvgaDAC far *palette)
{
	asm{
		push	ds
		lds	di,palette
		/*�ȴ�CRT��ֱ��ɨ*/
		/*����״̬�Ĵ���03dah����λ=1��ʾCRT���ڴ�ֱ��ɨ*/
		mov	dx,03dah
	}
	WaitNotVsync:asm{
		in	al,dx
		test	al,8
		jnz	short WaitNotVsync
	}
	WaitVsync:asm{
		in	al,dx
		test	al,8
		jz	short	WaitVsync
		mov	ax,start
		mov	bx,ax
		shl	bx,2
		add	si,bx
		mov	cx,num
	}
	nextColor:asm{
		push	ax
		mov	dx,3c7h
		out	dx,al
		mov	byte ptr [di+3],0
		add	dx,2
		in	al,dx
		mov	[di+2],al
		in	al,dx
		mov	[di+1],al
		in	al,dx
		mov	[di+0],al
		add	di,4
		pop	ax
		inc	ax
		loop	short	nextColor
		pop	ds
	}
	return	1;
}
/*
signed int getpaletteVGA(unsigned int start,unsigned int num,SvgaDAC far *palette)
{
  asm{
    push  ds
    lds di,palette
    mov ax,start
    mov dx,3c7h
    out	dx,al
    mov bx,ax
    shl bx,2
    add di,bx
    mov cx,num
  }
  NextColor:asm{
    push  ax
    mov dx,3c9h
    mov byte ptr [di+3],0 /*�����ֽ�*//*
    in  al,dx
    mov [di+2],al         /*��ɫ*//*
    in  al,dx
    mov [di+1],al         /*��ɫ*//*
    in  al,dx
    mov [di+0],al         /*��ɫ*//*
    pop ax
    inc ax
    loop  short NextColor
    pop ds
  }
  return(1);
}*/