// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#ifndef __FUNCIONDISPARO_HPP__
#define __FUNCIONDISPARO_HPP__


#include "DefinicionEcuacion.hpp"

// Indica el método numérico de integración activo
#define RUNGE-KUTTA


////////////////////////////////////////////////////////////////////////////////////////////////////
//      Clase que contiene los metodos numericos de integracion de ecuaciones diferenciales       //
////////////////////////////////////////////////////////////////////////////////////////////////////


class FuncionDisparo: public DefinicionEcuacion
{
   private:
                             
      long double            K1,K2,K3,K4,Kt,K8;  // Variables temporales para el metodo númerico
      long double            Y, Yn, T1, Cota;    // Variables temporales para la función de disparo

   public:

      ////////////////////////////////////////////////////////////////////////
      // Método númerico
      ////////////////////////////////////////////////////////////////////////

      long double           *Dis_Tiempo_maximo;
      long double           *Dis_Paso_integracion;
   
                             // Rutina que integra numericamente
      inline long double     Integra(const long double x, const long double t, const long double paso)
                             {
#ifdef RUNGE-KUTTA                             
                                // Método Runge-Kutta cuarto orden paso fijo
                                K8 = paso * 0.5;
                                K1 = paso*Ecuacion(x,t);
                                K2 = paso*Ecuacion(x+0.5*K1,t+K8);
                                K3 = paso*Ecuacion(x+0.5*K2,t+K8);
                                K4 = paso*Ecuacion(x+K3,t+paso);
                                Kt = x + (K1+ (K2 * 2.0) + (K3 * 2.0) + K4) / 6.0;
                                return Kt;
#endif                                
#ifdef EULER  
                                // Método Euler paso fijo
                                return(x + (paso * Ecuacion(x,t)));
#endif                                
                             }
        
      ////////////////////////////////////////////////////////////////////////
      // Función de disparo
      ////////////////////////////////////////////////////////////////////////
        
                             // Indica el error en calculo (si este existe)
      unsigned int           Error_calculo;

                             // Constructor de la clase
                             FuncionDisparo(void);

                             // Cálculo de la función de disparo
      long double            Calcula(const long double T, const bool FI);
};

#endif

