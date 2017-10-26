#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include </usr/local/include/microhttpd.h>
#include "spaServer.h"
#include "appComun.h"
#include "moduloPut0.h"
#include "loginApp.h"




int conexionInfoInit(
             struct MHD_Connection *connection                                           
           , const char *url
           , const char *method
           , const char *version
           , conexionInfoProcesoCB_t **procesoCB
           , conexionInfoProcesoEndCB_t **procesoEndCB
           , conexionInfoEndCB_t **endCB
           , void **modCls  )
           {
            int ret;
            // Inicia El modulo

            ret=modPut0InitCB(
        		  connection
           		, loginInitCB
           		, procesoCB
           		, procesoEndCB
           		, endCB
           		, modCls  );

             return(ret);
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

