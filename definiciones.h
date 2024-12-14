#include "estructuras.h"


//Retornos
TipoRet insertInto (string nombreTabla, string columnaTupla, string valoresTupla);
TipoRet createTable (string nombreTabla);
TipoRet addCol(string nombreTabla, string nombreCol, string tipoCol, string calificadorCol);
TipoRet dropCol (string nombreTabla, string  nombreCol);
TipoRet dropTable (string nombreTabla);
TipoRet deleteTupla (string  nombreTabla, string  condicionEliminar);
TipoRet printDataTable (string  nombreTabla, string ordenadaPor);
//Funciones
tabla crearTabla(string nombre);
columna crearCol(tabla t, string nombreCol, string tipoCol, string calificadorCol);
lista separar(string cadena, char caracter);
void insertarTupla(tabla t, lista nombresCol, lista valores);
tabla eliminarTabla(tabla t);
void eliminarCol(tabla t, columna col);
tabla eliminarColUnica(tabla t);
celda insertarCelda(columna col, string valor);
void imprimirTabla(tabla t);
void eliminarTupla(tabla t, int pos);
void eliminarTuplas(tabla & t, columna colComparar,string condicion,string valor);
lista insertoPrincipio(lista cabezal,string x);
lista separar(string cadena, char caracter);
void muestroRetorno(TipoRet retorno);
void ayudas();
void fin();
