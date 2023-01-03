	.file	"main.c"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.text
.LHOTB0:
	.p2align 4,,15
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
	.section	.text.unlikely
.LCOLDE0:
	.text
.LHOTE0:
	.section	.text.unlikely
.LCOLDB1:
	.text
.LHOTB1:
	.p2align 4,,15
	.globl	bubble_sort
	.type	bubble_sort, @function
bubble_sort:
.LFB39:
	.cfi_startproc
	subq	$4, %rsi
	cmpq	%rdi, %rsi
	je	.L2
	.p2align 4,,10
	.p2align 3
.L8:
	movq	%rdi, %rax
	.p2align 4,,10
	.p2align 3
.L5:
	movl	(%rax), %edx
	movl	4(%rax), %ecx
	cmpl	%ecx, %edx
	jle	.L4
	movl	%ecx, (%rax)
	movl	%edx, 4(%rax)
.L4:
	addq	$4, %rax
	cmpq	%rax, %rsi
	jne	.L5
	subq	$4, %rsi
	cmpq	%rsi, %rdi
	jne	.L8
.L2:
	rep ret
	.cfi_endproc
.LFE39:
	.size	bubble_sort, .-bubble_sort
	.section	.text.unlikely
.LCOLDE1:
	.text
.LHOTE1:
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"%d"
	.section	.text.unlikely
.LCOLDB3:
	.section	.text.startup,"ax",@progbits
.LHOTB3:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB40:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movl	$.LC2, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	subq	$16, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	leaq	4(%rsp), %rsi
	movl	$0, 4(%rsp)
	call	__isoc99_scanf
	movslq	4(%rsp), %rdi
	salq	$2, %rdi
	call	malloc
	movl	4(%rsp), %edi
	movq	%rax, %rbp
	testl	%edi, %edi
	jle	.L13
	movq	%rax, %r12
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L14:
	movq	%r12, %rsi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	__isoc99_scanf
	movl	4(%rsp), %edi
	addl	$1, %ebx
	addq	$4, %r12
	cmpl	%ebx, %edi
	jg	.L14
	movslq	%edi, %rax
	leaq	-4(%rbp,%rax,4), %rsi
	cmpq	%rsi, %rbp
	je	.L21
	.p2align 4,,10
	.p2align 3
.L20:
	movq	%rbp, %rax
	.p2align 4,,10
	.p2align 3
.L19:
	movl	(%rax), %edx
	movl	4(%rax), %ecx
	cmpl	%ecx, %edx
	jle	.L18
	movl	%ecx, (%rax)
	movl	%edx, 4(%rax)
.L18:
	addq	$4, %rax
	cmpq	%rsi, %rax
	jne	.L19
	subq	$4, %rsi
	cmpq	%rsi, %rbp
	jne	.L20
.L21:
	xorl	%ebx, %ebx
	testl	%edi, %edi
	jle	.L17
	.p2align 4,,10
	.p2align 3
.L24:
	movl	0(%rbp,%rbx,4), %edx
	xorl	%eax, %eax
	movl	$.LC2, %esi
	movl	$1, %edi
	addq	$1, %rbx
	call	__printf_chk
	movslq	4(%rsp), %rax
	cmpq	%rbx, %rax
	jg	.L24
.L17:
	xorl	%eax, %eax
	movq	8(%rsp), %rdi
	xorq	%fs:40, %rdi
	jne	.L32
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L13:
	.cfi_restore_state
	movslq	%edi, %rax
	leaq	-4(%rbp,%rax,4), %rsi
	cmpq	%rbp, %rsi
	jne	.L20
	jmp	.L17
.L32:
	call	__stack_chk_fail
	.cfi_endproc
.LFE40:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE3:
	.section	.text.startup
.LHOTE3:
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
