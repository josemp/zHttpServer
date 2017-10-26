#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include </usr/local/include/microhttpd.h>
#include <spaServer.h>
#include "appComun.h"
#include "moduloPut0.h"
// este modulo necesita ser POST Y el encode =MHD_HTTP_POST_ENCODING_FORM_URLENCODED,

#define POSTBUFFERSIZE  512


// Tipo callback de inicio de este modulo propuesto
typedef int (zhdModInitCB_t)(
             MHD_Connection_t *connection
           , void **modCls
           , void *appCls  );



typedef struct {
     modPut0DataCB_t *appDataCB;
     zHttpAppProcesoCB_t *appProcesoCB;
     zHttpAppEndCB_t *appEndCB;
     struct MHD_PostProcessor *postProcessor;
     int endRequest;
     void *appCls;
     
}modPut0_t;



static int
modPut0IteratePost (void *cls, enum MHD_ValueKind kind, const char *key,
              const char *filename, const char *content_type,
              const char *transfer_encoding, const char *data, uint64_t off,
              size_t size)
{
printf("EmodPut0IteratePost  1\n");fflush(stdout);
 modPut0_t *modPut0 = (modPut0_t *) cls;

 if (modPut0->endRequest==1) // Siempre manda algo despues de cerrar
      return MHD_YES;        // el procesador, hago esto para que no ocurra
printf("EmodPut0IteratePost  2\n");fflush(stdout);    
    // App recoge valores
    modPut0->appDataCB(modPut0->appCls,(char *)key, (char *)data);
printf("EmodPut0IteratePost  3\n");fflush(stdout);    

printf("POST  key=<%s> filename=<%s> content_type=<%s> transfer_encoding=<%s> size=<%d> data=<%.*s> offset =<%d>\n"
       , key,filename,content_type,transfer_encoding,size,size,data, off);
//peper
return MHD_YES;
}


int modPut0ProcesoCB(
             MHD_Connection_t *connection
           , const char *data, size_t *dataLen
           , void *modCls)
{
printf("modPut0ProcesoCB\n");fflush(stdout);
	
   modPut0_t *modPut0 =(modPut0_t *) modCls;
    printf("modPut0ProcesoCB 1 ---><%ld>\n",*dataLen);fflush(stdout);        
    if (modPut0->postProcessor==NULL) { printf("ES QUE ES NULLLLLL\n");fflush(stdout);}
   // Aqui tenemos una iteración de datos           
   if (MHD_post_process (modPut0->postProcessor,
                                data,
                                *dataLen) != MHD_YES)
      {// No estoy convencido de que esto funcione
                // Lo peor que puede pasar es que corte la conexion
                  printf("modPut0ProcesoCB 2\n");fflush(stdout);
                *dataLen=0;
                return  spaServerResponseError(connection
                  , MHD_HTTP_BAD_REQUEST
                  , "Error processing POST data");

      }
    *dataLen=0;
    printf("modPut0ProcesoCB final\n");fflush(stdout);
    return(MHD_YES);
}

int modPut0ProcesoEndCB(
             MHD_Connection_t *connection           
           , void *modCls)
           {
 modPut0_t *modPut0 = (modPut0_t *) modCls;
 modPut0->appProcesoCB(connection,modPut0->appCls);
}

int modPut0EndCB(
             void *modCls)
{
 modPut0_t *modPut0 = (modPut0_t *) modCls;
	printf("modPut0EndCB\n");

// Cierra app

   modPut0->appEndCB(modPut0->appCls);
      
// Cierra el postprocessor

   if (modPut0->postProcessor!=NULL)
           MHD_destroy_post_processor (modPut0->postProcessor);

// Libera la closure o handler
   
   free(modPut0);

   return(0); // No vale para nada pero por compatibilidad lo usamos

}             

int modPut0InitCB(
             MHD_Connection_t *connection
           , zHttpAppInitCB_t *appInitCB
           , conexionInfoProcesoCB_t **procesoCB
           , conexionInfoProcesoEndCB_t **procesoEndCB
           , conexionInfoEndCB_t **endCB
           , void **modCls  )

{
printf("modPut0InitCB\n");fflush(stdout);

// Create and set closure

 modPut0_t * modPut0 = malloc(sizeof(modPut0_t));
 memset(modPut0,0,sizeof(modPut0_t));
 *modCls=(void *) modPut0;
 
// Iniciando la aplicacion

 appInitCB(
            &modPut0->appDataCB
          , &modPut0->appProcesoCB
          , &modPut0->appEndCB
          , &modPut0->appCls);


// Set Module Events

      *procesoCB=modPut0ProcesoCB;
      *procesoEndCB=modPut0ProcesoEndCB;
      *endCB=modPut0EndCB;


// Genera el postProcessor

 modPut0->postProcessor=MHD_create_post_processor (
               connection
             , POSTBUFFERSIZE 
             , modPut0IteratePost  
             , modPut0             
             );
             
 if (modPut0->postProcessor==NULL)
     return spaServerResponseError(connection
                            , MHD_HTTP_BAD_REQUEST
                            , "Error processing POST data");

// Nos vamos

printf("modPut0InitCB end\n");fflush(stdout);


return(MHD_YES); // Solo queda el ret= YES

}

