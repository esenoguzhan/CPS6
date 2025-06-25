clc, clearvars, close all

%% Lab03: Generation, Processing, and Analysis of Sine Waves

%% 1) Read Data
% Load the provided signal 'signal.csv' using the 'csvread()' function and
% split the signal and time into two separate vectors.

fprintf('1) Start.\n')

data = readmatrix('signal.csv');

sample_interval = data(2,2);
Fs = 1 / sample_interval;
t = (0:2499) * sample_interval;
signal = data(1:2500, 4);

% Plot the signal
figure(1)
plot(t, signal)
title('Original Signal')
xlabel('Time (s)')
ylabel('Amplitude (V)')
grid on

fprintf('1) Done.\n')

%% 2/3) Butterworth Filter
% Design a Butterworth filter that fulfills the required characteristics 
% given in the assignment description. Use the built-in functions of
% Matlab. The 'doc' and 'help' functions might be useful to obtain detailed
% information.

% 2) First, calculate the required filter order and cutoff frequency and
% print the result.

fprintf('2) Start.\n')

wp = 30 / (Fs / 2);
ws = 200 / (Fs / 2);
%[gpass, gstop] = deal(3, 40);
[N, Wn] = buttord(wp, ws, 3, 40);
fprintf('Butterworth filter order: %d, Cutoff frequency (normalized): %.4f\n', N, Wn)

fprintf('2) Done.\n')

% 3) Calculate the filter coefficients and apply them to the signal, i.e.,
% filter the signal. Plot the filtered signal into the same figure as the
% original signal. Make sure to add a title, the axis descriptions, and a
% legend.

fprintf('3) Start.\n')

[b, a] = butter(N, Wn, 'low');
filtered_signal = filtfilt(b, a, signal);

figure(1)
hold on
plot(t, filtered_signal, 'LineWidth', 2)
legend('Original Signal', 'Filtered Signal')

fprintf('3) Done.\n')

%% 4. Fourier Transform
% Calculate the single-sided Fourier transform of the filtered signal.

fprintf('4) Start.\n')

% 4.1) First, obtain the length of the original and filtered signal and 
% calculate their means. Print both mean values.

N = length(signal);
mean_orig = mean(signal);
mean_filt = mean(filtered_signal);
fprintf('Original Mean: %.4f, Filtered Mean: %.4f\n', mean_orig, mean_filt)

% 4.2) Do the FFT for both signals; see the docs and lecture slides for
% help. Make sure to remove the mean from the signals.

% 4.2.1.) Original signal
orig_fft = abs(fft(signal - mean_orig)) / N;

% 4.2.2) Filtered signal
filt_fft = abs(fft(filtered_signal - mean_filt)) / N;

% 4.2.3) When plotting, only visualize the spectrum of to 500 Hz.

f = (0:N/2) * (Fs / N);

figure(2)
plot(f, orig_fft(1:N/2+1), 'DisplayName', 'Original Signal'); hold on;
plot(f, filt_fft(1:N/2+1), 'LineWidth', 2, 'DisplayName', 'Filtered Signal');
xlim([0 500])
title('FFT of Original and Filtered Signal')
xlabel('Frequency (Hz)')
ylabel('Magnitude')
legend
grid on

fprintf('4) Done.\n')

%% 5. Frequency Identification
% Write a function that automatically detects a signals frequency based
% on its frequency spectrum. You can assume there's only a single signal
% and noise has been removed. The function must return the amplitude and
% the frequency of this signal.

fprintf('5) Start.\n')

% 5.2) What is the frequency of the signal you have analyzed?
[freq, amp] = detect_frequency(filtered_signal, Fs);
fprintf('Detected frequency: %.2f Hz, Amplitude: %.4f\n', freq, amp)

fprintf('5) Done.\n')

% 5.1) Define function
function [freq, amplitude] = detect_frequency(signal, Fs)
    N = length(signal);
    Y = abs(fft(signal - mean(signal))) / N;
    freqs = (0:N/2) * (Fs / N);
    [amplitude, idx] = max(Y(1:N/2+1));
    freq = freqs(idx);
end
