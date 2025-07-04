clc, clearvars, close all

%% Lab03: Generation, Processing, and Analysis of Sine Waves

%% 1) Read Data
fprintf('1) Start.\n')

% Load the signal data
data = readmatrix('signal.csv');
time = data(:,1); % First column is time
signal = data(:,2); % Second column is signal values

% Plot the signal
figure(1)
plot(time, signal)
title('Original Signal')
xlabel('Time (s)')
ylabel('Amplitude')
grid on

fprintf('1) Done.\n')

%% 2/3) Butterworth Filter
fprintf('2) Start.\n')

% Determine sampling frequency
Ts = time(2) - time(1); % Sampling interval
Fs = 1/Ts; % Sampling frequency

% Filter specifications
fp = 30; % Passband frequency (Hz)
fs = 200; % Stopband frequency (Hz)
Rp = 3; % Passband ripple (dB)
Rs = 40; % Stopband attenuation (dB)

% Calculate filter order and cutoff frequency
[n, Wn] = buttord(fp/(Fs/2), fs/(Fs/2), Rp, Rs);
fprintf('Butterworth filter order: %d\n', n);
fprintf('Cutoff frequency: %.2f Hz\n', Wn*(Fs/2));

% Design Butterworth filter
[b, a] = butter(n, Wn, 'low');

fprintf('2) Done.\n')

fprintf('3) Start.\n')

% Apply filter
filtered_signal = filtfilt(b, a, signal);

% Plot both signals
figure(1)
hold on
plot(time, filtered_signal, 'r')
title('Original and Filtered Signals')
xlabel('Time (s)')
ylabel('Amplitude')
legend('Original', 'Filtered')
grid on
hold off

fprintf('3) Done.\n')

%% 4. Fourier Transform
fprintf('4) Start.\n')

% 4.1) Calculate means
mean_orig = mean(signal);
mean_filt = mean(filtered_signal);
fprintf('Original signal mean: %.4f\n', mean_orig);
fprintf('Filtered signal mean: %.4f\n', mean_filt);

% 4.2) FFT calculations
N = length(signal);
f = Fs*(0:(N/2))/N; % Frequency vector (single-sided)

% Original signal FFT
Y_orig = fft(signal - mean_orig);
P2_orig = abs(Y_orig/N);
P1_orig = P2_orig(1:N/2+1);
P1_orig(2:end-1) = 2*P1_orig(2:end-1);

% Filtered signal FFT
Y_filt = fft(filtered_signal - mean_filt);
P2_filt = abs(Y_filt/N);
P1_filt = P2_filt(1:N/2+1);
P1_filt(2:end-1) = 2*P1_filt(2:end-1);

% 4.2.3) Plot spectra up to 500 Hz
max_freq = 500;
idx = f <= max_freq;

figure(2)
plot(f(idx), P1_orig(idx), 'b')
hold on
plot(f(idx), P1_filt(idx), 'r')
title('Single-Sided Amplitude Spectrum')
xlabel('Frequency (Hz)')
ylabel('|P1(f)|')
legend('Original', 'Filtered')
grid on
hold off

fprintf('4) Done.\n')

%% 5. Frequency Identification
fprintf('5) Start.\n')

% Find signal frequency
[signal_freq, amplitude] = find_signal_frequency(f, P1_filt);
fprintf('Detected signal frequency: %.2f Hz\n', signal_freq);
fprintf('Signal amplitude: %.4f\n', amplitude);

fprintf('5) Done.\n')

%% Function Definition
function [freq, amp] = find_signal_frequency(f, P1)
    % Find the peak in the spectrum (ignoring DC component)
    [amp, idx] = max(P1(2:end));
    freq = f(idx+1); % +1 because we skipped DC component
end
