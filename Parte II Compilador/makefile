all:
# traduce el codigo de flex a C, creando el archivo lex.yy.c
flex scanner.l

# traduce el codigo de bison a C, creando los archivos bison.tab.c y bison.tab.h
bison -d parser.y	

# crea archivo ejecutable
gcc lex.yy.c tabla_valores.c parser.tab.c -o compilador.exe 

# Ejecutar programa
compilador.exe