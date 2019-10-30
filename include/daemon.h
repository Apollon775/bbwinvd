#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "query.h"

#define DLOG_MSG 0
#define DLOG_ERR 1
#define DLOG_WAR 2


//gibt den FD eines Sockets zurueck der entwerde mit inetaddr und auf port erstellt wird
// wenn address == NULL damnn wird INADDR_ANY angenommen
//wenn port == 0, dann port = 6661
int bind_sock(char *inetaddr, struct sockaddr_in *address, uint16_t port);

//Funktion zum Schreiben von von Log-eintraegen
int logwrite(FILE* log, const char  *msg, int flag);

int recv_data(int sock, hdata_t *data);



