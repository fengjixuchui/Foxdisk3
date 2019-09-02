;Ϊ����ƽ̨�޹�,�����˳������������㺯��

.model small
.386p
.data
.code

public _lintmul
public _lintdiv
public _lintmod
public _lintshr     
public _lintshl
;unsigned long lintmul(unsigned long m1,unsigned long m2);
;��ڲ���:������������
;���ڲ���:�����ĳ˻�
;ע��:ֻ�ʺϳ˻�������һ�������������ֵ
_lintmul	proc	near
	push	bp
	mov	bp,sp
	;
	mov	eax,dword ptr [bp+4]
	mov	edx,dword ptr [bp+8]
	mul	edx
	mov	edx,eax
	shr	edx,16				;dx:ax���س�������
	;
	mov	sp,bp
	pop	bp
	ret
_lintmul	endp

;unsigned long lintdiv(unsigned long divisor,unsigned long dividend);
;��ڲ���:divisor:����   dividend:������
;���ڲ���:��������
_lintdiv	proc	near
	push	bp
	mov	bp,sp
	push	ecx
	;
	xor	edx,edx
	mov	eax,dword ptr [bp+8]		;������
	mov	ecx,dword ptr [bp+4]		;����
	div	ecx											;��->eax ,����->edx
	mov	edx,eax
	shr	edx,16				;dx:ax���س�������
	
	pop	ecx
	mov	sp,bp
	pop	bp
	ret
_lintdiv	endp

;unsigned long lintmod(unsigned long divisor,unsigned long dividend);
;��ڲ���:divisor:����   dividend:������
;���ڲ���:dividend mod divisor
_lintmod	proc	near
	push	bp
	mov	bp,sp
	push	ecx
	;
	xor	edx,edx
	mov	eax,dword ptr [bp+8]		;������
	mov	ecx,dword ptr [bp+4]		;����
	div	ecx											;��->eax ,����->edx
	mov	ax,dx
	shr	edx,16				;dx:ax���س�������
	;	
	pop	ecx
	mov	sp,bp
	pop	bp
	ret
_lintmod	endp

;unsigned long lintshr(unsigned int n,unsigned long number)
;��ڲ���: n:��λ�� number:Դ��
;���ڲ��� number >>n
_lintshr	proc	near
	push	bp
	mov bp,sp
	push	cx
	;
	mov eax,dword ptr [bp+6]
	mov cx,word ptr [bp+4]
	mov	ch,0
	shr eax,cl
	mov	edx,eax
	shr	edx,16				;dx:ax���س�������
	
	pop	cx
	mov	sp,bp
	pop	bp
	ret
_lintshr endp

;unsigned long lintshr(unsigned int n,unsigned long number)
;��ڲ���: n:��λ�� number:Դ��
;���ڲ��� number >>n
_lintshl	proc	near
	push	bp
	mov bp,sp
	push	cx
	;
	mov eax,dword ptr [bp+6]
	mov cx,word ptr [bp+4]
	mov	ch,0
	shl eax,cl
	mov	edx,eax
	shr	edx,16				;dx:ax���س�������
	
	pop	cx
	mov	sp,bp
	pop	bp
	ret
_lintshl endp

end