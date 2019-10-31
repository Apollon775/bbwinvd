#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <mysql.h>

#include <query.h>
#include <daemon.h>


int main(void)
{
    pid_t pid, sid;
    
    int sock, new_sock;
    socklen_t addrlen;
    struct sockaddr_in addr;
    ssize_t size;
    const int y = -1;
    
    hdata_t *data;
   
    
    pid = fork();
    
    if ( pid < 0)
    {
        printf("could not fork()");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0)
    {
        printf("neuer child process %d", pid);
        exit(EXIT_SUCCESS);
    }
    
    umask(0);
    
    char* logmsg = malloc(BUFFER);
    
    FILE *logfile = fopen("var.log", "a+");
    
    if (logfile == NULL)
    {
       free(logmsg);
       exit(EXIT_FAILURE);
    }
    else
    {
        logwrite(logfile, "bbwinvd wurde gestartet", DLOG_MSG);
    } 
    
    sid = setsid();
    
    if (sid < 0)
    {
        fputs("Fehler: sid konnte nicht korrekt initialisert werden\n", logfile);
        fclose(logfile);
        free(logmsg);
        exit(EXIT_FAILURE);
    }
    
    close(STDIN_FILENO);
    close(STDERR_FILENO);
    close(STDOUT_FILENO);
    
    sock = bind_sock(NULL, &addr, 25001);
    if (sock ==  -1)
    {
        logwrite(logfile, "Socket konnte nicht erstellt werden", DLOG_ERR);
        free(logmsg);
        fclose(logfile);
        exit(EXIT_FAILURE);
    } else if ( sock == -2)
    {
        sprintf(logmsg, "bind(): Socket konnte auf Port %i nicht gebindet werden", ntohs(addr.sin_port)); 
        logwrite(logfile, logmsg, DLOG_ERR);
        fclose(logfile);
        exit(EXIT_FAILURE);
    }
    
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
    
    MYSQL *my_handle = connect_sql();
    if( mysql_real_connect(
        my_handle,
        "127.0.0.1",
        "user",
        "Qwert123",
        "BBWInv",
        0,
        NULL,
        0) == NULL)
    {
        fprintf(logfile, "Fehler mysql_real_connect(): %u (%s)\n",
        mysql_errno(my_handle), mysql_error(my_handle));
    }
    
    listen (sock, 5);
    addrlen = sizeof(struct sockaddr_in);
    
    while(1)
    {
        new_sock = accept(sock,(struct sockaddr*) &addr, &addrlen);
        if (new_sock > 0)
        {
            
            sprintf(logmsg, "host mit der Adresse %s hat sich verbunden", inet_ntoa(addr.sin_addr));
            logwrite(logfile, logmsg, DLOG_MSG);
        }
            
        if (recv_data(new_sock, data) == 0)
        {
            insert_data(my_handle, data);
        }
        else
        {
            sprintf(logmsg,"Vom Client bekommene Daten fehlerhaft");
            logwrite(logfile, logmsg, DLOG_ERR);
        }
        
        close(new_sock);
    }
    
    mysql_close(my_handle);
    fclose(logfile);

}

