	.arch armv7-a
	.eabi_attribute 28, 1
	.fpu vfpv3-d16
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"main.c"
	.text
	.align	2
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
	.align	2
	.global	bubble_sort
	.syntax unified
	.thumb
	.thumb_func
	.type	bubble_sort, %function
bubble_sort:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	subs	r1, r1, #4
	cmp	r1, r0
	beq	.L12
	push	{r4}
.L6:
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
	subs	r1, r1, #4
	cmp	r0, r1
	bne	.L6
	ldr	r4, [sp], #4
.L12:
	bx	lr
	.size	bubble_sort, .-bubble_sort
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	movw	r4, #:lower16:__stack_chk_guard
	movt	r4, #:upper16:__stack_chk_guard
	sub	sp, sp, #8
	add	r1, sp, #8
	movs	r6, #0
	ldr	r3, [r4]
	movw	r0, #:lower16:.LC0
	str	r6, [r1, #-8]!
	movt	r0, #:upper16:.LC0
	str	r3, [sp, #4]
	bl	__isoc99_scanf
	ldr	r0, [sp]
	lsls	r0, r0, #2
	bl	malloc
	ldr	lr, [sp]
	cmp	lr, r6
	mov	r5, r0
	ble	.L15
	movw	r8, #:lower16:.LC0
	movt	r8, #:upper16:.LC0
	mov	r7, r0
.L16:
	mov	r1, r7
	mov	r0, r8
	bl	__isoc99_scanf
	ldr	lr, [sp]
	adds	r6, r6, #1
	adds	r7, r7, #4
	cmp	lr, r6
	bgt	.L16
	lsl	r0, lr, #2
	subs	r0, r0, #4
	add	r0, r0, r5
	cmp	r5, r0
	beq	.L23
.L22:
	mov	r3, r5
.L21:
	ldr	r2, [r3]
	ldr	r1, [r3, #4]
	cmp	r2, r1
	it	gt
	stmgt	r3, {r1, r2}
	adds	r3, r3, #4
	cmp	r3, r0
	bne	.L21
	subs	r0, r0, #4
	cmp	r5, r0
	bne	.L22
.L23:
	cmp	lr, #0
	ble	.L19
	movw	r8, #:lower16:.LC0
	subs	r5, r5, #4
	movt	r8, #:upper16:.LC0
	movs	r6, #0
	movs	r7, #0
.L24:
	adds	r6, r6, #1
	ldr	r2, [r5, #4]!
	mov	r1, r8
	mov	r0, #1
	adc	r7, r7, #0
	bl	__printf_chk
	ldr	r2, [sp]
	cmp	r6, r2
	asr	r3, r2, #31
	sbcs	r3, r7, r3
	blt	.L24
.L19:
	ldr	r2, [sp, #4]
	movs	r0, #0
	ldr	r3, [r4]
	cmp	r2, r3
	bne	.L33
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L15:
	lsl	r0, lr, #2
	subs	r0, r0, #4
	add	r0, r0, r5
	cmp	r5, r0
	bne	.L22
	b	.L19
.L33:
	bl	__stack_chk_fail
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"%d\000"
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
