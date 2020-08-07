%{
  #include <stdio.h>
  #include <string.h>
  #include <time.h>
  #include <windows.h>
  #include <stdbool.h>

  extern FILE*yyin;
  extern int yylex(void);
  extern void yyerror(char*);
  
  typedef struct nodoVar repositorioDeVariables;
  typedef struct nodoVar entradaDeTabla;

  entradaDeTabla* findEntrada(char*identificador, repositorioDeVariables*);

  void declararVariable(char*identificador, int valor, repositorioDeVariables**);
  void asignarVariable(char*identificador, int nuevoValor, repositorioDeVariables**);
  void leer(char*identificador, repositorioDeVariables*);

  int obtenerValor(char*identificador, repositorioDeVariables*);
  int operar(int operando, short signo, int otroOperando);
  
  repositorioDeVariables* tablaDeValores = NULL;

%}
%union{
   char* cadena;
   int entero;
   short signo;
}
%token INICIO LEER ESCRIBIR FIN
%token ASIGNACION PUNTOYCOMA PARENTESIS_IZQ PARENTESIS_DER
%token <entero> CONSTANTE
%token <cadena> ID
%token <signo> OPERADOR_ADITIVO

%type <entero> expresion
%type <entero> primaria 
 
%%
programa: inicio sentencias fin {return 0;}
| inicio fin					{return 0;}
;

inicio: INICIO PUNTOYCOMA
;

sentencias: sentencias sentencia
| sentencia
;

sentencia: ID ASIGNACION expresion PUNTOYCOMA	{printf("-- Se va a hacer %s = %d\n", $1, $3); asignarVariable($1, $3, &tablaDeValores);}
| ESCRIBIR ID CONSTANTE PUNTOYCOMA  			{printf("-- Se va a hacer %s = %d\n", $2, $3); asignarVariable($2, $3, &tablaDeValores);}
| LEER ID PUNTOYCOMA  							{printf("-- Se va a leer %s\n", $2); leer($2, tablaDeValores);}
;	

expresion: primaria
|expresion OPERADOR_ADITIVO primaria			{$$=operar($1, $2, $3);}
;

primaria: ID									{$$=obtenerValor($1, tablaDeValores)};
|CONSTANTE
|PARENTESIS_IZQ expresion PARENTESIS_DER		{$$=$2;}
;

fin: FIN PUNTOYCOMA
;

%%

//Programa Principal
int main(int argc, char*argv[]){
  printf("\n*****************************************************************\n");
  
  if((yyin = fopen(argv[1], "r+b")))
  {
	  printf(">> Se compilara el archivo %s\n\n", argv[1]);
  }

  else
  {
	  printf(">> No se encontro ningun archivo, por favor ingrese el codigo manualmente\n\n");
  }

  yyparse();
  fclose(yyin);
  printf(">> Gracias por confiar en nosotros, tenga un buen dia!\n*****************************************************************\n"); 
  Sleep(1.5*1000);
  return 0;
}

//**************************************************** Funciones Declaradas ********************************************************************

//variable
typedef struct{
  char* nombre;
  int valor;
} variable;

variable variableCreate(char* identificador, int valor){
	return (variable){identificador, valor};
}

//Historial de variables almacenadas implementado en una pila
typedef struct nodoVar{
	variable info;
	struct nodoVar* sig;
} nodoVariable;

void addVariable(variable var, repositorioDeVariables** tabla){
	entradaDeTabla* nuevaEntrada = malloc(sizeof(entradaDeTabla));
	nuevaEntrada->info = var;
	nuevaEntrada->sig = *tabla;
	*tabla = nuevaEntrada;
}

bool hayMasVariables(repositorioDeVariables* almacen){
	return almacen!=NULL;
}

entradaDeTabla* findEntrada(char*identificador, repositorioDeVariables* tabla){
	while(hayMasVariables(tabla))
	{
		if(!strcmp(tabla->info.nombre, identificador))
		{
			return tabla;
		}
			
		tabla = tabla->sig;			
	}
	
	return NULL;
}

variable* getVariable(char*identificador, repositorioDeVariables* almacen){
	entradaDeTabla* entrada = findEntrada(identificador, almacen);
	return entrada? &(entrada->info) : NULL;
}

//Mis funciones

void declararVariable(char*identificador, int valor, repositorioDeVariables**tablaDeValores){
//	if(!findEntrada(identificador, *tablaDeValores)) //
//	{
		variable nuevaVariable = variableCreate(identificador, valor);
		addVariable(nuevaVariable, tablaDeValores); // <-- \ o / es horrible pero la profe dijo que era asi
//	}
}

void asignarVariable(char*identificador, int nuevoValor, repositorioDeVariables**tablaDeValores){
	variable* var = getVariable(identificador, *tablaDeValores);
	
	if(var)	var->valor=nuevoValor;
	
	else declararVariable(identificador, nuevoValor, tablaDeValores);
}

int obtenerValor(char*identificador, repositorioDeVariables* tablaDeValores){
	variable*unaVariable=getVariable(identificador, tablaDeValores);
	return unaVariable? unaVariable->valor : 0;
}

void leer(char*identificador, repositorioDeVariables* almacen){
	variable* variableDeLectura = getVariable(identificador, almacen);
	if(variableDeLectura){
		printf(">>  %s = %d\n", variableDeLectura->nombre, variableDeLectura->valor);
	}
	else printf(">>  El identificador %s no responde a ninguna variable\n", identificador);
}

int operar(int operando, short signo, int otroOperando){
	printf("--  Se resolvera %d+%d*%d\n", operando, signo, otroOperando);
	return operando+signo*otroOperando;
}

//Error
void yyerror(char*s){
  printf(">> Error de compilacion\n\n");
}

int yywrap(){
 return 1;
}