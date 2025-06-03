clear;
close all;
clc;

%% Load Raw Sensor Data
load station03s.mat % Assuming this contains tvec, rawAccel, rawGyro, rawMag
% load rawData_movement.mat
numSamples = length(tvec);
imuFs = 100; % IMU sample rate (Hz)
imu = struct(...
    'gyro', [0 0 0], ...
    'accel', [0 0 0]);

NAV = 100;

meanAccel = mean(Accelerometer(1:NAV,:));
meanMag = mean(Magnetometer(1:NAV,:));
meanAlti = mean(Altimeter(1:NAV,:));

initState = initStateEKF(meanAccel, meanMag, meanAlti);
sys = ekf10Init(imuFs, initState);

Euler = zeros(numSamples, 3);
estAlti = zeros(numSamples, 1);
for ii = 1:numSamples
    imu.accel = Accelerometer(ii, :);
    imu.gyro = Gyroscope(ii, :);
    
    sys = ekf10Step(sys, imu, Magnetometer(ii, :), Altimeter(ii));
    Euler(ii, :) = sys.euler; % log data
    estAlti(ii) = sys.x(5);
end

%% Plot Results
% Euler Angles
figure;
plot(tvec, rad2deg(Euler), 'LineWidth', 1.5);
title('EKF Orientation (Combined System)');
legend('Yaw', 'Pitch', 'Roll');
ylabel('Rotation (degrees)');
xlabel('Time (s)');
grid on;

figure;
plot(tvec, estAlti, 'LineWidth', 1.5);
title('estAlti');
legend('z');
ylabel('Altitude');
xlabel('Time (s)');
grid on;

% Raw Sensor Data
figure;
subplot(3, 1, 1); plot(tvec, Accelerometer); title('Accelerometer Data'); legend('X', 'Y', 'Z'); ylabel('m/s^2'); grid on;
subplot(3, 1, 2); plot(tvec, Gyroscope); title('Gyroscope Data'); legend('X', 'Y', 'Z'); ylabel('rad/s'); grid on;
subplot(3, 1, 3); plot(tvec, Magnetometer); title('Magnetometer Data'); legend('X', 'Y', 'Z'); ylabel('gauss'); grid on;

figure
plot(tvec, Altimeter, 'LineWidth', 1.5); title('Altimeter Data'); legend('Z'); ylabel('meters'); xlabel('Time (s)'); grid on;
