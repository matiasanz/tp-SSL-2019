Comandos para compilar con flex y byson desde windows:

flex scanner.l	    	#crea archivo lex.yy.c
bison -d bison.y	#crea bison.tab.c y bison.tab.h
gcc lex.yy.c bison.tab.c -o ejecutable.exe #crea archivo ejecutable
ejecutable.exe
