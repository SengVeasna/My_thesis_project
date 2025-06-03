function sys = ekf10Init(freq, initState)
    % Initialize Extended Kalman Filter for combined orientation and altitude
    % NED frame (North-East-Down), using BMP280 as barometer sensor
    dt = 1 / freq; % Sampling time
    % State vector: [qw, qx, qy, qz, h, vh] 
    % Quaternion (orientation), altitude (h), vertical velocity (vh) in NED
    
    % Gyroscope Process Noise (MPU6500)
    gyro_noise_density = deg2rad(0.005); % MPU6500: 0.005 °/s/√Hz to rad/s/√Hz
    Q_gyro_base = gyro_noise_density^2 * dt; % Continuous to discrete-time variance
    Q_gyro_quat = 0.25 * dt^3 * gyro_noise_density^2; % Quaternion noise (x, y, z)
    
    % Accelerometer Process Noise (MPU6500)
    acc_noise_density = 300e-6 * 9.81; % MPU6500: 300 µg/√Hz to m/s^2/√Hz
    Q_acc_base = acc_noise_density^2 * dt; % Discrete-time variance
    
    % Process Noise Covariance (Q) - 6x6 for quaternion and altitude/velocity
    Q = diag([Q_gyro_quat, Q_gyro_quat, Q_gyro_quat, Q_gyro_base, ... % Quaternion
              Q_acc_base, Q_acc_base]); % Altitude, velocity (tuned for stability)
    
    % Measurement Noise - Accelerometer (MPU6500)
    R_acc_noise = acc_noise_density^2 * dt; % Discrete-time variance
    R_acc = diag([R_acc_noise, R_acc_noise, R_acc_noise]); % 3x3 for accel
    
    % Measurement Noise - Magnetometer (QMC5883P)
    mag_noise_density = 0.5; % QMC5883P: ~0.1 gauss noise
    R_mag_noise = mag_noise_density^2 * dt; % Discrete-time variance
    R_mag = diag([R_mag_noise, R_mag_noise, R_mag_noise]); % 3x3 for mag
    
    % Measurement Noise - Barometer (BMP280)
    baro_noise = 0.05; % BMP280 typical noise ~0.02-0.05 m, adjusted for NED
    R_baro = baro_noise^2; % Scalar variance for altitude
    
    % Initial State Covariance (P) - 6x6 for combined system
    P = diag([1e-5, 1e-5, 1e-5, 1e-5, 1e-2, 1e-2]); % Relaxed for convergence
    
    % Output Combined System Structure
    sys = struct(...
        'euler', [0 0 0], ...
        'dt', dt, ...
        'Q', Q, ... % Process noise covariance
        'R_acc', R_acc, ... % Accelerometer measurement noise
        'R_mag', R_mag, ... % Magnetometer measurement noise
        'R_baro', R_baro, ... % Barometer (BMP280) measurement noise
        'x', initState, ... % Initial state: [qw qx qy qz h vh]
        'P', P); % Initial covariance
end