#ifndef _FOXDISK_H
#define _FOXDISK_H


#define PARALBAERR 	0x01				/* �洢��Ҫ������Ϣ���������ƻ������޷����� */
#define PARAADRERR	0x02				/* ��ǰ�����Ĳ�����ŵ�ַ������б���ĵ�ַ��ƥ�� */
#define OSMSGERR		0x04				/* ϵͳ����Ϣ��Դ���ƻ� */
#define WPMSGERR		0x08				/* ��ֽ��ŵ���Ϣ���ƻ� */

extern void foxdisk(void);
extern int SavFoxPara(FOXPARA *para);
extern int SavOSMsg(FOXPARA *para,OS *os);
extern int i_memcmp(void *cs,void *ct,unsigned int count);
extern void	BootEntry(void);

#endif