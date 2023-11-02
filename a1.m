% APL771 Assignment - Conjugate gradient method 
% Author - Parth Patel
% Entry Number - 2021CS10550
% References used - Optimization for Engineering Design by K. Deb

% Define the objective function (Modify it based on your objective)
f = @(x) (x(1)^2+x(2)-11)^2 + (x(1)+x(2)^2-7)^2; 

% Initial guess 
x0 = [0; 0];  % Initial guess

eps1 = 1e-5;  % Termination parameter for line search
eps2 = 1e-5;  % Termination parameter for step size
eps3 = 1e-5;  % Termination parameter for gradient norm

% Calling Conjugate Gradient Method
[x_min, f_min] = conjugateGradientMethod(f, x0, eps1, eps2, eps3);

% Results
fprintf('Minimum point: x_min = [%f, %f]\n', x_min(1), x_min(2));
fprintf('Minimum function value: f_min = %f\n', f_min);


% grad function to compute the gradient of a function at some point numerically
function grad = gradient(f, x) 
    h1 = [1e-5; 0];
    h2 = [0; 1e-5];
    h = 1e-5;
    grad_x = (f(x+h1) - f(x))/h;
    grad_y = (f(x+h2) - f(x))/h;
    grad = [grad_x; grad_y];
end
    
% Conjugate gradient method
function [x_min, f_min] = conjugateGradientMethod(f, x0, eps1, eps2, eps3)
    % Step 1: Initialization
    x_0 = x0;

    % Step 2: Find grad(x_0)
    s = -gradient(f, x_0);
    s = s / norm(s);

    % Step 3: Golden section search for lambda
    lambda_0 = goldenSectionSearch(f, x_0, s, eps1);
    x_1 = x_0 + lambda_0*s;

    iterations = 0; % iterations
              
    while true % Note that this can be changed to a While iterations < 10000 type condition also

        
        % Step 4: Update search direction
        temp = norm(gradient(f,x_1))^2 / norm(gradient(f,x_0))^2;
        s = -gradient(f,x_1)/norm(gradient(f,x_1)) + temp * s;
        s = s/norm(s); % normalizing 

        % Step 5: Find lambda k and update x
        lambda = goldenSectionSearch(f, x_1, s, eps1);
        x_0 = x_1;
        x_1 = x_1 + lambda * s;
        
        % Step 6: Checking for termination
        if norm(x_1 - x_0) / norm(x_0) <= eps2 || norm(gradient(f,x_1)) <= eps3
            x_min = x_1;
            f_min = f(x_1);
            break;
        end
        iterations = iterations + 1;
        
    end
end


% Golden section search to find lambda
function lambda = goldenSectionSearch(f, x, s, eps1)
    a = 0;
    b = 1e8; % A large upper bound which can be adjusted as per the optimization problem
    while (b - a) > eps1
        f1 = f(x + (a + (b - a) * 0.382) * s);
        f2 = f(x + (b - (b - a) * 0.382) * s);
        if f2 < f1
            a = (a + (b - a) * 0.382);
        else
            b = (a + (b - a) * 0.382);
        end
    end
    lambda = (a + b) / 2;
end
