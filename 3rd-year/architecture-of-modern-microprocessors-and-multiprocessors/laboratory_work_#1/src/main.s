	.file	"main.c"
	.text
	.section	.rodata
	.align 8
.LC1:
	.string	"Processor is warmed up! result = %d\n"
	.align 8
.LC2:
	.string	"Amount of clocks per empty iteration: %lf\n"
	.align 8
.LC3:
	.string	"Amount of clocks per iteration with division: %lf, result = %d\n\n"
	.align 8
.LC4:
	.string	"Division clocks for independent operations: %lf\n\n"
	.align 8
.LC5:
	.string	"Division clocks for dependent operations: %lf\n\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB3923:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	$0, -32(%rbp)
	movq	$0, -24(%rbp)
	movl	$-2147483648, -48(%rbp)
	movl	$2, -36(%rbp)
	movl	$0, -44(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	movl	$0, %edi
	call	time@PLT
	movq	%rax, -8(%rbp)
	jmp	.L2
.L3:
	movl	-48(%rbp), %eax
	movl	$0, %edx
	divl	-36(%rbp)
	movl	%eax, -44(%rbp)
.L2:
	movl	$0, %edi
	call	time@PLT
	subq	-8(%rbp), %rax
	cmpq	$9, %rax
	jle	.L3
	movl	-44(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -32(%rbp)
#------------------------------- empty cycle -------------------------------
    movl    $0, %ecx
	jmp	.L5
.L6:
	addl	$1, %ecx
.L5:
	cmpl	$99999999, %ecx
	jle	.L6
    movl	%ecx, -40(%rbp)
#------------------------------- empty cycle -------------------------------
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	subq	-32(%rbp), %rax
	testq	%rax, %rax
	js	.L8
	cvtsi2sdq	%rax, %xmm0
	jmp	.L9
.L8:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L9:
	cvtsi2sd	-40(%rbp), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	leaq	.LC2(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -32(%rbp)
#------------------------------- independent operations -------------------------------
	movl	$0, %ecx
    movl	-48(%rbp), %edi
    movl    -36(%rbp), %ebx
	jmp	.L11
.L12:
	movl	%edi, %eax
    movl	$0, %edx
	divl	%ebx

	movl	%edi, %eax
    movl	$0, %edx
	divl	%ebx

	movl	%edi, %eax
    movl	$0, %edx
	divl	%ebx

	movl	%edi, %eax
    movl	$0, %edx
	divl	%ebx

	addl	$4, %ecx
.L11:
	cmpl	$2499999, %ecx
	jle	.L12
	movl	%eax, -44(%rbp)
    movl	%ecx, -40(%rbp)
#------------------------------- independent operations -------------------------------
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	subq	-32(%rbp), %rax
	testq	%rax, %rax
	js	.L14
	cvtsi2sdq	%rax, %xmm0
	jmp	.L15
.L14:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L15:
	cvtsi2sd	-40(%rbp), %xmm1
	divsd	%xmm1, %xmm0
	movl	-44(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movq	-24(%rbp), %rax
	subq	-32(%rbp), %rax
	testq	%rax, %rax
	js	.L16
	cvtsi2sdq	%rax, %xmm0
	jmp	.L17
.L16:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L17:
	cvtsi2sd	-40(%rbp), %xmm1
	divsd	%xmm1, %xmm0
	subsd	-16(%rbp), %xmm0
	leaq	.LC4(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movl	$-2147483648, -48(%rbp)
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -32(%rbp)
#------------------------------- dependent operations -------------------------------
	movl	$0, %ecx
    movl	-48(%rbp), %eax
    movl    -36(%rbp), %ebx
	jmp	.L19
.L20:
	movl	$0, %edx
	divl	%ebx

	movl	$0, %edx
	divl	%ebx

	movl	$0, %edx
	divl	%ebx

	movl	$0, %edx
	divl	%ebx

	addl	$4, %ecx
.L19:
	cmpl	$2499999, %ecx
	jle	.L20
    movl	%eax, -48(%rbp)
    movl	%ecx, -40(%rbp)
#------------------------------- dependent operations -------------------------------
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	subq	-32(%rbp), %rax
	testq	%rax, %rax
	js	.L22
	cvtsi2sdq	%rax, %xmm0
	jmp	.L23
.L22:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L23:
	cvtsi2sd	-40(%rbp), %xmm1
	divsd	%xmm1, %xmm0
	movl	-48(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movq	-24(%rbp), %rax
	subq	-32(%rbp), %rax
	testq	%rax, %rax
	js	.L24
	cvtsi2sdq	%rax, %xmm0
	jmp	.L25
.L24:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L25:
	cvtsi2sd	-40(%rbp), %xmm1
	divsd	%xmm1, %xmm0
	subsd	-16(%rbp), %xmm0
	leaq	.LC5(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3923:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
