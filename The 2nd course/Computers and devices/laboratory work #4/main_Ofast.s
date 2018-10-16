	.arch armv7-a
	.eabi_attribute 28, 1
	.fpu vfpv3-d16
	.eabi_attribute 23, 1
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
	push	{r4, r5, r6, r7, r8, r9, lr}
	movw	r5, #:lower16:__stack_chk_guard
	movt	r5, #:upper16:__stack_chk_guard
	sub	sp, sp, #12
	add	r1, sp, #8
	mov	r8, #0
	ldr	r3, [r5]
	movw	r0, #:lower16:.LC0
	str	r8, [r1, #-8]!
	movt	r0, #:upper16:.LC0
	str	r3, [sp, #4]
	bl	__isoc99_scanf
	ldr	r7, [sp]
	lsls	r4, r7, #2
	mov	r0, r4
	bl	malloc
	cmp	r7, r8
	mov	r6, r0
	ble	.L15
	movw	r9, #:lower16:.LC0
	movt	r9, #:upper16:.LC0
	mov	r4, r8
	mov	r8, r0
.L16:
	mov	r1, r8
	mov	r0, r9
	bl	__isoc99_scanf
	ldr	r7, [sp]
	adds	r4, r4, #1
	add	r8, r8, #4
	cmp	r7, r4
	bgt	.L16
	lsls	r0, r7, #2
	subs	r0, r0, #4
	add	r0, r0, r6
	cmp	r6, r0
	beq	.L23
.L22:
	mov	r3, r6
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
	cmp	r6, r0
	bne	.L22
.L23:
	cmp	r7, #0
	ble	.L19
	movw	r4, #:lower16:.LC0
	subs	r6, r6, #4
	movt	r4, #:upper16:.LC0
	mov	r8, #0
	mov	r9, #0
.L24:
	adds	r8, r8, #1
	ldr	r2, [r6, #4]!
	mov	r1, r4
	mov	r0, #1
	adc	r9, r9, #0
	bl	__printf_chk
	ldr	r2, [sp]
	cmp	r8, r2
	asr	r3, r2, #31
	sbcs	r3, r9, r3
	blt	.L24
.L19:
	ldr	r2, [sp, #4]
	movs	r0, #0
	ldr	r3, [r5]
	cmp	r2, r3
	bne	.L33
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L15:
	subs	r0, r4, #4
	add	r0, r0, r6
	cmp	r6, r0
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
