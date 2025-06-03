function sys = ekf10Step(sys, imu, magNED, alti)
    % Extended Kalman Filter Step for Combined Orientation and Altitude
    % NED frame (North-East-Down), using BMP280 as barometer sensor
    dt = sys.dt; % Sampling period
    
    % Extract current state
    q = sys.x(1:4); % Quaternion [qw, qx, qy, qz]
    h = sys.x(5);   % Altitude (positive up in NED)
    vh = sys.x(6);  % Vertical velocity (positive up in NED)
    
    % --- Prediction Step ---
    % 1. Quaternion propagation (using gyroscope)
    gyro_quat = [0, imu.gyro]; % Gyroscope quaternion (body frame rates)
    q_dot = 0.5 * quatmultiply(q', gyro_quat)'; % Quaternion derivative
    q_pred = q + dt * q_dot; % Propagate quaternion
    q_pred = quatnormalize(q_pred')'; % Normalize quaternion
    
    % 2. Transform accelerometer from body to NED frame
    rotm = quat2rotm(q_pred'); % Rotation matrix from body to NED
    accel_ned = rotm * imu.accel'; % Acceleration in NED frame
    % In NED, z is down, gravity is +9.80665 m/s^2, subtract to get motion accel
    a_z = accel_ned(3) - 9.81; % Vertical acceleration (down positive)
    % Convert to upward positive for altitude convention
    a_up = -a_z; % Motion acceleration upward (positive up)
    
    % 3. Altitude and velocity propagation (positive up)
    h_pred = h + dt * vh + 0.5 * dt^2 * a_up; % Altitude update
    vh_pred = vh + dt * a_up; % Velocity update
    
    % 4. Combined predicted state
    x_pred = [q_pred; h_pred; vh_pred];
    
    % 5. State transition Jacobian (F) - Linearized process model
    F = eye(6); % 6x6 identity as base
    % Quaternion part (small-angle approximation)
    F(1:4, 1:4) = eye(4) + 0.5 * dt * [0 -imu.gyro(1) -imu.gyro(2) -imu.gyro(3); ...
                                       imu.gyro(1) 0 imu.gyro(3) -imu.gyro(2); ...
                                       imu.gyro(2) -imu.gyro(3) 0 imu.gyro(1); ...
                                       imu.gyro(3) imu.gyro(2) -imu.gyro(1) 0];
    % Altitude and velocity part (upward positive)
    F(5, 6) = dt; % dh/dvh
    
    % 6. Predict covariance
    sys.P = F * sys.P * F' + sys.Q;
    
    % --- Update Step ---
    % Normalize measurements (avoid division by zero)
    accel_norm = norm(imu.accel);
    if accel_norm == 0, return; end
    z_acc = imu.accel / accel_norm; % Normalized accelerometer (body frame)
    
    mag_norm = norm(magNED);
    if mag_norm == 0, return; end
    z_mag = magNED / mag_norm; % Normalized magnetometer (NED frame)
    
    z_baro = alti; % Barometer measurement (BMP280, altitude up)
    
    % 7. Measurement function (h(x))
    % Accelerometer measurement model (gravity in body frame)
    h_acc = [2 * (q_pred(2) * q_pred(4) - q_pred(1) * q_pred(3)); ...
             2 * (q_pred(1) * q_pred(2) + q_pred(3) * q_pred(4)); ...
             q_pred(1)^2 - q_pred(2)^2 - q_pred(3)^2 + q_pred(4)^2];
    
    % Magnetometer measurement model (NED frame)
    h_global = quatmultiply(q_pred', quatmultiply([0, z_mag], quatinv(q_pred')))'; 
    b = [norm([h_global(2), h_global(3)]), 0, h_global(4)]; % Magnetic field components
    h_mag = [2 * b(1) * (0.5 - q_pred(3)^2 - q_pred(4)^2) + 2 * b(3) * (q_pred(2) * q_pred(4) - q_pred(1) * q_pred(3)); ...
             2 * b(1) * (q_pred(2) * q_pred(3) - q_pred(1) * q_pred(4)) + 2 * b(3) * (q_pred(1) * q_pred(2) + q_pred(3) * q_pred(4)); ...
             2 * b(1) * (q_pred(1) * q_pred(3) + q_pred(2) * q_pred(4)) + 2 * b(3) * (0.5 - q_pred(2)^2 - q_pred(3)^2)];
    
    % Barometer measurement model (altitude up)
    h_baro = h_pred;
    
    % Combined measurement vector
    z = [z_acc'; z_mag'; z_baro];
    h = [h_acc; h_mag; h_baro];
    
    % 8. Measurement Jacobian (H)
    H = zeros(7, 6); % 7 measurements (3 accel, 3 mag, 1 baro) x 6 states
    % Accelerometer Jacobian (wrt quaternion only)
    H(1:3, 1:4) = 2 * [-q_pred(3), q_pred(4), -q_pred(1), q_pred(2); ...
                       q_pred(2), q_pred(1), q_pred(4), q_pred(3); ...
                       q_pred(1), -q_pred(2), -q_pred(3), q_pred(4)];
    % Magnetometer Jacobian (wrt quaternion only)
    H(4:6, 1:4) = [ -2 * b(3) * q_pred(3),  2 * b(3) * q_pred(4), -4 * b(1) * q_pred(3) - 2 * b(3) * q_pred(1), -4 * b(1) * q_pred(4) + 2 * b(3) * q_pred(2); ...
                    -2 * b(1) * q_pred(4) + 2 * b(3) * q_pred(2), 2 * b(1) * q_pred(3) + 2 * b(3) * q_pred(1), 2 * b(1) * q_pred(2) + 2 * b(3) * q_pred(4), -2 * b(1) * q_pred(1) + 2 * b(3) * q_pred(3); ...
                     2 * b(1) * q_pred(3),  2 * b(1) * q_pred(4) - 4 * b(3) * q_pred(2), 2 * b(1) * q_pred(1) - 4 * b(3) * q_pred(3), 2 * b(1) * q_pred(2)];
    % Barometer Jacobian (wrt altitude only)
    H(7, 5) = 1;
    
    % 9. Combined measurement noise covariance
    R = blkdiag(sys.R_acc, sys.R_mag, sys.R_baro);
    
    % 10. Kalman update
    y = z - h; % Residual
    S = H * sys.P * H' + R; % Innovation covariance
    K = sys.P * H' / S; % Kalman gain
    sys.x = x_pred + K * y; % State update
    sys.P = (eye(6) - K * H) * sys.P; % Covariance update
    
    % 11. Normalize quaternion in updated state
    sys.x(1:4) = quatnormalize(sys.x(1:4)')';
    
    % 12. Compute Euler angles for output (NED convention)
    sys.euler = quat2eul(sys.x(1:4)'); % [yaw, pitch, roll] in radians
end