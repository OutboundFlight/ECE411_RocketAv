filename = '08122016_launch3.csv';

%canonical launch time (USER DECISION)
tlaunch = 6;
ilaunch = 1212;

tland = 11;
iland = 1782;

%scale of configured full-scale range 
%xm scale in g
accel_scale = 16;
g = 9.81;

%gyro scale in deg per sec
gyro_scale = 2000;

%mag scale in gauss
mag_scale = 2;

half_range = 2^15;

accel_convert = g*accel_scale/half_range;
gyro_convert = gyro_scale/half_range;
mag_convert = mag_scale/half_range;

data = csvread(filename);

time = data(:, 1)/1000;
time = time - time(1);

accels = data(:, 2:4) * accel_convert;
accelmag = sqrt(dot(accels, accels,2));

gyros = data(:, 5:7) * gyro_convert;
mags = data(:, 8:10) * mag_convert;


subplot(2, 2, 1);
plot(time, accels(:, 1))
xlim([tlaunch, tland])
xlabel('Time (s)')
ylabel('Acceleration (m/s)')
title('Sensor X acceleration')

subplot(2, 2, 2);
plot(time, accels(:, 2))
xlim([tlaunch, tland])
xlabel('Time (s)')
ylabel('Acceleration (m/s)')
title('Sensor Y acceleration')

subplot(2,2,3);
plot(time, accels(:, 3))
xlim([tlaunch, tland])
xlabel('Time (s)')
ylabel('Acceleration (m/s)')
title('Sensor Z acceleration')


subplot(2, 2, 4);
plot(time, accelmag);
xlim([tlaunch, tland])
xlabel('Time (s)')
ylabel('Acceleration (m/s)')
title('Sensor acceleration magnitude')

figure();
plot(time, accelmag);
xlim([5 6.5])
xlabel('Time (s)')
ylabel('Acceleration (m/s)')
title('Sensor acceleration magnitude')
