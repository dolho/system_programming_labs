#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Need input file\n");
        exit(1);
    }
    if (argc < 3)
    {
        printf("Need output file\n");
        exit(1);
    }
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    int fIn = open(input_filename, O_RDONLY);
    if (fIn == -1)
    {
        printf("Input file %s.\n Error: %s\n", input_filename, strerror(errno));
        exit(1);
    }
    int fOut = open(output_filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fOut == -1)
    {
        printf("Output file %s.\n Error: %s\n", output_filename, strerror(errno));
        exit(1);
    }
    uint changed_bytes_count = 0;
    char buffer[512];
    
    while (1)
    {
        int bytes = read(fIn, buffer, sizeof(buffer));
        for (uint i = 0; i < sizeof(buffer); i++)
        {
            if (buffer[i] >= 'a' && buffer[i] <= 'z')
            {
                buffer[i] -= 32;
                changed_bytes_count++;
            }
        }
        write(fOut, buffer, sizeof(buffer));
        if (bytes < sizeof(buffer))
            break;
    }
    printf("During runtime %d bytes was changed\n", changed_bytes_count);
    close(fIn);
    close(fOut);
    return 0;
}