	.file	"main.c"
	.text
	.globl	swap
	.type	swap, @function
swap:
.LFB38:
	.cfi_startproc
	movl	(%rdi), %eax
	movl	(%rsi), %edx
	movl	%edx, (%rdi)
	movl	%eax, (%rsi)
	ret
	.cfi_endproc
.LFE38:
	.size	swap, .-swap
	.globl	bubble_sort
	.type	bubble_sort, @function
bubble_sort:
.LFB39:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %r12
	leaq	-4(%rsi), %rbp
	jmp	.L3
.L5:
	movl	4(%rbx), %eax
	cmpl	%eax, (%rbx)
	jle	.L4
	leaq	4(%rbx), %rsi
	movq	%rbx, %rdi
	call	swap
.L4:
	addq	$4, %rbx
	jmp	.L6
.L7:
	movq	%r12, %rbx
.L6:
	cmpq	%rbx, %rbp
	jne	.L5
	subq	$4, %rbp
.L3:
	cmpq	%r12, %rbp
	jne	.L7
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE39:
	.size	bubble_sort, .-bubble_sort
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d"
	.text
	.globl	main
	.type	main, @function
main:
.LFB40:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	movl	$0, 4(%rsp)
	leaq	4(%rsp), %rsi
	movl	$.LC0, %edi
	call	__isoc99_scanf
	movslq	4(%rsp), %rdi
	salq	$2, %rdi
	call	malloc
	movq	%rax, %rbp
	movl	$0, %ebx
	jmp	.L10
.L11:
	movslq	%ebx, %rax
	leaq	0(%rbp,%rax,4), %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	addl	$1, %ebx
.L10:
	movl	4(%rsp), %eax
	cmpl	%eax, %ebx
	jl	.L11
	cltq
	leaq	0(%rbp,%rax,4), %rsi
	movq	%rbp, %rdi
	call	bubble_sort
	movl	$0, %ebx
	jmp	.L12
.L13:
	movl	0(%rbp,%rbx,4), %edx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	addq	$1, %rbx
.L12:
	movslq	4(%rsp), %rax
	cmpq	%rax, %rbx
	jl	.L13
	movl	$0, %eax
	movq	8(%rsp), %rcx
	xorq	%fs:40, %rcx
	je	.L14
	call	__stack_chk_fail
.L14:
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE40:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
