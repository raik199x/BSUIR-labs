% entering base matrix
baseMatrix = InputMatrix(3);
baseLocalPriorities = CalculateSaati(baseMatrix, 3);

% entering first
matrixOne = InputMatrix(5);
firstLocalPriorities = CalculateSaati(matrixOne, 5);

% entering second
matrixTwo = InputMatrix(5);
secondLocalPriorities = CalculateSaati(matrixTwo, 5);

% entring third
matrixThree = InputMatrix(5);
thirdLocalPriorities = CalculateSaati(matrixThree, 5);

% result
globalPriorities = zeros(5);
for i = 1:5
    globalPriorities(i) = firstLocalPriorities(i) * baseLocalPriorities(1) + secondLocalPriorities(i) * baseLocalPriorities(2) + thirdLocalPriorities(i) * baseLocalPriorities(3);
    fprintf('GP%d = %f\n', i, globalPriorities(i));
end