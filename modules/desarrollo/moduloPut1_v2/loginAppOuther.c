#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include </usr/local/include/microhttpd.h>
#include "spaServer.h"
#include "appComun.h"
#include "moduloPut0.h"
#include "loginApp.h"


/* 
 * Este programa lo unico que tiene que hacer es procesar unos datos 
 * de una estructura
*/





int loginDataCB(void *dataCls, char *key, char *value)
{
 appData_t *data = (appData_t *) dataCls;

 if (strcmp(key,"user")==0)
   strcpy(data->user,value);
 if (strcmp(key,"password")==0)
   strcpy(data->password,value);

}




loginEndCB(void *dataCls)
{
   if (dataCls!=NULL)
    free(dataCls);
}
/*
loginProcesoCB(MHD_Connection_t *connection,void *dataCls)
{

 appData_t *data = (appData_t *) dataCls;
 printf("<----------- >    Proceso de datos\n user = <%s>\n password =<%s>\n",data->user,data->password);
return spaServerResponseOK(connection
                , MHD_HTTP_OK
                , "de puta madre");

}
*/




int loginInitCB(modPut0DataCB_t **appDataCB
        , zHttpAppProcesoCB_t **appProcesoCB
        , zHttpAppEndCB_t **appEndCB
        , void **dataCls)
{

// inicia and set Closure

appData_t *data=malloc(sizeof(appData_t));
memset(data,0,sizeof(appData_t));
*dataCls=(void *)data;

// set CallBack Events
  
  *appDataCB=loginDataCB;
  *appProcesoCB=loginProcesoCB;
  *appEndCB=loginEndCB;

printf("pero esto que eeees 2\n");fflush(stdout);
return(0);
}
