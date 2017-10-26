typedef int (modPut0DataCB_t)(
             void *dataCls
           , char *key
           , char *value );

typedef int (zHttpAppProcesoCB_t)(
            MHD_Connection_t *connection
          , void *dataCls);

typedef int (zHttpAppEndCB_t)(
            void *dataCls);

int modPut0InitCB(
             MHD_Connection_t *connection
           , modPut0DataCB_t  *appDataCB
           , zHttpAppProcesoCB_t *appProcesoCB
           , zHttpAppEndCB_t *appEndCB
           , void *appCls
           , void **modCls  );




int modPut0ProcesoCB(
             MHD_Connection_t *connection
           , const char *data, size_t *dataLen
           , void *modCls);

int modPut0ProcesoEndCB(
             MHD_Connection_t *connection
           , void *modCls);

int modPut0EndCB(
             void *modCls);


