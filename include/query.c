#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

#include "query.h"
#include "daemon.h"

int mysql_err_exit()
{
    
    return -1;
}

MYSQL* connect_sql()
{
    MYSQL *my;
    my = mysql_init(NULL);

    if(my == NULL)
    {
        return NULL;
    }

    return my;
}

int insert_data(MYSQL *handle, hdata_t *data)
{
    char stm[BUFFER];
    sprintf(stm, "INSERT INTO fiae2019(Hostname, OS) VALUES(\'%s\', \'%s\')", data->name, data->kernel);
    
    if (mysql_query(handle, stm))
        return -1;
    
    return 0;
}

hdata_t *hdata_init()
{
    hdata_t *data = malloc(sizeof(hdata_t));

    data->name = malloc(BUFFER);
    data->kernel = malloc(BUFFER);
    data->cpu = malloc(BUFFER);
    
    return data;
}

void hdata_del(hdata_t *data)
{
    free(data->name);
    free(data);
}
