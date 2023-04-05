function localPriorities = CalculateSaati(inputMatrix, size)
    % Calculating geometric mean of matrix rows
    geometrixMeanMatrixRows = zeros(size); % empty array

    for i = 1:size
        geometrixMeanMatrixRows(i) = inputMatrix(i, 1);

        for j = 2:size
            geometrixMeanMatrixRows(i) = geometrixMeanMatrixRows(i) * inputMatrix(i, j); % summing all elements in line
        end

        geometrixMeanMatrixRows(i) = nthroot(geometrixMeanMatrixRows(i), size);
    end

    sumGeometricMeans = 0;

    for i = 1:size
        fprintf('C%d = %f\n', i, geometrixMeanMatrixRows(i));
        sumGeometricMeans = sumGeometricMeans + geometrixMeanMatrixRows(i);
    end

    fprintf('Sum of geometric Means is %f\n', sumGeometricMeans);

    localPriorities = zeros(size);

    for i = 1:size
        localPriorities(i) = geometrixMeanMatrixRows(i) / sumGeometricMeans;
        fprintf('LP%d =  %f\n', i, localPriorities(i));
    end

end
