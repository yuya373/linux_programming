#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#define _GNU_SOURCE
#include <getopt.h>


static void do_grep(regex_t* pat, FILE* f, int opt);
static int opt_invert = 1;
static int opt_ignorecase = 1;

int main(int argc, char* argv[])
{
    regex_t pat;
    int opt;

    while ((opt = getopt(argc, argv, "vi")) != -1) {
        switch (opt) {
            case 'v':
                opt_invert = 0;
                 break;
            case 'i':
                 opt_ignorecase = 0;
                 break;
            case '?':
                 fprintf(stderr, "-v or -i\n", argv[0]);
                 exit(1);
        }
    }

    if (argc < 2) {
        fputs("no pattern\n", stderr);
        exit(1);
    }

    int re_mode = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
    if (opt_ignorecase != 1) {
        re_mode |= REG_ICASE;
    }

    int err = regcomp(&pat, argv[optind], re_mode);
    if (err != 0) {
        char buf[1024];

        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }
    if (argc == optind + 1) {
        do_grep(&pat, stdin, opt);
    } else {
        int i;
        for (i = optind + 1; i < argc; i++) {
            FILE* f;
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f, opt);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t* pat, FILE* f, int opt)
{
    char buf[4096];
    while (fgets(buf, sizeof buf, f)) {
        int matched = regexec(pat, buf, 0, NULL, 0);
        if (opt_invert == 0) {
            if (matched != 0) {
                fputs(buf, stdout);
            }
        }
        if (opt_invert == 1) {
            if (matched == 0) {
                fputs(buf, stdout);
            }
        }
    }
}
