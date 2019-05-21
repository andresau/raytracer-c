/* ************ FIGURAS.H *************************
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
 #include <math.h>
 #include <imagenes.h>
 
 #define FIGURAS
 #define NORMA(vec) sqrtl(vec.dire.x * vec.dire.x + \
						  vec.dire.y * vec.dire.y + \
						  vec.dire.z * vec.dire.z)
						  
#define PRODUCTO_PUNTO(v1, v2) v1.dire.x * v2.dire.x \
		+ v1.dire.y * v2.dire.y + v1.dire.z * v2.dire.z

#define PRODUCTO_CRUZ(v, w)  (Vector) {{ \
								v.dire.y * w.dire.z - w.dire.y * v.dire.z, \
								w.dire.x * v.dire.z - v.dire.x * w.dire.z, \
								v.dire.x * w.dire.y - w.dire.x * v.dire.y \
							}}
		
#define CREAR_VECTOR(origen,destino) (Vector) {{ \
												 destino.x - origen.x, \
												 destino.y - origen.y, \
												 destino.z - origen.z \
											   }}
#define MULTIPLICA(vector,escalar) (Vector) {{ \
											  vector.dire.x * escalar, \
											  vector.dire.y * escalar, \
											  vector.dire.z * escalar, \
											}}
#define SUMA_VECTOR(v1,v2) (Vector) {{ \
										v1.dire.x + v2.dire.x, \
										v1.dire.y + v2.dire.y, \
 										v1.dire.z + v2.dire.z  \
								     }}
#define RESTA_VECTOR(v1,v2) (Vector) {{ \
										v1.dire.x - v2.dire.x, \
										v1.dire.y - v2.dire.y, \
 										v1.dire.z - v2.dire.z  \
								     }}
typedef struct _punto{
	long double x,y,z;
} Punto; 
typedef struct _vector{
	Punto dire;
} Vector;
typedef struct _rayo{
	Vector vector;
	Punto ancla;
} Rayo;
typedef struct _plano{
	Vector ABC;
	long double D;
	int dire;
} Plano;
typedef struct _esfera{
	Punto centro;
	long double radio,radio2;
	Color color;
	long double Kd;
	long double Ka;
	long double Ks;
	long double Kn;
	long double Kt;
	long double o1,o2,o3;
	int cortes;
	int *planos_corte;
	int textura,calado,bump;
	long double maxbump;
	long double tu,tv;
	Vector G,Norte,Correcto,U,V;
	long double DCorrecto, Dnorte;
} Esfera;
typedef struct _cilindro{
	Rayo rayo;
	long double h1,h2;
	long double radio,radio2;
	Color color;
	long double Kd;
	long double Ka;
	long double Ks;
	long double Kn;
	long double Kt;
	long double o1,o2,o3;
	long double d;
	int cortes;
	int *planos_corte;
	int textura,calado,bump;
	long double maxbump;
	long double tu,tv;
	Vector G,Correcto,U,V;
	long double D;
} Cilindro;
typedef struct _cono{
	Rayo rayo;
	long double h1,h2;
	Color color;
	long double Kd;
	long double Ka;
	long double Ks;
	long double Kn;
	long double Kt;
	long double o1,o2,o3;
	long double d;
	long double uv;
	int cortes;
	int *planos_corte;
} Cono;
typedef struct _poligono{
	int numPuntos;
	Punto *puntos;
	Color color;
	long double Kd;
	long double Ka;
	long double Ks;
	long double Kn;
	long double Kt;
	long double o1,o2,o3;
	long double D;
	Vector Normal,U,V;
	long double texD,texH;
	int aplastado;
	int cortes;
	int *planos_corte;
	int textura;
	int calado;
	int bump;
	long double maxBump;
	long double tu,tv;
	int disco;
	Punto centro;
	long double radio;
} Poligono;
typedef struct _cuadratica{
	long double A,B,C,D,E,F,G,H,J,K;
	Color color;
	long double Kd;
	long double Ka;
	long double Ks;
	long double Kn;
	long double Kt;
	long double o1,o2,o3;
	int cortes;
	int *planos_corte;
} Cuadratica;
typedef struct _luz{
	Punto posicion;
	long double c1;
	long double c2;
	long double c3;
	long double Intensidad;
	Color color;
} Luz;
typedef struct _escena{
	int numEsferas;
	Esfera *Esferas;
	int numLuces;
	Luz *luces;
	int numPoligonos;
	Poligono *Poligonos;
	int numCilindros;
	Cilindro *Cilindros;
	int numConos;
	Cono *Conos;
	Cuadratica *Cuadraticas;
	int numCuadraticas;
	int numPlanos;
	int nivelEspejo;
	int nivelAntialiasing;
	int nivelTransparencia;
	
	int numTexturas;
	Imagen *Texturas;
	Plano *Planos;
	
	int resX;
	int resY;
	long double Xmin, Xmax; 
	long double Ymin, Ymax;
	long double Ia;
	Punto Ojo;
	Color Background;
} Escena;
//typedef Rayo Luz;
typedef struct _objeto{
	void *obj;
	int tipo;
	Vector (*Normal)(void *,Punto);
	long double Kd, Ka, Ks, Kn, Kt;
	long double o1,o2,o3;
	long double d;
	int textura;
	int bump;
	long double maxBump;
	Color color;
} Objeto;

long double Normalizar(Vector *vec);
Vector NormalEsfera(void *esf,Punto inter);
Vector NormalCilindro(void *cil,Punto inter);
Vector NormalPoligono(void *esf,Punto inter);
Vector NormalCono(void *esf,Punto inter);
Vector NormalCuadratica(void *cua,Punto inter);
