#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <stdexcept>
#include <utility>

#define SIZEOF(x)  (sizeof(x) / sizeof((x)[0]))

using namespace std;

ifstream is("HaxLogs.txt");
string token="";
string tipo="";
string tokenReconhecido = "";
string palavras[25];

string SE = "Simbolo Especial";
string ID = "Identificador";
string PR = "Palavra Reservada";
string NU = "Numero";
string CO = "Comentario";

class Automato {
 public:
 	void charesp(char c);
 	void id_pr(char c);
 	void numero(char c);
 	void obter_token();
 	void programa();
    void bloco();
    void deftipo();
    void defvar();
    void srotina();
    void lident();
    void defproc();
    void deffunc();
    void parform();
    void comcomp();
    void srot();
    void atr();
    void proc();
    void comcond();
    void comrep();
    void leia();
    void imp();
    void listexp();
    void exp();
    void rel();
    void expsim();
    void termo();
    void fator();
    void func();
};

void Automato::charesp(char c){
    if(is.eof()){
            return;
    }

	int estado = 1;
	token += c;
	if(((c==';')||(c==',')||(c==')')||(c=='@')||(c=='=')||(c=='{')||(c=='}'))){
        estado = 2;
    }else{
        if(c=='+'){
            estado = 3;
        }else if(c=='-'){
            estado = 4;
        }else if(c=='<'){
            estado = 5;
        }else if((c=='>')||(c==':')){
            estado = 6;
        }else if(c=='*'){
            estado = 7;
        }else if(c=='.'){
            estado = 8;
        }else if(c=='/'){
			estado = 9;
		}else if(c=='('){
			estado = 12;
		}else{
			if(c==' '||c=='\n'||c=='\r'||c=='\t'){
			    token = "";
			    is.get(c);
                if(isalpha(c)||c=='@'){
                    id_pr(c);
                }else if(isdigit(c)||c=='-') {
                    numero(c);
                }else charesp(c);

				return;
			}else{
				printf("Nao Reconhecido: %s \n", token.c_str());
        		exit(1);
			}
		}
	}
	try{
        while(is.get(c)){
            switch (estado){
               case 2:
                    tipo = SE;
                    is.unget();
                    return;

               case 3:
                    if(c=='+'){
                        token += c;
                        estado = 2;
                        break;
                    }else{
                        tipo = SE;
                        is.unget();

                        return;
                    }
               break;

               case 4:
                    if(c=='-'){
                        token += c;
                        estado = 2;
                        break;
                    }else{
                        is.unget();
                        tipo = SE;
                        return;
                    }
               break;

               case 5:
                    if((c=='>')||(c=='=')){
                        token += c;
                        estado = 2;
                        break;
                    }else{
                        is.unget();
                        tipo = SE;
                        return;
                    }
               break;

               case 6:
                    if(c=='='){
                        token += c;
                        estado = 2;
                        break;
                    }else{
                        is.unget();
                        tipo = SE;
                        return;
                    }
               break;

               case 7:
                    if(c=='*'){
                        token += c;
                       estado = 2;
                        break;
                    }else{
                        is.unget();
                        tipo = SE;
                        return;
                    }
               break;

               case 8:
                    if(c=='.'){
                       token += c;
                       estado = 2;
                        break;
                    }else{
                        is.unget();
                        tipo = SE;
                        return;
                    }
               break;

                case 9:
                   if(c=='/'){
                        token += c;
                        estado = 10;
                    }else{
                        is.unget();
                        tipo = SE;
                        return;
                    }
               break;

               case 10:
                    if(c=='/'){
                        estado = 11;
                    }
               break;

               case 11:
                    if(c=='/'){
                        token+='/';
                        token += c;
                        estado = 15;
                    }else{
                        estado = 10;
                    }
               break;

               case 12:
                   if(c=='<'){
                        estado = 13;
                        token += c;
                    }else{
                        is.unget();
                        tipo = SE;
                        return;
                    }
               break;

               case 13:
                    if(c=='>'){
                        estado = 14;
                    }
               break;

               case 14:
                    if(c==')'){
                        token+='>';
                        estado = 15;
                        token += c;
                    }else{
                        estado = 13;
                    }
               break;

               case 15:
                is.unget();
                tipo = CO;
                return;


               default:
                 exit(1);
                 break;
            }
        }
        if(estado==15){
            tipo = CO;
            return;
        }
        if(estado==2||estado==3||estado==4||estado==5||estado==6||estado==9||estado==11||estado==12||estado==14){
            tipo = SE;
            return;
        }
    }catch (int e){
 	 	 	cerr << "Erro: " << e << "\n";
    }
}

void Automato::id_pr(char c){
    if(is.eof()){
            return;
    }

	int estado = 1;

	if(isalpha(c)){
	   	estado=2;
	   	token+=c;
	}else if(c=='@'){
		estado=6;
		token+=c;
	}else{
		if(c==' '||c=='\n'||c=='\r'){
				is.unget();
				return;
			}else{
				printf("Nao Reconhecido: %s \n", token.c_str());
        		exit(1);
			}
	}

	while(is.get(c)){
		switch(estado){
   	    	case 2:
   	    		if(c=='@'){
					estado=7;
					token+=c;
				}
				else if(c=='_'){
					estado=5;
					token+=c;
				}
				else if(isalpha(c)){
					estado=8;
					token+=c;
				}
				else {
                    is.unget();
                    tipo = ID;
					return;
				}
            break;

            case 3:
            	if(isalpha(c)||isdigit(c)){
                    token+=c;
					estado=3;
				}
				else if(c=='@'){
				    token+=c;
					estado=4;
				}
				else {
                    is.unget();
                    tipo = ID;
					return;
				}
   	        break;

   	        case 4:
                is.unget();
                tipo = ID;
            	return;
   	        break;

   	        case 5:
            	if(isalpha(c)||isdigit(c)){
                    token+=c;
					estado=3;
				}
				else {
                    token += c;

					printf("Nao Reconhecido: %s \n",token.c_str());
					exit(1);
				}
   	        break;

   	        case 6:
            	if(isalpha(c)){
                    token+=c;
					estado=2;
				}else if(c=='@'){
					estado = 9;
	           		token += c;
				}else{
                    is.unget();
                    tipo = SE;
					return;
				}
   	        break;

   	        case 7:
            	if(c=='@'){
                    token+=c;
					estado=7;
				}
				else if(isalpha(c)){
				    token+=c;
					estado=3;
				}
				else {
                    token += c;
					printf("Nao Reconhecido: %s \n",token.c_str());
					exit(1);
				}
   	        break;

   	        case 8:
   	        	if(isalpha(c)){
                    token+=c;
					estado=8;
				}else{
				    for(int i = 0; i<SIZEOF(palavras);i++){
                        if(token == palavras[i]){
                            is.unget();
                            tipo = PR;
                            return;
                        }
                    }
                    is.unget();
                    tipo = ID;
					return;
				}
   	        break;

   	    	case 9:
    			if(c=='\n'){
					is.unget();
                    tipo = CO;
					return;
				}
  	   	   	break;

   	        default:
   	         exit(1);
             return;
             break;
		}
	}

	if(estado==4||estado==3||estado==2){
		tipo = ID;
	}
	if(estado==8){
		for(int i = 0; i<SIZEOF(palavras);i++){
            if(token == palavras[i]){
                tipo = PR;
                break;
            }
        }
	}
	if(estado==9){
		tipo = CO;
	}

}

void Automato::numero(char c){
    if(is.eof()){
            return;
    }

	int estado=1;
	token += c;
	if(isdigit(c)){
        estado=2;
    }else if(c=='-'){
        estado=5;
    }else{
       if(c==' '||c=='\n'||c=='\r'){

				return;
			}else{
				printf("Nao Reconhecido: %s \n", token.c_str());
        		exit(1);
			}
    }

	while(is.get(c)){
		switch(estado){
            case 2:
                if(isdigit(c)){
					estado=2;
                    token += c;
				}else if(c==','){
					estado=3;
					token += c;
				}else{
                    is.unget();
                    tipo = NU;
					return;
				}
            break;

            case 3:
            	if(isdigit(c)){
					estado=4;
					token += c;
				}
				else{
                    token += c;
                    printf("Nao Reconhecido: %s \n", token.c_str());
					exit(1);
				}
   	        break;

   	        case 4:
            	if(isdigit(c)){
					estado=4;
					token += c;
				}else{
                    is.unget();
                    tipo = NU;
                    return;
				}
   	        break;

   	        case 5:
            	if(isdigit(c)){
					estado=2;
					token += c;
				}else if(c=='-'){
				    estado = 6;
				    token += c;
				}else{
                    is.unget();
                    tipo = SE;
			   		return;
				}
   	        break;

   	        case 6:
                is.unget();
                tipo = SE;
                return;
            break;

   	        default:
             return;
             break;
		}
	}
	if(estado==2||estado==4){
		tipo = NU;
	}

	if(estado == 5 || estado == 6){
	    tipo = SE;
	}

}

void Automato::obter_token(){
    char c;
    is.get(c);
    if(isalpha(c)||c=='@'){
        id_pr(c);
    }else if(isdigit(c)||c=='-') {
        numero(c);
    }else charesp(c);

    if(tipo == CO){
        token = "";
        obter_token();
        return;
    }

    cout << "Tipo: " << tipo << " -> " << token << endl << endl;
    tokenReconhecido = token;
    token = "";
}

Automato a;
int main(){
    ifstream arquivo_pr("palavras.txt");
    for(int i = 0; i<25;i++){
        std::getline(arquivo_pr, palavras[i]);
    }
    arquivo_pr.close();
    a.programa();
    return 0;
}

void Automato::programa(){
    obter_token();
    if(tokenReconhecido!="programa"){
        cerr << "Erro: \"programa\" esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tipo!=ID){
        cerr << "Erro: Identificador esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tokenReconhecido!=";"){
        cerr << "Erro: \";\" esperado " << endl;
        exit(1);
    }

    bloco();

    obter_token();
    if(tokenReconhecido!="."){
        cerr << "Erro: \".\" esperado " << endl;
        exit(1);
    }
}

void Automato::bloco(){
    obter_token();
    cout << tokenReconhecido<< endl;
    if(tokenReconhecido=="tipo"){
        deftipo();
    }
    if(tokenReconhecido=="var"){
        defvar();
    }
    if(tokenReconhecido=="procedimento"||tokenReconhecido=="funcao"){
        srotina();
    }
    comcomp();
}

void Automato::deftipo(){

    obter_token();
    if(tipo!=ID){
        cerr << "Erro: Identificador esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tokenReconhecido!="="){
        cerr << "Erro: \"=\" esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tipo!=ID){
        cerr << "Erro: Identificador esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tokenReconhecido!=";"){
        obter_token();
        cerr << "Erro: \";\" esperado " << endl;
        exit(1);
    }

    obter_token();
    while(tipo==ID){
        obter_token();
        if(tokenReconhecido!="="){
            cerr << "Erro: \"=\" esperado " << endl;
            exit(1);
        }
        obter_token();
        if(tipo!=ID){
            cerr << "Erro: Identificador esperado " << endl;
            exit(1);
        }
        obter_token();
        if(tokenReconhecido!=";"){
            obter_token();
            cerr << "Erro: \";\" esperado " << endl;
            exit(1);
        }
        obter_token();
    }
}

void Automato::defvar(){
    obter_token();
    lident();
    if(tokenReconhecido!=":"){
        cerr << "Erro: \":\" esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tipo!=ID){
        cerr << "Erro: Identificador esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tokenReconhecido!=";"){
        cerr << "Erro: \";\" esperado " << endl;
        exit(1);
    }
    obter_token();
    while(tipo==ID){
        lident();
        if(tokenReconhecido!=":"){
            cerr << "Erro: \":\" esperado " << endl;
            exit(1);
        }
        obter_token();
        if(tipo!=ID){
            cerr << "Erro: Identificador esperado " << endl;
            exit(1);
        }
        obter_token();
        if(tokenReconhecido!=";"){
            cerr << "Erro: \";\" esperado " << endl;
            exit(1);
        }
        obter_token();
    }
}

void Automato::lident(){
    if(tipo!=ID){
        cerr << "Erro: Identificador esperado " << endl;
        exit(1);
    }
    obter_token();
    while(tokenReconhecido==","){
        obter_token();
        if(tipo!=ID){
            cerr << "Erro: Identificador esperado " << endl;
            exit(1);
        }
        obter_token();
    }
}

void Automato::srotina(){
    while(tokenReconhecido=="procedimento"||tokenReconhecido=="funcao"){
        if(tokenReconhecido=="procedimento"){
            defproc();
        }else{
            deffunc();
        }

        obter_token();
        if(tokenReconhecido!=";"){
            cerr << "Erro: \";\" esperado " << endl;
            exit(1);
        }

        obter_token();
    }
}

void Automato::defproc(){
    obter_token();
    if(tipo!=ID){
        cerr << "Erro: Identificador esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tokenReconhecido=="("){
        parform();
        obter_token();
    }
    if(tokenReconhecido!=";"){
        cerr << "Erro: \";\" esperado " << endl;
        exit(1);
    }
    bloco();
}

void Automato::deffunc(){
    obter_token();
    if(tipo!=ID){
        cerr << "Erro: Identificador esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tokenReconhecido=="("){
        parform();
        obter_token();
    }
    if(tokenReconhecido!=":"){
        cerr << "Erro: \":\" esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tipo!=ID){
        cerr << "Erro: Identificador esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tokenReconhecido!=";"){
        cerr << "Erro: \";\" esperado " << endl;
        exit(1);
    }
    bloco();
}

void Automato::parform(){
    obter_token();
    if(tokenReconhecido=="var"){
       obter_token();
    }
    lident();
    if(tokenReconhecido!=":"){
        cerr << "Erro: \":\" esperado " << endl;
        exit(1);
    }
    obter_token();
    if(tipo!=ID){
        cerr << "Erro: Identificador esperado " << endl;
        exit(1);
    }
    obter_token();
    while(tokenReconhecido==";"){
        obter_token();
        if(tokenReconhecido=="var"){
            obter_token();
        }
        lident();
        if(tokenReconhecido!=":"){
            cerr << "Erro: \":\" esperado " << endl;
            exit(1);
        }
        obter_token();
        if(tipo!=ID){
            cerr << "Erro: Identificador esperado " << endl;
            exit(1);
        }
        obter_token();
    }
    if(tokenReconhecido!=")"){
        cerr << "Erro: \")\" esperado " << endl;
        exit(1);
    }
}

void Automato::comcomp(){
    if(tokenReconhecido!="inicio"){
        cerr << "Erro: \"inicio\" esperado " << endl;
        exit(1);
    }
    obter_token();
    srot();

    if(tokenReconhecido!=";"){
        cerr << "Erro: \";\" esperado " << endl;
        exit(1);
    }
    obter_token();
    while(tokenReconhecido=="se"||
          tokenReconhecido=="enquanto"||
          tokenReconhecido=="leia"||
          tokenReconhecido=="imprima"||
          tipo==ID){
        srot();
        if(tokenReconhecido!=";"){
            cerr << "Erro: \";\" esperado " << endl;
            exit(1);
        }
        obter_token();
    }
    if(tokenReconhecido!="fim"){
        cerr << "Erro: \"fim\" esperado " << endl;
        exit(1);
    }
}

void Automato::srot(){
    if(tipo==ID){
        obter_token();
        if(tokenReconhecido==":="){
            atr();
        }else if(tokenReconhecido=="("){
            proc();
            obter_token();
        }else{
            cerr << "Erro: Comando sem rotulo esperado " << endl;
            exit(1);
        }
    }else if(tokenReconhecido=="se"){
        comcond();
    }else if(tokenReconhecido=="enquanto"){
        comrep();
    }else if(tokenReconhecido=="leia"){
        leia();
        obter_token();
    }else if(tokenReconhecido=="imprima"){
        imp();
        obter_token();
    }else{
        cerr << "Erro: Comando sem rotulo esperado " << endl;
        exit(1);
    }
}

void Automato::atr(){
    if(tokenReconhecido!=":="){
        cerr << "Erro: \":=\" esperado " << endl;
        exit(1);
    }
    exp();
}

void Automato::proc(){
    if(tokenReconhecido!="("){
        return;
    }
    listexp();
    if(tokenReconhecido!=")"){
        cerr << "Erro: \")\" esperado " << endl;
        exit(1);
    }
}

void Automato::comcond(){
    exp();
    if(tokenReconhecido!="entao"){
        cerr << "Erro: entao esperado " << endl;
        exit(1);
    }
    obter_token();
    srot();
    if(tokenReconhecido!="senao"){
        return;
    }
    obter_token();
    srot();
}

void Automato::comrep(){
    exp();
    if(tokenReconhecido!="do"){
        cerr << "Erro: \"do\" esperado " << endl;
        exit(1);
    }
    obter_token();
    srot();
}

void Automato::leia(){
    obter_token();
    if(tokenReconhecido!="("){
        cerr << "Erro: \"(\" esperado " << endl;
        exit(1);
    }
    obter_token();
    lident();
    if(tokenReconhecido!=")"){
        cerr << "Erro: \")\" esperado " << endl;
        exit(1);
    }
}

void Automato::imp(){
    obter_token();
    if(tokenReconhecido!="("){
        cerr << "Erro: \"(\" esperado " << endl;
        exit(1);
    }
    listexp();
    if(tokenReconhecido!=")"){
        cerr << "Erro: \")\" esperado " << endl;
        exit(1);
    }
}

void Automato::listexp(){
    exp();
    while(tokenReconhecido==";"){
        exp();
    }
}

void Automato::exp(){
    expsim();
    if(tokenReconhecido!="="&&
       tokenReconhecido!="<>"&&
       tokenReconhecido!="<"&&
       tokenReconhecido!=">"&&
       tokenReconhecido!=">="&&
       tokenReconhecido!="<="){
        return;
    }
    expsim();
}

void Automato::expsim(){
    obter_token();
    if(tokenReconhecido=="+"||tokenReconhecido=="-"){
        obter_token();
    }
    termo();
    while(tokenReconhecido=="+"||tokenReconhecido=="-"||tokenReconhecido=="ou"){
        obter_token();
        termo();
    }
}

void Automato::termo(){
    fator();
    while(tokenReconhecido=="*"||tokenReconhecido=="div"||tokenReconhecido=="e"){
        obter_token();
        fator();
    }
}

void Automato::fator(){
    if(tipo==ID){
        obter_token();
        if(tokenReconhecido=="("){
            proc();
            obter_token();
        }
    }else if(tipo==NU){
        obter_token();
    }else if(tokenReconhecido=="("){
        exp();
        if(tokenReconhecido!=")"){
            cerr << "Erro: \")\" esperado " << endl;
            exit(1);
        }
        obter_token();
    }else{
        cerr << "Erro: Fator esperado " << endl;
        exit(1);
    }
}
