#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef void (*sighandler_t)(int);

void handle_sigint(int sig)
{
    printf("SIGINT!!\n");
    // exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[])
{
    struct sigaction act, old;
    act.sa_handler = handle_sigint;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &act, &old) < 0) {
        fprintf(stderr, "sigaction failed\n");
        exit(EXIT_SUCCESS);
    }
    pause();
    return 0;
}
