#include "Global.h"
#include "Font.h"	
#include "Vesa.h"
#include "DISKDRV.h"
#include "DISKAPP.h"

/* 注意,以下的标志和版本信息不能换位置 */
char  gFoxdisk[] = "iXLZ";					/* 标志而已 我老婆的简称 */
unsigned int iFoxVer=0x0102;				/* 版本信息,注意修改下面的一些字符串与之对应 */

unsigned char debug1[]="%d %d";
unsigned char debug2[]="%lx ";

						

/* ====================BootMenu.c==================== */
unsigned char *bootStr[]={
		"   ==Foxdisk启动程序==",
		"正在启动系统: %d.%s",
		"启动倒计时: "
	};
unsigned long bmsg_color=0;
unsigned long bbg_color=0;
/* ====================MainMenu.c==================== */
unsigned char helpTitle[]="帮助文档";
unsigned char *helpStr[]={
	"                 Foxdisk3.01 帮助文档",
	"一 简述 ",
	"    本程序由五个部分组成:帮助文档(F1) 系统菜单(F2) 参数设定(F3) 工",
	"具集(F4) 关于(F5).",
	"帮助文档:在系统菜单出现的时候使用'F1'可以调出,对程序的简单说明    ",
	"系统菜单:提供对系统进行分区管理 更改系统菜单名称 调整菜单顺序 以及",
	"         设置密码的功能.",
	"参数设定:提供设置倒计时和取消倒计时 设置缺省系统 更换壁纸的功能",
	"工具集:  全盘坏道检测 扇区信息显示 快速格式化 虚拟软盘 LBOS1.0 ",
	"         在这个版本中还没有实现这些功能,这是一个督促自己的计划.",
	"关于:    程序的版本和作者信息",
	"二 名词解释",
	"    缺省系统:当前最多可引导4个不同的系统,可以由用户设定启动的时候",
	"             进入其中一个,这个就是缺省系统.",
	"    系统菜单:指可以引导的四个系统的显示菜单.",
	"    壁纸:启动的过程中的背景图案,可以在安装的时候指定,必须是256色的",
	"         BMP图.可以安装多个,在参数设定中使用更换功能来选择",
	"三 键盘使用说明",
	"    可以使用方向键中的上下键和Tab键来更换选定菜单,使用方向键中的右",
	"键来唤出功能菜单(当然是有功能菜单提供的情况下.所以,在能够使用上下",
	"键的菜单中试试用方向键的右移看看,说不定就有新的发现^^)",
	"    注意:所有更改均在内存中进行,退出Foxdisk前才会存档!!! ",
	"其他功能自己慢慢试验吧,祝使用愉快!",
	"                                                ---知更鸟的小屋---"
	};
unsigned char aboutTitle[]="关于Foxdisk...";
   /* 系统功能菜单 */
unsigned char *sysFun_str[]={"分 区 管 理 ","系统菜单命名","系统菜单排序","设 置 密 码 ","   退  出   "};
   /* 更名系统菜单 */
unsigned char *sysFun_Name[]={
	"提示: 最大支持12个字符名称,尽量简化自己的系统名称.可以接受",
	"      空字符(相当于清除了系统名),选择你喜欢的名字填入吧.",
	"  当前系统名: %s",
	"  请输入新的系统名: %s",
	"  新的系统名为: %s",
	"已经更改成功,按任意键结束..."
};
unsigned char *sysFun_Sort[]={
	"提示: 通过互换两个不同系统的方法来实现调换菜单显示顺序.",
	"当前菜单: 1.%s  2.%s",
	"          3.%s  4.%s",
	"请输入欲调换的源菜单号(1~4): %d",
	"请输入欲调换的目的菜单号(1~4): %d",
	"恭喜,已经更换成功!   您还需要继续更换吗(Y/N)?: %c",
	"更换结果为:  1.%s  2.%s",
	"             3.%s  4.%s",
	"    操作完成,按任意键结束..."
};
unsigned char *sysFun_Pwd[]={
	"提示: 为当前系统设置密码或者取消密码,取消密码可以通过输入空字符实",
	"      现.只允许最大12字节长的由数字或字母组成的密码.",
	"当前系统名: %s",
	"请输入原来的密码: %p",
	"您的输入不正确,还要继续尝试吗(Y/N)?: %c",
	"    请输入新的密码: %p",
	"    请确认新密码:   %p",
	"输入不匹配,继续设置密码吗:(Y/N)?: %c",
	"密码设置成功,按任意键结束...",
	"密码取消成功,按任意键结束..."
};	
unsigned char hideFun_str0[]="luobing";
unsigned char hideFun_str1[]="隐藏的功能菜单";
unsigned char hideFun_str2[]="请输入管理员密码: %s";
unsigned char hideFun_str3[]="错误! 您没有获取敏感信息的权限.按任意键退出...";
unsigned char hideFun_str4[]="系统%d: 名称为%s  密码为%s  对应MBR为0x%lx";
unsigned char hideFun_str5[]="    显示完毕,按任意键退出...";

unsigned char *about_str[]={
		"知更鸟的小屋(R) Foxdisk",
		"版本号3.01.02",
		"版权所有(C)2008-2108",
		"罗冰2008.11撰于南京 江宁枫彩园蜗居",
		"时值冬至,秦淮河水在晚上散发着奇怪的味道,空负了个水月秦淮的美名.",
	};
unsigned char *exitSys_str[]={
		"主程序-退出处理",
		"您已经改变系统信息,是否确定保存(Y/N)?:%c",
		"保存成功,按任意键退出...",
		"保存失败,硬盘可能有坏道存在!"
	};		

/* ====================SetPara.c==================== */
	unsigned char *paramenu_str[]={  
		"      设置倒计时 ",  
		"      取消倒计时 ",  
		"      设置缺省网 ",  
		"      更 换 壁 纸",  
		"      隐 藏 壁 纸",
		"     返回系统菜单",	
	};
	unsigned char *cntdown_str[]={
		"注意:取消倒计时后,在设置中输入新的时间即意味着打开倒计时.",
		"当前倒计时状态: ","打开","关闭",
		"当前倒计时时间(秒): ","无效",
		"请输入新的时间数(3--99): %d",
		"您确定要关闭倒计时吗(Y/N)?:%c",
		"已接受您的输入,按任意键退出...",	
		"%d"
	};
	unsigned char *setdefos_str[]={
		"所有系统信息如下:",
		"系统%d: %s","不存在",
		"当前缺省系统编号: 系统%d.%s",
		"抱歉,当前只有不大于一个的系统存在,不能设置!",
		"请选择您要设置为缺省启动的系统(1--4):%d ",
		"设置成功! 任意键退出..."
	};
	unsigned char *setwallp_str[]={
		"无法即时显示壁纸,非常抱歉.此功能列入了下一版本的开发计划中",
		"当前壁纸数: %d","隐藏",
		"当前使用的壁纸编号: ",
		"没有可用的壁纸,无法执行.",
		"请选择新的壁纸(1--%d):","%d",
		"已接受您的输入,任意键退出...",
		"%s",
		"您确定要执行吗(Y/N)?:%c"   
	};
	unsigned char *exitPara_str[]={
		"参数设定-退出处理",
		"您已经改变参数,是否确定保存(Y/N)?:%c",
		"保存成功,按任意键退出...",
		"保存失败,硬盘可能有坏道存在!"
	};
/* ====================Fdisk.c==================== */
unsigned char *fdisk_str[]={
	"发现分区重迭,你使用的分区工具有问题,找他去,别找我!",
	"分区表存在错误,本软件没有提供类似的容错机制!",
	"硬盘访问错误,可能是硬盘没有接好或者MBR区损坏",
	"按任意键退出显示...",
	"[逻辑分区]",
	"分区号 活动  起始磁道  结束磁道  容量(M)  分区类型 分区类型描述",
	"%d","%c","%ld","%x","%s",
	"unuse"
};
unsigned char *part_fun[]={																		/* 分区管理功能菜单所用的字符串 */
	" 创建主分区 ",
	"创建扩展分区",
	"创建逻辑分区",
	"设置活动标志",
	"清除活动标志",
	"设置分区类型",
	"删 除 分 区 ",
	"分区表面检测",
	"  退    出  "
};
unsigned char *creatpart_str[]={															/* 创建分区所用的字符串 */
		"硬盘信息:   总容量:%ldM  磁道:%ld  磁头:%ld  扇区:%ld",
		"可用磁道信息:   最小:%ld  最大:%ld",
		" 只有一个磁道,无法再分了.按任意键退出...",
		"    请输入起始磁道 (%ld--%ld):",
		"    请输入结束磁道 (%ld-- %ld):",
		"%ld",
		"您划分的分区为:%ld--%ld,",
		"确认吗(Y/N)?: %c",
	};
unsigned char *delPartn_str[]={																/* 删除分区所用的字符串 */
		"分区信息: 编号:%d   起始磁道:%ld  结束磁道:%ld",
		"您确定要删除此分区(Y/N)?: %c"
	};	
unsigned char *chkpart_str[]={
	"未检测","正常磁道","坏道",
	"起始磁道:%ld 结束磁道:%ld",
	"总磁道数:%ld  总坏道数:%ld  当前磁道:%ld  剩余磁道数:%ld",
	"提示: 您确定要进行分区的表面坏道检测吗(Y/N)?: %c",
	"提示: 检测完毕,恭喜,没有发现坏道^^,按任意键退出...",
	"提示: 检测完毕,发现坏道,您要显示详情吗(Y/N)?: %c",
	"提示: 按[ESC]键退出检测, 空格键[SPACE]暂停检测",
	"分区信息:  起始磁道:%ld  结束磁道:%ld    坏道总数:%ld",
	"提示: 当前最大只能显示%ld个坏道,按[ESC]键退出显示.",
	"%ld,"
	};
unsigned char *savepart_str[]={
		"分区状态错误,不能存储您的改动!",
		"分区有改动,您要存储吗(Y/N)?: %c",
		"您没有设置活动分区,仍然继续存储并退出分区管理吗(Y/N)?:%c",
		"抱歉,意外错误导致写入失败!",
		"恭喜,当前系统分区信息保存成功!"
	};
unsigned char setid_array[]={
		00,  01,  04,  05,  06,  07,  0xb,  0xc,  0xe,
		0xf, 0x3c,0x63,0x80,0x81,0x82,0x83, 0x85,0x86,
		0x8e,0xa0,0xa5,0xa6,0xa9,0xc1,0xc4, 0xc6,0xf2
		};
	unsigned char *setid_str[]={
	"请输入分区类型(十六进制 0~ff): %x",
	"%x ","%s",
	"错误! 扩展分区已经存在,不能设置为扩展分区!"
};
char ID_00[] = "none";
char ID_01[] = "DOS FAT12";
char ID_02[] = "XENIX root";
char ID_03[] = "XENIX usr";
char ID_04[] = "DOS FAT16 <32M";
char ID_05[] = "Extended";
char ID_06[] = "DOS FAT16 >=32M";
char ID_07[] = "HPFS/NTFS";
char ID_08[] = "AIX";
char ID_09[] = "AIX bootable";
char ID_0A[] = "OS/2 Boot Manag";
char ID_0B[] = "DOS FAT32";
char ID_0C[] = "DOS FAT32(LBA)";
char ID_0E[] = "DOS FATs(LBA)";
char ID_0F[] = "DOS Extend(LBA)";
char ID_10[] = "OPUS";
char ID_12[] = "Compaq diagnost";
char ID_18[] = "AST WindowsSwap";
char ID_20[] = "SPF Boot Manage";
char ID_24[] = "NECDOS";
char ID_39[] = "Plan9";
char ID_3C[] = "Partition Magic";
char ID_40[] = "Venix 80286";
char ID_41[] = "PPC PReP Boot";
char ID_42[] = "SFS";
char ID_4D[] = "QNX4.x";
char ID_4E[] = "QNX4.x 2nd part";
char ID_4F[] = "QNX4.x 3nd part";
char ID_50[] = "OnTrack DM";
char ID_51[] = "OnTrack DM6 Aux";
char ID_52[] = "CP/M";
char ID_53[] = "OnTrack DM6 Aux";
char ID_54[] = "OnTrack DM6";
char ID_55[] = "EZ-Drive";
char ID_56[] = "Golden Bow";
char ID_5C[] = "Priam Edisk";
char ID_61[] = "SpeedStor";
char ID_63[] = "GNU HURD";
char ID_64[] = "Novell Netware";
char ID_65[] = "Novell Netware";
char ID_70[] = "DiskSecure Mult";
char ID_75[] = "PC/IX";
char ID_80[] = "Old MINIX";
char ID_81[] = "MINIX/old Linux";
char ID_82[] = "Linux swap";
char ID_83[] = "Linux native";
char ID_84[] = "hidden OS/2 C:";
char ID_85[] = "Linux extended";
char ID_86[] = "NTFS volume set";
char ID_87[] = "NTFS volume set";
char ID_8E[] = "Linux LVM";
char ID_93[] = "Amoeba";
char ID_94[] = "Amoeba BBT";
char ID_9F[] = "BSD/OS";
char ID_A0[] = "IBM Thinkpad hi";
char ID_A5[] = "FreeBSD";
char ID_A6[] = "Open BSD";
char ID_A7[] = "NEXTSTEP";
char ID_A9[] = "NetBSD";
char ID_B7[] = "BSDI fs";
char ID_B8[] = "BSDI swap";
char ID_C1[] = "DRDOS/sec FAT";
char ID_C4[] = "DRDOS/sec FAT";
char ID_C6[] = "DRDOS/sec FAT";
char ID_C7[] = "Syrinx";
char ID_DA[] = "Non-FS data";
char ID_DB[] = "CP/M";
char ID_DE[] = "Dell Utility";
char ID_E1[] = "DOS access";
char ID_E3[] = "DOS R/O";
char ID_E4[] = "SpeedStor";
char ID_EB[] = "BeOS fs";
char ID_EE[] = "EFI GPT";
char ID_EF[] = "EFI (FAT-12/16)";
char ID_F1[] = "SpeedStor";
char ID_F2[] = "DOS secondary";
char ID_F4[] = "SpeedStor";
char ID_FE[] = "LAN step";
char ID_FF[] = "BBT";

char ID_UN[] = "未知类型";
char ID_FP[] = "软盘";
char ID_BD[] = "备份ID遗失";
char ID_NE[] = "[无法启动]";

char *g_SysName[] =
{
    ID_00,  ID_01,  ID_02,  ID_03,  ID_04,  ID_05,  ID_06,  ID_07,
    ID_08,  ID_09,  ID_0A,  ID_0B,  ID_0C,  ID_UN,  ID_0E,  ID_0F,
    ID_10,  ID_UN,  ID_12,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_18,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_20,  ID_UN,  ID_UN,  ID_UN,  ID_24,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_39,  ID_UN,  ID_UN,  ID_3C,  ID_UN,  ID_UN,  ID_UN,
    ID_40,  ID_41,  ID_42,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_4D,  ID_4E,  ID_4F,
    ID_50,  ID_51,  ID_52,  ID_53,  ID_54,  ID_55,  ID_56,  ID_5C,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_61,  ID_UN,  ID_63,  ID_64,  ID_65,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_70,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_75,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_80,  ID_81,  ID_82,  ID_83,  ID_84,  ID_85,  ID_86,  ID_87,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_8E,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_93,  ID_94,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_9F,
    ID_A0,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_A5,  ID_A6,  ID_A7,
    ID_UN,  ID_A9,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_B7,
    ID_B8,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_C1,  ID_UN,  ID_UN,  ID_C4,  ID_UN,  ID_C6,  ID_C7,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_DA,  ID_DB,  ID_UN,  ID_UN,  ID_DE,  ID_UN,
    ID_UN,  ID_E1,  ID_UN,  ID_E3,  ID_E4,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_EB,  ID_UN,  ID_UN,  ID_EE,  ID_EF,
    ID_UN,  ID_F1,  ID_F2,  ID_UN,  ID_F4,  ID_UN,  ID_UN,  ID_UN,
    ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_UN,  ID_FE,  ID_FF,
    ID_UN,  ID_FP,  ID_BD,  ID_NE
};

/* ====================INT1ch==================== */
unsigned int seg_oldint=0xffff;							/* 保存以前的中断向量 */
unsigned int ofs_oldint=0xffff;

/* bit0=1打开显示时钟的开关 bit1=1打开光标显示 bit2=1打开倒计时的显示  */
unsigned int _sw_INT_=0x00;									/* 中断函数使用的开关 */
unsigned int i_int_count=0;									/* 我的中断钟使用的计数器 */
unsigned char i_number[]="0123456789";			/* 当前显示使用的字符表 */
/* 时钟显示用到的字符串数组 */
unsigned char i_when[]="00:00:00";
unsigned int oldhour_min=0x00;
unsigned char old_sec=0x00;
unsigned int _hms_x=0,_hms_y=0;			/* 时钟显示的坐标 */

/* 光标显示用到的变量 */
/*x_cur,y_cur作为全局变量存在 */
unsigned char curcolor=0;
unsigned char bg_curcolor=0;
unsigned char cur_flag=1;

/* 倒计时用到的变量 */
unsigned char cntd_str[]="00";
unsigned char cov_pci[(CNTD_SWID+1)*(CNTD_SHEI+1)]={0};
unsigned int cntdown_times=0;					/* 倒计时的总时间 */
unsigned int time_rad=0;							/* 全局变量倍数计算器*/
unsigned int cntdown_over=0;					/* 倒计时完成的标志 1表示完成*/
unsigned int _cntd_x=0,_cntd_y=0;
unsigned long cntd_color=0,cntd_bcolor=0;

/* ====================Painter.c==================== */
unsigned int x_cur=0;
unsigned int y_cur=0;
unsigned char digits[]="0123456789ABCDEFabcdefGHIJKLMNOPQRSTUVWXYZghijklmnopqrstuvwxyz ~`!@#$%^&*()-_+=|/,.<>?;:\\\'\"[]{}";
unsigned char iFaceStr[]="F1 帮助  F2 系统菜单  F3 参数设定  F4 工具集  F5 关于   00:00:00";
unsigned char iFaceStr1[]="F1 帮助  ";
unsigned char iFaceStr2[]="F2 系统菜单  ";
unsigned char iFaceStr3[]="F3 参数设定  ";
unsigned char iFaceStr4[]="F4 工具集  ";
unsigned char iFaceStr5[]="F5 关于   ";
unsigned char ifoxdisk[]="FoxDisk 3.01";
unsigned char iflag[]="知 更 鸟 的 小 屋";
unsigned char wing[17][3]=\
{
		0x70,0x00,0x00,      0x88,0x00,0x00,			//0   1
		0x84,0x00,0x00,      0x82,0x00,0x00,			//2   3
		0x81,0x80,0x00,      0x40,0x60,0x00,	    //4   5
		0x40,0x18,0x00,      0x60,0x06,0x00,      //6   7
		0x98,0x01,0x00,      0x80,0x00,0x80,      //8   9
		0x80,0x30,0x40,      0x40,0x48,0x40,      //10  11
		0x38,0x48,0x40,      0x20,0x48,0x40,		  //12  13
		0x10,0x20,0x80,      0x0c,0x11,0x00,      //14  15
		0x03,0xfe,0x00                       };
/* ====================Graphic.c==================== */
SvgaDAC sys_pal[256]={\
	68>>2,	70>>2,	68>>2,	0>>2,				/*	GROUND1	*/\
	108>>2,106>>2,	108>>2,	0>>2,				/*	MGROUND1	*/\
	180>>2,178>>2,	180>>2,	0>>2,				/*	LIGHTSHADOW1	*/\
	208>>2,	208>>2,	208>>2,	0>>2,				/*	MLIGSHADOW1	*/\
	148>>2,146>>2,	148>>2,	0>>2,				/*	SURFACE1	*/\
	84>>2,	86>>2,	84>>2,	0>>2,				/*	DARKSHADOW1	*/\
	36>>2,	38>>2,	36>>2,	0>>2,				/*	DARKPROJECT1*/\
	
	0>>2,		0>>2,		0>>2,		0>>2,				/*	BLACK	*/\
	255>>2,	255>>2,	255>>2,	0>>2,				/*	WHITE	*/\
	200>>2,	208>>2,	212>>2,	0>>2,				/*	LIGHT GRAY	*/\
	167>>2,	95>>2,	18>>2,	0>>2,				/*	DEEP BLUE	*/\
	64>>2,	64>>2,	64>>2,	0>>2,				/*	DARK BLACK	*/\
	128>>2,	128>>2,	128>>2,	0>>2,				/*	LIGHT BLACK	*/\
	0>>2,		255>>2,	255>>2,	0>>2,				/*	YELLOW	*/\
	0>>2,		153>>2,	0>>2,		0>>2,				/*	GREEN	*/\
	0>>2,		0>>2,		153>>2,	0>>2,				/*	RED		*/\
	192>>2,	0>>2,		0>>2,		0>>2,				/* BLUE		*/			  	
};
/* ====================Vesa.c==================== */
void (far *bankSwitch)(void)=NULL;   			/* Direct bank switching function */
void (*setbank)(unsigned int bank)=NULL;
signed int (*setpalette)(unsigned int start,unsigned int num,SvgaDAC far *palette)=NULL;
signed int (*getpalette)(unsigned int start,unsigned int num,SvgaDAC far *palette)=NULL;
void (*putpixel)(unsigned int x,unsigned int y,unsigned long color)=NULL;
unsigned long	(*getpixel)(unsigned int x,unsigned int y)=NULL;
/*    下列函数需要用到的全局变量       */
VBEInfo vbeinfo={0};         							/* VESA 环境信息表 00h功能调用 */
ModeInfoBlock modeinfo={0};   						/* VESA 模式详细信息 01h功能调用 */
unsigned int xres=0,yres=0;         			/* Resolution of video mode used */
unsigned int bytesperline=0;      			/* Logical CRT scanline length */
unsigned char bytesperpixel=0;    			/* 每象素所占字节数 */
unsigned int curBank=0;           			/* Current read/write bank */
unsigned int bankShift=0;         			/* Bank granularity adjust factor */
unsigned int oldMode=0;           			/* Old video mode number */
unsigned long	maxpage=0;								/* 最大显示页面 */
char far *screenPtr=NULL;            			/* Pointer to start of video memory */
unsigned int VRAMStartX=0,VRAMStartY=0; 	/* 逻辑显示起始 */
unsigned int WriteMode=0;								/* 画图写模式 */	

/* ====================DISKAPP.c===================== */	
DiskDT    g_DTable[1]={0};        /** 指向执行时配置的磁碟参数表(阵列) **/
  														/*由于针对的是单硬盘，所以此参数表其实包含了所需要的信息*/
INFO      g_Info={0};             /** 综合系统资讯结构，图形显示及fdisk所用 **/		
DiskDRV  g_diskDrv =																	/*  Disk Driver Structure initial */
{
    drv_DkOpen,
    drv_DkClose,
    drv_DkReset,
    drv_DkGetAttr,
    drv_LBA_DkRead,
    drv_LBA_DkWrite,
    drv_LBA_DkCheck,
    drv_CHS_DkRead,
    drv_CHS_DkWrite,
    drv_CHS_DkCheck
};

#include "hztable.h"	
#ifdef HZK24S_ACTIVE	/* 宋体 */
#include "hzk24s.h"
#endif
/*
#ifdef HZK24T_ACTIVE	/* HZK24公用的字符字库,本版本中已经提供缺省支持
												 即使用任何一款字体，都会提取其中的汉字全角字符 *//*
#include "hzk24t.h"
#endif*/
#ifdef HZK24K_ACTIVE	/* 楷体 */
#include "hzk24k.h"
#endif
#ifdef HZK24H_ACTIVE	/* 黑体 */
#include "hzk24h.h"
#endif
#ifdef HZK24F_ACTIVE	/* 仿宋体*/
#include "hzk24f.h"
#endif

int hzcount=HZCOUNT;
int hzk24kcount=HZK24KCOUNT;
unsigned long selfont=0;			/* 在显示中使用何种字符,特别是对字符串而言非常有用*/
														/* 高16位用来选定英文字符 低16位用来选定中文字符,具体见font.h*/
struct	ascii_typ	ascii_table[ASCIIMAX]={
{0x00,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x01,{0x00,0x00,0x7e,0x81,0xa5,0x81,0x81,0xbd,0x99,0x81,0x81,0x7e,0x00,0x00,0x00,0x00}},
{0x02,{0x00,0x00,0x7e,0xff,0xdb,0xff,0xff,0xc3,0xe7,0xff,0xff,0x7e,0x00,0x00,0x00,0x00}},
{0x03,{0x00,0x00,0x00,0x00,0x6c,0xfe,0xfe,0xfe,0xfe,0x7c,0x38,0x10,0x00,0x00,0x00,0x00}},
{0x04,{0x00,0x00,0x00,0x00,0x10,0x38,0x7c,0xfe,0x7c,0x38,0x10,0x00,0x00,0x00,0x00,0x00}},
{0x05,{0x00,0x00,0x00,0x18,0x3c,0x3c,0xe7,0xe7,0xe7,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x06,{0x00,0x00,0x00,0x18,0x3c,0x7e,0xff,0xff,0x7e,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x07,{0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x3c,0x3c,0x18,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x08,{0xff,0xff,0xff,0xff,0xff,0xff,0xe7,0xc3,0xc3,0xe7,0xff,0xff,0xff,0xff,0xff,0xff}},
{0x09,{0x00,0x00,0x00,0x00,0x00,0x3c,0x66,0x42,0x42,0x66,0x3c,0x00,0x00,0x00,0x00,0x00}},
{0x0a,{0xff,0xff,0xff,0xff,0xff,0xc3,0x99,0xbd,0xbd,0x99,0xc3,0xff,0xff,0xff,0xff,0xff}},
{0x0b,{0x00,0x00,0x1e,0x0e,0x1a,0x32,0x78,0xcc,0xcc,0xcc,0xcc,0x78,0x00,0x00,0x00,0x00}},
{0x0c,{0x00,0x00,0x3c,0x66,0x66,0x66,0x66,0x3c,0x18,0x7e,0x18,0x18,0x00,0x00,0x00,0x00}},
{0x0d,{0x00,0x00,0x3f,0x33,0x3f,0x30,0x30,0x30,0x30,0x70,0xf0,0xe0,0x00,0x00,0x00,0x00}},
{0x0e,{0x00,0x00,0x7f,0x63,0x7f,0x63,0x63,0x63,0x63,0x67,0xe7,0xe6,0xc0,0x00,0x00,0x00}},
{0x0f,{0x00,0x00,0x00,0x18,0x18,0xdb,0x3c,0xe7,0x3c,0xdb,0x18,0x18,0x00,0x00,0x00,0x00}},
{0x10,{0x00,0x80,0xc0,0xe0,0xf0,0xf8,0xfe,0xf8,0xf0,0xe0,0xc0,0x80,0x00,0x00,0x00,0x00}},
{0x11,{0x00,0x02,0x06,0x0e,0x1e,0x3e,0xfe,0x3e,0x1e,0x0e,0x06,0x02,0x00,0x00,0x00,0x00}},
{0x12,{0x00,0x00,0x18,0x3c,0x7e,0x18,0x18,0x18,0x7e,0x3c,0x18,0x00,0x00,0x00,0x00,0x00}},
{0x13,{0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x66,0x66,0x00,0x00,0x00,0x00}},
{0x14,{0x00,0x00,0x7f,0xdb,0xdb,0xdb,0x7b,0x1b,0x1b,0x1b,0x1b,0x1b,0x00,0x00,0x00,0x00}},
{0x15,{0x00,0x7c,0xc6,0x60,0x38,0x6c,0xc6,0xc6,0x6c,0x38,0x0c,0xc6,0x7c,0x00,0x00,0x00}},
{0x16,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xfe,0xfe,0xfe,0x00,0x00,0x00,0x00}},
{0x17,{0x00,0x00,0x18,0x3c,0x7e,0x18,0x18,0x18,0x7e,0x3c,0x18,0x7e,0x00,0x00,0x00,0x00}},
{0x18,{0x00,0x00,0x18,0x3c,0x7e,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00}},
{0x19,{0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7e,0x3c,0x18,0x00,0x00,0x00,0x00}},
{0x1a,{0x00,0x00,0x00,0x00,0x00,0x18,0x0c,0xfe,0x0c,0x18,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x1b,{0x00,0x00,0x00,0x00,0x00,0x30,0x60,0xfe,0x60,0x30,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x1c,{0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xc0,0xc0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x1d,{0x00,0x00,0x00,0x00,0x00,0x28,0x6c,0xfe,0x6c,0x28,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x1e,{0x00,0x00,0x00,0x00,0x10,0x38,0x38,0x7c,0x7c,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00}},
{0x1f,{0x00,0x00,0x00,0x00,0xfe,0xfe,0x7c,0x7c,0x38,0x38,0x10,0x00,0x00,0x00,0x00,0x00}},
{0x20,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x21,{0x00,0x00,0x18,0x3c,0x3c,0x3c,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00}},
{0x22,{0x00,0x66,0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x23,{0x00,0x00,0x00,0x6c,0x6c,0xfe,0x6c,0x6c,0x6c,0xfe,0x6c,0x6c,0x00,0x00,0x00,0x00}},
{0x24,{0x18,0x18,0x7c,0xc6,0xc2,0xc0,0x7c,0x06,0x06,0x86,0xc6,0x7c,0x18,0x18,0x00,0x00}},
{0x25,{0x00,0x00,0x00,0x00,0xc2,0xc6,0x0c,0x18,0x30,0x60,0xc6,0x86,0x00,0x00,0x00,0x00}},
{0x26,{0x00,0x00,0x38,0x6c,0x6c,0x38,0x76,0xdc,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x27,{0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x28,{0x00,0x00,0x0c,0x18,0x30,0x30,0x30,0x30,0x30,0x30,0x18,0x0c,0x00,0x00,0x00,0x00}},
{0x29,{0x00,0x00,0x30,0x18,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x18,0x30,0x00,0x00,0x00,0x00}},
{0x2a,{0x00,0x00,0x00,0x00,0x00,0x66,0x3c,0xff,0x3c,0x66,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x2b,{0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x2c,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00,0x00}},
{0x2d,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x2e,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00}},
{0x2f,{0x00,0x00,0x00,0x00,0x02,0x06,0x0c,0x18,0x30,0x60,0xc0,0x80,0x00,0x00,0x00,0x00}},
{0x30,{0x00,0x00,0x38,0x6c,0xc6,0xc6,0xd6,0xd6,0xc6,0xc6,0x6c,0x38,0x00,0x00,0x00,0x00}},
{0x31,{0x00,0x00,0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x7e,0x00,0x00,0x00,0x00}},
{0x32,{0x00,0x00,0x7c,0xc6,0x06,0x0c,0x18,0x30,0x60,0xc0,0xc6,0xfe,0x00,0x00,0x00,0x00}},
{0x33,{0x00,0x00,0x7c,0xc6,0x06,0x06,0x3c,0x06,0x06,0x06,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x34,{0x00,0x00,0x0c,0x1c,0x3c,0x6c,0xcc,0xfe,0x0c,0x0c,0x0c,0x1e,0x00,0x00,0x00,0x00}},
{0x35,{0x00,0x00,0xfe,0xc0,0xc0,0xc0,0xfc,0x06,0x06,0x06,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x36,{0x00,0x00,0x38,0x60,0xc0,0xc0,0xfc,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x37,{0x00,0x00,0xfe,0xc6,0x06,0x06,0x0c,0x18,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00}},
{0x38,{0x00,0x00,0x7c,0xc6,0xc6,0xc6,0x7c,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x39,{0x00,0x00,0x7c,0xc6,0xc6,0xc6,0x7e,0x06,0x06,0x06,0x0c,0x78,0x00,0x00,0x00,0x00}},
{0x3a,{0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00}},
{0x3b,{0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x00}},
{0x3c,{0x00,0x00,0x00,0x06,0x0c,0x18,0x30,0x60,0x30,0x18,0x0c,0x06,0x00,0x00,0x00,0x00}},
{0x3d,{0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x3e,{0x00,0x00,0x00,0x60,0x30,0x18,0x0c,0x06,0x0c,0x18,0x30,0x60,0x00,0x00,0x00,0x00}},
{0x3f,{0x00,0x00,0x7c,0xc6,0xc6,0x0c,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00}},
{0x40,{0x00,0x00,0x00,0x7c,0xc6,0xc6,0xde,0xde,0xde,0xdc,0xc0,0x7c,0x00,0x00,0x00,0x00}},
{0x41,{0x00,0x00,0x10,0x38,0x6c,0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00}},
{0x42,{0x00,0x00,0xfc,0x66,0x66,0x66,0x7c,0x66,0x66,0x66,0x66,0xfc,0x00,0x00,0x00,0x00}},
{0x43,{0x00,0x00,0x3c,0x66,0xc2,0xc0,0xc0,0xc0,0xc0,0xc2,0x66,0x3c,0x00,0x00,0x00,0x00}},
{0x44,{0x00,0x00,0xf8,0x6c,0x66,0x66,0x66,0x66,0x66,0x66,0x6c,0xf8,0x00,0x00,0x00,0x00}},
{0x45,{0x00,0x00,0xfe,0x66,0x62,0x68,0x78,0x68,0x60,0x62,0x66,0xfe,0x00,0x00,0x00,0x00}},
{0x46,{0x00,0x00,0xfe,0x66,0x62,0x68,0x78,0x68,0x60,0x60,0x60,0xf0,0x00,0x00,0x00,0x00}},
{0x47,{0x00,0x00,0x3c,0x66,0xc2,0xc0,0xc0,0xde,0xc6,0xc6,0x66,0x3a,0x00,0x00,0x00,0x00}},
{0x48,{0x00,0x00,0xc6,0xc6,0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00}},
{0x49,{0x00,0x00,0x3c,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x4a,{0x00,0x00,0x1e,0x0c,0x0c,0x0c,0x0c,0x0c,0xcc,0xcc,0xcc,0x78,0x00,0x00,0x00,0x00}},
{0x4b,{0x00,0x00,0xe6,0x66,0x66,0x6c,0x78,0x78,0x6c,0x66,0x66,0xe6,0x00,0x00,0x00,0x00}},
{0x4c,{0x00,0x00,0xf0,0x60,0x60,0x60,0x60,0x60,0x60,0x62,0x66,0xfe,0x00,0x00,0x00,0x00}},
{0x4d,{0x00,0x00,0xc6,0xee,0xfe,0xfe,0xd6,0xc6,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00}},
{0x4e,{0x00,0x00,0xc6,0xe6,0xf6,0xfe,0xde,0xce,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00}},
{0x4f,{0x00,0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x50,{0x00,0x00,0xfc,0x66,0x66,0x66,0x7c,0x60,0x60,0x60,0x60,0xf0,0x00,0x00,0x00,0x00}},
{0x51,{0x00,0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xd6,0xde,0x7c,0x0c,0x0e,0x00,0x00}},
{0x52,{0x00,0x00,0xfc,0x66,0x66,0x66,0x7c,0x6c,0x66,0x66,0x66,0xe6,0x00,0x00,0x00,0x00}},
{0x53,{0x00,0x00,0x7c,0xc6,0xc6,0x60,0x38,0x0c,0x06,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x54,{0x00,0x00,0x7e,0x7e,0x5a,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x55,{0x00,0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x56,{0x00,0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x6c,0x38,0x10,0x00,0x00,0x00,0x00}},
{0x57,{0x00,0x00,0xc6,0xc6,0xc6,0xc6,0xd6,0xd6,0xd6,0xfe,0xee,0x6c,0x00,0x00,0x00,0x00}},
{0x58,{0x00,0x00,0xc6,0xc6,0x6c,0x7c,0x38,0x38,0x7c,0x6c,0xc6,0xc6,0x00,0x00,0x00,0x00}},
{0x59,{0x00,0x00,0x66,0x66,0x66,0x66,0x3c,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x5a,{0x00,0x00,0xfe,0xc6,0x86,0x0c,0x18,0x30,0x60,0xc2,0xc6,0xfe,0x00,0x00,0x00,0x00}},
{0x5b,{0x00,0x00,0x3c,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3c,0x00,0x00,0x00,0x00}},
{0x5c,{0x00,0x00,0x00,0x80,0xc0,0xe0,0x70,0x38,0x1c,0x0e,0x06,0x02,0x00,0x00,0x00,0x00}},
{0x5d,{0x00,0x00,0x3c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x3c,0x00,0x00,0x00,0x00}},
{0x5e,{0x10,0x38,0x6c,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x5f,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00}},
{0x60,{0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x61,{0x00,0x00,0x00,0x00,0x00,0x78,0x0c,0x7c,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x62,{0x00,0x00,0xe0,0x60,0x60,0x78,0x6c,0x66,0x66,0x66,0x66,0x7c,0x00,0x00,0x00,0x00}},
{0x63,{0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xc0,0xc0,0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x64,{0x00,0x00,0x1c,0x0c,0x0c,0x3c,0x6c,0xcc,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x65,{0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xfe,0xc0,0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x66,{0x00,0x00,0x38,0x6c,0x64,0x60,0xf0,0x60,0x60,0x60,0x60,0xf0,0x00,0x00,0x00,0x00}},
{0x67,{0x00,0x00,0x00,0x00,0x00,0x76,0xcc,0xcc,0xcc,0xcc,0xcc,0x7c,0x0c,0xcc,0x78,0x00}},
{0x68,{0x00,0x00,0xe0,0x60,0x60,0x6c,0x76,0x66,0x66,0x66,0x66,0xe6,0x00,0x00,0x00,0x00}},
{0x69,{0x00,0x00,0x18,0x18,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x6a,{0x00,0x00,0x06,0x06,0x00,0x0e,0x06,0x06,0x06,0x06,0x06,0x06,0x66,0x66,0x3c,0x00}},
{0x6b,{0x00,0x00,0xe0,0x60,0x60,0x66,0x6c,0x78,0x78,0x6c,0x66,0xe6,0x00,0x00,0x00,0x00}},
{0x6c,{0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x6d,{0x00,0x00,0x00,0x00,0x00,0xec,0xfe,0xd6,0xd6,0xd6,0xd6,0xc6,0x00,0x00,0x00,0x00}},
{0x6e,{0x00,0x00,0x00,0x00,0x00,0xdc,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00}},
{0x6f,{0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x70,{0x00,0x00,0x00,0x00,0x00,0xdc,0x66,0x66,0x66,0x66,0x66,0x7c,0x60,0x60,0xf0,0x00}},
{0x71,{0x00,0x00,0x00,0x00,0x00,0x76,0xcc,0xcc,0xcc,0xcc,0xcc,0x7c,0x0c,0x0c,0x1e,0x00}},
{0x72,{0x00,0x00,0x00,0x00,0x00,0xdc,0x76,0x66,0x60,0x60,0x60,0xf0,0x00,0x00,0x00,0x00}},
{0x73,{0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0x60,0x38,0x0c,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x74,{0x00,0x00,0x10,0x30,0x30,0xfc,0x30,0x30,0x30,0x30,0x36,0x1c,0x00,0x00,0x00,0x00}},
{0x75,{0x00,0x00,0x00,0x00,0x00,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x76,{0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x3c,0x18,0x00,0x00,0x00,0x00}},
{0x77,{0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xd6,0xd6,0xd6,0xfe,0x6c,0x00,0x00,0x00,0x00}},
{0x78,{0x00,0x00,0x00,0x00,0x00,0xc6,0x6c,0x38,0x38,0x38,0x6c,0xc6,0x00,0x00,0x00,0x00}},
{0x79,{0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7e,0x06,0x0c,0xf8,0x00}},
{0x7a,{0x00,0x00,0x00,0x00,0x00,0xfe,0xcc,0x18,0x30,0x60,0xc6,0xfe,0x00,0x00,0x00,0x00}},
{0x7b,{0x00,0x00,0x0e,0x18,0x18,0x18,0x70,0x18,0x18,0x18,0x18,0x0e,0x00,0x00,0x00,0x00}},
{0x7c,{0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00}},
{0x7d,{0x00,0x00,0x70,0x18,0x18,0x18,0x0e,0x18,0x18,0x18,0x18,0x70,0x00,0x00,0x00,0x00}},
{0x7e,{0x00,0x00,0x76,0xdc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x7f,{0x00,0x00,0x00,0x00,0x10,0x38,0x6c,0xc6,0xc6,0xc6,0xfe,0x00,0x00,0x00,0x00,0x00}},
{0x80,{0x00,0x00,0x3c,0x66,0xc2,0xc0,0xc0,0xc0,0xc2,0x66,0x3c,0x0c,0x06,0x7c,0x00,0x00}},
{0x81,{0x00,0x00,0xcc,0x00,0x00,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x82,{0x00,0x0c,0x18,0x30,0x00,0x7c,0xc6,0xfe,0xc0,0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x83,{0x00,0x10,0x38,0x6c,0x00,0x78,0x0c,0x7c,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x84,{0x00,0x00,0xcc,0x00,0x00,0x78,0x0c,0x7c,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x85,{0x00,0x60,0x30,0x18,0x00,0x78,0x0c,0x7c,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x86,{0x00,0x38,0x6c,0x38,0x00,0x78,0x0c,0x7c,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x87,{0x00,0x00,0x00,0x00,0x3c,0x66,0x60,0x60,0x66,0x3c,0x0c,0x06,0x3c,0x00,0x00,0x00}},
{0x88,{0x00,0x10,0x38,0x6c,0x00,0x7c,0xc6,0xfe,0xc0,0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x89,{0x00,0x00,0xc6,0x00,0x00,0x7c,0xc6,0xfe,0xc0,0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x8a,{0x00,0x60,0x30,0x18,0x00,0x7c,0xc6,0xfe,0xc0,0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x8b,{0x00,0x00,0x66,0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x8c,{0x00,0x18,0x3c,0x66,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x8d,{0x00,0x60,0x30,0x18,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0x8e,{0x00,0xc6,0x00,0x10,0x38,0x6c,0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00}},
{0x8f,{0x38,0x6c,0x38,0x00,0x38,0x6c,0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00}},
{0x90,{0x18,0x30,0x60,0x00,0xfe,0x66,0x60,0x7c,0x60,0x60,0x66,0xfe,0x00,0x00,0x00,0x00}},
{0x91,{0x00,0x00,0x00,0x00,0x00,0xcc,0x76,0x36,0x7e,0xd8,0xd8,0x6e,0x00,0x00,0x00,0x00}},
{0x92,{0x00,0x00,0x3e,0x6c,0xcc,0xcc,0xfe,0xcc,0xcc,0xcc,0xcc,0xce,0x00,0x00,0x00,0x00}},
{0x93,{0x00,0x10,0x38,0x6c,0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x94,{0x00,0x00,0xc6,0x00,0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x95,{0x00,0x60,0x30,0x18,0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x96,{0x00,0x30,0x78,0xcc,0x00,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x97,{0x00,0x60,0x30,0x18,0x00,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0x98,{0x00,0x00,0xc6,0x00,0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7e,0x06,0x0c,0x78,0x00}},
{0x99,{0x00,0xc6,0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x9a,{0x00,0xc6,0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0x9b,{0x00,0x18,0x18,0x3c,0x66,0x60,0x60,0x60,0x66,0x3c,0x18,0x18,0x00,0x00,0x00,0x00}},
{0x9c,{0x00,0x38,0x6c,0x64,0x60,0xf0,0x60,0x60,0x60,0x60,0xe6,0xfc,0x00,0x00,0x00,0x00}},
{0x9d,{0x00,0x00,0x66,0x66,0x3c,0x18,0x7e,0x18,0x7e,0x18,0x18,0x18,0x00,0x00,0x00,0x00}},
{0x9e,{0x00,0xf8,0xcc,0xcc,0xf8,0xc4,0xcc,0xde,0xcc,0xcc,0xcc,0xc6,0x00,0x00,0x00,0x00}},
{0x9f,{0x00,0x0e,0x1b,0x18,0x18,0x18,0x7e,0x18,0x18,0x18,0x18,0x18,0xd8,0x70,0x00,0x00}},
{0xa0,{0x00,0x18,0x30,0x60,0x00,0x78,0x0c,0x7c,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0xa1,{0x00,0x0c,0x18,0x30,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00}},
{0xa2,{0x00,0x18,0x30,0x60,0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0xa3,{0x00,0x18,0x30,0x60,0x00,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00}},
{0xa4,{0x00,0x00,0x76,0xdc,0x00,0xdc,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00}},
{0xa5,{0x76,0xdc,0x00,0xc6,0xe6,0xf6,0xfe,0xde,0xce,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00}},
{0xa6,{0x00,0x3c,0x6c,0x6c,0x3e,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xa7,{0x00,0x38,0x6c,0x6c,0x38,0x00,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xa8,{0x00,0x00,0x30,0x30,0x00,0x30,0x30,0x60,0xc0,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00}},
{0xa9,{0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xc0,0xc0,0xc0,0xc0,0x00,0x00,0x00,0x00,0x00}},
{0xaa,{0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x06,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00}},
{0xab,{0x00,0xc0,0xc0,0xc2,0xc6,0xcc,0x18,0x30,0x60,0xdc,0x86,0x0c,0x18,0x3e,0x00,0x00}},
{0xac,{0x00,0xc0,0xc0,0xc2,0xc6,0xcc,0x18,0x30,0x66,0xce,0x9e,0x3e,0x06,0x06,0x00,0x00}},
{0xad,{0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x18,0x3c,0x3c,0x3c,0x18,0x00,0x00,0x00,0x00}},
{0xae,{0x00,0x00,0x00,0x00,0x00,0x36,0x6c,0xd8,0x6c,0x36,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xaf,{0x00,0x00,0x00,0x00,0x00,0xd8,0x6c,0x36,0x6c,0xd8,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xb0,{0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44}},
{0xb1,{0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa}},
{0xb2,{0xdd,0x77,0xdd,0x77,0xdd,0x77,0xdd,0x77,0xdd,0x77,0xdd,0x77,0xdd,0x77,0xdd,0x77}},
{0xb3,{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xb4,{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xf8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xb5,{0x18,0x18,0x18,0x18,0x18,0xf8,0x18,0xf8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xb6,{0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xf6,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xb7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xb8,{0x00,0x00,0x00,0x00,0x00,0xf8,0x18,0xf8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xb9,{0x36,0x36,0x36,0x36,0x36,0xf6,0x06,0xf6,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xba,{0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xbb,{0x00,0x00,0x00,0x00,0x00,0xfe,0x06,0xf6,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xbc,{0x36,0x36,0x36,0x36,0x36,0xf6,0x06,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xbd,{0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xbe,{0x18,0x18,0x18,0x18,0x18,0xf8,0x18,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xbf,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xc0,{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xc1,{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xc2,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xc3,{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1f,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xc4,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xc5,{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xc6,{0x18,0x18,0x18,0x18,0x18,0x1f,0x18,0x1f,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xc7,{0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x37,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xc8,{0x36,0x36,0x36,0x36,0x36,0x37,0x30,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xc9,{0x00,0x00,0x00,0x00,0x00,0x3f,0x30,0x37,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xca,{0x36,0x36,0x36,0x36,0x36,0xf7,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xcb,{0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xf7,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xcc,{0x36,0x36,0x36,0x36,0x36,0x37,0x30,0x37,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xcd,{0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xce,{0x36,0x36,0x36,0x36,0x36,0xf7,0x00,0xf7,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xcf,{0x18,0x18,0x18,0x18,0x18,0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xd0,{0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xd1,{0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xd2,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xd3,{0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xd4,{0x18,0x18,0x18,0x18,0x18,0x1f,0x18,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xd5,{0x00,0x00,0x00,0x00,0x00,0x1f,0x18,0x1f,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xd6,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xd7,{0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xff,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
{0xd8,{0x18,0x18,0x18,0x18,0x18,0xff,0x18,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xd9,{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xda,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xdb,{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}},
{0xdc,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}},
{0xdd,{0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0}},
{0xde,{0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f}},
{0xdf,{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xe0,{0x00,0x00,0x00,0x00,0x00,0x76,0xdc,0xd8,0xd8,0xd8,0xdc,0x76,0x00,0x00,0x00,0x00}},
{0xe1,{0x00,0x00,0x78,0xcc,0xcc,0xcc,0xd8,0xcc,0xc6,0xc6,0xc6,0xcc,0x00,0x00,0x00,0x00}},
{0xe2,{0x00,0x00,0xfe,0xc6,0xc6,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x00,0x00,0x00,0x00}},
{0xe3,{0x00,0x00,0x00,0x00,0xfe,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x00,0x00,0x00,0x00}},
{0xe4,{0x00,0x00,0x00,0xfe,0xc6,0x60,0x30,0x18,0x30,0x60,0xc6,0xfe,0x00,0x00,0x00,0x00}},
{0xe5,{0x00,0x00,0x00,0x00,0x00,0x7e,0xd8,0xd8,0xd8,0xd8,0xd8,0x70,0x00,0x00,0x00,0x00}},
{0xe6,{0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x7c,0x60,0x60,0xc0,0x00,0x00,0x00}},
{0xe7,{0x00,0x00,0x00,0x00,0x76,0xdc,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00}},
{0xe8,{0x00,0x00,0x00,0x7e,0x18,0x3c,0x66,0x66,0x66,0x3c,0x18,0x7e,0x00,0x00,0x00,0x00}},
{0xe9,{0x00,0x00,0x00,0x38,0x6c,0xc6,0xc6,0xfe,0xc6,0xc6,0x6c,0x38,0x00,0x00,0x00,0x00}},
{0xea,{0x00,0x00,0x38,0x6c,0xc6,0xc6,0xc6,0x6c,0x6c,0x6c,0x6c,0xee,0x00,0x00,0x00,0x00}},
{0xeb,{0x00,0x00,0x1e,0x30,0x18,0x0c,0x3e,0x66,0x66,0x66,0x66,0x3c,0x00,0x00,0x00,0x00}},
{0xec,{0x00,0x00,0x00,0x00,0x00,0x7e,0xdb,0xdb,0xdb,0x7e,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xed,{0x00,0x00,0x00,0x03,0x06,0x7e,0xdb,0xdb,0xf3,0x7e,0x60,0xc0,0x00,0x00,0x00,0x00}},
{0xee,{0x00,0x00,0x1c,0x30,0x60,0x60,0x7c,0x60,0x60,0x60,0x30,0x1c,0x00,0x00,0x00,0x00}},
{0xef,{0x00,0x00,0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00}},
{0xf0,{0x00,0x00,0x00,0x00,0xfe,0x00,0x00,0xfe,0x00,0x00,0xfe,0x00,0x00,0x00,0x00,0x00}},
{0xf1,{0x00,0x00,0x00,0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00,0xff,0x00,0x00,0x00,0x00}},
{0xf2,{0x00,0x00,0x00,0x30,0x18,0x0c,0x06,0x0c,0x18,0x30,0x00,0x7e,0x00,0x00,0x00,0x00}},
{0xf3,{0x00,0x00,0x00,0x0c,0x18,0x30,0x60,0x30,0x18,0x0c,0x00,0x7e,0x00,0x00,0x00,0x00}},
{0xf4,{0x00,0x00,0x0e,0x1b,0x1b,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xf5,{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xd8,0xd8,0xd8,0x70,0x00,0x00,0x00,0x00}},
{0xf6,{0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x7e,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00}},
{0xf7,{0x00,0x00,0x00,0x00,0x00,0x76,0xdc,0x00,0x76,0xdc,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xf8,{0x00,0x38,0x6c,0x6c,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xf9,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xfa,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xfb,{0x00,0x0f,0x0c,0x0c,0x0c,0x0c,0x0c,0xec,0x6c,0x6c,0x3c,0x1c,0x00,0x00,0x00,0x00}},
{0xfc,{0x00,0xd8,0x6c,0x6c,0x6c,0x6c,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xfd,{0x00,0x70,0xd8,0x30,0x60,0xc8,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xfe,{0x00,0x00,0x00,0x00,0x7c,0x7c,0x7c,0x7c,0x7c,0x7c,0x7c,0x00,0x00,0x00,0x00,0x00}},
{0xff,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
};	
int hz24_count=0;						/* 24x24汉字的总数 */
struct hzk24_typ	*hzk24_table=NULL;			/* 指定的汉字字模 */
/* bootOS.c */
unsigned char bootos_str[]="启动系统: %s";
unsigned char bootos_str1[]="请输入系统密码:";
unsigned char bootos_str2[]="%p";

unsigned char	gEndOfData[]="end";	/* 全局数据段结束 */
