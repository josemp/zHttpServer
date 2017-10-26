
int appDataCB(void *dataCls, char *key, char *value);

int appInitCB(void **dataCls);

int appEndCB(void *dataCls);

int appProcesoCB(MHD_Connection_t *connection,void *dataCls);

