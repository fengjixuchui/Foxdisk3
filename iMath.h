#ifndef _IMATH_H
#define _IMATH_H
/* ====iMath==== */
extern unsigned long lintmul(unsigned long m1,unsigned long m2);		/* �����γ˷� */
extern unsigned long lintmod(unsigned long divisor,unsigned long dividend); /* ���� */
extern unsigned long lintdiv(unsigned long divisor,unsigned long dividend); /* ģ���� */

extern unsigned long lintshr(unsigned int n,unsigned long number);	/* ���� */
extern unsigned long lintshl(unsigned int n,unsigned long number);	/* ���� */
#endif