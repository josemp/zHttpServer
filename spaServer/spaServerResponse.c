#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include </usr/local/include/microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "spaServer.h"

int spaServerResponseError(struct MHD_Connection *connection
                     ,unsigned int statusCode
                     ,char *texto)
{
	int ret;
	struct MHD_Response *response;
	  char serror[400];
      const char *errorstr =
        "<html><body>%s</body></html>";
      sprintf(serror,errorstr,texto);
      printf("Enviando error<%s>\n",serror);
     response =
	MHD_create_response_from_buffer (strlen (serror),
					 (void *) serror,
					 MHD_RESPMEM_MUST_COPY);	
	
     if (NULL != response)
        {
          ret =
            MHD_queue_response (connection, statusCode,
                                response);
          MHD_destroy_response (response);
          printf("envio correcto del error<%d>\n",strlen(serror));
          return ret;// Puede ser MHD_YES o MHD_NO
        }
      else
      {
		          return MHD_NO;	
	  }
	
}
//STATUS OK STANDARD ES 200 MHD_HTTP_OK
int spaServerResponseOK(struct MHD_Connection *connection
                , unsigned int status_code
                , char *texto)
{
	int ret;
	char serror[400];
	struct MHD_Response *response;
      const char *errorstr =
        "<html><body>%s</body></html>";
      sprintf(serror,errorstr,texto);
      printf("Peticion correcta <%s>\n",serror);
     response =
	MHD_create_response_from_buffer (strlen (serror),
					 (void *) serror,
					 MHD_RESPMEM_MUST_COPY);	
	
     if (NULL != response)
        {
          ret =
            MHD_queue_response (connection, status_code,
                                response);
          MHD_destroy_response (response);
          printf("envio correcto del error<%d>\n",strlen(serror));
          return ret;// Puede ser MHD_YES o MHD_NO
        }
      else
      {
		          return MHD_NO;	
	  }
	
}
int
spaServerResponseBuffer(MHD_Connection_t  *connection,
           const char *buffer,
           char *contentTipe)
{
  int ret;
  struct MHD_Response *response;
  response =
    MHD_create_response_from_buffer (strlen (buffer),
                                     (void *) buffer,
                     MHD_RESPMEM_MUST_COPY);
  if (!response)
    return MHD_NO;
  MHD_add_response_header (response,
                           MHD_HTTP_HEADER_CONTENT_TYPE,
                           contentTipe);
  ret = MHD_queue_response (connection,
                            MHD_HTTP_OK,
                            response);
  MHD_destroy_response (response);
  return ret;
}
