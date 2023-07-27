#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <utility>
#include <vector>
#include <algorithm>


using namespace std;

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores) {
	// actualizo la variable actual
	vector<pair<int, int>> distancias;
	vector<estado> objetv;
	vector<estado> objetv_real;
	vector<int> indices;
	int dManhattan0=0, dManhattan1=0, dManhattan2=0;

	actual.fila			= sensores.posF;
	actual.columna		= sensores.posC;
	actual.orientacion	= sensores.sentido;



	//capturo los destinos

	objetivos.clear();
	cout << "NUMERO DE DESTINOS: " << sensores.num_destinos << endl;
if (sensores.nivel==4){
	for (int i = 0; i<sensores.num_destinos; i++){
		estado aux;
		aux.fila = sensores.destino[2*i];
		aux.columna = sensores.destino[2*i+1];
		
		//objetivos.push_back(aux);
		objetv.push_back(aux);
		
	} 

	for (int i=0; i< objetv.size(); i++){
		pair <int, int> aux;
		if(i==0){
			distancias.push_back(aux);
			dManhattan0= abs(actual.fila - objetv[i].fila) + abs(actual.columna -objetv[i].columna);
			distancias[0].first=dManhattan0;
			distancias[0].second=0;

		}else if(i==1){
			distancias.push_back(aux);
			dManhattan1= abs(actual.fila - objetv[i].fila) + abs(actual.columna -objetv[i].columna);

			distancias[1].first=dManhattan1;
			distancias[1].second=1;
		}else{
			distancias.push_back(aux);
			dManhattan2= abs(actual.fila - objetv[i].fila) + abs(actual.columna -objetv[i].columna);
			distancias[2].first=dManhattan2;
			distancias[2].second=2;
		}
	}
	// cout << "DEPURACION "<< endl;
	// cout << "LAS PAREJAS SIN ORDENAR SON" << endl;
	// cout << "Distancia: " << distancias[0].first << " indice: "<< distancias[0].second << endl;
	// cout << "Distancia: " << distancias[1].first << " indice: "<< distancias[1].second << endl;
	// cout << "Distancia: " << distancias[2].first << " indice: "<< distancias[2].second << endl;

	sort(distancias.begin(), distancias.end());
	
	// cout << "LAS PAREJAS ORDENADAS SON" << endl;
	// cout << "Distancia: " << distancias[0].first << " indice: "<< distancias[0].second << endl;
	// cout << "Distancia: " << distancias[1].first << " indice: "<< distancias[1].second << endl;
	// cout << "Distancia: " << distancias[2].first << " indice: "<< distancias[2].second << endl;

	objetivos.push_back(objetv[distancias[0].second]);
	objetivos.push_back(objetv[distancias[1].second]);
	objetivos.push_back(objetv[distancias[2].second]);
	
	if(actual.destinosPendientes.empty()){
		
		actual.destinosPendientes=objetivos;
	}
	// Si no hay plan, construyo uno
	if (!hayPlan){
		hayPlan = pathFinding (sensores.nivel, actual, actual.destinosPendientes, plan);
	}

	cout << "LOS DESTINOS PENDIENTES SON" << endl;
	cout << "Fila : "<< actual.destinosPendientes.front().fila << " Columna: " << actual.destinosPendientes.front().fila;
	cout << "hayPlan==== "<< hayPlan << endl;
}else{
	for (int i = 0; i<sensores.num_destinos; i++){
		estado aux;
		aux.fila = sensores.destino[2*i];
		aux.columna = sensores.destino[2*i+1];
		
		objetivos.push_back(aux);
				
	} 

		// Si no hay plan, construyo uno
	if (!hayPlan){
		hayPlan = pathFinding (sensores.nivel, actual, actual.destinosPendientes, plan);
	}
}

	Action sigAccion;
	if (hayPlan and plan.size()>0){ //hay un plan no vacio
		if(sensores.nivel == 4){ 		//nivel 4::Reto
            actualizarMapa(sensores);

            if (bateriaBien){ //Si el jugador va bien de batería, busca objetivos
                cout << "Bateria Bien" << endl;
			contador++;


                if (pasos==0){

                    hayPlan =  pathFinding(sensores.nivel, actual, actual.destinosPendientes, plan);  // Recalculamos el plan cada X pasos
                    if (sensores.tiempo > 200)

                        pasos=-1; //plan de ultimo recurso, por si el personaje se queda sin tiempo. Consiste en acabar el ultima plan que haya establecido

                    else 
                        pasos=maxPasos;
                }


                if (mapaResultado[sensores.posF][sensores.posC]=='D' and zapatillas==false){
                    	actual.zapatillas=true;
						zapatillas=true;
						actual.bikini=false;
						bikini=false;
				}
                if (mapaResultado[sensores.posF][sensores.posC]=='K' and bikini==false){
                   		actual.bikini=true;
						bikini=true;
						actual.zapatillas=false;
						zapatillas=false;
				}


                if(HayObstaculoDelante(actual) and plan.front() == actFORWARD){
                		cout << "\nCambiando ruta..." << endl;
                		hayPlan = pathFinding(sensores.nivel, actual, actual.destinosPendientes, plan);
                }

                if (sensores.superficie[2]=='a' and plan.front() == actFORWARD){ //Si hay un aldeano en frente
                        return actIDLE;
                }

                if (mapaResultado[sensores.posF][sensores.posC] == 'X' and sensores.bateria < max(1600, int (sensores.tiempo)))
                    return actIDLE;

                if (sensores.bateria < bateriaMin){ //Si la batería está por debajo del mínimo 

                    if (encuentraBateria){ //El jugador repasa si ya ha encontrado la casilla de la bateria 

                        bateriaBien=false; //Si efectivamente ya la ha encontrado va hasta alli para recargar
                        bateriaMin = bateriaMin/2; // Se reduce el mínimo de batería a la mitad para no perder tiempo
                        pasos=1;

                    }
                }
                pasos --;
				
				
            }
			else{ //Si la batería es baja va a la casilla de Batería si la ha encontrado

                cout << "-------------------------->Bateria baja<------------------------" << endl;
                if (pasos==0){

                    hayPlan =  pathFinding_bateria(sensores.nivel, actual, bateria, plan);  
                    pasos=maxPasos;

                }
				
                if (mapaResultado[sensores.posF][sensores.posC]=='D' and zapatillas==false){
                    	actual.zapatillas=true;
						zapatillas=true;
						actual.bikini=false;
						bikini=false;
				}
                if (mapaResultado[sensores.posF][sensores.posC]=='K' and bikini==false){
                   		actual.bikini=true;
						bikini=true;
						actual.zapatillas=false;
						zapatillas=false;
				}
                if(HayObstaculoDelante(actual) and plan.front() == actFORWARD){

                	cout << "\nRecalculando ruta..." << endl;
                	hayPlan = pathFinding_bateria(sensores.nivel, actual, bateria, plan);
                }

                if (sensores.superficie[2]=='a' and plan.front() == actFORWARD){
                        return actIDLE;
                }

                if (mapaResultado[sensores.posF][sensores.posC] == 'X' and sensores.bateria < sensores.tiempo)
                    return actIDLE;


				
                if (sensores.bateria>= bateriaMax){
					cout<<"Ya he recargado completamente <--------------------------"<< endl;
                    bateriaBien=true; //Vuelve a buscar objetivos
                    bateriaMax=bateriaMax/2; 
					
                    pasos=maxPasos;
                    return actIDLE;
					
				}

                pasos --;
            }
		}
		sigAccion = plan.front(); //tomo la siguiente accion del Plan
		plan.erase(plan.begin());

	}
	else{
		cout << "Buscamos otro objetivo...\n";
		//objetivos.pop_front();
		
		actual.destinosPendientes.pop_front();
		if(actual.destinosPendientes.empty()){
		
			actual.destinosPendientes=objetivos;
		}
		
			if(mapaResultado[sensores.posF][sensores.posC] != 'X' or sensores.bateria>=bateriaMax){
			hayPlan = pathFinding(sensores.nivel, actual, actual.destinosPendientes, plan);
			}else if(!bateriaBien){
				return actTURN_R;
				pathFinding_bateria(sensores.nivel, actual, bateria, plan);  
			}
		
	}
  return sigAccion;
}


// Llama al algoritmo de busqueda que se usara en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const list<estado> &destino, list<Action> &plan){
	estado unObjetivo=objetivos.front();
	switch (level){
		case 0: cout << "Demo\n";
						
						cout << "fila: " << objetivos.front().fila << " col:" << objetivos.front().columna << endl;
			      return pathFinding_Profundidad(origen,unObjetivo,plan);
						break;

		case 1: cout << "Optimo numero de acciones\n";
						cout << "fila: " << objetivos.front().fila << " col:" << objetivos.front().columna << endl;
			      return pathFinding_Nivel1(origen,unObjetivo,plan);
						break;
		case 2: cout << "Optimo en coste 1 Objetivo\n";

						cout << "fila: " << objetivos.front().fila << " col:" << objetivos.front().columna << endl;
			      return pathFinding_CostoUniforme(origen,unObjetivo,plan);
						break;
		case 3: cout << "Optimo en coste 3 Objetivos\n";
						// Incluir aqui la llamada al algoritmo de busqueda para 3 objetivos
						cout << "No implementado aun\n";
						break;
		case 4: cout << "Algoritmo de busqueda usado en el reto\n";
		
						cout << "fila: " << objetivos.front().fila << " col: " << objetivos.front().columna << endl;
			      		return pathFinding_Nivel4(origen,actual.destinosPendientes.front(),plan);

						break;
	}
	return false;
}

bool ComportamientoJugador::pathFinding_bateria (int level, const estado &origen, const estado &destino, list<Action> &plan){
		switch (level){
			case 4: cout << "Plan de busqueda de la casilla de recarga\n";
					return pathFinding_Nivel4(origen, destino, plan);
			break;
		}
}
//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el codigo en caracter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M')
		return true;
	else
	  return false;
}


// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  // calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
		// No hay obstaculo, actualizo el parametro st poniendo la casilla de delante.
    st.fila = fil;
		st.columna = col;
		return false;
	}
	else{
	  return true;
	}
}




struct nodo{
	estado st;
	list<Action> secuencia;
	int coste=0;
};

bool operator< (const nodo &a, const nodo &b){
		if(a.coste > b.coste)
				return true;
		else
				return false;
}


struct nodoAestrella{
	nodo n;
	int funcion=0;
	int distancia=0;

	void setFuncion (){
		funcion=distancia + n.coste;
	}

	void distManhattan (nodoAestrella &b){
		int dManhattan= abs(n.st.fila - b.n.st.fila) + abs(n.st.columna -b.n.st.columna);
		distancia=dManhattan;
	}
};

bool operator< (const nodoAestrella &a, const nodoAestrella &b){
		if(a.funcion > b.funcion)
				return true;
		else
				return false;
}


struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion) or
		  (a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini>n.bikini) or 
		  (a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini==n.bikini and a.zapatillas>n.zapatillas))
			return true;
		else
			return false;
	}
};


// Implementación de la busqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> Cerrados; // Lista de Cerrados
	stack<nodo> Abiertos;								 // Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

  while (!Abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (Cerrados.find(hijoForward.st) == Cerrados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty()){
			current = Abiertos.top();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}

//Funcion para calcular lo pedido en el nivel 1
bool ComportamientoJugador::pathFinding_Nivel1(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> Cerrados; // Lista de Cerrados
	queue<nodo> Abiertos;								 // Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

  while (!Abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

	Abiertos.pop();
	if(Cerrados.find(current.st)==Cerrados.end()){
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;

		if (Cerrados.find(hijoTurnR.st) == Cerrados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
			
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
			
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (Cerrados.find(hijoForward.st) == Cerrados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
				
			}
		}
	}
	
		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty()){
			current = Abiertos.front();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}



//***********************************************************************************************************************


//Funcion para calcular Coste uniforme
bool ComportamientoJugador::pathFinding_CostoUniforme(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> Cerrados; // Lista de Cerrados
	priority_queue<nodo, vector<nodo>, less<vector<nodo>::value_type>> Abiertos;								 // Lista de Abiertos

	

  nodo current;
	current.st = origen;
	current.secuencia.empty();
	current.st.bikini=bikini;
	current.st.zapatillas=zapatillas;

	Abiertos.push(current);

  while (!Abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

	Abiertos.pop();
	if(Cerrados.find(current.st)==Cerrados.end()){
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			hijoTurnR.coste += calcular_costeLR(hijoTurnR.st); 
			Abiertos.push(hijoTurnR);
			
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			hijoTurnL.coste += calcular_costeLR(hijoTurnL.st); 
			Abiertos.push(hijoTurnL);
			
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (Cerrados.find(hijoForward.st) == Cerrados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				hijoForward.coste += calcular_costeF(hijoForward.st); 
				Abiertos.push(hijoForward);
				
			}
		}
	}
		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty()){
			current = Abiertos.top();

				//Si el nodo actual es un bikini o las zapatillas
				if (mapaResultado[current.st.fila][current.st.columna]== 'K' && current.st.bikini==false){
							current.st.bikini=true;
							current.st.zapatillas=false;
				}
				if (mapaResultado[current.st.fila][current.st.columna]== 'D' && current.st.zapatillas==false){
							current.st.zapatillas=true;
							current.st.bikini=false;	
				}
		}

	
			
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}



// Sacar por la consola la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}


// Funcion auxiliar para poner a 0 todas las casillas de una matriz
void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}


// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}


 int ComportamientoJugador::calcular_costeF(estado &st){
	 unsigned char casilla=mapaResultado[st.fila][st.columna];
		
	 switch (casilla){
		 	case 'A':
			 		if(st.bikini)
					 		return 10;
					else
							return 200;
			break;

			case 'B':
					if(st.zapatillas)
							return 15;
					else
							return 100;
			break;

			case 'T':
					return 2;
			break;

			case 'X':  //Es necesario tener en cuenta las casillas de recarga para el coste minimo
					return -10;
			break;

			default:
					return 1;
			
	 }
 }

int ComportamientoJugador::calcular_costeLR(estado &st){
	 unsigned char casilla=mapaResultado[st.fila][st.columna];

	 switch (casilla){
		 	case 'A':
			 		if(st.bikini)
					 		return 5;
					else
							return 500;
			break;

			case 'B':
					if(st.zapatillas)
							return 1;
					else
							return 3;
			break;

			case 'T':
					return 2;
			break;

			case 'X':  //Es necesario tener en cuenta las casillas de recarga para el coste minimo
					return -10;
			break;

			default:
					return 1;
			
	 }
 }

 //Funcion para calcular lo pedido en el nivel 2
bool ComportamientoJugador::pathFinding_Nivel4(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> Cerrados; // Lista de Cerrados
	priority_queue<nodoAestrella, vector<nodoAestrella>, less<vector<nodoAestrella>::value_type>> Abiertos;								 // Lista de Abiertos

	int coste_bat=0;

  nodoAestrella current, dest;
	current.n.st = origen;
	current.n.secuencia.empty();
	dest.n.st=destino;

	Abiertos.push(current);

  while (!Abiertos.empty() and (current.n.st.fila!=destino.fila or current.n.st.columna != destino.columna)){

	Abiertos.pop();
	if(Cerrados.find(current.n.st)==Cerrados.end()){
		
		Cerrados.insert(current.n.st);

		// Generar descendiente de girar a la derecha
		nodoAestrella hijoTurnR = current;
		hijoTurnR.n.st.orientacion = (hijoTurnR.n.st.orientacion+1)%4;
		if (Cerrados.find(hijoTurnR.n.st) == Cerrados.end()){
			hijoTurnR.n.secuencia.push_back(actTURN_R);
			hijoTurnR.n.coste += calcular_costeLRestrella(hijoTurnR.n.st); 
			hijoTurnR.distManhattan(dest);
			hijoTurnR.setFuncion();
			Abiertos.push(hijoTurnR);
			
		}

		// Generar descendiente de girar a la izquierda
		nodoAestrella hijoTurnL = current;
		hijoTurnL.n.st.orientacion = (hijoTurnL.n.st.orientacion+3)%4;
		if (Cerrados.find(hijoTurnL.n.st) == Cerrados.end()){
			hijoTurnL.n.secuencia.push_back(actTURN_L);
			hijoTurnL.n.coste += calcular_costeLRestrella(hijoTurnL.n.st); 
			hijoTurnL.distManhattan(dest);
			hijoTurnL.setFuncion();
			Abiertos.push(hijoTurnL);
			
		}

		// Generar descendiente de avanzar
		nodoAestrella hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.n.st)){
			if (Cerrados.find(hijoForward.n.st) == Cerrados.end()){
				hijoForward.n.secuencia.push_back(actFORWARD);
				hijoForward.n.coste += calcular_costeFestrella(hijoForward.n.st); 
				hijoForward.distManhattan(dest);
				hijoForward.setFuncion();
				Abiertos.push(hijoForward);
				
			}
		}
	}
		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty()){
			current = Abiertos.top();

				

		}
	
			
	}

  cout << "Terminada la busqueda\n";
	
	if (current.n.st.fila == destino.fila and current.n.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.n.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
} 


int ComportamientoJugador::calcular_costeFestrella(estado &st){
	 unsigned char casilla=mapaResultado[st.fila][st.columna];
	 int coste=1;

	 cout <<"CONTADOR: " << contador << endl;
	 switch (casilla){
		 	case 'A':
			 		if(bikini)
					 		return 10;
					else
							return 200; 
			break;

			case 'B':
					if(zapatillas)
							return 15;
					else	
							return 100;
			break;

			case 'T':
					return 2;
			break;

			case 'X':  //Es necesario tener en cuenta las casillas de recarga para el coste minimo
					
				if(bateriaBien)
					return 1;
				else
					return -10;
			break;

			case 'K':
					if(!bikini)
							coste= 1;
					if(zapatillas)
							coste=100;
					if( !bikini && mapaResultado[actual.destinosPendientes.front().fila][actual.destinosPendientes.front().columna] == 'A')
							coste=-100;
					if(mapaResultado[actual.destinosPendientes.front().fila][actual.destinosPendientes.front().columna] == 'B')
							coste=100;
					if(contador<800)
							coste=1;
					return coste;		
			break;

			case 'D':
					if(!zapatillas)
							coste= 1;
					if(bikini)
							coste= 100;
					if(!zapatillas && mapaResultado[actual.destinosPendientes.front().fila][actual.destinosPendientes.front().columna] == 'B')
							coste=-100;
					if(mapaResultado[actual.destinosPendientes.front().fila][actual.destinosPendientes.front().columna] == 'A')
							coste=100;
					if(contador<800)
							coste=1;
					return coste;
			break;

			case '?':
				if (contador<1000)
					return 1;
				else
					return 15;
			break;

			default:
					return 1;
			break;
	 }
	 
 }


int ComportamientoJugador::calcular_costeLRestrella(estado &st){
	 unsigned char casilla=mapaResultado[st.fila][st.columna];
	 int coste=1;
	 switch (casilla){
		 	case 'A':
			 		if(bikini)
					 		return 5;
					else
							return 500;
			break;

			case 'B':
					if(zapatillas)
							return 1;
					else	
							return 3;
			break;

			case 'T':
					return 2;
			break;

			case 'X':  //Es necesario tener en cuenta las casillas de recarga para el coste minimo
					
				if(bateriaBien)
					return 1;
				else
					return -10;
			break;

			case 'K':
					if(!bikini)
							coste= 1;
					if(zapatillas)
							coste=100;
					if( !bikini && mapaResultado[actual.destinosPendientes.front().fila][actual.destinosPendientes.front().columna] == 'A')
							coste=-100;
					if(mapaResultado[actual.destinosPendientes.front().fila][actual.destinosPendientes.front().columna] == 'B')
							coste=100;
					if(contador<800)
							coste=1;
					return coste;		
			break;

			case 'D':
					if(!zapatillas)
							coste= 1;
					if(bikini)
							coste= 100;
					if(!zapatillas && mapaResultado[actual.destinosPendientes.front().fila][actual.destinosPendientes.front().columna] == 'B')
							coste=-100;
					if(mapaResultado[actual.destinosPendientes.front().fila][actual.destinosPendientes.front().columna] == 'A')
							coste=100;
					if(contador<800)
							coste=1;
					return coste;
			break;


			case '?':
				if (contador<1000)
					return 1;
				else
					return 15;
			break;

			default:
					return 1;
			break;
	 }
 }

//Funcion que actualiza el mapa conforme al campo de vision del jugador
void ComportamientoJugador::actualizarMapa(Sensores &sensores){
//Posicion actual
	int filaAc=sensores.posF;
	int colAc=sensores.posC;
	int pos=0; //Se usa para recorrer sensores.terreno
	
	switch (sensores.sentido){//Dependiendo de la orientacion rellena el mapa
		case 0:  //Mirando al norte
            for (int i=0; i<=3; i++){
                for (int j=-i; j<=i; j++){
                    mapaResultado[filaAc-i][colAc+j]=sensores.terreno[pos];
                    if (sensores.terreno[pos]=='X'){ //si encontramos la batería guardamos su fila y columna 
                        encuentraBateria=true;
                        bateria.fila=filaAc-i;
                        bateria.columna=colAc+j;
						
                    }
                    pos++;
                }
            }
        break;

        case 1: //Mirando al este
            for (int j=0; j<=3; j++){
                for (int i=-j; i<=j; i++){
                    mapaResultado[filaAc+i][colAc+j]=sensores.terreno[pos];
                    if (sensores.terreno[pos]=='X'){ //idem
                        encuentraBateria=true;
                        bateria.fila=filaAc+i;
                        bateria.columna=colAc+j;
                    }
                    pos++;
                }
            }
        break;

		case 2: //Mirando al sur
            for (int i=0; i<=3; i++){
                for (int j=-i; j<=i; j++){
                    mapaResultado[filaAc+i][colAc-j]=sensores.terreno[pos];
                    if (sensores.terreno[pos]=='X'){ //idem
                        encuentraBateria=true;
                        bateria.fila=filaAc+i;
                        bateria.columna=colAc-j;
                    }
                    pos++;
                }
            }
        break;

        case 3: //Mirando al oeste
            for (int i=0; i<=3; i++){
                for (int j=-i; j<=i; j++){
                    mapaResultado[filaAc-j][colAc-i]=sensores.terreno[pos];
                    if (sensores.terreno[pos]=='X'){ //idem 
                        encuentraBateria=true;
                        bateria.fila=filaAc-j;
                        bateria.columna=colAc-i;

                    }
                    pos++;
                }
            }
        break;
	}

}