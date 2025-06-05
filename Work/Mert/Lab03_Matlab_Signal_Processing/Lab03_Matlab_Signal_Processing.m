clc, clearvars, close all

%% Lab03: Generation, Processing, and Analysis of Sine Waves

%% 1) Read Data
% Load the provided signal 'signal.csv' using the 'csvread()' function and
% split the signal and time into two separate vectors.

fprintf('1) Start.\n')

%{ 
    Add your code here
%}

% Plot the signal
figure(1)

%{ 
    Add your code here
%}

fprintf('1) Done.\n')

%% 2/3) Butterworth Filter
% Design a Butterworth filter that fulfills the required characteristics 
% given in the assignment description. Use the built-in functions of
% Matlab. The 'doc' and 'help' functions might be useful to obtain detailed
% information.

% 2) First, calculate the required filter order and cutoff frequency and
% print the result.

fprintf('2) Start.\n')

%{ 
    Add your code here
%}

fprintf('2) Done.\n')

% 3) Calculate the filter coefficients and apply them to the signal, i.e.,
% filter the signal. Plot the filtered signal into the same figure as the
% original signal. Make sure to add a title, the axis descriptions, and a
% legend.

fprintf('3) Start.\n')

%{ 
    Add your code here
%}

fprintf('3) Done.\n')

%% 4. Fourier Transform
% Calculate the single-sided Fourier transform of the filtered signal.

fprintf('4) Start.\n')

% 4.1) First, obtain the length of the original and filtered signal and 
% calculate their means. Print both mean values.

%{ 
    Add your code here
%}


% 4.2) Do the FFT for both signals; see the docs and lecture slides for
% help. Make sure to remove the mean from the signals.

% 4.2.1.) Original signal

%{ 
    Add your code here
%}

% 4.2.2) Filtered signal

%{ 
    Add your code here
%}


% 4.2.3) When plotting, only visualize the spectrum of to 500 Hz.


figure(2)

%{ 
    Add your code here
%}

fprintf('4) Done.\n')

%% 5. Frequency Identification
% Write a function that automatically detects a signals frequency based
% on its frequency spectrum. You can assume there's only a single signal
% and noise has been removed. The function must return the amplitude and
% the frequency of this signal.

fprintf('5) Start.\n')

% 5.2) What is the frequency of they signal you have analyzed?

%{ 
    Add your code here
%}

fprintf('5) Done.\n')

% 5.1) Define function

%{ 
    Add your code here
%}

 
