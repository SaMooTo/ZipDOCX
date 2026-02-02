//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <jpeg.hpp>
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
TJPEGImage *jpg = new TJPEGImage();
TBitmap *Bmp = new Graphics::TBitmap();
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
	if (OpenDialog1 -> Execute()) {
		jpg->LoadFromFile(OpenDialog1->FileName);
		Bmp->Assign(jpg);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button2Click(TObject *Sender)
{
	Form2->Caption = Bmp->Width;
}
//---------------------------------------------------------------------------

