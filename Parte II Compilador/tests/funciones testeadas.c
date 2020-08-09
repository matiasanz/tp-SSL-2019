#include "../tabla_valores.h"

variable variableCreate(char* identificador, int valor){
	return (variable){identificador, valor};
}

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

//Tests
int main(){
	nodoVariable*variablesDeclaradas = NULL;
	
	variable var = variableCreate("Hola", 50);
		printf("1) test variableCreate: %s = %d\n",var.nombre, var.valor);
	
	addVariable(var, &variablesDeclaradas);
		printf("2) test addVariable: %s = %d\n",variablesDeclaradas->info.nombre, variablesDeclaradas->info.valor);
	
	declararVariable("jellllou", 2, &variablesDeclaradas);
		printf("3) test declararVariable: %s = %d\n",variablesDeclaradas->info.nombre, variablesDeclaradas->info.valor);
	
	declararVariable("chau", -1, &variablesDeclaradas);
		printf("4) test declarar otra Variable: %s = %d\n",variablesDeclaradas->info.nombre, variablesDeclaradas->info.valor);
	
	nodoVariable* variableAnterior = variablesDeclaradas->sig;
		printf("5) siguiente Variable: %s = %d aguein\n",variableAnterior->info.nombre, variableAnterior->info.valor);
		
	nodoVariable* acaEsta = findEntrada("Hola", variablesDeclaradas);
		printf("6) findEntrada:  %s = %d \n",acaEsta->info.nombre, acaEsta->info.valor);
		
				   acaEsta = findEntrada("ola", variablesDeclaradas);
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
		
	return 0;
}
