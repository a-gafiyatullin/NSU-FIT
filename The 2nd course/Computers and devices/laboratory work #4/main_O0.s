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
	push	{r4, r7, lr}	//сохранение на стеке значений регистров r4, r7(общие регистры), lr(регистр связи)
	sub	sp, sp, #36	//резервирование 36 байт на стеке
	add	r7, sp, #0	//r7 = sp(указатель на голову стека)
	movw	r3, #:lower16:__stack_chk_guard //во вторые 16 бит r3 записывается вторая половина __stack_chk_guard (защита стека)
	movt	r3, #:upper16:__stack_chk_guard	//в первые 16 бит r3 записывается первая половина __stack_chk_guard (защита стека)
	ldr	r3, [r3]	//r3 = *r3
	str	r3, [r7, #28]	//значение в __stack_chk_guard сохраняется на стеке
	movs	r3, #0		//r3 = 0
	str	r3, [r7, #4]	//сохранение значения из r3 на стеке									//int size = 0;
	adds	r3, r7, #4	//получение в r3 адреса переменной size на стеке
	mov	r1, r3		//r1 = r3, т.е r1 = &size, r1 - это второй аргумент функции scanf
	movw	r0, #:lower16:.LC0//во вторые 16 бит r0 записывается вторая половина LCO ("%d")
	movt	r0, #:upper16:.LC0//в первые 16 бит r0 записывается первая половина LCO ("%d"), r0 - первый аргумент функции scanf
	bl	__isoc99_scanf	//вызов функции scanf с сохранением адреса возврата							//scanf("%d", &size);
	ldr	r3, [r7, #4]	//r3 = size
	lsls	r3, r3, #2	//r3 = r3 * 4, т.е r3 = (size * sizeof(int))
	mov	r0, r3		//r0 = r3, r0 - аргумент функции malloc
	bl	malloc		//вызов функции malloc с сохранением адреса возврата							//int *array = (int*)malloc(size * sizeof(int));
	mov	r3, r0		//r3 = r0, в r0 вернулся указатель на аллоцированную память (array)
	str	r3, [r7, #12]	//сохранение значения r3 (array) на стеке
	movs	r3, #0		//r3 = 0
	str	r3, [r7, #8]	//сохранение значения из r3 (i) на стеке 								//int i = 0;
	b	.L9		//переход к проверке условия входа в цикл
.L10:
	ldr	r3, [r7, #8]	//r3 = i
	lsls	r3, r3, #2	//r3 = r3 * 4, т.е r3 = (i * sizeof(int))
	ldr	r2, [r7, #12]	//r2 = array
	add	r3, r3, r2	//r3 = r3 + r2, т.е r3 = (array + (i * sizeof(int))) - получили адрес i-го элемента массива
	mov	r1, r3		//r1 = r3, в r1 адрес i-го элемента массива, r1 - это второй аргумент функции scanf
	movw	r0, #:lower16:.LC0//во вторые 16 бит r0 записывается вторая половина LCO ("%d")
	movt	r0, #:upper16:.LC0//в первые 16 бит r0 записывается первая половина LCO ("%d"), r0 - первый аргумент функции scanf
	bl	__isoc99_scanf	//вызов функции scanf с сохранением адреса возврата
	ldr	r3, [r7, #8]	//r3 = i
	adds	r3, r3, #1	//r3 = r3 + 1, т.е i = i + 1										//i++
	str	r3, [r7, #8]	//сохранение значения из r3 (i) на стеке
.L9:
	ldr	r3, [r7, #4]	//r3 = size
	ldr	r2, [r7, #8]	//r2 = i
	cmp	r2, r3		//r2 < r3, т.е i < size											//i < size;
	blt	.L10		//если i < size, то переход в тело цикла
	ldr	r3, [r7, #4]	//если i >= size, то r3 = size
	lsls	r3, r3, #2	//r3 = r3 * 4, т.е r3 = (size * sizeof(int))
	ldr	r2, [r7, #12]	//r2 = array
	add	r3, r3, r2	//r3 = r3 + r2, т.е r3 = (array + (size * sizeof(int))), получили в r3 конец массива
	mov	r1, r3		//r1 = r3, в r1 адрес конца массива, r1 - второй аргумент функции bubble_sort
	ldr	r0, [r7, #12]	//r0 = array
	bl	bubble_sort	//вызов функции bubble_sort с сохранением адреса возврата						//bubble_sort(array, array + size);
	mov	r3, #0		//r3 = 0
	mov	r4, #0		//r4 = 0
	strd	r3, [r7, #16]	//сохранение значения из r3 (i) на стеке								//long long i = 0;
	b	.L11		//переход к проверке условия выхода из цикла
.L12:
	ldr	r3, [r7, #16]	//r3 = i
	lsls	r3, r3, #2	//r3 = r3 * 4, т.е r3 = (i * sizeof(int))
	ldr	r2, [r7, #12]	//r2 = array
	add	r3, r3, r2	//r3 = r3 + r2, т.е r3 = (array + (i * sizeof(int))) - получили адрес i-го элемента массива
	ldr	r3, [r3]	//r3 = *r3,т.е r3 = array[i]
	mov	r1, r3		//r1 = r3, r1 = array[i], r1 - второй аргумент функции printf
	movw	r0, #:lower16:.LC0//во вторые 16 бит r0 записывается вторая половина LCO ("%d")
	movt	r0, #:upper16:.LC0//в первые 16 бит r0 записывается первая половина LCO ("%d"), r0 - первый аргумент функции printf
	bl	printf		//вызов функции printf с сохранением адреса возврата							//printf("%d", array[i]);
	ldrd	r3, [r7, #16]	//загрузка двух 32-битных слов, r3 = i, r4 = 0(в нашем случае)
	adds	r3, r3, #1	//r3 = r3 + 1,т.е i = i + 1, суффикс s команды adds позволяет менять флаги				//i++
	adc	r4, r4, #0	//если из младшего слова 64-битного числа i есть перенос, то прибавим его к старшему слову
	strd	r3, [r7, #16]	//сохраняем значения из r3 (i) на стеке
.L11:
	ldr	r3, [r7, #4]	//r3 = size
	asr	r4, r3, #31	//арифметический сдвиг вправо на 31 бит значения size, получим 0 (в нашем случае), r4 = 0, старшее 32-битное слово в i
	ldrd	r1, [r7, #16]	//загрузка двух 32-битных слов, r1 = i, r2 = 0
	cmp	r1, r3		//сравниваем младшие 32-битные слова перменныx i и size
	sbcs	r3, r2, r4	//r3 = r2 - r4 с переносом, т.е сравниваются старшие слова переменных size и i путем вычитания с переносом
	blt	.L12		//если i < size, то переходим в тело цикла								//i < size;
	movs	r3, #0		//r3 = 0
	mov	r0, r3		//r0 = r3
	movw	r3, #:lower16:__stack_chk_guard	//во вторые 16 бит r3 записывается вторая половина __stack_chk_guard (защита стека)
	movt	r3, #:upper16:__stack_chk_guard	//в первые 16 бит r3 записывается первая половина __stack_chk_guard (защита стека)
	ldr	r2, [r7, #28]	//получим в r2 значение __stack_chk_guard, которое сохраняли в начале программы
	ldr	r3, [r3]	//r3 = *r3, получим в r3 значение __stack_chk_guard, которое должно быть
	cmp	r2, r3		//сранвим их
	beq	.L14		//если они равны, то все хорошо, можно завершать программу
	bl	__stack_chk_fail//а иначе проверим стек  на ошибки
.L14:
	adds	r7, r7, #36	//вернем зарезервированную память
	mov	sp, r7		//sp = r7, указатель стека вернулся к изначальному состоянию (как до запуска программы)
	@ sp needed
	pop	{r4, r7, pc}	//вернем данные со стека, которые сохранили в начале программы, обратно в регистры
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
