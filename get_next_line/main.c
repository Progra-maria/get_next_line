#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;

    fd = open("./test_files/lines_around_10.txt", O_RDONLY);
    while(fd)
    {
        line = get_next_line(fd);
        if (line == NULL)
            break;
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
