/* ************ RAYTRACER.H *************************
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
 
#include <imagenes.h> 
#include <figuras.h>



#define PLOT(x,y,color,imagen) imagen.ima[x][y] = color

	#define MIN(x,y) (x < y? x : y)
	#define MAX(x,y) (x > y? x : y)

#ifndef EPSILON
#define EPSILON 0.0000005
#define PI 3.14159265


typedef struct _intersection{
	long double t;
	Punto Pint;
	Objeto objeto;
	long double d;
} Interseccion;

#endif

Color ProcesarPixel(long double i, long double j, long double di0, long double dj0,
        long double di1, long double dj1,Escena *escena,Rayo ray, int ncolor,Color col, Color col2,int nivel);
Color RayoSubpixel(long double i, long double j, long double di, long double dj,Escena *escena, Rayo ray);
void iniciarRayTracing(char*,int);
Color De_Que_Color(Rayo,Escena *,int);
Interseccion *Primera_Interseccion(Rayo ray,Escena *, int *);
void posicionTexturaPoligono(Poligono poli, int tex, Punto pinter,Escena *escena, int *i, int *j);
void posicionTexturaEsfera(Esfera esf, int tex ,Punto pinter,Escena *escena, int *i, int *j, Vector Normal);
void posicionTexturaCilindro(Cilindro cil, int tex ,Punto pinter,Escena *escena, int *i, int *j, Vector Normal);
Punto Bump(int text, int i, int j,Escena *escena, Punto pint, 
           long double maxBump,Vector *normal,Vector U, Vector V,long double tu, long double tv);
Color Textura(int text, int i, int j,Escena *escena);
int Calado(int text, int i, int j,Escena *escena);
