u = linspace(0, 2 * %pi);
v = linspace(0, 2 * %pi);
x = (cos(u) .* u)' * (cos(v ./ 2) + 1);
y = (u ./ 2)' * sin(v);
z = (sin(u) .* u)' * (cos(v ./ 2) + 1);
plot3d2(x, y, z)
