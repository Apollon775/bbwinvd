#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

MYSQL* connect_sql()
{
    MYSQL *my;
    my = mysql_init(NULL);

    if(my == NULL)
    {
        fprintf(stderr, "Fehler beim Iitialisieren des Handles \n");
        exit (EXIT_FAILURE);
    }

    if( mysql_real_connect(
        my,
        "127.0.0.1",
        "user",
        "Qwert123",
        "BBWInv",
        0,
        NULL,
        0) == NULL)
    {
        fprintf(stderr, "Fehler mysql_real_connect(): %u (%s)\n",
        mysql_errno(my), mysql_error(my));
    }
    else
        printf("Erfolgreich mit dem MySQL-Server verbunden\n");

    return my;
}