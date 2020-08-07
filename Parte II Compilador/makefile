all:
# traduce el codigo de flex a C, creando el archivo lex.yy.c
flex scanner.l

# traduce el codigo de bison a C, creando los archivos bison.tab.c y bison.tab.h
bison -d bison.y	

# crea archivo ejecutable
gcc lex.yy.c tabla_de_valores.c bison.tab.c -o ejecutable.exe 

# Ejecutar programa
ejecutable.exe

test:
cd tests
gcc ../tabla_de_valores.c funciones_testeadas.c -o tests.exe
tests.exe

clean:
rm lex.yy.c bison.tab.c bison.tab.h