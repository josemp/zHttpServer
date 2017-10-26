typedef int (modPut0DataCB_t)(
             void *dataCls
           , char *key
           , char *value );

typedef int (zHttpAppInitCB_t)(
             modPut0DataCB_t  **appDataCB
           , zHttpAppProcesoCB_t **appProcesoCB
           , zHttpAppEndCB_t **appEndCB
           , void **dataCls);

