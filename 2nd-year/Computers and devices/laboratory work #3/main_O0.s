	.file	"main.c"	// имя компилируемого файла
.text
	.globl	swap		// идентификатор swap имеет глобальную область видимости, виден линковщику
	.type	swap, @function	// swap - это функция
swap:
.LFB2:
	.cfi_startproc		// отладочная информация			// void swap(int *left, int *right){
	pushq	%rbp		// кладем на стек текущую базу кадра
	.cfi_def_cfa_offset 16	// отладочная информация
	.cfi_offset 6, -16	// отладочная информация
	movq	%rsp, %rbp	// теперь база указывает на свое старое значение и тем самым разделяет фреймы функций
	.cfi_def_cfa_register 6	// отладочная информация
	movq	%rdi, -24(%rbp)	// загрузим left во фрейм
	movq	%rsi, -32(%rbp)	// загрузим right во фрейм
	movq	-24(%rbp), %rax	// left -> rax					// left;
	movl	(%rax), %eax	// значение по rax -> eax			// eax = *left;
	movl	%eax, -4(%rbp)	// загрузим tmp во фрейм			// int temp = eax;
	movq	-32(%rbp), %rax	// right -> rax					// right;
	movl	(%rax), %edx	// значение по rax -> edx			// edx = *right;
	movq	-24(%rbp), %rax	// left -> rax					// left;
	movl	%edx, (%rax)	// edx -> в значение по rax			// *left = *right;
	movq	-32(%rbp), %rax	// right -> rax					// right;
	movl	-4(%rbp), %edx	// tmp -> edx					// tmp = *right
	movl	%edx, (%rax)	// tmp -> в значение по rax			// *right = tmp;
	nop			// нет операции
	popq	%rbp		// удаляем фрейм, снимая со стека указатель на него
	.cfi_def_cfa 7, 8	// отладочная информация
	ret			// возвращаем уппавление вызвавшей функции	// }
	.cfi_endproc		// отладочная информация
.LFE2:
	.size	swap, .-swap	// отладочная информация
	.globl	bubble_sort	// идентификатор bubble_sort - глобальный, виден линковщику
	.type	bubble_sort, @function	// bubble_sort - функция
bubble_sort:
.LFB3:
	.cfi_startproc		// отладочная информация			// void bubble_sort(int *begin, int *end){
	pushq	%rbp		// кладем на стек текущую базу кадра
	.cfi_def_cfa_offset 16	// отладочная информация
	.cfi_offset 6, -16	// отладочная информация
	movq	%rsp, %rbp	// теперь база указывает на свое старое значение и тем самым разделяет фреймы функций
	.cfi_def_cfa_register 6	// отладочная информация
	subq	$32, %rsp	// резервируем 32 байта памяти на фрейме
	movq	%rdi, -24(%rbp)	// загрузим begin во фрейм
	movq	%rsi, -32(%rbp)	// загрузим end во фрейм
	movq	-32(%rbp), %rax	// end -> rax					// end;
	subq	$4, %rax	// rax = rax - 4				// end - 1;
	movq	%rax, -16(%rbp)	// загрузим out_iter во фрейм			// int *out_iter = end - 1;
	jmp	.L3		// прыгаем на сравнение во внешнем цикле алгоритма
.L7:
	movq	-24(%rbp), %rax	// begin -> rax					// begin;
	movq	%rax, -8(%rbp)	// загрузим in_iter во фрейм			// int *in_iter = begin;
	jmp	.L4		// прыгаем во внутренний цикл
.L6:
	movq	-8(%rbp), %rax	// in_iter -> rax				// in_iter
	movl	(%rax), %edx	// rax -> edx					// *in_iter;
	movq	-8(%rbp), %rax	// in_iter -> rax				// in_iter;
	addq	$4, %rax	// сдвинем in_iter на 1 ячейку массива вправо	// in_iter + 1;
	movl	(%rax), %eax	// значение по (in_iter + 1) -> eax		// *(in_iter + 1);
	cmpl	%eax, %edx	// сравним значения, которые лежат в in_iter и в (in_iter + 1)	// if(*in_iter > *(in_iter + 1))
	jle	.L5		// если в (in_iter + 1) меньшее значение,чем в iter,то продолжаем этот блок,а иначе переходим к изменению in_iter
	movq	-8(%rbp), %rax	// in_iter -> rax				// in_iter;
	leaq	4(%rax), %rdx	// (in_iter + 1) -> rdx				// in_iter + 1
	movq	-8(%rbp), %rax	// in_iter -> rax				// in_iter
	movq	%rdx, %rsi	// передаем (in_iter + 1) в swap
	movq	%rax, %rdi	// передаем in_iter в swap
	call	swap		// вызываем swap				// swap(in_iter, in_iter + 1);
.L5:
	addq	$4, -8(%rbp)	// сдвигаем in_iter на 1 ячейку вправо		// in_iter++
.L4:
	movq	-8(%rbp), %rax		// in_iter -> rax			// in_iter
	cmpq	-16(%rbp), %rax		// rax срвнивается с out_iter		// in_iter != out_iter;
	jne	.L6			// если не равны, то прыгаем в тело цикла
	subq	$4, -16(%rbp)		// иначе сдвигаем out_iter на одну ячеку влево	// out_iter--
.L3:
	movq	-16(%rbp), %rax		// out_iter -> rax			// out_iter
	cmpq	-24(%rbp), %rax		// rax сравнивается с  begin		// out_iter != begin;
	jne	.L7			// если не равны, то прыгаем на подготовку ко внутреннему циклу
	nop				// иначе нет операции
	leave				// выбрасываем последний кадр стека
	.cfi_def_cfa 7, 8		// отладочная информация
	ret				// возвращаем управление вызывающей функции // }
	.cfi_endproc			// отладочная информация
.LFE3:
	.size	bubble_sort, .-bubble_sort// отладочная информация
	.section	.rodata		// выбор секции файла
.LC0:
	.string	"%d"			// строковый литерал
	.text
	.globl	main			// идентификатор main - глобальный, виден линковщику
	.type	main, @function		// main - имя функции
main:
.LFB4:
	.cfi_startproc		// отладочная информация			// int main(){
	pushq	%rbp		// кладем на стек текущую базу кадра
	.cfi_def_cfa_offset 16	// отладочная информация
	.cfi_offset 6, -16	// отладочная информация
	movq	%rsp, %rbp	// теперь база указывает на свое старое значение и тем самым разделяет фреймы функций
	.cfi_def_cfa_register 6	// отладочная информвция
	subq	$32, %rsp	// зарезервируем 32 байта на стеке
	movq	%fs:40, %rax	// получаем канарейку (защита стека)
	movq	%rax, -8(%rbp)	// записываем значение канарейки
	xorl	%eax, %eax	// обнуляем eax
	movl	$0, -32(%rbp)	// запишем 0 в size				// int size = 0;
	leaq	-32(%rbp), %rax	// адрес size -> rax				// &size;
	movq	%rax, %rsi	// адрес size - второй аргумент функции scanf	// "%d"
	movl	$.LC0, %edi	// форматная строка - первый арумент функции scanf
	movl	$0, %eax	// обнулили переменную, куда произойдет возврат значения scanf
	call	__isoc99_scanf	// вызов scanf					// scanf("%d", &size);
	movl	-32(%rbp), %eax	// size -> eax					// size;
	cltq			// eax -> rax
	salq	$2, %rax	// умножим size на 4				// size * sizeof(int);
	movq	%rax, %rdi	// передаем size * 4 в malloc
	call	malloc		// вызов malloc					// int *array = (int*)malloc(size * sizeof(int));
	movq	%rax, -16(%rbp)	// запишем адрес array во фрейм
	movl	$0, -28(%rbp)	// обнуляем итератор i				// int i = 0;
	jmp	.L9		// прыгаем в цикл
.L10:
	movl	-28(%rbp), %eax	// i -> eax
	cltq			// eax -> rax					// i;
	leaq	0(,%rax,4), %rdx// вычислим смещение относительно начала массива array и -> rdx	// i * 4;
	movq	-16(%rbp), %rax	// array -> rax							// array;
	addq	%rdx, %rax	// получаем нужную позицию в массиве array, сохраняем в rax	// (array + i);
	movq	%rax, %rsi	// rax -> rsi (передача аргумента)
	movl	$.LC0, %edi	// форматная строка - первый аргумент функции scanf (передача аргумента)
	movl	$0, %eax	// обнулили переменную, куда произойдет возврат значения scanf
	call	__isoc99_scanf	// вызов scanf							// scanf("%d", array + i);
	addl	$1, -28(%rbp)	// увеличили итератор i на 1					// i++;
.L9:
	movl	-32(%rbp), %eax	// size -> eax							// size;
	cmpl	%eax, -28(%rbp)	// сравниваем eax с i						// i < size;
	jl	.L10		// если i < size, то считываем значение
	movl	-32(%rbp), %eax	// size -> eax							// size;
	cltq			// eax -> rax
	leaq	0(,%rax,4), %rdx// вычислим смещение относительно начала массива array и -> rdx	// size * 4;
	movq	-16(%rbp), %rax	// array -> rax							// array;
	addq	%rax, %rdx	// получаем нужную позицию в массиве array, сохраняем в rdx	// array + (size * 4);
	movq	-16(%rbp), %rax	// array -> rax							// array;
	movq	%rdx, %rsi	// rdx -> rsi (передача 2-го аргумента)
	movq	%rax, %rdi	// rax -> rdi (передача 1-го аргумента)
	call	bubble_sort	// вызов функции bubble_sort					// bubble_sort(array, array + size);
	movq	$0, -24(%rbp)	// обнуляем итератор i						// long long i = 0;
	jmp	.L11		// переходим к циклу
.L12:
	movq	-24(%rbp), %rax	// i -> rax
	leaq	0(,%rax,4), %rdx// вычислим смещение относительно начала массива array и -> rdx	// i * 4;
	movq	-16(%rbp), %rax	// array -> rax							// array;
	addq	%rdx, %rax	// получаем нужную позицию в массиве array, сохраняем в rax	// (array + i);
	movl	(%rax), %eax	// получаем значение, лежащее по rax -> eax			// array[i]
	movl	%eax, %esi	// eax -> esi (передача 2-го аргумента)
	movl	$.LC0, %edi	// форматная строка - первый аргумент printf			// "%d";
	movl	$0, %eax	// в eax будет возвращен результат работы printf
	call	printf		// вызов printf							// printf("%d", array[i]);
	addq	$1, -24(%rbp)	// увеличиваем i на 1						// i++;
.L11:
	movl	-32(%rbp), %eax	// size -> eax							// size;
	cltq			// eax -> rax
	cmpq	-24(%rbp), %rax	// сравниваем i и rax						// i < size;
	jg	.L12		// если i < size, то переходим в тело цикла
	movl	$0, %eax	// 0 -> eax
	movq	-8(%rbp), %rcx	// получаем канарейку
	xorq	%fs:40, %rcx	// сравниваем  с ранее записанным значением
	je	.L14		// если все нормально(они равны), то завершаем программу
	call	__stack_chk_fail// иначе проверяем стек на ошибки
.L14:
	leave			// удаляем данные с кадра
	.cfi_def_cfa 7, 8	// отладочная информация
	ret			// возвращаем управление операционной системе			// }
	.cfi_endproc		// отладочная информация
.LFE4:
	.size	main, .-main	// отладочная информация
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits// выбор секции файла
