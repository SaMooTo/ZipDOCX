#include <iostream>
#include <tchar.h>
#include <fstream>

using namespace std;

// Функция шифрования/дешифрования (пример для байтов)
void CesarBytes(vector<char>& data, int Step) {
    for (char& byte : data) {
        byte = (byte + Step) & 0xFF;  // Учитываем переполнение
    }
}

int _tmain(int argc, _TCHAR* argv[]) {

    const size_t BUFFER_SIZE = 1024;  // Размер буфера (1 КБ)

    // Шифрование
    std::ifstream file1("C:\\Users\\Gustov\\Desktop\\image1.jpg", std::ios::binary);
    std::ofstream file2("C:\\Users\\Gustov\\Desktop\\image2.txt", std::ios::binary);

    std::vector<char> buffer(BUFFER_SIZE);
    while (file1.read(buffer.data(), BUFFER_SIZE)) {
        size_t bytesRead = file1.gcount();
        CesarBytes(buffer, 1);
        file2.write(buffer.data(), bytesRead);
    }

    // Обрабатываем остаток (если файл не кратен BUFFER_SIZE)
    size_t remaining = file1.gcount();
    if (remaining > 0) {
        CesarBytes(buffer, 1);
        file2.write(buffer.data(), remaining);
    }

    file1.close();
    file2.close();

    // Дешифрование
    std::ifstream file3("C:\\Users\\Gustov\\Desktop\\image2.txt", std::ios::binary);
    std::ofstream file4("C:\\Users\\Gustov\\Desktop\\image3.jpg", std::ios::binary);  // ! Важно: .jpg, а не .txt

    while (file3.read(buffer.data(), BUFFER_SIZE)) {
        size_t bytesRead = file3.gcount();
        CesarBytes(buffer, -1);  // Обратный сдвиг
        file4.write(buffer.data(), bytesRead);
    }

    remaining = file3.gcount();
    if (remaining > 0) {
        CesarBytes(buffer, -1);
        file4.write(buffer.data(), remaining);
    }

    file3.close();
    file4.close();

    system("pause");
    return 0;
}
