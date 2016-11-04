Vthresh = 3.0;   %threshold voltage for the comparator
Vhyst = 0.1;     %hysteresis voltage (100mv)
IR = 4E-6;       %maximum allowable resistor current (4uA)

Rtot = 4.2/IR;
R1 = Rtot * (5/(Vthresh + Vhyst) - 1)
R2 = Rtot * (5/Vthresh - 1) - R1
R3 = Rtot - R1 - R2

%R1 = 649kOhm, R2 = 56.2kOhm, R3 = 348kOhm