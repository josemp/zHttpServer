
int spaServerResponseError(struct MHD_Connection *connection
                     ,unsigned int statusCode
                     ,char *texto);

//STATUS OK STANDARD ES 200 MHD_HTTP_OK
int spaServerResponseOK(struct MHD_Connection *connection
                , unsigned int status_code
                , char *texto);
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
