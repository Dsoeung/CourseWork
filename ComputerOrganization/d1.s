	.file	"d1.c"
	.text
.globl divide
	.type	divide, @function
divide:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	%edx, %eax
	sarl	$31, %edx
	idivl	12(%ebp)
	popl	%ebp
	ret
	.size	divide, .-divide
	.ident	"GCC: (Ubuntu 4.3.3-5ubuntu4) 4.3.3"
	.section	.note.GNU-stack,"",@progbits
