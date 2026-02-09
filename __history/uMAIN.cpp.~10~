//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
using namespace std;

#include "uMAIN.h"
#include <filesystem>
#include <fstream>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
string File;
string Path;
string zipPath;

void unzipDOCX(string file) {
	system(("cd " + Path + " && mkdir temp && cd temp && tar -xf " + File).c_str());
}

void CesarBytes(vector<char>& data, int Step) {
    for (char& byte : data) {
		byte = (byte + Step) & 0xFF;
	}
}

void ImageCrypting(filesystem::path ImgPath, int Step) {
	const size_t BUFFER_SIZE = 1024;  // Размер буфера (1 КБ)
	string temp_file_path = ImgPath.parent_path().string() + "\\temp.jpeg";

	ifstream file1(ImgPath, ios::binary);
	ofstream file2(temp_file_path, ios::binary);

	vector<char> buffer(BUFFER_SIZE);
	while (file1.read(buffer.data(), BUFFER_SIZE)) {
		size_t bytesRead = file1.gcount();
		CesarBytes(buffer, Step);
		file2.write(buffer.data(), bytesRead);
	}

	// Обрабатываем остаток (если файл не кратен BUFFER_SIZE)
	size_t remaining = file1.gcount();
	if (remaining > 0) {
		CesarBytes(buffer, Step);
		file2.write(buffer.data(), remaining);
	}

	file1.close();
	file2.close();
	system(("cd " + ImgPath.parent_path().string() +
			" && del " + ImgPath.filename().string() +
			" && ren temp.jpeg " + ImgPath.filename().string()).c_str());
}

string Cesar(string s, int Step) {
	for (int i = 0; i < s.length(); i++) {
		s[i] += Step;
	}
	return s;
}

void Crypt_TXT_file(filesystem::path file, int Step) {
	fstream f(file, ios::in);
	string temp_file_path = file.parent_path().string() + "\\temp.txt";
	fstream temp_file(temp_file_path, ios::out);
	while (!f.eof()) {
		string temp;
		getline(f, temp);
		temp_file << Cesar(temp, Step) << endl;
	}
	f.close();
	temp_file.close();
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
    File = AnsiString(PATH->Text).c_str();
	Path = filesystem::path(File).parent_path().u8string();
	zipPath = filesystem::current_path().u8string();
	unzipDOCX(File);
	Path = Path + "\\temp";
	for (filesystem::path file : filesystem::recursive_directory_iterator(Path)) {
		if (filesystem::is_directory(file)) {} //Тут всё что нужнос делать с папками
		if (filesystem::is_regular_file(file)) {
			if (file.extension() != ".jpeg" || file.extension() != ".jpg") { //Обработчик txt файлов
				Crypt_TXT_file(file, -1);
			}
			else {
				ImageCrypting(file, -1);
            }
		}
	}
	system(("cd " + Path +
			" && " + zipPath + "\\7z " + "a -tzip -mx5 -r0 " + string(&Path[0], &Path[Path.rfind("\\")]) +
			"\\Deshifred.zip * && cd .. && ren Deshifred.zip Deshifred.docx && rmdir temp /s /q").c_str());
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ZIPClick(TObject *Sender)
{
	File = AnsiString(PATH->Text).c_str();
	Path = filesystem::path(File).parent_path().u8string();
	zipPath = filesystem::current_path().u8string();
	unzipDOCX(File);
	Path = Path + "\\temp";
	for (filesystem::path file : filesystem::recursive_directory_iterator(Path)) {
		if (filesystem::is_directory(file)) {} //Тут всё что нужнос делать с папками
		if (filesystem::is_regular_file(file)) {
			if (file.extension() != ".jpeg" || file.extension() != ".jpg") { //Обработчик txt файлов
				Crypt_TXT_file(file, 1);
			}
			else {
				ImageCrypting(file, 1);
			}
		}
	}
	system(("cd " + Path +
			" && " + zipPath + "\\7z " + "a -tzip -mx5 -r0 " + string(&Path[0], &Path[Path.rfind("\\")]) +
			"\\Shifred.zip * && cd .. && ren Shifred.zip Shifred.docx && rmdir temp /s /q").c_str());
}
//---------------------------------------------------------------------------

