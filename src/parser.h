/* ************ PARSER.H *************************
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
 *   2 de Abril 2012
 * 
 ****************************************************/
 #include <string.h>
 #ifndef FIGURAS
	#include <figuras.h>
 #endif
 #define VALIDA(escena,token,file) \
          if (!esperaToken(token,file)  || !esperaToken(IGUAL,file)) \
			{ \
			    liberarEscena(escena);\
				fclose(file); \
				return NULL; \
			}
 #ifndef TOKENS
	#define MIN(x,y) (x < y? x : y)
	#define MAX(x,y) (x > y? x : y)
	int leeespacios;
	#define ESCENA 0
	#define ESFERA 1
	#define CORCHETE_INI 2
	#define CORCHETE_FIN 3
	#define PUNTO_COMA 4
	#define X 5
	#define Y 6
	#define Z 7
	#define RADIO 8
	#define COLOR_T 9
	#define IGUAL 10
	#define COMA 11
	#define RESX 12
	#define RESY 13
	#define LUZAMBIENTE 14
	#define C1 15
	#define C2 16
	#define C3 17
	#define LUZ 18
	#define INTENSIDAD 19
	#define KMATERIAL 20
	#define KA 21
	#define OJOX 22
	#define OJOY 23
	#define OJOZ 24
	#define BACKGROUND 25
	#define XMAX 26
	#define YMAX 27
	#define XMIN 28
	#define YMIN 29
	#define KS 30
	#define KN 31
	#define POLIGONO 32
	#define VERTICE 33
	#define CILINDRO 34
	#define ANCLA 35
	#define DIRECCION 36
	#define H1 37
	#define H2 38
	#define ESF_CIL_ESF 39
	#define CONO 40
	#define UTOKEN 41
	#define VTOKEN 42
	#define ABCDEFGHJK 43
	#define PLANO 44
	#define CORTES 45
	#define CUADRATICA 46
	#define ESPEJO 47
	#define O1 48
	#define O2 49
	#define ANTIALIASING 50
	#define TRANSPARENCIA 51
	#define O3 52
	#define KT 53
	#define TEXTURA 54
	#define CALADO 55
	#define BUMP 56
	#define TU 57
	#define TV 58
	#define DISCO 59
	#define NORTE 60
	#define TOKENS
 #endif
void aplastarPoligonos(Escena *escena);
Escena *parseEscena(char *arch);
void liberarEscena(Escena *escena);
