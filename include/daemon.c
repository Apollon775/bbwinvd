#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

#include "daemon.h"

#define BUFFER 256



int logwrite(FILE* log, const char* msg, int flag)
{
    time_t now = time(NULL);
    char * date = ctime(&now);
    char * flagstr;
    char * new_l;
    
    new_l = strchr(date, '\n');
    memset(new_l, '\0' , sizeof(char));
    
    switch(flag)
    {
        case 0: flagstr = "MSG"; break;
        case 1: flagstr = "ERROR"; break;
        case 2: flagstr = "WARN"; break;
        default: flagstr = "MSG"; break;
    }
    
    fprintf(log, "[%s] [%s]: %s\n", date, flagstr, msg );
    
    fflush(log);

    return 0;
}


// Bindet einen Socket an eine vorgegebene Adresse 
//falls keine Adresse übergeben wird wird IN_ADDRANY angenommen
int bind_sock(char *inetaddr, struct sockaddr_in *address, uint16_t port)
{
    int sock;
   
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
       return -1; 
    }

    if(inetaddr == NULL)
    {
        address->sin_addr.s_addr = INADDR_ANY;
    }
    
    if (port == 0)
    {
        port = 6661;
    }
    
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if(bind(sock, (struct sockaddr*) address, sizeof(*address)) != 0)
    {
        return -2;
    }

    return sock;
}



int recv_data(int sock, hdata_t *data, FILE *logfile)
{
    char *buffer = malloc(BUFFER);
    int size;
    
    size = recv(sock, buffer, BUFFER-1, 0);
    if (size > 0)
    {
            buffer[size] = '\0';
            data->name = buffer;
            logwrite(logfile, buffer, DLOG_MSG);
            size = send(sock, (char*)'0', sizeof(char), 0);
            if (size != sizeof(char))
            {
                free(buffer);
                logwrite(logfile, "Send() error", DLOG_MSG);
                return -1;
            }
    }
    else
    {
        send(sock, (char*)'1' , sizeof(char), 0);
        free(buffer);
        return -1;
    }
    
    free(buffer);
    return 0;
}
