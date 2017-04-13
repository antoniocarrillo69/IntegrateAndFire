// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en trámite 1999-2000
// Revisión  1.1-A


//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <Printers.hpp>
#include "Acecade.h"
#include "Ayuda.h"
#include "VentanaCurvasIntegrales.h"
#include "Sincronizaciones.hpp"
#include "NumeroRotacion.hpp"
#include "ExponenteLyapunov.hpp"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
// Objetosglobales del sistema IyD
extern  Sincronizaciones       sincro;
extern  NumeroRotacion         num_rot;
extern  ExponenteLyapunov      exp_lyap;

// Variables globales de control de escenarios
extern  bool                   Limpiar_ventana;
extern  void                  *Ventana_activa;
extern  bool                   Calcular_sincronizaciones;
extern  bool                   Calcular_numero_rotacion;
extern  bool                   Calcular_exponente_lyapunov;  
extern  bool                   Configura_escenario;
extern  bool                   Calcula_escenario;
extern  bool                   Detener_Calculo;

// Al cerrar la forma ...
void __fastcall TFormaCurvasIntegrales::FormClose(TObject *Sender, TCloseAction &Action)
{
   Ventana_activa = NULL;
   Free();        
}

// Al solicitar cerrar la ventana ...
void __fastcall TFormaCurvasIntegrales::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   if (MessageBox(Handle,"Do you wish to close this window?","Integral curves scenery",MB_YESNO + MB_ICONQUESTION)  == IDYES) CanClose = true;
    else CanClose = false;
}


// Al cambiar de tamaño la forma ...
void __fastcall TFormaCurvasIntegrales::FormResize(TObject *Sender)
{
   // Calcula la escala de la ventana de visualizacion
   Escala.X = (VCIntegrales->Width -1) / (Dim_Vtn.Xf - Dim_Vtn.Xi);
   Escala.Y = (VCIntegrales->Height -1) / (Dim_Vtn.Yf - Dim_Vtn.Yi);
   Grafica();        
}


///////////////////////////////////////////////////////////////////////////////
//  Definición de Comportamientos del Menu 
///////////////////////////////////////////////////////////////////////////////

// Menu->Archivo->Cerrar
void __fastcall TFormaCurvasIntegrales::MenuArchivoCerrarClick(TObject *Sender)
{
   Close();        
}

// Menu->Archivo->Grabar BMP
void __fastcall TFormaCurvasIntegrales::MenuArchivoGrabarBMPClick(TObject *Sender)
{
   TRect   xRect = Rect(0,0,VCIntegrales->Width,VCIntegrales->Height);
   Graphics::TBitmap *Bitmap = new Graphics::TBitmap;
   Bitmap->Width = VCIntegrales->Width;
   Bitmap->Height = VCIntegrales->Height;
   Bitmap->Canvas->CopyRect(xRect, VCIntegrales->Canvas, xRect);
   SaveDialog->Title = "Save as ...";
   if (SaveDialog->Execute()) {
      Bitmap->SaveToFile(SaveDialog->FileName);           
   }
   VCIntegrales->Canvas->Draw(0,0,Bitmap);
   delete Bitmap;
}

// Menu->Archivo->Imprimir
void __fastcall TFormaCurvasIntegrales::MenuArchivoImprimirClick(TObject *Sender)
{
   TRect   xRect = Rect(0,0,VCIntegrales->Width,VCIntegrales->Height);
   Graphics::TBitmap *Bitmap = new Graphics::TBitmap;
   Bitmap->Width = VCIntegrales->Width;
   Bitmap->Height = VCIntegrales->Height;
   Bitmap->Canvas->CopyRect(xRect, VCIntegrales->Canvas, xRect);
   VCIntegrales->Canvas->Draw(0,0,Bitmap);
   Printer()->BeginDoc();
   Printer()->Canvas->StretchDraw(Rect(50,50,Printer()->PageWidth-50,Printer()->PageWidth-50),Bitmap);
   Printer()->EndDoc();
   delete Bitmap;
}

// Menu->Calcular->Curvas Integrales
void __fastcall TFormaCurvasIntegrales::MenuCalcularCurvasIntegralesClick(TObject *Sender)
{
   Detener_calculo = false;
   Calcula_gama();
   Calcula_curv_int();
}

// Menu->Calcular->Numero de Rotación
void __fastcall TFormaCurvasIntegrales::MenuCalcularNumRotacionClick(TObject *Sender)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   long double p[NUM_MAX_PARAMETROS], rot;
   BarraDeEstadosCurvasIntegrales->SimpleText = (AnsiString) "Calculating rotation number (this process may last several seconds).";
   // Recupera los parámetros por omisión
   fun_disp.RetornaParametros(p);
   // Cambiar parámetros
   num_rot.AsignaParametros(p);
   rot = num_rot.Calcula(Tipo_integracion);
   char xcad[200];
   sprintf(xcad,"Rotation number: %Lf",rot);
   MessageBox(NULL,xcad,"Integral curves scenery",MB_OK+MB_ICONINFORMATION);
   BarraDeEstadosCurvasIntegrales->SimpleText = (AnsiString) " ";
   // Cambia el cursor del mouse
   Cursor = antcursor;
}

// Menu->Calcular->Sincronización
void __fastcall TFormaCurvasIntegrales::MenuCalcularSincronizacionClick(TObject *Sender)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   long double p[NUM_MAX_PARAMETROS];
   unsigned int xp, xq;
   BarraDeEstadosCurvasIntegrales->SimpleText = (AnsiString) "Calculating synchronization (this process may last several seconds).";
   // Recupera los parámetros por omisión
   fun_disp.RetornaParametros(p);
   // Cambiar parámetros
   sincro.AsignaParametros(p);
   sincro.Calcula(Tipo_integracion, xp, xq);
   char xcad[200];
   sprintf(xcad,"Synchronization: %u /%u ",xp,xq);
   MessageBox(NULL,xcad,"Integral curves scenery",MB_OK+MB_ICONINFORMATION);
   BarraDeEstadosCurvasIntegrales->SimpleText = (AnsiString) " ";
   // Cambia el cursor del mouse
   Cursor = antcursor;
}

// Menu->Calcular->Exponente de Lyapunov
void __fastcall TFormaCurvasIntegrales::MenuCalcularExpLyapunovClick(TObject *Sender)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   long double p[NUM_MAX_PARAMETROS], rot;
   BarraDeEstadosCurvasIntegrales->SimpleText = (AnsiString) "Calculating Lyapunov exponent (this process may last several seconds).";
   // Recupera los parámetros por omisión
   fun_disp.RetornaParametros(p);
   // Cambiar parámetros
   exp_lyap.AsignaParametros(p);
   rot = exp_lyap.Calcula(Tipo_integracion);
   char xcad[200];
   sprintf(xcad,"Lyapunov exponent: %Lf",rot);
   MessageBox(NULL,xcad,"Integral curves scenery",MB_OK+MB_ICONINFORMATION);
   BarraDeEstadosCurvasIntegrales->SimpleText = (AnsiString) " ";
   // Cambia el cursor del mouse
   Cursor = antcursor;
}

// Menu->Ventana->Limpiar
void __fastcall TFormaCurvasIntegrales::MenuVentanaLimpiarClick(TObject *Sender)
{
   Limpia_matriz_pixeles();
   Grafica();
}


// Menu->Configurar->Parametros
void __fastcall TFormaCurvasIntegrales::MenuConfigurarParametrosClick(TObject *Sender)
{
   configura = new TVCap_CIntegrales(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 0;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}

// Menu->Configurar->Dimensiones
void __fastcall TFormaCurvasIntegrales::MenuConfigurarDimensionesClick(TObject *Sender)
{
   configura = new TVCap_CIntegrales(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 1;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}

// Menu->Configurar->Visualizar
void __fastcall TFormaCurvasIntegrales::MenuConfigurarVisualizarClick(TObject *Sender)
{
   configura = new TVCap_CIntegrales(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 2;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}

// Menu->Configurar->Escenario
void __fastcall TFormaCurvasIntegrales::MenuConfigurarCalculoClick(TObject *Sender)
{
   configura = new TVCap_CIntegrales(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 3;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}


// Menu->Ayuda->Acerca de ...
void __fastcall TFormaCurvasIntegrales::MenuAyudaAcercadeClick(TObject *Sender)
{
   TVAcercaDe *Acercade = new TVAcercaDe(this);
   if (Acercade) {
      Acercade->ShowModal();
      delete Acercade;
   }
}

// Menu->Ayuda->Curvas integrales
void __fastcall TFormaCurvasIntegrales::MenuAyudaCurvasIntegralesClick(TObject *Sender)
{
   TAyudaForm *Ayuda = new TAyudaForm(this);
   if (Ayuda) {
      Ayuda->Abrir_archivo("Integral curves scenery","CurvasIntegrales.hlp");
      Ayuda->ShowModal();
      delete Ayuda;
   }
}



///////////////////////////////////////////////////////////////////////////////
//  Definición de comportamientos de Curvas Integrales 
///////////////////////////////////////////////////////////////////////////////


// Constructor de la clase
__fastcall TFormaCurvasIntegrales::TFormaCurvasIntegrales(TComponent* Owner) : TForm(Owner)
{
   Caption = "Integral curves scenery 1.0";
   Detener_calculo = false;
   // Dimenciones de la ventana
   Dim_Vtn.Xi = 0.0;
   Dim_Vtn.Xf = 10.0;
   Dim_Vtn.Yi = 0.0;
   Dim_Vtn.Yf = 5.0;
   // Longitud de la ventana de pixeles 
   Pix_x = 1024;
   Pix_y = 768;
   // Inicializa la ventana de pixeles
   pix[0].Inicializa(Pix_x,Pix_y, false, Dim_Vtn);
   pix[1].Inicializa(Pix_x,Pix_y, false, Dim_Vtn);
   pix[2].Inicializa(Pix_x,Pix_y, false, Dim_Vtn);
   pix[3].Inicializa(Pix_x,Pix_y, false, Dim_Vtn);
   pix[4].Inicializa(Pix_x,Pix_y, false, Dim_Vtn);
   // Condición inicial
   Condicion_inicial.X = 0.0;
   Condicion_inicial.Y = 0.0;
   // Color de los graficos
   Color_grafica[0] = clAqua;    // Grafico
   Color_grafica[1] = clRed;     // Linea del Grafico
   Color_grafica[2] = clYellow;  // Continuacion Grafico
   Color_grafica[3] = clBlue;    // Forzamiento
   Color_grafica[4] = clWhite;   // Referencia
   Color_fondo = clSilver;
   // Indica que graficos estaran activos
   Sw_Visualiza_grafico = true;
   Sw_Visualiza_linea_grf = true;
   Sw_Visualiza_continuacion_grf = true;
   Sw_Visualiza_forzamiento = true;
   Sw_Visualiza_referencia = true;
   // Puntos a trazar en la curva
   Puntos_trazar = Pix_x*5;
   // Tipo de inegración en este escenario
   Tipo_integracion = ADELANTE;
   // Calcular referencia
   Calcula_referencia();
}


void TFormaCurvasIntegrales::Calcula_gama(void)
{
   long double y ,t, paso = (Dim_Vtn.Xf - Dim_Vtn.Xi) / Puntos_trazar;
   int px, py;

   t = Dim_Vtn.Xi;
   while(t <= Dim_Vtn.Xf) {
      Application->ProcessMessages();
      y = fun_disp.G(t);
      pix[3].Asigna_valor(t, y, true);
      px = (t - Dim_Vtn.Xi) * Escala.X;
      py = VCIntegrales->Height - (((y - Dim_Vtn.Yi) * Escala.Y) + 1.0);
      if (Sw_Visualiza_forzamiento) {
         if (px >= 0 && px < VCIntegrales->Width && py >= 0 && py < VCIntegrales->Height) VCIntegrales->Canvas->Pixels[px][py] = Color_grafica[3];
      }   
      t += paso;
      if (Detener_calculo) break;
   }
}


// Grafica la curvas integrales
void TFormaCurvasIntegrales::Calcula_curv_int(void)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   long double y ,t, xt, xy, xxy;
   int px, py;

   BarraDeEstadosCurvasIntegrales->SimpleText=(AnsiString) "Calculating integral curves (this process may last several seconds).";
   t = Condicion_inicial.X;
   y = Condicion_inicial.Y;
   if(Tipo_integracion == ADELANTE) {   
      while(t <= Dim_Vtn.Xf) {
         Application->ProcessMessages();
         if (Detener_calculo) break;
         y = fun_disp.Integra(y,t,*fun_disp.Dis_Paso_integracion);
         if(fun_disp.Error_calculo) break;
         pix[0].Asigna_valor(t,y,true);
         px = (t - Dim_Vtn.Xi) * Escala.X;
         py = VCIntegrales->Height - (((y - Dim_Vtn.Yi) * Escala.Y) + 1.0);
         if (Sw_Visualiza_grafico) {
            if (px >= 0 && px < VCIntegrales->Width && py >= 0 && py < VCIntegrales->Height) VCIntegrales->Canvas->Pixels[px][py] = Color_grafica[0];
         }
         t += *fun_disp.Dis_Paso_integracion;
         if(y >= 1.0) {
            y = 0.0;
            pix[1].Asigna_linea(t,0.0,t,1.0);
            if (Sw_Visualiza_linea_grf) {
               VCIntegrales->Canvas->Pen->Color = Color_grafica[1];
               xxy = VCIntegrales->Height - (((-Dim_Vtn.Yi) * Escala.Y) + 1.0);
               VCIntegrales->Canvas->MoveTo(px, xxy);
               VCIntegrales->Canvas->LineTo(px,py);
            }   
            xt = t;
            xy = 1.0;
            while (xt <= Dim_Vtn.Xf) {
               Application->ProcessMessages();
               if (Detener_calculo) break;
               xy = fun_disp.Integra(xy,xt,*fun_disp.Dis_Paso_integracion);
               if(fun_disp.Error_calculo) break;
               pix[2].Asigna_valor(xt,xy,true);
               px = (xt - Dim_Vtn.Xi) * Escala.X;
               py = VCIntegrales->Height - (((xy - Dim_Vtn.Yi) * Escala.Y) + 1.0);
               if (Sw_Visualiza_continuacion_grf) {
                  if (px >= 0 && px < VCIntegrales->Width && py >= 0 && py < VCIntegrales->Height) VCIntegrales->Canvas->Pixels[px][py] = Color_grafica[2];
               }   
               xt += *fun_disp.Dis_Paso_integracion;
            }
         }
      }
    } else {
      while(t >= Dim_Vtn.Xi) {
         Application->ProcessMessages();
         if (Detener_calculo) break;
         y = fun_disp.Integra(y,t,-*fun_disp.Dis_Paso_integracion);
         pix[0].Asigna_valor(t,y,true);
         px = (t - Dim_Vtn.Xi) * Escala.X;
         py = VCIntegrales->Height - (((y - Dim_Vtn.Yi) * Escala.Y) + 1.0);
         if (Sw_Visualiza_grafico) {
            if (px >= 0 && px < VCIntegrales->Width && py >= 0 && py < VCIntegrales->Height) VCIntegrales->Canvas->Pixels[px][py] = Color_grafica[0];
         }   
         t -= *fun_disp.Dis_Paso_integracion;
         if (y <= 0.0) {
            y = 1.0;
            pix[1].Asigna_linea(t,0.0,t,1.0);
            if (Sw_Visualiza_linea_grf) {
               VCIntegrales->Canvas->Pen->Color = Color_grafica[1];
               xxy = VCIntegrales->Height - (((-Dim_Vtn.Yi) * Escala.Y) + 1.0);
               VCIntegrales->Canvas->MoveTo(px,xxy);
               VCIntegrales->Canvas->LineTo(px,py);
            }   
            xt = t;
            xy = 1.0;
            while (xt <= Dim_Vtn.Xi) {
               Application->ProcessMessages();
               if (Detener_calculo) break;
               xy = fun_disp.Integra(xy,xt,*fun_disp.Dis_Paso_integracion);
               if(fun_disp.Error_calculo) break;
               pix[2].Asigna_valor(xt,xy,true);
               px = (xt - Dim_Vtn.Xi) * Escala.X;
               py = VCIntegrales->Height - (((xy - Dim_Vtn.Yi) * Escala.Y) + 1.0);
               if (Sw_Visualiza_continuacion_grf) {
                  if (px >= 0 && px < VCIntegrales->Width && py >= 0 && py < VCIntegrales->Height) VCIntegrales->Canvas->Pixels[px][py] = Color_grafica[2];
               }   
               xt += *fun_disp.Dis_Paso_integracion;
            }
         }
      }
   }
   BarraDeEstadosCurvasIntegrales->SimpleText=(AnsiString) " ";
   // Cambia el cursor del mouse
   Cursor = antcursor;
}


// Grafica la referencia de la ventana
void TFormaCurvasIntegrales::Calcula_referencia(void)
{
   pix[4].Asigna_linea(Dim_Vtn.Xi,1.0,Dim_Vtn.Xf,1.0);
   pix[4].Asigna_linea(Dim_Vtn.Xi,0.0,Dim_Vtn.Xf,0.0);
}

// Grafica las curvas calculadas
void TFormaCurvasIntegrales::Grafica(void)
{
   unsigned int x,y,i;
   // Calcula la escala de la ventana de visualizacion con respecto a la ventana de pixeles
   C2D escala;
   
   Limpiar_vantana();
   escala.X = VCIntegrales->Width / (long double) Pix_x;
   escala.Y = VCIntegrales->Height / (long double) Pix_y;

   for (i = 0; i < 5; i++) {
      // Controla que solo se visualize lo necesario
      if(i == 0 && !Sw_Visualiza_grafico) i++;
      if(i == 1 && !Sw_Visualiza_linea_grf) i++;
      if(i == 2 && !Sw_Visualiza_continuacion_grf) i++;
      if(i == 3 && !Sw_Visualiza_forzamiento) i++;
      if(i == 4 && !Sw_Visualiza_referencia) break;
      // Asigna el color a la grafica
      for (y = 0; y < Pix_y; y++) {
          for (x = 0; x < Pix_x; x++) {
             if(pix[i].Retorna_valor(x,y)) VCIntegrales->Canvas->Pixels[x * escala.X][y * escala.Y] = Color_grafica[i];
          }
       }
   }
}


// Limpia la ventana de graficacion
void TFormaCurvasIntegrales::Limpiar_vantana(void)
{
   VCIntegrales->Canvas->Pen->Color = Color_fondo;
   VCIntegrales->Canvas->Brush->Color = Color_fondo;
   VCIntegrales->Canvas->Rectangle(0, 0, VCIntegrales->Width, VCIntegrales->Height);
}


//#error Si se solicita cerrar ventana cancelar calculo y visualización





void __fastcall TFormaCurvasIntegrales::VCIntegralesMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
   Vs.Xf = X, Vs.Yf = Y;
   Vs.Xi = Vs.Xf / Escala.X + Dim_Vtn.Xi;
   Vs.Yi = (-(Vs.Yf - VCIntegrales->Height) / Escala.Y) + Dim_Vtn.Yi;

   sprintf(Msg,"Mouse position: (%3.8Lf, %3.8Lf)", Vs.Xi, Vs.Yi);
   BarraDeEstadosCurvasIntegrales->SimpleText = (AnsiString) Msg;
}


void __fastcall TFormaCurvasIntegrales::FijarCondicIninicialClick(TObject *Sender)
{
   Condicion_inicial.X = Vs.Xi; 
   Condicion_inicial.Y = 0.0;
}


// Pasa los valores de configuración de la ventana de captura
void TFormaCurvasIntegrales::PasarValoresConfiguracion(void)
{
   ////////////////////////////////////////////////////////
   // Parámetros
   ////////////////////////////////////////////////////////
   // Parámetros
   configura->ListBoxParametros->Items->Clear();
   for( int i = 0; i < fun_disp.Numero_parametros; i++) {
      sprintf(xcad,"%-6s = %1.9Lf",fun_disp.Nombre_parametros[i],fun_disp.P[i] );
      configura->ListBoxParametros->Items->Add(xcad);
   }
   ////////////////////////////////////////////////////////
   // Dimensiones
   ////////////////////////////////////////////////////////
   sprintf(xcad,"%Lf",Dim_Vtn.Xi);
   configura->EditHMin->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",Dim_Vtn.Xf);
   configura->EditHMax->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",Dim_Vtn.Yi);
   configura->EditVMin->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",Dim_Vtn.Yf);
   configura->EditVMax->Text = (AnsiString) xcad;
   ////////////////////////////////////////////////////////
   // Visualizar
   ////////////////////////////////////////////////////////
   // Check Box
   configura->CheckBox1->Checked = Sw_Visualiza_grafico;
   configura->CheckBox2->Checked = Sw_Visualiza_continuacion_grf;
   configura->CheckBox3->Checked = Sw_Visualiza_linea_grf;
   configura->CheckBox4->Checked = Sw_Visualiza_forzamiento;
   configura->CheckBox5->Checked = Sw_Visualiza_referencia;
   // Colores
   configura->Panel5->Color = Color_grafica[0];
   configura->Panel6->Color = Color_grafica[2];
   configura->Panel7->Color = Color_grafica[1];
   configura->Panel8->Color = Color_grafica[3];
   configura->Panel9->Color = Color_grafica[4];
   configura->Panel10->Color = Color_fondo;
   ////////////////////////////////////////////////////////
   // Escenario
   ////////////////////////////////////////////////////////
   sprintf(xcad,"%Lf",Condicion_inicial.X);
   configura->Edit8->Text = (AnsiString) xcad;
   configura->RadioButton1->Checked = Tipo_integracion;   
   configura->RadioButton2->Checked = !Tipo_integracion;   
}

// Retorna los valores de configuración de la ventana de captura
void TFormaCurvasIntegrales::RetornarValoresConfiguracion(void)
{
   if (!configura->Aceptar) return;
   ////////////////////////////////////////////////////////
   // Parámetros
   ////////////////////////////////////////////////////////
   for (int i = 0; i < fun_disp.Numero_parametros; i++) {
       unsigned int i1, i2;
       char xcad[100], xcad1[100];
       strcpy(xcad1,configura->ListBoxParametros->Items->Strings[i].c_str());
       // Valor del parámetro
       for (i2 = 0, i1 = 9; i1 < strlen(xcad1); i2++, i1++) xcad[i2] = xcad1[i1];
       xcad[i2] = 0;
       fun_disp.P[i] = _atold(xcad);
   }
   ////////////////////////////////////////////////////////
   // Dimensiones
   ////////////////////////////////////////////////////////
   Dim_Vtn.Xi = _atold(configura->EditHMin->Text.c_str());       
   Dim_Vtn.Xf = _atold(configura->EditHMax->Text.c_str());       
   Dim_Vtn.Yi = _atold(configura->EditVMin->Text.c_str());       
   Dim_Vtn.Yf = _atold(configura->EditVMax->Text.c_str());       
   ////////////////////////////////////////////////////////
   // Visualizar
   ////////////////////////////////////////////////////////
   // Check Box
   Sw_Visualiza_grafico          = configura->CheckBox1->Checked;
   Sw_Visualiza_continuacion_grf = configura->CheckBox2->Checked;
   Sw_Visualiza_linea_grf        = configura->CheckBox3->Checked;
   Sw_Visualiza_forzamiento      = configura->CheckBox4->Checked;
   Sw_Visualiza_referencia       = configura->CheckBox5->Checked;
   // Colores
   Color_grafica[0] = configura->Panel5->Color;
   Color_grafica[2] = configura->Panel6->Color;
   Color_grafica[1] = configura->Panel7->Color;
   Color_grafica[3] = configura->Panel8->Color;
   Color_grafica[4] = configura->Panel9->Color;
   Color_fondo      = configura->Panel10->Color;
   ////////////////////////////////////////////////////////
   // Escenario
   ////////////////////////////////////////////////////////
   Condicion_inicial.X = _atold(configura->Edit8->Text.c_str());       
   Tipo_integracion    = configura->RadioButton1->Checked;   
   /////////////////////////////////////////////////////////
   // Al terminar
   /////////////////////////////////////////////////////////
   pix[0].Cambia_dimension(Dim_Vtn);
   pix[1].Cambia_dimension(Dim_Vtn);
   pix[2].Cambia_dimension(Dim_Vtn);
   pix[3].Cambia_dimension(Dim_Vtn);
   pix[4].Cambia_dimension(Dim_Vtn);
   Calcula_referencia();
   FormResize(this);
}

void __fastcall TFormaCurvasIntegrales::Timer1Timer(TObject *Sender)
{
   // Almacena la ventana activa
   if (Focused()) {
      Ventana_activa = this;
   }   
   // Revisa si es la ventana activa 
   if (Ventana_activa == this) {
      // Calcula el número de rotacion
      if (Calcular_numero_rotacion) {
          Calcular_numero_rotacion = false;
          MenuCalcularNumRotacionClick(this);
      }
      // Calcula la sincronización
      if (Calcular_sincronizaciones) {
          Calcular_sincronizaciones = false;
          MenuCalcularSincronizacionClick(this);
      }
      // Calcula el exponente de Luapunov
      if (Calcular_exponente_lyapunov) {
          Calcular_exponente_lyapunov = false;
          MenuCalcularExpLyapunovClick(this);
      }
      // Configura el escenario
      if (Configura_escenario) {
          Configura_escenario = false;
          MenuConfigurarParametrosClick(this);
      }
      // Calcula
      if (Calcula_escenario) {
          Calcula_escenario = false;
          MenuCalcularCurvasIntegralesClick(this);
      }
      // Detener cálculo
      if (Detener_Calculo) {
          Detener_Calculo = false;
          MenuCalcularDetenerelcalculoClick(this);
      }
      // Limpia la ventana
      if (Limpiar_ventana) {
          Limpiar_ventana = false;
          MenuVentanaLimpiarClick(this);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormaCurvasIntegrales::MenuCalcularDetenerelcalculoClick(TObject *Sender)
{
   Detener_calculo = true;        
}


// #error revisar que funcione correctamente el control de visualizacion (poder no visializar algo)


void __fastcall TFormaCurvasIntegrales::VCIntegralesPaint(TObject *Sender)
{
   Grafica();        
}

