fprintf('Welcome to the program for calculating the priority of the groups of buildings!\n');
fprintf('Please, follow the instructions below:\n');
fprintf('1. Enter the priorities of the matrix for hierarchy of buildings\n');
hierarchySaatiMatrix = createSaati(4);
hierarchyLocalPriorities = CalculateSaati(hierarchySaatiMatrix, 4);

fprintf('2. Enter the priorities of the matrix for the cost of building\n');
costOfBuildingSaatiMatrix = createSaati(3);
costOfBuildingLocalPriorities = CalculateSaati(costOfBuildingSaatiMatrix, 3);

fprintf('3. Enter the priorities of the matrix for the first parameter\n');
gp1 = Calculations(hierarchyLocalPriorities, costOfBuildingLocalPriorities);
fprintf('4. Enter the priorities of the matrix for the second parameter\n');
gp2 = Calculations(hierarchyLocalPriorities, costOfBuildingLocalPriorities);
fprintf('5. Enter the priorities of the matrix for the third parameter\n');
gp3 = Calculations(hierarchyLocalPriorities, costOfBuildingLocalPriorities);

% Calculating final result
% writting % of each group
fprintf('6. Enter the percentage of each group\n');
A = input('What is the percentage of group 1? (write type do1uble number ex 0.2): ');
B = input('What is the percentage of group 2? (write type double number): ');
C = input('What is the percentage of group 3? (write type double number): ');

EA = gp1(1) * A + gp2(1) * B + gp3(1) * C;
EB = gp1(2) * A + gp2(2) * B + gp3(2) * C;
EC = gp1(3) * A + gp2(3) * B + gp3(3) * C;

fprintf('The final result is:\n');
fprintf('EA = %f\n', EA);
fprintf('EB = %f\n', EB);
fprintf('EC = %f\n', EC);