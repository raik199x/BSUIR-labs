function skipComments(fd)

    while true
        data = fread(fd, 1, 'char');

        if data == '#'
            fgetl(fd);
        else
            break;
        end

    end

    fseek(fd, -1, 'cof');
end
