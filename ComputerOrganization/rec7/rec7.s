	.file	"rec7.c"
	.text
	.globl	switchRow
	.type	switchRow, @function
switchRow:
	pushl	%ebp
	movl	%esp, %ebp
####################################
#   Insert termination condition
#   here. Assign return value then
#   execute jump condition
#	switchRow( array, length)
####################################
	pushl %ebx
	pushl %edi
	movl 8(%ebp), %ebx #ebx = A
	movl 12(%ebp), %ecx  #ecx = length
	movl $0, %edi		#edi = index
	cmpl %edi ,%ecx #index < len
	jge .L1
	
.L2:
####################################
#   Insert "value switch" conditions
#   here. Recursively call switch row
#   after value switch. If you used 
#   "push" don't forget to "pop" to 
#   maintain correct stack pointer
####################################
	movl  8(%ebp), %ebx
	leal (%ebx, %edi, 4), %ebx
	
	
	
	movl 12(%ebp), %eax
	pushl %eax
	
	
	
.L1:
	
	popl %edi
	popl %ebx
	
	movl    %ebp, %esp
    popl    %ebp
	ret
	.size	switchRow, .-switchRow
	.globl	switchArray
	.type	switchArray, @function
switchArray:
	pushl	%ebp
	movl	%esp, %ebp
####################################
#   Initialize index variable. Then
#   write the jump condition
####################################
	pushl %ebx
	pushl %edx
	pushl %esi
	movl 8(%ebp), %ebx
	movl 12(%ebp), %ecx
	movl 16(%ebp), %edx
	movl $0, %edi
	cmpl %ecx, %edi
	jge .L6
	

.L7:
####################################
#   Insert your call to switchRow
#   here. If you use a push 
#   instruction, don't forget to 
#   pop to maintain correct esp
#   value
####################################
	movl 16(%ebp), %eax
	pushl %eax
	movl 12(%ebp), %eax
	pushl %eax
	movl 8(%ebp), %eax
	pushl %eax
	call switchRow
	popl %eax
	popl %eax
	popl %eax
.L6:
####################################
#   Insert termination condition for
#   your for loop here
####################################
        movl    %ebp, %esp
        popl    %ebp
	ret
	.size	switchArray, .-switchArray
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.4) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
