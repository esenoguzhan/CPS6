clc, clearvars, close all

%% Lab03: Generation, Processing, and Analysis of Sine Waves

%% 1) Read Data
fprintf('1) Start.\n')

data = readmatrix('signal.csv');
t = data(:, 1);           % Time vector
x = data(:, 2);           % Signal vector

% Plot the signal
figure(1)
plot(t, x)
title('Original Signal')
xlabel('Time [s]')
ylabel('Amplitude [V]')
grid on

fprintf('1) Done.\n')

%% 2/3) Butterworth Filter

fprintf('2) Start.\n')

% Sampling frequency
fs = 1 / mean(diff(t));   % Hz

% Filter specs
Wp = 30 / (fs / 2);       % Passband (normalized)
Ws = 200 / (fs / 2);      % Stopband (normalized)
Rp = 3;                   % Passband ripple (dB)
Rs = 40;                  % Stopband attenuation (dB)

% Calculate order and cutoff
[n, Wn] = buttord(Wp, Ws, Rp, Rs);
fprintf('Filter order: %d\nCutoff frequency (normalized): %.4f\n', n, Wn);

fprintf('2) Done.\n')

fprintf('3) Start.\n')

% Coefficients
[b, a] = butter(n, Wn);

% Filter the signal
x_filtered = filter(b, a, x);   %filtfilt(b, a, x);

% Plot both signals
figure(1)
hold on
plot(t, x_filtered, 'r')
legend('Original', 'Filtered')
title('Original and Filtered Signal')
xlabel('Time [s]')
ylabel('Amplitude [V]')
grid on

fprintf('3) Done.\n')

%% 4. Fourier Transform
fprintf('4) Start.\n')

% Signal lengths
N = length(x);

% Mean values
mean_orig = mean(x);
mean_filt = mean(x_filtered);
fprintf('Mean (original): %.4f V\nMean (filtered): %.4f V\n', mean_orig, mean_filt)

% Remove DC
x_noDC = x - mean_orig;
x_filt_noDC = x_filtered - mean_filt;

% FFTs
X = abs(fft(x_noDC) / N);
X_filt = abs(fft(x_filt_noDC) / N);

f = (0:N/2-1) * fs / N;  % One-sided frequency vector

figure(2)
plot(f, 2*X(1:N/2), 'b'); hold on
plot(f, 2*X_filt(1:N/2), 'r')
xlim([0 500])
legend('Original', 'Filtered')
xlabel('Frequency [Hz]')
ylabel('Amplitude')
title('Single-Sided Amplitude Spectrum')
grid on

fprintf('4) Done.\n')

%% 5. Frequency Identification
fprintf('5) Start.\n')

[amp, freq] = detect_frequency(x_filt_noDC, fs);
fprintf('Detected Frequency: %.2f Hz, Amplitude: %.2f\n', freq, amp)

fprintf('5) Done.\n')

%% Define Frequency Detection Function
function [amplitude, frequency] = detect_frequency(signal, fs)
    N = length(signal);
    signal = signal - mean(signal);
    Y = abs(fft(signal) / N);
    Y = Y(1:N/2);                  % One-sided spectrum
    f = (0:N/2-1) * fs / N;

    [amplitude, idx] = max(Y(2:end));  % Skip DC (index 1)
    frequency = f(idx + 1);            % +1 to correct for skipping DC
end

