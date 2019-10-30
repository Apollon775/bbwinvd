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
    char * stm;
    sprintf(stm, "INSERT INTO fiae2019(hostname) VALUES(%s)", data->name);
    
    if (mysql_query(handle, stm))
        return mysql_err_exit();
    
    return 0;
}
