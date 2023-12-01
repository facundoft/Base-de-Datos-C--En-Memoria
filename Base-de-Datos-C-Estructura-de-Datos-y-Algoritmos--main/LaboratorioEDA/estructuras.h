#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include<iostream>
#include<string.h>

using namespace std;


typedef  char *cadena;


union valor_celda{
	int ival;
	char * sval;
};

typedef struct _celda {
	valor_celda * valor;
	_celda *abajo;
}*celda;


typedef struct _columna{
	string nombre;
	string tipo;
	string calificador;
	_columna * sigCol;
	celda primera=NULL;
}*columna;

typedef struct _tabla{
	string nombre;
	columna columnas = NULL;
	int tuplas=0;
}tabla;



typedef struct nodo {
	string valor;
	nodo * sig;
}*lista;

typedef enum _opcion{
	CreateTable, DropTable, InsertInto, DeleteTupla, AddCol, DropCol, PrintDataTable, Fin, Error
}opcion;


typedef enum retorno{
	OK, ERROR, NO_IMPLEMENTADA
}TipoRet;

#endif
