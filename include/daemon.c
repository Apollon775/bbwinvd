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

#define ACC "ACC"
#define ERR "ERR"



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
    
//--------------- Hostname------------
    
    size = recv(sock, buffer, BUFFER-1, 0);
    if (size > 0)
    {
        buffer[size] = '\0';
        strcpy(data->name, buffer);
        
        size = send(sock, (char*)ACC, strlen(ACC), 0);
        if (size != strlen(ACC))
        {
            logwrite(logfile, "Send() error", DLOG_MSG);
        }
    }
    else
    {
        send(sock, (char*)'1' , sizeof(char), 0);
        free(buffer);
        return -1;
    }
    
//--------------Kernel--------------
    
    size = recv(sock, buffer, BUFFER-1,0);
    if (size > 0)
    {
        buffer[size] = '\0';
        strcpy(data->kernel, buffer);
        
        size = send(sock, (char*)ACC, strlen(ACC), 0);
        if (size != strlen(ACC))
        {
            logwrite(logfile, "Send() error", DLOG_MSG);
        }
        
    }
    
 //-----------Interfaces--------------
 
    do
    {
        int index;
        
        size = recv(sock, buffer, BUFFER-1, 0);
        if (size > 0)
        {
            buffer[size] = '\0';
            index = if_pushback(data);
            strcpy(data->interfaces[index]->physical, buffer);
        }
        
        size = recv(sock, buffer, BUFFER-1, 0);
        if (size > 0)
        {
            buffer[size] = '\0';
            strcpy(data->interfaces[index]->ipv4, buffer);
            size = send(sock, (char*)ACC, strlen(ACC), 0);
            if (size != strlen(ACC))
            {
                logwrite(logfile, "Send() error", DLOG_MSG);
            }
        }
        
        
        size = recv(sock, buffer, BUFFER-1, 0);
        if (size > 0)
        {
            buffer[size] = '\0';
        }
        
        
    } while (!(strcmp(buffer, "NXT")));
    
 
    
    
    
    return 0;
}
