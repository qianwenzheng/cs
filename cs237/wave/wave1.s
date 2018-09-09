;;; A program to find a halt instruction in a WARM program.
	jmp	main
	
addstr:	.string	"add"
adcstr:	.string	"adc"
substr:	.string	"sub"
cmpstr:	.string	"cmp"
eorstr:	.string	"eor"
orrstr:	.string	"orr"
andstr:	.string	"and"
tststr:	.string	"tst"
mulstr:	.string	"mul"
mlastr:	.string	"mla"
divstr:	.string	"div"
movstr:	.string	"mov"
mvnstr:	.string	"mvn"
swistr:	.string	"swi"
ldmstr:	.string	"ldm"
stmstr:	.string	"stm"
ldrstr:	.string	"ldr"
strstr:	.string	"str"
ldustr:	.string	"ldu"
stustr:	.string	"stu"
adrstr:	.string	"adr"
bstr:	.string	"b"
blstr:	.string	"bl"
nullstr:.string	"BAD"
	
instab:	.data	addstr,adcstr,substr,cmpstr,eorstr, orrstr, andstr, tststr
	.data	mulstr,mlastr,divstr,movstr,mvnstr,swistr, ldmstr, stmstr
	.data	ldrstr,strstr,ldustr,stustr,adrstr, nullstr,nullstr,nullstr
	.data   bstr,  bstr,  blstr, blstr, nullstr,nullstr,nullstr,nullstr
	.requ	wpc,r10
	.requ	wir,r11
	.equ	SWIHALT,0x06800000
main:	lea	WARM,r0		;take the thing im pointing to in memory, get the addr of that thing and put that addr in r0, could do as mov $WARM,r0
	trap	$SysOverlay
	mov	$0,wpc
_loop:	mov	WARM(wpc),wir	;grab instruction of warm and move it into wir
	mov	wpc,r0
	trap	$SysPutNum
	mov	$':,r0
	trap	$SysPutChar
	mov	$'\t,r0
	trap	$SysPutChar
	mov	wir,r1
	shr	$23,r1
	and	$0x1f,r1
	mov	instab(r1),r0
	call	print
	call	nl
	cmp	$SWIHALT,wir
	je	_done
	add	$1,wpc
	jmp	_loop

_done:	  lea     hey,r0
	call    print
	mov     wpc,r0
	trap    $SysPutNum
	call    nl
	trap    $SysHalt
	trap	$SysHalt
	
;;; print a string pointed to by r0; trashes r1
print:	mov	r0,r1
_loop:	mov	(r1),r0
	cmp	$0,r0
	je	_done
	trap	$SysPutChar
	add	$1,r1
	jmp	_loop
_done:	ret
	
hey:	.string "Hey, I foung the #SysHalt at location "
	
;;; print a newline; trashes r0
nl:	mov	$'\n,r0
	trap	$SysPutChar
	ret

	;; the edge of WARM address space; write nothing below this line
WARM:	
