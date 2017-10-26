#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include </usr/local/include/microhttpd.h>
#include "spaServer.h"


/* 
 * Este programa lo unico que tiene que hacer es procesar unos datos 
 * de una estructura
*/


typedef struct
{
 char user[100];
 char password[100];
} appData_t;


int appDataCB(void *dataCls, char *key, char *value)
{
 appData_t *data = (appData_t *) dataCls;

 if (strcmp(key,"user")==0)
   strcpy(data->user,value);
 if (strcmp(key,"password")==0)
   strcpy(data->password,value);

}




appInitCB(void **dataCls)
{
printf("pero esto que eeees\n");fflush(stdout);
appData_t *data=malloc(sizeof(appData_t));
memset(data,0,sizeof(appData_t));

printf("pero esto que eeees 1\n");fflush(stdout);
*dataCls=(void *)data;

printf("pero esto que eeees 2\n");fflush(stdout);
}
appEndCB(void *dataCls)
{
   if (dataCls!=NULL)
    free(dataCls);
}
appProcesoCB(MHD_Connection_t *connection,void *dataCls)
{

 appData_t *data = (appData_t *) dataCls;
 printf("<----------- >    Proceso de datos\n user = <%s>\n password =<%s>\n",data->user,data->password);
return spaServerResponseOK(connection
                , MHD_HTTP_OK
                , "de puta madre");

}

