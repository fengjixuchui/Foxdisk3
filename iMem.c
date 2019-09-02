/* iMem.c */
#include "iMem.h"

/* �ڴ濽�� */
/*ע�⣺���ܿ�������64K���ڴ� */
void * imemcpy(void * dest,const void *src,unsigned int count) 
{ 
	unsigned char *tmp = (unsigned char *) dest, *s = (unsigned char *) src; 

	while (count--) 
  	*tmp++ = *s++; 

	return dest; 
} 

/* �ڴ�Ƚ� */
int imemcmp(const void * cs,const void * ct,unsigned int count) 
{ 
	const unsigned char *su1, *su2; 
	int res = 0; 

	for( su1 = (unsigned char *)cs, su2 = (unsigned char *)ct; 0 < count; ++su1, ++su2, count--) 
		if ((res = *su1 - *su2) != 0) 
			break; 

	return res; 
} 
/* �ڴ���� */
void * imemset(void * s,int ch,unsigned int count) 
{ 
	unsigned char *xs = (unsigned char *) s; 
	while (count--) 
  	*xs++ = ((unsigned char)(ch)); 
	return s; 
} 


