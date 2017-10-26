

typedef struct
{
 char user[100];
 char password[100];
} appData_t;



int loginInitCB(modPut0DataCB_t **appDataCB
        , zHttpAppProcesoCB_t **appProcesoCB
        , zHttpAppEndCB_t **appEndCB
        , void **dataCls);

int loginProcesoCB(MHD_Connection_t *connection,void *dataCls);
