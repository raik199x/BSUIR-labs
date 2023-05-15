function result = electraMethod(numericRatings, nAlternatives, nCriteria, alternativeWeights, c, d)
    agreementMatrix = zeros(nCriteria, nCriteria);
    % calculating agreement matrix
    for i = 1:nCriteria

        for j = 1:nCriteria

            if i == j
                continue;
            end

            for k = 1:nAlternatives

                if numericRatings(k, i) < numericRatings(k, j)
                    agreementMatrix(i, j) = agreementMatrix(i, j) + alternativeWeights(k);
                end

            end

        end

    end

    disp('Agreement Matrix');
    disp(agreementMatrix');

    % calculating disagreement matrix
    disagreementMatrix = zeros(nCriteria, nCriteria);

    for i = 1:nCriteria

        for j = 1:nCriteria

            if i == j
                continue;
            end

            for k = 1:nAlternatives

                if numericRatings(k, j) < numericRatings(k, i)
                    disagreementMatrix(i, j) = disagreementMatrix(i, j) + numericRatings(k, i) - numericRatings(k, j);
                end

            end

        end

    end

    disp('Disagreement Matrix');
    disp(disagreementMatrix');

    % finding limit value of the agreement index
    disp("Limit value of the agreement index");
    agreementIndex = zeros(1, nCriteria);

    for i = 1:nCriteria

        for j = 1:nCriteria

            if i == j
                continue;
            elseif agreementIndex(1, j) == 0
                agreementIndex(1, j) = agreementMatrix(i, j);
            elseif agreementIndex(1, j) > agreementMatrix(i, j)
                agreementIndex(1, j) = agreementMatrix(i, j);
            end

        end

    end

    disp(agreementIndex);

    % finding limit value of the disagreement index
    disp("Limit value of the disagreement index");
    disagreementIndex = zeros(1, nCriteria);

    for i = 1:nCriteria

        for j = 1:nCriteria

            if i == j
                continue;
            elseif disagreementIndex(1, j) == 0
                disagreementIndex(1, j) = disagreementMatrix(i, j);
            elseif disagreementIndex(1, j) < disagreementMatrix(i, j)
                disagreementIndex(1, j) = disagreementMatrix(i, j);
            end

        end

    end

    disp(disagreementIndex);

    % calculating result
	result = 0;
	for i  = 1:nCriteria
		if agreementIndex(i) > c && disagreementIndex(i) < d
			disp(agreementIndex(i) + " > " + c + " && " + disagreementIndex(i) + " < " + d);
			result = i;
			break;
		end
end
