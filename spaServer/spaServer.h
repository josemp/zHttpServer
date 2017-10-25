/* 
 * spaServer.h
 * 
 * Copyright 2017 J. M. PARRA <josem.parra@gmail.com>
 * 
 */

/**
 * spaServer es un interfaz para lanzar un daemon libmicroHttpd
 * 
 * Resumen de funciones de la interfaz spaServer
 * 
 * 		spaServerStart - Inicia el daemon
 * 		spaServerStop   - Para y destruye el daemon 
 * 
 * Eventos lanzados por la interfaz spaServer
 * 
 * 	conexionInfoInitCB_t       - LLegada de una conexion HTTP, este evento 
 *                               se registra en spaServerStart metodo.
 *  conexionInfoProcesoCB_t    - Llegan datos HTTP, este proceso se registra en el evento conexionInfoInitCB
 *  conexionInfoProcesoEndCB_t - Fin  de datos HTTP, este proceso se registra en el evento conexionInfoInitCB
 *  conexionInfoEndCB_t        - Fin de conexion,    este proceso se registra en el evento conexionInfoInitCB
 */ 

/**
 * MAX_HEADER_VALUE_SIZE es la maxima longitud del valor de un dato de cabecera
 * 
 */ 
#define MAX_HEADER_VALUE_SIZE 200


/** 
 * typedef creado solamente por comodidad 
 * struct MHD_Connection es el Handler de libmicrohttp de una conexion
 * puede utilizarse para utilizar alguna funcion de libmicrohttp sobre la conexion y datos HTTP
 * Por ejemplo existe una utilidad para leer los datos POST.
 */
typedef struct MHD_Connection MHD_Connection_t;

/*
 *    TIPOS DE EVENTOS LANZADOS POR LA INTERFAZ spaServer 
 * 
 */

/**
 * typedef function conexionInfoProcesoCB
 *
 * @brief formato del spaServer evento de llegada de datos HTTP, se registra en el evento conexionInfoInitCB
 *
 * spaServer lanza este evento cuando llegan datos HTTP
 * Este evento es registrado en spaServer cuando se inicia una conexion HTTP
 *     es decir cuando llega el evento conexionInfoInitCB_t
 * 
 * NOTA IMPORTANTE : En este evento no se puede enviar una respuesta
 * 
 * @param[in]  {MHD_Connection_t *} connection 	- libmicrohttpd handler conexion
 * @param[in]       {char *}        data 	    - Buffer de datos
 * @param[in][out]  {size_t *}      dataLen     - [in]  Len de datos pointer
 * 												  [out]	Len de datos sin procesar
 *                                                      Actualmente siempre el out es 0
 *                                                      Los valores !=0 no se comportan adecuadamente
 * @param[in]       {void   *}      appCls      - Closure iniciada en conexionInfoInitCB
 *                                      
 * @return          {int}                       - MHD_YES  Si se desea que continue el proceso
 * 												- MHD_NO   Si se desea que no continue el proceso  
 */

typedef int (conexionInfoProcesoCB_t)(
             MHD_Connection_t *connection
           , const char *data, size_t *dataLen
           , void *appCls);

/**
 * typedef function conexionInfoProcesoEndCB
 *
 * @brief formato del spaServer evento de fin de datos HTTP, se registra en el evento conexionInfoInitCB
 *
 * spaServer lanza este evento cuando han llegado todos los datos hTTP
 * Este evento es registrado en spaServer cuando se inicia una conexion HTTP
 *     es decir cuando llega el evento conexionInfoInitCB_t
 * 
 * NOTA IMPORTANTE : En este evento SI se puede enviar una respuesta. 
 *                   Debe de terminarse con una respuesta o con MHD_NO
 * 
 * @param[in]  {MHD_Connection_t *} connection 	- libmicrohttpd handler conexion
 *
 * @return          {int}                       - MHD_YES  Si se desea que continue el proceso
 * 												- MHD_NO   Si se desea que no continue el proceso  
 */

typedef int (conexionInfoProcesoEndCB_t)(
             MHD_Connection_t *connection           
           , void *appCls);

/**
 * typedef function conexionInfoEndCB
 *
 * @brief formato del spaServer evento de fin de conexion HTTP, se registra en el evento conexionInfoInitCB
 *
 * spaServer lanza este evento cuando termina una conexion HTTP (no conexion TCP)
 * Este evento es registrado en spaServer cuando se inicia una conexion HTTP
 *     es decir cuando llega el evento conexionInfoInitCB_t
 * 
 * NOTA IMPORTANTE : En este evento NO se puede enviar una respuesta
 *                   No hay nada de spaServer que se tenga que liberar
 *                   Seguramente es el lugar para liberar la closure appCls si procede
 * 
 *
 * @return          {int}                       - MHD_YES  Si se desea que continue el proceso
 * 												- MHD_NO   Si se desea que no continue el proceso  
 */
           
typedef int (conexionInfoEndCB_t)(
             void *appCls);

/**
 * typedef function conexionInfoInitCB
 *
 * @brief formato del spaServer evento de llegada de una conexion HTTP, se registra en el metodo spaServerStart
 *
 * spaServer lanza este evento cuando llega una conexion HTTP, están disponibles los datos del Header.
 * Este evento es registrado en spaServer cuando se lanza el Servicio spaServer con el metodo spaServerStart
 * 
 * NOTA. Si que se puede enviar una respuesta en este vento, pero no llegaran los eventos Proceso y ProcesoEnd
 *       
 * 
 * @param[in]  {MHD_Connection_t *} connection 	 - libmicrohttpd handler conexion
 * @param[in]       {char *}        url 	     - url HTTP
 * @param[in]       {char *}        method 	     - method HTTP (GET, POST, PUT ...)
 * @param[in]       {char *}        version	     - version HTTP
 * 
 * @param[out]      {conexionInfoProcesoCB_t **}     procesoCB    - Set Event callback de datos HTTP
 * @param[out]      {conexionInfoProcesoEndCB_t **}  procesoEndCB - Set Event callback de fin de datos HTTP
 * @param[out]      {conexionInfoEndCB_t **}         endCB - Set Event callback de fin de conexion HTTP

 * @param[out]      {void   **}      appCls      - Set Closure 
 *                                      
 * @return          {int}                       - MHD_YES  Si se desea que continue el proceso
 *                                                        
 * 												- MHD_NO   Si se desea que no continue el proceso  
 * 														   la conexion socket se rompe.  
 */

typedef int (conexionInfoInitCB_t)(
             struct MHD_Connection *connection                                           
           , const char *url
           , const char *method
           , const char *version
           , conexionInfoProcesoCB_t **procesoCB
           , conexionInfoProcesoEndCB_t **procesoEndCB
           , conexionInfoEndCB_t **endCB
           , void **appCls  );



/**
 * 
 * @brief struct handler (closure) for spaServer http service
 *
 * Esta estrcutura es creada  por spaServer por la llamada a la funcion spaServerStart
 * Es liberada por el propio spaServer cuando se llama a spaServerStop
 * 
 */

typedef struct
{
	
  struct MHD_Daemon * daemon;    /**< MHD handler for daemon */
  conexionInfoInitCB_t *initCB;  /**< CallBack for Conection Event*/
  int puerto;                    /**< Port server conection  */
  
} spaServerInfo_t;


/**
 * 
 * @brief struct handler for spaServer http conection
 *
 * - Esta estrcutura es creada a la llegada de la conexion por el microhttpd
 *   evento de conexion
 * - Es liberada cuandollega el evento microhttpd de fin de conexion
 */

typedef struct
{
  conexionInfoProcesoCB_t *procesoCB; /**< CallBack for arrival of data Event*/
  conexionInfoProcesoEndCB_t *procesoEndCB;  /**< CallBack for end process Event*/
  conexionInfoEndCB_t *endCB;         /**< CallBack for end Conexion*/
  void *appCls;                       /**< Closure of process */ 
 
} spaServerConexionInfo_t;


/**
 * function spaServerStart
 *
 * @brief start spaServer HTTP (based in libMicroHttpd)
 *
 * spaServerStar launch a deamon (thread) tcp server
 * 
 * USING :  spaServerInfo_t *spaServerInfo =spaServerStart (80
 *                                           , conexionInfoInit);
 * @param[in]  {int} port	 	- serve port
 * @param[in]  {conexionInfoInitCB_t *} conexionInfoInitCB 	- CallBack for
 *                                      conexion event.
 * @return     {spaServerInfo_t *}      Handler for spaServerdaemon 
 */
 
spaServerInfo_t *
spaServerStart (int port
             , conexionInfoInitCB_t *conexionInfoInitCB);

/**
 * function spaServerStop
 *
 * @brief stop spaServer HTTP (based in libMicroHttpd)
 *
 * spaServerStop libera eldeamon y la structura spaServerInfo
 * 
 * USING :  spaServerStop (spaServerInfo);
 * 
 * @return     void
 */

void spaServerStop (spaServerInfo_t *spaServerInfo);

/**
 * function utility spaServerGetHeaderKey
 *
 * @brief utilidad para tomar los valores de campos de cabecera HTTP
 *
 * spaServerStar launch a deamon (thread) tcp server
 * 
 * USING :  spaServerInfo_t *spaServerInfo =spaServerStart (80
 *                                           , conexionInfoInit);
 * @param[in]  {int} port	 	- serve port
 * @param[in]  {conexionInfoInitCB_t *} conexionInfoInitCB 	- CallBack for
 *                                      conexion event.
 * @return     {spaServerInfo_t *}      Handler for spaServerdaemon 
 */

char *spaServerGetHeaderKey(MHD_Connection_t *conexion, char *key,char *value);
