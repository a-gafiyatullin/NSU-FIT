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
	push	{r7}		//сохранение на стеке значений регистров r7(общий регистр)			//void swap(int *left, int *right){
	sub	sp, sp, #20	//выделение 20 байт на стеке
	add	r7, sp, #0	//r7 = sp
	str	r0, [r7, #4]	//сохранение знаечния in_iter на стеке
	str	r1, [r7]	//сохранение знаечния out_iter на стеке
	ldr	r3, [r7, #4]	//r3 = in_iter
	ldr	r3, [r3]	//r3 = *r3, т.е r3 = *in_iter
	str	r3, [r7, #12]	//сохраняем значение, лежащее по адресу in_iter на стеке			//int tmp = *left;
	ldr	r3, [r7]	//r3 = out_iter
	ldr	r2, [r3]	//r2 = *out_iter
	ldr	r3, [r7, #4]	//r3 = in_iter
	str	r2, [r3]	//*r3 = r2, т.е *in_iter = *out_iter						//*left = *right;
	ldr	r3, [r7]	//r3 = out_iter
	ldr	r2, [r7, #12]	//r2 = temp
	str	r2, [r3]	//*r3 = temp									//*right = tmp;
	nop			//ничего не делать
	adds	r7, r7, #20	//возвращаем 20 байт на стеке
	mov	sp, r7		//sp = r7
	@ sp needed
	ldr	r7, [sp], #4	//r7 = sp, которое было до входа в функцию, снимаем со стека это значение
	bx	lr		//смена системы команд от THUMB к ARM при возврате к вызывающей функции (если потребуется)	//}
	.size	swap, .-swap	//директива компилятора для подсчета размера функции
	.align	2		//установка выравнивания - выравнивание на 4 байта
	.global	bubble_sort	//bubble_sort - глобальный идентификатор, виден линковщику
	.syntax unified		//выбор однообразного синтаксиса для ARM и THUMB
	.thumb			//выбор генерации системы команд THUMB
	.thumb_func		//следующая функция - функция системы команд THUMB
	.type	bubble_sort, %function	//bubble_sort - это функция
bubble_sort:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}	//сохранение на стеке значений регистров r7(общий регистр), lr(регистр связи)	//void bubble_sort(int *begin, int *end){
	sub	sp, sp, #16	//выделение 16 байт на стеке
	add	r7, sp, #0	//r7 = sp
	str	r0, [r7, #4]	//записываем указатель array на стек
	str	r1, [r7]	//записываем указатель (array + size * 4) на стек
	ldr	r3, [r7]	//r3 = *r7, т.е r3 = array + size * 4
	subs	r3, r3, #4	//r3 = r3 - 4, т.е r3 = array + size * 4 - 4					//int *out_iter = end - 1;
	str	r3, [r7, #8]	//загрузим значение итератора out_iter на стек
	b	.L3		//переход к проверке условия внешнего цикла
.L7:
	ldr	r3, [r7, #4]	//r3 = begin
	str	r3, [r7, #12]	//записываем значение итератора in_iter на стек					//int *in_iter = begin;
	b	.L4		//переходим к проверке условия внутреннего цикла
.L6:
	ldr	r3, [r7, #12]	//r3 = in_iter
	ldr	r2, [r3]	//r2 = *in_iter
	ldr	r3, [r7, #12]	//r3 = in_iter
	adds	r3, r3, #4	//r3 = in_iter + 4
	ldr	r3, [r3]	//r3 = *r3, т.е r3 = *(in_iter + 4)
	cmp	r2, r3		//сравниваем значения в r2 и r3							//if(*in_iter > *(in_iter + 1))
	ble	.L5		//если r2 <= r3, то переходим на следуюущую итерацию
	ldr	r3, [r7, #12]	//r3 = in_iter
	adds	r3, r3, #4	//r3 = in_iter + 1
	mov	r1, r3		//r1 = r3, r1 = in_iter + 1, r1 - второй аргумент функции swap
	ldr	r0, [r7, #12]	//r0 = in_iter, r0 - первый аргумент функции swap
	bl	swap		//вызов функции swap с сохранением адреса возврата в lr				//swap(in_iter, in_iter + 1);
.L5:
	ldr	r3, [r7, #12]	//r3 = in_iter
	adds	r3, r3, #4	//r3 = in_iter + 4
	str	r3, [r7, #12]	//обновим значение итератра in_iter на стеке					//in_iter++
.L4:
	ldr	r2, [r7, #12]	//r2 = in_iter
	ldr	r3, [r7, #8]	//r3 = out_iter
	cmp	r2, r3		//сравниваем итераторы								//in_iter != out_iter;
	bne	.L6		//если не равны, то переходим в тело цикла
	ldr	r3, [r7, #8]	//r3 = out_iter
	subs	r3, r3, #4	//r3 = out_iter - 4
	str	r3, [r7, #8]	//обновим значение итератра out_iter на стеке					//out_iter--
.L3:
	ldr	r2, [r7, #8]	//r2 = out_iter
	ldr	r3, [r7, #4]	//r3 = array, т.е r3 = begin
	cmp	r2, r3		//сравниваем итреторы								//out_iter != begin;
	bne	.L7		//если не равны, то переходим к внутреннему циклу
	nop			//иначе, ничего не делать
	adds	r7, r7, #16	//возвращем 16 байт на стеке
	mov	sp, r7		//sp = r7
	@ sp needed
	pop	{r7, pc}	//вернем данные со стека, которые сохранили при входе в функцию, обратно в регистры	//}
	.size	bubble_sort, .-bubble_sort	//директива компилятора для подсчета размера функции
	.section	.rodata	//секция глобальных и статических неизменяемых данных
	.align	2		//установка выравнивания - выравнивание на 4 байта
.LC0:
	.ascii	"%d\000"	//форматная строка
	.text			//секция скомпилированного машинного кода
	.align	2		//установка выравнивания - выравнивание на 4 байта
	.global	main		//main - глобальный идентификатор, виден линковщику
	.syntax unified		//выбор однообразного синтаксиса для ARM и THUMB
	.thumb			//выбор генерации системы команд THUMB
	.thumb_func		//следующая функция - функция системы команд THUMB
	.type	main, %function	//main - это функция
main:
	@ args = 0, pretend = 0, frame = 32												//int main(){
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
	bl	__isoc99_scanf	//вызов функции scanf с сохранением адреса возврата в lr						//scanf("%d", &size);
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
	bl	__isoc99_scanf	//вызов функции scanf с сохранением адреса возврата в lr						//scanf("%d", array + i);
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
	bl	bubble_sort	//вызов функции bubble_sort с сохранением адреса возврата в lr						//bubble_sort(array, array + size);
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
	bl	printf		//вызов функции printf с сохранением адреса возврата в lr						//printf("%d", array[i]);
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
	cmp	r2, r3		//сравним их
	beq	.L14		//если они равны, то все хорошо, можно завершать программу
	bl	__stack_chk_fail//а иначе проверим стек  на ошибки
.L14:
	adds	r7, r7, #36	//вернем зарезервированную память
	mov	sp, r7		//sp = r7
	@ sp needed
	pop	{r4, r7, pc}	//вернем данные со стека, которые сохранили в начале программы, обратно в регистры			//}
	.size	main, .-main	//директива компилятора для подсчета размера функции
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609" //директива совместимости на уровне исходного кода
	.section	.note.GNU-stack,"",%progbits	//указание линовщику пометить стек и данные, как неисполняемые
