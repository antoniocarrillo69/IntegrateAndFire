// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Direcci�n: Amsterdam 312 col. Hip�dromo Condesa
// Tel�fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr�mite 1999-2000
// Revisi�n  1.1-A


//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include "ParamToro.h"
#pragma hdrstop


//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

// Constructor de la clase
__fastcall TVCap_Toro::TVCap_Toro(TComponent* Owner) : TForm(Owner)
{
   Aceptar = false;
}

// Al presionar el boton de acptar
void __fastcall TVCap_Toro::Button1Click(TObject *Sender)
{
   Aceptar = true;
   Close();        
}

// Cambio de color
void __fastcall TVCap_Toro::Panel5Click(TObject *Sender)
{
   ColorDialog->Color = Panel5->Color;
   if (ColorDialog->Execute()) Panel5->Color = ColorDialog->Color;           
}

// Cambio de color
void __fastcall TVCap_Toro::Panel6Click(TObject *Sender)
{
   ColorDialog->Color = Panel6->Color;
   if (ColorDialog->Execute()) Panel6->Color = ColorDialog->Color;           
}

// Cambio de color
void __fastcall TVCap_Toro::Panel7Click(TObject *Sender)
{
   ColorDialog->Color = Panel7->Color;
   if (ColorDialog->Execute()) Panel7->Color = ColorDialog->Color;           
}

// Cambio de color
void __fastcall TVCap_Toro::Panel8Click(TObject *Sender)
{
   ColorDialog->Color = Panel8->Color;
   if (ColorDialog->Execute()) Panel8->Color = ColorDialog->Color;           
}


// Activa la captura del par�metro seleccionado
void __fastcall TVCap_Toro::ListBoxParametrosClick(TObject *Sender)
{
   unsigned int Log_max_param = 6; // longitud m�xima del nombre del par�metro
   unsigned int i1, i2;
   int i;
   for (i = 0; i < ListBoxParametros->Items->Count; i++) {
       if (ListBoxParametros->Selected[i]) {
           char xcad[100], xcad1[100];
           // Nombre del par�metro
           strcpy(xcad1,ListBoxParametros->Items->Strings[i].c_str());
           for (i1 = 0; i1 < Log_max_param; i1++) { 
               if (xcad1[i1] != ' ') xcad[i1] = xcad1[i1];
                else break;
           }
           xcad[i1] = ':';
           xcad[i1+1] = 0;
           NombreParametro->Caption = xcad;
           // Valor del par�metro
           for (i2 = 0, i1 = (Log_max_param + 3); i1 < strlen(xcad1); i2++, i1++) { 
               xcad[i2] = xcad1[i1];
           }
           xcad[i2] = 0;
           EditValorParametro->Text = xcad;
           // Se hace visible el nombre y valor del par�metro
           NombreParametro->Visible = true;
           EditValorParametro->Visible = true;     
           break;
       }
   }
}


// Cambia el valor del par�metro activo
void __fastcall TVCap_Toro::EditValorParametroKeyPress(TObject *Sender, char &Key)
{
   if (Key == VK_RETURN) {
      Key = 0;
      PostMessage(Handle, WM_NEXTDLGCTL,0,0);
      char xcad[100], xcad1[100];
      strcpy(xcad1,NombreParametro->Caption.c_str());
      int i, lg;
      lg = strlen(xcad1);
      xcad1[lg-1] = 0;
      sprintf(xcad,"%-6s = %1.9Lf",xcad1,_atold(EditValorParametro->Text.c_str()));
      for (i = 0; i < ListBoxParametros->Items->Count; i++) {
          if (ListBoxParametros->Selected[i]) ListBoxParametros->Items->Strings[i] = xcad; 
      }    
      NombreParametro->Visible = false;
      EditValorParametro->Visible = false;     
   }
}


