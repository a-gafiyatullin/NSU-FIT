A = [-1 -1 -2 -3; 3 -1 -1 -2; 2 3 -1 -1; 1 2 3 -1];
b = [2; -8; -12; 8];
x = linsolve(A, -b);
disp(x);
printf("Проверка:\nA * x = ");
disp(A * x);
printf("\n b = ");
disp(b);
