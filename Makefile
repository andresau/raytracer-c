# ************ MAKEFILE ******************************
# * 
# *   Proyecto 2: Raytracer Basico
# * 
# *   Prof.: 
# * 	 Francisco J. Torres Rojas PhD
# * 
# *   Autor:
# *   Andres Aguilar Umana
# *   200928927
# * 
# *   14 de Abril 2012
# * 
# ****************************************************/
 
CC     = gcc
CFLAGS =  
LIBS   = -I. -lm
WDEBUG  = -Wall -g -fopenmp

all: clean fuente

fuente:
	cd src && make
    
clean:
	rm -f bin/*.o bin/proyecto3
