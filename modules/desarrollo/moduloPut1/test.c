#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include </usr/local/include/microhttpd.h>
#include "spaServer.h"
#include "moduloPut0.h"
#include "app.h"





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
            void *appCls;
                  printf("Comenzaaaando\n");fflush(stdout);
                  appInitCB(&appCls);
                  
		   printf("conexionInfoInit 1\n");fflush(stdout);
		   *procesoCB=modPut0ProcesoCB;
		   *procesoEndCB=modPut0ProcesoEndCB;
		   *endCB=modPut0EndCB;
		   printf("conexionInfoInit 2\n");fflush(stdout);

            modPut0InitCB( connection
                       ,  *appDataCB
                       ,  *appProcesoCB
                       ,  *appEndCB
                       , appCls
                       , modCls  );

		   printf("conexionInfoInit 3\n");fflush(stdout);
             return(MHD_YES);
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

