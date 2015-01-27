#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char* argv[])
{
    int i;
    for (i = 1; i < argc; i++)
    {
        FILE* f;
        int c;
        int new_line_num;
        f = fopen(argv[i], "r");
        if (!f) {
            perror(argv[i]);
            exit(1);
        }
        new_line_num = 0;
        while((c = fgetc(f)) != EOF) {
            if (c == '\t') {
                errno = 0;
                fputs("\\t", stdout);
                if (errno != 0) exit(1);
            } else if (c == '\n') {
                errno = 0;
                fputs("$\n", stdout);
                new_line_num++;
                if (errno != 0) exit(1);
            } else if (putchar(c) < 0) {
                exit(1);
            }
        }
        printf("%i lines\n", new_line_num);
        fclose(f);
    }
    exit(0);
}
