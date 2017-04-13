// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Direcci�n: Amsterdam 312 col. Hip�dromo Condesa
// Tel�fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr�mite 1999-2000
// Revisi�n  1.1-A


//---------------------------------------------------------------------------
#ifndef ParamLevantamientosH
#define ParamLevantamientosH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Tabnotbk.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Dialogs.hpp>
//---------------------------------------------------------------------------
class TVCap_Levantamientos : public TForm
{
__published:	// IDE-managed Components
        TTabbedNotebook *TabbedNotebook1;
        TPanel *Panel1;
        TPanel *Panel2;
        TGroupBox *GroupBox2;
        TLabel *Label6;
        TLabel *Label7;
        TGroupBox *GroupBox1;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *EditHMin;
        TEdit *EditHMax;
        TEdit *EditVMin;
        TEdit *EditVMax;
        TPanel *Panel3;
        TCheckBox *CheckBox1;
        TCheckBox *CheckBox2;
        TPanel *Panel5;
        TPanel *Panel6;
        TPanel *Panel4;
        TLabel *Label8;
        TEdit *Edit8;
        TGroupBox *GroupBox3;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TButton *Button1;
        TColorDialog *ColorDialog;
        TCheckBox *CheckBox5;
        TPanel *Panel9;
        TLabel *Label2;
        TEdit *Edit7;
        TLabel *Label10;
        TEdit *Edit9;
        TStaticText *StaticText1;
        TCheckBox *CheckBox3;
        TPanel *Panel7;
        TCheckBox *CheckBox4;
        TPanel *Panel8;
        TListBox *ListBoxParametros;
        TStaticText *NombreParametro;
        TEdit *EditValorParametro;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Panel5Click(TObject *Sender);
        void __fastcall Panel6Click(TObject *Sender);
        void __fastcall Panel7Click(TObject *Sender);
        void __fastcall Panel8Click(TObject *Sender);
        void __fastcall Panel9Click(TObject *Sender);
        void __fastcall ListBoxParametrosClick(TObject *Sender);
        void __fastcall EditValorParametroKeyPress(TObject *Sender,
          char &Key);
private:	// User declarations
public:		// User declarations
        bool Aceptar;

        __fastcall TVCap_Levantamientos(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
 
