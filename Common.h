/* �����û����������ݽṹ��Ϊͨ�õ����ݽṹ��ȡ���� */
#ifndef _COMMON_H
#define _COMMON_H

//#ifndef LBDEBUG						/* ���Կ��� */
//#define LBDEBUG
//#endif 

#define KEY_UP	    0x4800
#define KEY_DOWN    0x5000
#define KEY_LEFT		0x4b00
#define KEY_RIGHT		0x4d00
#define KEY_ENTER   0x1c0d
#define KEY_TAB			0x0f09
#define KEY_ALTR		0x1300
#define KEY_F1      0x3b00
#define KEY_F2      0x3c00
#define KEY_F3      0x3d00
#define KEY_F4      0x3e00
#define KEY_F5      0x3f00
#define KEY_SPACE   0x3920

#define KEY_ALTF9   0x7000
#define KEY_1				0x0231
#define KEY_2				0x0332
#define KEY_3				0x0433
#define KEY_4				0x0534
#define KEY_5				0x0635
#define KEY_6				0x0736
#define KEY_7				0x0837
#define KEY_8				0x0938
#define KEY_9				0x0A39
#define KEY_0				0x0B30
#define KEY_NUM1				0x4F31
#define KEY_NUM2				0x5032
#define KEY_NUM3				0x5133
#define KEY_NUM4				0x4B34
#define KEY_NUM5				0x4C35
#define KEY_NUM6				0x4D36
#define KEY_NUM7				0x4737
#define KEY_NUM8				0x4838
#define KEY_NUM9				0x4939
#define KEY_NUM0				0x5230

#define KEY_ESC			0x011b
#define KEY_PGUP		0x4900
#define KEY_PGDN		0x5100
#define KEY_CTRLS		0x1F13
/* Ϊ��ʵ�ֿɱ�����Ĵ�ȡ,����C���Ե�ʵ�ֶ�����һ���� */
/*__i_size(x) �е�д����Ϊ���ڴ�����*/

typedef void far *i_va_list;
#define __i_size(x) ((sizeof(x)+sizeof(int)-1) & ~(sizeof(int)-1))
#define i_va_start(ap, parmN) ((void)((ap) = (i_va_list)((char far *)(&parmN)+__i_size(parmN))))
#define i_va_arg(ap, type) (*(type far *)(((*(char far *far *)&(ap))+=__i_size(type))-(__i_size(type))))
#define i_va_end(ap)          ((void)0)


typedef struct navBar
{
	unsigned int os_index;			/* ��������Ӧ����Ӧ����Դ���(OS) */
	unsigned int x,y;						/* ��������λ�� */
	unsigned char msg[13];			/* ��������ʾ���ַ��� */
	unsigned long	msg_color;		/* �ַ�������ɫ */
	unsigned long font;					/* ���� */
	unsigned long	chg_color;		/* ѡ���󵼺�����ɫ,ע��,��ʼ����ʱ��Ϊ��8λ����ɫ,��8λѡ�������ɫ */
}BAR;
typedef	struct menuitem
{
	//unsigned char index;				/* �˵���������� */
	unsigned int x,y;						  /* �˵���λ�� */
	unsigned char valid;				  /* �˵����Ƿ���Ч */
	unsigned char *msg;					  /* �˵���ʾ�ַ��� */
	//unsigned long font;					/* ����̶�Ϊ16x16���� */
	unsigned long msg_color;		  /* �˵��ַ�������ɫ ��8λΪ��Чʱ�����ɫ,��8λΪ��Чʱ�����ɫ */
	unsigned long chg_color;		  /* ���˵�ѡ��ʱ�����ɫ ע��,��ʼ����ʱ��Ϊ��8λ����ɫ,��8λѡ�������ɫ */
}MenuItem;

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
	unsigned long lba_ifox;							/* foxdisk�������ŵ�λ�� */	
	unsigned int ver;									  /* �汾��Ǩ����,��0x0100��ʼ */
	iWallPaper iwapaper[7];
	
	/*signed int modified;								/* �û��Ƿ������ϵͳ���� 0Ϊδ���� */
}FOXPARA;


typedef struct OperationSystem{
	unsigned char bar_index;				/* ����������ʾ˳�� 0~3 */
	unsigned char os_valid;					/* �˽ṹ�Ƿ���ʵ��Ӳ����Դ��Ӧ */
	unsigned char os_name[13];			/* ϵͳ����,���Ϊ12���ַ��� */
	unsigned char pwd_valid;				/* �����Ƿ���Ч */
	unsigned char pwd[13];					/* ��������12���ַ������� */
	unsigned long os_mbr;						/* ��ǰOS��Ӧ��MBR����ʼ��ַ */	
}OS;   

#endif