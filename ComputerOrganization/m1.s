	.file	"m1.c"
	.text
.globl multi
	.type	multi, @function
multi:
	pushl	%ebp
	movl	%esp, %ebp
	movl	12(%ebp), %eax
	imull	8(%ebp), %eax
	popl	%ebp
	ret
	.size	multi, .-multi
	.ident	"GCC: (Ubuntu 4.3.3-5ubuntu4) 4.3.3"
	.section	.note.GNU-stack,"",@progbits
