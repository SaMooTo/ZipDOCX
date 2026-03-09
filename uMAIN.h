//---------------------------------------------------------------------------

#ifndef uMAINH
#define uMAINH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TOpenDialog *OpenDialogDOCX;
	TButton *UNZIP;
	TButton *CHOOSE;
	TEdit *PATH;
	TButton *ZIP;
	void __fastcall CHOOSEClick(TObject *Sender);
	void __fastcall UNZIPClick(TObject *Sender);
	void __fastcall ZIPClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
