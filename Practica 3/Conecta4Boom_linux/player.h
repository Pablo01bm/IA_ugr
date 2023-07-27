#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
      double Poda_AlfaBeta(const Environment & tablero, int jugador_, int profundidad,const int profundidad_max, Environment::ActionType & action , double alpha, double beta);
    private:
      int jugador_;
      Environment actual_;

      // const double INF_MAX = 9999999999.0;
      // const double INF_MIN = -9999999999.0;
};
#endif
