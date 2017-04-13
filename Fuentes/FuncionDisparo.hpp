// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Direcci�n: Amsterdam 312 col. Hip�dromo Condesa
// Tel�fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr�mite
// Revisi�n  1.0-A


#ifndef __FUNCIONDISPARO_HPP__
#define __FUNCIONDISPARO_HPP__


#include "DefinicionEcuacion.hpp"

// Indica el m�todo num�rico de integraci�n activo
#define RUNGE-KUTTA


////////////////////////////////////////////////////////////////////////////////////////////////////
//      Clase que contiene los metodos numericos de integracion de ecuaciones diferenciales       //
////////////////////////////////////////////////////////////////////////////////////////////////////


class FuncionDisparo: public DefinicionEcuacion
{
   private:
                             
      long double            K1,K2,K3,K4,Kt,K8;  // Variables temporales para el metodo n�merico
      long double            Y, Yn, T1, Cota;    // Variables temporales para la funci�n de disparo

   public:

      ////////////////////////////////////////////////////////////////////////
      // M�todo n�merico
      ////////////////////////////////////////////////////////////////////////

      long double           *Dis_Tiempo_maximo;
      long double           *Dis_Paso_integracion;
   
                             // Rutina que integra numericamente
      inline long double     Integra(const long double x, const long double t, const long double paso)
                             {
#ifdef RUNGE-KUTTA                             
                                // M�todo Runge-Kutta cuarto orden paso fijo
                                K8 = paso * 0.5;
                                K1 = paso*Ecuacion(x,t);
                                K2 = paso*Ecuacion(x+0.5*K1,t+K8);
                                K3 = paso*Ecuacion(x+0.5*K2,t+K8);
                                K4 = paso*Ecuacion(x+K3,t+paso);
                                Kt = x + (K1+ (K2 * 2.0) + (K3 * 2.0) + K4) / 6.0;
                                return Kt;
#endif                                
#ifdef EULER  
                                // M�todo Euler paso fijo
                                return(x + (paso * Ecuacion(x,t)));
#endif                                
                             }
        
      ////////////////////////////////////////////////////////////////////////
      // Funci�n de disparo
      ////////////////////////////////////////////////////////////////////////
        
                             // Indica el error en calculo (si este existe)
      unsigned int           Error_calculo;

                             // Constructor de la clase
                             FuncionDisparo(void);

                             // C�lculo de la funci�n de disparo
      long double            Calcula(const long double T, const bool FI);
};

#endif

