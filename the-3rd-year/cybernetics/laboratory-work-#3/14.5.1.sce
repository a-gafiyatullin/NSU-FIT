// P = As^3 + Bs^2 + D
function zr = G(c, s)
    zr = s(2) - c(1) * s(1)^3 - c(2) * s(1)^2 - c(3)
endfunction

c = [0; 0; 0];
x = [0 1:0.5:5];
y = [12 10.1 11.58 17.4 30.68 53.6 87.78 136.9 202.5 287];
z = [x; y];

disp("P = As^3 + Bs^2 + D коэффициенты и ошибка:")
[a, err] = datafit(G, z, c);
disp(a);
disp(err);

// график данных:
plot2d(x, y)
p = a(1) * x.^3 + a(2) * x.^2 - a(3); 
plot2d(x, p);

// Линия регрессии через datafit
function zr = P(c, s)
    zr = s(2) - c(1) - c(2) * s(1)
endfunction

с = [0; 0];
disp("P = As + B коэффициенты и ошибка через datafit:")
[a, err] = datafit(P, z, c);
disp(a);
disp(err);

// Линия регрессии через reglin
disp("P = As + B коэффициенты через reglin:")
[a, b] = reglin(x, y);
disp(a);
disp(b);
