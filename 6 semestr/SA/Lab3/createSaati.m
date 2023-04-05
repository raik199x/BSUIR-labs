function M = createSaati(n)
    % CREATE_SAATI_MATRIX Generate an n x n Saati matrix
    %
    % Syntax: M = create_saati_matrix(n)
    %
    % Inputs:
    %   n - Number of elements in the matrix
    %
    % Outputs:
    %   M - A square Saati matrix of size n x n

    % Initialize the matrix to zeros
    M = zeros(n);

    % Loop over the rows and columns
    for i = 1:n

        for j = 1:n

            if i == j
                % For the diagonal elements, set to 1
                M(i, j) = 1;
            elseif i < j
                % For the upper triangular elements, prompt the user for a value
                prompt = sprintf('Enter the weight of %d compared to %d (1/%d): ', i, j, j - i);
                M(i, j) = input(prompt);
            else
                % For the lower triangular elements, calculate the reciprocal of the corresponding upper triangular element
                M(i, j) = 1 / M(j, i);
            end

        end

    end

    % Print the Saati matrix to the console
    disp('Saati matrix:');
    disp(M);
end