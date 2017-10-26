Un modulo zHTTP es un interfaz que utiliza el servidor spaServer de modo que solamente tenemos que registrar una función de proceso.

De este modo para nuestro servicio HTTP , simplemente tenemos de alguna manera que tomar los datos y responder con algo.

La idea consiste en tener un evento de proceso

eventProcess(conexion,data)
{
procesamos nuestros datos aqui, y enviamos la respuesta.
}

para poder tener nuestros datos, necesitamos de alguna forma pasar una estructura al modulo que 'rellene' los datos.
Esto sera otro evento evidentemente y es lo que realmente dará la especialidad del modulo.
por ejemplo si el relleno de datos es con parejas clave-valor
eventData(data,clave,valor)
{
 rellena datos con clave-valor
}


De alguna forma se tendrá que iniciar todo, una forma es con un evento de inicio, donde registaremos los eventos anteriores y crearemos la closure
Nota. Si tenemos registrados los callback, no necesitaremos los eventos
eventInit( ... event **eventProcess, event **eventData, void **closure)
{
*closure=malloc closure
*eventProcess=eventProcess
*eventData=eventData
}


Como es posible que ni siquiera entre en el proceso, para liberar la estructura necesitaremos un evente
eventEnd(data)
{
 Libera data
}

-- No entro por ahora en temas de sesion.

¿Como diseño entonces el módulo?
Tenemos el spa server
El modulo necesita
Un evento init que además de los datos del spaServerInit, nos proporcione los callback de la aplicacion que va a procesar los datos

Suponemos que url,method y version, no son necesarios para el modulo, porque son modulos distintos, ¿Los necesitará para pasarlos al app ?
Los Callbacks del modulo, es posible que esten registrados y no haya que pasarlos
typedef int (conexionInfoInitCB_t)(
             struct MHD_Connection *connection
           , void **moduloCls // closure necearia para el propio modulo
           , callBack appInit // ??? Este puede ser llamado por otro
           , callBack appEnd  // ??? Este puede ser llamado por otro
           , callBack appData
           , callBack appProceso
           , void *appCls     // ??? closure del app
          );



¿Como hago el diseño global?
Necesitamos un Evento de inicio para el spaServer que con los datos de la conexion, sepa el modulo que vamos a tratar y la aplicación que va a procesar.

Suponemos el spaServer lanzado y un RouterEventoInicio y una:
TABLA ROUTER, poco a poco la iremos rellenando y servirá para saber el modulo y programa que vamos a tratar.

TablaRouter.Url
TablaRouter.tipo (GET, PUT ...)
TablaRouter.ContentTipe
TablaRouter.Modulo
TablaRouter.App

TABLA MODULOS
TablaModulos.Name
TablaModulos.callBackInit
TablaModulos.callBackProceso //standard spaServer
TablaModulos.callBackProcesoEnd //standar spaServer
TablaModulos.callBackEnd        //standar spaServer


TABLA APP
tablaApp.Name
tablaApp.callbackInit
tablaApp.callBackData
tablaApp.callBackProceso
tablaApp.callBackEnd



PROCESO DEL ROUTER
Supongamos que el spaServer está lanzado hacia el proceso init del router
conexionInfoInitCB_t
int routerInitConexion( 
             struct MHD_Connection *connection
           , const char *url
           , const char *method
           , const char *version
           , conexionInfoProcesoCB_t **procesoCB
           , conexionInfoProcesoEndCB_t **procesoEndCB
           , conexionInfoEndCB_t **endCB
           , void **appCls  )
{
  chequea la TABLA ROUTER LA TABLA APP y la TABLA MODULOS
  Si no hay concordancia, envia un error.

  tenemos entonces:
  numRuta numModulo, numApp
  void *appCls;
  ejecutamos tablaApp[i].callbackInit(&appCls);
  ejecutamos tablaModulo[i].callBackInit(connection,
                                         moduloCls,
                                         tablaApp[i].callBackProceso,
                                         tablaApp[i].callBackData,
                                         tablaApp[i].callBackEnd,
                                         appCls);
 procesoCB =tablaModulo[i].callBackProceso
 procesoEndCB=tablaModulo[i].callBackProceosEnd
 endCB=tablaModulo[i].callBackEnd;
 // appCls ya está asignado
}


¿COMO ESCRIBIMOS UN MODULO?

Un modulo constará de lo siguiente
Un typedef de callBackData
un typedef de callBackProceso
un typedef de callBackEnd


