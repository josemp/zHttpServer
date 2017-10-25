#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include </usr/local/include/microhttpd.h>
#include "spaServer.h"

 int conexionInfoProceso(
             MHD_Connection_t *connection
           , const char *data, size_t *dataLen
           , void *appCls)
           {
			   int ret;
			   printf("proceso data <%ld>\n",*dataLen);
			   *dataLen=0;
			   			   ret= spaServerResponseOK(connection
                , MHD_HTTP_OK
                , "de puta madre");
                if (ret==MHD_YES)
                  printf("con yes\n");
                else  
                  printf("con no\n");
               return(ret);
//			   return(MHD_YES);
		   }

int conexionInfoProcesoEnd(
             MHD_Connection_t *connection           
           , void *appCls)
           {
			   char value[200+1]; 
			   printf("proceso end\n");
			   strcpy(value,"");
			   spaServerGetHeaderKey(connection, "Accept-Encoding",value);

			   return spaServerResponseOK(connection
                , MHD_HTTP_OK
                , "de puta madre");
			   
		   }
           
int conexionInfoEnd(
             void *appCls)
{
	printf("conexion end\n");
	return(MHD_YES);
}             






int conexionInfoInit(
             struct MHD_Connection *connection                                           
           , const char *url
           , const char *method
           , const char *version
           , conexionInfoProcesoCB_t **procesoCB
           , conexionInfoProcesoEndCB_t **procesoEndCB
           , conexionInfoEndCB_t **endCB
           , void **appCls  )
           {
			   printf("conexion\n");
			   *procesoCB=conexionInfoProceso;
			   *procesoEndCB=conexionInfoProcesoEnd;
			   *endCB=conexionInfoEnd;
		   }





main()
{

printf("entrando\n");
spaServerInfo_t *spaServerInfo=spaServerStart (80
             , conexionInfoInit);
    printf("main. daemon <%d>\n",(NULL==spaServerInfo));
  if (NULL == spaServerInfo)
    return 1;

  (void) getchar ();

  spaServerStop(spaServerInfo);

  return 0;
}



