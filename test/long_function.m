% This file was generated using Claude Sonnet 4 AI for testing purposes.
% Generated on: 8 August 2025

function [results] = analyzeStudentGrades(grades, studentNames, assignmentNames)
% ANALYZESTUDENTGRADES - Comprehensive analysis of student grade data
% 
% This function performs detailed statistical analysis on student grades
% including grade distribution, performance trends, and risk assessment
%
% Input:
%   grades - NxM matrix where N = students, M = assignments
%   studentNames - Nx1 cell array of student names
%   assignmentNames - Mx1 cell array of assignment names
%
% Output:
%   results - Structure containing comprehensive analysis results

    % Initialize results structure
    results = struct();
    
    % Get dimensions
    [numStudents, numAssignments] = size(grades);
    
    % Validate inputs
    if length(studentNames) ~= numStudents
        error('Number of student names must match number of rows in grades matrix');
    end
    
    if length(assignmentNames) ~= numAssignments
        error('Number of assignment names must match number of columns in grades matrix');
    end
    
    fprintf('=== STUDENT GRADE ANALYSIS SYSTEM ===\n');
    fprintf('Analyzing %d students across %d assignments...\n\n', numStudents, numAssignments);
    
    %% SECTION 1: Basic Statistics Calculation
    fprintf('1. CALCULATING BASIC STATISTICS...\n');
    
    % Initialize arrays for student statistics
    studentAverages = zeros(numStudents, 1);
    studentMedians = zeros(numStudents, 1);
    studentStdDevs = zeros(numStudents, 1);
    
    % Calculate statistics for each student using for loop
    for i = 1:numStudents
        studentGrades = grades(i, :);
        
        % Remove any NaN values (missing assignments)
        validGrades = studentGrades(~isnan(studentGrades));
        
        if ~isempty(validGrades)
            studentAverages(i) = mean(validGrades);
            studentMedians(i) = median(validGrades);
            studentStdDevs(i) = std(validGrades);
        else
            studentAverages(i) = NaN;
            studentMedians(i) = NaN;
            studentStdDevs(i) = NaN;
        end
        
        % Progress indicator
        if mod(i, 10) == 0 || i == numStudents
            fprintf('  Processed %d/%d students\n', i, numStudents);
        end
    end
    
    % Calculate assignment statistics using for loop
    assignmentAverages = zeros(numAssignments, 1);
    assignmentStdDevs = zeros(numAssignments, 1);
    
    fprintf('  Calculating assignment statistics...\n');
    for j = 1:numAssignments
        assignmentGrades = grades(:, j);
        validGrades = assignmentGrades(~isnan(assignmentGrades));
        
        if ~isempty(validGrades)
            assignmentAverages(j) = mean(validGrades);
            assignmentStdDevs(j) = std(validGrades);
        else
            assignmentAverages(j) = NaN;
            assignmentStdDevs(j) = NaN;
        end
    end
    
    %% SECTION 2: Grade Distribution Analysis
    fprintf('\n2. ANALYZING GRADE DISTRIBUTIONS...\n');
    
    % Define grade boundaries
    gradeBoundaries = [90, 80, 70, 60, 0];
    gradeLetters = {'A', 'B', 'C', 'D', 'F'};
    gradeDistribution = zeros(length(gradeLetters), 1);
    
    % Count students in each grade category
    for i = 1:numStudents
        average = studentAverages(i);
        
        if ~isnan(average)
            % Determine letter grade using nested if statements
            if average >= gradeBoundaries(1)
                gradeDistribution(1) = gradeDistribution(1) + 1;
            elseif average >= gradeBoundaries(2)
                gradeDistribution(2) = gradeDistribution(2) + 1;
            elseif average >= gradeBoundaries(3)
                gradeDistribution(3) = gradeDistribution(3) + 1;
            elseif average >= gradeBoundaries(4)
                gradeDistribution(4) = gradeDistribution(4) + 1;
            else
                gradeDistribution(5) = gradeDistribution(5) + 1;
            end
        end
    end
    
    % Display distribution
    fprintf('  Grade Distribution:\n');
    for i = 1:length(gradeLetters)
        percentage = (gradeDistribution(i) / numStudents) * 100;
        fprintf('    %s: %d students (%.1f%%)\n', gradeLetters{i}, ...
                gradeDistribution(i), percentage);
    end
    
    %% SECTION 3: Performance Trend Analysis
    fprintf('\n3. ANALYZING PERFORMANCE TRENDS...\n');
    
    % Track improvement/decline patterns for each student
    trendAnalysis = cell(numStudents, 1);
    improvingStudents = 0;
    decliningStudents = 0;
    stableStudents = 0;
    
    for i = 1:numStudents
        studentGrades = grades(i, :);
        validIndices = find(~isnan(studentGrades));
        
        if length(validIndices) >= 3  % Need at least 3 grades for trend analysis
            validGrades = studentGrades(validIndices);
            
            % Calculate trend using simple linear regression approach
            x = 1:length(validGrades);
            trend = polyfit(x, validGrades, 1);
            slope = trend(1);
            
            % Categorize trend with multiple conditions
            if slope > 2
                trendAnalysis{i} = 'Strong Improvement';
                improvingStudents = improvingStudents + 1;
            elseif slope > 0.5
                trendAnalysis{i} = 'Moderate Improvement';
                improvingStudents = improvingStudents + 1;
            elseif slope < -2
                trendAnalysis{i} = 'Strong Decline';
                decliningStudents = decliningStudents + 1;
            elseif slope < -0.5
                trendAnalysis{i} = 'Moderate Decline';
                decliningStudents = decliningStudents + 1;
            else
                trendAnalysis{i} = 'Stable Performance';
                stableStudents = stableStudents + 1;
            end
        else
            trendAnalysis{i} = 'Insufficient Data';
        end
    end
    
    fprintf('  Performance Trends:\n');
    fprintf('    Improving: %d students\n', improvingStudents);
    fprintf('    Declining: %d students\n', decliningStudents);
    fprintf('    Stable: %d students\n', stableStudents);
    
    %% SECTION 4: At-Risk Student Identification
    fprintf('\n4. IDENTIFYING AT-RISK STUDENTS...\n');
    
    atRiskStudents = [];
    atRiskReasons = {};
    atRiskCount = 0;
    
    % Multiple criteria for at-risk identification
    for i = 1:numStudents
        isAtRisk = false;
        reasons = {};
        
        % Criterion 1: Low overall average
        if ~isnan(studentAverages(i)) && studentAverages(i) < 70
            isAtRisk = true;
            reasons{end+1} = 'Low average';
        end
        
        % Criterion 2: High variability in grades
        if ~isnan(studentStdDevs(i)) && studentStdDevs(i) > 15
            isAtRisk = true;
            reasons{end+1} = 'Inconsistent performance';
        end
        
        % Criterion 3: Declining trend
        if strcmp(trendAnalysis{i}, 'Strong Decline') || strcmp(trendAnalysis{i}, 'Moderate Decline')
            isAtRisk = true;
            reasons{end+1} = 'Declining performance';
        end
        
        % Criterion 4: Recent poor performance (last 3 assignments)
        if numAssignments >= 3
            recentGrades = grades(i, end-2:end);
            recentValidGrades = recentGrades(~isnan(recentGrades));
            
            if ~isempty(recentValidGrades) && mean(recentValidGrades) < 65
                isAtRisk = true;
                reasons{end+1} = 'Recent poor performance';
            end
        end
        
        % Criterion 5: Missing assignments
        missingCount = sum(isnan(grades(i, :)));
        if missingCount > numAssignments * 0.2  % More than 20% missing
            isAtRisk = true;
            reasons{end+1} = sprintf('%d missing assignments', missingCount);
        end
        
        % Store at-risk information
        if isAtRisk
            atRiskCount = atRiskCount + 1;
            atRiskStudents(end+1) = i;
            atRiskReasons{end+1} = strjoin(reasons, ', ');
        end
    end
    
    fprintf('  Found %d at-risk students:\n', atRiskCount);
    for i = 1:length(atRiskStudents)
        studentIdx = atRiskStudents(i);
        fprintf('    %s: %s (Avg: %.1f)\n', studentNames{studentIdx}, ...
                atRiskReasons{i}, studentAverages(studentIdx));
    end
    
    %% SECTION 5: Assignment Difficulty Analysis
    fprintf('\n5. ANALYZING ASSIGNMENT DIFFICULTY...\n');
    
    % Classify assignments by difficulty based on class performance
    easyAssignments = [];
    moderateAssignments = [];
    difficultAssignments = [];
    
    for j = 1:numAssignments
        avgScore = assignmentAverages(j);
        
        if ~isnan(avgScore)
            % Use nested if-elseif structure for classification
            if avgScore >= 85
                easyAssignments(end+1) = j;
                difficulty = 'Easy';
            elseif avgScore >= 75
                moderateAssignments(end+1) = j;
                difficulty = 'Moderate';
            else
                difficultAssignments(end+1) = j;
                difficulty = 'Difficult';
            end
            
            fprintf('    %s: %.1f avg, %.1f std (%s)\n', assignmentNames{j}, ...
                    avgScore, assignmentStdDevs(j), difficulty);
        end
    end
    
    %% SECTION 6: Generate Recommendations
    fprintf('\n6. GENERATING RECOMMENDATIONS...\n');
    
    recommendations = {};
    
    % Class-level recommendations
    classAverage = mean(studentAverages(~isnan(studentAverages)));
    
    if classAverage < 75
        recommendations{end+1} = 'Class average is below 75% - consider reviewing teaching methods';
    end
    
    if length(difficultAssignments) > numAssignments * 0.3
        recommendations{end+1} = 'More than 30% of assignments are difficult - consider adjusting difficulty';
    end
    
    if atRiskCount > numStudents * 0.25
        recommendations{end+1} = 'High number of at-risk students - implement intervention programs';
    end
    
    % Display recommendations using while loop
    fprintf('  Recommendations:\n');
    recIdx = 1;
    while recIdx <= length(recommendations)
        fprintf('    %d. %s\n', recIdx, recommendations{recIdx});
        recIdx = recIdx + 1;
    end
    
    %% SECTION 7: Compile Results
    fprintf('\n7. COMPILING FINAL RESULTS...\n');
    
    % Store all results in output structure
    results.studentStatistics.averages = studentAverages;
    results.studentStatistics.medians = studentMedians;
    results.studentStatistics.standardDeviations = studentStdDevs;
    results.studentStatistics.trends = trendAnalysis;
    
    results.assignmentStatistics.averages = assignmentAverages;
    results.assignmentStatistics.standardDeviations = assignmentStdDevs;
    results.assignmentStatistics.easyAssignments = easyAssignments;
    results.assignmentStatistics.moderateAssignments = moderateAssignments;
    results.assignmentStatistics.difficultAssignments = difficultAssignments;
    
    results.classStatistics.gradeDistribution = gradeDistribution;
    results.classStatistics.gradeLabels = gradeLetters;
    results.classStatistics.classAverage = classAverage;
    results.classStatistics.improvingStudents = improvingStudents;
    results.classStatistics.decliningStudents = decliningStudents;
    results.classStatistics.stableStudents = stableStudents;
    
    results.atRiskAnalysis.studentIndices = atRiskStudents;
    results.atRiskAnalysis.reasons = atRiskReasons;
    results.atRiskAnalysis.count = atRiskCount;
    
    results.recommendations = recommendations;
    results.metadata.analysisDate = datestr(now);
    results.metadata.numStudents = numStudents;
    results.metadata.numAssignments = numAssignments;
    
    %% SECTION 8: Generate Summary Report
    fprintf('\n=== ANALYSIS COMPLETE ===\n');
    fprintf('Summary Statistics:\n');
    fprintf('  Class Average: %.1f%%\n', classAverage);
    fprintf('  Students Analyzed: %d\n', numStudents);
    fprintf('  Assignments Analyzed: %d\n', numAssignments);
    fprintf('  At-Risk Students: %d (%.1f%%)\n', atRiskCount, (atRiskCount/numStudents)*100);
    
    % Find top and bottom performers
    [~, topPerformerIdx] = max(studentAverages);
    [~, bottomPerformerIdx] = min(studentAverages);
    
    if ~isnan(studentAverages(topPerformerIdx))
        fprintf('  Top Performer: %s (%.1f%%)\n', studentNames{topPerformerIdx}, ...
                studentAverages(topPerformerIdx));
    end
    
    if ~isnan(studentAverages(bottomPerformerIdx))
        fprintf('  Needs Support: %s (%.1f%%)\n', studentNames{bottomPerformerIdx}, ...
                studentAverages(bottomPerformerIdx));
    end
    
    fprintf('\nDetailed results stored in output structure.\n');
    fprintf('Analysis completed at: %s\n', results.metadata.analysisDate);

end