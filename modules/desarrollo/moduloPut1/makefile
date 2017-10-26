# l programa main sin el .c, estea el output del programa
PROGRAMA_MAIN    := test
# Otras fuentes que compilaran a objetos
OTRAS_FUENTES  := moduloPut0.c app.c ../../spaServer/spaServer.c  ../../spaServer/spaServerResponse.c
# dependencias de los objetos (de todos)
INCLUDES :=  moduloPut0.h
# directorios e includes externos
OUTHER_DIR_INCLUDE := ../../spaServer
OUTHER_DIR_LIB:= 
# librerias utilizadas
OUTHER_LIBS:= -pthread  /usr/local/lib/libmicrohttpd.a
# ----------- Variables Intermedias --------
CFLAGS = $(OUTHER_DIR_INCLUDE:%=-I%)
LDFLAGS = $(OUTHER_DIR_LIB:%=-L%)
LDLIBS =     $(OUTHER_LIBS)
OBJETOS      := $(PROGRAMA_MAIN:%=%.o) $(OTRAS_FUENTES:%.c=%.o)
# ------------ RULES ------------------
all: $(PROGRAMA_MAIN)
$(OBJETOS): $(PROGRAMA_MAIN:%=%.c) $(OTRAS_FUENTES) $(INCLUDES)
$(PROGRAMA_MAIN): $(OBJETOS)
clean:
	- rm -f $(OBJETOS) $(PROGRAM_OUTPUT)

