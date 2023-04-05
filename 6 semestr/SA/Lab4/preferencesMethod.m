function alternativeWeights = preferencesMethod(nExperts, nAlternatives, methodPreference)
    % First, convert the methodPreference
    for i = 1:nExperts

        for j = 1:nAlternatives
            methodPreference(i, j) = nAlternatives - methodPreference(i, j);
        end

    end

    % Second, get the sum of converted methodPreference
    sumConvertedOpinions = zeros(nAlternatives);

    for j = 1:nAlternatives

        for i = 1:nExperts
            sumConvertedOpinions(j) = sumConvertedOpinions(j) + methodPreference(i, j);
        end

    end

	% Third, getting the sum of all
	sumAll = 0;
	for i = 1:nAlternatives
		sumAll = sumAll + sumConvertedOpinions(i);
	end

	% Fourth, getting the alternative weights
	alternativeWeights = zeros(1, nAlternatives);

	for j = 1:nAlternatives
		alternativeWeights(j) = sumConvertedOpinions(j) / sumAll;
	end

end