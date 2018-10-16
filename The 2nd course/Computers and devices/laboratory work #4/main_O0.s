	.arch armv7-a		//архитектура armv7-a
	.eabi_attribute 28, 1	//выбор различных атрибутов
	.fpu vfpv3-d16		//выбор математического сопроцессора
	.eabi_attribute 20, 1	//выбор различных атрибутов
	.eabi_attribute 21, 1	//выбор различных атрибутов
	.eabi_attribute 23, 3	//выбор различных атрибутов
	.eabi_attribute 24, 1	//выбор различных атрибутов
	.eabi_attribute 25, 1	//выбор различных атрибутов
	.eabi_attribute 26, 2	//выбор различных атрибутов
	.eabi_attribute 30, 6	//выбор различных атрибутов
	.eabi_attribute 34, 1	//выбор различных атрибутов
	.eabi_attribute 18, 4	//выбор различных атрибутов
	.file	"main.c"	//имя компилируемого файла
	.text
	.align	2		//установка выравнивания - выравнивание на 4 байта
	.global	swap		//swap - глобальный идентификатор, виден линковщику
	.syntax unified		//выбор однообразного синтаксиса для ARM и THUMB
	.thumb			//выбор генерации системы команд THUMB
	.thumb_func		//следующая функция - функция системы команд THUMB
	.type	swap, %function	//swap - это функция
swap:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #20
	add	r7, sp, #0
	str	r0, [r7, #4]
	str	r1, [r7]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	str	r3, [r7, #12]
	ldr	r3, [r7]
	ldr	r2, [r3]
	ldr	r3, [r7, #4]
	str	r2, [r3]
	ldr	r3, [r7]
	ldr	r2, [r7, #12]
	str	r2, [r3]
	nop
	adds	r7, r7, #20
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
	.size	swap, .-swap
	.align	2
	.global	bubble_sort
	.syntax unified
	.thumb
	.thumb_func
	.type	bubble_sort, %function
bubble_sort:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #16
	add	r7, sp, #0
	str	r0, [r7, #4]
	str	r1, [r7]
	ldr	r3, [r7]
	subs	r3, r3, #4
	str	r3, [r7, #8]
	b	.L3
.L7:
	ldr	r3, [r7, #4]
	str	r3, [r7, #12]
	b	.L4
.L6:
	ldr	r3, [r7, #12]
	ldr	r2, [r3]
	ldr	r3, [r7, #12]
	adds	r3, r3, #4
	ldr	r3, [r3]
	cmp	r2, r3
	ble	.L5
	ldr	r3, [r7, #12]
	adds	r3, r3, #4
	mov	r1, r3
	ldr	r0, [r7, #12]
	bl	swap
.L5:
	ldr	r3, [r7, #12]
	adds	r3, r3, #4
	str	r3, [r7, #12]
.L4:
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #8]
	cmp	r2, r3
	bne	.L6
	ldr	r3, [r7, #8]
	subs	r3, r3, #4
	str	r3, [r7, #8]
.L3:
	ldr	r2, [r7, #8]
	ldr	r3, [r7, #4]
	cmp	r2, r3
	bne	.L7
	nop
	adds	r7, r7, #16
	mov	sp, r7
	@ sp needed
	pop	{r7, pc}
	.size	bubble_sort, .-bubble_sort
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%d\000"
	.text
	.align	2
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r7, lr}
	sub	sp, sp, #36
	add	r7, sp, #0
	movw	r3, #:lower16:__stack_chk_guard
	movt	r3, #:upper16:__stack_chk_guard
	ldr	r3, [r3]
	str	r3, [r7, #28]
	movs	r3, #0
	str	r3, [r7, #4]
	adds	r3, r7, #4
	mov	r1, r3
	movw	r0, #:lower16:.LC0
	movt	r0, #:upper16:.LC0
	bl	__isoc99_scanf
	ldr	r3, [r7, #4]
	lsls	r3, r3, #2
	mov	r0, r3
	bl	malloc
	mov	r3, r0
	str	r3, [r7, #12]
	movs	r3, #0
	str	r3, [r7, #8]
	b	.L9
.L10:
	ldr	r3, [r7, #8]
	lsls	r3, r3, #2
	ldr	r2, [r7, #12]
	add	r3, r3, r2
	mov	r1, r3
	movw	r0, #:lower16:.LC0
	movt	r0, #:upper16:.LC0
	bl	__isoc99_scanf
	ldr	r3, [r7, #8]
	adds	r3, r3, #1
	str	r3, [r7, #8]
.L9:
	ldr	r3, [r7, #4]
	ldr	r2, [r7, #8]
	cmp	r2, r3
	blt	.L10
	ldr	r3, [r7, #4]
	lsls	r3, r3, #2
	ldr	r2, [r7, #12]
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [r7, #12]
	bl	bubble_sort
	mov	r3, #0
	mov	r4, #0
	strd	r3, [r7, #16]
	b	.L11
.L12:
	ldr	r3, [r7, #16]
	lsls	r3, r3, #2
	ldr	r2, [r7, #12]
	add	r3, r3, r2
	ldr	r3, [r3]
	mov	r1, r3
	movw	r0, #:lower16:.LC0
	movt	r0, #:upper16:.LC0
	bl	printf
	ldrd	r3, [r7, #16]
	adds	r3, r3, #1
	adc	r4, r4, #0
	strd	r3, [r7, #16]
.L11:
	ldr	r3, [r7, #4]
	asr	r4, r3, #31
	ldrd	r1, [r7, #16]
	cmp	r1, r3
	sbcs	r3, r2, r4
	blt	.L12
	movs	r3, #0
	mov	r0, r3
	movw	r3, #:lower16:__stack_chk_guard
	movt	r3, #:upper16:__stack_chk_guard
	ldr	r2, [r7, #28]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L14
	bl	__stack_chk_fail
.L14:
	adds	r7, r7, #36
	mov	sp, r7
	@ sp needed
	pop	{r4, r7, pc}
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
