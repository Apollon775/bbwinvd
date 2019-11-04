#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

struct _interfacedata
{
    char* ipv4;
    char* physical;
};

typedef struct _interfacedata ifdata_t;

struct _hostdata
{
    char* name;
    char* kernel;
    char* cpu;
    ifdata_t **interfaces;
    size_t ifcount;
    size_t ifcap;
};

typedef struct _hostdata hdata_t;

MYSQL* connect_sql();

int insert_data(MYSQL* handle, hdata_t *data);

hdata_t *hdata_init();

void hdata_del(hdata_t *data);

int if_pushback( hdata_t *data);

void ifdata_del(ifdata_t **interfaces);




