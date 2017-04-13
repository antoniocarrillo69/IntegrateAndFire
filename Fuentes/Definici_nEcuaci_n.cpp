// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Direcci�n: Amsterdam 312 col. Hip�dromo Condesa
// Tel�fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr�mite 1999-2000
// Revisi�n  1.1-A


#include "DefinicionEcuacion.hpp"

// Constructor de la clase
DefinicionEcuacion::DefinicionEcuacion(void)
{
   // Nombre del sistema
   Nombre_sistema       = "Generalization of KHR model";
   // N�mero de par�metros
   Numero_parametros          = 6;
   Numero_parametros_bifurcar = 6;
   // Nombre de los par�metros (si el nombre de el par�metro excede de 6 letras
   // hay que indicar el tama�o maximo de este en captura de par�metros en cada escenario)
   Nombre_parametros[0] = "Sigma";
   Nombre_parametros[1] = "S";
   Nombre_parametros[2] = "H1";
   Nombre_parametros[3] = "K1";
   Nombre_parametros[4] = "H2";
   Nombre_parametros[5] = "K2";
   // Valores por omisi�n de los parametros
   P[0] = 0.375;
   P[1] = 1.0;
   P[2] = 0.0;
   P[3] = 2.0;
   P[4] = 0.5;
   P[5] = 1.0;
   // Ecuaci�n en formato texto
   Ecuacion_texto = "-Sigma * x + S + H1 * cos(2 * Pi * K1 * t) + H2 * sin(2 * Pi * K2 * t)";
}



// Asigna valor a los par�metros
void DefinicionEcuacion::AsignaParametros(const long double parm[NUM_MAX_PARAMETROS])
{
   for (int i = 0; i < Numero_parametros; i++) P[i] = parm[i];
}

// Retorna el valor de los par�metros
void DefinicionEcuacion::RetornaParametros(long double *parm)
{
   for (int i = 0; i < Numero_parametros; i++) parm[i] = P[i];
}


