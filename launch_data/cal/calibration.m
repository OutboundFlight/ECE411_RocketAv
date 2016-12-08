clear all;
fullscale_accel = 16;
fullscale_gyro = 2000; 
fullscale_mag = 2;

tlaunch = 5;
ilaunch = 3040;

convert_accel = fullscale_accel/2^15;
convert_rate = fullscale_gyro/2^15;
convert_mag = fullscale_mag/2^15;

data = csvread('accel_zcal.csv');

time = data(:, 1);
time = time - time(1);
time = time / 1000;

accelx = data(:, 2);
accely = data(:, 3);
accelz = data(:, 4);

ratex = data(:, 5);
ratey = data(:, 6);
ratez = data(:, 7);

magx = data(:, 8);
magy = data(:, 9);
magz = data(:, 10);

temp = data(:, 11);

calratex = mean(ratex(1:ilaunch));
calratey = mean(ratey(1:ilaunch));
calratez = mean(ratez(1:ilaunch));
