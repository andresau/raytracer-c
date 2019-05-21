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

OBJECTS = imagenes.o figuras.o parser.o raytracer.o proyecto3.o

all: imagenes.o figuras.o parser.o raytracer.o proyecto3 fin

imagenes.o: 
	$(CC) $(WDEBUG) $(LIBS) -c imagenes.c -o $@

figuras.o: 
	$(CC) $(WDEBUG) $(LIBS) -c figuras.c -o $@
	 
parser.o: 
	$(CC) $(WDEBUG) $(LIBS) -c parser.c -o $@
	 
raytracer.o: 
	$(CC) $(WDEBUG) $(LIBS) -c raytracer.c -o $@
	 
proyecto3: $(OBJECTS)
	$(CC) $(WDEBUG)  $(OBJECTS) -o $@ $(LIBS)

proyecto3.o: 
	$(CC) $(WDEBUG) $(LIBS) $(CFLAGS) -c proyecto3.c -o $@

    
fin:
	cp *.o proyecto3 ../bin && rm -f *.o proyecto3
