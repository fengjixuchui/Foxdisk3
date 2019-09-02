/*luobing 2008-10-3 11:41 diskapp.c*/
/*ΪFoxdisk2��д��ͨ�ô��̷��ʳ���: Ӧ�ò�*/
#include "DISKDRV.h"
#include "DISKAPP.h"
#include "Global.h"

/* �����õ�ȫ�ֱ��� */
/*
unsigned int LB_FLAG=0;
unsigned int LB_FLAG1=0;*/



signed int CreateDiskParaTable (void);
signed int Get_Disk_Info (DiskDT *dk, PARTN *partnArr, unsigned long mbr_addr,signed int find_flag);
void Init_Fdisk (void);
static signed int Partn_Info (PARTN *partnArr, unsigned char *buff);
static void Get_One_Partn (unsigned long begin, PARTN *ptr, unsigned char *buff, signed int field, DiskDT *dk);
signed int Get_Logic_Partn (PARTN *partnArr, PARTN *extPartn);
signed int Find_Problem (PARTN *partnArr);
static signed int Find_Overlap (PARTN *ptr, PARTN *end);
signed int IsHiddenPartn (PARTN *ptr);
signed int IsValid (DiskDT *dk);
signed int Is_Extended (unsigned char id);
signed int IsHiddenWinID (signed int id);
signed int IsHiddenID (signed int id);
signed int ChkSysID (unsigned char id, signed int cond);
signed int linear_WriteWDK (unsigned long linearSect, unsigned long nSects, void *buff);
signed int linear_ReadWDK (unsigned long linearSect, unsigned long nSects, void *buff);
signed int chs_WriteWDK (unsigned long head, unsigned long cyl, unsigned long sect, unsigned long nSects, void *buff);
signed int chs_ReadWDK (unsigned long head, unsigned long cyl, unsigned long sect, unsigned long nSects, void *buff);
signed int chs_CheckWDK (unsigned long head, unsigned long cyl, unsigned long sect, unsigned long nSects, void *buff);
unsigned long GetStart (PARTN *ptr);
unsigned long GetEnd (PARTN *ptr);
unsigned long Get_nSector (PARTN *ptr);
static signed int ForStartCyl (const void *elem1, const void *elem2);
static signed int ForNO (const void *elem1, const void *elem2);
void Sort_Partn (PARTN *partnArr, signed int kind);
void Select_Sort (void *base, signed int elem_num, signed int elem_size,
                  signed int (*fcmp)(const void *elem1, const void *elem2));
static void Swap_Element (void *elem1, void *elem2, signed int elem_size);
void Find_space (PARTN *partnArr);
signed int Out_of_ext (PARTN *ptr);
void Ins_elem (PARTN *partnArr, signed int ndx, unsigned long begin, unsigned long end);
void CopyMem (void *target, const void *source, unsigned int size);

/*
void lbdebug(PARTN *partnArr)
{
	if(LB_FLAG==1)
 		{
 			 int i;
 			 LB_FLAG=0;
 			 asm xor ax,ax
			 asm mov ax,3
			 asm int 0x10
			 for(i=0;i<4;i++)
				{
				printf("partnArr[%d]: ",i);
				printf(" active=%02x,no=%04x,id=%04x,old_id=%04x,",partnArr[i].active,partnArr[i].no,partnArr[i].id,partnArr[i].old_id);
				printf("remake=%04x,adjust=%04x\n",partnArr[i].remake,partnArr[i].adjust);
				printf("              stCyl=%.10ld,endCyl=%.10ld\n",partnArr[i].stCyl,partnArr[i].endCyl);
				}
			 	printf("g_info:\n");
				printf(" ext_head:%ld,ext_end:%ld,logic_exist:%04x,maxParNO:%04x,used:%04x\n",g_Info.fd.ext_head,
			           g_Info.fd.ext_end,g_Info.fd.logic_exist,g_Info.fd.maxParNO,g_Info.fd.used);
			printf(" pri_use:%x,primary:%x,modified:%x,use_fat32:%x,canSaveFlag%x,clr_screen:%x\n",
					g_Info.fd.pri_use,g_Info.fd.primary,g_Info.fd.modified,g_Info.fd.use_fat32,g_Info.fd.canSaveFlag,g_Info.fd.clr_screen);
			printf("CUR_DK:lgeo,bytePerS:%d,sectPerCyl:%ld,maxCyl:%ld,maxHead:%ld,maxSect:%ld,tnSector:%ld\n",CUR_DK->lgeo.bytePerSect,\
			          CUR_DK->lgeo.sectPerCyl,CUR_DK->lgeo.maxCyl,CUR_DK->lgeo.maxHead,CUR_DK->lgeo.maxSect,(unsigned long)(CUR_DK->lgeo.tnSector));
			printf("CUR_DK:pgeo,bytePerS:%d,sectPerCyl:%ld,maxCyl:%ld,maxHead:%ld,maxSect:%ld,tnSector:%ld\n",CUR_DK->pgeo.bytePerSect,\
			          CUR_DK->pgeo.sectPerCyl,CUR_DK->pgeo.maxCyl,CUR_DK->pgeo.maxHead,CUR_DK->pgeo.maxSect,(unsigned long)(CUR_DK->pgeo.tnSector));
			 asm xor ax,ax
			 asm int 0x16
			 //exit(0);
 		}
}*/
/*----------------------- Create Disk Parameter table ------------------------+*/  
signed int CreateDiskParaTable (void)
{
    signed int   no;
    DiskDT  *dk;
   /*  ����������ֱ����Ӳ�������У��϶���Ӳ�̴���ֱ�ӹ����������򼴿� */
    no = 0x80;
    dk = g_DTable;																/*ָ��ȫ�ִ��̲�����*/
    dk->flag = HARD_DISK;
    dk->diskNO = no;
		dk->drv    = &g_diskDrv;                      /** ����ʹ�õ� driver **/
  
		dk->drv->open(dk);                             /** ��������ŵ��豸 **/  
		dk->drv->getAttr(dk);                    /** ȡ�ôŵ����μ��������� **/  
		/* foxdisk �˴�ΪFoxdiskԤ��10���ŵ� foxdisk */
		dk->pgeo.maxCyl-=10;
		dk->lgeo.maxCyl-=10;
		
		dk->drv->close(dk);                            /** �ر�����ŵ��豸 **/  
 
    return(0);
} /* end CreateDiskParaTable */

/*=============== ������̼�������Ϣ�� Partn ���鼰 info ���� ================+
|   ����ֵ��                                                                  |
|                                                                             |
|       CUT_ERR_PARTN       ���������ݷ��ִ����г��������                  |
|       INV_PARTN_TAB       ��Ч�ķ�����                                      |
|       PARTN_NUM_TOO_MUCH  �����ɽ��ܵķ�������                              |
|       INV_DISK_PARA       �ж� 13h ����ʧ�ܣ�������Ӳ�����                 |
|       PARTN_ACCESS_ERR    Ӳ�̴�ȡ����                                      |
|       PARTN_OVERLAP       ���ַ����ص�                                      |
+============================================================================*/
signed int Get_Disk_Info (DiskDT *dk, PARTN *partnArr, unsigned long mbr_addr,signed int find_flag)
{
    unsigned char   buff[512];
    signed int  err;
    unsigned int i;
    unsigned char *ptr;
 
    Init_Fdisk();                                            /** ���� fdisk info **/
 
    if ( !IsValid(dk) )
        return( INV_DISK_PARA );                             /** ��Ч������ **/
 		
 		ptr=(unsigned char *)(partnArr);
 		for(i=0;i<(sizeof(PARTN) * MAX_PARTN_ELEM);i++)					 /** ���������г�ʼ **/
 			ptr[i]=0x0;
    
    dk->drv    = &g_diskDrv;                      					 /** ����ʹ�õ� driver **/
 		if(dk->drv->linearRead(g_DTable,mbr_addr,1,buff))
    	*(unsigned int *)&buff[510] = 0;                       /** ���������ȡʧ�ܣ�**/
    
    
    err = Partn_Info(partnArr, buff);                        /** ������������Ѷ **/
 
 
    if ( Find_Problem(partnArr) )                            /** �������������ص������ **/
        err |= PARTN_OVERLAP;
 
    if ( find_flag && (err & ~PARTN_OVERLAP_OR_TOO_MUCH) == 0 )
        if ( err & PARTN_OVERLAP_OR_TOO_MUCH )
            g_Info.fd.canSaveFlag = FALSE;
 
    Find_space(partnArr);                              /** ��Ѱδ����֮���� **/
    return( err ); 
} /* end Get_Disk_Info */
/*============================= Reset FDisk =================================*/
void Init_Fdisk (void)
{                      
    g_Info.fd.ext_head    = -1;
    g_Info.fd.ext_end     = -1;
    g_Info.fd.modified      =  0;
    g_Info.fd.maxParNO    =  0;  
    g_Info.fd.used        =  0;       
    g_Info.fd.pri_use     =  0;    
    g_Info.fd.primary     =  0;
 /* g_Info.fd.use_fat32   =  0; */
    g_Info.fd.logic_exist = FALSE;
    g_Info.fd.canSaveFlag = TRUE;
    g_Info.fd.clr_screen  = TRUE;
} /* end Init_Fdisk */
/*======================== �� �� �� �� �� �� Ϣ ==============================+
|   ����ֵ��                                                                  |
|                                                                             |
|         CUT_ERR_PARTN       ���������ݷ��ִ����г��������                |
|         INV_PARTN_TAB       ��Ч�ķ�����                                    |
|         PARTN_NUM_TOO_MUCH  �����ɽ��ܵķ�������                            |
|         PARTN_ACCESS_ERR    Ӳ�̴�ȡ����                                    |
|         PARTN_OVERLAP       ����Σ�շ�������                                |
+============================================================================*/
static signed int Partn_Info (PARTN *partnArr, unsigned char *buff)
{        
    signed int  adr, err, ndx, ext, i;
    unsigned long  sector;
    PARTN  *ptr;
 
    adr = 0x1be;
    err = 0;
    ndx = 0;
    ptr = partnArr;
    ext = -1;
 		
    if ( *((unsigned int *)(&buff[510])) != 0xaa55 )
        err |= INV_PARTN_TAB;                                /** ��Ч������ **/
    else
    {
        for ( i = 0 ; i < 4 ; i++,  adr += 0x10 )            /** ��ȡ������ **/
            if ( buff[adr+4] != 0 )
            {
                if ( (ext == -1) && ( Is_Extended(buff[adr+4]) ))
                {
                    ext = ndx;
                 } /* end if */
    						
    						
                sector = *((unsigned long *)(&(buff[adr+8])));
    						
                Get_One_Partn(sector, ptr, buff, i + 1, CUR_DK);
                
                g_Info.fd.maxParNO = 4;
                g_Info.fd.primary++;
                ptr->no = i + 1;                               /** ������� **/
    
                ndx++;
                ptr++;
            } /* end if */
  
        g_Info.fd.pri_use = g_Info.fd.primary;
        g_Info.fd.used    = g_Info.fd.primary;
  
          
        if ( ext > -1 )
        {
            g_Info.fd.ext_head = partnArr[ext].stCyl;   /** ��¼ ext �������� **/
            g_Info.fd.ext_end  = partnArr[ext].endCyl;
            err = Get_Logic_Partn(partnArr, partnArr + ext); /** ��ȡ�߼����� **/
        } /* end if */
    } /* end if */
 
    /*----------------------------------------+
    |   �� �� �� �� �� �� �� ʼ �� �� �� ��   |
    +----------------------------------------*/
    Sort_Partn(partnArr, FOR_STCYL);               /** �������и��Դ������� **/
    g_Info.fd.modified = 0;
 
    return( err );
} /* end Partn_Info */

/*========================== �� �� �� �� �� Ѷ ==============================*/
static void Get_One_Partn (unsigned long begin, PARTN *ptr, unsigned char *buff, signed int field, DiskDT *dk)
{
    unsigned char   *bak_id = buff + field + 0x19f;
    unsigned char   *elem   = buff + field * 0x10 + 0x1ae;
    unsigned long  end;

    ptr->adjust =
    ptr->remake = 0;
    ptr->stCyl  =
    ptr->endCyl = 0;
    ptr->active = elem[0];
    ptr->id     = elem[4];

    if ( ptr->id != 0 )
    {
        if ( !IsHiddenPartn(ptr) )
            ptr->old_id = ptr->id;
        else if ( IsHiddenWinID(ptr->id) )
            ptr->old_id = ptr->id & 0x0f;
        else if ( *bak_id != 0 )
            ptr->old_id = *bak_id;
        else
            ptr->old_id = HIDDEN_ID;

        end = begin + *(unsigned long *)&elem[12] - 1;               /** ����� CHS **/
        Sect_to_CHS(end, &ptr->endCyl, &ptr->side, &ptr->sector, dk);
                                                            				 /** ����ʼ CHS **/
        Sect_to_CHS(begin, &ptr->stCyl, &ptr->side, &ptr->sector, dk);
    }
    else
        ptr->old_id = 0;
} /* end Get_One_Partn */
/*======================== ȡ �� �� �� �� �� �� ==============================+
|    ����ֵ��                                                                 |
|                                                                             |
|        CUT_ERR_PARTN        ���������ݷ��ִ����г��������                |
|        PARTN_NUM_TOO_MUCH   �����ɽ��ܵķ�������                            |
|        PARTN_ACCESS_ERR     Ӳ�̴�ȡ����                                    |
+============================================================================*/
signed int Get_Logic_Partn (PARTN *partnArr, PARTN *extPartn)
{                         
    unsigned long  extStart, sector;
    signed int  err, ndx, no;
    unsigned char   buff[512];
    PARTN  *ptr;
 
    extStart = GetStart(extPartn);                          /** ȡ����ʼ���� **/
    sector   = extStart;
 
    /*============ �����һ���߼������� ============*/
    if ( linear_ReadWDK(sector, 1, buff) )
        return( PARTN_ACCESS_ERR );
 
    /*============ ��һ���߼����������� ============*/
    if ( (*(unsigned long *)&buff[0x1be] == (unsigned long)0xf6f6f6f6L) || (buff[0x1c2] == 0) )
        return(0);
 
    no  = 5;
    err = 0;
    ndx = g_Info.fd.pri_use;
    ptr = partnArr + ndx;
    while ( 1 )
    {
        if ( ndx > (MAX_PARTN_ELEM - 1) )
        {
            err |= PARTN_NUM_TOO_MUCH;             /** �����ɽ��ܵķ������� **/
            break;
        } /* end if */
  
        if ( *(unsigned int *)&buff[510] != 0xaa55 )           /** ��Ч���߼������� **/
        {
            err |= CUT_ERR_PARTN;
            break;
        } /* end if */
  
        sector += *(unsigned long *)&buff[0x1c6];                /** �������ش����� **/
        Get_One_Partn(sector, ptr, buff, 1, CUR_DK);
        ptr->no = no;                                          /** ������� **/
        no++;
        ndx++;
        ptr++;
  
        /*---------------------------------------------------+
        |        �߼�������ڶ���һ�������������յ�        |
        +---------------------------------------------------*/
        if ( (*(unsigned long *)&buff[0x1d2] == 0) || !Is_Extended(buff[0x1d2]) )
            break;
  
        /*---------------------------------------------------+
        |            �� �� �� һ �� �� �� �� �� ��           |
        +---------------------------------------------------*/
        sector = extStart + *(unsigned long *)&buff[0x1d6];
        if ( linear_ReadWDK(sector, 1, buff) )
        {
            err |= PARTN_ACCESS_ERR;
            break;
        } /* end if */
    } /* end while */
 
    if ( ndx > g_Info.fd.used )
    {
        g_Info.fd.logic_exist = TRUE;
        g_Info.fd.maxParNO    = no - 1;
        g_Info.fd.used        = ndx;
    } /* end if */
     
    return( err );
} /* end Get_Logic_Partn */
/*============================== �� �� �� �� �� �� �� �� ==============================*/
signed int Find_Problem (PARTN *partnArr)
{
    PARTN  *cmp, *ext, *end;
    signed int  errFlag = FALSE;
 
    end = partnArr + g_Info.fd.used;
    for ( cmp = partnArr ; cmp < end ; cmp++ )
        cmp->problem = FALSE;
 
    ext = NULL;
    end = partnArr + g_Info.fd.pri_use;
    for ( cmp = partnArr ; cmp < end ; cmp++ )         /** ��Ѱ���������ص� **/
        if ( cmp->id )
        {
            if ( Is_Extended(cmp->old_id) )
            {
                if ( ext )
                {
                    errFlag = TRUE;
                    cmp->problem = TRUE;
                }
                else
                    ext = cmp;
            } /* end if */
  
            if ( (cmp->stCyl > cmp->endCyl) || Find_Overlap(cmp, end) )
            {
                cmp->problem = TRUE;
                errFlag = TRUE;                                /** �����ص� **/
            } /* end if */
        } /* end if */
 
    if ( ext )
    {
        end = partnArr + g_Info.fd.used;
        for ( ; cmp < end ; cmp++ )                  /** ��Ѱ�߼��������ص� **/
            if ( cmp->stCyl > cmp->endCyl || Out_of_ext(cmp) ||
                 Find_Overlap(cmp, end) || Is_Extended(cmp->old_id) )
            {
                cmp->problem = TRUE;
                errFlag = TRUE;                                /** �����ص� **/
            } /* end if */
    } /* end if */
 
    g_Info.fd.canSaveFlag = ( errFlag ) ? FALSE : TRUE;
 
    return( errFlag );
} /* end Find_Problem */


/*========================== �� �� �� �� �� �� ==============================*/
static signed int Find_Overlap (PARTN *ptr, PARTN *end)
{
    PARTN  *cmp = ptr++;

/*  if ( cmp->endCyl > CUR_DK->lgeo.maxCyl )
        return( TRUE );
*/
    for ( ; ptr < end ; ptr++ )
        if ( cmp->endCyl >= ptr->stCyl )
            return( TRUE );                                    /** �����ص� **/
    return( FALSE );
} /* end Find_Overlap */

/*======================= �� �� Ϊ �� �� �� �� �� �� ========================*/
signed int IsHiddenPartn (PARTN *ptr)
{
    return( IsHiddenID(ptr->id) );
} /* end IsHiddenPartn */

/*==================== �� �� �� �� �� �� �� �� �� �� Ч =====================*/
signed int IsValid (DiskDT *dk)
{
    return( dk && (dk->flag & VALID_DISKDT) );
}  /* end IsValid */
/*--------------------------- Is Extended sysID -----------------------------*/
signed int Is_Extended (unsigned char id)
{
    return( ChkSysID(id, EXTENDED_ID) );
} /* end Is_Extended */
/*---------------------- �� �� Ϊ DOS �� �� �� ϵ ͳ ID ---------------------*/
signed int IsHiddenWinID (signed int id)
{
    return( ChkSysID(id, HIDD_ID | NT_FAT12_16_32) );
} /* end IsHiddenWinID */


/*----------------------- �� �� Ϊ �� �� �� ϵ ͳ ID ------------------------*/
/* added for dos hidden ID by SPF, 2002.05.01 */
signed int IsHiddenID (signed int id)
{
    return( ChkSysID(id, HIDD_ID) || IsHiddenWinID(id) );
} /* end IsHiddenID */

/*------------ �� �� Ϊ DOS�� OS/2�� NT �� DOS ���ص�ϵͳ�ɣ� ----------------+
| ������                                                                      |
|     id      System ID                                                       |
|     cond    FAT12_16 / NT_ID / LBA_FAT / EXTENDED_ID                        |
+-----------------------------------------------------------------------------+
|     ���Ҫ��� hidden id�� cond ���� HIDD_ID �� or �����ٴ���               |
+----------------------------------------------------------------------------*/
signed int ChkSysID (unsigned char id, signed int cond)
{
    signed int  ret = 0;

    if ( cond & HIDD_ID )
    {
        if ( (cond == HIDD_ID) && (id == 0x26) )
            ret = HIDD_ID;
        else if ( (cond & FAT12_16) && ((id == 0x11) || (id == 0x14) || (id == 0x16)) )
            ret = HIDD_ID | FAT12_16;
        else if ( (cond & LBA_FAT) && ((id == 0x1b) || (id == 0x1c) || (id == 0x1e)) )
            ret = HIDD_ID | LBA_FAT;
        else if ( (cond & NT_ID) && (id == 0x17) )
            ret = HIDD_ID | NT_ID;
    }
    else
    {
        if ( (cond & EXTENDED_ID) && ((id == 0x05) || (id == 0x0f) || (id == 0x85)) )
            ret = EXTENDED_ID;
        else if ( (cond & FAT12_16) && ((id == 0x01) || (id == 0x04) || (id == 0x06)) )
            ret = FAT12_16;
        else if ( (cond & NT_ID) && (id == 0x07) )
            ret = NT_ID;
        else if ( (cond & LBA_FAT) && ((id == 0x0b) || (id == 0x0c) || (id == 0x0e)) )
            ret = LBA_FAT;
    } /* end if */

    return( ret );
} /* end ChkSysID */

/*=========================== LBA ��ַд�빤��Ӳ�� ============================*/
signed int linear_WriteWDK (unsigned long linearSect, unsigned long nSects, void *buff)
{
    return( CUR_DK->drv->linearWrite(CUR_DK, linearSect, nSects, buff) );
} /* end linear_WriteWDK */


/*=========================== LBA ��ַ��ȡ����Ӳ�� ============================*/
signed int linear_ReadWDK (unsigned long linearSect, unsigned long nSects, void *buff)
{
    return( CUR_DK->drv->linearRead(CUR_DK, linearSect, nSects, buff) );
} /* end linear_ReadWDK */


/*=========================== CHS ��ַд�빤��Ӳ�� ============================*/
signed int chs_WriteWDK (unsigned long head, unsigned long cyl, unsigned long sect, unsigned long nSects, void *buff)
                              
{
    return( CUR_DK->drv->chsWrite(CUR_DK, head, cyl, sect, nSects, buff) );
} /* end ChsWriteWDK */


/*=========================== CHS ��ַ��ȡ����Ӳ�� ============================*/
signed int chs_ReadWDK (unsigned long head, unsigned long cyl, unsigned long sect, unsigned long nSects, void *buff)
{      
    return( CUR_DK->drv->chsRead(CUR_DK, head, cyl, sect, nSects, buff) );
} /* end ChsReadeWDK */


/*=========================== CHS ��ַ���鹤��Ӳ�� ============================*/
signed int chs_CheckWDK (unsigned long head, unsigned long cyl, unsigned long sect, unsigned long nSects, void *buff)
{
    return( CUR_DK->drv->chsCheck(CUR_DK, head, cyl, sect, nSects, buff) );
} /* end chs_CheckWDK */

/*========================== �� �� �� ʼ �� �� ==============================*/
unsigned long GetStart (PARTN *ptr)
{
    return( CHS_to_Sect(ptr->stCyl, ptr->side, ptr->sector, CUR_DK) );
} /* end GetStart */


/*========================== �� �� �� �� �� �� ==============================*/
unsigned long GetEnd (PARTN *ptr)
{
    return( CHS_to_Sect(ptr->endCyl, CUR_DK->lgeo.maxHead,
                        CUR_DK->lgeo.maxSect, CUR_DK) );
} /* end GetEnd */


/*=================== �� �� ʼ �� �� �� �� �� �� �� �� �� ===================*/
static signed int ForStartCyl (const void *elem1, const void *elem2)
{
    const PARTN *e1 = (const PARTN *)elem1;
    const PARTN *e2 = (const PARTN *)elem2;
 
    return( (e1->stCyl < e2->stCyl) ? -1 :
            (e1->stCyl > e2->stCyl) ?  1 : 0 );
} /* end fcmp */


/*=================== �� �� �� �� �� �� �� �� �� �� �� �� ===================*/
static signed int ForNO (const void *elem1, const void *elem2)
{
    const PARTN *e1 = (const PARTN *)elem1;
    const PARTN *e2 = (const PARTN *)elem2;
 
    return( (e1->id == 0) ? 1 :
            (e2->id == 0) ? 0 : e1->no - e2->no );
} /* end ForNO */


/*============================== ������������ ===============================*/
void Sort_Partn (PARTN *partnArr, signed int kind)
{
    signed int (*fcmp)(const void *elem1, const void *elem2);
 
    fcmp = ( kind == FOR_STCYL ) ? ForStartCyl : ForNO;
 
    /*---------- sort primary partition ----------*/
    Select_Sort(partnArr, g_Info.fd.pri_use, sizeof(PARTN), fcmp);
 
    /*---------- sort logic partition ----------*/
    Select_Sort(partnArr + g_Info.fd.pri_use, 
                g_Info.fd.used - g_Info.fd.pri_use, sizeof(PARTN), fcmp);
} /* end Sort_Partn */

/*=============================== Selection sort ===========================*/
void Select_Sort (void *base, signed int elem_num, signed int elem_size,
                  signed int (*fcmp)(const void *elem1, const void *elem2))
{
    unsigned char   *curr, *walk, *min;
    signed int  i, j;

    curr = (unsigned char *)base;
    for ( i = 0 ; i < elem_num - 1 ; i++ )
    {
        min  = curr;
        walk = curr + elem_size;

        /*----------------- find minmum element -----------------*/
        for ( j = i + 1 ; j < elem_num ; j++ )
        {
            if ( (*fcmp)(min, walk) > 0 )
                min = walk;
            walk += elem_size;
        } /* end for */

        /*---------- swap element ----------*/
        if ( min != curr )
            Swap_Element(min, curr, elem_size);
             
        curr += elem_size;
    } /* end for */
} /* end Select_Sort */
/*============================== Swap two element ===========================*/
static void Swap_Element (void *elem1, void *elem2, signed int elem_size)
{
    unsigned char   *e1 = (unsigned char *)elem1;
    unsigned char   *e2 = (unsigned char *)elem2;
    unsigned char   tmp;

    while ( elem_size-- > 0 )
    {
        tmp   = *e1;
        *e1++ = *e2;
        *e2++ = tmp;
    } /* end while */
} /* end Swap_Element */

/**/
/*============================= �� Ѱ �� �� =================================*/
/*       ������֮��������  id < 4        δ����֮��������  id == 0           */
/*       ������֮�߼������� id > 4       δ����֮�߼������� id == -1         */
/*       ��������ʼ����Ϊ��ֵ����󣬲ſ�ʹ�ñ���ʽ���������ý��������ȷ    */
/*===========================================================================*/
void Find_space (PARTN *partnArr)
{       
    PARTN  *ptr, *pre;
    signed int  i;
 		
    if ( g_Info.fd.used >= MAX_PARTN_ELEM )			/*����������������(һ�㲻���ܳ����������)*/
        return;
 
    if ( g_Info.fd.primary == 0 )								/*û�з���,ȫ��Ӳ�̶�����*/
        Ins_elem(partnArr, 0, 0, CUR_DK->lgeo.maxCyl);
    else
    {
    		/*1 ��һ��������(������Ϊ��չ����)֮ǰ�Ƿ���ʣ��ռ�*/
        ptr = partnArr;
        if ( ptr->stCyl > 0 && (!Is_Extended(ptr->id) || ptr->stCyl > 1) )
            Ins_elem(partnArr, 0, 0, ptr->stCyl - 1);
         
  			/*2 �������֮���Ƿ���ʣ��ռ�*/
        pre = ptr++;
        for ( i = 1 ; i < g_Info.fd.pri_use ; i++,  ptr++,  pre++ )
            if ( ptr->stCyl > (pre->endCyl + 1) )
                Ins_elem(partnArr, i, pre->endCyl + 1, ptr->stCyl - 1);
         
               
  			/*3 ���һ������֮���Ƿ�����չ����*/
        if ( pre->endCyl < CUR_DK->lgeo.maxCyl )			/* ����1TӲ�̵�ʱ����������ڴ˴� */
        {
        		/*LB_FLAG1=1;*/
            Ins_elem(partnArr, i, pre->endCyl + 1, CUR_DK->lgeo.maxCyl); 
         }
            
  			/*4 ����չ�������д���*/
        i = g_Info.fd.pri_use;
        if ( g_Info.fd.ext_head != -1 )
        {
            if ( g_Info.fd.logic_exist == FALSE )
                Ins_elem(partnArr, i, g_Info.fd.ext_head, g_Info.fd.ext_end);
            else
            {
                ptr = partnArr + i;
                if ( !Out_of_ext(ptr) && (ptr->stCyl > (g_Info.fd.ext_head + 1)) )
                    Ins_elem(partnArr, i, g_Info.fd.ext_head, ptr->stCyl - 1);

                pre = ptr++;
                for ( ++i ; i < g_Info.fd.used ; i++,  ptr++,  pre++ )
                    if ( !Out_of_ext(ptr) && (ptr->stCyl > (pre->endCyl + 1)) )
                        Ins_elem(partnArr, i, pre->endCyl + 1, ptr->stCyl - 1);
    
                if ( !Out_of_ext(pre) && pre->endCyl < g_Info.fd.ext_end )
                    Ins_elem(partnArr, i, pre->endCyl + 1, g_Info.fd.ext_end);
            } /* end if */
        } /* end if */
    } /* end if */
    		
} /* end Find_space */
/*==================== �� �� �� �� �� �� �� �� extended �� Χ ===============*/
signed int Out_of_ext (PARTN *ptr)
{
    return ( g_Info.fd.ext_head == -1 ||
             ptr->stCyl < g_Info.fd.ext_head || ptr->endCyl > g_Info.fd.ext_end ||
             ptr->stCyl > g_Info.fd.ext_end || ptr->endCyl < g_Info.fd.ext_head );
} /* end Out_of_ext */


/*============================ �� �� �� �� Ԫ �� ============================*/
void Ins_elem (PARTN *partnArr, signed int ndx, unsigned long begin, unsigned long end)
{ 
		unsigned int i;
		unsigned char *ptr_tmp;
    PARTN *ptr = partnArr + ndx;							/*ָ����Ҫ����ķ���*/
 
    if ( g_Info.fd.used >= MAX_PARTN_ELEM )
       return;  
    CopyMem(&partnArr[ndx + 1], &partnArr[ndx],
            sizeof(PARTN) * (MAX_PARTN_ELEM - 1 - ndx) );	/*�����ݺ���һ������Ԫ��*/
    /*        
 		if(LB_FLAG1==1)			/*lbdebug*//*
 		{
 			if(LB_FLAG==1)
 			{
 				lbdebug(partnArr);
 				printf("ndx=%d,begin=%ld,end=%ld",ndx,begin,end);
 				exit(0);
 			}
 		}*/
    /*Init_Partn_Elem(ptr);  */                           /** ���� ptr ��ָ���� **/
    ptr_tmp=(unsigned char *)(ptr);
    for(i=0;i<sizeof(PARTN);i++)
    	ptr_tmp[i]=0;
     ptr->sector = 1;
     
    ptr->stCyl  = begin;
    ptr->endCyl = end;
    g_Info.fd.used++;
 
    if ( begin >= g_Info.fd.ext_head && end <= g_Info.fd.ext_end )
       ptr->no = -1;                                       /** ����������� **/
    else
    {
        ptr->no = 0;                                         /** ���������� **/
        g_Info.fd.pri_use++;
    } /* end if */
} /* end Ins_elem */
/*======================== ���鿽��(���Զ��ж��Ƿ��ص�) =====================*/
void CopyMem (void *target, const void *source, unsigned int size)
{
    unsigned char  *sptr = (unsigned char *)source;
    unsigned char  *tptr = (unsigned char *)target;
 
    if ( sptr != tptr )
    {
        if ( (tptr > sptr) && (tptr < (sptr + size)) )         /** �ռ��ص� **/
        {
            sptr += size;
            tptr += size;
            while ( --sptr >= (unsigned char *)source )                 /** ���򿽱� **/
                *(--tptr) = *sptr;
        }
        else
        {
            for ( ; size > 0 ; size-- )
               *tptr++ = *sptr++;
        } /* end if */
    } /* end if */
} /* end CopyMem */

unsigned long Get_nSector (PARTN *ptr)
{
   unsigned long begin = GetStart(ptr);
   unsigned long end   = GetEnd(ptr);

   return( ((end > begin) ? end - begin : begin - end) + 1 );
} /* end compute_sects */