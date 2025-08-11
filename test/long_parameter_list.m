% This file was generated using Claude Sonnet 4 AI for testing purposes.
% Generated on: 8 August 2025

function result = analyzeSignal(data, samplingRate, filterType, cutoffFreq, ...
                                order, windowType, overlapPercent, nfft, ...
                                plotResults, saveOutput, outputPath, normalization)
% ANALYZESIGNAL - Comprehensive signal analysis with many parameters
%
% Inputs:
%   data           - Input signal vector
%   samplingRate   - Sampling frequency in Hz
%   filterType     - Filter type ('lowpass', 'highpass', 'bandpass')
%   cutoffFreq     - Cutoff frequency(ies) in Hz
%   order          - Filter order
%   windowType     - Window function ('hamming', 'hanning', 'blackman')
%   overlapPercent - Overlap percentage for windowing (0-99)
%   nfft           - Number of FFT points
%   plotResults    - Boolean flag to show plots
%   saveOutput     - Boolean flag to save results
%   outputPath     - File path for saving results
%   normalization  - Normalization method ('none', 'unity', 'rms')
%
% Output:
%   result - Structure containing analysis results

    % Input validation
    if nargin < 12
        error('All 12 parameters are required');
    end
    
    % Apply filtering
    if strcmp(filterType, 'lowpass')
        filteredData = lowpass(data, cutoffFreq, samplingRate);
    elseif strcmp(filterType, 'highpass')
        filteredData = highpass(data, cutoffFreq, samplingRate);
    else
        filteredData = bandpass(data, cutoffFreq, samplingRate);
    end
    
    % Apply windowing and compute spectrum
    windowFunc = str2func(windowType);
    overlap = round(length(data) * overlapPercent / 100);
    [psd, freqs] = pwelch(filteredData, windowFunc(256), overlap, nfft, samplingRate);
    
    % Normalize data
    switch normalization
        case 'unity'
            filteredData = filteredData / max(abs(filteredData));
        case 'rms'
            filteredData = filteredData / rms(filteredData);
    end
    
    % Package results
    result.originalData = data;
    result.filteredData = filteredData;
    result.powerSpectrum = psd;
    result.frequencies = freqs;
    result.parameters = struct('samplingRate', samplingRate, 'filterType', filterType, ...
                              'cutoffFreq', cutoffFreq, 'order', order);
    
    % Optional plotting
    if plotResults
        figure;
        subplot(2,1,1); plot(data); title('Original Signal');
        subplot(2,1,2); plot(filteredData); title('Filtered Signal');
    end
    
    % Optional saving
    if saveOutput
        save(outputPath, 'result');
        fprintf('Results saved to: %s\n', outputPath);
    end
    
end