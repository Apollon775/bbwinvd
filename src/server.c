#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <mysql.h>

#include "query.h"
#include "connection.h"

int main (int argc, char **argv)
{
    pid_t pid, sid;
    FILE *logfile = create_daemon(&sid, &pid, "/var/log/bbwinvd.log");
    
    if (logfile == NULL)
    {
       kill(pid, SIGKILL);
    }


    MYSQL *my_handle = connect_sql();
    if (my_handle == NULL)
    {
        fputs("Could connect to mysql@localhost\n", logfile);
        kill(pid, SIGKILL);
    }

    mysql_close(my_handle);
    fclose(logfile);

    while(1)
    {
        printf("Alles cool!");
    }
}

