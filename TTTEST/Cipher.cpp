#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <System.hpp>

using namespace std;

// Класс для шифрования UnicodeString
class UnicodeEncryptor {
public:
	UnicodeString key;

    // Генерация ключа на основе seed
	UnicodeString generateKey(size_t length, int seed = 0) {
		UnicodeString key;
		mt19937 gen(seed == 0 ? time(nullptr) : seed);
        uniform_int_distribution<int> dist(0x0020, 0xFFFF);

        for (size_t i = 0; i < length; ++i) {
			key += static_cast<wchar_t>(dist(gen));
        }
        return key;
    }

    // XOR шифрование
	UnicodeString xorEncrypt(const UnicodeString& input, const UnicodeString& key) {
        UnicodeString output;
        for (int i = 1; i <= input.Length(); ++i) {
            wchar_t inputChar = input[i];
			wchar_t keyChar = key[(i - 1) % key.Length() + 1];
            output += static_cast<wchar_t>(inputChar ^ keyChar);
		}
		return output;
	}

    // Перестановка символов
    UnicodeString permuteEncrypt(const UnicodeString& input, int seed) {
        // Конвертируем в вектор для удобства работы
        vector<wchar_t> chars;
        for (int i = 1; i <= input.Length(); ++i) {
            chars.push_back(input[i]);
        }

        mt19937 gen(seed);
        shuffle(chars.begin(), chars.end(), gen);

        // Конвертируем обратно в UnicodeString
        UnicodeString output;
        for (wchar_t c : chars) {
            output += c;
        }
        return output;
    }

    // Обратная перестановка
    UnicodeString permuteDecrypt(const UnicodeString& input, int seed, const vector<int>& indices) {
        UnicodeString output;
        output.SetLength(input.Length());

        // Восстанавливаем оригинальный порядок
        for (int i = 0; i < indices.size(); ++i) {
            output[indices[i] + 1] = input[i + 1];
        }

        return output;
    }

    // Получение индексов для восстановления перестановки
    vector<int> getPermutationIndices(int length, int seed) {
        vector<int> indices(length);
        for (int i = 0; i < length; ++i) {
            indices[i] = i;
        }

        mt19937 gen(seed);
        shuffle(indices.begin(), indices.end(), gen);

        return indices;
    }

    UnicodeEncryptor(const UnicodeString& customKey = UnicodeString()) {
        if (customKey.IsEmpty()) {
			key = generateKey(16); // Генерация ключа по умолчанию
        } else {
            key = customKey;
        }
    }

    // Основной метод шифрования
    UnicodeString encrypt(const UnicodeString& input, int seed = 42) {
        if (input.IsEmpty()) return UnicodeString();

        // Шаг 1: XOR шифрование
        UnicodeString step1 = xorEncrypt(input, key);

        // Шаг 2: Получаем индексы для перестановки
        vector<int> indices = getPermutationIndices(step1.Length(), seed);

        // Шаг 3: Перестановка символов
        UnicodeString step2 = permuteEncrypt(step1, seed);

        // Шаг 4: Дополнительное XOR с ключом в обратном порядке
        UnicodeString reversedKey;
        for (int i = key.Length(); i >= 1; --i) {
            reversedKey += key[i];
        }
        UnicodeString encrypted = xorEncrypt(step2, reversedKey);

        return encrypted;
    }

    // Метод дешифрования
	UnicodeString decrypt(const UnicodeString& encrypted, int seed = 42) {
        if (encrypted.IsEmpty()) return UnicodeString();

        // Шаг 1: Обратное XOR с ключом в обратном порядке
        UnicodeString reversedKey;
        for (int i = key.Length(); i >= 1; --i) {
            reversedKey += key[i];
        }
        UnicodeString step1 = xorEncrypt(encrypted, reversedKey);

        // Шаг 2: Получаем индексы для восстановления перестановки
        vector<int> indices = getPermutationIndices(step1.Length(), seed);

        // Шаг 3: Восстанавливаем перестановку
        UnicodeString step2 = permuteDecrypt(step1, seed, indices);

        // Шаг 4: Обратное XOR шифрование
        UnicodeString decrypted = xorEncrypt(step2, key);

        return decrypted;
    }

    // Получить текущий ключ
	UnicodeString getKey() const {
        return key;
	}

    // Установить новый ключ
	void setKey(const UnicodeString& newKey) {
		key = newKey;
	}
};

// Функция для отображения hex представления
/*void printHex(const UnicodeString& str, const string& label) {
    cout << label << " (hex): ";
    for (int i = 1; i <= str.Length(); ++i) {
        wchar_t c = str[i];
        cout << hex << static_cast<int>(c) << " ";
    }
    cout << dec << endl;
}

int main() {
    // Пример использования
    cout << "=== UnicodeString Encryption ===" << endl;

    // Создаем шифратор с кастомным ключом
	UnicodeEncryptor encryptor(L"МойСекретныйКлюч123");

    // Исходный текст
	UnicodeString originalText = L"Привет, мир! Hello, World!";

    cout << "Original text: " << originalText.c_str() << endl;
    cout << "Key: " << encryptor.getKey().c_str() << endl;

    // Шифрование
	UnicodeString encrypted = encryptor.encrypt(originalText);
	cout << "Encrypted text: " << encrypted.c_str() << endl;

    // Покажем hex представление для наглядности
	printHex(originalText, "Original");
	printHex(encrypted, "Encrypted");

	// Дешифрование
	UnicodeString decrypted = encryptor.decrypt(encrypted);
	cout << "Decrypted text: " << decrypted.c_str() << endl;

	// Проверка
	if (originalText == decrypted) {
        cout << "✓ Encryption/Decryption successful!" << endl;
	} else {
        cout << "✗ Encryption/Decryption failed!" << endl;
    }

    // Дополнительный пример с русским текстом
	cout << "\n=== Russian Text Example ===" << endl;

    UnicodeString russianText = L"Русский текст: привет, как дела?";
	UnicodeString encryptedRussian = encryptor.encrypt(russianText);
    UnicodeString decryptedRussian = encryptor.decrypt(encryptedRussian);

	cout << "Original: " << russianText.c_str() << endl;
	cout << "Encrypted: " << encryptedRussian.c_str() << endl;
	cout << "Decrypted: " << decryptedRussian.c_str() << endl;

	// Пример с автогенерацией ключа
	cout << "\n=== Auto-generated Key Example ===" << endl;
	UnicodeEncryptor autoEncryptor; // Автогенерация ключа
	UnicodeString testText = L"Test message for auto key";
	UnicodeString encryptedTest = autoEncryptor.encrypt(testText);
	UnicodeString decryptedTest = autoEncryptor.decrypt(encryptedTest);

	cout << "Auto key: " << autoEncryptor.getKey().c_str() << endl;
	cout << "Original: " << testText.c_str() << endl;
	cout << "Decrypted: " << decryptedTest.c_str() << endl;

	// Тест с пустой строкой
	cout << "\n=== Empty String Test ===" << endl;
	UnicodeString emptyText = L"";
	UnicodeString encryptedEmpty = encryptor.encrypt(emptyText);
	UnicodeString decryptedEmpty = encryptor.decrypt(encryptedEmpty);

	cout << "Empty original: '" << emptyText.c_str() << "'" << endl;
	cout << "Empty encrypted: '" << encryptedEmpty.c_str() << "'" << endl;
	cout << "Empty decrypted: '" << decryptedEmpty.c_str() << "'" << endl;

    return 0;
} */
