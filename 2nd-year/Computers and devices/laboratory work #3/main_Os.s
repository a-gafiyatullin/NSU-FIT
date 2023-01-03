	.file	"main.c"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.text
.LHOTB0:
	.globl	swap
	.type	swap, @function
swap:
.LFB20:
	.cfi_startproc
	movl	(%rdi), %eax
	movl	(%rsi), %edx
	movl	%edx, (%rdi)
	movl	%eax, (%rsi)
	ret
	.cfi_endproc
.LFE20:
	.size	swap, .-swap
	.section	.text.unlikely
.LCOLDE0:
	.text
.LHOTE0:
	.section	.text.unlikely
.LCOLDB1:
	.text
.LHOTB1:
	.globl	bubble_sort
	.type	bubble_sort, @function
bubble_sort:
.LFB21:
	.cfi_startproc
.L8:
	subq	$4, %rsi
	cmpq	%rdi, %rsi	//проверяем условие выхода из внешнего цикла
	je	.L10
	movq	%rdi, %rax
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
	jmp	.L8		//после полного прохода внутреннего цикла, возвращаемся обратно к началу функции bubble_sort, чтобы проверить условие выхода из внешнего цикла
.L10:
	ret
	.cfi_endproc
.LFE21:
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
	.globl	main
	.type	main, @function
main:
.LFB22:
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
	xorl	%ebx, %ebx
	subq	$16, %rsp
	.cfi_def_cfa_offset 48
	leaq	4(%rsp), %rsi
	movl	$0, 4(%rsp)
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	call	__isoc99_scanf
	movslq	4(%rsp), %rdi
	salq	$2, %rdi
	call	malloc
	movq	%rax, %rbp
	movq	%rax, %r12
.L12:
	movslq	4(%rsp), %rax
	cmpl	%eax, %ebx
	jge	.L18
	movq	%r12, %rsi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	__isoc99_scanf
	incl	%ebx		//инкременитруем итератор цикла с помощью унарной операции
	addq	$4, %r12
	jmp	.L12
.L18:
	leaq	0(%rbp,%rax,4), %rsi
	movq	%rbp, %rdi
	xorl	%ebx, %ebx
	call	bubble_sort
.L14:
	movslq	4(%rsp), %rax
	cmpq	%rax, %rbx
	jge	.L19
	movl	0(%rbp,%rbx,4), %edx
	movl	$.LC2, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	incq	%rbx
	call	__printf_chk
	jmp	.L14
.L19:
	xorl	%eax, %eax
	movq	8(%rsp), %rcx
	xorq	%fs:40, %rcx
	je	.L16
	call	__stack_chk_fail
.L16:
	addq	$16, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE22:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE3:
	.section	.text.startup
.LHOTE3:
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
