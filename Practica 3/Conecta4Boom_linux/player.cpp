#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();
      
    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar





int consecutivasVertical (const Environment &estado, int fil, int col, int player, int n_casillas){
   int consecutivas = 0;
//cout << "VERTICAL<-----------------------"<< endl;
    for (int i= fil ; i<7; i++) {
        if (estado.See_Casilla(i,col) == player)
            consecutivas += 1;
        else
            break;
    }

    if (consecutivas >= n_casillas)
        return 1;
    else
        return 0;
}

int consecutivasHorizontal (const Environment &estado, int fil, int col, int player, int n_casillas){
     int consecutivas = 0;
//cout << "HORIZONTAL<-----------------------"<< endl;
    for (int i= col ; i<7; i++) {
        if (estado.See_Casilla(i,col) == player)
            consecutivas += 1;
        else
            break;
    }

    if (consecutivas >= n_casillas)
        return 1;
    else
        return 0;
}

int consecutivasEnDiagonal (const Environment &estado, int fil, int col, int player, int n_casillas){
   int total = 0;

    // Comprobamos los diagonales ascendentes
    double consecutivas = 0;

    int j = col;

    for (int i=fil; i<7;i++) {
        if (j > 6) // Control para no pasarnos del maximo de columnas
            break;
        else if (estado.See_Casilla(i,j) == player)
            consecutivas += 1;
        else
            break;
        j+=1; // Incrementamos la columna cuando se incrementa la fila

    }

    if (consecutivas >= n_casillas)
        total += 1;

    // Comprobamos los diagonales descendentes
    consecutivas = 0;
    j = col;
    for (int i=fil; i>=0; i--) {
        if (j > 6) // Control para no pasarnos del maximo de columnas
            break;
        else if (estado.See_Casilla(i,j) == player)
            consecutivas += 1;
        else
            break;
        j+=1; // Incrementamos la columna cuando se decrementa la fila

    }

    if (consecutivas >= n_casillas)
        total += 1;

    return total;

}

int checkConsecutivas(const Environment &estado, int player, int n_casillas){
   int contador = 0;

    
    for (int i=0; i<7;i++) { //Comprobamos todas las fichas del tablero
        for (int j=0; j<7;j++) { 

            if (estado.See_Casilla(i,j) == player ){// Si es el jugador

                // Comprueba las casillas consecutivas 
                contador += consecutivasVertical(estado, i, j, player, n_casillas);

                // Comprueba los horizontales de la fila
                contador += consecutivasHorizontal(estado, i, j, player, n_casillas);

                // Comprueba los diagonales (los dos tipos)
                contador += consecutivasEnDiagonal(estado, i, j, player, n_casillas);
            }
        }
    }

    // Devolvemos la suma de los adyacentes encontrados para el "nivel"
    return contador;

}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){
    double total_jugador, total_opuesto;
    // Obtenemos el judador opuesto
    int jugador_opuesto = 1;
    if (jugador == 1) 
        jugador_opuesto = 2;

    // Comprobamos los adyacentes del jugador actual
    int cuatros = checkConsecutivas(estado, jugador, 4);
    int treses =  checkConsecutivas(estado, jugador, 3);
    int doses =   checkConsecutivas(estado, jugador, 2);

    // Comprobamos los adyacentes del jugador opuesto
    int cuatros_opuesto = checkConsecutivas(estado, jugador_opuesto, 4);
    int treses_opuesto =  checkConsecutivas(estado, jugador_opuesto, 3);
    int doses_opuesto =   checkConsecutivas(estado, jugador_opuesto, 2);

    // Calculamos el valor heurístico del tablero
    // cout << "----->Cuatros: "<<cuatros<<endl;
    // cout << "----->Treses: "<<treses<<endl;
    // cout << "----->Doses: "<<doses<<endl;
    // cout << "Cuatros_opesto: "<<cuatros_opuesto<<endl;
    // cout << "Treses_opesto: "<<treses_opuesto<<endl;
    // cout << "Doses_opesto: "<<doses_opuesto<<endl;
    total_jugador=(cuatros*1000000 + treses*100 + doses*10);
    total_opuesto=(cuatros_opuesto*10000000 + treses_opuesto*100 + doses_opuesto*10);
    cout << "Total jugador : "<< total_jugador<< endl;
    cout << "Total opuesto : "<< total_jugador<< endl;
    return total_jugador-total_opuesto;

}

// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}






// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acci�n que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha=menosinf, beta=masinf; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    //--------------------- COMENTAR Desde aqui
   //  cout << "\n\t";
   //  int n_opciones=0;
   //  JuegoAleatorio(aplicables, opciones, n_opciones);

   //  if (n_act==0){
   //    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
   //    cout << " No puede realizar ninguna accion!!!\n";
   //    //accion = Environment::actIDLE;
   //  }
   //  else if (n_act==1){
   //         (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
   //          cout << " Solo se puede realizar la accion "
   //               << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
   //          accion = static_cast< Environment::ActionType > (opciones[0]);

   //       }
   //       else { // Hay que elegir entre varias posibles acciones
   //          int aleatorio = rand()%n_opciones;
   //          cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
   //          accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
   //       }

    //--------------------- COMENTAR Hasta aqui


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}
/**********************************************************/
//PODA ALFA-BETA
double Player::Poda_AlfaBeta(const Environment & tablero, int jugador_, int profundidad, const int profundidad_max, Environment::ActionType & action , double alpha, double beta){
      
      Environment::ActionType act_anterior;
      double aux;

      bool opciones[8];
      int n_act = tablero.possible_actions(opciones); //De esta manera se determina el numero de acciones que se pueden hacer

      if (profundidad==profundidad_max || n_act==0){ //Si hemos llegado al nodo final o si ya no hay mas acciones

         return Valoracion(tablero, jugador_);

      }else { 

            int last_act=-1;
            Environment hijo = tablero.GenerateNextMove(last_act);

            if (profundidad%2==0){// Con esto indicamos que el nodo es MAX

                  while (!(hijo==tablero)){ //Recorremos los hijos, cuando se iguale significará que ha terminado
                   
                     aux = Poda_AlfaBeta(hijo, jugador_, profundidad+1, profundidad_max, action, alpha, beta);

                     if (aux>alpha){
                        alpha=aux;
                        action = static_cast <Environment::ActionType> (last_act); // El movimiento a arrastrar
                     }

                     if (beta <= alpha){
                        break; //Poda beta
                     }

                     hijo = tablero.GenerateNextMove(last_act); //Generamos el siguiente tablero hijo
                  }

               return alpha;

            } else { //Nodo MIN
                 while (!(hijo==tablero)){ //Idem
                 
                     aux = Poda_AlfaBeta(hijo, jugador_, profundidad+1, profundidad_max, action, alpha, beta);

                     if (aux<beta){
                        beta=aux;
                        action = static_cast <Environment::ActionType> (last_act);
                     }

                     if (beta <= alpha){
                        break; //Poda alpha
                     }

                  hijo = tablero.GenerateNextMove(last_act); //Generamos el siguiente tablero hijo

                 }
               return beta;
            }
      }
}


