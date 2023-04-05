fprintf('Welcome to the program for calculating lab4!\n');

filename = 'condition.txt';

if exist(filename, 'file') ~= 2
    disp('File with your data was not found, now we will create it');
    disp('Please, fill up the file');
    creatingFile();
    return;
end

disp('File with your data was found, now we will read it');
fd = fopen(filename, 'r');

if fd == -1
    disp('Error while opening file');
    return;
end

% Read data from file
skipComments(fd);
nExperts = fscanf(fd, '%d', 1);
fread(fd, 1, 'char'); % skip newline
skipComments(fd);

nAlternatives = fscanf(fd, '%d', 1);
fread(fd, 1, 'char'); % skip newline
skipComments(fd);

% Reading array of experts
methodPreference = zeros(nExperts, nAlternatives);

for i = 1:nExperts

    for j = 1:nAlternatives
        methodPreference(i, j) = fscanf(fd, '%d', 1);
        fread(fd, 1, 'char');
    end

end

% Displaying readded info
disp("Number of experts: " + nExperts);
disp("Number of alternatives: " + nAlternatives);
disp("Method of preference:")
disp(methodPreference);

disp('Weights of alternatives:');
alternativeWeights = preferencesMethod(nExperts, nAlternatives, methodPreference);
disp(alternativeWeights)

% reading number of criteria for kememeny-snell mythod
skipComments(fd);
nCriteria = fscanf(fd, '%d', 1);
fread(fd, 1, 'char'); % skip newline
skipComments(fd);

% reading array of criteria
rankingMatrix = zeros(nAlternatives, nCriteria);

for i = 1:nAlternatives

    for j = 1:nCriteria
        rankingMatrix(i, j) = fscanf(fd, '%d', 1);
        fread(fd, 1, 'char');
    end

end

disp('Ranking matrix:');
disp(rankingMatrix);

modifiedKemmenySnellMethod(rankingMatrix, nAlternatives, nCriteria, alternativeWeights);

% reading numeric ratings for electra method
skipComments(fd);
numericRatings = zeros(nAlternatives, nCriteria);

for i = 1:nAlternatives

    for j = 1:nCriteria
        numericRatings(i, j) = fscanf(fd, '%f', 1);
        fread(fd, 1, 'char');
    end

end

skipComments(fd);
c = fscanf(fd, '%f', 1);
fread(fd, 1, 'char');
skipComments(fd);
d = fscanf(fd, '%f', 1);
fread(fd, 1, 'char');

disp('Numeric ratings:');
disp(numericRatings);

disp("C* = " + c);
disp("D* = " + d);

electraResult = electraMethod(numericRatings, nAlternatives, nCriteria, alternativeWeights, c, d);
disp('Electra result:');
disp(electraResult);
disp("Ended Calculations");