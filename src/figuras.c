/* ************ FIGURAS.C *************************
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
 #include <figuras.h>
 

long double Normalizar(Vector *vec){
	long double norma;
	 norma = NORMA((*vec)); //(*vec) es como si no fuera ptr
	 vec->dire.x = vec->dire.x / norma;
	 vec->dire.y = vec->dire.y / norma;
	 vec->dire.z = vec->dire.z / norma;
	 return norma;
}
Vector NormalEsfera(void *esfera,Punto inter){
	Esfera *esf;
	Vector res;
	esf = (Esfera *)((Objeto *)esfera)->obj;
	res.dire.x = (inter.x - esf->centro.x)/esf->radio;
	res.dire.y = (inter.y - esf->centro.y)/esf->radio;
	res.dire.z = (inter.z - esf->centro.z)/esf->radio;
	return res;
}
Vector NormalCilindro(void *cilindro,Punto inter){
	Cilindro *cil;
	Vector res;
	cil = (Cilindro *)((Objeto *)cilindro)->obj;
	long double d = ((Objeto *)cilindro)->d;
	                
	res.dire.x = (inter.x - (cil->rayo.ancla.x + d * cil->rayo.vector.dire.x))/cil->radio;
	res.dire.y = (inter.y - (cil->rayo.ancla.y + d * cil->rayo.vector.dire.y))/cil->radio;
	res.dire.z = (inter.z - (cil->rayo.ancla.z + d * cil->rayo.vector.dire.z))/cil->radio;
	return res;
}
Vector NormalCono(void *cono, Punto inter){
	Cono *con;
	Vector res;
	con = (Cono *)((Objeto *)cono)->obj;
	long double d = ((Objeto *)cono)->d;
	long double tx,ty,tz,XQ,YQ,ZQ;
	
	XQ = con->rayo.vector.dire.x;
	YQ = con->rayo.vector.dire.y;
	ZQ = con->rayo.vector.dire.z;
	
	tx = con->rayo.ancla.x + d * XQ - inter.x;
	ty = con->rayo.ancla.y + d * YQ - inter.y;
	tz = con->rayo.ancla.z + d * ZQ - inter.z;
	                
	res.dire.x = 2 * (tx * (XQ * XQ - 1) + ty * XQ * YQ + tz * XQ*ZQ - con->uv*d*XQ);
	
	res.dire.y = 2*(tx*XQ*YQ + ty*(YQ*YQ-1) + tz*YQ*ZQ - con->uv*d*YQ);
	res.dire.z = 2*(tx*XQ*ZQ + ty*YQ*ZQ + tz*(ZQ*ZQ-1) - con->uv*d*ZQ);
	Normalizar(&res);
	return res;
}

Vector NormalPoligono(void *poligono,Punto inter){
	return ((Poligono *)((Objeto *)poligono)->obj)->Normal;
}
Vector NormalCuadratica(void *cuadratica,Punto inter){
	Vector vec;
	Cuadratica *cua = ((Cuadratica *)((Objeto *)cuadratica)->obj);
	vec.dire.x =  (cua->A * inter.x 
						+ cua->D * inter.y
						+ cua->F * inter.z
						+ cua->G);
	vec.dire.y =  (cua->B * inter.y
						+ cua->D * inter.x
						+ cua->E * inter.z 
						+ cua->H);
	vec.dire.z =  (cua->C * inter.z
						+ cua->E * inter.y
						+ cua->F * inter.x
						+ cua->J);
	Normalizar(&vec);
	
	return vec;
}

