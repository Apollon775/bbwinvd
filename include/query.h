#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

struct _hostdata
{
    char* name;
};

typedef struct _hostdata hdata_t;

MYSQL* connect_sql();

int insert_data(MYSQL* handle, hdata_t *data);

hdata_t *hdata_init();

void hdata_del(hdata_t *data)


