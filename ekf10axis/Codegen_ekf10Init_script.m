% CODEGEN_EKF10INIT_SCRIPT   Generate static library ekf10filter from ekf10Init,
%  ekf10Step, initStateEKF.
% 
% Script generated from project 'lib_ekf10Init.prj' on 21-May-2025.
% 
% See also CODER, CODER.CONFIG, CODER.TYPEOF, CODEGEN.

clear
clc

%% Create configuration object of class 'coder.EmbeddedCodeConfig'.
cfg = coder.config('lib','ecoder',true);
newObj = coder.MATLABCodeTemplate('myTemplate.cgt');
cfg.CodeTemplate = newObj;
cfg.Hardware = coder.hardware('STM32F4-Discovery');
cfg.HardwareImplementation.TargetHWDeviceType = 'ARM Compatible->ARM Cortex-M';
cfg.ConvertIfToSwitch = true;
cfg.FilePartitionMethod = 'SingleFile';
cfg.GenerateReport = true;
cfg.PreserveArrayDimensions = true;
cfg.PreserveInputData = true;
cfg.ReportPotentialDifferences = false;
cfg.MultiInstanceCode = true;
cfg.OptimizeReductions = true;
cfg.GenCodeOnly = true;

%% Define argument types for entry-point 'ekf10Init'.
ARGS = cell(3,1);
ARGS{1} = cell(2,1);
ARGS{1}{1} = coder.typeof(0);
ARGS{1}{2} = coder.typeof(0,[6 1]);

%% Define argument types for entry-point 'ekf10Step'.
ARGS{2} = cell(4,1);
ARGS_2_1 = struct;
ARGS_2_1.euler = coder.typeof(0,[1 3]);
ARGS_2_1.dt = coder.typeof(0);
ARGS_2_1.Q = coder.typeof(0,[6 6]);
ARGS_2_1.R_acc = coder.typeof(0,[3 3]);
ARGS_2_1.R_mag = coder.typeof(0,[3 3]);
ARGS_2_1.R_baro = coder.typeof(0);
ARGS_2_1.x = coder.typeof(0,[6 1]);
ARGS_2_1.P = coder.typeof(0,[6 6]);
ARGS{2}{1} = coder.typeof(ARGS_2_1);
ARGS{2}{1} = coder.cstructname(ARGS{2}{1},'sysInit_t');
ARGS_2_2 = struct;
ARGS_2_2.gyro = coder.typeof(0,[1 3]);
ARGS_2_2.accel = coder.typeof(0,[1 3]);
ARGS{2}{2} = coder.typeof(ARGS_2_2);
ARGS{2}{2} = coder.cstructname(ARGS{2}{2},'imuData_t');
ARGS{2}{3} = coder.typeof(0,[1 3]);
ARGS{2}{4} = coder.typeof(0);

%% Define argument types for entry-point 'initStateEKF'.
ARGS{3} = cell(3,1);
ARGS{3}{1} = coder.typeof(0,[1 3]);
ARGS{3}{2} = coder.typeof(0,[1 3]);
ARGS{3}{3} = coder.typeof(0);

%% Invoke MATLAB Coder.
codegen -config cfg -o ekf10filter ekf10Init -args ARGS{1} ekf10Step -args ARGS{2} initStateEKF -args ARGS{3}

