// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Direcci�n: Amsterdam 312 col. Hip�dromo Condesa
// Tel�fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr�mite
// Revisi�n  1.0-A

#ifndef __EXPONENTELYAPUNOV_HPP__
#define __EXPONENTELYAPUNOV_HPP__


#include "FuncionDisparo.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
//      Clase que contiene los metodos numericos de integracion de ecuaciones diferenciales       //
////////////////////////////////////////////////////////////////////////////////////////////////////


class ExponenteLyapunov: public FuncionDisparo
{

   public:
   
    long double              Condicion_inicial;
    bool                     Sw_cond_inicial_aleatoria;
    int                      Max_num_iteraciones;
    int                      Min_num_iteraciones;
    long double              Epsilon;
    long double              Paso_derivada;
        
      

                             // Constructor de la clase
                             ExponenteLyapunov(void)
                             {
                                Condicion_inicial = 0.0;
                                Sw_cond_inicial_aleatoria = true;
                                Max_num_iteraciones = 1000;
                                Min_num_iteraciones = 0;
                                Epsilon = 0.0001;
                                Paso_derivada = 0.01;
                             }

                             // Retorna el exponente de Lyapunov
      long double            Calcula(const bool FI);
};

#endif

