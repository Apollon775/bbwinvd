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
    
    switch(flag)
    {
        case 0: flagstr = "msg"; break;
        case 1: flagstr = "error"; break;
        case 2: flagstr = "warning"; break;
        default: flagstr = "msg"; break;
    }
    
    fprintf(log, "[%s] %s: %s\n", date, flagstr, msg );

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

    if(bind(sock, (struct sockaddr*) address, sizeof(address)) != 0)
    {
        return -1;
    }

    return sock;
}



int recv_data(int sock, hdata_t *data)
{
    char *buffer = malloc(BUFFER);
    
    read(sock, (char*) buffer, BUFFER);
    
    if (buffer != NULL)
    {
        data->name = buffer;
        write(sock, (int*) '1', 1);
    }
    else
    {
        write(sock, (int*) '0', 1);
        return -1;
    }
    
    return 0;
}
