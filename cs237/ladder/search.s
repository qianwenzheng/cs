# A translation of isWordC from ladder.c ; appears as isWord.
# (c) 2017 Qianwen (Tiffany) Zheng
# I know there's a segmentation fault around the area in the loop where things are being assigned to
# %rax but I can't seem to fix it (I tried changing multiple registers to no avail). At some point, after
# I moved dtn to %rax, the value for %rax is zero but I can't figure out what caused it. My guess was that 
# something was off in how I handled moving around mid when I added it to dict, but it still gave me the seg
# fault after I changed how I moved it. It goes through the loop twice and into the elseif branch, but on the
# third time around the loop, that's when the seg fault occurs. If it helps, the seg fault came after I fixed
# the issue of the word not being in the dictionary and then an infinite loop problem.
	
# Entry equates (entry size is 32)
	.equ	word,8
	.equ	que,16
	.equ	ladd,24
	.equ	seen,0
	
# Dict equates
	.equ	dict,8
	.equ	alloced,0
	.equ	count,16
	
	.text
	.globl	isWord
	.type	isWord,@function

# Frame layout:
	
# ans	-48
# ent	-40
# dif	-32
# mid	-28
# hi	-24
# lo	-20
# dtn	-16	
# w	-8
# oldfp	 +0
# return +8
	
# Frame equates (frame size is 48):
	.equ	w,-8
	.equ	dtn,-16
	.equ	lo,-20
	.equ	hi,-24
	.equ	mid,-28
	.equ	dif,-32
	.equ	ent,-40
	.equ	ans,-48
	
isWord:
	pushq 	%rbp			#caller's old frame pointer
	movq	%rsp,%rbp		#set frame pointer to %rsp
	subq	$48,%rsp		#move stack pointer to END of frame
	movq	%rdi,w(%rbp)		#set first parameter value
	movq	%rsi,dtn(%rbp)		#set second parameter value

	movq	dtn(%rbp),%rax		#set %rax to dtn		
	movl	count(%rax),%edi	#set %edi to count
	subl	$1,%edi			#subtract one from count
	movl	%edi,hi(%rbp)		#int hi=(dtn->count)-1

	movl	$0,lo(%rbp)		#int lo = 0
	movq 	$0,ans(%rbp)		#entry *ans=0
loop:	movl	lo(%rbp),%edi		#store lo in %edi
	movl	hi(%rbp),%esi		#store hi in %esi
	cmpl	%esi,%edi		#lo <= hi
	jg	overloop		#if lo > hi
	addl	%esi,%edi		#lo+hi 
	sarl	$1,%edi			#(lo+hi)/2
	movl	%edi,mid(%rbp)		#mid = (lo+hi)/2
	movq	dtn(%rbp),%rax		#set %rax to dtn
	movq	dict(%rax),%rdi		#set %rdi to dict
	movq	mid(%rbp),%rsi		#set %rsi to mid
	salq	$5,%rsi			#mid*32
	addq	%rsi,%rdi		#(dtn->dict)+mid 
	movq	%rdi,ent(%rbp)		#ent=(dtn->dict)+mid
	movq	w(%rbp),%rdi		#set first parameter of strcmp
	movq	ent(%rbp),%rax		#set %rax to ent
	movq	word(%rax),%rsi		#set second parameter of strcmp
	call	strcmp			#call strcmp(w,ent->word)
	movl	%eax,dif(%rbp)		#dif=strcmp(w,end->word)
	cmpl	$0,%eax			#dif < 0
	jg	elseif			#if dif > 0
	je	else			#if dif==0
	movl	mid(%rbp),%edi		#move mid into  %edi
	subl  	$1,%edi			#subtract one from %edi
	movl	%edi,hi(%rbp)		#hi=mid-1
	jmp	loop			#go through the loop again
elseif:	movl	mid(%rbp),%edi		#move mid into %edi
	addl	$1,%edi			#mid+1
	movl	%edi,lo(%rbp)		#lo=mid+1
	jmp 	loop			#go through the loop again
else:	movq	ent(%rbp),%rsi		#ans=ent
	movq	%rsi,ans(%rbp)		#ans=ent
	jmp	overloop		#break
overloop:	
	movq	ans(%rbp),%rax		#set %rax to the value of ans
	movq	%rbp,%rsp		#set stack pointer to %rbp
	popq	%rbp			#pop the base pointer off top of stack
	ret
	.size	isWord,.-isWord
