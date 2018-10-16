	.arch armv7-a
	.eabi_attribute 28, 1
	.fpu vfpv3-d16
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 4
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"main.c"
	.text
	.align	1
	.global	swap
	.syntax unified
	.thumb
	.thumb_func
	.type	swap, %function
swap:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0]
	ldr	r2, [r1]
	str	r2, [r0]
	str	r3, [r1]
	bx	lr
	.size	swap, .-swap
	.align	1
	.global	bubble_sort
	.syntax unified
	.thumb
	.thumb_func
	.type	bubble_sort, %function
bubble_sort:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
.L8:
	subs	r1, r1, #4
	cmp	r1, r0
	beq	.L10
	mov	r3, r0
.L5:
	ldm	r3, {r2, r4}
	adds	r3, r3, #4
	cmp	r2, r4
	itt	gt
	strgt	r4, [r3, #-4]
	strgt	r2, [r3]
	cmp	r1, r3
	bne	.L5
	b	.L8
.L10:
	pop	{r4, pc}
	.size	bubble_sort, .-bubble_sort
	.section	.text.startup,"ax",%progbits
	.align	1
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	add	r1, sp, #8
	ldr	r5, .L19
	movs	r4, #0
	ldr	r0, .L19+4
	str	r4, [r1, #-8]!
	ldr	r3, [r5]
	ldr	r6, .L19+4
	str	r3, [sp, #4]
	bl	__isoc99_scanf
	ldr	r0, [sp]
	lsls	r0, r0, #2
	bl	malloc
	mov	r8, r0
.L12:
	ldr	r1, [sp]
	cmp	r4, r1
	bge	.L17
	add	r1, r8, r4, lsl #2
	mov	r0, r6
	bl	__isoc99_scanf
	adds	r4, r4, #1
	b	.L12
.L17:
	add	r1, r8, r1, lsl #2
	mov	r0, r8
	bl	bubble_sort
	ldr	r4, .L19+4
	movs	r6, #0
	movs	r7, #0
.L14:
	ldr	r2, [sp]
	cmp	r6, r2
	asr	r3, r2, #31
	sbcs	r3, r7, r3
	bge	.L18
	lsls	r3, r6, #2
	mov	r1, r4
	movs	r0, #1
	ldr	r2, [r8, r3]
	bl	__printf_chk
	adds	r6, r6, #1
	adc	r7, r7, #0
	b	.L14
.L18:
	ldr	r2, [sp, #4]
	movs	r0, #0
	ldr	r3, [r5]
	cmp	r2, r3
	beq	.L16
	bl	__stack_chk_fail
.L16:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L20:
	.align	2
.L19:
	.word	__stack_chk_guard
	.word	.LC0
	.size	main, .-main
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"%d\000"
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
