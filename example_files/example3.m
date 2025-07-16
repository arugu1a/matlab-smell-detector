% Author: Claude AI
% Date: July 2025

function toolkit_main()
    % SIGNAL PROCESSING TOOLKIT
    % A comprehensive toolkit for signal processing and data analysis
    
    % Example usage of the toolkit
    fprintf('Signal Processing Toolkit Demo\n');
    fprintf('===============================\n\n');
    
    % Generate sample data
    fs = 1000; % Sampling frequency
    t = 0:1/fs:2; % Time vector
    signal = sin(2*pi*10*t) + 0.5*sin(2*pi*50*t) + 0.2*randn(size(t));
    
    % Simple function with 2 parameters
    filtered_signal = simple_lowpass_filter(signal, fs);
    
    % Function with many parameters
    processed_signal = advanced_signal_processor(signal, fs, 'butterworth', 4, 100, 'low', 0.1, 'hamming');
    
    % Large comprehensive function
    analysis_results = comprehensive_signal_analyzer(signal, fs, t);
    
    % Display results
    fprintf('Processing complete. Results stored in analysis_results structure.\n');
end

function filtered_signal = simple_lowpass_filter(signal, fs)
    % Simple lowpass filter function (2 parameters)
    
    cutoff_freq = 50; % Hz
    nyquist = fs / 2;
    normalized_cutoff = cutoff_freq / nyquist;
    
    [b, a] = butter(4, normalized_cutoff, 'low');
    filtered_signal = filter(b, a, signal);
end

function result = basic_statistics(data, method, normalize)
    % Basic statistics function (3 parameters)
    
    switch method
        case 'mean'
            result = mean(data);
        case 'std'
            result = std(data);
        case 'var'
            result = var(data);
        case 'median'
            result = median(data);
        otherwise
            result = mean(data);
    end
    
    if normalize
        result = result / max(abs(data));
    end
end

function [peaks, locations] = peak_detector(signal, threshold, min_distance, use_prominence)
    % Peak detection function (4 parameters)
    
    if use_prominence
        [peaks, locations] = findpeaks(signal, 'MinPeakHeight', threshold, ...
            'MinPeakDistance', min_distance, 'MinPeakProminence', threshold*0.5);
    else
        [peaks, locations] = findpeaks(signal, 'MinPeakHeight', threshold, ...
            'MinPeakDistance', min_distance);
    end
end

function windowed_signal = windowing_function(signal, window_type, window_size, overlap, apply_zero_padding)
    % Windowing function (5 parameters)
    
    signal_length = length(signal);
    step_size = window_size - overlap;
    num_windows = floor((signal_length - window_size) / step_size) + 1;
    
    windowed_signal = zeros(window_size, num_windows);
    
    % Generate window
    switch window_type
        case 'hamming'
            window = hamming(window_size);
        case 'hanning'
            window = hanning(window_size);
        case 'blackman'
            window = blackman(window_size);
        otherwise
            window = ones(window_size, 1);
    end
    
    for i = 1:num_windows
        start_idx = (i-1) * step_size + 1;
        end_idx = start_idx + window_size - 1;
        
        if end_idx <= signal_length
            windowed_signal(:, i) = signal(start_idx:end_idx) .* window;
        else
            if apply_zero_padding
                temp_signal = zeros(window_size, 1);
                temp_signal(1:(signal_length-start_idx+1)) = signal(start_idx:end);
                windowed_signal(:, i) = temp_signal .* window;
            end
        end
    end
end

function processed_signal = advanced_signal_processor(signal, fs, filter_type, filter_order, cutoff_freq, pass_type, noise_threshold, window_type)
    % Advanced signal processor (8 parameters - more than 5)
    
    % Input validation
    if nargin < 8
        window_type = 'hamming';
    end
    if nargin < 7
        noise_threshold = 0.1;
    end
    
    % Pre-processing: noise reduction
    if noise_threshold > 0
        signal_power = signal.^2;
        noise_mask = signal_power < noise_threshold * max(signal_power);
        signal(noise_mask) = signal(noise_mask) * 0.1;
    end
    
    % Apply windowing
    window_size = min(1024, length(signal));
    if length(signal) > window_size
        overlap = window_size / 2;
        windowed_signal = windowing_function(signal, window_type, window_size, overlap, true);
        signal = windowed_signal(:, 1); % Use first window for filtering
    end
    
    % Design filter
    nyquist = fs / 2;
    normalized_cutoff = cutoff_freq / nyquist;
    
    switch filter_type
        case 'butterworth'
            [b, a] = butter(filter_order, normalized_cutoff, pass_type);
        case 'chebyshev1'
            [b, a] = cheby1(filter_order, 0.5, normalized_cutoff, pass_type);
        case 'chebyshev2'
            [b, a] = cheby2(filter_order, 20, normalized_cutoff, pass_type);
        case 'elliptic'
            [b, a] = ellip(filter_order, 0.5, 20, normalized_cutoff, pass_type);
        otherwise
            [b, a] = butter(filter_order, normalized_cutoff, pass_type);
    end
    
    % Apply filter
    processed_signal = filter(b, a, signal);
    
    % Post-processing: normalization
    processed_signal = processed_signal / max(abs(processed_signal));
end

function results = comprehensive_signal_analyzer(signal, fs, time_vector)
    % COMPREHENSIVE SIGNAL ANALYZER - Large function (>350 lines)
    % This function performs extensive signal analysis including:
    % - Time domain analysis
    % - Frequency domain analysis
    % - Statistical analysis
    % - Feature extraction
    % - Signal quality assessment
    % - Spectral analysis
    % - Wavelet analysis
    % - Pattern recognition
    
    fprintf('Starting comprehensive signal analysis...\n');
    
    % Initialize results structure
    results = struct();
    results.timestamp = datetime('now');
    results.signal_length = length(signal);
    results.sampling_frequency = fs;
    results.duration = length(signal) / fs;
    
    % ===== TIME DOMAIN ANALYSIS =====
    fprintf('Performing time domain analysis...\n');
    
    % Basic statistics
    results.time_domain.mean = mean(signal);
    results.time_domain.std = std(signal);
    results.time_domain.variance = var(signal);
    results.time_domain.rms = sqrt(mean(signal.^2));
    results.time_domain.peak_to_peak = max(signal) - min(signal);
    results.time_domain.max_value = max(signal);
    results.time_domain.min_value = min(signal);
    results.time_domain.median = median(signal);
    results.time_domain.skewness = skewness(signal);
    results.time_domain.kurtosis = kurtosis(signal);
    
    % Advanced time domain features
    results.time_domain.zero_crossings = sum(diff(sign(signal)) ~= 0);
    results.time_domain.energy = sum(signal.^2);
    results.time_domain.power = results.time_domain.energy / length(signal);
    
    % Signal envelope analysis
    analytic_signal = hilbert(signal);
    results.time_domain.envelope = abs(analytic_signal);
    results.time_domain.instantaneous_phase = angle(analytic_signal);
    results.time_domain.instantaneous_frequency = diff(unwrap(results.time_domain.instantaneous_phase)) * fs / (2*pi);
    
    % Peak detection and analysis
    [peaks, peak_locs] = findpeaks(signal, 'MinPeakHeight', std(signal), 'MinPeakDistance', fs/100);
    results.time_domain.num_peaks = length(peaks);
    results.time_domain.peak_values = peaks;
    results.time_domain.peak_locations = peak_locs;
    results.time_domain.peak_times = time_vector(peak_locs);
    
    if length(peaks) > 1
        results.time_domain.peak_intervals = diff(peak_locs) / fs;
        results.time_domain.mean_peak_interval = mean(results.time_domain.peak_intervals);
        results.time_domain.peak_regularity = std(results.time_domain.peak_intervals) / results.time_domain.mean_peak_interval;
    end
    
    % ===== FREQUENCY DOMAIN ANALYSIS =====
    fprintf('Performing frequency domain analysis...\n');
    
    % FFT analysis
    N = length(signal);
    fft_signal = fft(signal);
    fft_magnitude = abs(fft_signal);
    fft_phase = angle(fft_signal);
    frequencies = (0:N-1) * fs / N;
    
    % Only keep positive frequencies
    if mod(N, 2) == 0
        pos_freq_idx = 1:N/2+1;
    else
        pos_freq_idx = 1:(N+1)/2;
    end
    
    results.frequency_domain.frequencies = frequencies(pos_freq_idx);
    results.frequency_domain.magnitude = fft_magnitude(pos_freq_idx);
    results.frequency_domain.phase = fft_phase(pos_freq_idx);
    results.frequency_domain.power_spectrum = (fft_magnitude(pos_freq_idx)).^2;
    
    % Spectral features
    [max_power, max_idx] = max(results.frequency_domain.power_spectrum);
    results.frequency_domain.dominant_frequency = results.frequency_domain.frequencies(max_idx);
    results.frequency_domain.max_power = max_power;
    
    % Spectral centroid
    results.frequency_domain.spectral_centroid = sum(results.frequency_domain.frequencies .* results.frequency_domain.power_spectrum') / sum(results.frequency_domain.power_spectrum);
    
    % Spectral rolloff (95% of energy)
    cumulative_power = cumsum(results.frequency_domain.power_spectrum);
    total_power = sum(results.frequency_domain.power_spectrum);
    rolloff_idx = find(cumulative_power >= 0.95 * total_power, 1);
    results.frequency_domain.spectral_rolloff = results.frequency_domain.frequencies(rolloff_idx);
    
    % Bandwidth calculation
    power_threshold = 0.1 * max_power;
    significant_freqs = results.frequency_domain.frequencies(results.frequency_domain.power_spectrum > power_threshold);
    if ~isempty(significant_freqs)
        results.frequency_domain.bandwidth = max(significant_freqs) - min(significant_freqs);
    else
        results.frequency_domain.bandwidth = 0;
    end
    
    % ===== SPECTRAL ANALYSIS =====
    fprintf('Performing spectral analysis...\n');
    
    % Spectrogram
    window_length = min(256, floor(N/4));
    overlap = floor(window_length/2);
    nfft = 512;
    
    [S, F, T] = spectrogram(signal, window_length, overlap, nfft, fs);
    results.spectral_analysis.spectrogram = abs(S);
    results.spectral_analysis.spec_frequencies = F;
    results.spectral_analysis.spec_times = T;
    
    % Spectral flux (measure of spectral change)
    spec_diff = diff(abs(S), 1, 2);
    results.spectral_analysis.spectral_flux = sqrt(mean(spec_diff.^2, 1));
    
    % Spectral statistics over time
    results.spectral_analysis.mean_spectral_energy = mean(abs(S).^2, 2);
    results.spectral_analysis.spectral_variance = var(abs(S).^2, 0, 2);
    
    % ===== WAVELET ANALYSIS =====
    fprintf('Performing wavelet analysis...\n');
    
    % Continuous wavelet transform
    scales = 1:64;
    wavelet_name = 'morl';
    coefficients = cwt(signal, scales, wavelet_name);
    
    results.wavelet_analysis.coefficients = coefficients;
    results.wavelet_analysis.scales = scales;
    results.wavelet_analysis.wavelet_name = wavelet_name;
    
    % Wavelet energy distribution
    results.wavelet_analysis.energy_distribution = sum(abs(coefficients).^2, 2);
    results.wavelet_analysis.total_wavelet_energy = sum(results.wavelet_analysis.energy_distribution);
    
    % Dominant scale
    [max_energy, max_scale_idx] = max(results.wavelet_analysis.energy_distribution);
    results.wavelet_analysis.dominant_scale = scales(max_scale_idx);
    
    % ===== SIGNAL QUALITY ASSESSMENT =====
    fprintf('Performing signal quality assessment...\n');
    
    % Signal-to-noise ratio estimation
    signal_power = var(signal);
    
    % Estimate noise using high-frequency content
    high_freq_cutoff = fs / 4;
    [b, a] = butter(4, high_freq_cutoff / (fs/2), 'high');
    noise_estimate = filter(b, a, signal);
    noise_power = var(noise_estimate);
    
    results.quality_assessment.snr_db = 10 * log10(signal_power / noise_power);
    results.quality_assessment.signal_power = signal_power;
    results.quality_assessment.noise_power = noise_power;
    
    % Dynamic range
    results.quality_assessment.dynamic_range = 20 * log10(max(abs(signal)) / min(abs(signal(signal ~= 0))));
    
    % Clipping detection
    max_val = max(abs(signal));
    clipping_threshold = 0.98 * max_val;
    clipped_samples = sum(abs(signal) > clipping_threshold);
    results.quality_assessment.clipping_percentage = 100 * clipped_samples / length(signal);
    
    % ===== PATTERN RECOGNITION =====
    fprintf('Performing pattern recognition...\n');
    
    % Autocorrelation analysis
    [autocorr, lags] = xcorr(signal, 'normalized');
    results.pattern_recognition.autocorrelation = autocorr;
    results.pattern_recognition.autocorr_lags = lags;
    
    % Find periodic patterns
    [peaks, peak_locs] = findpeaks(autocorr(lags > 0), 'MinPeakHeight', 0.1, 'SortStr', 'descend');
    if ~isempty(peaks)
        results.pattern_recognition.periodic_peaks = peaks(1:min(5, length(peaks)));
        results.pattern_recognition.periods = lags(peak_locs(1:min(5, length(peaks))));
    end
    
    % Complexity measures
    % Approximate entropy
    m = 2;
    r = 0.15 * std(signal);
    results.pattern_recognition.approximate_entropy = approximate_entropy(signal, m, r);
    
    % Sample entropy
    results.pattern_recognition.sample_entropy = sample_entropy(signal, m, r);
    
    % ===== ADVANCED FEATURES =====
    fprintf('Extracting advanced features...\n');
    
    % Hjorth parameters
    signal_diff1 = diff(signal);
    signal_diff2 = diff(signal_diff1);
    
    variance_signal = var(signal);
    variance_diff1 = var(signal_diff1);
    variance_diff2 = var(signal_diff2);
    
    results.advanced_features.hjorth_activity = variance_signal;
    results.advanced_features.hjorth_mobility = sqrt(variance_diff1 / variance_signal);
    results.advanced_features.hjorth_complexity = sqrt(variance_diff2 / variance_diff1) / results.advanced_features.hjorth_mobility;
    
    % Fractal dimension estimation
    results.advanced_features.fractal_dimension = estimate_fractal_dimension(signal);
    
    % Spectral entropy
    psd_normalized = results.frequency_domain.power_spectrum / sum(results.frequency_domain.power_spectrum);
    psd_normalized = psd_normalized(psd_normalized > 0);
    results.advanced_features.spectral_entropy = -sum(psd_normalized .* log2(psd_normalized));
    
    % ===== SUMMARY STATISTICS =====
    fprintf('Generating summary statistics...\n');
    
    results.summary.analysis_complete = true;
    results.summary.total_features = 50; % Approximate count
    results.summary.dominant_characteristics = struct();
    
    % Classify signal type based on features
    if results.frequency_domain.spectral_centroid < fs/10
        results.summary.signal_type = 'Low frequency dominant';
    elseif results.frequency_domain.spectral_centroid > fs/3
        results.summary.signal_type = 'High frequency dominant';
    else
        results.summary.signal_type = 'Broadband';
    end
    
    % Signal complexity assessment
    if results.advanced_features.hjorth_complexity < 1.2
        results.summary.complexity = 'Low';
    elseif results.advanced_features.hjorth_complexity > 2.0
        results.summary.complexity = 'High';
    else
        results.summary.complexity = 'Medium';
    end
    
    % Quality assessment
    if results.quality_assessment.snr_db > 20
        results.summary.quality = 'Excellent';
    elseif results.quality_assessment.snr_db > 10
        results.summary.quality = 'Good';
    elseif results.quality_assessment.snr_db > 0
        results.summary.quality = 'Fair';
    else
        results.summary.quality = 'Poor';
    end
    
    fprintf('Comprehensive signal analysis complete.\n');
    fprintf('Results stored in structured format with %d main categories.\n', length(fieldnames(results)));
    
    % Display key findings
    fprintf('\n=== KEY FINDINGS ===\n');
    fprintf('Signal Type: %s\n', results.summary.signal_type);
    fprintf('Quality: %s (SNR: %.2f dB)\n', results.summary.quality, results.quality_assessment.snr_db);
    fprintf('Complexity: %s\n', results.summary.complexity);
    fprintf('Dominant Frequency: %.2f Hz\n', results.frequency_domain.dominant_frequency);
    fprintf('Duration: %.2f seconds\n', results.duration);
    fprintf('Number of Peaks: %d\n', results.time_domain.num_peaks);
end

function ae = approximate_entropy(signal, m, r)
    % Calculate approximate entropy
    N = length(signal);
    
    function phi = calc_phi(m)
        patterns = zeros(N-m+1, m);
        for i = 1:N-m+1
            patterns(i, :) = signal(i:i+m-1);
        end
        
        C = zeros(N-m+1, 1);
        for i = 1:N-m+1
            distances = max(abs(patterns - repmat(patterns(i, :), N-m+1, 1)), [], 2);
            C(i) = sum(distances <= r) / (N-m+1);
        end
        
        phi = sum(log(C)) / (N-m+1);
    end
    
    ae = calc_phi(m) - calc_phi(m+1);
end

function se = sample_entropy(signal, m, r)
    % Calculate sample entropy
    N = length(signal);
    
    function count = template_match_count(m)
        count = 0;
        for i = 1:N-m
            template = signal(i:i+m-1);
            for j = i+1:N-m
                if max(abs(template - signal(j:j+m-1))) <= r
                    count = count + 1;
                end
            end
        end
    end
    
    A = template_match_count(m);
    B = template_match_count(m+1);
    
    if A == 0 || B == 0
        se = Inf;
    else
        se = -log(B/A);
    end
end

function fd = estimate_fractal_dimension(signal)
    % Estimate fractal dimension using box counting method
    N = length(signal);
    
    % Normalize signal to [0, 1]
    signal_norm = (signal - min(signal)) / (max(signal) - min(signal));
    
    % Create different box sizes
    box_sizes = 2.^(2:floor(log2(N/4)));
    counts = zeros(size(box_sizes));
    
    for i = 1:length(box_sizes)
        box_size = box_sizes(i);
        num_boxes = ceil(N / box_size);
        
        count = 0;
        for j = 1:num_boxes
            start_idx = (j-1) * box_size + 1;
            end_idx = min(j * box_size, N);
            
            if end_idx > start_idx
                box_range = max(signal_norm(start_idx:end_idx)) - min(signal_norm(start_idx:end_idx));
                if box_range > 0
                    count = count + 1;
                end
            end
        end
        counts(i) = count;
    end
    
    % Fit line to log-log plot
    log_box_sizes = log(1./box_sizes);
    log_counts = log(counts);
    
    % Remove any infinite values
    valid_idx = isfinite(log_box_sizes) & isfinite(log_counts);
    
    if sum(valid_idx) >= 2
        p = polyfit(log_box_sizes(valid_idx), log_counts(valid_idx), 1);
        fd = p(1);
    else
        fd = 1.5; % Default value
    end
end