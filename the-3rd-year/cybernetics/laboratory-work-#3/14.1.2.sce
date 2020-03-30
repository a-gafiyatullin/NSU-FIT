A = [2 3 -1; 4 5 2; -1 0 7];
B = [-1 0 5; 0 1 3; 2 -2 4];
D = (A * A + B) * (B * 2 - A) * 2;
if det(D) <> 0 then
    printf("D^(-1) = ")
    C = inv(D);
    disp(C);
    printf("Проверка:\nD * D^(-1) = ");
    disp(D * C);
    printf("Проверка:\nD^(-1) * D = ");
    disp(C * D);
end
