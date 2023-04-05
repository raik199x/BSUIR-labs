function globalPriorities = Calculations(hierarchyLocalPriorities, costOfBuildingLocalPriorities)

fprintf('First Matrix)\n');
ALP = CalculateSaati(createSaati(3), 3);
fprintf('Second Matrix)\n');
BLP = CalculateSaati(createSaati(3), 3);
fprintf('Third Matrix)\n');
CLP = CalculateSaati(createSaati(3), 3);

globalPriorities = zeros(3);
fprintf('1 = A, 2 = B, 3 = C\n');
for i = 1:3
    globalPriorities(i) = ALP(i) * hierarchyLocalPriorities(1) + BLP(i) * hierarchyLocalPriorities(2) + CLP(i) * hierarchyLocalPriorities(3) + costOfBuildingLocalPriorities(i) * hierarchyLocalPriorities(4);
    fprintf('GP%d = %f\n', i, globalPriorities(i));
end


end
