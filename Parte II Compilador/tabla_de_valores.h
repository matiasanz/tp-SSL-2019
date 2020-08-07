# ifndef _DOMINIO_
# define _DOMINIO_

  #include <stdio.h>
  #include <string.h>
  #include <time.h>
  #include <windows.h>
  #include <stdbool.h>

//variable
typedef struct{
  char* nombre;
  int valor;
} variable;

//Historial de variables almacenadas implementado en una pila
typedef struct nodoVar{
	variable info;
	struct nodoVar* sig;
} nodoVariable;

typedef nodoVariable repositorioDeVariables;
typedef nodoVariable entradaDeTabla;

  entradaDeTabla* findEntrada(char*identificador, repositorioDeVariables*);

  void declararVariable(char*identificador, int valor, repositorioDeVariables**);
  void asignarVariable(char*identificador, int nuevoValor, repositorioDeVariables**);
  void leer(char*identificador, repositorioDeVariables*);

  int obtenerValor(char*identificador, repositorioDeVariables*);
  int operar(int operando, short signo, int otroOperando);
  
# endif
