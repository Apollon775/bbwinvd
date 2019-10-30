#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include "connection.h"

FILE* create_daemon(pid_t *sid, pid_t *pid, char *logpath)
{
    FILE *log;

   *pid = fork();
    if (*pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if (*pid > 0)
    {
        printf("Hallo %i", *pid);
        exit(EXIT_SUCCESS);
    }

    umask(0);


    if(logpath != NULL)
    {
        log = fopen(logpath, "w");
        if( log == NULL)
        {
            printf("logaptah konnte nicht gefunden werden");
            return NULL;
        }
    }

    *sid = setsid();
    printf("%i\n", *sid);
    if (*sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    return log;
}