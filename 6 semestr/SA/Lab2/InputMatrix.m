function resMatrix = InputMatrix(size)
    resMatrix = zeros(size, size);
    for i = 1:size
        for j = 1:size
            fprintf('Enter value at (%d, %d): ', i, j);
            resMatrix(i,j) = input(''); 
        end
    end
end