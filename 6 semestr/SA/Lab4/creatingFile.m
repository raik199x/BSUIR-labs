function creatingFile()
    file = fopen('condition.txt', 'w');
    fprintf(file, '# This is the file for your condition to run lab 4\n# All line starting with # will be ignored\n');
    fprintf(file, '# This program solve ONLY second way of calculating\n');
    fprintf(file, '# NOTE: the set of parreto must be already applied to your task\n');
    fprintf(file, '# There will be used preference method\n#\n');
    fprintf(file, '# Please write number of experts\n');
    fprintf(file, '2\n');
    fprintf(file, '# Please write number of alternatives\n');
    fprintf(file, '3\n');
    fprintf(file, '# Please write preferences of two experts\n');
    fprintf(file, '2 2 1\n1 3 2\n');
    fprintf(file, '# End of preferences method\n#\n');

    fprintf(file, '# Modified Kemeny-Snell algorithm\n');
    fprintf(file, '# Please write number of criteria\n');
    fprintf(file, '5\n');
    fprintf(file, '# Write alternatives with ranking according to each of the criteria\n');
    fprintf(file, '4 2 1 3 4\n');
    fprintf(file, '2 4 3 1 3\n');
    fprintf(file, '2 3 4 3 1\n');
    fprintf(file, '# End of modified Kemeny-Snell algorithm\n#\n');

    fprintf(file, '# ELECTRA method\n# Please, write dimensionless estimates\n');
    fprintf(file, '0.36 0.8 0.9 0.62 0.36\n');
    fprintf(file, '0.7 0.36 0.2 0.79 0.62\n');
    fprintf(file, '0.7 0.62 0.36 0.62 0.9\n');
    fprintf(file, "# Enter C* value\n");
    fprintf(file, '0.5\n');
    fprintf(file, '# Enter D* value\n');
    fprintf(file, '0.5\n');
    fprintf(file, '# End of ELECTRA method\n# End of file');
end
