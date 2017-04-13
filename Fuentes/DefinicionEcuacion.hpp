// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Direcci�n: Amsterdam 312 col. Hip�dromo Condesa
// Tel�fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr�mite 1999-2000
// Revisi�n  1.1-A


#ifndef __DefinicionEcuacion_HPP__
#define __DefinicionEcuacion_HPP__




extern "C" {
   #include <math.h>
}

#define NUM_MAX_PARAMETROS 15


/////////////////////////////////////////////////////////////////////////////////
//         Clase que contiene la definicion de la ecuaci�n del sistema         //
/////////////////////////////////////////////////////////////////////////////////

class DefinicionEcuacion {

   public:
                           // Nombre del sistema
      char                *Nombre_sistema;                  
                           // N�mero de par�metros
      char                 Numero_parametros;               
                           // N�mero de par�metros a bifurcar
      char                 Numero_parametros_bifurcar;               
                           // Nombre de par�metros
      char                *Nombre_parametros[NUM_MAX_PARAMETROS];            
                           // Ecuaci�n en formato texto
      char                *Ecuacion_texto;
                           // Par�metros del sistema
      long double          P[NUM_MAX_PARAMETROS];


      
                           // Constructor de la clase
                           DefinicionEcuacion(void);
                           // Asigna valor a los par�metros
      void                 AsignaParametros(const long double parm[NUM_MAX_PARAMETROS]);
                           // Retorna el valor de los par�metros
      void                 RetornaParametros(long double *parm);
      
                           // Ecuacion del sistema 
      long double          Ecuacion(const long double x, const long double t)
                           { 
#ifdef DSIERRA
                              return ((P[2] * x) + P[0] + (P[1] * powl(sinl(2.0 * M_PI * x),P[3])));
#else                              
                              return (-(P[0] * x) + P[1] + (P[2] * cosl(2.0 * M_PI * P[3]* t)) + (P[4] * sinl(2.0 * M_PI * P[5]* t)) ); 
#endif                              
                           } 
                           // Curva de extremos
      long double          G(const long double t)
                           {
#ifdef DSIERRA
                              return (0.0);
#else                              
                              return((1.0 / P[0]) * (P[1] + (P[2] * cosl(2.0 * M_PI * P[3]* t)) + (P[4] * sinl(2.0 * M_PI * P[5]* t))) );
#endif                              
                           }
                           
};

#endif







