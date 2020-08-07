%{
    #include<stdlib.h>
	#include"parser.tab.h"
    #include<string.h>

%}
LETRA						{MINUSCULA}|{MAYUSCULA}
MINUSCULA					[a-z]
MAYUSCULA					[A-Z]
DIGITO						[0-9]
ESPACIO						" "+

IDENTIFICADOR				{LETRA}({LETRA}|{DIGITO})*
NUMERO_ENTERO				{DIGITO}+
PALABRA_RESERVADA 			"INICIO"|"FIN"|"LEER"|"ESCRIBIR"
OPERADOR_DE_ASIGNACION	 	":="
OPERADOR_DE_ADICION			"+"
OPERADOR_DE_SUSTRACCION     "-"
SIGNO_DE_PUNTUACION 		"("|")"|";"

%%
{ESPACIO}                   /*omitir*/;
"INICIO"   /*P. Reservada*/ return INICIO;
"FIN"      /*P. Reservada*/ return FIN;
"ESCRIBIR" /*P. Reservada*/ return ESCRIBIR;
"LEER"     /*P. Reservada*/ return LEER;                  
{IDENTIFICADOR}		 		{yylval.cadena=strdup(yytext); return ID;}
{NUMERO_ENTERO}				{yylval.entero=atoi(yytext); return CONSTANTE;}
{OPERADOR_DE_ASIGNACION} 	return ASIGNACION;
{OPERADOR_DE_ADICION}		{yylval.signo=1; return OPERADOR_ADITIVO;}
{OPERADOR_DE_SUSTRACCION}   {yylval.signo=-1;return OPERADOR_ADITIVO;}
{SIGNO_DE_PUNTUACION}		return !strcmp(yytext,"(")? PARENTESIS_IZQ : !strcmp(yytext,")")? PARENTESIS_DER : PUNTOYCOMA;

%%