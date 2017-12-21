#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;
int main ( int argc, char **argv )
{
    // For testing purposes, we can set SIGPIPE-ignore here
    char * foo = getenv("SCPIPEWRENCH_SIG");
    if (foo != NULL && strcmp(foo, "SIG_IGN"))
    {
        if (! unsetenv("SCPIPEWRENCH_SIG"))
        {
            perror("Failed to clear environment variable SCPIPEWRENCH_SIG");
            exit(1);
        }
        if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        {
            perror("Failed to set SIGPIPE to ignore.");
            exit(1);
        }
    }
    else
    {
        if (signal(SIGPIPE, SIG_DFL) == SIG_ERR)
        {
            perror("Failed to set SIGPIPE to default handler.");
            exit(1);
        }
    }
    execve(argv[1], &argv[1], environ);
    perror("Failed exec.");
    return 1;
}
