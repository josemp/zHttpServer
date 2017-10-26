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

int loginProcesoCB(MHD_Connection_t *connection,void *dataCls)
{

 appData_t *data = (appData_t *) dataCls;
 printf("<----------- >    Proceso de datos\n user = <%s>\n password =<%s>\n",data->user,data->password);
return spaServerResponseOK(connection
                , MHD_HTTP_OK
                , "de puta madre");

}
