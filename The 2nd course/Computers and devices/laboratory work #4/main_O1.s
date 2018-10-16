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
	@ link register save eliminated.
	subs	r1, r1, #4
	cmp	r1, r0
	bne	.L12
	bx	lr
.L6:
	ldr	r2, [r3]
	ldr	r4, [r3, #4]
	cmp	r2, r4
	itt	gt
	strgt	r4, [r3]
	strgt	r2, [r3, #4]
	adds	r3, r3, #4
	cmp	r3, r1
	bne	.L6
.L7:
	subs	r1, r1, #4
	cmp	r0, r1
	beq	.L2
	b	.L10
.L12:
	push	{r4}
.L10:
	cmp	r0, r1
	it	ne
	movne	r3, r0
	bne	.L6
	b	.L7
.L2:
	ldr	r4, [sp], #4
	bx	lr
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
	push	{r4, r5, r6, r7, r8, lr}
	sub	sp, sp, #8
	movw	r3, #:lower16:__stack_chk_guard
	movt	r3, #:upper16:__stack_chk_guard
	ldr	r3, [r3]
	str	r3, [sp, #4]
	add	r1, sp, #8
	movs	r3, #0
	str	r3, [r1, #-8]!
	movw	r0, #:lower16:.LC0
	movt	r0, #:upper16:.LC0
	bl	__isoc99_scanf
	ldr	r0, [sp]
	lsls	r0, r0, #2
	bl	malloc
	mov	r6, r0
	ldr	r1, [sp]
	cmp	r1, #0
	ble	.L14
	mov	r5, r0
	movs	r4, #0
	movw	r7, #:lower16:.LC0
	movt	r7, #:upper16:.LC0
.L15:
	mov	r1, r5
	mov	r0, r7
	bl	__isoc99_scanf
	adds	r4, r4, #1
	ldr	r1, [sp]
	adds	r5, r5, #4
	cmp	r1, r4
	bgt	.L15
.L14:
	add	r1, r6, r1, lsl #2
	mov	r0, r6
	bl	bubble_sort
	ldr	r3, [sp]
	cmp	r3, #0
	ble	.L16
	subs	r6, r6, #4
	movs	r4, #0
	movs	r5, #0
	movw	r7, #:lower16:.LC0
	movt	r7, #:upper16:.LC0
	mov	r8, #1
.L17:
	ldr	r2, [r6, #4]!
	mov	r1, r7
	mov	r0, r8
	bl	__printf_chk
	adds	r4, r4, #1
	adc	r5, r5, #0
	ldr	r2, [sp]
	asrs	r3, r2, #31
	cmp	r4, r2
	sbcs	r3, r5, r3
	blt	.L17
.L16:
	movs	r0, #0
	movw	r3, #:lower16:__stack_chk_guard
	movt	r3, #:upper16:__stack_chk_guard
	ldr	r2, [sp, #4]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L18
	bl	__stack_chk_fail
.L18:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"%d\000"
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
