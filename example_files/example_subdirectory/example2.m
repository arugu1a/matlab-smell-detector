%% Generic MATLAB Analysis Script
% This script demonstrates various MATLAB functionalities including
% data generation, signal processing, statistical analysis, and visualization
% Author: Claude AI
% Date: July 2025

clear all; close all; clc;

%% Section 1: Data Generation and Basic Operations
fprintf('=== Starting Generic MATLAB Analysis ===\n');

% Generate time vector
fs = 1000; % Sampling frequency
t = 0:1/fs:10; % Time vector from 0 to 10 seconds
N = length(t);

% Generate synthetic signals
f1 = 5; % Frequency 1
f2 = 15; % Frequency 2
f3 = 30; % Frequency 3

% Create composite signal with noise
signal = 2*sin(2*pi*f1*t) + 1.5*cos(2*pi*f2*t) + 0.8*sin(2*pi*f3*t);
noise = 0.5*randn(size(t));
noisy_signal = signal + noise;

% Generate random data matrices
data_matrix = randn(100, 5) * 10 + 50; % 100x5 matrix
correlation_matrix = corrcoef(data_matrix);

fprintf('Generated %d samples of synthetic data\n', N);
fprintf('Signal contains frequencies: %.1f, %.1f, %.1f Hz\n', f1, f2, f3);

%% Section 2: Statistical Analysis
fprintf('\n=== Statistical Analysis ===\n');

% Basic statistics
mean_values = mean(data_matrix);
std_values = std(data_matrix);
median_values = median(data_matrix);
min_values = min(data_matrix);
max_values = max(data_matrix);

% Display statistics table
fprintf('Column\tMean\t\tStd\t\tMedian\t\tMin\t\tMax\n');
fprintf('-----\t----\t\t---\t\t------\t\t---\t\t---\n');
for i = 1:size(data_matrix, 2)
    fprintf('%d\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n', ...
        i, mean_values(i), std_values(i), median_values(i), ...
        min_values(i), max_values(i));
end

% Histogram analysis
histogram_data = [];
for i = 1:size(data_matrix, 2)
    [counts, centers] = hist(data_matrix(:,i), 20);
    histogram_data = [histogram_data; counts];
end

%% Section 3: Signal Processing
fprintf('\n=== Signal Processing Analysis ===\n');

% FFT Analysis
Y = fft(noisy_signal);
P2 = abs(Y/N);
P1 = P2(1:N/2+1);
P1(2:end-1) = 2*P1(2:end-1);
f = fs*(0:(N/2))/N;

% Find peaks in frequency domain
[pks, locs] = findpeaks(P1, 'MinPeakHeight', 0.1);
peak_frequencies = f(locs);

fprintf('Detected peak frequencies: ');
for i = 1:length(peak_frequencies)
    fprintf('%.1f Hz ', peak_frequencies(i));
end
fprintf('\n');

% Filtering
% Design low-pass filter
fc = 25; % Cutoff frequency
[b, a] = butter(4, fc/(fs/2), 'low');
filtered_signal = filter(b, a, noisy_signal);

% Design band-pass filter
f_low = 10;
f_high = 20;
[b_bp, a_bp] = butter(2, [f_low f_high]/(fs/2), 'bandpass');
bandpass_signal = filter(b_bp, a_bp, noisy_signal);

%% Section 4: Mathematical Operations
fprintf('\n=== Mathematical Operations ===\n');

% Matrix operations
A = randn(5, 5);
B = randn(5, 5);
C = A * B;
D = A + B;
E = A - B;

% Eigenvalue analysis
[V, lambda] = eig(A);
eigenvalues = diag(lambda);

fprintf('Matrix A eigenvalues:\n');
for i = 1:length(eigenvalues)
    if imag(eigenvalues(i)) == 0
        fprintf('λ%d = %.3f\n', i, real(eigenvalues(i)));
    else
        fprintf('λ%d = %.3f + %.3fi\n', i, real(eigenvalues(i)), imag(eigenvalues(i)));
    end
end

% Polynomial operations
p = [1, -3, 2, 5]; % Coefficients of polynomial
roots_p = roots(p);
x_poly = linspace(-3, 3, 100);
y_poly = polyval(p, x_poly);

% Differential equation solving (simple example)
% dy/dt = -2y + sin(t)
tspan = [0 5];
y0 = 1;
[t_ode, y_ode] = ode45(@(t,y) -2*y + sin(t), tspan, y0);

%% Section 5: Data Analysis and Curve Fitting
fprintf('\n=== Curve Fitting Analysis ===\n');

% Generate data for curve fitting
x_data = linspace(0, 10, 50);
y_true = 2*exp(-0.5*x_data) + 0.1*x_data.^2;
y_data = y_true + 0.2*randn(size(x_data));

% Polynomial fitting
p_fit = polyfit(x_data, y_data, 3);
y_fit = polyval(p_fit, x_data);

% Calculate R-squared
ss_res = sum((y_data - y_fit).^2);
ss_tot = sum((y_data - mean(y_data)).^2);
r_squared = 1 - (ss_res / ss_tot);

fprintf('Polynomial fit R-squared: %.4f\n', r_squared);

% Moving average
window_size = 5;
y_smooth = movmean(y_data, window_size);

%% Section 6: Optimization
fprintf('\n=== Optimization Example ===\n');

% Define objective function
objective = @(x) (x(1) - 2).^2 + (x(2) - 1).^2 + x(1)*x(2);

% Initial guess
x0 = [0; 0];

% Optimize using fminunc
options = optimset('Display', 'off');
[x_opt, fval] = fminunc(objective, x0, options);

fprintf('Optimization results:\n');
fprintf('Optimal x: [%.3f, %.3f]\n', x_opt(1), x_opt(2));
fprintf('Minimum value: %.6f\n', fval);

%% Section 7: File I/O Operations
fprintf('\n=== File Operations ===\n');

% Create sample data structure
sample_data.timestamp = datetime('now');
sample_data.parameters = struct('fs', fs, 'N', N, 'frequencies', [f1, f2, f3]);
sample_data.results = struct('mean_vals', mean_values, 'eigenvals', eigenvalues);
sample_data.signals = struct('time', t(1:100), 'original', signal(1:100), ...
                            'filtered', filtered_signal(1:100));

% Save data (commented out to avoid creating files)
% save('analysis_results.mat', 'sample_data');
% fprintf('Data saved to analysis_results.mat\n');

%% Section 8: Advanced Array Operations
fprintf('\n=== Advanced Array Operations ===\n');

% Multidimensional arrays
tensor_3d = randn(10, 10, 5);
tensor_sum = sum(tensor_3d, 3);
tensor_mean = mean(tensor_3d, 3);

% Reshaping and indexing
reshaped_data = reshape(data_matrix, 25, 20);
selected_data = data_matrix(data_matrix(:,1) > mean(data_matrix(:,1)), :);

fprintf('3D tensor dimensions: %dx%dx%d\n', size(tensor_3d));
fprintf('Selected %d rows based on first column criteria\n', size(selected_data, 1));

% Logical operations
logical_mask = data_matrix > 45;
masked_data = data_matrix(logical_mask);
percentage_above = (sum(logical_mask(:)) / numel(data_matrix)) * 100;

fprintf('%.1f%% of data points are above threshold\n', percentage_above);

%% Section 9: String and Cell Array Operations
fprintf('\n=== String and Cell Operations ===\n');

% Cell arrays
cell_data = {'Analysis', 'Results', 'MATLAB', 'Processing'};
numeric_cells = {data_matrix, correlation_matrix, eigenvalues};

% String operations
analysis_name = 'Generic_MATLAB_Analysis';
parts = strsplit(analysis_name, '_');
reconstructed = strjoin(parts, ' ');

fprintf('Original: %s\n', analysis_name);
fprintf('Reconstructed: %s\n', reconstructed);

% Character arrays
char_array = char(cell_data);
fprintf('Character array dimensions: %dx%d\n', size(char_array));

%% Section 10: Performance Timing
fprintf('\n=== Performance Analysis ===\n');

% Time different operations
tic;
large_matrix = randn(1000, 1000);
large_multiply = large_matrix * large_matrix';
time_multiply = toc;

tic;
large_svd = svd(large_matrix);
time_svd = toc;

tic;
large_eig = eig(large_matrix);
time_eig = toc;

fprintf('Matrix multiplication (1000x1000): %.4f seconds\n', time_multiply);
fprintf('SVD decomposition (1000x1000): %.4f seconds\n', time_svd);
fprintf('Eigenvalue computation (1000x1000): %.4f seconds\n', time_eig);

%% Section 11: Control Flow Examples
fprintf('\n=== Control Flow Examples ===\n');

% For loop with conditional
count_positive = 0;
count_negative = 0;
sample_array = randn(1, 100);

for i = 1:length(sample_array)
    if sample_array(i) > 0
        count_positive = count_positive + 1;
    else
        count_negative = count_negative + 1;
    end
end

fprintf('Positive values: %d, Negative values: %d\n', count_positive, count_negative);

% While loop example
iteration = 0;
convergence_value = 1000;
tolerance = 1e-6;

while convergence_value > tolerance && iteration < 100
    iteration = iteration + 1;
    convergence_value = convergence_value * 0.9;
end

fprintf('Converged after %d iterations\n', iteration);

% Switch statement example
analysis_type = 'frequency';
switch analysis_type
    case 'time'
        fprintf('Performing time-domain analysis\n');
    case 'frequency'
        fprintf('Performing frequency-domain analysis\n');
    case 'statistical'
        fprintf('Performing statistical analysis\n');
    otherwise
        fprintf('Unknown analysis type\n');
end

%% Section 12: Function Handles and Anonymous Functions
fprintf('\n=== Function Handles ===\n');

% Anonymous functions
square_func = @(x) x.^2;
cube_func = @(x) x.^3;
custom_func = @(x, a, b) a*exp(-b*x);

% Test functions
test_values = [1, 2, 3, 4, 5];
squared_values = square_func(test_values);
cubed_values = cube_func(test_values);
custom_values = custom_func(test_values, 2, 0.5);

fprintf('Test values: ');
fprintf('%.0f ', test_values);
fprintf('\nSquared: ');
fprintf('%.0f ', squared_values);
fprintf('\nCubed: ');
fprintf('%.0f ', cubed_values);
fprintf('\n');

%% Section 13: Error Handling
fprintf('\n=== Error Handling Example ===\n');

try
    % Attempt potentially problematic operation
    result = sqrt(-1); % This will produce a complex result, not an error
    fprintf('Square root of -1: %.3f + %.3fi\n', real(result), imag(result));
    
    % Force an error for demonstration
    problematic_matrix = randn(3, 4);
    inverse_result = inv(problematic_matrix); % This will cause an error
    
catch ME
    fprintf('Caught error: %s\n', ME.message);
    fprintf('Error occurred in function: %s\n', ME.stack(1).name);
end

%% Section 14: Final Summary
fprintf('\n=== Analysis Summary ===\n');
fprintf('Total execution completed successfully\n');
fprintf('Generated %d data points across %d variables\n', size(data_matrix, 1), size(data_matrix, 2));
fprintf('Processed %.1f seconds of signal data\n', max(t));
fprintf('Performed %d different analysis sections\n', 14);
fprintf('Peak memory usage: %.2f MB\n', memory_usage());

% Helper function for memory usage (if available)
function mem_mb = memory_usage()
    try
        mem_info = memory;
        mem_mb = mem_info.MemUsedMATLAB / 1024 / 1024;
    catch
        mem_mb = 0; % Return 0 if memory function not available
    end
end

fprintf('\n=== Script Execution Complete ===\n');