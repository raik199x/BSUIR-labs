function result = modifiedKemmenySnellMethod(rankingMatrix, nAlternatives, nCriteria, alternativeWeights)
    % modifiedKemmenySnellMethod - modified Kemmeny-Snell method
    matrcies = zeros(nCriteria, nCriteria, nAlternatives);

    % Creating matrix of pairwise comparisons
    for curAlternative = 1:nAlternatives

        for curCriteria = 1:nCriteria

            for i = 1:nCriteria
 
                if curCriteria == i
                    matrcies(curCriteria, i, curAlternative) = 9;
                elseif rankingMatrix(curAlternative, curCriteria) > rankingMatrix(curAlternative, i)
                    matrcies(curCriteria, i, curAlternative) = 1;
                elseif rankingMatrix(curAlternative, curCriteria) < rankingMatrix(curAlternative, i)
                    matrcies(curCriteria, i, curAlternative) = -1;
                end

            end

        end

    end

    % displaying created matrix
    disp("Matrix of pairwise comparisons: ")
    disp("Note: 9 means that that comprasion was skipped")

    for i = 1:nAlternatives
        disp(matrcies(:, :, i)');
    end

    % creating matrix of lost
    lostMatrix = zeros(nCriteria, nCriteria);

    for i = 1:nCriteria

        for j = 1:nCriteria

            if i == j
                lostMatrix(i, j) = 0;
            else

                for k = 1:nAlternatives
                    lostMatrix(i, j) = lostMatrix(i, j) + (alternativeWeights(k) * abs(matrcies(i, j, k) - 1));
                end

            end

        end

    end

    disp("Matrix of lost: ")
    disp("Note: 0 means that that comprasion was skipped")
    disp(lostMatrix');

    % Calculating preliminary ranking of alternatives
    disp("Calculating preliminary ranking of alternatives: ");
	result  = zeros(1, nCriteria);
    for prep = 1:nCriteria-1
        preliminary_ranking_of_alternatives = zeros(1, nCriteria);

        for i = 1:nCriteria

            for j = 1:nCriteria
                preliminary_ranking_of_alternatives(i) = preliminary_ranking_of_alternatives(i) + lostMatrix(j, i);
            end

        end

        % removing 0 (since its removed lines)
        without_zeros_preliminary_ranking_of_alternatives = preliminary_ranking_of_alternatives(preliminary_ranking_of_alternatives ~= 0);
        disp(without_zeros_preliminary_ranking_of_alternatives);

        without_zeros_preliminary_ranking_of_alternatives = min(without_zeros_preliminary_ranking_of_alternatives);
        disp("The best is: " + without_zeros_preliminary_ranking_of_alternatives);
        idx = find(preliminary_ranking_of_alternatives == without_zeros_preliminary_ranking_of_alternatives);
        lostMatrix(:, idx(1)) = 0; % setting all elements in column to 0
        lostMatrix(idx(1), :) = 0; % setting all elements in row to 0
		result(prep) = idx(1);
		if(prep == nCriteria-1)
			disp("The last is: " + max(preliminary_ranking_of_alternatives));
			result(prep+1) = find(preliminary_ranking_of_alternatives == max(preliminary_ranking_of_alternatives));
		end
    end

	disp("Preliminary ranking of alternatives: ");
	disp("Note:remember, indexes might change since program does not know about criteria that was deleted by parreto");
	disp(result);

	% calculating final ranking of alternatives
	disp("Calculating finel result: ");
	for i = nCriteria-1:-1:1
		if lostMatrix(result(i), result(i+1)) > lostMatrix(result(i+1), result(i))
			disp("R"+result(i)+result(i+1)+" is bigger than R"+result(i+1)+result(i)+", so we swap them");
			temp = result(i);
			result(i) = result(i+1);
			result(i+1) = temp;
		else
			disp("R"+result(i)+result(i+1)+" is smaller than R"+result(i+1)+result(i)+", so we do not swap them");
		end
	end
	disp("Final result: ")
	disp(result);
end
