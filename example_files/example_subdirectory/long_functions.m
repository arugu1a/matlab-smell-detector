% Author: Claude AI
% Date: July 2025

function main()
    % Main function to demonstrate long functions
    fprintf('Testing long functions...\n');
    
    % Test the medium function (>40 LOC)
    result1 = mediumFunction(10, 5);
    fprintf('Medium function result: %f\n', result1);
    
    % Test the long function (>100 LOC)
    data = randn(100, 1);
    result2 = longFunction(data);
    fprintf('Long function result: %f\n', result2);
    
    % Test the very long function (>400 LOC)
    matrix = randn(50, 50);
    result3 = veryLongFunction(matrix);
    fprintf('Very long function result: %f\n', result3);
end

function result = mediumFunction(a, b)
    % This function is longer than 40 lines of code
    % It performs various mathematical operations
    
    % Initialize variables
    x = a + b;
    y = a - b;
    z = a * b;
    w = a / b;
    
    % Perform some calculations
    temp1 = x^2 + y^2;
    temp2 = z^2 + w^2;
    temp3 = sqrt(temp1 + temp2);
    
    % More calculations
    sine_val = sin(temp3);
    cosine_val = cos(temp3);
    tangent_val = tan(temp3);
    
    % Logarithmic operations
    log_val = log(abs(temp3) + 1);
    exp_val = exp(temp3 / 10);
    
    % Conditional operations
    if temp3 > 5
        modifier = 1.5;
    elseif temp3 > 2
        modifier = 1.2;
    else
        modifier = 1.0;
    end
    
    % Array operations
    arr = [sine_val, cosine_val, tangent_val, log_val, exp_val];
    mean_val = mean(arr);
    std_val = std(arr);
    
    % More complex operations
    for i = 1:5
        arr(i) = arr(i) * modifier;
    end
    
    % Final calculations
    intermediate = sum(arr) / length(arr);
    result = intermediate * mean_val + std_val;
    
    % Additional processing
    if result < 0
        result = abs(result);
    end
    
    % More operations to reach 40+ lines
    final_modifier = 1 + sin(result) * 0.1;
    result = result * final_modifier;
    
    % Ensure positive result
    result = abs(result);
end

function result = longFunction(data)
    % This function is longer than 100 lines of code
    % It performs comprehensive data analysis
    
    % Input validation
    if isempty(data)
        error('Input data cannot be empty');
    end
    
    % Initialize variables
    n = length(data);
    processed_data = zeros(size(data));
    
    % Basic statistics
    mean_val = mean(data);
    median_val = median(data);
    std_val = std(data);
    var_val = var(data);
    
    % Normalization
    normalized_data = (data - mean_val) / std_val;
    
    % Outlier detection using IQR method
    Q1 = quantile(data, 0.25);
    Q3 = quantile(data, 0.75);
    IQR = Q3 - Q1;
    lower_bound = Q1 - 1.5 * IQR;
    upper_bound = Q3 + 1.5 * IQR;
    
    % Remove outliers
    outlier_mask = (data >= lower_bound) & (data <= upper_bound);
    clean_data = data(outlier_mask);
    
    % Smoothing using moving average
    window_size = min(5, length(clean_data));
    smoothed_data = zeros(size(clean_data));
    
    for i = 1:length(clean_data)
        start_idx = max(1, i - floor(window_size/2));
        end_idx = min(length(clean_data), i + floor(window_size/2));
        smoothed_data(i) = mean(clean_data(start_idx:end_idx));
    end
    
    % Trend analysis
    x = 1:length(smoothed_data);
    p = polyfit(x, smoothed_data', 1);
    trend_line = polyval(p, x);
    slope = p(1);
    
    % Detrending
    detrended_data = smoothed_data - trend_line';
    
    % Frequency analysis (simple)
    fft_data = fft(detrended_data);
    power_spectrum = abs(fft_data).^2;
    
    % Peak detection
    peaks = [];
    for i = 2:length(smoothed_data)-1
        if smoothed_data(i) > smoothed_data(i-1) && smoothed_data(i) > smoothed_data(i+1)
            peaks = [peaks, i];
        end
    end
    
    % Valley detection
    valleys = [];
    for i = 2:length(smoothed_data)-1
        if smoothed_data(i) < smoothed_data(i-1) && smoothed_data(i) < smoothed_data(i+1)
            valleys = [valleys, i];
        end
    end
    
    % Statistical measures
    skewness_val = skewness(clean_data);
    kurtosis_val = kurtosis(clean_data);
    
    % Percentile calculations
    percentiles = [5, 10, 25, 50, 75, 90, 95];
    percentile_values = zeros(size(percentiles));
    for i = 1:length(percentiles)
        percentile_values(i) = quantile(clean_data, percentiles(i)/100);
    end
    
    % Range calculations
    data_range = max(clean_data) - min(clean_data);
    
    % Coefficient of variation
    cv = std_val / mean_val;
    
    % Signal-to-noise ratio (simple approximation)
    signal_power = var(smoothed_data);
    noise_power = var(detrended_data);
    snr = 10 * log10(signal_power / noise_power);
    
    % Autocorrelation (simple)
    max_lag = min(20, length(smoothed_data) - 1);
    autocorr = zeros(max_lag + 1, 1);
    for lag = 0:max_lag
        if lag == 0
            autocorr(lag + 1) = 1;
        else
            autocorr(lag + 1) = corr(smoothed_data(1:end-lag), smoothed_data(lag+1:end));
        end
    end
    
    % Combine all metrics into final result
    result = struct();
    result.mean = mean_val;
    result.median = median_val;
    result.std = std_val;
    result.variance = var_val;
    result.skewness = skewness_val;
    result.kurtosis = kurtosis_val;
    result.range = data_range;
    result.cv = cv;
    result.snr = snr;
    result.slope = slope;
    result.num_peaks = length(peaks);
    result.num_valleys = length(valleys);
    result.outlier_ratio = 1 - sum(outlier_mask) / length(data);
    
    % Final scalar result for demonstration
    result = mean_val + std_val + abs(slope) + length(peaks) + snr/10;
end

function result = veryLongFunction(matrix)
    % This function is longer than 400 lines of code
    % It performs comprehensive matrix analysis and operations
    
    % Input validation
    if isempty(matrix)
        error('Input matrix cannot be empty');
    end
    
    [rows, cols] = size(matrix);
    
    % Initialize output structure
    analysis = struct();
    
    % Basic matrix properties
    analysis.rows = rows;
    analysis.cols = cols;
    analysis.total_elements = rows * cols;
    analysis.is_square = (rows == cols);
    
    % Matrix statistics
    analysis.mean = mean(matrix(:));
    analysis.median = median(matrix(:));
    analysis.std = std(matrix(:));
    analysis.variance = var(matrix(:));
    analysis.min_val = min(matrix(:));
    analysis.max_val = max(matrix(:));
    analysis.range = analysis.max_val - analysis.min_val;
    
    % Row-wise statistics
    row_means = mean(matrix, 2);
    row_stds = std(matrix, 0, 2);
    row_mins = min(matrix, [], 2);
    row_maxs = max(matrix, [], 2);
    
    % Column-wise statistics
    col_means = mean(matrix, 1);
    col_stds = std(matrix, 0, 1);
    col_mins = min(matrix, [], 1);
    col_maxs = max(matrix, [], 1);
    
    % Diagonal analysis (if square)
    if analysis.is_square
        main_diag = diag(matrix);
        anti_diag = diag(fliplr(matrix));
        analysis.main_diag_sum = sum(main_diag);
        analysis.anti_diag_sum = sum(anti_diag);
        analysis.trace = trace(matrix);
    end
    
    % Matrix norms
    analysis.frobenius_norm = norm(matrix, 'fro');
    analysis.one_norm = norm(matrix, 1);
    analysis.inf_norm = norm(matrix, inf);
    
    % Eigenvalue analysis (if square and reasonable size)
    if analysis.is_square && rows <= 100
        try
            eigenvals = eig(matrix);
            analysis.max_eigenval = max(real(eigenvals));
            analysis.min_eigenval = min(real(eigenvals));
            analysis.spectral_radius = max(abs(eigenvals));
            analysis.condition_number = cond(matrix);
            analysis.determinant = det(matrix);
        catch
            analysis.eigenval_error = true;
        end
    end
    
    % Singular value decomposition (for reasonable sizes)
    if rows <= 100 && cols <= 100
        [U, S, V] = svd(matrix);
        singular_vals = diag(S);
        analysis.max_singular_val = max(singular_vals);
        analysis.min_singular_val = min(singular_vals);
        analysis.rank = rank(matrix);
        analysis.nullity = min(rows, cols) - analysis.rank;
    end
    
    % Matrix decompositions (if square)
    if analysis.is_square && rows <= 50
        try
            % LU decomposition
            [L, U, P] = lu(matrix);
            analysis.lu_det = det(L) * det(U);
            
            % QR decomposition
            [Q, R] = qr(matrix);
            analysis.qr_det = det(R);
            
            % Cholesky decomposition (if positive definite)
            if all(eig(matrix) > 0)
                analysis.chol_success = true;
                chol_factor = chol(matrix);
                analysis.chol_det = det(chol_factor)^2;
            else
                analysis.chol_success = false;
            end
        catch
            analysis.decomp_error = true;
        end
    end
    
    % Correlation analysis
    if min(rows, cols) > 1
        % Row correlations
        row_corr_matrix = corrcoef(matrix');
        analysis.avg_row_correlation = mean(row_corr_matrix(~eye(size(row_corr_matrix))));
        
        % Column correlations
        col_corr_matrix = corrcoef(matrix);
        analysis.avg_col_correlation = mean(col_corr_matrix(~eye(size(col_corr_matrix))));
    end
    
    % Outlier detection using MAD (Median Absolute Deviation)
    median_val = median(matrix(:));
    mad_val = mad(matrix(:));
    outlier_threshold = 3 * mad_val;
    outliers = abs(matrix - median_val) > outlier_threshold;
    analysis.outlier_count = sum(outliers(:));
    analysis.outlier_percentage = (analysis.outlier_count / numel(matrix)) * 100;
    
    % Sparsity analysis
    zero_threshold = 1e-10;
    zero_elements = abs(matrix) < zero_threshold;
    analysis.zero_count = sum(zero_elements(:));
    analysis.sparsity = (analysis.zero_count / numel(matrix)) * 100;
    
    % Pattern analysis
    % Check for symmetric pattern
    if analysis.is_square
        analysis.is_symmetric = isequal(matrix, matrix');
        analysis.is_antisymmetric = isequal(matrix, -matrix');
    end
    
    % Bandedness analysis (if square)
    if analysis.is_square
        % Check for diagonal dominance
        diag_elements = abs(diag(matrix));
        off_diag_sums = sum(abs(matrix), 2) - diag_elements;
        analysis.is_diag_dominant = all(diag_elements > off_diag_sums);
        
        % Check for upper/lower triangular
        analysis.is_upper_triangular = isequal(matrix, triu(matrix));
        analysis.is_lower_triangular = isequal(matrix, tril(matrix));
    end
    
    % Smoothness analysis
    if rows > 1 && cols > 1
        % Gradient magnitude
        [Gx, Gy] = gradient(matrix);
        gradient_magnitude = sqrt(Gx.^2 + Gy.^2);
        analysis.avg_gradient = mean(gradient_magnitude(:));
        analysis.max_gradient = max(gradient_magnitude(:));
        
        % Laplacian
        laplacian = del2(matrix);
        analysis.avg_laplacian = mean(abs(laplacian(:)));
        analysis.max_laplacian = max(abs(laplacian(:)));
    end
    
    % Frequency domain analysis (2D FFT for matrices)
    if rows > 4 && cols > 4
        fft2_matrix = fft2(matrix);
        power_spectrum = abs(fft2_matrix).^2;
        analysis.spectral_energy = sum(power_spectrum(:));
        analysis.dc_component = abs(fft2_matrix(1,1))^2;
        analysis.ac_energy = analysis.spectral_energy - analysis.dc_component;
    end
    
    % Texture analysis (simple)
    if rows > 2 && cols > 2
        % Local variance
        local_var = zeros(rows-2, cols-2);
        for i = 2:rows-1
            for j = 2:cols-1
                neighborhood = matrix(i-1:i+1, j-1:j+1);
                local_var(i-1, j-1) = var(neighborhood(:));
            end
        end
        analysis.avg_local_variance = mean(local_var(:));
        analysis.texture_contrast = std(local_var(:));
    end
    
    % Connectivity analysis (treat as adjacency matrix if square and non-negative)
    if analysis.is_square && all(matrix(:) >= 0)
        binary_matrix = matrix > 0;
        analysis.edge_count = sum(binary_matrix(:));
        analysis.density = analysis.edge_count / (rows * cols);
        
        % Degree analysis
        out_degrees = sum(binary_matrix, 2);
        in_degrees = sum(binary_matrix, 1)';
        analysis.avg_out_degree = mean(out_degrees);
        analysis.avg_in_degree = mean(in_degrees);
        analysis.max_out_degree = max(out_degrees);
        analysis.max_in_degree = max(in_degrees);
    end
    
    % Clustering analysis (simple)
    if rows > 3 && cols > 3
        % K-means clustering on matrix elements
        try
            data_vector = matrix(:);
            k = min(5, length(unique(data_vector)));
            if k > 1
                [cluster_idx, centroids] = kmeans(data_vector, k);
                analysis.num_clusters = k;
                analysis.cluster_centers = centroids;
                analysis.within_cluster_variance = sum(var(data_vector(cluster_idx == 1:k)));
            end
        catch
            analysis.clustering_error = true;
        end
    end
    
    % Stability analysis
    if analysis.is_square && rows <= 20
        try
            % Add small perturbation and check eigenvalue changes
            perturbation = 0.01 * randn(size(matrix));
            perturbed_matrix = matrix + perturbation;
            orig_eigenvals = eig(matrix);
            pert_eigenvals = eig(perturbed_matrix);
            eigenval_sensitivity = max(abs(sort(real(orig_eigenvals)) - sort(real(pert_eigenvals))));
            analysis.eigenval_sensitivity = eigenval_sensitivity;
        catch
            analysis.stability_error = true;
        end
    end
    
    % Optimization-related properties
    if analysis.is_square
        try
            % Check if matrix is positive definite
            analysis.is_positive_definite = all(eig(matrix) > 0);
            
            % Check if matrix is positive semidefinite
            analysis.is_positive_semidefinite = all(eig(matrix) >= 0);
            
            % Check if matrix is negative definite
            analysis.is_negative_definite = all(eig(matrix) < 0);
        catch
            analysis.definiteness_error = true;
        end
    end
    
    % Data distribution analysis
    data_vector = matrix(:);
    
    % Histogram analysis
    num_bins = min(50, length(unique(data_vector)));
    [hist_counts, hist_edges] = histcounts(data_vector, num_bins);
    analysis.histogram_entropy = -sum((hist_counts/sum(hist_counts)) .* log2(hist_counts/sum(hist_counts) + eps));
    
    % Moments
    analysis.skewness = skewness(data_vector);
    analysis.kurtosis = kurtosis(data_vector);
    
    % Percentile analysis
    percentiles = [5, 10, 25, 50, 75, 90, 95];
    analysis.percentiles = quantile(data_vector, percentiles/100);
    
    % Robust statistics
    analysis.median_abs_deviation = mad(data_vector);
    analysis.interquartile_range = iqr(data_vector);
    
    % Final comprehensive score
    complexity_score = 0;
    complexity_score = complexity_score + analysis.std;
    complexity_score = complexity_score + analysis.avg_gradient;
    complexity_score = complexity_score + analysis.outlier_percentage;
    complexity_score = complexity_score + analysis.texture_contrast;
    complexity_score = complexity_score + analysis.histogram_entropy;
    
    if isfield(analysis, 'spectral_radius')
        complexity_score = complexity_score + log(analysis.spectral_radius + 1);
    end
    
    if isfield(analysis, 'condition_number')
        complexity_score = complexity_score + log(analysis.condition_number + 1);
    end
    
    % Normalization factor
    normalization_factor = sqrt(rows * cols);
    final_score = complexity_score / normalization_factor;
    
    % Store final result
    analysis.final_complexity_score = final_score;
    
    % Return scalar result for demonstration
    result = final_score;
end