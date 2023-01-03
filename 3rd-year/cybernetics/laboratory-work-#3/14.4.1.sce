printf("Корни первого многочлена:");
x = roots(poly([-0.9 -1 0 0 1.1], 'x', 'c'));
disp(x);
printf("Проверка:");
disp(1.1 * x.^4 - x - 0.9);

printf("Корни второго многочлена: ");
x1 = roots(poly([-4 1 0 1], 'x', 'c'));
disp(x1);
printf("Проверка:");
disp(x1.^3 + x1 - 4);
