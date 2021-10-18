#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    const unsigned int buffer_length = 512;
    if (argc < 2)
    {
        fprintf(stderr, "Need input file\n");
        exit(1);
    }
    if (argc < 3)
    {
        fprintf(stderr, "Need output file\n");
        exit(1);
    }
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    int input_file = open(input_filename, O_RDONLY);
    int output_file = open(output_filename, O_CREAT | O_TRUNC | O_WRONLY , 0644); 
    if (input_file == -1)
    {
        fprintf(stderr, "Input file %s.\n Error: %s\n", input_filename, strerror(errno));
        exit(1);
    }
    if (output_file == -1)
    {
        fprintf(stderr, "Output file %s.\n Error: %s\n", output_filename, strerror(errno));
        exit(1);
    }
    unsigned int changed_bytes_count = 0;
    char buffer[buffer_length];
    while (1)
    {
        int count_of_bytes = read(input_file, buffer, buffer_length);
        for (int i = 0; i < count_of_bytes; i++)
        {
            if (buffer[i] >= 'a' && buffer[i] <= 'z')
            {
                buffer[i] -= 32;
                changed_bytes_count++;
            }
        }
        write(output_file, buffer, count_of_bytes);
        if (count_of_bytes < buffer_length)
            break;
    }
    printf("During runtime %d bytes was changed\n", changed_bytes_count);
    close(input_file);
    close(output_file);
    return 0;
}