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
    sprintf(stm, "UPDATE fiae2019 SET OS = \'%s\' WHERE Hostname = \'%s\'; SELECT IF( ROW_COUNT() = 0  THEN INSERT INTO fiae2019(Hostname, OS) VALUES(\'%s\', \'%s\')); ",
            data->kernel, data->name, data->name, data->kernel);
    
    if (mysql_query(handle, stm))
        return -1;
    
    for (int i = 0; data->interfaces[i] != NULL; ++i)
    {    
        sprintf(stm, "UPDATE interfaces SET IPv4 = \'%s\', Hostname = \'%s\', Eingegeben am = NOW() WHERE MAC = \'%s\' ; IF ROW_COUNT() = 0 THEN INSERT INTO interfaces(MAC, IPv4, Hostname) VALUES(\'%s\', \'%s\', \'%s\') ; END IF; ",
                data->interfaces[i]->ipv4, data->name, data->interfaces[i]->physical, data->interfaces[i]->physical, data->interfaces[i]->ipv4, data->name);
    }
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
    
    data->interfaces = malloc(sizeof(ifdata_t **));
    data->ifcount = 0;
    data->ifcap = 1;
    
    return data;
}

void ifdata_del(ifdata_t **interfaces)
{
    for (int i = 0; interfaces[i] != NULL; ++i)
    {
        free(interfaces[i]->physical);
        free(interfaces[i]->ipv4);
    }
    
    free(interfaces);
    
}

void hdata_del(hdata_t *data)
{
    free(data->name);
    free(data->kernel);
    free(data->cpu);
    
    ifdata_del(data->interfaces);
    
    free(data);
}

int if_pushback( hdata_t *data)
{
    int index;
        
    if (data->ifcount > data->ifcap)
    {
        data->interfaces = realloc(data->interfaces, data->ifcap * 2);
        data->ifcap *= 2;
    }
    
    data->interfaces[data->ifcount] = malloc (sizeof(ifdata_t));
    index = data->ifcount;
    data->interfaces[index]->ipv4 = malloc(BUFFER);
    data->interfaces[index]->physical = malloc(BUFFER);
    
    data->ifcount++;
    
    return index;
}





