//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "uMAIN.h"
#include <filesystem>
#include <stdio.h>
#include "xtea3.cpp"
#include <random>
using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define SIZE_FILE 100*1024*1024

TForm1 *Form1;
string File;
string Path;
string zipPath = filesystem::current_path().u8string();

static uint8_t tmp;
static uint8_t buffer[SIZE_FILE];
uint32_t key[8];

void unzipDOCX(string file) {
	system(("cd " + Path + " && mkdir temp && " + zipPath + "\\7z x \"" + File + "\" -o" + Path + "\\temp").c_str());
}

void CryptFile(filesystem::path file) {
	xtea3 *ptr_xtea_lib = new xtea3;

	unsigned int count = 0;
	unsigned int size_file = 0;
	string temp_file_path = file.parent_path().string() + "\\temp.txt";

	FILE *hFileOpen = fopen(file.string().c_str(), "rb");
	FILE *hFileCrypt = fopen(temp_file_path.c_str(), "wb+");

	if ((hFileOpen == NULL) || (hFileCrypt == NULL)) return;
	while (!feof(hFileOpen))
	{
		tmp = getc(hFileOpen);
		if (size_file < SIZE_FILE) buffer[size_file] = tmp;
		else return;
		size_file++;
	}

	fclose(hFileOpen);
	//Зашифруем буфер:
	uint8_t *p_crypt_data = ptr_xtea_lib->data_crypt((uint8_t*)buffer, key, size_file - 1);
	if (p_crypt_data == NULL) return;

	//Сохраним зашифрованные данные на диск:
	fwrite(p_crypt_data, 1, ptr_xtea_lib->get_crypt_size(), hFileCrypt);

	//Закроем файл
	fclose(hFileCrypt);
	system(("cd " + file.parent_path().string() +
			" && del " + file.filename().string() +
			" && ren temp.txt " + file.filename().string()).c_str());
}

void DecryptFile(filesystem::path file) {
	xtea3 *ptr_xtea_lib = new xtea3;

	unsigned int count = 0;
	unsigned int size_file = 0;
	string temp_file_path = file.parent_path().string() + "\\temp.txt";

	FILE *hFileOpen = fopen(file.string().c_str(), "rb");
	FILE *hFileDecrypt = fopen(temp_file_path.c_str(), "wb+");

	if ((hFileOpen == NULL) || (hFileDecrypt == NULL)) return;
	while (!feof(hFileOpen))
	{
		tmp = getc(hFileOpen);
		if (size_file < SIZE_FILE) buffer[size_file] = tmp;
		else return;
		size_file++;
	}

	fclose(hFileOpen);
	//Зашифруем буфер:
	uint8_t *p_decrypt_data = ptr_xtea_lib->data_decrypt((uint8_t*)buffer, key, size_file - 1);
	if (p_decrypt_data == NULL) return;

	//Сохраним зашифрованные данные на диск:
	fwrite(p_decrypt_data, 1, ptr_xtea_lib->get_decrypt_size(), hFileDecrypt);

	//Закроем файл
	fclose(hFileDecrypt);
	system(("cd " + file.parent_path().string() +
			" && del " + file.filename().string() +
			" && ren temp.txt " + file.filename().string()).c_str());
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CHOOSEClick(TObject *Sender)
{
	if (OpenDialogDOCX->Execute()) {
		PATH->Text = OpenDialogDOCX->FileName;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UNZIPClick(TObject *Sender)
{
	if (PATH->Text == "") {
		ShowMessage("Неправильный формат директории файла!");
		return;
	}
	if (Edit1->Text == "") {
		ShowMessage("Неправильный формат ключа!");
		return;
	}
	File = AnsiString(PATH->Text).c_str();
	Path = filesystem::path(File).parent_path().u8string();
	unzipDOCX(File);
	Path = Path + "\\temp";
	for (filesystem::path file : filesystem::recursive_directory_iterator(Path)) {
		if (filesystem::is_directory(file)) {} //Тут всё что нужнос делать с папками
		if (filesystem::is_regular_file(file)) DecryptFile(file);
	}
	system(("cd " + Path +
			" && " + zipPath + "\\7z a -tzip -mx5 -r0 " + string(&Path[0], &Path[Path.rfind("\\")]) +
			"\\Deshifred.zip * && cd .. && ren Deshifred.zip Deshifred-\"" + filesystem::path(File).filename().string() + "\"&& rmdir temp /s /q").c_str());
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ZIPClick(TObject *Sender)
{
    if (PATH->Text == "") {
		ShowMessage("Неправильный формат директории файла!");
		return;
	}
	if (Edit1->Text == "") {
		ShowMessage("Неправильный формат ключа!");
		return;
	}
	File = AnsiString(PATH->Text).c_str();
	Path = filesystem::path(File).parent_path().u8string();
	unzipDOCX(File);
	Path = Path + "\\temp";
	for (filesystem::path file : filesystem::recursive_directory_iterator(Path)) {
		if (filesystem::is_directory(file)) {} //Тут всё что нужнос делать с папками
		if (filesystem::is_regular_file(file)) CryptFile(file);
	}
	system(("cd " + Path +
			" &&" + zipPath + "\\7z a -tzip -mx5 -r0 " + string(&Path[0], &Path[Path.rfind("\\")]) +
			"\\Shifred.zip * && cd .. && ren Shifred.zip Shifred-\"" + filesystem::path(File).filename().string() + "\"&& rmdir temp /s /q").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::keyGenClick(TObject *Sender)
{
	srand(time(NULL));
	Edit1->Text = "";
	for (int i = 0; i < 8; i++) {
		int r = rand() % 255 + 1;
		key[i] = r;
		if (0 <= r && r <= 9) Edit1->Text += ("00" + to_string(r)).c_str();
		else if (10 <= r && r <= 99) Edit1->Text += ("0" + to_string(r)).c_str();
		else Edit1->Text += + to_string(r).c_str();
	}
}
void __fastcall TForm1::saveKeyClick(TObject *Sender)
{
	if (Edit1->Text == "") {
		ShowMessage("Неправильный формат ключа. Сгенерируй новый");
		return;
	}
	if (keySaveDialog->Execute()) {
		FILE *hFileSave = fopen(AnsiString(keySaveDialog->FileName).c_str(), "wb+");
		fwrite(Edit1->Text.c_str(), 1, 48, hFileSave);
		fclose(hFileSave);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::loadKeyClick(TObject *Sender)
{
	if (keyOpenDialog->Execute()) {
		FILE *hFileOpen = fopen(AnsiString(keyOpenDialog->FileName).c_str(), "rb");
		char buffer[24];
		for (int i = 0; i < 48; i++) {
			char tmp = char(getc(hFileOpen));
			Edit1->Text += tmp;
			if (tmp == 0) continue;
			buffer[i/2] = tmp;
		}
		fclose(hFileOpen);

		bool conversionSuccess = true;
		for (int i = 0; i < 8; ++i) {
            char block[4] = {0};
			strncpy(block, buffer + i * 3, 3);
			char* endPtr;
			key[i] = static_cast<uint32_t>(strtoul(block, &endPtr, 10));
			if (endPtr == block) {
                conversionSuccess = false;
                break;
            }
		}
	}
}
//---------------------------------------------------------------------------

