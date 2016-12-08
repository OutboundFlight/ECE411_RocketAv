data = csvread('BlueGyro2.txt');

time = data(:, 1);
time = time - time(1);
time = time/1000;

xdata = data(:, 5);
ydata = data(:, 6);
zdata = data(:, 7);
