# include "tabla_de_valores.h"

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
