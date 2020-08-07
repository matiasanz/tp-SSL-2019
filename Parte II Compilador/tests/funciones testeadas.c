  #include <stdio.h>
  #include <string.h>
  #include <time.h>
  #include <windows.h>

  extern FILE*yyin;
  extern int yylex(void);
  extern void yyerror(char*);
  
  typedef struct almacenDeVariables* punteroAlmacen;

  punteroAlmacen findVariable(char*identificador, punteroAlmacen almacen);
  struct variable* getVariable(char*identificador, punteroAlmacen almacen);

  void declararVariable(char*identificador, int valor, struct almacenDeVariables**variablesDeclaradas);
  void asignarVariable(char*identificador, int nuevoValor, punteroAlmacen*variablesDeclaradas);
  void leer(char*identificador, struct almacenDeVariables*almacenDeVariables);

  int obtenerValor(char*identificador, struct almacenDeVariables*variablesDeclaradas);
  int operar(int operando, short signo, int otroOperando);
  
  struct almacenDeVariables* variablesDeclaradas = NULL;
  
  //****************************************************************************
  
  //variable
typedef struct variable{
  char* nombre;
  int valor;
}variable;

struct variable variableCreate(char* identificador, int valor){
	return (variable){identificador, valor};
}

//Historial de variables almacenadas implementado en una lista enlazada
typedef struct almacenDeVariables{
	struct variable info;
	struct almacenDeVariables* sig;
}variablesAlmacenadas;

//typedef struct almacenDeVariables* punteroAlmacen;

void addVariable(struct variable var, punteroAlmacen*almacen){
	punteroAlmacen nuevaVariable = (punteroAlmacen)malloc(sizeof(almacen));
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
//	printf(">>  Se resolvera %d+%d*%d\n", operando, signo, otroOperando);
	return operando+signo*otroOperando;
}

//*****************************************************************************************

//Tests
int main(){
	struct almacenDeVariables* variablesDeclaradas = NULL;
	
	struct variable var = variableCreate("Hola", 50);
		printf("1) test variableCreate: %s = %d\n",var.nombre, var.valor);
	
	addVariable(var, &variablesDeclaradas);
		printf("2) test addVariable: %s = %d\n",variablesDeclaradas->info.nombre, variablesDeclaradas->info.valor);
	
	declararVariable("jellllou", 2, &variablesDeclaradas);
		printf("3) test declararVariable: %s = %d\n",variablesDeclaradas->info.nombre, variablesDeclaradas->info.valor);
	
	declararVariable("chau", -1, &variablesDeclaradas);
		printf("4) test declarar otra Variable: %s = %d\n",variablesDeclaradas->info.nombre, variablesDeclaradas->info.valor);
	
	punteroAlmacen variableAnterior = variablesDeclaradas->sig;
		printf("5) siguiente Variable: %s = %d aguein\n",variableAnterior->info.nombre, variableAnterior->info.valor);
		
	punteroAlmacen acaEsta = findVariable("Hola", variablesDeclaradas);
		printf("6) findVariable:  %s = %d \n",acaEsta->info.nombre, acaEsta->info.valor);
		
				   acaEsta = findVariable("ola", variablesDeclaradas);
	if(!acaEsta){
		printf("7) aca no esta \"ola\"\n");
	}
	
		printf("8) leerVariable:\n   ");	leer("jellllou", variablesDeclaradas);
	
	asignarVariable("jellllou", 88, &variablesDeclaradas);
		printf("9) reasignarVariable:\n   ");	leer("jellllou", variablesDeclaradas);
		
	asignarVariable("HOLAAAAAAAAAAAAAA", 22, &variablesDeclaradas);
		printf("10) No se encontro y se declaro %s = %d\n", variablesDeclaradas->info.nombre, variablesDeclaradas->info.valor);
	
		printf("11) Valor obtenido de chau: %d\n", obtenerValor("chau", variablesDeclaradas));

		printf("12) Operar 1+4=%d\n", operar(1,1,4));
		
		printf("13) Operar 1-4=%d\n", operar(1,-1,4));
	
	//**************
	
//	char* unaCadena = strdup("uno");
//	char* otraCadena = strdup(unaCadena);
//	unaCadena = strdup("dosssssssssssssssssssssssssssssssss");
//	printf("\n %s -> %s", otraCadena, unaCadena);
	
	return 0;
}
