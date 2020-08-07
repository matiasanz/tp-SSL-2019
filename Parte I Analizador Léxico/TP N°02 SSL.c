#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/*
	Alumno: Matías Anzorandía
	Curso: 	K2055
	Docente: Roxana Leituz
	Especialidad: Ing. en Sistemas de Información
	Año: 2019
*/

//funciones auxiliares
int esLetra(char caracter)
{
	return ('a'<=caracter&&caracter<='z')
		 ||('A'<=caracter&&caracter<='Z');
}

int esDigito(char caracter)
{
	return ('0'<=caracter&&caracter<='9');
}

char* substring(char cadena[], int length)
{
	char* subcadena = (char*)malloc(sizeof(char)*(length+1));
	subcadena[length]='\0';
	strncpy(subcadena, cadena, length);
	
	return subcadena;
}

	
//Automata Finito Deterministico Reconocedor de Lexemas
	#define Error 12
	int matriz[][10]=
	{
		{1, 2, 3, Error, 7, 8, 9, 10, 11, Error},
		{1, 1, 4, 4, 4, 4, 4, 4, 4, 4},
		{5, 2, 5, 5, 5, 5, 5, 5, 5, 5},
		{Error, Error, Error, 6, Error, Error, Error, Error, Error, Error}
	};
	
int columna(char caracter)
{
	return (esLetra(caracter)||caracter=='_')? 0:
			esDigito(caracter)?	1:
			(caracter==':')   ? 2:
			(caracter=='=')	  ? 3:
			(caracter=='(')   ? 4:
			(caracter==')')   ? 5:
			(caracter=='+')	  ? 6:
			(caracter=='-')   ? 7:
			(caracter==';')   ? 8:
								9;
}

int esDeAceptacion(int estado)
{
	return 4<=estado&&estado<=11;
}

int automata(int estado, char caracter) //reconoce lexemas
{
	int indice=0;
	return matriz[estado][columna(caracter)];
}

//Mis Funciones
void concatenar(char caracter, char lexema[], int*length)
{
	if(*length<25) // tamaño array lexema
	{
		lexema[(*length)++]=caracter;
	}
}

int leerLexema(FILE*archivo, char lexema[], int*length, int* estado)
{
	char caracter;
	while (!esDeAceptacion(*estado)&&(caracter = fgetc(archivo))!=EOF&&*estado!=Error)
	{
		concatenar(caracter, lexema, length);
		*estado = automata(*estado, caracter);
	}
	
	//retroceder una posicion en el archivo si es espurio
	if(*estado==4||*estado==5)
	{
		fseek(archivo, -1, 	SEEK_CUR);
		(*length)--; // "Eliminar" última letra del lexema
	}
	
	else if(!esDeAceptacion(*estado))
	{
		*estado = automata(*estado, '$'); //fuerzo espurio para caso una sola letra
	}
	
	return !feof(archivo)||esDeAceptacion(*estado);
}

// reconocedor de tokens

int reconocerToken(int key)
{
	switch(key)
	{
		case 4:
			printf("identificador\n");
			break;
		case 5:
			printf("numero entero\n");
			break;
		case 6:
			printf("Operador de Asignacion\n");
			break;
		case 7:
			printf("Signo de Puntuacion '('\n");
			break;
		case 8:
			printf("Signo de Puntuacion ')'\n");
			break;
		case 9:
			printf("Operador de Adicion\n");
			break;
		case 10:
			printf("Operador de Sustraccion\n");
			break;
		case 11:
			printf("Signo de Puntuacion ';'\n");
			break;
		default:
			printf("UTN: Unidentified Token Name");
			return 0;
	}
		
	return 1;
}

//Programa Principal
int main(int argc, char *argv[]) 
{
	FILE*archivo = fopen("microcodigo.txt", "r+b"); //No acepta espacios.
	char caracter, lexema[25];
	int estado=0, length=0;
	
	int hayMasLexemas;
	while((hayMasLexemas=leerLexema(archivo, lexema, &length, &estado)))
	{
		printf("%s ", substring(lexema, length));
		int reconocimientoExitoso = reconocerToken(estado);
		if(!reconocimientoExitoso)
		{
			fclose(archivo);
			return 1;	//En caso de no reconocer un token se detiene el programa, ya que podría ocasionar una mala lectura
		}
		
		length=0, estado=0;
		
	} 
	
	fclose(archivo);
	return 0;
}

