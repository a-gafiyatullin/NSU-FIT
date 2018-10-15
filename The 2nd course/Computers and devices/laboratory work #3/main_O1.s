	.file	"main.c"
	.text
	.globl	swap
	.type	swap, @function
swap:
.LFB38:
	.cfi_startproc
	movl	(%rdi), %eax 	//не выделяется память на стеке при входе в функцию
	movl	(%rsi), %edx	//все операции копирования переменных и указателей происходят прямо на регистрах
	movl	%edx, (%rdi)	//не совершается огромное число бесполезных обращений к стеку
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
	subq	$4, %rsi
	cmpq	%rdi, %rsi
	jne	.L11
	rep ret
.L9:
	movl	(%rax), %edx	//swap замененен вставкой более оптимальной последовательности команд
	movl	4(%rax), %ecx
	cmpl	%ecx, %edx
	jle	.L5
	movl	%ecx, (%rax)
	movl	%edx, 4(%rax)
.L5:
	addq	$4, %rax
	cmpq	%rsi, %rax
	jne	.L9
.L7:
	subq	$4, %rsi
	cmpq	%rsi, %rdi
	je	.L2
.L11:
	movq	%rdi, %rax
	cmpq	%rsi, %rdi
	jne	.L9
	jmp	.L7
.L2:
	rep ret
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
	subq	$24, %rsp	//в rsp хранится начало выделенной памяти, обращения к стеку происходят в сторону роста адресов
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
	movl	4(%rsp), %eax
	testl	%eax, %eax	//сравнение size с нулем
	jle	.L13		//если size меньше или равен 0, то можно не пытаться входить в цикл
	movl	$0, %ebx
.L14:
	movslq	%ebx, %rax	//цикл со считыванием данных в массив организован намного компактнее
	leaq	0(%rbp,%rax,4), %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	addl	$1, %ebx
	movl	4(%rsp), %eax
	cmpl	%ebx, %eax
	jg	.L14
.L13:
	cltq
	leaq	0(%rbp,%rax,4), %rsi
	movq	%rbp, %rdi
	call	bubble_sort
	cmpl	$0, 4(%rsp)
	jle	.L15
	movl	$0, %ebx
.L16:
	movl	0(%rbp,%rbx,4), %edx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk	//вызывает защищенную версию printf
	addq	$1, %rbx
	movslq	4(%rsp), %rax
	cmpq	%rbx, %rax
	jg	.L16
.L15:
	movl	$0, %eax
	movq	8(%rsp), %rcx
	xorq	%fs:40, %rcx
	je	.L17
	call	__stack_chk_fail
.L17:
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
