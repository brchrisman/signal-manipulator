#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;
int main ( int argc, char **argv )
{
    char * signalHandlerDisposition = getenv("SIGNAL_WRENCH_DISPOSITION");
    char * signalString = getenv("SIGNAL_WRENCH_SIGNAL_STRING");

    if (signalHandlerDisposition)
    {
        if (! unsetenv("SIGNAL_WRENCH_DISPOSITION"))
        {
            perror("Failed to clear environment variable SIGNAL_WRENCH_DISPOSITION");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "Must set SIGNAL_WRENCH_DISPOSITION.\n");
        exit(1);
    }

    if (signalString)
    {
        if (! unsetenv("SIGNAL_WRENCH_SIGNAL_STRING"))
        {
            perror("Failed to clear environment variable SIGNAL_WRENCH_SIGNAL_STRING");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "Must set SIGNAL_SIGNAL_STRING.\n");
        exit(1);
    }

    if (! strncmp(signalHandlerDisposition, "SIG_IGN", 8) && ! strncmp(signalHandlerDisposition, "SIG_DFL", 8))
    {
        fprintf(stderr, "Disposition must be SIG_IGN or SIG_DFL, not (%s).\n", signalHandlerDisposition);
        exit(1);
    }

    int i, signalNumber;

    for (i = 1; i < 32; i++)
    {
        if (strncmp(signalString, strsignal(i), strlen(strsignal(i))))
        {
            signalNumber = i;
            break;
        }
    }

    if (i == 32)
    {
        fprintf(stderr, "No signal (%s) found.\n", signalString);
        exit(1);
    }

    if (strncmp(signalHandlerDisposition, "SIG_IGN", 8))
    {
        if (signal(signalNumber, SIG_IGN) == SIG_ERR)
        {
            perror(strcat("Failed to set ignore handler for signal -- ", strsignal(signalNumber)));
            exit(1);
        }
    }
    else
    {
        if (signal(signalNumber, SIG_DFL) == SIG_ERR)
        {
            perror(strcat("Failed to set default handler for signal -- ", strsignal(signalNumber)));
            exit(1);
        }
    }
    execve(argv[1], &argv[1], environ);
    perror("Failed exec.");
    return 1;
}
