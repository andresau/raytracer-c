/* ************ RAYTRACER.C *************************
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
#include <raytracer.h>
#include <parser.h>
#include <math.h>
#include <omp.h>

int hola;
int colocar(Interseccion *v, int b, int t)
{
    int i;
    int pivote;
	Interseccion valor_pivote;
    Interseccion temp;
 
    pivote = b;
    valor_pivote = v[pivote];
    for (i=b+1; i<=t; i++){
        if (v[i].t < valor_pivote.t){
                 pivote++;    
                temp=v[i];
                v[i]=v[pivote];
                v[pivote]=temp;
 
        }
    }
    temp=v[b];
    v[b]=v[pivote];
    v[pivote]=temp;
    return pivote;
} 

void Quicksort(Interseccion* v, int b, int t)
{
     int pivote;
     if(b < t){
        pivote=colocar(v, b, t);
        Quicksort(v, b, pivote-1);
        Quicksort(v, pivote+1, t);
     }  
}


void iniciarRayTracing(char* file, int n){
	
	//n = 1; file = "escena5.rt";
	Imagen imagen;
	int i,j;
	//long double Xw, Yw;
	Rayo ray;
	unsigned long total;
	int porcentaje;
	int actual;
	Color color;
	int tid,nthreads,chunk;
	Escena *escena;
	char dele;
	
	omp_set_num_threads(n);
	
	chunk = 10;
	
	dele = -1;	
	
	//Paralelizamos con OpenMP (utilizamos los cores del CPU)
	#pragma omp parallel shared(imagen,escena,nthreads,dele,total,porcentaje,actual) private(i,j,ray,color,tid)
	{
		
		
		
		 tid = omp_get_thread_num();
		 	  
		 if (tid == 0){	
		     escena=NULL;
		     dele=0;
			nthreads = omp_get_num_threads();
			escena = parseEscena(file);
			
			printf("Resolucion: %d %d\n",escena->resX,escena->resY);
			printf("Ventana: %0.2Lf %0.2Lf %0.2LF %0.2LF \n",escena->Xmin,escena->Xmax,escena->Ymin,escena->Ymax);
			printf("Antialiasing: %d\n",escena->nivelAntialiasing);
			printf("Transparencia: %d\n",escena->nivelTransparencia);
			printf("Texturas: %d\n",escena->numTexturas);
			printf("Espejo: %d\n",escena->nivelEspejo);
			printf("Luces: %d\n",escena->numLuces);
			printf("Esferas: %d\n",escena->numEsferas);
			printf("Poligonos: %d\n",escena->numPoligonos);
			printf("Cilindros: %d\n",escena->numCilindros);
			printf("Conos: %d\n",escena->numConos);
			printf("Cuadraticas: %d\n",escena->numCuadraticas);
			
			
			printf("Background: %f %f %f %f\n",escena->Background.a,escena->Background.r,escena->Background.g,escena->Background.b);
			printf("Numero de Hilos = %d\n", nthreads);
			aplastarPoligonos(escena);
			
			crearImagen(&imagen,escena->resX,escena->resY);
		
			chunk = roundl(escena->resX / (nthreads*10));
			total = escena->resX * escena->resY;
			actual = 0;
			
			porcentaje=1;
			dele = 1;
			
		}else{
			
			while(!dele){}
			
		}
	ray.ancla = escena->Ojo;
	color = escena->Background;
	printf("Hilo %d iniciando...%d chunk: %d\n",tid,dele,chunk);
	
	
	#pragma omp for schedule(dynamic,chunk)	
		for (i = 0; i < escena->resX; i++){
			for (j = 0; j < escena->resY; j++){
				color = ProcesarPixel(i,j,0.0,0.0,1.0,1.0,escena,ray,0,color,color,escena->nivelAntialiasing);
				PLOT(i,j,color,imagen);
				actual++;
				if (actual == total/100){
					
					if (porcentaje % 4 == 0)
						printf("Completado: \\ %d",++porcentaje);
					else if (porcentaje % 4 == 1)
						printf("Completado: | %d",++porcentaje);
					else if (porcentaje % 4 == 2)
						printf("Completado: / %d",++porcentaje);
					else
						printf("Completado: - %d",++porcentaje);
					printf("%%\n\033[F\033[J");
					actual = 0;
				}
			}
		}
	}
	printf("Completado: %d%% \n",100);
	
	liberarEscena(escena);
	guardarImagen(imagen,"ima.avs");
	liberarImagen(imagen);
}
Color ProcesarPixel(long double i, long double j, long double di0, long double dj0,
        long double di1, long double dj1,Escena *escena,Rayo ray, int ncolor,Color col, Color col2, int nivel){
	Color color[4];
	Color res;
	Color centro;
	int co;
	int entra = 0;
	long double diff =0;
	dj0+=0.05;
	di0+=0.05;
	dj1-=0.05;
	di1-=0.05;
	centro = RayoSubpixel(i,j,di1/2.0,dj1/2.0,escena,ray);
	if (escena->nivelAntialiasing != 0 && nivel > 0){
		if (ncolor == 1){
			color[0] = col;
			color[2] = col2;
		}else if (ncolor!=3)
			color[0] = RayoSubpixel(i,j,di0,dj0,escena,ray);
			
		if (ncolor == 2){
			color[1] = col;
			color[3] = col2;
		}else if (ncolor!=4)
			color[1] = RayoSubpixel(i,j,di1,dj0,escena,ray);
			
		if (ncolor == 3){
			color[2] = col;
			color[0] = col2;
		}else if (ncolor!=1)
			color[2] = RayoSubpixel(i,j,di1,dj1,escena,ray);
			
		if (ncolor == 4){
			color[3] = col;
			color[1] = col2;
		}else if (ncolor!=2)
			color[3] = RayoSubpixel(i,j,di0,dj1,escena,ray);
			
		res.r=0;
		res.g=0;
		res.b=0;
		for (co=0;co<4;co++){
			res.r += color[co].r;
			res.g += color[co].g;
			res.b += color[co].b;
		}
		res.r /= 4;
		res.g /= 4;
		res.b /= 4;
		entra = 0;
		for (co=0;co<4;co++){
			diff = sqrtl(
				(res.r - color[co].r)*(res.r - color[co].r)
				+(res.g - color[co].g)*(res.g - color[co].g)
				+(res.b - color[co].b)*(res.b - color[co].b)
			);
			if (diff >= 0.3){
				
				if (co == 0){
					color[0] = ProcesarPixel(i,j,0.0,0.0,di1/2.0,dj1/2.0,escena,ray,1,color[0],centro,nivel-1);
				}
				if (co == 1){
					color[1] = ProcesarPixel(i+di1/2.0,j,0.0,0.0,di1/2.0,dj1/2.0,escena,ray,2,color[1],centro,nivel-1);
				}
				if (co == 2)
					color[2] = ProcesarPixel(i+di1/2.0,j+dj1/2.0,0.0,0.0,di1/2.0,dj1/2.0,escena,ray,3,color[2],centro,nivel-1);
				if (co == 3)
					color[3] = ProcesarPixel(i,j+dj1/2.0,0.0,0.0,di1/2.0,dj1/2.0,escena,ray,4,color[3],centro,nivel-1);
				entra++;
			}
		}
		if (entra != 0){
			
			res.r=0;
			res.g=0;
			res.b=0;
			for (co=0;co<4;co++){
				res.r += color[co].r;
				res.g += color[co].g;
				res.b += color[co].b;
			}
			res.r /= 4;
			res.g /= 4;
			res.b /= 4;
		}

	}else
		res = centro;
	
	return res;
}

Color RayoSubpixel(long double i, long double j, long double di, long double dj,Escena *escena, Rayo ray){
	long double Xw,Yw;
	//printf("i %LF + %LF   j  %LF + %LF\n",i,di,j,dj);
	Xw = (((escena->Xmax - escena->Xmin)*(di+i))/escena->resX)+escena->Xmin;
	Yw = (((escena->Ymax - escena->Ymin)*(di+j))/escena->resY)+escena->Ymin;
	ray.vector.dire.x = Xw - escena->Ojo.x;
	ray.vector.dire.y = Yw - escena->Ojo.y;
	ray.vector.dire.z = -escena->Ojo.z;
	Normalizar(&(ray.vector));
	return De_Que_Color(ray,escena,escena->nivelEspejo);
}

Color De_Que_Color(Rayo rayo, Escena *escena, int nivelEspejo){
	Color *color,colorEspejo, colorRes;
	Interseccion *inter;
	Objeto Q;
	Vector N,L,V,R;
	Vector UU,VV;
	Luz luz;
	int k,i,som;
	int ii,jj;
	int num,num2;
	long double I,E,dt,Fatt,LxN,intensidad;
	Interseccion *inter2;
	Rayo ray;
	long double RxV;
	hola = 0;
	num = 0;
	num2 = 0;
	inter = Primera_Interseccion(rayo,escena,&num);
	color = malloc(num*sizeof(Color));
	
	i = 0;
	if (inter==NULL)
		return escena->Background;  //Si es NULL no hubo interseccion
	else
	{
		
		
		color[0].a = 1.0;
		i=-1;
		do{
			i++;
			Q = inter[i].objeto;
			
			N = Q.Normal(&Q,inter[i].Pint);
			
			Normalizar(&N);

			if (PRODUCTO_PUNTO(N,rayo.vector)>0.0){
				N.dire.x *= -1.0;
				N.dire.y *= -1.0;
				N.dire.z *= -1.0;
			}
			
			if (Q.textura != -1){
				if (Q.tipo == CILINDRO){
					posicionTexturaCilindro(
								*((Cilindro *)Q.obj),
								((Cilindro *)Q.obj)->textura,
								inter[i].Pint,
								escena, &ii, &jj,N);
					Q.color = Textura(((Cilindro *)Q.obj)->textura, ii, jj, escena);
				}else if (Q.tipo == ESFERA){
					posicionTexturaEsfera(
								*((Esfera *)Q.obj),
								((Esfera *)Q.obj)->textura,
								inter[i].Pint,
								escena, &ii, &jj,N);
					Q.color = Textura(((Esfera *)Q.obj)->textura, ii, jj, escena);
				}
			}
			
			//Bump Mapping
			if (Q.bump != -1){
				
				
				if (Q.tipo == POLIGONO){
				//	printf("Hago bump %LF %LF %LF --> ",N.dire.x,N.dire.y,N.dire.z);
					posicionTexturaPoligono(*((Poligono *)Q.obj),((Poligono *)Q.obj)->bump,inter[i].Pint,escena,&ii,&jj);
					inter[i].Pint = 
						Bump(((Poligono *)Q.obj)->bump, ii, jj, escena,
								inter[i].Pint,escena->Poligonos[i].maxBump,&N,((Poligono *)Q.obj)->U
								,((Poligono *)Q.obj)->V,((Poligono *)Q.obj)->tu,((Poligono *)Q.obj)->tv);
				}else
				if (Q.tipo == CILINDRO){
					UU = PRODUCTO_CRUZ(N,((Cilindro *)Q.obj)->rayo.vector);
					VV = ((Cilindro *)Q.obj)->rayo.vector;
					posicionTexturaCilindro(*((Cilindro *)Q.obj),
							((Cilindro *)Q.obj)->bump,inter[i].Pint,
								escena,&ii,&jj,N);
					inter[i].Pint = 
						Bump(((Cilindro *)Q.obj)->bump, ii, jj, escena,
								inter[i].Pint,escena->Poligonos[i].maxBump,&N,UU
								,VV,((Cilindro *)Q.obj)->tu,((Cilindro *)Q.obj)->tv);
				}else
				if (Q.tipo == ESFERA){
					UU = PRODUCTO_CRUZ(((Esfera *)Q.obj)->Norte,N);
					VV = PRODUCTO_CRUZ(UU,N);
					posicionTexturaEsfera(*((Esfera *)Q.obj),
								((Esfera *)Q.obj)->bump,
									inter[i].Pint,escena,&ii,&jj,N);
					inter[i].Pint = 
						Bump(((Esfera *)Q.obj)->bump, ii, jj, escena,
								inter[i].Pint,escena->Poligonos[i].maxBump,&N,UU
								,VV,((Esfera *)Q.obj)->tu,((Esfera *)Q.obj)->tv);
				}
				
			}
			
			I = 0.0;
			E = 0.0;
			for (k=0; k<escena->numLuces; k++)
			{
				luz = escena->luces[k];
				
				//Vector que senala desde la interseccion hacia la Luz K
				L = CREAR_VECTOR(inter[i].Pint,luz.posicion);
				
				
				dt = Normalizar(&L);
				
				//Rayo hacia la luz K para detectar obstaculos
				ray.ancla = inter[i].Pint;
				ray.vector = L;
				
				
				
				//Detectar obstaculos para Sombras
				inter2 = Primera_Interseccion(ray,escena,&num2);
				intensidad = luz.Intensidad;
				
				//Translucencia!! Aca bajamos la intensidad de esa luz, hasta llegar incluso a 0
					for (som=0;som<num2;som++){
							if (inter2[som].objeto.Kt>0.0)
								intensidad *= inter2[som].objeto.Kt;
							else{
								intensidad *= 0;
								break;
								}
						}
					
					
					//Ley de Lambert
					LxN = PRODUCTO_PUNTO(L,N);
					
					//Si el objeto esta de frente a la fuente de luz
					if (LxN > 0.0){
						
						//Calculamos la intensidad de la luz dependiendo de su distancia
						Fatt = MIN(1.0, 1 / (luz.c1 + luz.c2 * dt + luz.c3 * dt * dt));
						
						
						
						//Vector que senala desde la interseccion hacia el ojo
						V = CREAR_VECTOR(inter[i].Pint,escena->Ojo);
						//V = (Vector){{-rayo.vector.dire.x,-rayo.vector.dire.y,-rayo.vector.dire.z}};
						Normalizar(&V);
						
						Normalizar(&N);
						//Vector reflejo de L con respecto a N
						R = RESTA_VECTOR(MULTIPLICA(MULTIPLICA(N,2),LxN),L);
						Normalizar(&R);
						
						//Aumentamos la cantidad de luz difusa en el area
						I += (LxN * Q.Kd * Fatt * intensidad);
						
						RxV = (PRODUCTO_PUNTO(V,R));
						//Aumentamos la cantidad de reflexion especular E
						if (RxV > 0.0)
							E += (powl(RxV,Q.Kn) * Q.Ks * intensidad * Fatt);
						
					}
					
				//}
				free(inter2);
			}
			//Cubrimos las mentiras de E
			E = MIN(1.0, E);
			
			I += escena->Ia * Q.Ka;// * (1-Q.Kt);
			
			//Cubrimos las mentiras de I
			I = MIN(1.0,I);
			
			color[i].a = Q.color.a;
			color[i].r = I * Q.color.r;
			color[i].g = I * Q.color.g;
			color[i].b = I * Q.color.b;
			
			//Especular
			color[i].r = color[i].r + E * (1.0-color[i].r);
			color[i].g = color[i].g + E * (1.0-color[i].g);
			color[i].b = color[i].b + E * (1.0-color[i].b);
			//color = inter[0].color;
			if (nivelEspejo > 0){
				
				V = CREAR_VECTOR(inter->Pint,escena->Ojo);
				Normalizar(&V);
				LxN = PRODUCTO_PUNTO(V,N);
				R = RESTA_VECTOR(MULTIPLICA(MULTIPLICA(N,2),LxN),V);
				Normalizar(&R);
				
				ray.vector = R;
				colorEspejo = De_Que_Color(ray,escena,nivelEspejo-1);
				
				color[i].r = color[i].r * Q.o1 + colorEspejo.r * Q.o2;
				color[i].g = color[i].g * Q.o1 + colorEspejo.g * Q.o2;
				color[i].b = color[i].b * Q.o1 + colorEspejo.b * Q.o2;
			}
			
		}while (Q.Kt > 0.0 && escena->nivelTransparencia>0 && i+1<num);
		
		if (i<=0)
			i=-1;
		else{
			color[i].r += escena->Background.r * (inter[i].objeto.o3);
			color[i].g += escena->Background.g * (inter[i].objeto.o3);
			color[i].b += escena->Background.b * (inter[i].objeto.o3);
		}
		for (;i>=0;i--){
			if (i>0){
				color[i-1].r += color[i].r * (inter[i-1].objeto.o3);
				color[i-1].g += color[i].g * (inter[i-1].objeto.o3);
				color[i-1].b += color[i].b * (inter[i-1].objeto.o3);
			}else{
				color[0].r*=(inter[0].objeto.o3);
				color[0].g*=(inter[0].objeto.o3);
				color[0].b*=(inter[0].objeto.o3);
			}
		}
	}
	

	free(inter);
	
	colorRes = color[0];
	free(color);
	return colorRes;
}





Interseccion InterCuadratica(Rayo ray,long double *t,Cuadratica cuaTemp,Escena *escena){
	long double a,b,c;
	
	long double discr;
	long double temp1,temp2;
	long double sqr;
	long double t2;
	long double res;
	long double XD,YD,ZD;
	long double Xe,Ye,Ze;
	int i;
	char nohay;
	Interseccion inter;
	XD = ray.vector.dire.x;
	YD = ray.vector.dire.y;
	ZD = ray.vector.dire.z;
	Xe = ray.ancla.x;
	Ye = ray.ancla.y;
	Ze = ray.ancla.z;
	a = (cuaTemp.A * XD * XD) +  (cuaTemp.B * YD * YD) +  (cuaTemp.C * ZD * ZD) 
		+ 2 * (cuaTemp.D*XD*YD + cuaTemp.E*YD*ZD + cuaTemp.F*XD*ZD);
	b = 2 * (
		cuaTemp.J*ZD
		+Xe*cuaTemp.F*ZD
		+Ye*cuaTemp.E*ZD
		+Ze*cuaTemp.C*ZD
		+cuaTemp.H*YD
		+Ze*cuaTemp.E*YD
		+Xe*cuaTemp.D*YD
		+Ye*cuaTemp.B*YD
		+cuaTemp.G*XD
		+Ze*cuaTemp.F*XD
		+Ye*cuaTemp.D*XD
		+Xe*cuaTemp.A*XD
	);
	c = cuaTemp.K
		+2*(Ze*cuaTemp.J
			+Ye*cuaTemp.H
			+Xe*cuaTemp.G
			+Xe*Ze*cuaTemp.F
			+Ye*Ze*cuaTemp.E
			+Xe*Ye*cuaTemp.D
			)
		+Ze*Ze*cuaTemp.C
		+Ye*Ze*cuaTemp.B
		+Xe*Xe*cuaTemp.A;
	discr = (b * b) - (4 * c * a); 
	if (discr < 0){
		*t = INFINITY;
		inter.t = *t;
		return inter;
	}
	sqr = sqrtl(discr);
	temp1 = (-b + sqr)/(2.0 * a);
	temp2 = (-b - sqr)/(2.0 * a);
	inter.objeto.color = cuaTemp.color;
	if (temp1 < 0.0)
		temp1 = INFINITY;
	if (temp2 < 0.0)
		temp2 = INFINITY;
	
	*t = MIN(temp1,temp2);
	if (*t<EPSILON){
		*t = MAX(temp1,temp2);
		t2 = MIN(temp1,temp2);
	}else{
		t2 = MAX(temp1,temp2);
	}
	inter.t = *t;
	
	inter.Pint.x = ray.ancla.x + (*t) * ray.vector.dire.x;
	inter.Pint.y = ray.ancla.y + (*t) * ray.vector.dire.y;
	inter.Pint.z = ray.ancla.z + (*t) * ray.vector.dire.z;
	nohay = 0;
	if (cuaTemp.cortes>0){
		for (i=0;i<cuaTemp.cortes;i++){
			res = escena->Planos[cuaTemp.planos_corte[i]].ABC.dire.x * inter.Pint.x;
			res += escena->Planos[cuaTemp.planos_corte[i]].ABC.dire.y * inter.Pint.y;
			res += escena->Planos[cuaTemp.planos_corte[i]].ABC.dire.z * inter.Pint.z;
			res += escena->Planos[cuaTemp.planos_corte[i]].D;
			if ((escena->Planos[cuaTemp.planos_corte[i]].dire && res<0)
			     || (!escena->Planos[cuaTemp.planos_corte[i]].dire && res>0)){
							nohay = 1;
				
			}
		}
		if (nohay){
			*t = t2;
			inter.Pint.x = ray.ancla.x + (*t) * ray.vector.dire.x;
			inter.Pint.y = ray.ancla.y + (*t) * ray.vector.dire.y;
			inter.Pint.z = ray.ancla.z + (*t) * ray.vector.dire.z;
			inter.t = *t;
			for (i=0;i<cuaTemp.cortes;i++){
				res = escena->Planos[cuaTemp.planos_corte[i]].ABC.dire.x * inter.Pint.x;
				res += escena->Planos[cuaTemp.planos_corte[i]].ABC.dire.y * inter.Pint.y;
				res += escena->Planos[cuaTemp.planos_corte[i]].ABC.dire.z * inter.Pint.z;
				res += escena->Planos[cuaTemp.planos_corte[i]].D;
				if ((escena->Planos[cuaTemp.planos_corte[i]].dire && res<0)
			     || (!escena->Planos[cuaTemp.planos_corte[i]].dire && res>0)){
					*t = INFINITY;
					inter.t = *t;
					return inter;
				}
			}
		}
	}
		
	return inter;
}

Interseccion InterPoligono(Rayo ray,long double *t,Poligono poliTemp,Escena *escena){
	Interseccion inter;
	Vector ABC;
	long double D,denominador,m,b;
	int i,x,y;
	int xx,yy;
	long double res;
	
	xx=0;
	yy=0;
	ABC = poliTemp.Normal;
	D = poliTemp.D;
	denominador = ABC.dire.x * ray.vector.dire.x;
	denominador += ABC.dire.y * ray.vector.dire.y;
	denominador += ABC.dire.z * ray.vector.dire.z;
	if (denominador == 0){
		*t = INFINITY;
		inter.t = *t;
		return inter;
	}
	*t = ABC.dire.x * ray.ancla.x + ABC.dire.y * ray.ancla.y + 
		 ABC.dire.z * ray.ancla.z + D;
    *t *=-1.0;
    *t /= denominador;
    inter.objeto.color = poliTemp.color;
    //inter.
    inter.t = *t;
    
    inter.Pint.x = ray.ancla.x + *t * ray.vector.dire.x;
	inter.Pint.y = ray.ancla.y + *t * ray.vector.dire.y;
	inter.Pint.z = ray.ancla.z + *t * ray.vector.dire.z;
	if (poliTemp.aplastado == X){
		x = inter.Pint.y;
		y = inter.Pint.z;
	}else if (poliTemp.aplastado == Y){
		x = inter.Pint.x;
		y = inter.Pint.z;
	}else{
		x = inter.Pint.x;
		y = inter.Pint.y;
	}
    
    
    
    
	xx = 0;
	yy = 0;
    Punto p0,p1;
    int contaParedes=0;
    p0 = poliTemp.puntos[0];
    p0.x -=x; //trasladar
    p0.y -=y;
    for (i=1;i!=-2; i++){
		p1  = poliTemp.puntos[i];
		p1.x -=x; //trasladar
		p1.y -=y;
			if (p0.y == yy && p0.x > xx){
				contaParedes = 0;
				break;
			}else
			//Revisamos casos
			if (((p0.y > yy && p1.y < yy) || (p0.y < yy && p1.y > yy)) && 
				(p0.x > xx && p1.x > xx)){
				contaParedes++;
			}else if (!
				((p0.x < xx && p1.x < xx) || 
				 (p0.y > yy && p1.y > yy) || 
				 (p0.y < yy && p1.y < yy))
				){
				
				m = (p1.y - p0.y) / (p1.x - p0.x);
				b = (p0.y - m * p0.x);
				if ((-b / m)>xx)
					contaParedes++;
			}
			p0 = p1;
			if (i==poliTemp.numPuntos-1)
				i=-1;
			if (i==0){
				break;
			}
	}
	if (!(contaParedes % 2)){
		*t = INFINITY;
		inter.t = *t;
	}
	
	
	
	inter.t = *t;
	
	if (poliTemp.cortes>0){
		for (i=0;i<poliTemp.cortes;i++){
			res = escena->Planos[poliTemp.planos_corte[i]].ABC.dire.x * inter.Pint.x;
			res += escena->Planos[poliTemp.planos_corte[i]].ABC.dire.y * inter.Pint.y;
			res += escena->Planos[poliTemp.planos_corte[i]].ABC.dire.z * inter.Pint.z;
			res += escena->Planos[poliTemp.planos_corte[i]].D;
			if ((escena->Planos[poliTemp.planos_corte[i]].dire && res<0)
			     || (!escena->Planos[poliTemp.planos_corte[i]].dire && res>0)){
					*t = INFINITY;
					inter.t = *t;
					return inter;	
			}
		}
		
	}
	
	if (poliTemp.disco==1 && NORMA(CREAR_VECTOR(poliTemp.centro,inter.Pint)) > poliTemp.radio){
		*t = INFINITY;
		inter.t = *t;
	}
	return inter;
}

Interseccion InterEsfera(Rayo ray,long double *t,Esfera cirTemp,Escena *escena){
	long double a,b,c;
	long double Xe_Xc,Ye_Yc,Ze_Zc;
	long double discr;
	long double temp1,temp2;
	long double sqr;
	long double t2;
	long double res;
	int i;
	char nohay;
	Interseccion inter;
	
	Xe_Xc = ray.ancla.x - cirTemp.centro.x;
	Ye_Yc = ray.ancla.y - cirTemp.centro.y;
	Ze_Zc = ray.ancla.z - cirTemp.centro.z;
	a = 1.0;
	b = 2.0 * (Xe_Xc * ray.vector.dire.x + Ye_Yc * ray.vector.dire.y 
			 + Ze_Zc * ray.vector.dire.z);
	c = ((Xe_Xc * Xe_Xc)+ (Ye_Yc * Ye_Yc)+ (Ze_Zc * Ze_Zc) 
			- (cirTemp.radio2));
	discr = (b * b) - (4 * c * a); //Se omite el a xq a = 1.0 (Esfera)
	if (discr < 0){
		*t = INFINITY;
		inter.t = *t;
		return inter;
	}
	sqr = sqrtl(discr);
	temp1 = (-b + sqr)/2.0;
	temp2 = (-b - sqr)/2.0;
	inter.objeto.color = cirTemp.color;
	if (temp1 < 0.0)
		temp1 = INFINITY;
	if (temp2 < 0.0)
		temp2 = INFINITY;
	
	*t = MIN(temp1,temp2);
	if (*t<=EPSILON){
		*t = MAX(temp1,temp2);
		t2 = MIN(temp1,temp2);
	}else{
		t2 = MAX(temp1,temp2);
	}
	inter.t = *t;
	
	inter.Pint.x = ray.ancla.x + (*t) * ray.vector.dire.x;
	inter.Pint.y = ray.ancla.y + (*t) * ray.vector.dire.y;
	inter.Pint.z = ray.ancla.z + (*t) * ray.vector.dire.z;
	nohay = 0;
	if (cirTemp.cortes>0){
		for (i=0;i<cirTemp.cortes;i++){
			res = escena->Planos[cirTemp.planos_corte[i]].ABC.dire.x * inter.Pint.x;
			res += escena->Planos[cirTemp.planos_corte[i]].ABC.dire.y * inter.Pint.y;
			res += escena->Planos[cirTemp.planos_corte[i]].ABC.dire.z * inter.Pint.z;
			res += escena->Planos[cirTemp.planos_corte[i]].D;
			if ((escena->Planos[cirTemp.planos_corte[i]].dire && res<0)
			     || (!escena->Planos[cirTemp.planos_corte[i]].dire && res>0)){
				nohay = 1;	
			}
		}
		if (nohay){
			inter.t = *t;
			*t = t2;
			t2 = inter.t;
			inter.Pint.x = ray.ancla.x + (*t) * ray.vector.dire.x;
			inter.Pint.y = ray.ancla.y + (*t) * ray.vector.dire.y;
			inter.Pint.z = ray.ancla.z + (*t) * ray.vector.dire.z;
			inter.t = *t;
			for (i=0;i<cirTemp.cortes;i++){
				res = escena->Planos[cirTemp.planos_corte[i]].ABC.dire.x * inter.Pint.x;
				res += escena->Planos[cirTemp.planos_corte[i]].ABC.dire.y * inter.Pint.y;
				res += escena->Planos[cirTemp.planos_corte[i]].ABC.dire.z * inter.Pint.z;
				res += escena->Planos[cirTemp.planos_corte[i]].D;
				if ((escena->Planos[cirTemp.planos_corte[i]].dire && res<0)
			     || (!escena->Planos[cirTemp.planos_corte[i]].dire && res>0)){
					*t = INFINITY;
					inter.t = *t;
					return inter;
				}
			}
		}
	}
	if (*t == INFINITY){
		inter.t = *t;
		return inter;
	}
	//Mapas de Calado
	if (cirTemp.calado != -1){
		int ii,jj,pega;
		Vector N;
		Objeto Q;
		Q.obj = &cirTemp;
		N = NormalEsfera(&Q,inter.Pint);
		
		
		//printf("esfera\n");
		posicionTexturaEsfera(cirTemp,cirTemp.calado,inter.Pint,escena,&ii,&jj,N);
		
		pega = Calado(cirTemp.calado,ii,jj,escena);
		
		if (!pega && t2!=INFINITY && t2>EPSILON){
			*t = t2;
			inter.Pint.x = ray.ancla.x + *t * ray.vector.dire.x;
			inter.Pint.y = ray.ancla.y + *t * ray.vector.dire.y;
			inter.Pint.z = ray.ancla.z + *t * ray.vector.dire.z;
			N = NormalEsfera(&Q,inter.Pint);
			posicionTexturaEsfera(cirTemp,cirTemp.calado,inter.Pint,escena,&ii,&jj,N);
			
			pega = Calado(cirTemp.calado,ii,jj,escena);
			if (!pega){
				*t = INFINITY;
				inter.t = *t;
				return inter;
			}
		}
	}
	
	
	
	
	return inter;
}

Interseccion InterCilindro(Rayo ray,long double *t,Cilindro cilTemp,Escena *escena){
	long double a,b,c;
	long double discr;
	long double temp1,temp2;
	long double sqr;
	long double XDXQ,YDYQ,ZDZQ;
	long double XQ,XQ2,XQ3,XQ4;
	long double YQ,YQ2,YQ3,YQ4;
	long double ZQ,ZQ2,ZQ3,ZQ4;
	long double XD,YD,ZD;
	long double XD2,YD2,ZD2;
	long double Xo2,Yo2,Zo2;
	long double Xe2,Ye2,Ze2;
	long double Xo,Yo,Zo;
	long double Xe,Ye,Ze;
	long double XeXo,YeYo,ZeZo;
	long double _XeXo,_YeYo;
	long double XoXe,YoYe,ZoZe;
	long double _XoXe,_YoYe;
	long double t2;
	long double res;
	char nohay;
	int i;
	Punto Q = cilTemp.rayo.vector.dire;
	Interseccion inter;
	
	XQ = Q.x;
	XQ2 = XQ * XQ;
	XQ3 = XQ * XQ * XQ;
	XQ4 = XQ * XQ * XQ * XQ;
	
	YQ = Q.y;
	YQ2 = YQ * YQ;
	YQ3 = YQ * YQ * YQ;
	YQ4 = YQ * YQ * YQ * YQ;
	
	ZQ = Q.z;
	ZQ2 = ZQ * ZQ;
	ZQ3 = ZQ * ZQ * ZQ;
	ZQ4 = ZQ * ZQ * ZQ * ZQ;
	
	XDXQ = ray.vector.dire.x * Q.x;
	YDYQ = ray.vector.dire.y * Q.y;
	ZDZQ = ray.vector.dire.z * Q.z;
	
	XD = ray.vector.dire.x;
	XD2 = ray.vector.dire.x * ray.vector.dire.x;
	YD = ray.vector.dire.y;
	YD2 = ray.vector.dire.y * ray.vector.dire.y;
	ZD = ray.vector.dire.z;
	ZD2 = ray.vector.dire.z * ray.vector.dire.z;
	
	Xo = cilTemp.rayo.ancla.x; Yo = cilTemp.rayo.ancla.y; Zo = cilTemp.rayo.ancla.z;
	Xo2 = Xo * Xo; Yo2 = Yo * Yo; Zo2 = Zo * Zo;
	
	Xe = ray.ancla.x; Ye = ray.ancla.y; Ze = ray.ancla.z;
	Xe2 = Xe * Xe; Ye2 = Ye * Ye; Ze2 = Ze * Ze;
	
	
	a =  ( +2*(YDYQ +XDXQ)*ZDZQ*ZQ2 +((YQ2 +XQ2-2)*ZD2 +YD2*YQ2
			+2*XDXQ*YDYQ +XD2*XQ2)*ZQ2 +(2*YDYQ*YQ2 +2*XDXQ*YQ2
			+2*((XQ2-2)*YDYQ + XDXQ*XQ2 -2*XDXQ))*ZDZQ 
			+ZD2*ZQ4 +YD2*YQ4 +XD2*XQ4 +2*XDXQ*YDYQ*YQ2 +((XQ2-2)*YD2
			+XD2*XQ2)*YQ2 +2*(XDXQ*XQ2-2*XDXQ)*YDYQ +YD2 +ZD2 +XD2
			-2*XD2*XQ2);

	XeXo = 2 * (ray.ancla.x - cilTemp.rayo.ancla.x);
	YeYo = 2 * (ray.ancla.y - cilTemp.rayo.ancla.y);
	ZeZo = 2 * (ray.ancla.z - cilTemp.rayo.ancla.z);
	XoXe = 4 * (cilTemp.rayo.ancla.x - ray.ancla.x);
	YoYe = 4 * (cilTemp.rayo.ancla.y - ray.ancla.y);
	ZoZe = 4 * (cilTemp.rayo.ancla.z - ray.ancla.z);
	
	b  = ((ZeZo*ZDZQ
		  +(YeYo*YQ+XeXo*XQ)*ZD
		  +ZeZo*YDYQ
		  +ZeZo*XDXQ
		)*ZQ3
		+((ZeZo*YQ2+ZeZo*XQ2+ZoZe)*ZD
		   +(XeXo*XQ*YD+YeYo*XDXQ)*YQ
		   +YeYo*YDYQ*YQ
		   +XeXo*XDXQ*XQ
		 )*ZQ2
		+((YeYo*YQ3
		   +XeXo*XQ*YQ2
		   +(YeYo*XQ2+YoYe)*YQ
		   +XeXo*XQ3+XoXe*XQ
		  )*ZD
		  +ZeZo*XDXQ*YQ2
		  +(ZeZo*XQ2+ZoZe)*YD*YQ
		  +ZeZo*YDYQ*YQ3
		  +ZeZo*XDXQ*XQ2
		  +ZoZe*XDXQ
		)*ZQ
		+(YeYo*YDYQ
		  +XeXo*XQ*YD
		  +YeYo*XDXQ
		)*YQ3
		+((YeYo*XQ2
			+YoYe
		  )*YD
		  +XeXo*XDXQ*XQ
		)*YQ2
		+((XeXo*XQ3
			+XoXe*XQ
		  )*YD
		  +YeYo*XDXQ*XQ2
		  +YoYe*XDXQ
		)*YQ
		+XeXo*XDXQ*XQ3
		+XoXe*XDXQ*XQ
		+XeXo*XD
		+YeYo*YD
		+ZeZo*ZD);
	//b = -2*((Zo-Ze)*ZD*ZQ4+(((Yo-Ye)*YQ+(Xo-Xe)*XQ)*ZD+(Zo-Ze)*YD*YQ+(Zo-Ze)*XD*XQ)*ZQ3+(((Zo-Ze)*YQ2+(Zo-Ze)*XQ2+ZeZo)*ZD+(Yo-Ye)*YD*YQ2+((Xo-Xe)*XQ*YD+(Yo-Ye)*XD*XQ)*YQ+(Xo-Xe)*XD*XQ2)*ZQ2+(((Yo-Ye)*YQ3+(Xo-Xe)*XQ*YQ2+((Yo-Ye)*XQ2+YeYo)*YQ+(Xo-Xe)*XQ3+XeXo*XQ)*ZD+(Zo-Ze)*YD*YQ3+(Zo-Ze)*XD*XQ*YQ2+((Zo-Ze)*XQ2+ZeZo)*YD*YQ+(Zo-Ze)*XD*XQ3+ZeZo*XD*XQ)*ZQ+(Zo-Ze)*ZD+(Yo-Ye)*YD*YQ4+((Xo-Xe)*XQ*YD+(Yo-Ye)*XD*XQ)*YQ3+(((Yo-Ye)*XQ2+YeYo)*YD+(Xo-Xe)*XD*XQ2)*YQ2+(((Xo-Xe)*XQ3+XeXo*XQ)*YD+(Yo-Ye)*XD*XQ3+YeYo*XD*XQ)*YQ+(Yo-Ye)*YD+(Xo-Xe)*XD*XQ4+XeXo*XD*XQ2+(Xo-Xe)*XD);
	                 
	_XeXo = 4 * (ray.ancla.x - cilTemp.rayo.ancla.x);
	_YeYo = 4 * (ray.ancla.y - cilTemp.rayo.ancla.y);
	
	_XoXe = 2 * (cilTemp.rayo.ancla.x - ray.ancla.x);
	_YoYe = 2 * (cilTemp.rayo.ancla.y - ray.ancla.y);
	
		c = ((Zo2 -  2*Ze*Zo  +  Ze2)*ZQ4
		+((Zo*_YoYe  +  YeYo*Ze)*YQ
		   +(Zo*_XoXe  +  XeXo*Ze)*XQ
		  )*ZQ3
		+((Zo2  -  2*Ze*Zo  +  Ze2  +  Yo2  -  2*Ye*Yo + Ye2)*YQ2
			+(Yo*_XoXe + XeXo*Ye)*XQ*YQ
			+(Zo2 - 2*Ze*Zo + Ze2 + Xo2 - 2*Xe*Xo + Xe2)*XQ2
			-2*Zo2
		   +4*Ze*Zo - 2*Ze2)*ZQ2

		+((Zo*_YoYe + YeYo*Ze)*YQ3
		   +(Zo*_XoXe + XeXo*Ze)*XQ*YQ2
		   +((Zo*_YoYe + YeYo*Ze)*XQ2
		   +Zo*_YeYo + YoYe*Ze)*YQ
		   +(Zo*_XoXe + XeXo*Ze)*XQ3
		   +(Zo*_XeXo + XoXe*Ze)*XQ
		  )*ZQ
		+(Yo2 - 2*Ye*Yo + Ye2)*YQ4
		+(Yo*_XoXe + XeXo*Ye)*XQ*YQ3
		+((Yo2 - 2*Ye*Yo + Ye2 + Xo2 - 2*Xe*Xo + Xe2)*XQ2
		   +2*(-Yo2 +2*Ye*Yo -Ye2)
		  )*YQ2
		+((Yo*_XoXe + XeXo*Ye)*XQ3
		   +(Yo*_XeXo + XoXe*Ye)*XQ
		  )*YQ
		+(Xo2 - 2*Xe*Xo + Xe2)*XQ4
		+2*(-Xo2 + 2*Xe*Xo - Xe2)*XQ2
		- (cilTemp.radio2)
		+Zo2
		-2*(Ze*Zo + Ye*Yo  +Xe*Xo)
		+Ze2
		+Yo2
		+Ye2
		+Xo2
		+Xe2);

	
	//c = (Zo2-2*Ze*Zo+Ze2)*ZQ4+(((-YeYo)*Zo+2*(Ye-Yo)*Ze)*YQ+((-XeXo)*Zo+XeXo*Ze)*XQ)*ZQ3+((Zo2-2*Ze*Zo+Ze2+Yo2-2*Ye*Yo+Ye2)*YQ2+((-XeXo)*Yo+XeXo*Ye)*XQ*YQ+(Zo2-2*Ze*Zo+Ze2+Xo2-2*Xe*Xo+Xe2)*XQ2-2*Zo2+4*Ze*Zo-2*Ze2)*ZQ2+(((-YeYo)*Zo+2*(Ye-Yo)*Ze)*YQ3+((-XeXo)*Zo+XeXo*Ze)*XQ*YQ2+(((-YeYo)*Zo+2*(Ye-Yo)*Ze)*XQ2+4*(Ye-Yo)*Zo+YoYe*Ze)*YQ+((-XeXo)*Zo+XeXo*Ze)*XQ3+((-XoXe)*Zo+XoXe*Ze)*XQ)*ZQ+(Yo2-2*Ye*Yo+Ye2)*YQ4+((-XeXo)*Yo+XeXo*Ye)*XQ*YQ3+((Yo2-2*Ye*Yo+Ye2+Xo2-2*Xe*Xo+Xe2)*XQ2-2*Yo2+4*Ye*Yo-2*Ye2)*YQ2+(((-XeXo)*Yo+XeXo*Ye)*XQ3+((-XoXe)*Yo+XoXe*Ye)*XQ)*YQ+(Xo2-2*Xe*Xo+Xe2)*XQ4+(-2*Xo2+4*Xe*Xo-2*Xe2)*XQ2
	//  -(cilTemp.radio2)+Zo2-2*Ze*Zo+Ze2+Yo2-2*Ye*Yo+Ye2+Xo2-2*Xe*Xo+Xe2;
	
	
	discr = (b * b) - (4 * c * a); 
	if (discr < 0){
		*t = INFINITY;
		inter.t = *t;
		return inter;
	}
	sqr = sqrtl(discr);
	temp1 = (-b + sqr)/(2.0 * a);
	temp2 = (-b - sqr)/(2.0 * a);
	inter.objeto.color = cilTemp.color;
	if (temp1 < 0.0)
		temp1 = INFINITY;
	if (temp2 < 0.0)
		temp2 = INFINITY;
	*t = MIN(temp1,temp2);
	if (*t<=EPSILON){
		*t = MAX(temp1,temp2);
		t2 = MIN(temp1,temp2);
	}else{
		t2 = MAX(temp1,temp2);
	}
	if (*t<=EPSILON){
		*t = INFINITY;
		inter.t = *t;
		return inter;
	}
	
	inter.Pint.x = ray.ancla.x + *t * ray.vector.dire.x;
	inter.Pint.y = ray.ancla.y + *t * ray.vector.dire.y;
	inter.Pint.z = ray.ancla.z + *t * ray.vector.dire.z;
	
	nohay = 0;
	if (cilTemp.cortes>0){
		for (i=0;i<cilTemp.cortes;i++){
			res = escena->Planos[cilTemp.planos_corte[i]].ABC.dire.x * inter.Pint.x;
			res += escena->Planos[cilTemp.planos_corte[i]].ABC.dire.y * inter.Pint.y;
			res += escena->Planos[cilTemp.planos_corte[i]].ABC.dire.z * inter.Pint.z;
			res += escena->Planos[cilTemp.planos_corte[i]].D;
			if ((escena->Planos[cilTemp.planos_corte[i]].dire && res<0)
			     || (!escena->Planos[cilTemp.planos_corte[i]].dire && res>0)){
				nohay = 1;	
			}
		}
		if (nohay){
			inter.t = *t;
			*t = t2;
			t2 = inter.t;
			inter.Pint.x = ray.ancla.x + (*t) * ray.vector.dire.x;
			inter.Pint.y = ray.ancla.y + (*t) * ray.vector.dire.y;
			inter.Pint.z = ray.ancla.z + (*t) * ray.vector.dire.z;
			inter.t = *t;
			for (i=0;i<cilTemp.cortes;i++){
				res = escena->Planos[cilTemp.planos_corte[i]].ABC.dire.x * inter.Pint.x;
				res += escena->Planos[cilTemp.planos_corte[i]].ABC.dire.y * inter.Pint.y;
				res += escena->Planos[cilTemp.planos_corte[i]].ABC.dire.z * inter.Pint.z;
				res += escena->Planos[cilTemp.planos_corte[i]].D;
				if ((escena->Planos[cilTemp.planos_corte[i]].dire && res<0)
			     || (!escena->Planos[cilTemp.planos_corte[i]].dire && res>0)){
					*t = INFINITY;
					inter.t = *t;
					return inter;
				}
			}
		}
	}
	
	

	
	
	
	
	
	
	long double d = (inter.Pint.x - cilTemp.rayo.ancla.x) * cilTemp.rayo.vector.dire.x
	                + (inter.Pint.y - cilTemp.rayo.ancla.y) * cilTemp.rayo.vector.dire.y
	                + (inter.Pint.z - cilTemp.rayo.ancla.z) * cilTemp.rayo.vector.dire.z;
	
	if (!(cilTemp.h1 <= d && d <= cilTemp.h2)){
		*t = MAX(temp1,temp2);
		inter.Pint.x = ray.ancla.x + *t * ray.vector.dire.x;
		inter.Pint.y = ray.ancla.y + *t * ray.vector.dire.y;
		inter.Pint.z = ray.ancla.z + *t * ray.vector.dire.z;
		d = (inter.Pint.x - cilTemp.rayo.ancla.x) * cilTemp.rayo.vector.dire.x
	                + (inter.Pint.y - cilTemp.rayo.ancla.y) * cilTemp.rayo.vector.dire.y
	                + (inter.Pint.z - cilTemp.rayo.ancla.z) * cilTemp.rayo.vector.dire.z;
	    if (!(cilTemp.h1 <= d && d <= cilTemp.h2)){
			*t = INFINITY;
			inter.t = *t;
			return inter;
		}
			if (*t == INFINITY){
				inter.t = *t;
				return inter;
			}
		
		
	}
	

	
//Mapas de Calado
		if (cilTemp.calado != -1 ){
			int ii,jj,pega;
			Vector N;
			Objeto Q;
			Q.obj = &cilTemp;
			N = NormalCilindro(&Q,inter.Pint);
			
		//	printf("d %LF\n",d);
		//	printf("cil %LF %LF %LF \n",inter.Pint.x,inter.Pint.y,inter.Pint.z);
			posicionTexturaCilindro(cilTemp,cilTemp.calado,inter.Pint,escena,&ii,&jj,N);
			
			pega = Calado(cilTemp.calado,ii,jj,escena);
			
			if (!pega && t2!=INFINITY && t2>EPSILON){
				*t = t2;
				inter.Pint.x = ray.ancla.x + *t * ray.vector.dire.x;
				inter.Pint.y = ray.ancla.y + *t * ray.vector.dire.y;
				inter.Pint.z = ray.ancla.z + *t * ray.vector.dire.z;
				N = NormalCilindro(&Q,inter.Pint);
			//	printf("cil %LF %LF %LF \n",inter.Pint.x,inter.Pint.y,inter.Pint.z);
				posicionTexturaCilindro(cilTemp,cilTemp.calado,inter.Pint,escena,&ii,&jj,N);
				
				pega = Calado(cilTemp.calado,ii,jj,escena);
				if (!pega){
					*t = INFINITY;
					inter.t = *t;
					return inter;
				}
			}
		}
	
	
	
	
	
	inter.d = d;
	inter.t = *t;
	
	return inter;
}

Interseccion InterCono(Rayo ray,long double *t,Cono conTemp,Escena *escena){
	long double a,b,c;
	long double discr;
	long double temp1,temp2;
	long double sqr;
	long double XDXQ,YDYQ,ZDZQ;
	long double XQ,XQ2,XQ3,XQ4;
	long double YQ,YQ2,YQ3,YQ4;
	long double ZQ,ZQ2,ZQ3,ZQ4;
	long double XD,YD,ZD;
	long double XD2,YD2,ZD2;
	long double Xo2,Yo2,Zo2;
	long double Xe2,Ye2,Ze2;
	long double Xo,Yo,Zo;
	long double Xe,Ye,Ze;
	long double XeXo,YeYo,ZeZo;
	long double _XeXo,_YeYo;
	long double XoXe,YoYe,ZoZe;
	long double _XoXe,_YoYe;
	long double machete, machete2;
	long double t2;
	long double res;
	int i;
	char nohay;
	Punto Q = conTemp.rayo.vector.dire;
	Interseccion inter;
	
	XQ = Q.x;
	XQ2 = XQ * XQ;
	XQ3 = XQ * XQ * XQ;
	XQ4 = XQ * XQ * XQ * XQ;
	
	YQ = Q.y;
	YQ2 = YQ * YQ;
	YQ3 = YQ * YQ * YQ;
	YQ4 = YQ * YQ * YQ * YQ;
	
	ZQ = Q.z;
	ZQ2 = ZQ * ZQ;
	ZQ3 = ZQ * ZQ * ZQ;
	ZQ4 = ZQ * ZQ * ZQ * ZQ;
	
	XDXQ = ray.vector.dire.x * Q.x;
	YDYQ = ray.vector.dire.y * Q.y;
	ZDZQ = ray.vector.dire.z * Q.z;
	
	XD = ray.vector.dire.x;
	XD2 = ray.vector.dire.x * ray.vector.dire.x;
	YD = ray.vector.dire.y;
	YD2 = ray.vector.dire.y * ray.vector.dire.y;
	ZD = ray.vector.dire.z;
	ZD2 = ray.vector.dire.z * ray.vector.dire.z;
	
	Xo = conTemp.rayo.ancla.x; Yo = conTemp.rayo.ancla.y; Zo = conTemp.rayo.ancla.z;
	Xo2 = Xo * Xo; Yo2 = Yo * Yo; Zo2 = Zo * Zo;
	
	Xe = ray.ancla.x; Ye = ray.ancla.y; Ze = ray.ancla.z;
	Xe2 = Xe * Xe; Ye2 = Ye * Ye; Ze2 = Ze * Ze;
	
	
	machete = (XDXQ) + (YDYQ) + (ZDZQ);
	
	
	a =  ( +2*(YDYQ +XDXQ)*ZDZQ*ZQ2 +((YQ2 +XQ2-2)*ZD2 +YD2*YQ2
			+2*XDXQ*YDYQ +XD2*XQ2)*ZQ2 +(2*YDYQ*YQ2 +2*XDXQ*YQ2
			+2*((XQ2-2)*YDYQ + XDXQ*XQ2 -2*XDXQ))*ZDZQ 
			+ZD2*ZQ4 +YD2*YQ4 +XD2*XQ4 +2*XDXQ*YDYQ*YQ2 +((XQ2-2)*YD2
			+XD2*XQ2)*YQ2 +2*(XDXQ*XQ2-2*XDXQ)*YDYQ +YD2 +ZD2 +XD2
			-2*XD2*XQ2);
	a -= (machete*machete)*conTemp.uv;
	
	XeXo = 2 * (ray.ancla.x - conTemp.rayo.ancla.x);
	YeYo = 2 * (ray.ancla.y - conTemp.rayo.ancla.y);
	ZeZo = 2 * (ray.ancla.z - conTemp.rayo.ancla.z);
	XoXe = 4 * (conTemp.rayo.ancla.x - ray.ancla.x);
	YoYe = 4 * (conTemp.rayo.ancla.y - ray.ancla.y);
	ZoZe = 4 * (conTemp.rayo.ancla.z - ray.ancla.z);
	
	b  = ((ZeZo*ZDZQ
		  +(YeYo*YQ+XeXo*XQ)*ZD
		  +ZeZo*YDYQ
		  +ZeZo*XDXQ
		)*ZQ3
		+((ZeZo*YQ2+ZeZo*XQ2+ZoZe)*ZD
		   +(XeXo*XQ*YD+YeYo*XDXQ)*YQ
		   +YeYo*YDYQ*YQ
		   +XeXo*XDXQ*XQ
		 )*ZQ2
		+((YeYo*YQ3
		   +XeXo*XQ*YQ2
		   +(YeYo*XQ2+YoYe)*YQ
		   +XeXo*XQ3+XoXe*XQ
		  )*ZD
		  +ZeZo*XDXQ*YQ2
		  +(ZeZo*XQ2+ZoZe)*YD*YQ
		  +ZeZo*YDYQ*YQ3
		  +ZeZo*XDXQ*XQ2
		  +ZoZe*XDXQ
		)*ZQ
		+(YeYo*YDYQ
		  +XeXo*XQ*YD
		  +YeYo*XDXQ
		)*YQ3
		+((YeYo*XQ2
			+YoYe
		  )*YD
		  +XeXo*XDXQ*XQ
		)*YQ2
		+((XeXo*XQ3
			+XoXe*XQ
		  )*YD
		  +YeYo*XDXQ*XQ2
		  +YoYe*XDXQ
		)*YQ
		+XeXo*XDXQ*XQ3
		+XoXe*XDXQ*XQ
		+XeXo*XD
		+YeYo*YD
		+ZeZo*ZD);
	             
	             
	machete2 = (Xo-Xe)*XQ + (Yo-Ye)*YQ + (Zo-Ze)*ZQ;
	
	b += 2 * machete * conTemp.uv * machete2;
	             
	_XeXo = 4 * (ray.ancla.x - conTemp.rayo.ancla.x);
	_YeYo = 4 * (ray.ancla.y - conTemp.rayo.ancla.y);
	
	_XoXe = 2 * (conTemp.rayo.ancla.x - ray.ancla.x);
	_YoYe = 2 * (conTemp.rayo.ancla.y - ray.ancla.y);
	
		c = ((Zo2 -  2*Ze*Zo  +  Ze2)*ZQ4
		+((Zo*_YoYe  +  YeYo*Ze)*YQ
		   +(Zo*_XoXe  +  XeXo*Ze)*XQ
		  )*ZQ3
		+((Zo2  -  2*Ze*Zo  +  Ze2  +  Yo2  -  2*Ye*Yo + Ye2)*YQ2
			+(Yo*_XoXe + XeXo*Ye)*XQ*YQ
			+(Zo2 - 2*Ze*Zo + Ze2 + Xo2 - 2*Xe*Xo + Xe2)*XQ2
			-2*Zo2
		   +4*Ze*Zo - 2*Ze2)*ZQ2

		+((Zo*_YoYe + YeYo*Ze)*YQ3
		   +(Zo*_XoXe + XeXo*Ze)*XQ*YQ2
		   +((Zo*_YoYe + YeYo*Ze)*XQ2
		   +Zo*_YeYo + YoYe*Ze)*YQ
		   +(Zo*_XoXe + XeXo*Ze)*XQ3
		   +(Zo*_XeXo + XoXe*Ze)*XQ
		  )*ZQ
		+(Yo2 - 2*Ye*Yo + Ye2)*YQ4
		+(Yo*_XoXe + XeXo*Ye)*XQ*YQ3
		+((Yo2 - 2*Ye*Yo + Ye2 + Xo2 - 2*Xe*Xo + Xe2)*XQ2
		   +2*(-Yo2 +2*Ye*Yo -Ye2)
		  )*YQ2
		+((Yo*_XoXe + XeXo*Ye)*XQ3
		   +(Yo*_XeXo + XoXe*Ye)*XQ
		  )*YQ
		+(Xo2 - 2*Xe*Xo + Xe2)*XQ4
		+2*(-Xo2 + 2*Xe*Xo - Xe2)*XQ2
		//- (cilTemp.radio2)
		+Zo2
		-2*(Ze*Zo + Ye*Yo  +Xe*Xo)
		+Ze2
		+Yo2
		+Ye2
		+Xo2
		+Xe2);

     c -= (machete2 * machete2 * conTemp.uv);
	
	
	discr = (b * b) - (4 * c * a); 
	if (discr < 0){
		*t = INFINITY;
		inter.t = *t;
		return inter;
	}
	sqr = sqrtl(discr);
	temp1 = (-b + sqr)/(2.0 * a);
	temp2 = (-b - sqr)/(2.0 * a);
	inter.objeto.color = conTemp.color;
	if (temp1 < 0.0)
		temp1 = INFINITY;
	if (temp2 < 0.0)
		temp2 = INFINITY;
	*t = MIN(temp1,temp2);
	if (*t<=EPSILON){
		*t = MAX(temp1,temp2);
		t2 = MIN(temp1,temp2);
	}else{
		t2 = MAX(temp1,temp2);
	}
	inter.Pint.x = ray.ancla.x + *t * ray.vector.dire.x;
	inter.Pint.y = ray.ancla.y + *t * ray.vector.dire.y;
	inter.Pint.z = ray.ancla.z + *t * ray.vector.dire.z;
	
	nohay = 0;
	if (conTemp.cortes>0){
		for (i=0;i<conTemp.cortes;i++){
			res = escena->Planos[conTemp.planos_corte[i]].ABC.dire.x * inter.Pint.x;
			res += escena->Planos[conTemp.planos_corte[i]].ABC.dire.y * inter.Pint.y;
			res += escena->Planos[conTemp.planos_corte[i]].ABC.dire.z * inter.Pint.z;
			res += escena->Planos[conTemp.planos_corte[i]].D;
			if ((escena->Planos[conTemp.planos_corte[i]].dire && res<0)
			     || (!escena->Planos[conTemp.planos_corte[i]].dire && res>0)){
				nohay = 1;	
			}
		}
		if (nohay){
			*t = t2;
			inter.Pint.x = ray.ancla.x + (*t) * ray.vector.dire.x;
			inter.Pint.y = ray.ancla.y + (*t) * ray.vector.dire.y;
			inter.Pint.z = ray.ancla.z + (*t) * ray.vector.dire.z;
			inter.t = *t;
			for (i=0;i<conTemp.cortes;i++){
				res = escena->Planos[conTemp.planos_corte[i]].ABC.dire.x * inter.Pint.x;
				res += escena->Planos[conTemp.planos_corte[i]].ABC.dire.y * inter.Pint.y;
				res += escena->Planos[conTemp.planos_corte[i]].ABC.dire.z * inter.Pint.z;
				res += escena->Planos[conTemp.planos_corte[i]].D;
				if ((escena->Planos[conTemp.planos_corte[i]].dire && res<0)
			     || (!escena->Planos[conTemp.planos_corte[i]].dire && res>0)){
					*t = INFINITY;
					inter.t = *t;
					return inter;
				}
			}
		}
	}
	
	
	
	
	
	long double d = (inter.Pint.x - conTemp.rayo.ancla.x) * conTemp.rayo.vector.dire.x
	                + (inter.Pint.y - conTemp.rayo.ancla.y) * conTemp.rayo.vector.dire.y
	                + (inter.Pint.z - conTemp.rayo.ancla.z) * conTemp.rayo.vector.dire.z;
	
	if (!(conTemp.h1 <= d && d <= conTemp.h2)){
		*t = MAX(temp1,temp2);
		inter.Pint.x = ray.ancla.x + *t * ray.vector.dire.x;
		inter.Pint.y = ray.ancla.y + *t * ray.vector.dire.y;
		inter.Pint.z = ray.ancla.z + *t * ray.vector.dire.z;
		d = (inter.Pint.x - conTemp.rayo.ancla.x) * conTemp.rayo.vector.dire.x
	                + (inter.Pint.y - conTemp.rayo.ancla.y) * conTemp.rayo.vector.dire.y
	                + (inter.Pint.z - conTemp.rayo.ancla.z) * conTemp.rayo.vector.dire.z;
	    if (!(conTemp.h1 <= d && d <= conTemp.h2)){
			*t = INFINITY;
		}
	}
	
	
	
	inter.d = d;
	inter.t = *t;
	return inter;
}

long double mod(long double a, long double b)
{
	long int result = ((long) a )/ (long) b ;
	return a - result * b;
}

void posicionTexturaPoligono(Poligono poli, int tex ,Punto pinter,Escena *escena, int *i, int *j){
	long double u,v,xx,yy;
	Vector vPinter;
	
	vPinter = CREAR_VECTOR(poli.puntos[0],pinter);
	u = PRODUCTO_PUNTO(vPinter,poli.U);
	v = PRODUCTO_PUNTO(vPinter,poli.V);
	xx=truncl(u/escena->Texturas[tex].mosaicoX);
	xx *=escena->Texturas[tex].mosaicoX;
	yy=truncl(v/escena->Texturas[tex].mosaicoY);
	yy *=escena->Texturas[tex].mosaicoY;
	
	u = u - xx;
	v = v - yy;
	u /= escena->Texturas[tex].mosaicoX;
	v /= escena->Texturas[tex].mosaicoY;
	*i = roundl( u * escena->Texturas[tex].resX);
	*j = roundl( v * escena->Texturas[tex].resY);
	if (*i<0)
		(*i)*=-1;
	if (*j<0)
		(*j)*=-1;
}
void posicionTexturaEsfera(Esfera esf, int tex ,Punto pinter,Escena *escena, int *i, int *j, Vector Normal){
	long double u,v;
	long double denominador,t;
	Punto pinter2;
	Vector H;
	v = 1- (acos(PRODUCTO_PUNTO(Normal,esf.Norte)))/(PI);
	
	denominador = esf.Norte.dire.x * -esf.Norte.dire.x;
	denominador += esf.Norte.dire.y * -esf.Norte.dire.y;
	denominador += esf.Norte.dire.z * -esf.Norte.dire.z;
	t = esf.Norte.dire.x  * pinter.x + esf.Norte.dire.y  * pinter.y + 
		 esf.Norte.dire.z * pinter.z + esf.Dnorte;
    t *=-1.0;
    
    t /= denominador;
    
    pinter2.x= pinter.x + t * -esf.Norte.dire.x;
    pinter2.y= pinter.y + t * -esf.Norte.dire.y;
    pinter2.z= pinter.z + t * -esf.Norte.dire.z;
    
    H = CREAR_VECTOR(esf.centro,pinter2);
    Normalizar(&H);
    u = acosl(PRODUCTO_PUNTO(H,esf.G));
    u /= (2*PI);
    
    if (esf.Correcto.dire.x*pinter.x 
		+ esf.Correcto.dire.y*pinter.y 
		+ esf.Correcto.dire.z*pinter.z 
		+ esf.DCorrecto > 0.0){
			u = 1 - u;
	}
    
	v = MIN(v,1.0);
	v = MAX(v,0.0);
	u = MIN(u,1.0);
	u = MAX(u,0.0);
	
	*i = roundl( u * escena->Texturas[tex].resX);
	*j = roundl( v * escena->Texturas[tex].resY);
	if (*i<0)
		(*i)*=-1;
	if (*j<0)
		(*j)*=-1;
}
void posicionTexturaCilindro(Cilindro cil, int tex ,Punto pinter,Escena *escena, int *i, int *j, Vector Normal){
	long double u,v;
	Vector vec;
	vec = CREAR_VECTOR(cil.rayo.ancla,pinter);
	//Normalizar(&vec);
	v = PRODUCTO_PUNTO(vec,cil.rayo.vector) - cil.h1;
//	v1 = v;
	v /= cil.h2 - cil.h1;
	
	
	u=PRODUCTO_PUNTO(Normal,cil.G);
	u = acosl(u);
	
	u /= (2 * PI);
	
	
	v = MIN(v,1.0);
	v = MAX(v,0.0);
	u = MIN(u,1.0);
	u = MAX(u,0.0);
	if (cil.Correcto.dire.x*pinter.x 
		+ cil.Correcto.dire.y*pinter.y 
		+ cil.Correcto.dire.z*pinter.z 
		+ cil.D > 0.0){
			u = 1 - u;
	}
	
	*i = roundl( u * escena->Texturas[tex].resX);
	*j = roundl( v * escena->Texturas[tex].resY);
	if (*i<0)
		(*i)*=-1;
	if (*j<0)
		(*j)*=-1;
	
	if (*i> escena->Texturas[tex].resX || *j> escena->Texturas[tex].resY){
		printf("\n\ncilindro %d: %d %d %d %d\n",tex,escena->Texturas[tex].resX,escena->Texturas[tex].resY, *i, *j);
	}
}


Punto Bump(int text, int i, int j,Escena *escena, Punto pint, 
           long double maxBump,Vector *normal,Vector U, Vector V, long double Tu, long double Tv){	
			   
	long double HM = escena->Texturas[text].ima[MAX(0,j-1)][MAX(0,i-1)].r;
	long resX, resY;
	long double Ku, Kv;
	HM /= 255.0;
	HM *= maxBump;
	pint.x += HM * normal->dire.x;
	pint.y += HM * normal->dire.y;
	pint.z += HM * normal->dire.z;
	
	resX = escena->Texturas[text].resX;
	resY = escena->Texturas[text].resY;
	
	i = MAX(1,i-1);
	i = MIN(i,resX-2);
	
	j = MAX(1,j-1);
	j = MIN(j,resY-2);
	
	Ku = -1 * (escena->Texturas[text].ima[j][i+1].r - escena->Texturas[text].ima[j][i-1].r)/2.0;
	Ku *= Tu;
	
	
	Kv = -1 * (escena->Texturas[text].ima[j+1][i].r - escena->Texturas[text].ima[j-1][i].r)/2.0;
	Kv *= Tv;
	//printf("KU KV = %LF %LF, %d %d\n",Ku,Kv,i,j);
	(*normal).dire.x += U.dire.x * Ku + V.dire.x * Kv;
	(*normal).dire.y += U.dire.y * Ku + V.dire.y * Kv;
	(*normal).dire.z += U.dire.z * Ku + V.dire.z * Kv;
	return pint;
}

Color Textura(int text, int i, int j,Escena *escena){	
	
	return escena->Texturas[text].ima[MAX(0,j-1)][MAX(0,i-1)];
}

int Calado(int text, int i, int j,Escena *escena){	
	Color col = escena->Texturas[text].ima[MAX(0,j-1)][MAX(0,i-1)];
	return (col.r > 0.5 && col.g > 0.5 && col.b > 0.5);
}

Interseccion *Primera_Interseccion(Rayo ray,Escena *escena, int *num){
	Interseccion *inter_m;
	Interseccion inter;
	long double t;
	int i,conta;
	inter_m = NULL;
	conta = 0;
	for (i=0;i<escena->numEsferas; i++){
		inter = InterEsfera(ray,&t,escena->Esferas[i],escena);
		if (t!=INFINITY && t>EPSILON){
			conta++;
			inter_m = realloc(inter_m,conta*sizeof(Interseccion));

			inter.objeto.obj = &escena->Esferas[i];
			inter.objeto.tipo = ESFERA;
			inter.objeto.Normal = NormalEsfera;
			inter.objeto.Kd = escena->Esferas[i].Kd;
			inter.objeto.Ka = escena->Esferas[i].Ka;
			inter.objeto.Ks = escena->Esferas[i].Ks;
			inter.objeto.Kn = escena->Esferas[i].Kn;
			inter.objeto.Kt = escena->Esferas[i].Kt;
			inter.objeto.o1 = escena->Esferas[i].o1;
			inter.objeto.o2 = escena->Esferas[i].o2;
			inter.objeto.o3 = escena->Esferas[i].o3;
			inter.objeto.bump = escena->Esferas[i].bump;
			inter.objeto.textura = escena->Esferas[i].textura;
			inter_m[conta-1] = inter;
		}
	}
	for (i=0;i<escena->numCilindros; i++){
		inter = InterCilindro(ray,&t,escena->Cilindros[i],escena);
		if (t!=INFINITY && t>EPSILON){
			conta++;
			inter_m = realloc(inter_m,conta*sizeof(Interseccion));
			inter.Pint.x = ray.ancla.x + t * ray.vector.dire.x;
			inter.Pint.y = ray.ancla.y + t * ray.vector.dire.y;
			inter.Pint.z = ray.ancla.z + t * ray.vector.dire.z;
			inter.objeto.obj = &escena->Cilindros[i];
			inter.objeto.tipo = CILINDRO;
			inter.objeto.Normal = NormalCilindro;
			inter.objeto.d = inter.d;
			inter.objeto.Kd = escena->Cilindros[i].Kd;
			inter.objeto.Ka = escena->Cilindros[i].Ka;
			inter.objeto.Ks = escena->Cilindros[i].Ks;
			inter.objeto.Kn = escena->Cilindros[i].Kn;
			inter.objeto.Kt = escena->Cilindros[i].Kt;
			inter.objeto.o1 = escena->Cilindros[i].o1;
			inter.objeto.o2 = escena->Cilindros[i].o2;
			inter.objeto.o3 = escena->Cilindros[i].o3;
			inter.objeto.bump = escena->Cilindros[i].bump;
			inter.objeto.textura = escena->Cilindros[i].textura;
			inter_m[conta-1] = inter;
		}
	}
	for (i=0;i<escena->numConos; i++){
		inter = InterCono(ray,&t,escena->Conos[i],escena);
		if (t!=INFINITY && t>EPSILON){
			conta++;
			inter_m = realloc(inter_m,conta*sizeof(Interseccion));
			inter.objeto.obj = &escena->Conos[i];
			inter.objeto.tipo = CONO;
			inter.objeto.Normal = NormalCono;
			inter.objeto.d = inter.d;
			inter.objeto.Kd = escena->Conos[i].Kd;
			inter.objeto.Ka = escena->Conos[i].Ka;
			inter.objeto.Ks = escena->Conos[i].Ks;
			inter.objeto.Kn = escena->Conos[i].Kn;
			inter.objeto.Kt = escena->Conos[i].Kt;
			inter.objeto.o1 = escena->Conos[i].o1;
			inter.objeto.o2 = escena->Conos[i].o2;
			inter.objeto.o3 = escena->Conos[i].o3;
			inter.objeto.bump = -1;
			inter.objeto.textura = -1;
			inter_m[conta-1] = inter;
		}
	}
	for (i=0;i<escena->numPoligonos; i++){
		int pega = 1;
		inter = InterPoligono(ray,&t,escena->Poligonos[i],escena);
		if (t!=INFINITY && t>EPSILON){
			int ii, jj;
			
			//Texturas normales
			if (escena->Poligonos[i].textura != -1){
				posicionTexturaPoligono(escena->Poligonos[i],escena->Poligonos[i].textura,inter.Pint,escena,&ii,&jj);
				inter.objeto.color = Textura(escena->Poligonos[i].textura,ii,jj,escena);
			}
			
			//Mapas de Calado
			if (escena->Poligonos[i].calado != -1){
				posicionTexturaPoligono(escena->Poligonos[i],escena->Poligonos[i].calado,inter.Pint,escena,&ii,&jj);
				pega = Calado(escena->Poligonos[i].calado,ii,jj,escena);
			}
			
			if (pega){
				conta++;
				inter_m = realloc(inter_m,conta*sizeof(Interseccion));
				inter.objeto.obj = &escena->Poligonos[i];
				inter.objeto.tipo = POLIGONO;
				inter.objeto.Normal = NormalPoligono;
				inter.objeto.Kd = escena->Poligonos[i].Kd;
				inter.objeto.Ka = escena->Poligonos[i].Ka;
				inter.objeto.Ks = escena->Poligonos[i].Ks;
				inter.objeto.Kn = escena->Poligonos[i].Kn;
				inter.objeto.Kt = escena->Poligonos[i].Kt;
				inter.objeto.o1 = escena->Poligonos[i].o1;
				inter.objeto.o2 = escena->Poligonos[i].o2;
				inter.objeto.o3 = escena->Poligonos[i].o3;
				inter.objeto.bump = escena->Poligonos[i].bump;
				inter.objeto.maxBump = escena->Poligonos[i].maxBump;
				inter.objeto.textura = -1;
				
				inter_m[conta-1] = inter;
			}
		}
	}
	for (i=0;i<escena->numCuadraticas; i++){
		inter = InterCuadratica(ray,&t,escena->Cuadraticas[i],escena);
		if (t!=INFINITY && t>EPSILON){
			conta++;
			inter_m = realloc(inter_m,conta*sizeof(Interseccion));
			inter.objeto.obj = &escena->Cuadraticas[i];
			inter.objeto.tipo = CUADRATICA;
			inter.objeto.Normal = NormalCuadratica;
			inter.objeto.Kd = escena->Cuadraticas[i].Kd;
			inter.objeto.Ka = escena->Cuadraticas[i].Ka;
			inter.objeto.Ks = escena->Cuadraticas[i].Ks;
			inter.objeto.Kn = escena->Cuadraticas[i].Kn;
			inter.objeto.Kt = escena->Cuadraticas[i].Kt;
			inter.objeto.o1 = escena->Cuadraticas[i].o1;
			inter.objeto.o2 = escena->Cuadraticas[i].o2;
			inter.objeto.o3 = escena->Cuadraticas[i].o3;
			inter.objeto.bump = -1;
			inter.objeto.textura = -1;
			
			inter_m[conta-1] = inter;
		}
	}
	
	
	Quicksort(inter_m,0,conta-1);
	*num = conta;
	return inter_m;
}


