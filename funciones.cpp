#include "definiciones.h"
#include<fstream>
void ayudas(){
	fstream fIn;
	fIn.open( "./ascii-art.txt", ios::in );
	
	if( fIn.is_open() )
	{
		string s;
		while( getline( fIn, s ) )
		{
			cout << s << endl;
			// Tokenize s here into columns (probably on spaces)
		}
		fIn.close();
	}
	else
	   cout << "Error opening file " << errno << endl;
	cout<<"Comandos:"<<endl;
	cout<<" CreateTable"<<endl<<" DropTable"<<endl<<" InsertInto"<<endl<<" DeleteTupla"<<endl<<" AddCol"<<endl<<" DropCol"<<endl<<" PrintDataTable"<<endl<<" Fin \"()\""<<endl;
}

tabla crearTabla(string nombre){
	tabla nuevaTabla;
	nuevaTabla.nombre = nombre;
	nuevaTabla.columnas=NULL;
	return nuevaTabla;
}

columna crearCol(tabla t, string nombreCol, string tipoCol, string calificadorCol){
	columna nuevaCol=new _columna;
	nuevaCol->nombre=nombreCol;
	nuevaCol->calificador=calificadorCol;
	nuevaCol->tipo=tipoCol;
	nuevaCol->sigCol=t.columnas;
	return nuevaCol;
}
celda insertarCelda(columna col, string valor){
	celda nuevaCelda = new _celda;
	nuevaCelda->valor = new valor_celda;
	if(col->tipo=="int"){
		if(valor=="EMPTY"){
			nuevaCelda->valor->sval=new char;
			strcpy(nuevaCelda->valor->sval,valor.c_str());
			nuevaCelda->valor->ival=0;
		}else{
			
			nuevaCelda->valor->ival = stoi(valor);	
		}
	}else{
		nuevaCelda->valor->sval=new char;
		strcpy(nuevaCelda->valor->sval,valor.c_str());
	}
	nuevaCelda->abajo=col->primera;
	return nuevaCelda;
}


void insertarTupla(tabla t, lista nombresCol, lista valores){
	columna auxCol=t.columnas;

	
	while(nombresCol!=NULL){
		while(auxCol!=NULL){
			if(nombresCol->valor == auxCol->nombre){
				auxCol->primera=insertarCelda(auxCol,valores->valor );
				break;
			}else{
				auxCol=auxCol->sigCol;
			}
		}
		nombresCol=nombresCol->sig;
		valores = valores->sig;
		auxCol=t.columnas;
	}
}
tabla eliminarTabla(tabla t){
	celda auxCelda;
	columna auxCol;
	while(t.columnas){
		while (t.columnas->primera){
			auxCelda = t.columnas->primera;
			t.columnas->primera = t.columnas->primera->abajo;
			delete auxCelda;
		}
		auxCol = t.columnas;
		t.columnas = t.columnas->sigCol;
		delete auxCol;
	}
	tabla t0 = crearTabla("");
	return t0;
}


void eliminarCol(tabla t, columna colAnterior){

	celda aux;
	columna colDeleted = colAnterior->sigCol;
	while(colDeleted->primera){
		aux = colDeleted->primera;
		colDeleted->primera = colDeleted->primera->abajo;
		delete aux;
	}
	colAnterior->sigCol = colDeleted->sigCol;
	delete colDeleted;
}

tabla eliminarColUnica(tabla t){
	celda aux;
	while(t.columnas->primera){
		aux = t.columnas->primera;
		t.columnas->primera = t.columnas->primera->abajo;
		delete aux;
	}
	t.columnas = NULL;
	return t;
}

void imprimo(lista cabezal){
	while(cabezal){
		cout<<cabezal->valor<<"->";
		cabezal=cabezal->sig;
	}
	cout<<"FIN";
}
lista insertoPrincipio(lista cabezal,string x){
	lista auxcabezal=new nodo;
	auxcabezal->valor=x;
	auxcabezal->sig=cabezal;
	return auxcabezal; 
}
		
lista separar(string cadena, char caracter){
	lista l = NULL;
	int pos;
	while(cadena.find(caracter)!=string::npos){
		pos = cadena.find_first_of(caracter,0);
		l = insertoPrincipio(l,cadena.substr(0,pos));
		cadena = cadena.substr(pos + 1,cadena.length());
	}
	
	l = insertoPrincipio(l,cadena);
	return l;
}

void muestroRetorno(TipoRet retorno){
    switch (retorno)
    {
    case OK:
        cout << "Ok"<< endl;
        break;
    case ERROR:
        cout << "Error"<<endl;
        break;
    case NO_IMPLEMENTADA:
        cout << "No Implementada"<<endl;
        break;
    }
}

void imprimirTabla(tabla t){
	columna auxCol = t.columnas;
	celda auxCelda;
	while(auxCol!=NULL){
		cout<<auxCol->nombre<<"-";
		auxCol= auxCol->sigCol;
	}
	cout<<endl;
	auxCol = t.columnas;
	for(int i=0;i<t.tuplas;i++){
		while(auxCol!=NULL){
			auxCelda=auxCol->primera;
			for(int j =0;j<i;j++){
				auxCelda= auxCelda->abajo;
			}
			if(auxCol->tipo=="string"){
				cout << auxCelda->valor->sval;
			}else{
				if(auxCelda->valor->ival){
					cout << auxCelda->valor->ival;
				}else{
					if(auxCelda->valor->sval){
						cout<<"EMPTY";
					}else cout<< 0;
				}
			cout<<"-";
			}
			auxCol=auxCol->sigCol;
		}
		cout<<endl;
		auxCol=t.columnas;
		
	}
}
	
	
void eliminarTuplas(tabla & t,columna colComparar,string condicion,string valor){
	int i = 0;
	celda auxCelda = colComparar->primera;
	
	if(condicion=="="){
		if( colComparar->tipo=="string"){
			while(auxCelda){
				if(auxCelda->valor->sval==valor){
					eliminarTupla(t,i);
					t.tuplas=t.tuplas-1;
					i=0;
					auxCelda = colComparar->primera;
				}else{
					i++;
					auxCelda=auxCelda->abajo;
				}
			}
		}else{
			while(auxCelda){
				if(auxCelda->valor->ival==stoi(valor)){
					eliminarTupla(t,i);
					t.tuplas=t.tuplas-1;
					i=0;
					auxCelda = colComparar->primera;
				}else{
					i++;
					auxCelda=auxCelda->abajo;
				}
			}
		}
	}
	if(condicion=="<"){
		if( colComparar->tipo=="string"){
			while(auxCelda){
				if(auxCelda->valor->sval<valor){
					eliminarTupla(t,i);
					t.tuplas=t.tuplas-1;
					i=0;
					auxCelda = colComparar->primera;
				}else{
					i++;
					auxCelda=auxCelda->abajo;
				}
			}
		}else{
			while(auxCelda){
				if(auxCelda->valor->ival<stoi(valor)){
					eliminarTupla(t,i);
					t.tuplas=t.tuplas-1;
					i=0;
					auxCelda = colComparar->primera;
				}else{
					i++;
					auxCelda=auxCelda->abajo;
				}
			}
		}
	}
	if(condicion==">"){
		if( colComparar->tipo=="string"){
			while(auxCelda){
				if(auxCelda->valor->sval>valor){
					eliminarTupla(t,i);
					t.tuplas=t.tuplas-1;
					i=0;
					auxCelda = colComparar->primera;
				}else{
					i++;
					auxCelda=auxCelda->abajo;
				}
			}
		}else{
			while(auxCelda){
				if(auxCelda->valor->ival>stoi(valor)){
					eliminarTupla(t,i);
					t.tuplas=t.tuplas-1;
					i=0;
					auxCelda = colComparar->primera;
				}else{
					i++;
					auxCelda=auxCelda->abajo;
				}
			}
		}
	}
	if(condicion=="<>"){
		if( colComparar->tipo=="string"){
			while(auxCelda){
				if(auxCelda->valor->sval!=valor){
					eliminarTupla(t,i);
					t.tuplas=t.tuplas-1;
					i=0;
					auxCelda = colComparar->primera;
				}else{
					i++;
					auxCelda=auxCelda->abajo;
				}
			}
		}else{
			while(auxCelda){
				if(auxCelda->valor->ival!=stoi(valor)){
					eliminarTupla(t,i);
					t.tuplas=t.tuplas-1;
					i=0;
					auxCelda = colComparar->primera;
				}else{
					i++;
					auxCelda=auxCelda->abajo;
				}
			}
		}
	}
	
}

void eliminarTupla(tabla t, int pos){
	if(!t.columnas->primera)return;
	if(pos>t.tuplas)return;
	columna auxCol = t.columnas;
	celda auxCelda;
	celda eliminada;
	if(pos==0){
		while(auxCol){
			eliminada= auxCol->primera;
			auxCol->primera= auxCol->primera->abajo;
			delete eliminada;
			auxCol=auxCol->sigCol;
		}
		return;
	}
	while(auxCol){
		auxCelda=auxCol->primera;
		for(int i =1; i<pos;i++){
			auxCelda=auxCelda->abajo;
		}
		eliminada=auxCelda->abajo;
		if(!auxCelda->abajo){
			return;
		}
		auxCelda->abajo = auxCelda->abajo->abajo;
		delete eliminada;
		auxCol=auxCol->sigCol;
	}
}
void fin(){
	fstream fIn;
	fIn.open( "./fin.txt", ios::in );
	
	if( fIn.is_open() )
	{
		string s;
		while( getline( fIn, s ) )
		{
			cout << s << endl;
			// Tokenize s here into columns (probably on spaces)
		}
		fIn.close();
	}
	else
	   cout << "Error opening file " << errno << endl;
}
