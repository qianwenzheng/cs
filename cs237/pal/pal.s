;;; (c) Tiffany and Shanti !!!!!
;;; it does what it's supposed to do!!!!
	.requ	n,r1
	.requ	res,r2
	.requ	lbit,r3
	.requ	rbit,r4

loop1:	swi	#SysGetNum
	mov	n,r0
	cmp	n,#0
	blt	overwhile1
	cmp	n,#0
	blt	overif1
	cmp	n,#1023
	bgt	overif1
	b	isPal
isPal:	mov	res,#1
	cmp	n,#1
	ble	retRes
	mov	lbit,#512
	mov	rbit,#1
loop2:	mov	r5,n
	and	r5,r5,lbit
	cmp	r5,#0
	bne	overwhile2
	mov	lbit,lbit,lsr #1
	b	loop2
overwhile2:
loop3:	cmp	lbit,rbit
	ble	overwhile3
	mov	r6,lbit
	and	r6,r6,n
	cmp	r6,#0
	movne	r6,#1
	moveq	r6,#0
	mov	r7,rbit
	and	r7,r7,n
	cmp	r7,#0
	movne	r7,#1
	moveq	r7,#0
	cmp	r6,r7
	beq	overif2
	mov	res,#0
	b	retRes
overif2:
	mov	lbit,lbit,lsr #1
	mov	rbit,rbit,lsl #1
	b	loop3
overwhile3:
retRes:	cmp	res,#1
	bne	overif1
	mov	r0,n
	swi	#SysPutNum
	mov     r0,#'\n
	swi     #SysPutChar	
overif1:
	cmp	n,#0
	beq	overwhile1
	b	loop1
overwhile1:	
	swi     #SysHalt   

