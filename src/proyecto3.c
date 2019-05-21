/* ************ PROYECTO3.C *************************
 * 
 *   Proyecto 3: Ray Tracing Avanzado
 * 
 *   Prof.: 
 * 	 Francisco J. Torres Rojas PhD
 * 
 *   Autor:
 *   Andres Aguilar Umana
 *   200928927
 * 
 *   26 de Mayo 2012
 * 
 ****************************************************/

#include <stdio.h>
#include <raytracer.h>
#include <parser.h>

int main(int argc, char **args){
	
	
	
	
	iniciarRayTracing(args[1],atoi(args[2]));
	
	system("convert ima.avs ima2.jpg");
	//system("display ima.avs");
	return 1;
}
