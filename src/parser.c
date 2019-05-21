/* ************ PARSER.C *************************
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
 #include <parser.h>
 #include <stdio.h>
 #include <math.h>
 
void liberarEscena(Escena *escena){
	
	int i;
	
	
	for (i=0;i<escena->numPoligonos;i++){ 
		free(escena->Poligonos[i].puntos);
		free(escena->Poligonos[i].planos_corte);
	}
	for (i=0;i<escena->numEsferas;i++){ 
		free(escena->Esferas[i].planos_corte);
	}
	for (i=0;i<escena->numCilindros;i++){ 
		free(escena->Cilindros[i].planos_corte);
	} 
	for (i=0;i<escena->numConos;i++){ 
		free(escena->Conos[i].planos_corte);
	} 
	for (i=0;i<escena->numCuadraticas;i++){ 
		free(escena->Cuadraticas[i].planos_corte);
	} 
	if (escena->luces!=NULL) 
		free(escena->luces);
	if (escena->Esferas!=NULL) 
		free(escena->Esferas);
	if (escena->Poligonos!=NULL) 
		free(escena->Poligonos);
	if (escena->Cilindros!=NULL) 
		free(escena->Cilindros);
	if (escena->Conos!=NULL) 	
		free(escena->Conos);
	if (escena->Cuadraticas!=NULL) 	
		free(escena->Cuadraticas);
	if (escena->Texturas!=NULL) 	
		free(escena->Texturas);
	if (escena->Planos!=NULL)
		free(escena->Planos);
	free(escena);
 }
 
void aplastarPoligonos(Escena *escena){
	
	int i,j;
	long double mayor=-INFINITY;
	long double A,B,C;
	
	for (i=0;i<escena->numPoligonos;i++){
		
		if (escena->Poligonos[i].Normal.dire.x < 0){
			A = -escena->Poligonos[i].Normal.dire.x;
		}else{
			A = escena->Poligonos[i].Normal.dire.x;
		}
		if (escena->Poligonos[i].Normal.dire.y < 0){
			B = -escena->Poligonos[i].Normal.dire.y;
		}else{
			B = escena->Poligonos[i].Normal.dire.y;
		}
		if (escena->Poligonos[i].Normal.dire.z < 0){
			C = -escena->Poligonos[i].Normal.dire.z;
		}else{
			C = escena->Poligonos[i].Normal.dire.z;
		}
		
		//Mayor valor absoluto
		if (A > B){
			mayor = A;
		}else
			mayor = B;
		if (mayor < C)
			mayor = C;
			
			
		for (j=0;j<escena->Poligonos[i].numPuntos;j++){
			if (mayor == A){
				escena->Poligonos[i].puntos[j].x=0;
				escena->Poligonos[i].aplastado = X;
			}else if (mayor == B){
				escena->Poligonos[i].puntos[j].y=0;
				escena->Poligonos[i].aplastado = Y;
			}else{
				escena->Poligonos[i].puntos[j].z=0;
				escena->Poligonos[i].aplastado = Z;
			}
		}
		for (j=0;j<escena->Poligonos[i].numPuntos;j++){
			if (mayor == A){
				escena->Poligonos[i].puntos[j].x = escena->Poligonos[i].puntos[j].y;
				escena->Poligonos[i].puntos[j].y = escena->Poligonos[i].puntos[j].z;
				escena->Poligonos[i].puntos[j].z = 0;
			}else if (mayor == B){
				escena->Poligonos[i].puntos[j].y = escena->Poligonos[i].puntos[j].z;
				escena->Poligonos[i].puntos[j].z = 0;
			}else{
				escena->Poligonos[i].puntos[j].z=0;
			}
		}
		
	}
	
}
 

 char NextChar(FILE *file){
	 char caracter;
	 caracter = (char)getc(file);
	 while ((caracter== ' ' && leeespacios==1) || caracter== 10 || caracter== 13 || caracter == 9){ //ignorar los espacios y saltos de linea
		caracter = (char)getc(file);
	 }
	 if (caracter == '!'){ //Comentarios :)
		caracter = (char)getc(file);
		while (caracter!='!' &&  caracter!=EOF){
			caracter = (char)getc(file);
		}
		caracter = NextChar(file);
	}
	 return caracter;
 }
 int equal(int tam, char *s1, char *s2){
	int i;
	for (i=0; i<tam; i++){
		if (i<strlen(s2)){
			if (s1[i] != s2[i])
				return 0;
		}else
			return 0;
	}
	return tam==strlen(s2);
 }
 int NextToken(FILE *file){
	 char *token;
	 int i;
	 int salir;
	 char c;
	 int res;
	 i = 0;
	 salir = 1;
	 res = -1;
	 token = NULL;
	 while (salir){
		token = realloc(token,(i+1)*sizeof(char));
		c = NextChar(file);
		token[i] = c;
		
		
			i++;
		
			if (equal(i,token,"Escena")){
				res = ESCENA;
			}
			if (equal(i,token,"Esfera")){
				res = ESFERA;
			}
			if (equal(i,token,"x")){
				res = X;
			}
			if (equal(i,token,"y")){
				res = Y;
			}
			if (equal(i,token,"z")){
				res = Z;
			}
			if (equal(i,token,"Radio")){
				res = RADIO;
			}
			if (equal(i,token,"Color")){
				res = COLOR_T;
			}
			if (equal(i,token,"{")){
				res = CORCHETE_INI;
			}
			if (equal(i,token,"}")){
				res = CORCHETE_FIN;
			}
			if (equal(i,token,"=")){
				res = IGUAL;
			}
			if (equal(i,token,",")){
				res = COMA;
			}
			if (equal(i,token,"ResX")){
				res = RESX;
			}
			if (equal(i,token,"ResY")){
				res = RESY;
			}
			if (equal(i,token,"IluminacionAmbiente")){
				res = LUZAMBIENTE;
			}
			if (equal(i,token,"c1")){
				res = C1;
			}
			if (equal(i,token,"c2")){
				res = C2;
			}
			if (equal(i,token,"c3")){
				res = C3;
			}
			if (equal(i,token,"Luz")){
				res = LUZ;
			}
			if (equal(i,token,"Intensidad")){
				res = INTENSIDAD;
			}
			if (equal(i,token,"Kmaterial")){
				res = KMATERIAL;
			}
			if (equal(i,token,"Ka")){
				res = KA;
			}
			if (equal(i,token,"Ks")){
				res = KS;
			}
			if (equal(i,token,"OjoX")){
				res = OJOX;
			}
			if (equal(i,token,"OjoY")){
				res = OJOY;
			}
			if (equal(i,token,"OjoZ")){
				res = OJOZ;
			}
			if (equal(i,token,"Background")){
				res = BACKGROUND;
			}
			if (equal(i,token,"Xmin")){
				res = XMIN;
			}
			if (equal(i,token,"Ymin")){
				res = YMIN;
			}
			if (equal(i,token,"Xmax")){
				res = XMAX;
			}
			if (equal(i,token,"Ymax")){
				res = YMAX;
			}
			if (equal(i,token,"Ks")){
				res = KS;
			}
			if (equal(i,token,"Kn")){
				res = KN;
			}
			if (equal(i,token,"Poligono")){
				res = POLIGONO;
			}
			if (equal(i,token,"Vertice")){
				res = VERTICE;
			}
			if (equal(i,token,"Cilindro")){
				res = CILINDRO;
			}
			if (equal(i,token,"Ancla")){
				res = ANCLA;
			}
			if (equal(i,token,"Direccion")){
				res = DIRECCION;
			}
			if (equal(i,token,"H1")){
				res = H1;
			}
			if (equal(i,token,"H2")){
				res = H2;
			}
			if (equal(i,token,"Esf_Cil_Esf")){
				res = ESF_CIL_ESF;
			}
			if (equal(i,token,"Cono")){
				res = CONO;
			}
			if (equal(i,token,"Ucono")){
				res = UTOKEN;
			}
			if (equal(i,token,"Vcono")){
				res = VTOKEN;
			}
			if (equal(i,token,"Cortes")){
				res = CORTES;
			}
			if (equal(i,token,"Plano")){
				res = PLANO;
			}
			if (equal(i,token,"Cuadratica")){
				res = CUADRATICA;
			}
			if (equal(i,token,"ABCDEFGHJK")){
				res = ABCDEFGHJK;
			}
			if (equal(i,token,"Espejo")){
				res = ESPEJO;
			}
			if (equal(i,token,"O1")){
				res = O1;
			}
			if (equal(i,token,"O2")){
				res = O2;
			}
			if (equal(i,token,"Antialiasing")){
				res = ANTIALIASING;
			}
			if (equal(i,token,"Transparencia")){
				res = TRANSPARENCIA;
			}
			if (equal(i,token,"O3")){
				res = O3;
			}
			if (equal(i,token,"Kt")){
				res = KT;
			}
			if (equal(i,token,"Textura")){
				res = TEXTURA;
			}
			if (equal(i,token,"Calado")){
				res = CALADO;
			}
			if (equal(i,token,"Bump")){
				res = BUMP;
			}
			if (equal(i,token,"Tu")){
				res = TU;
			}
			if (equal(i,token,"Tv")){
				res = TV;
			}
			if (equal(i,token,"Disco")){
				res = DISCO;
			}
			if (equal(i,token,"Norte")){
				res = NORTE;
			}
			if (res!=-1)
				salir = 0;
	 }
	 //printf("%s\n",token);
	 free(token);
	 return res;
 }
 
 
 int esperaToken(int token,FILE *file){
	 int tok;
	 char *st;
	 
	 tok = NextToken(file);
	
	 if (tok != token){
		 printf("Error Se esperaba: ");
		 switch (token){
			case ESCENA:
				st = "Escena";
				break;
			case ESFERA:
				st = "Esfera";
				break;
			case CORCHETE_INI:
				st = "{";
				break;
			case CORCHETE_FIN:
				st = "}";
				break;
			case PUNTO_COMA:
				st = ";";
				break;
			case X:
				st = "x";
				break;
			case Y:
				st = "y";
				break;
			case Z:
				st = "z";
				break;
			case RADIO:
				st = "Radio";
				break;
			case COLOR_T:
				st = "Color";
				break;
			case IGUAL:
				st = "=";
				break;
			case COMA:
				st = ",";
				break;
			case RESX:
				st = "ResX";
				break;
			case RESY:
				st = "ResY";
				break;
			case LUZAMBIENTE:
				st = "IluminacionAmbiente";
				break;
			case C1:
				st = "c1";
				break;
			case C2:
				st = "c2";
				break;
			case C3:
				st = "c3";
				break;
			case LUZ:
				st = "Luz";
				break;
			case INTENSIDAD:
				st = "Intensidad";
				break;
			case KMATERIAL:
				st = "Kmaterial";
				break;
			case KA:
				st = "Ka";
				break;
			case KS:
				st = "Ks";
				break;
			case KN:
				st = "Kn";
				break;
			case OJOX:
				st = "OjoX";
				break;
			case OJOY:
				st = "OjoY";
				break;
			case OJOZ:
				st = "OjoZ";
				break;
			case BACKGROUND:
				st = "Background";
				break;
			case XMIN:
				st = "Xmin";
				break;
			case YMIN:
				st = "Ymin";
				break;
			case XMAX:
				st = "Xmax";
				break;
			case YMAX:
				st = "Ymax";
				break;
			case POLIGONO:
				st = "Poligono";
				break;
			case VERTICE:
				st = "Vertice";
				break;
			case CILINDRO:
				st = "Cilindro";
				break;
			case ANCLA:
				st = "Ancla";
				break;
			case DIRECCION:
				st = "Direccion";
				break;
			case H1:
				st = "H1";
				break;
			case H2:
				st = "H2";
				break;
			case ESF_CIL_ESF:
				st = "Esf_Cil_Esf";
				break;	
			case CONO:
				st = "Cono";
				break;	
			case UTOKEN:
				st = "Ucono";
				break;	
			case VTOKEN:
				st = "Vcono";
				break;	
			case CORTES:
				st = "Cortes";
				break;	
			case PLANO:
				st = "Plano";
				break;	
			case CUADRATICA:
				st = "Cuadratica";
				break;	
			case ABCDEFGHJK:
				st = "ABCDEFGHJK";
				break;	
			case ESPEJO:
				st = "ESPEJO";
				break;	
			case O1:
				st = "O1";
				break;
			case O2:
				st = "O2";
				break;
			case ANTIALIASING:
				st = "Antialiasing";
				break;
			case O3:
				st = "O3";
				break;
			case KT:
				st = "Kt";
				break;
			case TRANSPARENCIA:
				st = "Transparencia";
				break;
			case TEXTURA:
				st = "Textura";
				break;
			case CALADO:
				st = "Calado";
				break;
			case BUMP:
				st = "Bump";
				break;
			case TU:
				st = "Tu";
				break;
			case TV:
				st = "Tv";
				break;
			case DISCO:
				st = "Disco";
				break;
			case NORTE:
				st = "Norte";
				break;
		 }
		 printf("'%s'\n",st);
		 return 0;
	 }
	 return 1;
 }
char *obtenerTexto(FILE *file){
	 int i;
	 char *ss;
	 char c;
	 
	 c = NextChar(file);
	 ss=NULL;
	 i=0;
	 while((c==' ' && leeespacios == 0))
		c = NextChar(file);
	 while (c!=',' && c!=';'){
		 i++;
		 ss = realloc(ss,i*sizeof(char));
		 ss[i-1] = c;
		 c = NextChar(file);
	 }
	 i++;
	 ss = realloc(ss,i*sizeof(char));
	 ss[i-1] = '\0';
	 
	 return ss;
 } 
long double obtenerNumero(FILE *file){
	 int i;
	 char *ss;
	 char c;
	 double num;
	 
	 c = NextChar(file);
	 ss=NULL;
	 i=0;
	 while (c!=',' && c!=';'){
		 i++;
		 ss = realloc(ss,i*sizeof(char));
		 ss[i-1] = c;
		 c = NextChar(file);
	 }
	 i++;
	 ss = realloc(ss,i*sizeof(char));
	 ss[i-1] = 0;
	 num = atof(ss);
	 free(ss);
	 return (long double)num;
 }
 
 Escena *parseEscena(char *arch){
	 
	 FILE *file;
	 int token,i;
	 Escena *escena;
	 int esf,luz,poli,cil,con,plan,cua,tex;
	 char *str;
	 Vector v1,v2;
	 Punto *puntos;
	 Punto punto,punto2,punto3;
	 file = fopen(arch,"r");
	 escena = malloc(sizeof(Escena));
	 escena->numEsferas = 0;
	 escena->numCilindros = 0;
	 escena->numConos = 0;
	 escena->numPoligonos = 0;
	 escena->numLuces = 0;
	 escena->numTexturas = 0;
	 escena->luces = NULL;
	 escena->Esferas = NULL;
	 leeespacios = 1;
	 token = NextToken(file);
	 if (token == ESCENA){    
		 if (!esperaToken(CORCHETE_INI,file)){
			 free(escena);
			 fclose(file);
			 return NULL;
		 }
		 
		while ((token = NextToken(file)) != CORCHETE_FIN)
		{
	//****************** RESX *****************************************
			if (token == RESX){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->resX = (int)roundl(obtenerNumero(file));
	//*****************************************************************
	//***************** RESY ******************************************
			}else if (token == RESY){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->resY = (int)roundl(obtenerNumero(file));
	//*****************************************************************
	//***************** XMIN ******************************************
			}else if (token == XMIN){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->Xmin = (int)roundl(obtenerNumero(file));
	//*****************************************************************
	//***************** YMIN ******************************************
			}else if (token == YMIN){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->Ymin = (int)roundl(obtenerNumero(file));
	//*****************************************************************
	//***************** XMAX ******************************************
			 }else if (token == XMAX){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->Xmax = (int)roundl(obtenerNumero(file));
	//*****************************************************************
	//***************** YMAX ******************************************
			 }else if (token == YMAX){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->Ymax = (int)roundl(obtenerNumero(file));
	//*****************************************************************
	//***************** LUZAMBIENTE ***********************************
			}else if (token == LUZAMBIENTE){  
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->Ia = obtenerNumero(file);
	//*****************************************************************
	//***************** OJOX ******************************************
			 }else if (token == OJOX){  
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->Ojo.x = obtenerNumero(file);
	//*****************************************************************
	//***************** OJOY ******************************************
			 }else if (token == OJOY){  
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
				 return NULL;
				}
				escena->Ojo.y = obtenerNumero(file);
	//*****************************************************************
	//***************** OJOZ ******************************************
			 }else if (token == OJOZ){  
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
				 return NULL;
				}
				escena->Ojo.z = obtenerNumero(file);
    //*****************************************************************
	//***************** BACKGROUND*************************************
			 }else if (token == BACKGROUND){  
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->Background.a = obtenerNumero(file);
				escena->Background.r = obtenerNumero(file);
				escena->Background.g = obtenerNumero(file);
				escena->Background.b = obtenerNumero(file);
	//*****************************************************************
	//***************** ESPEJO ******************************************
			}else if (token == ESPEJO){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->nivelEspejo = (int)roundl(obtenerNumero(file));
	//*****************************************************************
	//***************** ANTIALIASING ******************************************
			}else if (token == ANTIALIASING){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->nivelAntialiasing = (int)roundl(obtenerNumero(file));
	//*****************************************************************
	//***************** TRANSPARENCIA**********************************
			}else if (token == TRANSPARENCIA){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				escena->nivelTransparencia = (int)roundl(obtenerNumero(file));
	//*****************************************************************
	//***************** TEXTURAS **************************************
			}else if (token == TEXTURA){
				if (!esperaToken(IGUAL,file)){
					free(escena);
					fclose(file);
					return NULL;
				}
				leeespacios = 0;
				escena->numTexturas++;
				tex = escena->numTexturas;
				escena->Texturas = realloc(escena->Texturas,tex*sizeof(Imagen));
				str = obtenerTexto(file);
				escena->Texturas[tex-1] = leerTextura(str);
				leeespacios = 1;
				escena->Texturas[tex-1].mosaicoX = obtenerNumero(file);
				escena->Texturas[tex-1].mosaicoY = obtenerNumero(file);
				free(str);
	//*****************************************************************
	//********************* PLANO ************************************
			 }else if (token == PLANO){
				escena->numPlanos++;
				plan = escena->numPlanos;
				escena->Planos = realloc(escena->Planos,plan*sizeof(Plano));
				
							// '{'
				if (!esperaToken(CORCHETE_INI,file))
				{ 
					free(escena->Planos);
					free(escena);
					fclose(file);
					
				 return NULL;
				}
						// 'Vertice'
				VALIDA(escena,VERTICE,file);
				punto.x = obtenerNumero(file);
				punto.y = obtenerNumero(file);
				punto.z = obtenerNumero(file);
				VALIDA(escena,VERTICE,file);
				punto2.x = obtenerNumero(file);
				punto2.y = obtenerNumero(file);
				punto2.z = obtenerNumero(file);
				VALIDA(escena,VERTICE,file);
				punto3.x = obtenerNumero(file);
				punto3.y = obtenerNumero(file);
				punto3.z = obtenerNumero(file);
				
				
				v1 = CREAR_VECTOR(punto,punto2);
				v2 = CREAR_VECTOR(punto,punto3);
				
				escena->Planos[plan-1].ABC = PRODUCTO_CRUZ(v1,v2);
				
				//long double norma = Normalizar(&(escena->Planos[plan-1].ABC));
				escena->Planos[plan-1].D = escena->Planos[plan-1].ABC.dire.x * punto.x;
				escena->Planos[plan-1].D += escena->Planos[plan-1].ABC.dire.y * punto.y;
				escena->Planos[plan-1].D += escena->Planos[plan-1].ABC.dire.z * punto.z;
				//escena->Planos[plan-1].D /=norma;
				escena->Planos[plan-1].D *=-1;
				
				
				
				//PARA INVERTIR LA NORMAL
				VALIDA(escena,VERTICE,file);
				i = roundl(obtenerNumero(file));
				escena->Planos[plan-1].dire = i;
				
				if (!esperaToken(CORCHETE_FIN,file))
				{
					free(escena->Planos);
					free(escena);
					fclose(file);
					return NULL;
				}
	//*****************************************************************
	//********************* ESFERA ************************************
			 }else if (token == ESFERA)
			 {
				 escena->numEsferas++;
				 esf = escena->numEsferas;
				 escena->Esferas = realloc(escena->Esferas,esf*sizeof(Esfera));
							 // '{'
				if (!esperaToken(CORCHETE_INI,file))
				{ 
					free(escena->Esferas);
					free(escena);
					fclose(file);
					
				 return NULL;
				}
				
							// 'x'
				VALIDA(escena,X,file);
				escena->Esferas[esf-1].centro.x = obtenerNumero(file);
				
							// 'y'
				VALIDA(escena,Y,file);
				escena->Esferas[esf-1].centro.y = obtenerNumero(file);
				
							// 'z'
				VALIDA(escena,Z,file);
				escena->Esferas[esf-1].centro.z = obtenerNumero(file);
				
							// 'Radio'
				VALIDA(escena,RADIO,file);
				escena->Esferas[esf-1].radio = obtenerNumero(file);
				escena->Esferas[esf-1].radio2 = escena->Esferas[esf-1].radio * 
												escena->Esferas[esf-1].radio;
							// 'Color'
				VALIDA(escena,COLOR_T,file);
				escena->Esferas[esf-1].color.a = obtenerNumero(file);
				escena->Esferas[esf-1].color.r = obtenerNumero(file);
				escena->Esferas[esf-1].color.g = obtenerNumero(file);
				escena->Esferas[esf-1].color.b = obtenerNumero(file);
				
							// 'Kmaterial'
				VALIDA(escena,KMATERIAL,file);
				escena->Esferas[esf-1].Kd = obtenerNumero(file);
							
							// 'Ka'
				VALIDA(escena,KA,file);
				escena->Esferas[esf-1].Ka = obtenerNumero(file);
				
							// 'Ks'
				VALIDA(escena,KS,file);
				escena->Esferas[esf-1].Ks = obtenerNumero(file);
				
							// 'Kn'
				VALIDA(escena,KN,file);
				escena->Esferas[esf-1].Kn = obtenerNumero(file);
				
							// 'Kt'
				VALIDA(escena,KT,file);
				escena->Esferas[esf-1].Kt = obtenerNumero(file);
				
							// 'O1'
				VALIDA(escena,O1,file);
				escena->Esferas[esf-1].o1 = obtenerNumero(file);
				
							// 'O2'
				VALIDA(escena,O2,file);
				escena->Esferas[esf-1].o2 = obtenerNumero(file);
				
							// 'O3'
				VALIDA(escena,O3,file);
				escena->Esferas[esf-1].o3 = obtenerNumero(file);
				
							
				
							// 'Cortes'
				VALIDA(escena,CORTES,file);
				escena->Esferas[esf-1].cortes = obtenerNumero(file);
				escena->Esferas[esf-1].planos_corte = 
						malloc(escena->Esferas[esf-1].cortes * sizeof(int));
						
				if (escena->Esferas[esf-1].cortes>0){
				VALIDA(escena,CORTES,file);
				for (i=0;i<escena->Esferas[esf-1].cortes;i++){
					escena->Esferas[esf-1].planos_corte[i] = roundl(obtenerNumero(file));
				}
				}
				
							// 'Textura'
				VALIDA(escena,TEXTURA,file);
				escena->Esferas[esf-1].textura = obtenerNumero(file);
							// 'Calado'
				VALIDA(escena,CALADO,file);
				escena->Esferas[esf-1].calado = obtenerNumero(file);
							// 'Bump'
				VALIDA(escena,BUMP,file);
				escena->Esferas[esf-1].bump = obtenerNumero(file);
				
				if (escena->Esferas[esf-1].bump != -1){
					escena->Esferas[esf-1].maxbump = obtenerNumero(file);
								// 'TU'
					VALIDA(escena,TU,file);
					escena->Esferas[esf-1].tu = obtenerNumero(file);
								// 'TV'
					VALIDA(escena,TV,file);
					escena->Esferas[esf-1].tv = obtenerNumero(file);
					
					
				}
				
				if (escena->Esferas[esf-1].textura != -1 || escena->Esferas[esf-1].bump != -1
				  || escena->Esferas[esf-1].calado != -1){
							// 'Norte'
					VALIDA(escena,NORTE,file);
					escena->Esferas[esf-1].Norte.dire.x = obtenerNumero(file);
					escena->Esferas[esf-1].Norte.dire.y = obtenerNumero(file);
					escena->Esferas[esf-1].Norte.dire.z = obtenerNumero(file);
					Normalizar(&escena->Esferas[esf-1].Norte);
					
								// 'Direccion'
					VALIDA(escena,DIRECCION,file);
					escena->Esferas[esf-1].G.dire.x = obtenerNumero(file);
					escena->Esferas[esf-1].G.dire.y = obtenerNumero(file);
					escena->Esferas[esf-1].G.dire.z = obtenerNumero(file);
					
					escena->Esferas[esf-1].G = 
					PRODUCTO_CRUZ(
						PRODUCTO_CRUZ(escena->Esferas[esf-1].G,escena->Esferas[esf-1].Norte)
						,escena->Esferas[esf-1].Norte);
					Normalizar(&escena->Esferas[esf-1].G);
					
					escena->Esferas[esf-1].Correcto = 
							PRODUCTO_CRUZ(escena->Esferas[esf-1].G
										 ,escena->Esferas[esf-1].Norte);
					
					escena->Esferas[esf-1].DCorrecto = 
								escena->Esferas[esf-1].Correcto.dire.x 
								* escena->Esferas[esf-1].centro.x;
					escena->Esferas[esf-1].DCorrecto += 
								escena->Esferas[esf-1].Correcto.dire.y 
								* escena->Esferas[esf-1].centro.y;
					escena->Esferas[esf-1].DCorrecto += 
								escena->Esferas[esf-1].Correcto.dire.z 
								* escena->Esferas[esf-1].centro.z;
					escena->Esferas[esf-1].DCorrecto *= -1;
					
					escena->Esferas[esf-1].Dnorte = escena->Esferas[esf-1].Norte.dire.x * escena->Esferas[esf-1].centro.x;
					escena->Esferas[esf-1].Dnorte += escena->Esferas[esf-1].Norte.dire.y * escena->Esferas[esf-1].centro.y;
					escena->Esferas[esf-1].Dnorte += escena->Esferas[esf-1].Norte.dire.z * escena->Esferas[esf-1].centro.z;
					escena->Esferas[esf-1].Dnorte *= -1;
				}
				
				
							// '}'
				if (!esperaToken(CORCHETE_FIN,file))
				{
					free(escena->Esferas);
					free(escena);
					fclose(file);
					return NULL;
				}
	//*****************************************************************
	//********************* CUADRATICA ********************************
			 }else if (token == CUADRATICA)
			 {
				 escena->numCuadraticas++;
				 cua = escena->numCuadraticas;
				 escena->Cuadraticas = realloc(escena->Cuadraticas,cua*sizeof(Cuadratica));
							 // '{'
				if (!esperaToken(CORCHETE_INI,file))
				{ 
					free(escena->Cuadraticas);
					free(escena);
					fclose(file);
					
				 return NULL;
				}
				
							// 'x'
				VALIDA(escena,ABCDEFGHJK,file);
				escena->Cuadraticas[cua-1].A = obtenerNumero(file);
				escena->Cuadraticas[cua-1].B = obtenerNumero(file);
				escena->Cuadraticas[cua-1].C = obtenerNumero(file);
				escena->Cuadraticas[cua-1].D = obtenerNumero(file);
				escena->Cuadraticas[cua-1].E = obtenerNumero(file);
				escena->Cuadraticas[cua-1].F = obtenerNumero(file);
				escena->Cuadraticas[cua-1].G = obtenerNumero(file);
				escena->Cuadraticas[cua-1].H = obtenerNumero(file);
				escena->Cuadraticas[cua-1].J = obtenerNumero(file);
				escena->Cuadraticas[cua-1].K = obtenerNumero(file);
				
							// 'Color'
				VALIDA(escena,COLOR_T,file);
				escena->Cuadraticas[cua-1].color.a = obtenerNumero(file);
				escena->Cuadraticas[cua-1].color.r = obtenerNumero(file);
				escena->Cuadraticas[cua-1].color.g = obtenerNumero(file);
				escena->Cuadraticas[cua-1].color.b = obtenerNumero(file);
				
							// 'Kmaterial'
				VALIDA(escena,KMATERIAL,file);
				escena->Cuadraticas[cua-1].Kd = obtenerNumero(file);
							
							// 'Ka'
				VALIDA(escena,KA,file);
				escena->Cuadraticas[cua-1].Ka = obtenerNumero(file);
				
							// 'Ks'
				VALIDA(escena,KS,file);
				escena->Cuadraticas[cua-1].Ks = obtenerNumero(file);
				
							// 'Kn'
				VALIDA(escena,KN,file);
				escena->Cuadraticas[cua-1].Kn = obtenerNumero(file);
				
							// 'Kt'
				VALIDA(escena,KT,file);
				escena->Cuadraticas[cua-1].Kt = obtenerNumero(file);
				
							// 'O1'
				VALIDA(escena,O1,file);
				escena->Cuadraticas[cua-1].o1 = obtenerNumero(file);
				
							// 'O2'
				VALIDA(escena,O2,file);
				escena->Cuadraticas[cua-1].o2 = obtenerNumero(file);
				
							// 'O3'
				VALIDA(escena,O3,file);
				escena->Cuadraticas[cua-1].o3 = obtenerNumero(file);
				
				
							// 'Cortes'
				VALIDA(escena,CORTES,file);
				escena->Cuadraticas[cua-1].cortes = obtenerNumero(file);
				escena->Cuadraticas[cua-1].planos_corte = 
						malloc(escena->Cuadraticas[cua-1].cortes * sizeof(int));
						
				if (escena->Cuadraticas[cua-1].cortes>0){
					VALIDA(escena,CORTES,file);
					for (i=0;i<escena->Cuadraticas[cua-1].cortes;i++){
						escena->Cuadraticas[cua-1].planos_corte[i] = roundl(obtenerNumero(file));
					}
				}
				
				
				
				if (!esperaToken(CORCHETE_FIN,file))
				{
					free(escena->Cuadraticas);
					free(escena);
					fclose(file);
					return NULL;
				}
	//******************************************************************
	//******** ESF_CIL_ESF *********************************************
		}else if (token == ESF_CIL_ESF){
				 escena->numEsferas+=2;
				 escena->numCilindros++;
				 esf = escena->numEsferas;
				 cil = escena->numCilindros;
				 
				 escena->Esferas = realloc(escena->Esferas,esf*sizeof(Esfera));
				 escena->Cilindros = realloc(escena->Cilindros,cil*sizeof(Cilindro));
							 // '{'
				if (!esperaToken(CORCHETE_INI,file))
				{ 
					free(escena->Esferas);
					free(escena->Cilindros);
					free(escena);
					fclose(file);
					
				 return NULL;
				}
							// 'x'
				VALIDA(escena,X,file);
				escena->Esferas[esf-1].centro.x = obtenerNumero(file);
				
							// 'y'
				VALIDA(escena,Y,file);
				escena->Esferas[esf-1].centro.y = obtenerNumero(file);
				
							// 'z'
				VALIDA(escena,Z,file);
				escena->Esferas[esf-1].centro.z = obtenerNumero(file);
				
				VALIDA(escena,X,file);
				escena->Esferas[esf-2].centro.x = obtenerNumero(file);
				
							// 'y'
				VALIDA(escena,Y,file);
				escena->Esferas[esf-2].centro.y = obtenerNumero(file);
				
							// 'z'
				VALIDA(escena,Z,file);
				escena->Esferas[esf-2].centro.z = obtenerNumero(file);
				
				
				
				//CILINDRO
				escena->Cilindros[cil-1].rayo.ancla = 
					escena->Esferas[esf-1].centro;
				escena->Cilindros[cil-1].rayo.vector = 
					CREAR_VECTOR(escena->Cilindros[cil-1].rayo.ancla, 
							escena->Esferas[esf-2].centro);
				escena->Cilindros[cil-1].h1 = 0;
				escena->Cilindros[cil-1].h2 = 
						Normalizar(&(escena->Cilindros[cil-1].rayo.vector));
				
				
				
							// 'Radio'
				VALIDA(escena,RADIO,file);
				escena->Esferas[esf-1].radio = obtenerNumero(file);
				escena->Esferas[esf-1].radio2 = escena->Esferas[esf-1].radio * 
												escena->Esferas[esf-1].radio;
							// 'Radio'		
				VALIDA(escena,RADIO,file);			
				escena->Esferas[esf-2].radio = obtenerNumero(file);
				escena->Esferas[esf-2].radio2 = escena->Esferas[esf-2].radio * 
												escena->Esferas[esf-2].radio;
												
				VALIDA(escena,RADIO,file);
				escena->Cilindros[cil-1].radio = obtenerNumero(file);
				escena->Cilindros[cil-1].radio2 = escena->Cilindros[cil-1].radio * 
												escena->Cilindros[cil-1].radio;
												
							// 'Color'
				VALIDA(escena,COLOR_T,file);
				escena->Esferas[esf-1].color.a = obtenerNumero(file);
				escena->Esferas[esf-1].color.r = obtenerNumero(file);
				escena->Esferas[esf-1].color.g = obtenerNumero(file);
				escena->Esferas[esf-1].color.b = obtenerNumero(file);
				
				VALIDA(escena,COLOR_T,file);
				escena->Esferas[esf-2].color.a = obtenerNumero(file);
				escena->Esferas[esf-2].color.r = obtenerNumero(file);
				escena->Esferas[esf-2].color.g = obtenerNumero(file);
				escena->Esferas[esf-2].color.b = obtenerNumero(file);
				
				VALIDA(escena,COLOR_T,file);
				escena->Cilindros[cil-1].color.a = obtenerNumero(file);
				escena->Cilindros[cil-1].color.r = obtenerNumero(file);
				escena->Cilindros[cil-1].color.g = obtenerNumero(file);
				escena->Cilindros[cil-1].color.b = obtenerNumero(file);
				
				
							// 'Kmaterial'
				VALIDA(escena,KMATERIAL,file);
				escena->Esferas[esf-1].Kd = obtenerNumero(file);
							
							// 'Ka'
				VALIDA(escena,KA,file);
				escena->Esferas[esf-1].Ka = obtenerNumero(file);
				
							// 'Ks'
				VALIDA(escena,KS,file);
				escena->Esferas[esf-1].Ks = obtenerNumero(file);
				
							// 'Kn'
				VALIDA(escena,KN,file);
				escena->Esferas[esf-1].Kn = obtenerNumero(file);
				
							// 'Kt'
				VALIDA(escena,KT,file);
				escena->Esferas[esf-1].Kt = obtenerNumero(file);
				
							// 'O1'
				VALIDA(escena,O1,file);
				escena->Esferas[esf-1].o1 = obtenerNumero(file);
				
							// 'O2'
				VALIDA(escena,O2,file);
				escena->Esferas[esf-1].o2 = obtenerNumero(file);
				
							// 'O3'
				VALIDA(escena,O3,file);
				escena->Esferas[esf-1].o3 = obtenerNumero(file);
				
				
				
							// 'Cortes'
				VALIDA(escena,CORTES,file);
				escena->Esferas[esf-1].cortes = obtenerNumero(file);
				escena->Esferas[esf-1].planos_corte = 
						malloc(escena->Esferas[esf-1].cortes * sizeof(int));
						
				if (escena->Esferas[esf-1].cortes>0){
				VALIDA(escena,CORTES,file);
				for (i=0;i<escena->Esferas[esf-1].cortes;i++){
					escena->Esferas[esf-1].planos_corte[i] = roundl(obtenerNumero(file));
				}
				}
				
				
				
				
				
				VALIDA(escena,KMATERIAL,file);
				escena->Esferas[esf-2].Kd = obtenerNumero(file);
							
							// 'Ka'
				VALIDA(escena,KA,file);
				escena->Esferas[esf-2].Ka = obtenerNumero(file);
				
							// 'Ks'
				VALIDA(escena,KS,file);
				escena->Esferas[esf-2].Ks = obtenerNumero(file);
				
							// 'Kn'
				VALIDA(escena,KN,file);
				escena->Esferas[esf-2].Kn = obtenerNumero(file);
				
							// 'Kt'
				VALIDA(escena,KT,file);
				escena->Esferas[esf-2].Kt = obtenerNumero(file);
				
							// 'O1'
				VALIDA(escena,O1,file);
				escena->Esferas[esf-2].o1 = obtenerNumero(file);
				
							// 'O2'
				VALIDA(escena,O2,file);
				escena->Esferas[esf-2].o2 = obtenerNumero(file);
				
							// 'O3'
				VALIDA(escena,O3,file);
				escena->Esferas[esf-2].o3 = obtenerNumero(file);
				
				
				
							// 'Cortes'
				VALIDA(escena,CORTES,file);
				escena->Esferas[esf-2].cortes = obtenerNumero(file);
				escena->Esferas[esf-2].planos_corte = 
						malloc(escena->Esferas[esf-1].cortes * sizeof(int));
						
				if (escena->Esferas[esf-2].cortes>0){
				VALIDA(escena,CORTES,file);
				for (i=0;i<escena->Esferas[esf-2].cortes;i++){
					escena->Esferas[esf-2].planos_corte[i] = roundl(obtenerNumero(file));
				}
				}
				
					// 'Kmaterial'
				VALIDA(escena,KMATERIAL,file);
				escena->Cilindros[cil-1].Kd = obtenerNumero(file);
							
							// 'Ka'
				VALIDA(escena,KA,file);
				escena->Cilindros[cil-1].Ka = obtenerNumero(file);
				
							// 'Ks'
				VALIDA(escena,KS,file);
				escena->Cilindros[cil-1].Ks = obtenerNumero(file);
				
							// 'Kn'
				VALIDA(escena,KN,file);
				escena->Cilindros[cil-1].Kn = obtenerNumero(file);
				
							// 'Kt'
				VALIDA(escena,KT,file);
				escena->Cilindros[cil-1].Kt = obtenerNumero(file);
				
							// 'O1'
				VALIDA(escena,O1,file);
				escena->Cilindros[cil-1].o1 = obtenerNumero(file);
				
							// 'O2'
				VALIDA(escena,O2,file);
				escena->Cilindros[cil-1].o2 = obtenerNumero(file);
				
							// 'O3'
				VALIDA(escena,O3,file);
				escena->Cilindros[cil-1].o3 = obtenerNumero(file);
				
				
				
							// 'Cortes'
				VALIDA(escena,CORTES,file);
				escena->Cilindros[cil-1].cortes = obtenerNumero(file);
				escena->Cilindros[cil-1].planos_corte = 
						malloc(escena->Cilindros[cil-1].cortes * sizeof(int));
						
				if (escena->Cilindros[cil-1].cortes>0){
					VALIDA(escena,CORTES,file);
					for (i=0;i<escena->Cilindros[cil-1].cortes;i++){
						escena->Cilindros[cil-1].planos_corte[i] = roundl(obtenerNumero(file));
					}
				}
				
							// '}'
				if (!esperaToken(CORCHETE_FIN,file))
				{
					free(escena->Esferas);
					free(escena);
					fclose(file);
					return NULL;
				}
	
	
				
	//*****************************************************************
	//**************** LUZ ********************************************
	
		  }else if (token == LUZ){
			  
			 escena->numLuces++;
			 luz = escena->numLuces;
			 escena->luces = realloc(escena->luces,luz*sizeof(Luz));
			             // '{'
			if (!esperaToken(CORCHETE_INI,file))
			{ 
				free(escena->luces);
				free(escena);
				fclose(file);
			 return NULL;
			}	  
			
						// 'x'
			VALIDA(escena,X,file);
			escena->luces[luz-1].posicion.x = obtenerNumero(file);
			
					    // 'y'
			VALIDA(escena,Y,file);
			escena->luces[luz-1].posicion.y = obtenerNumero(file);
			
					    // 'z'
			VALIDA(escena,Z,file);
			escena->luces[luz-1].posicion.z = obtenerNumero(file);
			
					    // 'c1'
			VALIDA(escena,C1,file);
			escena->luces[luz-1].c1 = obtenerNumero(file);
						
					    // 'c2'
			VALIDA(escena,C2,file);
			escena->luces[luz-1].c2 = obtenerNumero(file);
						
					    // 'c3'
			VALIDA(escena,C3,file);
			escena->luces[luz-1].c3 = obtenerNumero(file);
			
					    // 'Intensidad'
			VALIDA(escena,INTENSIDAD,file);
			escena->luces[luz-1].Intensidad = obtenerNumero(file);
			
					    // 'Color'
			VALIDA(escena,COLOR_T,file);
			escena->luces[luz-1].color.a = obtenerNumero(file);
			escena->luces[luz-1].color.r = obtenerNumero(file);
			escena->luces[luz-1].color.g = obtenerNumero(file);
			escena->luces[luz-1].color.b = obtenerNumero(file);
			
					    // '}'
			if (!esperaToken(CORCHETE_FIN,file))
			{
				free(escena->luces);
				free(escena);
				fclose(file);
			 return NULL;
			}
	//*****************************************************************
	//********************* CILINDRO ************************************
		}else if (token == CILINDRO)
		 {
			 escena->numCilindros++;
			 cil = escena->numCilindros;
			 escena->Cilindros = realloc(escena->Cilindros,cil*sizeof(Cilindro));
			             // '{'
			if (!esperaToken(CORCHETE_INI,file))
			{ 
				free(escena->Cilindros);
				free(escena);
				fclose(file);
				
			 return NULL;
			}
			
			
						// 'Ancla'
			VALIDA(escena,ANCLA,file);
			escena->Cilindros[cil-1].rayo.ancla.x = obtenerNumero(file);
			escena->Cilindros[cil-1].rayo.ancla.y = obtenerNumero(file);
			escena->Cilindros[cil-1].rayo.ancla.z = obtenerNumero(file);
			
			
						// 'Direccion'
			VALIDA(escena,DIRECCION,file);
			escena->Cilindros[cil-1].rayo.vector.dire.x = obtenerNumero(file);
			escena->Cilindros[cil-1].rayo.vector.dire.y = obtenerNumero(file);
			escena->Cilindros[cil-1].rayo.vector.dire.z = obtenerNumero(file);
			
			escena->Cilindros[cil-1].rayo.vector = 
			CREAR_VECTOR(escena->Cilindros[cil-1].rayo.ancla,escena->Cilindros[cil-1].rayo.vector.dire);
			
			
			
			Normalizar(&(escena->Cilindros[cil-1].rayo.vector));
			
			
						// 'H1'
			VALIDA(escena,H1,file);
			escena->Cilindros[cil-1].h1 = obtenerNumero(file);
			
			
						// 'H2'
			VALIDA(escena,H2,file);
			escena->Cilindros[cil-1].h2 = obtenerNumero(file);
			
			
					    // 'Radio'
			VALIDA(escena,RADIO,file);
			escena->Cilindros[cil-1].radio = obtenerNumero(file);
			escena->Cilindros[cil-1].radio2 = escena->Cilindros[cil-1].radio * 
											escena->Cilindros[cil-1].radio;
			
					    // 'Color'
			VALIDA(escena,COLOR_T,file);
			escena->Cilindros[cil-1].color.a = obtenerNumero(file);
			escena->Cilindros[cil-1].color.r = obtenerNumero(file);
			escena->Cilindros[cil-1].color.g = obtenerNumero(file);
			escena->Cilindros[cil-1].color.b = obtenerNumero(file);
			
					    // 'Kmaterial'
			VALIDA(escena,KMATERIAL,file);
			escena->Cilindros[cil-1].Kd = obtenerNumero(file);
						
					    // 'Ka'
			VALIDA(escena,KA,file);
			escena->Cilindros[cil-1].Ka = obtenerNumero(file);
			
					    // 'Ks'
			VALIDA(escena,KS,file);
			escena->Cilindros[cil-1].Ks = obtenerNumero(file);
			
						// 'Kn'
			VALIDA(escena,KN,file);
			escena->Cilindros[cil-1].Kn = obtenerNumero(file);
			
						// 'Kt'
			VALIDA(escena,KT,file);
			escena->Cilindros[cil-1].Kt = obtenerNumero(file);
			
						// 'O1'
			VALIDA(escena,O1,file);
			escena->Cilindros[cil-1].o1 = obtenerNumero(file);
			
						// 'O2'
			VALIDA(escena,O2,file);
			escena->Cilindros[cil-1].o2 = obtenerNumero(file);
			
						// 'O3'
			VALIDA(escena,O3,file);
			escena->Cilindros[cil-1].o3 = obtenerNumero(file);
			

			
						// 'Cortes'
			VALIDA(escena,CORTES,file);
			escena->Cilindros[cil-1].cortes = obtenerNumero(file);
			escena->Cilindros[cil-1].planos_corte = 
						malloc(escena->Cilindros[cil-1].cortes * sizeof(int));
						
			if (escena->Cilindros[cil-1].cortes>0){
				VALIDA(escena,CORTES,file);
				for (i=0;i<escena->Cilindros[cil-1].cortes;i++){
					escena->Cilindros[cil-1].planos_corte[i] = roundl(obtenerNumero(file));
				}
			}
			
						// 'Textura'
			VALIDA(escena,TEXTURA,file);
			escena->Cilindros[cil-1].textura = obtenerNumero(file);
						// 'Calado'
			VALIDA(escena,CALADO,file);
			escena->Cilindros[cil-1].calado = obtenerNumero(file);
						// 'Bump'
			VALIDA(escena,BUMP,file);
			escena->Cilindros[cil-1].bump = obtenerNumero(file);
			
			if (escena->Cilindros[cil-1].bump != -1){
				escena->Cilindros[cil-1].maxbump = obtenerNumero(file);
							// 'TU'
				VALIDA(escena,TU,file);
				escena->Cilindros[cil-1].tu = obtenerNumero(file);
							// 'TV'
				VALIDA(escena,TV,file);
				escena->Cilindros[cil-1].tv = obtenerNumero(file);
			}
			
			if (escena->Cilindros[cil-1].textura != -1 || 
				escena->Cilindros[cil-1].calado != -1 || escena->Cilindros[cil-1].bump != -1){
							// 'Direccion'
				VALIDA(escena,DIRECCION,file);
				escena->Cilindros[cil-1].G.dire.x = obtenerNumero(file);
				escena->Cilindros[cil-1].G.dire.y = obtenerNumero(file);
				escena->Cilindros[cil-1].G.dire.z = obtenerNumero(file);
				
				escena->Cilindros[cil-1].G = 
				PRODUCTO_CRUZ(
					PRODUCTO_CRUZ(escena->Cilindros[cil-1].G,escena->Cilindros[cil-1].rayo.vector)
					,escena->Cilindros[cil-1].rayo.vector);
				Normalizar(&escena->Cilindros[cil-1].G);
				
				escena->Cilindros[cil-1].Correcto = 
							PRODUCTO_CRUZ(escena->Cilindros[cil-1].G
										 ,escena->Cilindros[cil-1].rayo.vector);
				Normalizar(&escena->Cilindros[cil-1].Correcto);
				
				escena->Cilindros[cil-1].D = escena->Cilindros[cil-1].Correcto.dire.x * escena->Cilindros[cil-1].rayo.ancla.x;
				escena->Cilindros[cil-1].D += escena->Cilindros[cil-1].Correcto.dire.y * escena->Cilindros[cil-1].rayo.ancla.y;
				escena->Cilindros[cil-1].D += escena->Cilindros[cil-1].Correcto.dire.z * escena->Cilindros[cil-1].rayo.ancla.z;
				escena->Cilindros[cil-1].D *= -1;
			}
			
			
					    // '}'
			if (!esperaToken(CORCHETE_FIN,file))
			{
				free(escena->Esferas);
				free(escena);
				fclose(file);
			 return NULL;
			}	
			
			
	//*****************************************************************
	//********************* CONO ************************************
		}else if (token == CONO)
		 {
			 escena->numConos++;
			 con = escena->numConos;
			 escena->Conos = realloc(escena->Conos,con*sizeof(Cono));
			             // '{'
			if (!esperaToken(CORCHETE_INI,file))
			{ 
				free(escena->Conos);
				free(escena);
				fclose(file);
				
			 return NULL;
			}
			
			
						// 'Ancla'
			VALIDA(escena,ANCLA,file);
			escena->Conos[con-1].rayo.ancla.x = obtenerNumero(file);
			escena->Conos[con-1].rayo.ancla.y = obtenerNumero(file);
			escena->Conos[con-1].rayo.ancla.z = obtenerNumero(file);
			
			
						// 'Direccion'
			VALIDA(escena,DIRECCION,file);
			escena->Conos[con-1].rayo.vector.dire.x = obtenerNumero(file);
			escena->Conos[con-1].rayo.vector.dire.y = obtenerNumero(file);
			escena->Conos[con-1].rayo.vector.dire.z = obtenerNumero(file);
			
			escena->Conos[con-1].rayo.vector = 
			CREAR_VECTOR(escena->Conos[con-1].rayo.ancla,escena->Conos[con-1].rayo.vector.dire);
			
			
			
			Normalizar(&(escena->Conos[con-1].rayo.vector));
			
			
						// 'H1'
			VALIDA(escena,H1,file);
			escena->Conos[con-1].h1 = obtenerNumero(file);
			
			
						// 'H2'
			VALIDA(escena,H2,file);
			escena->Conos[con-1].h2 = obtenerNumero(file);
			
						// 'UTOKEN'
			VALIDA(escena,UTOKEN,file);
			escena->Conos[con-1].uv = obtenerNumero(file);
			
						// 'VTOKEN'
			VALIDA(escena,VTOKEN,file);
			escena->Conos[con-1].uv /= obtenerNumero(file);
			
			
					    // 'Color'
			VALIDA(escena,COLOR_T,file);
			escena->Conos[con-1].color.a = obtenerNumero(file);
			escena->Conos[con-1].color.r = obtenerNumero(file);
			escena->Conos[con-1].color.g = obtenerNumero(file);
			escena->Conos[con-1].color.b = obtenerNumero(file);
			
					    // 'Kmaterial'
			VALIDA(escena,KMATERIAL,file);
			escena->Conos[con-1].Kd = obtenerNumero(file);
						
					    // 'Ka'
			VALIDA(escena,KA,file);
			escena->Conos[con-1].Ka = obtenerNumero(file);
			
					    // 'Ks'
			VALIDA(escena,KS,file);
			escena->Conos[con-1].Ks = obtenerNumero(file);
			
						// 'Kn'
			VALIDA(escena,KN,file);
			escena->Conos[con-1].Kn = obtenerNumero(file);
			
						// 'Kt'
			VALIDA(escena,KT,file);
			escena->Conos[con-1].Kt = obtenerNumero(file);
			
						// 'O1'
			VALIDA(escena,O1,file);
			escena->Conos[con-1].o1 = obtenerNumero(file);
			
						// 'O2'
			VALIDA(escena,O2,file);
			escena->Conos[con-1].o2 = obtenerNumero(file);
			
						// 'O3'
			VALIDA(escena,O3,file);
			escena->Conos[con-1].o3 = obtenerNumero(file);
			
			
						// 'Cortes'
			VALIDA(escena,CORTES,file);
			escena->Conos[con-1].cortes = obtenerNumero(file);
			escena->Conos[con-1].planos_corte = 
						malloc(escena->Conos[con-1].cortes * sizeof(int));
						
			if (escena->Conos[con-1].cortes>0){
				VALIDA(escena,CORTES,file);
				for (i=0;i<escena->Conos[con-1].cortes;i++){
					escena->Conos[con-1].planos_corte[i] = roundl(obtenerNumero(file));
				}
			}
			
			
			
			
					    // '}'
			if (!esperaToken(CORCHETE_FIN,file))
			{
				free(escena->Conos);
				free(escena);
				fclose(file);
			 return NULL;
			}	
	
			
			
		// ***********************************************************
		// ************ POLIGONO ************************************
			
		 }else if (token == POLIGONO){
			 escena->numPoligonos++;
			 poli = escena->numPoligonos;
			 escena->Poligonos = realloc(escena->Poligonos,poli*sizeof(Poligono));
			 escena->Poligonos[poli-1].numPuntos=0;
			 escena->Poligonos[poli-1].puntos = NULL;
			             // '{'
			if (!esperaToken(CORCHETE_INI,file))
			{ 
				free(escena->Poligonos);
				free(escena);
				fclose(file);
			 return NULL;
			}
			
								    // 'Color'
			VALIDA(escena,COLOR_T,file);
			escena->Poligonos[poli-1].color.a = obtenerNumero(file);
			escena->Poligonos[poli-1].color.r = obtenerNumero(file);
			escena->Poligonos[poli-1].color.g = obtenerNumero(file);
			escena->Poligonos[poli-1].color.b = obtenerNumero(file);
			
					    // 'Kmaterial'
			VALIDA(escena,KMATERIAL,file);
			escena->Poligonos[poli-1].Kd = obtenerNumero(file);
						
					    // 'Ka'
			VALIDA(escena,KA,file);
			escena->Poligonos[poli-1].Ka = obtenerNumero(file);
			
					    // 'Ks'
			VALIDA(escena,KS,file);
			escena->Poligonos[poli-1].Ks = obtenerNumero(file);
			
						// 'Kn'
			VALIDA(escena,KN,file);
			escena->Poligonos[poli-1].Kn = obtenerNumero(file);

						// 'Kt'
			VALIDA(escena,KT,file);
			escena->Poligonos[poli-1].Kt = obtenerNumero(file);
			
						// 'O1'
			VALIDA(escena,O1,file);
			escena->Poligonos[poli-1].o1 = obtenerNumero(file);
			
						// 'O2'
			VALIDA(escena,O2,file);
			escena->Poligonos[poli-1].o2 = obtenerNumero(file);
			
						// 'O3'
			VALIDA(escena,O3,file);
			escena->Poligonos[poli-1].o3 = obtenerNumero(file);
			
			
			
						// 'Cortes'
			VALIDA(escena,CORTES,file);
			escena->Poligonos[poli-1].cortes = obtenerNumero(file);
			
			escena->Poligonos[poli-1].planos_corte = 
						malloc(escena->Poligonos[poli-1].cortes * sizeof(int));
						
			if (escena->Poligonos[poli-1].cortes>0){
				VALIDA(escena,CORTES,file);
				for (i=0;i<escena->Poligonos[poli-1].cortes;i++){
					escena->Poligonos[poli-1].planos_corte[i] = roundl(obtenerNumero(file));
				}
			}
			
						// 'Textura'
			VALIDA(escena,TEXTURA,file);
			escena->Poligonos[poli-1].textura = obtenerNumero(file);
			
						// 'Calado'
			VALIDA(escena,CALADO,file);
			escena->Poligonos[poli-1].calado = obtenerNumero(file);
			
						// 'Bump'
			VALIDA(escena,BUMP,file);
			escena->Poligonos[poli-1].bump = obtenerNumero(file);
			
			if (escena->Poligonos[poli-1].bump!=-1){
				escena->Poligonos[poli-1].maxBump = obtenerNumero(file);
							// 'TU'
				VALIDA(escena,TU,file);
				escena->Poligonos[poli-1].tu = obtenerNumero(file);
							// 'TV'
				VALIDA(escena,TV,file);
				escena->Poligonos[poli-1].tv = obtenerNumero(file);
			}
			
			
			
			
			
			puntos = NULL;
			
			while ((token = NextToken(file)) == VERTICE){
				if (!esperaToken(IGUAL,file)) 
				{ 
					liberarEscena(escena);
					fclose(file); 
					return NULL; 
				}
				escena->Poligonos[poli-1].numPuntos++;
				
				puntos = realloc(puntos,escena->Poligonos[poli-1].numPuntos * sizeof(Punto));
				
						
				punto.x = obtenerNumero(file);
				punto.y = obtenerNumero(file);
				punto.z = obtenerNumero(file);
				puntos[escena->Poligonos[poli-1].numPuntos-1] = punto;
			}
			escena->Poligonos[poli-1].puntos = puntos;
			
			v1 = CREAR_VECTOR(escena->Poligonos[poli-1].puntos[0],escena->Poligonos[poli-1].puntos[1]);
			v2 = CREAR_VECTOR(escena->Poligonos[poli-1].puntos[0],escena->Poligonos[poli-1].puntos[2]);
			escena->Poligonos[poli-1].Normal = PRODUCTO_CRUZ(v1,v2);
			Normalizar(&escena->Poligonos[poli-1].Normal);
			escena->Poligonos[poli-1].D = escena->Poligonos[poli-1].Normal.dire.x * escena->Poligonos[poli-1].puntos[0].x;
			escena->Poligonos[poli-1].D += escena->Poligonos[poli-1].Normal.dire.y * escena->Poligonos[poli-1].puntos[0].y;
			escena->Poligonos[poli-1].D += escena->Poligonos[poli-1].Normal.dire.z * escena->Poligonos[poli-1].puntos[0].z;
			escena->Poligonos[poli-1].D *= -1;
			
			if (escena->Poligonos[poli-1].textura!=-1 || escena->Poligonos[poli-1].bump!=-1){
				escena->Poligonos[poli-1].U = 
					CREAR_VECTOR(escena->Poligonos[poli-1].puntos[0],escena->Poligonos[poli-1].puntos[1]);
				escena->Poligonos[poli-1].V = 
					CREAR_VECTOR(escena->Poligonos[poli-1].puntos[0],escena->Poligonos[poli-1].puntos[3]);
				escena->Poligonos[poli-1].texD = Normalizar(&escena->Poligonos[poli-1].U);
				escena->Poligonos[poli-1].texH = Normalizar(&escena->Poligonos[poli-1].V);
				
			}
			
			
			
			escena->Poligonos[poli-1].disco = 0;
			
			if (token==DISCO){
				Punto p1,p2,p3,p4;
				p1 =escena->Poligonos[poli-1].puntos[0];
				p2 =escena->Poligonos[poli-1].puntos[1];
				p3 =escena->Poligonos[poli-1].puntos[2];
				p4 =escena->Poligonos[poli-1].puntos[3]; 
				
				escena->Poligonos[poli-1].disco = 1;
				VALIDA(escena,RADIO,file);
				escena->Poligonos[poli-1].radio = obtenerNumero(file);
				
				
				
				escena->Poligonos[poli-1].centro.x =
					MAX(MAX(p1.x,p2.x),MAX(p3.x,p4.x))+MIN(MIN(p1.x,p2.x),MIN(p3.x,p4.x));
				escena->Poligonos[poli-1].centro.y =
					MAX(MAX(p1.y,p2.y),MAX(p3.y,p4.y))+MIN(MIN(p1.y,p2.y),MIN(p3.y,p4.y));
				escena->Poligonos[poli-1].centro.z =
					MAX(MAX(p1.z,p2.z),MAX(p3.z,p4.z))+MIN(MIN(p1.z,p2.z),MIN(p3.z,p4.z));
				escena->Poligonos[poli-1].centro.x /=2.0;
				escena->Poligonos[poli-1].centro.y /=2.0;
				escena->Poligonos[poli-1].centro.z /=2.0;
				printf("%LF %LF %LF \n radio %LF\n",escena->Poligonos[poli-1].centro.x,escena->Poligonos[poli-1].centro.y,escena->Poligonos[poli-1].centro.z,escena->Poligonos[poli-1].radio);
				if (!esperaToken(CORCHETE_FIN,file))
				{
					free(escena->Conos);
					free(escena);
					fclose(file);
					return NULL;
				}	
			}else if (token!=CORCHETE_FIN)
			{
				liberarEscena(escena);
				fclose(file);
			 return NULL;
			}
	   }
	 
	}
		 
	 }else{
		 printf("Error de sintaxis, se Esperaba Escena\n");
		 free(escena);
		 fclose(file);
		 return NULL;
	 }
	 fclose(file);
	 return escena;
 }

 
