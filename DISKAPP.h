#ifndef __DISKAPP_H_
#define __DISKAPP_H_

#define  MAX_PARTN_ELEM     24                      		   /** �ɽ��ܵķָ������� **/

#define CUT_ERR_PARTN         0x01                         /** �г�������� **/ 
#define INV_PARTN_TAB         0x02                         /** ��Ч������ **/ 
#define PARTN_NUM_TOO_MUCH    0x04                         /** ��������������� **/ 
#define INV_DISK_PARA         0x08                         /** Ӳ����� **/ 
#define PARTN_ACCESS_ERR      0x10                         /** �������ȡʧ�� **/ 
#define PARTN_OVERLAP         0x20                         /** ���������ص� **/ 
#define GIVE_UP_CHG_WD        0x40                         /** �����л�Ӳ������ **/
#define DISK_ERROR            0x80                         /** �ŵ�������ֻ��һ��Ӳ�� **/
#define PARTN_OVERLAP_OR_TOO_MUCH \
            (PARTN_OVERLAP | PARTN_NUM_TOO_MUCH)
/* check system ID condition */
#define EXTENDED_ID           0x01
#define FAT12_16              0x02
#define LBA_FAT               0x04
#define NT_ID                 0x08
#define HIDD_ID               0x80
#define FAT12_16_32           (FAT12_16 | LBA_FAT)
#define NT_FAT12_16_32        (FAT12_16_32 | NT_ID)
#define HIDDEN_ID	0x26

#define  FOR_STCYL          0
#define  FOR_NUMBER         1

#define CUR_DK        (g_DTable)                         			/** Ŀǰ�Ĺ����� **/
#define FALSE                 0
#define TRUE                  1
            
/*����Ӧ�ò����Ϊ����fdisk��format׼����ص�API����*/
typedef struct partition
{
   unsigned char  active,                                                /* �����  */ 
         					problem;                                                               
   signed int    	no,                                                    /* �������  */ 
         					id,                                                    /* ϵͳ ID   */ 
         					old_id,                                                /* ����ǰ ID */ 
     
         					remake,                                                /* �ؽ����  */ 
         					adjust;                                                                
   unsigned long	stCyl,                                                 /* ��ʼ����  */ 
         					endCyl,                                                /* ��������  */ 
         					side,                                                  /* ��ʼ��    */ 
         					sector;                                                /* ��ʼ����  */ 
} PARTN;

typedef struct common_information
{               
    unsigned int      nFP,                                         /** ��������� **/
              nHD;                                         /** Ӳ�������� **/
    DiskDT    *wdk;                                /** ָ�������Ĳ����� **/
    signed int     reboot_flag;           /** ����ʱ�Ƿ������ʾ���¿���֮ѶϢ **/
    signed int     swap_status;                         /** �Ƿ��дŵ��û���Ϊ **/
    signed int     expertMode;                         /** ʹ��ģʽ(ר��,һ��) **/
    signed int     usingMenu;                                                     
    signed int     prevMenu;                                                      
                                                             
struct fdisk_info                                  /** FDisk ��ʽר�� **/
    {          
        unsigned long  ext_head,			/*��չ������ʼ,�粻������Ϊ-1*/
               ext_end;								/*��չ��������,�粻������Ϊ-1*/
        signed int  logic_exist,			/*�߼���������,0Ϊ������*/
               maxParNO,							/*���������,�߼�������'5'��ʼ��*/
               used,									/* ������������õ��˼��� */
               pri_use,								/*(���Ϊ4)mbr�з�������,����ڷ���������,��ʣ��ռ����������е�,no=0,����no=-1*/
               primary,								/*������������������չ����*/
               modified,
               use_fat32,
               canSaveFlag,
               clr_screen;
    } fd;

} INFO;

struct SavePara													/* ���������ʱ����Ҫ�õ������ݽṹ */
{
    PARTN  *partnArr;
    int  adjust, doom;
};
/*extern unsigned int LB_FLAG;				/* lbdebug */

extern DiskDT    g_DTable[1];        /** ָ��ִ��ʱ���õĴŵ�������(����) **/
  														       /*������Ե��ǵ�Ӳ�̣����Դ˲�������ʵ����������Ҫ����Ϣ*/
extern INFO      g_Info;             /** �ۺ�ϵͳ��Ѷ�ṹ��ͼ����ʾ��fdisk���� **/		
extern DiskDRV  g_diskDrv;
/*====================��������====================================*/
extern signed int CreateDiskParaTable (void);
extern signed int Get_Disk_Info (DiskDT *dk, PARTN *partnArr, unsigned long mbr_addr,signed int find_flag);
extern void Init_Fdisk (void);
extern unsigned long Get_nSector (PARTN *ptr);
extern signed int Is_Extended (unsigned char id);
extern void Find_space (PARTN *partnArr);
extern signed int Find_Problem (PARTN *partnArr);
extern signed int IsHiddenWinID (signed int id);
extern signed int IsHiddenID (signed int id);
extern void Ins_elem (PARTN *partnArr, signed int ndx, unsigned long begin, unsigned long end);
extern signed int linear_ReadWDK (unsigned long linearSect, unsigned long nSects, void *buff);
extern signed int linear_WriteWDK (unsigned long linearSect, unsigned long nSects, void *buff);
extern signed int Get_Logic_Partn (PARTN *partnArr, PARTN *extPartn);
extern unsigned long GetStart (PARTN *ptr);
extern signed int chs_CheckWDK (unsigned long head, unsigned long cyl, unsigned long sect, unsigned long nSects, void *buff);
extern void Sort_Partn (PARTN *partnArr, signed int kind);
#endif
