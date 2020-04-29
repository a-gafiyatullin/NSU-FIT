T0 = 0.76;
n = 3;
Tau = 1.2;
K = 0.481194691128603401778886212513064354113197630437637476688;
Ti = 1.30705212;

h = Tau / 100; // T / 10, T / 100
T = 96;
microcap_N = 8000;
microcap_h = T / microcap_N;

// дискретизация
s = poly(0, 's');
W1 = K * (1 + 1 / (s * Ti)) * ((-Tau * s + 2 * n)^n / (Tau * s + 2 * n)^n) / (1 + s * T0)^n;
W = W1 / (1 + W1);
Sys = syslin('c', W);
Sysd = dscr(Sys, h);

t = [0:h:T - h];
v = zeros(size(Sysd.C, 'c'), 1);
x = zeros(length(t));
u = 1;

for i = 1:length(t)
    x(i, 1) = Sysd.C * v;
    v = Sysd.A * v + Sysd.B * u;
end

plot(t', x, 'blue');

// расчеты Micro-Cap
chdir('C:\Users\agafi\Documents\programming_projects\NSU-FIT\the-3rd-year\cybernetics\laboratory-work-#5');
[microcap] = read('pi.CSV', microcap_N, 2);

t = [0:microcap_h:T - microcap_h];
plot(t', microcap(:, 2), 'red');

// ошибка
sum = 0;
N = T / h;
for i = 1:N
    sum = sum + (microcap(i * h / microcap_h, 2) - x(i, 1))^2;
end
disp('Error:');
e = ((1.0 / N) * sum)^(1/2);
disp(e);
