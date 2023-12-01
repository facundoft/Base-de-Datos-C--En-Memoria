#include "definiciones.h"
#define INT_MAX 2147483647
tabla t;


TipoRet createTable (string nombreTabla){
	t = crearTabla(nombreTabla);
	return OK;
}
TipoRet dropTable (string nombreTabla){
	if(t.nombre != nombreTabla){
		cout<< "Tabla "<<nombreTabla<<" no existente: ";
		return ERROR;
	}
	t = eliminarTabla(t);
	return OK;
}
TipoRet addCol(string nombreTabla, string nombreCol, string tipoCol, string calificadorCol){
	if(t.nombre != nombreTabla){
		cout<< "Tabla "<<nombreTabla<<" no existente: ";
		return ERROR;
	}
	columna auxCol=t.columnas;
	bool esPKEY = calificadorCol == "PRIMARY KEY";

	if(!esPKEY && calificadorCol != "ANY" && calificadorCol != "NOT EMPTY"){
		cout<<"Calificador invalido: ";
		return ERROR;
	}

	if(t.columnas!=NULL && t.columnas->primera!=NULL && calificadorCol!="ANY"){
		cout<<"Si la tabla posee tuplas el calificador de una columna nueva debe ser ANY: ";
		return ERROR;
	}
	
	if(tipoCol != "string" && tipoCol != "int"){
		cout<<"Tipo invalido: ";
		return ERROR;
	}
	while(auxCol){
		if(auxCol->nombre == nombreCol ){
			cout<<"Ya existe una columna con el nombre "<<nombreCol<<" :";
			return ERROR;
		}
		if( esPKEY && auxCol->calificador == "PRIMARY KEY" ){
			cout<< "Ya existe una PRIMARY KEY para la tabla: ";
			return ERROR;
		}
		auxCol=auxCol->sigCol;
	}
	
	t.columnas = crearCol(t, nombreCol, tipoCol, calificadorCol);
	for(int i=0;i<t.tuplas;i++){
		t.columnas->primera = insertarCelda(t.columnas, "EMPTY");
	}
	return OK;
}

TipoRet dropCol (string nombreTabla, string  nombreCol){
	columna colAnt = t.columnas;
	
	if(t.nombre != nombreTabla){
		cout<< "Tabla "<<nombreTabla<<" no existente: ";
		return ERROR;
	}
	if(t.columnas == NULL)return ERROR;
	if(t.columnas->nombre == nombreCol){
		t.columnas = t.columnas->sigCol;
		delete colAnt;
		return OK;
	}
	if(t.columnas->sigCol==NULL ){
		if(t.columnas->nombre==nombreCol){
			t = eliminarColUnica(t);
			return OK;
		}else{
			cout<< "No existe la columna "<<nombreCol<<": ";			
			return ERROR;
		}
	}

	while(colAnt->sigCol->nombre != nombreCol){
		colAnt = colAnt->sigCol;
		if(colAnt->sigCol==NULL && colAnt->nombre!=nombreCol){
			cout<< "No existe la columna "<<nombreCol<<": ";			
			return ERROR;
		}
		
	}
	eliminarCol(t, colAnt);
	return OK;
}
TipoRet insertInto ( string nombreTabla, string columnaTupla, string valoresTupla){
	if(t.nombre != nombreTabla){
		cout<< "Tabla "<<nombreTabla<<" no existente: ";
		return ERROR;
	}
	bool noExiste=true;
	columna auxCol = t.columnas;
	celda auxPkey;
	lista nombresCol = separar(columnaTupla, ':');
	lista auxNombres = nombresCol;
	lista valores = separar(valoresTupla, ':');
	lista auxValores = valores;
	
	while(auxNombres!=NULL){
		while(auxCol != NULL){
			if(auxNombres->valor == auxCol->nombre){
				if(auxCol->calificador=="PRIMARY KEY"){ //Depues de encontrar la columna con ese nombre, si es pkey
					auxPkey = auxCol->primera; // Va a la posicion de la primera celda de la columna encontrada
					while(auxPkey!=NULL){ 
						if(auxCol->tipo=="int"){ //Compara los valores de las celdas para saber si se repetirÃ¡ la pkey
							if( stoi(auxValores->valor) == auxPkey->valor->ival){
								cout<<"Primary key repetida: ";
								return ERROR;
							}
						}else{
							if( auxValores->valor == auxPkey->valor-> sval){
								cout<<"Primary key repetida: ";
								return ERROR;
							}
						}
						auxPkey = auxPkey->abajo;
					}
				}
				noExiste=false; // Despues de encontrar que el nombre exista en las columnas noExiste = false y rompe el while
				break;
			}
			auxCol=auxCol->sigCol; 
		}
		if(noExiste){ // Cuando sale del while, si noExiste retorna ERROR
			cout<<"No existe una columna con el nombre "<<auxNombres->valor<<": ";
			return ERROR;
		}
		noExiste=true; // noExiste es igual a true para empezar la comparacion con los siguientes nombres de columnas ingresados por el usuario
		auxCol=t.columnas; // auxCol pasa a ser la primera columna para recorrer posteriormente
		auxNombres = auxNombres->sig; // Pasa el siguiente nombre a evaluar
		auxValores = auxValores->sig; // Pasa el siguiente valor respectivo al nombre, para saber si el valor a ingresar es pkey y se repite
	}
	auxNombres= nombresCol;
	auxCol = t.columnas;
	noExiste=true;
	while(auxCol!=NULL){
		while(auxNombres != NULL){
			if(auxNombres->valor == auxCol->nombre){
				noExiste=false;
				break;
			}
			auxNombres=auxNombres->sig;
		}
		if(noExiste){// Si no hay valor para insertar en una columna de la tupla que se desea insertar
			if(auxCol->calificador != "ANY"){
				cout<<"No se puede dejar el campo "<<auxCol->nombre<<" vacio: "; // Si el calificador es distinto de any salta el error
				return ERROR;
			}else{
				nombresCol = insertoPrincipio(nombresCol,auxCol->nombre); // Si es ANY Le pasa la columna que no se especificó y el valor empty
				valores = insertoPrincipio(valores,"EMPTY");
			}
		}
		noExiste=true;
		auxNombres= nombresCol;
		auxCol = auxCol->sigCol;
	}
	insertarTupla(t, nombresCol,valores); // Si termina de recorrer y no salta ningun error, llama a la funcion que modifica la tabla
	t.tuplas++; // aumenta el contador de tuplas
	return OK;
}
	

TipoRet deleteTupla (string  nombreTabla, string  condicionEliminar){
	if(t.nombre != nombreTabla){
		cout<< "Tabla "<<nombreTabla<<" no existente: ";
		return ERROR;
	}
	int encuentra;
	columna Auxcol = t.columnas;
	if(Auxcol == NULL)return ERROR;
	if(condicionEliminar.find("<>")!=-1){
		encuentra=condicionEliminar.find("<>");
		while(Auxcol->nombre != condicionEliminar.substr(0,encuentra) && t.columnas!=NULL ){
			Auxcol=Auxcol->sigCol;
			if(Auxcol==NULL){
				return ERROR;
			}
		}
		eliminarTuplas(t, Auxcol,"<>",condicionEliminar.substr(encuentra+2,condicionEliminar.length()));
		return OK;
	}
	if(condicionEliminar.find("<")!=-1){
		encuentra=condicionEliminar.find("<");
		while(Auxcol->nombre != condicionEliminar.substr(0,encuentra) && t.columnas!=NULL ){
			Auxcol=Auxcol->sigCol;
			if(Auxcol==NULL){
				return ERROR;
			}
		}
		eliminarTuplas(t, Auxcol,"<",condicionEliminar.substr(encuentra+1,condicionEliminar.length()));
		return OK;
	}
	if(condicionEliminar.find(">")!=-1){
		encuentra=condicionEliminar.find(">");
		while(Auxcol->nombre != condicionEliminar.substr(0,encuentra) && t.columnas!=NULL ){
			Auxcol=Auxcol->sigCol;
			if(Auxcol==NULL){
				return ERROR;
			}
		}
		eliminarTuplas(t, Auxcol,">",condicionEliminar.substr(encuentra+1,condicionEliminar.length()));
		return OK;
	}

	if(condicionEliminar.find("=")!=-1){
		encuentra=condicionEliminar.find("=");
		while(Auxcol->nombre != condicionEliminar.substr(0,encuentra) && t.columnas!=NULL ){
			Auxcol=Auxcol->sigCol;
			if(Auxcol==NULL){
				return ERROR;
			}
		}
		eliminarTuplas(t, Auxcol,"=",condicionEliminar.substr(encuentra+1,condicionEliminar.length()));
		return OK;
	}
}

TipoRet printDataTable (string  nombreTabla, string ordenadaPor){
	bool repetido;
	bool encontroCol=false;
	columna mostrarCol=t.columnas;

	celda mostrarCelda;
	celda auxCelda;
	if(t.nombre != nombreTabla){
		cout<< "Tabla "<<nombreTabla<<" no existente: ";
		return ERROR;
	}
	columna auxCol = t.columnas;
	if(ordenadaPor==""){
		while(auxCol){
			if(auxCol->calificador=="PRIMARY KEY"){
				ordenadaPor= auxCol->nombre;
				break;
			}
			auxCol=auxCol->sigCol;
		}
		if(ordenadaPor == ""){
			imprimirTabla(t);
			return OK;
		}
	}

	auxCol = t.columnas;
	while(auxCol){
		if(auxCol->nombre==ordenadaPor){
			encontroCol=true;
			break;
		}
		auxCol=auxCol->sigCol;
	}
	if(!encontroCol){
		cout<<"Columna "<<ordenadaPor<<" no existente: ";
		return ERROR;
	}
	auxCelda = auxCol->primera;
	int orden [t.tuplas];

	bool esta=false;
	int i=0;
	string strMenor;
	int iMenor;
	int nroMenor=INT_MAX;
	string strMayor="";

	while(mostrarCol!=NULL){
		cout<<mostrarCol->nombre<<"-";
		mostrarCol= mostrarCol->sigCol;
	}
	cout<<endl;
	if(auxCol->tipo=="string"){
		strMenor=auxCol->primera->valor->sval;
	}
	for(int x=0;x<t.tuplas;x++){ // X va desde 0 a t.tuplas-1 para añadir al arreglo de los indices
		while(auxCelda){
			if(auxCol->tipo=="int"){				// Si es int

				if(auxCelda->valor->ival <= nroMenor){ // Si es menor al nroMenor
					for(int j = 0; j<=x;j++){
						if(orden[j]==i){
							esta = true;     //Verifica si el indice ya ha sido agregado al arreglo
						}else {
							if(auxCelda->valor->ival == nroMenor)repetido=true;
						}
					}
					if(!esta){
						nroMenor=auxCelda->valor->ival;	// Si no, nroMenor sera cambiado y guardado su indice
						if(repetido && auxCol->sigCol){	// HACER ALGO SI SE REPITE Y CAMBIAR IMENOR SEGUN SIGUIENTE COLUMNA
							
						}else{
							
						}
						iMenor = i;
					}
				}
			}
			else{								// Si es string
				if( x==0 && auxCelda->valor->sval >= strMayor){
					strMayor=auxCelda->valor->sval;
				}
				if(auxCelda->valor->sval <= strMenor){ // Si es menor al nroMenor
					for(int j = 0; j<=x;j++){
						if(orden[j]==i){
							esta = true;              //Verifica si el indice ya ha sido agregado al arreglo
						}
					}
					if(!esta){
						strMenor=auxCelda->valor->sval;	//Si no, strMenor sera cambiado y guardado su indice
						iMenor = i;
					}
				}
			}
		repetido=false;
		esta=false;
		auxCelda=auxCelda->abajo;				//Pasa a la celda de abajo para seguir buscando el menor
		i++;									//Aumenta i cada que pasa a la celda de abajo
		}													//FIN del while
	//	if(auxCol->tipo=="int"||strMenor != "EMPTY"){ Descomentar cuando este mostrarEmptys por sigCol
			auxCelda=auxCol->primera;
			mostrarCol= t.columnas;
			orden[x] = iMenor;
			while(mostrarCol!=NULL){
				mostrarCelda=mostrarCol->primera;
				for(int m=0;m<iMenor;m++){
					mostrarCelda=mostrarCelda->abajo;
				}
				
				if(mostrarCol->tipo=="string"){
					cout << mostrarCelda->valor->sval;
				}else{
					if(mostrarCelda->valor->ival){
						cout << mostrarCelda->valor->ival;
					}else{
						if(mostrarCelda->valor->sval){
							cout<<"EMPTY";
						}else cout<< 0;
						
					}
				}
				mostrarCol=mostrarCol->sigCol;
				cout<<"-";
			}
			cout<<endl;
		//} // Descomentar para cerrar el if
		i=0;										//Se resetea el contador i de bajadas de celdas
		nroMenor=INT_MAX;							//nroMenor es el maximo entero permitido por c para volver a comparar
		strMenor=strMayor;
	}
	return OK;
}

