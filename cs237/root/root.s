# A method to generate the string of bits representing the root of n.
# (c) 2017 Qianwen (Tiffany) Zheng
	.text
	.globl	root
	.type	root,@function

#Frame Layout:

# m -28
# r -24
# v -20
# i -16
# n -12
# bits -8
# oldfp +0
# return +8

# Frame equates (frame size is 28):
	.equ	bits,-8
	.equ	n,-12
	.equ	i,-16
	.equ	v,-20
	.equ	r,-24
	.equ	m,-28
	
root:
	pushq	%rbp			#caller's old frame pointer
	movq	%rsp,%rbp		#set frame pointer to %rsp
	subq	$32,%rsp		#move stack pointer to end of frame
	movl	%edi,n(%rbp)		#set first parameter value
	movq	%rsi,bits(%rbp)		#set second parameter value

	movl	$0,m(%rbp)		#int m = 0
	movl	$0,r(%rbp)		#int r = 0
	movl	$0,i(%rbp)		#i=0 (start of for loop)
loop:	movl	i(%rbp),%ebx		#store i in %ebx
	cmpl	$4,%ebx			#i<4
	jge	overl			#if i >= 4

	movl	m(%rbp),%edi		#store m in %edi
	sall	$2,%edi			#m<<2
	movl	n(%rbp),%esi		#store n in %esi
	sarl	$6,%esi			#n>>6
	addl	%edi,%esi		#(m<<2)+(n>>6)
	movl	%esi,v(%rbp)		#v=(m<<2)+(n>>6)
	
	movl	n(%rbp),%edi		#store n in %edi
	sall	$2,%edi			#n<<2
	andl	$0xff,%edi		#(n<<2)&0xff
	movl	%edi,n(%rbp)		#n=(n<<2)&0xff
	
	movl	r(%rbp),%edi		#store r in %edi
	sall	$2,%edi			#r<<2
	addl	$1,%edi			#(r<<2)+1
	movl	v(%rbp),%esi		#store v in %esi
	subl	%edi,%esi		#v-((r<<2)+1)
	movl	%esi,m(%rbp)		#m=v-((r<<2)+1)

	cmpl	$0,%esi			#if m<0
	jge	elseP			#if m>=0
	
	movq	bits(%rbp),%rax		#store bits[] in %rax
	movl	i(%rbp),%edi		#store i in %edi
	movb	$48,%cl			#store '0' in %cl
	movb	%cl,0(%rax,%rdi,1)	#bits[i]='0' 

	movl	v(%rbp),%edi		#store v in %edi
	movl	%edi,m(%rbp)		#m=v

	movl	r(%rbp),%esi		#store r in %esi
	sall	$1,%esi			#r<<1
	movl	%esi,r(%rbp)		#r<<=1

	incl	%ebx			#increment i by 1
	movl	%ebx,i(%rbp)		#++i
	jmp	loop			#go through the loop again

elseP:	movq	bits(%rbp),%rax		#store bits[] in %rax
	movl	i(%rbp),%edi		#store i in %edi
	movb	$49,%cl			#store '1' in %cl
	movb	%cl,0(%rax,%rdi,1)	#bits[i]='1'

	movl	r(%rbp),%esi		#store r in %esi
	sall	$1,%esi			#r<<1
	addl	$1,%esi			#(r<<1) + 1
	movl	%esi,r(%rbp)		#r=(r<<1) + 1
	
	incl	%ebx			#increment i by 1
	movl	%ebx,i(%rbp)		#++i
	jmp	loop			#go through loop again

overl:	movq	bits(%rbp),%rax		#store bits[] in %rax
	movl	i(%rbp),%edi		#store i in %edi
	movb	$0,%cl			#store 0 in %cl
	movb	%cl,0(%rax,%rdi,1)	#bits[i]=0

	movq	%rbp,%rsp		#set stack pointer to %rbp
	popq	%rbp			#pop the base pointer off top of stack
	ret
	.size	root,.-root

