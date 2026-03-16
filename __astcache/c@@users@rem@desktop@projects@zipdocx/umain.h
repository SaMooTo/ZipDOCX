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
	TEdit *Edit1;
	TButton *keyGen;
	TButton *saveKey;
	TButton *loadKey;
	TOpenDialog *keyOpenDialog;
	TSaveDialog *keySaveDialog;
	void __fastcall CHOOSEClick(TObject *Sender);
	void __fastcall UNZIPClick(TObject *Sender);
	void __fastcall ZIPClick(TObject *Sender);
	void __fastcall keyGenClick(TObject *Sender);
	void __fastcall saveKeyClick(TObject *Sender);
	void __fastcall loadKeyClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
