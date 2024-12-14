#include"definiciones.h"

using namespace std;

int main (int argc, char *argv[]) {
	string nombreTabla;
	string columnas,tipoCol,calificadorCol;
	string valores;
	system("pwd");
	tabla t;
	/*muestroRetorno(createTable("Personas"));
	
	muestroRetorno(addCol("Personas" ,"Nombre", "string", "ANY"));
	
	muestroRetorno(addCol("Personas" ,"Edad", "int", "ANY"));
	muestroRetorno(addCol("Personas" ,"CI", "int", "PRIMARY KEY"));
	
	muestroRetorno(insertInto("Personas","CI:Nombre:Edad","000:111:12"));
	muestroRetorno(insertInto("Personas","CI:Edad","222:222"));
	muestroRetorno(insertInto("Personas","CI:Nombre","333:333"));
	
	muestroRetorno(insertInto("Personas","CI:Nombre:Edad","444:Samuel:12"));
	
	muestroRetorno(insertInto("Personas","CI:Nombre:Edad","555:555:555"));
	

	
	muestroRetorno(insertInto("Personas","CI:Edad:Nombre","888:888:888"));
	muestroRetorno(insertInto("Personas","CI:Nombre","777:Samuel"));
	muestroRetorno(insertInto("Personas","CI:Edad","666:666"));
	muestroRetorno(printDataTable("Personas","Nombre"));
	muestroRetorno(deleteTupla("Personas","CI<333"));
	muestroRetorno(dropTable("Personas"));*/
	string comando;

	ayudas();
	cout << "> ";
	string op;

	opcion numOp;
	while (numOp!=Fin){
		getline(cin,comando);
		if(comando.substr(comando.length()-1,1) ==")"){
			comando = comando.substr(0,comando.length()-1);
		}else{
			comando = "";
		}
		
		lista lComando= separar(comando ,'(');
		string parametros = lComando->valor;
		lista lParametros = separar(parametros,',');
		op=comando.substr(0,comando.find('(', 0));
		if (op=="DropTable"){
			numOp = DropTable;
		}else if (op=="InsertInto"){
			numOp = InsertInto;
		}else if (op=="AddCol"){
			numOp = AddCol;
		}else if (op=="DropCol"){
			numOp = DropCol;
		}else if (op=="PrintDataTable"){
			numOp = PrintDataTable;
		}else if (op=="CreateTable"){
			numOp = CreateTable;
		}else if (op=="DeleteTupla"){
			numOp = DeleteTupla;
		}else if (op=="Fin"){
			numOp = Fin;
		}else{
			cout<<"Comando invalido"<<endl;
			numOp=Error;
		}
		
		switch (numOp){
		case DropTable:
			nombreTabla =lComando->valor;
			muestroRetorno(dropTable(nombreTabla));
			break;
		case DropCol:
			nombreTabla =lParametros->sig->valor;
			columnas = lParametros->valor;
			muestroRetorno(dropCol(nombreTabla,columnas));
			break;
		case InsertInto:
			nombreTabla =lParametros->sig->sig->valor;
			columnas = lParametros->sig->valor;
			valores = lParametros->valor;
			muestroRetorno(insertInto(nombreTabla,columnas,valores));
			break;
		case AddCol:
			nombreTabla =lParametros->sig->sig->sig->valor;
			tipoCol =lParametros->sig->valor;
			columnas = lParametros->sig->sig->valor;
			calificadorCol = lParametros->valor;
			muestroRetorno(addCol(nombreTabla,columnas,tipoCol,calificadorCol));
			break;
		case CreateTable:
			nombreTabla = lComando->valor;
			muestroRetorno(createTable(nombreTabla));
			break;
		case PrintDataTable:
			if(lParametros->sig){
				nombreTabla =lParametros->sig->valor;
				columnas = lParametros->valor;
			}else{
				columnas="";
				nombreTabla = lComando->valor;
			}
			muestroRetorno(printDataTable(nombreTabla,columnas));
			break;
		case DeleteTupla:
				nombreTabla =lParametros->sig->valor;
				columnas = lParametros->valor;
				muestroRetorno(deleteTupla(nombreTabla,columnas));
			break;
		case Fin:
			fin();
			break;
		case Error:
			break;
		}
		cout<<">";
	}
	return 0;
}
