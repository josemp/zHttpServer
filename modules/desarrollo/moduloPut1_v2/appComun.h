typedef int (zHttpAppProcesoCB_t)(
            MHD_Connection_t *connection
          , void *dataCls);

typedef int (zHttpAppEndCB_t)(
            void *dataCls);
/*
typedef int (zHttpAppInitCB_t)(
             modPut0DataCB_t  *appDataCB
           , zHttpAppProcesoCB_t *appProcesoCB
           , zHttpAppEndCB_t *appEndCB
           , void **dataCls);
*/
