#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <errno.h>
#include </usr/local/include/microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include "routeList.h"
#include "spaServer.h"




/*
 * Utility to get HTTP header values
 */


/**
 * 
 * @brief struct for get HTTP header values
 *
 * Esta estrcutura es creada  por spaServer por la llamada a la funcion spaServerStart
 * Es liberada por el propio spaServer cuando se llama a spaServerStop
 * 
 */
typedef struct
{
  char *key;
  char *value;	
} zServerPair_t;

int zServeFileIterateHead (
              void *cls                  // custom value specified when iteration was triggered;
            , enum  MHD_ValueKind  kind  // Iterator over key-value pairs.
                                         // This iterator can be used for several kinds of data
            , const char          *key   // key for the value, can be an empty string
            , const char          *value //value corresponding value, can be NULL
            )
{
	zServerPair_t *pair = (zServerPair_t *) cls;
	
	if (strcmp(pair->key,key)==0)
	{
	   if (value!=NULL)	
	    sprintf(pair->value,"%.*s",MAX_HEADER_VALUE_SIZE,value);
	   return(MHD_NO); // Ya hemos encontrado lo que queriamos, no iteres mas
    }
	return(MHD_YES);
}


 char *spaServerGetHeaderKey(MHD_Connection_t *conexion, char *key,char *value)
{
 	
 zServerPair_t pair;
 
 pair.key=key;
 pair.value=value;
 
 
 MHD_get_connection_values (conexion
                             , MHD_HEADER_KIND
                             , zServeFileIterateHead
                             , &pair);
                         
  value=pair.value;
  return(pair.value);                           
}

static void
request_completed (void *cls, struct MHD_Connection *connection,
                   void **con_cls, enum MHD_RequestTerminationCode toe)
{
	
	spaServerInfo_t *spaServerInfo=(spaServerInfo_t *)cls;
  spaServerConexionInfo_t *conInfo = *con_cls;
  
  //printf("Request Completed <%d>\n",spaServerInfo->puerto);fflush(stdout);
  if (conInfo->endCB!=NULL) 
      conInfo->endCB(conInfo->appCls);
  //printf("Request Completed x\n");fflush(stdout);
  if (NULL == *con_cls)
    return;
  free(*con_cls);
  *con_cls=NULL;
  //printf("Nos vamos\n");fflush(stdout);
}

// MHD_AccessHandlerCallback
static int 
answer_to_connection (
            void *cls                              // custom value selected at callback registration time;              
          , struct MHD_Connection *connection      // Handle for a connection / HTTP request. With HTTP/1.1,
                                                   // multiple requests can be run over the same connection. 
                                                   // However, MHD will only show one request per TCP connection to the client at any given time.
          , const char *url             // the URL requested by the client;
          , const char *method          // the HTTP method used by the client (GET, PUT, DELETE, POST, etc.);
          , const char *version         // the HTTP version string (i.e. HTTP/1.1);
          , const char *upload_data     // the data being uploaded (excluding headers):
          , size_t     *upload_data_size// set initially to the size of the upload_data provided.
          , void      **conInfoCls          // Para closure conexion. Inicialmente a nulo
       )
{
spaServerInfo_t *spaServerInfo=(spaServerInfo_t *)cls;
	

spaServerConexionInfo_t *conInfo = *conInfoCls;
int ret;


      //printf("ANSWER_TO_CONNECTION: upload_data_size = <%ld>\n", *upload_data_size);
// -----------------------------------
//    PRIMER EVENTO
//    Si enviamos respuesta, ya no habrá mas eventos answer to conexion
// -----------------------------------


  if (NULL == *conInfoCls)
    {
	  
      //printf("URL: [%s]\n",url);
      //printf("METODO: [%s]\n",method);
      //printf("VERSION: [%s]\n",version);
      
      conInfo=malloc(sizeof(spaServerConexionInfo_t));      
      memset(conInfo,0,sizeof(spaServerConexionInfo_t));
      ret=spaServerInfo->initCB(connection,url,method,version
                    ,&conInfo->procesoCB
                    ,&conInfo->procesoEndCB
                    ,&conInfo->endCB
                    ,&conInfo->appCls);
      *conInfoCls=conInfo;        
      
      return(ret);
 
    } /* fin primer Evento */


// -----------------------------------
//    SIGUIENTES EVENTOS
// -----------------------------------

//printf("SIGIUENTE EVENTO<%d>\n",*upload_data_size);fflush(stdout);
if (*upload_data_size == 0)
ret=conInfo->procesoEndCB(connection,conInfo->appCls);
else
ret=conInfo->procesoCB(connection,upload_data,upload_data_size,conInfo->appCls);

return(ret);


  
}

spaServerInfo_t *
spaServerStart (int port
             , conexionInfoInitCB_t *conexionInfoInitCB)
/*             
             , conexionInfoProcesoCB_t *conexionInfoProcesoCB
             , conexionInfoEndCB_t *conexionInfoEndCB)
*/             
{
  struct MHD_Daemon *daemon;

  spaServerInfo_t *spaServerInfo;
  
  // Init spaServerInfo
  
  spaServerInfo=malloc(sizeof(spaServerInfo_t));
  memset(spaServerInfo,0,sizeof(spaServerInfo_t));
  spaServerInfo->puerto=port;
  spaServerInfo->initCB=conexionInfoInitCB;

  //printf("el puerto es <%d>\n",port);
  
  daemon = MHD_start_daemon (
       MHD_USE_SELECT_INTERNALLY      // OR-ed combination of MHD_FLAG values;
     , port                           // port to bind to;
     , NULL                           // Accept Policy callback to call to check which clients will be allowed to connect (estamos hablando de sockets);
     , NULL                           // extra argument to apc (AcceptPolicyCallback);
     , &answer_to_connection          // AccessHandlerCallback default handler for all URIs;
     , spaServerInfo                  // extra argument to AccessHandlerCallback.
     // Additional arguments are a list of options (type-value pairs, terminated with MHD_OPTION_END).
     , MHD_OPTION_NOTIFY_COMPLETED    // Opcion de notificar la terminacion de la solicitud y envio                
     , request_completed              // Callback de notificacion completada
     , spaServerInfo                  // Argumento extra para el callback de notificacion completada
     , MHD_OPTION_END                 // parametro obligatorio de fin de parametros
    );
    if (daemon == NULL)
        {
			printf("No pudo lanzarse el daemon\n");
        free(spaServerInfo);
        return(NULL);
	    }
    spaServerInfo->daemon=daemon;
    return(spaServerInfo);
}

void spaServerStop (spaServerInfo_t *spaServerInfo)
{
	
  MHD_stop_daemon (spaServerInfo->daemon);
  free(spaServerInfo);	
	
}
