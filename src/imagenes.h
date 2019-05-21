/* ************ IMAGENES.H *************************
 * 
 *   Proyecto 2: Ray Tracing Basico
 * 
 *   Prof.: 
 * 	 Francisco J. Torres Rojas PhD
 * 
 *   Autor:
 *   Andres Aguilar Umana
 *   200928927
 * 
 *   1 de Abril 2012
 * 
 ****************************************************/
#include <stdlib.h>
#include <stdio.h>



         
#ifndef COLOR 
	typedef struct _color{
		float a,r,g,b;
	} Color;
	typedef struct _imagen{
		 Color **ima;
		 int resX;
		 int resY;
		 long double mosaicoX,mosaicoY;
	 } Imagen;
	#define COLOR
	#define SWAP(x)  ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
    #define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))    
#endif


void liberarImagen(Imagen ima);
void crearImagen(Imagen *ima,int resX, int resY);
void guardarImagen(Imagen ima, char *arch);
Imagen leerTextura(char *archivo);
