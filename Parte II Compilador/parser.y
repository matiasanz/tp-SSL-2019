%{
  #include "tabla_de_valores.h"

  extern FILE*yyin;
  extern int yylex(void);
  extern void yyerror(char*);
  
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

//Error
void yyerror(char*s){
  printf(">> Error de compilacion\n\n");
}

int yywrap(){
 return 1;
}