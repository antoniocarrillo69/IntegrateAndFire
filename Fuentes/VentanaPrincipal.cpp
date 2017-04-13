// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Direcci�n: Amsterdam 312 col. Hip�dromo Condesa
// Tel�fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr�mite 1999-2000
// Revisi�n  1.1-A


//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include "Acecade.h"
#include "Ayuda.h"
#include "Sincronizaciones.hpp"
#include "NumeroRotacion.hpp"
#include "ExponenteLyapunov.hpp"
#pragma hdrstop

#include "VentanaPrincipal.h"
#include "VentanaBifurcaciones.h"
#include "VentanaCirculo.h"
#include "VentanaToro.h"
#include "VentanaCurvasIntegrales.h"
#include "VentanaLevantamientos.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

TFormaPrincipal *FormaPrincipal;

// Objetos globales del sistema IyD
Sincronizaciones       sincro;
NumeroRotacion         num_rot;
ExponenteLyapunov      exp_lyap;

// Variables globales de configuraci�n del m�todo n�merico
long double            Tiempo_maximo_integracion   = 10.0;
long double            Paso_integracion            = 0.001;

// Variables globales de control de escenarios
bool                   Limpiar_ventana             = false;
bool                   Calcular_sincronizaciones   = false;
bool                   Calcular_numero_rotacion    = false;
bool                   Calcular_exponente_lyapunov = false;  
bool                   Configura_escenario         = false;
bool                   Calcula_escenario           = false;
bool                   Detener_Calculo             = false;
void                  *Ventana_activa              = NULL; 


// Constructor de la forma
__fastcall TFormaPrincipal::TFormaPrincipal(TComponent* Owner) : TForm(Owner)
{
   // Titulo de la aplicaci�n
   sprintf(TituloAplicacion,"Integrate and Fire Systems: %s",def_ecu.Nombre_sistema);
   Application->Title = "Integrate and Fire Systems 1.0";
   Caption = TituloAplicacion;
   try {
   Application->Icon->LoadFromFile("I&FS.ICO");
   } catch (...) {};
   // Tiempo maximo de muestra de Hits
   Application->HintHidePause = 10000;
}

// Al crear la forma ...
void __fastcall TFormaPrincipal::FormCreate(TObject *Sender)
{
   // Asigna la rutina de visualizacion de la barra de estado
   Application->OnHint = &OnHint;
}

// Controla la visualizacion de Hint en la barra de estado
void __fastcall TFormaPrincipal::OnHint(TObject *Sender)
{
   BarraDeEstadoPrincipal->SimpleText = Application->Hint;
}

// Controla la solicitud de cerrar la forma
void __fastcall TFormaPrincipal::FormCloseQuery(TObject *Sender,bool &CanClose)
{
   if (MessageBox(Handle,"Do you wish to end the program?",TituloAplicacion,MB_YESNO + MB_ICONQUESTION)  == IDYES) CanClose = true;
    else CanClose = false;
}


///////////////////////////////////////////////////////////////////////////////
//  Definici�n de Comportamientos del Menu 
///////////////////////////////////////////////////////////////////////////////

// MenuPrincipal->Archivo->Configura impresi�n
void __fastcall TFormaPrincipal::Printsetup1Click(TObject *Sender)
{
   PrinterSetupDialog->Execute();        
}


// MenuPrincipal->Archivo->Salir
void __fastcall TFormaPrincipal::MenuArchivoTerminarProgramaClick(TObject *Sender)
{
   Close();     
}

//MenuPrincipal->Configurar->Metodo Numerico
void __fastcall TFormaPrincipal::MenuConfigurarMetodoNumericoClick(TObject *Sender)
{
   configura = new TVCap_General(this);
   if(configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 0;
      configura->ShowModal();
      RetornarValoresConfiguracion();
   }
   delete configura;
}

//MenuPrincipal->Configurar->Numero de Rotaci�n
void __fastcall TFormaPrincipal::MenuConfigurarNumRotacionClick(TObject *Sender)
{
   configura = new TVCap_General(this);
   if(configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 1;
      configura->ShowModal();
      RetornarValoresConfiguracion();
   }
   delete configura;
}

//MenuPrincipal->Configurar->Sincronizaci�n
void __fastcall TFormaPrincipal::MenuConfigurarSincronizacionClick(TObject *Sender)
{
   configura = new TVCap_General(this);
   if(configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 2;
      configura->ShowModal();
      RetornarValoresConfiguracion();
   }
   delete configura;
}

//MenuPrincipal->Configurar->Exponente de Lyapunov
void __fastcall TFormaPrincipal::MenuConfigurarExpLyapunovClick(TObject *Sender)
{
   configura = new TVCap_General(this);
   if(configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 3;
      configura->ShowModal();
      RetornarValoresConfiguracion();
   }
   delete configura;
}

// Acerca de ...
void __fastcall TFormaPrincipal::MenuAyudaAcercadeClick(TObject *Sender)
{
   TVAcercaDe *Acercade = new TVAcercaDe(this);
   if (Acercade) {
      Acercade->ShowModal();
      delete Acercade;
   }
}

// Ayuda General
void __fastcall TFormaPrincipal::MenuAyudaGeneralClick(TObject *Sender)
{
   TAyudaForm *Ayuda = new TAyudaForm(this);
   if (Ayuda) {
      Ayuda->Abrir_archivo("General help","General.hlp");
      Ayuda->ShowModal();
      delete Ayuda;
   }
}

// Ayuda de Bifurcaciones
void __fastcall TFormaPrincipal::MenuAyudaBifurcacionesClick(TObject *Sender)
{
   TAyudaForm *Ayuda = new TAyudaForm(this);
   if (Ayuda) {
      Ayuda->Abrir_archivo("Bifurcations scenery","Bifurcations.hlp");
      Ayuda->ShowModal();
      delete Ayuda;
   }
}

// Ayuda de Circulo
void __fastcall TFormaPrincipal::MenuAyudaCirculoClick(TObject *Sender)
{
   TAyudaForm *Ayuda = new TAyudaForm(this);
   if (Ayuda) {
      Ayuda->Abrir_archivo("Circle scenery","Circle.hlp");
      Ayuda->ShowModal();
      delete Ayuda;
   }
}

// Ayuda de Toro
void __fastcall TFormaPrincipal::MenuAyudaToroClick(TObject *Sender)
{
   TAyudaForm *Ayuda = new TAyudaForm(this);
   if (Ayuda) {
      Ayuda->Abrir_archivo("Torus scenery","Torous.hlp");
      Ayuda->ShowModal();
      delete Ayuda;
   }
}

// Ayuda de Curvas Integrales
void __fastcall TFormaPrincipal::MenuAyudaCurvasIntegralesClick(TObject *Sender)
{
   TAyudaForm *Ayuda = new TAyudaForm(this);
   if (Ayuda) {
      Ayuda->Abrir_archivo("Integral curves scenery","IntegralCurves.hlp");
      Ayuda->ShowModal();
      delete Ayuda;
   }
}

// Ayuda de Levantamientos
void __fastcall TFormaPrincipal::MenuAyudaLevantamientosClick(TObject *Sender)
{
   TAyudaForm *Ayuda = new TAyudaForm(this);
   if (Ayuda) {
      Ayuda->Abrir_archivo("Lifts scenery","Lifts.hlp");
      Ayuda->ShowModal();
      delete Ayuda;
   }
}

///////////////////////////////////////////////////////////////////////////////
//  Definici�n de Comportamientos de los Iconos de Escenarios
///////////////////////////////////////////////////////////////////////////////

// Icono de Escenario de Bifurcaciones
void __fastcall TFormaPrincipal::IconoEscenarioBifurcacionesClick(
      TObject *Sender)
{
   TFormaBifurcaciones *vent_bif = new TFormaBifurcaciones(this);
   if(vent_bif) {
      vent_bif->Show();
   }
}

// Icono de Escenario de Circunferencia
void __fastcall TFormaPrincipal::IconoEscenarioCirculoClick(TObject *Sender)
{
   TFormaCirculo *vent_cir = new TFormaCirculo(this);
   if(vent_cir) {
      vent_cir->Show();
   }
}

// Icono de Escenario de Toro
void __fastcall TFormaPrincipal::IconoEscenarioToroClick(TObject *Sender)
{
   TFormaToro *vent_tor = new TFormaToro(this);
   if(vent_tor) {
      vent_tor->Show();
   }
}

// Icono de Escenario de Curvas Integrales
void __fastcall TFormaPrincipal::IconoEscenarioCurvasIntegralesClick(TObject *Sender)
{
   TFormaCurvasIntegrales *vent_cur = new TFormaCurvasIntegrales(this);
   if(vent_cur) {
      vent_cur->Show();
   }
}

// Icono de Escenario de Levantamientos
void __fastcall TFormaPrincipal::IconoEscenarioLevantamientosClick(TObject *Sender)
{
   TFormaLevantamientos *vent_lev = new TFormaLevantamientos(this);
   if(vent_lev) {
      vent_lev->Show();
   }
}

// Icono de Escenario de Lenguas de Arnold
void __fastcall TFormaPrincipal::IconoEscenarioTonguesClick(TObject *Sender)
{
   ShowMessage("En proceso de construccci�n ...");     
   MenuAyudaAcercadeClick(this);
}


///////////////////////////////////////////////////////////////////////////////
//  Definici�n de Comportamientos de los Iconos Acci�n 
///////////////////////////////////////////////////////////////////////////////

// Icono de calcula el n�mero de rotacion del escenario activo
void __fastcall TFormaPrincipal::IconoAccionNumRotacionClick(TObject *Sender)
{
   if (Ventana_activa) Calcular_numero_rotacion = true;
}

// Icono de calcula la sincronizaci�n del escenario activo
void __fastcall TFormaPrincipal::IconoAccionSincronizacionClick(TObject *Sender)
{
   if (Ventana_activa) Calcular_sincronizaciones = true;
}

// Icono de calcula el exponente de Luapunov del escenario activo
void __fastcall TFormaPrincipal::IconoAccionExpLyapunovClick(TObject *Sender)
{
   if (Ventana_activa) Calcular_exponente_lyapunov = true;
}

// Icono de configura el escenario  del escenario activo
void __fastcall TFormaPrincipal::IconoAccionConfigurarEscenarioClick(TObject *Sender)
{
   if (Ventana_activa) Configura_escenario = true;
}

// Icono de calcula del escenario activo
void __fastcall TFormaPrincipal::IconoAccionCalcularClick(TObject *Sender)
{
   if (Ventana_activa) Calcula_escenario = true;
}

// Icono de detener c�lculo del escenario activo
void __fastcall TFormaPrincipal::IconoAccionDetenerCalculoClick(TObject *Sender)
{
   if (Ventana_activa) Detener_Calculo = true;
}

// Icono de limpiar ventana del escenario activo
void __fastcall TFormaPrincipal::IconoAccionLimpiarVentanaClick(TObject *Sender)
{
   if (Ventana_activa) Limpiar_ventana = true;
}


///////////////////////////////////////////////////////////////////////////////
// Control de la ventana de captura de par�metros globales
///////////////////////////////////////////////////////////////////////////////

// Pasa los valores de configuraci�n de la ventana de captura
void TFormaPrincipal::PasarValoresConfiguracion(void)
{
   ////////////////////////////////////////////////////////
   // M�todo de integraci�n
   ////////////////////////////////////////////////////////
   sprintf(xcad,"%Lf",Paso_integracion);
   configura->EditN1->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",Tiempo_maximo_integracion);
   configura->EditN2->Text = (AnsiString) xcad;
   ////////////////////////////////////////////////////////
   // Numero de rotaci�n
   ////////////////////////////////////////////////////////
   sprintf(xcad,"%u",num_rot.Rot_Num_iteraciones);
   configura->EditR1->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",num_rot.Rot_Condicion_Inicial);
   configura->EditR2->Text = (AnsiString) xcad;
   configura->CheckBoxR1->Checked = num_rot.Rot_sw_Cond_inicial_aleatoria;      
   ////////////////////////////////////////////////////////
   // Sincronizaciones
   ////////////////////////////////////////////////////////
   sprintf(xcad,"%u",sincro.Res_Long_transitorios);
   configura->EditS1->Text = (AnsiString) xcad;
   sprintf(xcad,"%u",sincro.Res_Max_num_ciclos);
   configura->EditS2->Text = (AnsiString) xcad;
   sprintf(xcad,"%u",sincro.Res_Long_max_ciclo);
   configura->EditS3->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",sincro.Res_Tolerancia);
   configura->EditS4->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",sincro.Res_Condicion_Inicial);
   configura->EditS5->Text = (AnsiString) xcad;
   configura->CheckBoxS1->Checked = sincro.Res_sw_Cond_inicial_aleatoria;      
   ////////////////////////////////////////////////////////
   // Exponente de Lyapunov
   ////////////////////////////////////////////////////////
   sprintf(xcad,"%u",exp_lyap.Min_num_iteraciones);
   configura->EditL1->Text = (AnsiString) xcad;
   sprintf(xcad,"%u",exp_lyap.Max_num_iteraciones);
   configura->EditL2->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",exp_lyap.Epsilon);
   configura->EditL3->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",exp_lyap.Paso_derivada);
   configura->EditL4->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",exp_lyap.Condicion_inicial);
   configura->EditL5->Text = (AnsiString) xcad;
   configura->CheckBoxL1->Checked = exp_lyap.Sw_cond_inicial_aleatoria;
}

// Retorna los valores de configuraci�n de la ventana de captura
void TFormaPrincipal::RetornarValoresConfiguracion(void)
{
   if (!configura->Aceptar) return;
   ////////////////////////////////////////////////////////
   // M�todo de integraci�n
   ////////////////////////////////////////////////////////
   Paso_integracion = _atold(configura->EditN1->Text.c_str());       
   Tiempo_maximo_integracion = _atold(configura->EditN2->Text.c_str());       
   ////////////////////////////////////////////////////////
   // Numero de rotaci�n
   ////////////////////////////////////////////////////////
   num_rot.Rot_Num_iteraciones   = atoi(configura->EditR1->Text.c_str());       
   num_rot.Rot_Condicion_Inicial = _atold(configura->EditR1->Text.c_str());       
   num_rot.Rot_sw_Cond_inicial_aleatoria = configura->CheckBoxR1->Checked;   
   ////////////////////////////////////////////////////////
   // Sincronizaciones
   ////////////////////////////////////////////////////////
   sincro.Res_Long_transitorios  = atoi(configura->EditS1->Text.c_str());       
   sincro.Res_Max_num_ciclos     = atoi(configura->EditS2->Text.c_str());       
   sincro.Res_Long_max_ciclo     = atoi(configura->EditS3->Text.c_str());       
   sincro.Res_Tolerancia         = _atold(configura->EditS4->Text.c_str());       
   sincro.Res_Condicion_Inicial  = _atold(configura->EditS5->Text.c_str());       
   sincro.Res_sw_Cond_inicial_aleatoria = configura->CheckBoxS1->Checked;   
   ////////////////////////////////////////////////////////
   // Exponente de Lyapunov
   ////////////////////////////////////////////////////////
   exp_lyap.Min_num_iteraciones  = atoi(configura->EditL1->Text.c_str());       
   exp_lyap.Max_num_iteraciones  = atoi(configura->EditL2->Text.c_str());       
   exp_lyap.Epsilon              = _atold(configura->EditL3->Text.c_str());       
   exp_lyap.Paso_derivada        = _atold(configura->EditL4->Text.c_str());       
   exp_lyap.Condicion_inicial    = _atold(configura->EditL5->Text.c_str());       
   exp_lyap.Sw_cond_inicial_aleatoria = configura->CheckBoxL1->Checked;   
}




// #error Tener mecanismo para ajustar la cantidad de pixeles horizontales y verticales por escenario
// #error Activar en todos los m�dulos el men� Edit para transferencia de par�metros.
// #error Falta alguna caja donde configurar el n�mero de puntos de la malla horizontal al trazar los diagramas de bifurcaciones. En este escenario yo considero que es indispensable, pues hay gr�ficas que necesitan cualquier  cantidad de puntos para no perder el detalle (por ejemplo, imag�nate trazar funciones que crecen demasiado r�pido).
// #error En el men� File/Print revisar que se pueda hacer correctamente la impresion
// #error Tambi�n hay que decidir qu� hacer en todos escenario cuando le das par�metros que no generan sucesiones de disparos.
// #error En bifurcaciones hay que idear una forma inteligente en que aunque el usuario introduzca alg�n rango de valores donde supuestamente la gr�fica del exponente de Lyapunov sea trazada, �sta nunca se salga, pero sea trazada en su totalidad, pues ya sabemos que es muy tardado hacer los diagramas de bifurcaciones. Una forma ser�a que el programa vaya detectando el supremo y el �nfimo de esta gr�fica a cada iteraci�n, y que si en alg�n momento estos superan las dimensiones reales de la ventana donde est� confinada, que los dominios superior y/o inferior reales de la ventana se reasignen y que la gr�fica se vuelva a dibujar dentro de los nuevos l�mites. Claro que esto implicar�a guardar los valores de cada punto de la gr�fica dentro de alguna estructura como un array de puntos o algo as�...


