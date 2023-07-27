#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado {
  int fila ;
  int columna ;
  int orientacion;
  bool zapatillas;
  bool bikini;
  list<estado> destinosPendientes;
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      fil = col = 99;
      brujula = 0; //0: N, 1: Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      hayPlan=false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      fil = col = 99;
      brujula = 0; //0: N, 1: Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      hayPlan=false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

    int calcular_costeF(estado &st);
    int calcular_costeLR(estado &st);
        int calcular_costeFestrella(estado &st);
    int calcular_costeLRestrella(estado &st);
    
  private:
    // Declarar Variables de Estado
    int fil, col, brujula;
    estado actual, destino, bateria;
    int bateriaMin=1000, bateriaMax=2990;
    list<estado> objetivos;
    
    list<Action> plan;
    bool hayPlan, zapatillas=false, bikini=false, bateriaBien=true, encuentraBateria=false, objetivosAlcanzados=false, haHabidoRecarga=false;

    int contador=0;
    int pasos=3; 
    int maxPasos=3;
    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan);
    bool pathFinding_bateria(int level, const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Nivel1(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_CostoUniforme(const estado &origen, const estado &destino, list<Action> &plan);
    
    bool pathFinding_Nivel4(const estado &origen, const estado &destino, list<Action> &plan);
    void actualizarMapa(Sensores &sensores);

    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);
    
    

};

#endif
