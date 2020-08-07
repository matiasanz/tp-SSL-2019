%{
  #include <stdio.h>
  #include <string.h>
  #include <time.h>
  #include <windows.h>

  extern FILE*yyin;
  extern int yylex(void);
  extern void yyerror(char*);
  
  typedef struct almacenDeVariables* punteroAlmacen;

  punteroAlmacen findVariable(char*identificador, punteroAlmacen almacen);

  void declararVariable(char*identificador, int valor, struct almacenDeVariables**variablesDeclaradas);
  void asignarVariable(char*identificador, int nuevoValor, punteroAlmacen*variablesDeclaradas);
  void leer(char*identificador, struct almacenDeVariables*almacenDeVariables);

  int obtenerValor(char*identificador, struct almacenDeVariables*variablesDeclaradas);
  int operar(int operando, short signo, int otroOperando);
  
  struct almacenDeVariables* variablesDeclaradas = NULL;

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

sentencia: ID ASIGNACION expresion PUNTOYCOMA	{printf("-- Se va a hacer %s = %d\n", $1, $3); asignarVariable($1, $3, &variablesDeclaradas);}
| ESCRIBIR ID CONSTANTE PUNTOYCOMA  			{printf("-- Se va a hacer %s = %d\n", $2, $3); asignarVariable($2, $3, &variablesDeclaradas);}
| LEER ID PUNTOYCOMA  							{printf("-- Se va a leer %s\n", $2); leer($2, variablesDeclaradas);}
;	

expresion: primaria
|expresion OPERADOR_ADITIVO primaria			{$$=operar($1, $2, $3);}
;

primaria: ID									{$$=obtenerValor($1, variablesDeclaradas)};
|CONSTANTE
|PARENTESIS_IZQ expresion PARENTESIS_DER		{$$=$2;}
;

fin: FIN PUNTOYCOMA
;

%%

//variable
typedef struct variable{
  char* nombre;
  int valor;
} variable;

struct variable variableCreate(char* identificador, int valor){
	return (variable){identificador, valor};
}

//Historial de variables almacenadas implementado en una lista enlazada
typedef struct almacenDeVariables{
	struct variable info;
	struct almacenDeVariables* sig;
} almacenDeVariables;

//typedef struct almacenDeVariables* punteroAlmacen;

void addVariable(struct variable var, punteroAlmacen*almacen){
	punteroAlmacen nuevaVariable = (punteroAlmacen)malloc(sizeof(struct almacenDeVariables));
	nuevaVariable->info = var;
	nuevaVariable->sig = *almacen;
	*almacen = nuevaVariable;
}

int hayMasVariables(punteroAlmacen almacen){
	return almacen!=NULL;
}

punteroAlmacen findVariable(char*identificador, punteroAlmacen almacen){
	while(hayMasVariables(almacen))
	{
		if(!strcmp(almacen->info.nombre, identificador))
		{
			return almacen;
		}
			
		almacen = almacen->sig;			
	}
	
	return NULL;
}

struct variable* getVariable(char*identificador, punteroAlmacen almacen){
	punteroAlmacen posicion = findVariable(identificador, almacen);
	return posicion? &(posicion->info) : NULL;
}

//Mis funciones
void declararVariable(char*identificador, int valor, struct almacenDeVariables**variablesDeclaradas){
//	if(!findVariable(identificador, *variablesDeclaradas))
//	{
		struct variable nuevaVariable = variableCreate(identificador, valor);
		addVariable(nuevaVariable, variablesDeclaradas);
//	}
}

void asignarVariable(char*identificador, int nuevoValor, punteroAlmacen*variablesDeclaradas){
	variable* var = getVariable(identificador, *variablesDeclaradas);
	
	if(var)	var->valor=nuevoValor;
	
	else declararVariable(identificador, nuevoValor, variablesDeclaradas);
}

int obtenerValor(char*identificador, struct almacenDeVariables*variablesDeclaradas){
	variable*unaVariable=getVariable(identificador, variablesDeclaradas);
	return unaVariable? unaVariable->valor : 0;
}

void leer(char*identificador, struct almacenDeVariables *almacen){
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

//Error
void yyerror(char*s){
  printf(">> Error de compilacion\n\n");
}

int yywrap(){
 return 1;
}