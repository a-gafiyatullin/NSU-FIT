x = -10:0.01:10;
y = (1.2 * x .^ 3 + x .^2 - 2.8 * x - 1) ./ (x .^2 - 1)
plot(x, y)
