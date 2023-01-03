	.file	"main.c"
	.text
	.globl	swap
	.type	swap, @function
swap:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	12(%ebp), %eax
	movl	(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	12(%ebp), %eax
	movl	-4(%ebp), %edx
	movl	%edx, (%eax)
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	swap, .-swap
	.globl	bubble_sort
	.type	bubble_sort, @function
bubble_sort:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	12(%ebp), %eax
	subl	$4, %eax
	movl	%eax, -8(%ebp)
	jmp	.L3
.L7:
	movl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.L4
.L6:
	movl	-4(%ebp), %eax
	movl	(%eax), %edx
	movl	-4(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jle	.L5
	movl	-4(%ebp), %eax
	addl	$4, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	swap
	addl	$8, %esp
.L5:
	addl	$4, -4(%ebp)
.L4:
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	jne	.L6
	subl	$4, -8(%ebp)
.L3:
	movl	-8(%ebp), %eax
	cmpl	8(%ebp), %eax
	jne	.L7
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	bubble_sort, .-bubble_sort
	.section	.rodata
.LC0:
	.string	"%d"
	.text
	.globl	main
	.type	main, @function
main:
.LFB4:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x7c,0x6
	subl	$36, %esp
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	movl	$0, -36(%ebp)
	subl	$8, %esp
	leal	-36(%ebp), %eax
	pushl	%eax
	pushl	$.LC0
	call	__isoc99_scanf
	addl	$16, %esp
	movl	-36(%ebp), %eax
	sall	$2, %eax
	subl	$12, %esp
	pushl	%eax
	call	malloc
	addl	$16, %esp
	movl	%eax, -28(%ebp)
	movl	$0, -32(%ebp)
	jmp	.L9
.L10:
	movl	-32(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC0
	call	__isoc99_scanf
	addl	$16, %esp
	addl	$1, -32(%ebp)
.L9:
	movl	-36(%ebp), %eax
	cmpl	%eax, -32(%ebp)
	jl	.L10
	movl	-36(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	subl	$8, %esp
	pushl	%eax		//второй аргумент функции bubble_sort - конец массива передается через стек
	pushl	-28(%ebp)	//первый аргумент - начало массива - аналогично передается через стек
	call	bubble_sort	//вызов bubble_sort
	addl	$16, %esp	//возвращение позиции указателя вершины стека после работы функции
	movl	$0, -24(%ebp)	//обнуляем вторую половину(младшие разряды) 64-битного long long int итератора
	movl	$0, -20(%ebp)	//обнуляем первую половину(старшие разряды) 64-битного long long int итератора
	jmp	.L11
.L12:
	movl	-24(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-28(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	addl	$1, -24(%ebp)	//прибавляем 1 к младшему 32-битному слову 64-битного числа
	adcl	$0, -20(%ebp)	//если есть перенос, то прибавим его к старшему 32-битному слову 64-битного числа
.L11:
	movl	-36(%ebp), %eax	//size -> eax
	cltd			//eax -> представлеятся в виде двух слов, где в edx все биты заполнены знаковым битом
	cmpl	-20(%ebp), %edx	//сравниваем старшее 32-битное слово 64-битного числа с нулями(в нашем случае)
	jg	.L12
	cmpl	-20(%ebp), %edx
	jl	.L16
	cmpl	-24(%ebp), %eax //сравниваем младшее 32-битное слово 64-битного числа с size
	ja	.L12		//переход в тело цикла, если size оказался больше
.L16:
	movl	$0, %eax
	movl	-12(%ebp), %ecx
	xorl	%gs:20, %ecx
	je	.L15
	call	__stack_chk_fail
.L15:
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
