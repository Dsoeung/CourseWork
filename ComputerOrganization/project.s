	.file	"project.c"
	.text
	.globl	power
	.type	power, @function
power:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp		#Local variables
	movl	$1, -4(%ebp)	#1 = for iterator?(for lack of a better term);
	movl	$1, -8(%ebp)	#1 = x or result; initalized for x^0
	cmpl	$0, 12(%ebp)	#cmp n(size) to 0
	je	.L2					#jump L2/end if size=0
	movl	$1, -4(%ebp)	#initialize iterator
	jmp	.L3
.L4:
	movl	-8(%ebp), %eax	#result -> eax
	imull	8(%ebp), %eax	#multiply x*result
	movl	%eax, -8(%ebp)	#move eax into result
	addl	$1, -4(%ebp)	#iterate for loop
.L3:
	movl	-4(%ebp), %eax	#iterator to eax
	cmpl	12(%ebp), %eax	#compare iterator to n, max size
	jle	.L4
.L2:
	movl	-8(%ebp), %eax	#result = eax
	leave
	ret
	.size	power, .-power
	.globl	fillarray
	.type	fillarray, @function
fillarray:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp		#Local variables
	movl	$0, -4(%ebp)	#iterator init
	movl	8(%ebp), %eax	#x/result/constant = eax
	movl	%eax, -8(%ebp)	#eax -> local variable
	movl	$0, -4(%ebp)	#iterator
	jmp	.L7
.L8:
	movl	-4(%ebp), %eax	#iterator to eax
	movl	%eax, 4(%esp)	#iterator/eax to stack, arg2/power
	movl	8(%ebp), %eax	#x to eax
	movl	%eax, (%esp)	#x to arg1/base
	call	power			#power(x,iterator)
	movl	%eax, -8(%ebp)	#x to local
	movl	-4(%ebp), %eax	#iterator to eax
	sall	$2, %eax		#shift through array
	movl	%eax, %edx		#(4*eax) to edx
	addl	12(%ebp), %edx	#array iterated by (4*eax)
	movl	-8(%ebp), %eax	#x to eax
	movl	%eax, (%edx)	#eax value -> array
	addl	$1, -4(%ebp)	#iterator++
.L7:
	movl	-4(%ebp), %eax	#iterator to eax
	cmpl	16(%ebp), %eax	#iterator to n
	jl	.L8
	leave
	ret
	.size	fillarray, .-fillarray
.globl	fillarray2
	.type	fillarray2, @function
fillarray2:
	pushl	%ebp
	movl	%esp, %ebp	
	subl	$16, %esp		#Local variables
	movl	$1, -4(%ebp)	#outer iterator
	movl	$0, -8(%ebp)	#inner iterator
	movl	$1, -12(%ebp)	#moving power value/constant/result/x
	movl	$0, -4(%ebp)	#start outer iterator
	jmp	.L11				#done
.L14:
	movl	$0, -8(%ebp)	#outer loop
	jmp	.L12
.L13:
	movl	-12(%ebp), %eax	#x to eax
	imull	8(%ebp), %eax	#multiply eax*x
	movl	%eax, -12(%ebp)	#move eax*x to x
	addl	$1, -8(%ebp)	#inner iterator++
.L12:
	movl	-8(%ebp), %eax	#inner iterator
	cmpl	-4(%ebp), %eax	#compare outer iterator with inner iterator,
	jl	.L13				
	movl	-4(%ebp), %eax	#outer to eax
	sall	$2, %eax		#outerIt*2
	movl	%eax, %edx		#outerIt*2 to edx
	addl	12(%ebp), %edx	#a[] + outerIt*2	iterating through array
	movl	-12(%ebp), %eax	#x to eax
	movl	%eax, (%edx)	#x to (array)
	movl	$1, -12(%ebp)	#x = 1 in case of x^0
	addl	$1, -4(%ebp)	#iterate outer++
.L11:						#done
	movl	-4(%ebp), %eax
	cmpl	16(%ebp), %eax
	jle	.L14				#outer loop
	leave
	ret
	.size	fillarray2, .-fillarray2
.globl compare
	.type	compare, @function
compare:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$20, %esp		#Local variables
	movl	$0, -8(%ebp)	#0 to for loop
	jmp	.L26
.L29:
	movl	-8(%ebp), %eax	
	sall	$2, %eax		#iterate through array
	addl	8(%ebp), %eax	#iterate through a[]
	movl	(%eax), %edx	
	movl	-8(%ebp), %eax
	sall	$2, %eax
	addl	12(%ebp), %eax	#iterate through b[]
	movl	(%eax), %eax	
	cmpl	%eax, %edx		
	je	.L27
	movl	$0, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, -20(%ebp)
	jmp	.L28
.L27:
	addl	$1, -8(%ebp)	#iterator++
.L26:
	movl	-8(%ebp), %eax	
	cmpl	16(%ebp), %eax
	jl	.L29
	movl	$1, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, -20(%ebp)
.L28:
	movl	-20(%ebp), %eax
	leave
	ret
	.size	compare, .-compare
	.section	.rodata
.LC0:
	.string	"fillarray(2,a,10) correct"
.LC1:
	.string	"fillarray(2,a,10) incorrect"
.LC2:
	.string	"fillarray2(2,b,10) correct"
.LC3:
	.string	"fillarray2(2,b,10) incorrect"
.LC4:
	.string	"fillarray(3,a,10) correct"
.LC5:
	.string	"fillarray(3,a,10) incorrect"
.LC6:
	.string	"fillarray2(3,b,10) correct"
.LC7:
	.string	"fillarray2(3,b,10) incorrect"
	.text
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$192, %esp
	movl	$2, 28(%esp)
	movl	$1, 112(%esp)
	movl	$2, 116(%esp)
	movl	$4, 120(%esp)
	movl	$8, 124(%esp)
	movl	$16, 128(%esp)
	movl	$32, 132(%esp)
	movl	$64, 136(%esp)
	movl	$128, 140(%esp)
	movl	$256, 144(%esp)
	movl	$512, 148(%esp)
	movl	$1, 152(%esp)
	movl	$3, 156(%esp)
	movl	$9, 160(%esp)
	movl	$27, 164(%esp)
	movl	$81, 168(%esp)
	movl	$243, 172(%esp)
	movl	$729, 176(%esp)
	movl	$2187, 180(%esp)
	movl	$6561, 184(%esp)
	movl	$19683, 188(%esp)
	movl	$10, 8(%esp)
	leal	32(%esp), %eax
	movl	%eax, 4(%esp)
	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	fillarray
	movl	$10, 8(%esp)
	leal	112(%esp), %eax
	movl	%eax, 4(%esp)
	leal	32(%esp), %eax
	movl	%eax, (%esp)
	call	compare
	cmpl	$1, %eax
	jne	.L17
	movl	$.LC0, (%esp)
	call	puts
	jmp	.L18
.L17:
	movl	$.LC1, (%esp)
	call	puts
.L18:
	movl	$10, 8(%esp)
	leal	72(%esp), %eax
	movl	%eax, 4(%esp)
	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	fillarray2
	movl	$10, 8(%esp)
	leal	112(%esp), %eax
	movl	%eax, 4(%esp)
	leal	72(%esp), %eax
	movl	%eax, (%esp)
	call	compare
	cmpl	$1, %eax
	jne	.L19
	movl	$.LC2, (%esp)
	call	puts
	jmp	.L20
.L19:
	movl	$.LC3, (%esp)
	call	puts
.L20:
	movl	$3, 28(%esp)
	movl	$10, 8(%esp)
	leal	32(%esp), %eax
	movl	%eax, 4(%esp)
	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	fillarray
	movl	$10, 8(%esp)
	leal	152(%esp), %eax
	movl	%eax, 4(%esp)
	leal	32(%esp), %eax
	movl	%eax, (%esp)
	call	compare
	cmpl	$1, %eax
	jne	.L21
	movl	$.LC4, (%esp)
	call	puts
	jmp	.L22
.L21:
	movl	$.LC5, (%esp)
	call	puts
.L22:
	movl	$10, 8(%esp)
	leal	72(%esp), %eax
	movl	%eax, 4(%esp)
	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	fillarray2
	movl	$10, 8(%esp)
	leal	152(%esp), %eax
	movl	%eax, 4(%esp)
	leal	72(%esp), %eax
	movl	%eax, (%esp)
	call	compare
	cmpl	$1, %eax
	jne	.L23
	movl	$.LC6, (%esp)
	call	puts
	jmp	.L25
.L23:
	movl	$.LC7, (%esp)
	call	puts
.L25:
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.4) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
