	.arch armv7-a
	.eabi_attribute 28, 1
	.fpu vfpv3-d16
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 1
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
	push	{r4, r5, r6, lr}
	mov	r6, r0
	subs	r5, r1, #4
	b	.L3
.L5:
	ldr	r2, [r4]
	ldr	r3, [r4, #4]
	cmp	r2, r3
	ble	.L4
	adds	r1, r4, #4
	mov	r0, r4
	bl	swap
.L4:
	adds	r4, r4, #4
	b	.L6
.L7:
	mov	r4, r6
.L6:
	cmp	r5, r4
	bne	.L5
	subs	r5, r5, #4
.L3:
	cmp	r5, r6
	bne	.L7
	pop	{r4, r5, r6, pc}
	.size	bubble_sort, .-bubble_sort
	.align	2
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	sub	sp, sp, #8
	movw	r3, #:lower16:__stack_chk_guard
	movt	r3, #:upper16:__stack_chk_guard
	ldr	r3, [r3]
	str	r3, [sp, #4]
	movs	r4, #0
	add	r1, sp, #8
	str	r4, [r1, #-8]!
	movw	r0, #:lower16:.LC0
	movt	r0, #:upper16:.LC0
	bl	__isoc99_scanf
	ldr	r0, [sp]
	lsls	r0, r0, #2
	bl	malloc
	mov	r6, r0
	b	.L10
.L11:
	add	r1, r6, r4, lsl #2
	movw	r0, #:lower16:.LC0
	movt	r0, #:upper16:.LC0
	bl	__isoc99_scanf
	adds	r4, r4, #1
.L10:
	ldr	r1, [sp]
	cmp	r4, r1
	blt	.L11
	add	r1, r6, r1, lsl #2
	mov	r0, r6
	bl	bubble_sort
	movs	r4, #0
	movs	r5, #0
	b	.L12
.L13:
	lsls	r3, r4, #2
	ldr	r2, [r6, r3]
	movw	r1, #:lower16:.LC0
	movt	r1, #:upper16:.LC0
	movs	r0, #1
	bl	__printf_chk
	adds	r4, r4, #1
	adc	r5, r5, #0
.L12:
	ldr	r2, [sp]
	asrs	r3, r2, #31
	cmp	r4, r2
	sbcs	r3, r5, r3
	blt	.L13
	movs	r0, #0
	movw	r3, #:lower16:__stack_chk_guard
	movt	r3, #:upper16:__stack_chk_guard
	ldr	r2, [sp, #4]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L14
	bl	__stack_chk_fail
.L14:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"%d\000"
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
