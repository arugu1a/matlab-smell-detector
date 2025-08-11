% This file was generated using Claude Sonnet 4 AI for testing purposes.
% Generated on: 8 August 2025

classdef Rectangle < handle
    % A simple Rectangle class demonstrating MATLAB OOP
    
    properties
        Width
        Height
    end
    
    methods
        function obj = Rectangle(width, height)
            % Constructor method
            if nargin > 0
                obj.Width = width;
                obj.Height = height;
            end
        end
        
        function area = calculateArea(obj)
            % Basic method to calculate rectangle area
            area = obj.Width * obj.Height;
        end
        
        function perimeter = calculatePerimeter(obj)
            % Basic method to calculate rectangle perimeter
            perimeter = 2 * (obj.Width + obj.Height);
        end
        
        function displayInfo(obj)
            % Method to display rectangle information
            fprintf('Rectangle: %.2f x %.2f\n', obj.Width, obj.Height);
            fprintf('Area: %.2f\n', obj.calculateArea());
            fprintf('Perimeter: %.2f\n', obj.calculatePerimeter());
        end
    end
end