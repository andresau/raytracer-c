/* ************ IMAGENES.C *************************
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
 #include <stdlib.h>
 
void liberarImagen(Imagen ima){
	int i;
	for (i=0; i<ima.resX; i++)
		free(ima.ima[i]);
	free(ima.ima);
}

void crearImagen(Imagen *ima,int resX, int resY){
	int i;
	
	ima->ima = malloc(resX * sizeof(Color *));
	for (i=0; i<resX; i++){
		ima->ima[i] = malloc(resY*sizeof(Color));
	}
	ima->resX = resX;
	ima->resY = resY;
}

void guardarImagen(Imagen ima, char *arch){
	FILE *file;
	int numero;
	int i,j;
	unsigned char c;
	
	file = fopen(arch,"w");
	numero = ima.resX;
	numero = FIX(numero);
	fwrite(&numero,sizeof(unsigned int),1,file); //Ancho
	numero = ima.resY;
	numero = FIX(numero);
	fwrite(&numero,sizeof(unsigned int),1,file); //Alto
	for (j = 0; j < ima.resY; j++){
		for (i = 0; i < ima.resX; i++){
			c = (unsigned char) 255 * ima.ima[i][j].a;
			fputc(c,file);
			c = (unsigned char) 255 * ima.ima[i][j].r;
			fputc(c,file);
			c = (unsigned char) 255 * ima.ima[i][j].g;
			fputc(c,file);
			c = (unsigned char) 255 * ima.ima[i][j].b;
			fputc(c,file);
			//fwrite(&ima.ima[i][j],sizeof(Color),1,file);
		}
	}
	fclose(file);
}

Imagen leerTextura(char *archivo){
	FILE *arch;
	Imagen ima;
	int i,j,cc;
	ima.ima = NULL;
	if (!(arch=fopen(archivo,"r"))){
        printf("Error al abrir el archivo %s\n",archivo);
		return ima;
    } else{
		fread(&ima.resX,sizeof(int),1,arch);
		ima.resX = FIX(ima.resX);   //tomado de http://paulbourke.net/dataformats/avs_x/xtoraw.c para leer avs
		//printf("LeiX: %d\n",ima.resX);
		fread(&ima.resY,sizeof(int),1,arch);
		ima.resY = FIX(ima.resY);   //tomado de http://paulbourke.net/dataformats/avs_x/xtoraw.c para leer avs
		//printf("LeiY: %d\n",ima.resY);
		ima.ima = malloc(ima.resY*sizeof(Color*));
		for (i=0;i<ima.resY;i++) {
			ima.ima[i] = malloc(ima.resX*sizeof(Color));
		  for (j=0;j<ima.resX;j++) {
		     unsigned char a,r,g,b;
			 /* Read the current pixel */
			 a = fgetc(arch);
			 r = fgetc(arch);
			 g = fgetc(arch);
			 b = fgetc(arch);
			 if (a == EOF || g == EOF || r == EOF || b == EOF) {
				fprintf(stderr,"Unexpected end of file\n");
				exit(-1);
			 }
			 
			 ima.ima[i][j].a = (a/255.0);
			 ima.ima[i][j].r = (r/255.0);
			 ima.ima[i][j].g = (g/255.0);
			 ima.ima[i][j].b = (b/255.0);
			 cc++;
		  }
		}
		fclose(arch);
	}
	return ima;
}
