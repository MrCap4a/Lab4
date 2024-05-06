#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

// Базовый класс для работы с файлами
class CFile {
protected:
    fstream file;

public:
    // Открытие файла
    bool Open(const string& filename, ios_base::openmode mode) {
        file.open(filename, mode);
        return file.is_open();
    }

    // Закрытие файла
    void Close() {
        file.close();
    }

    // Установка позиции в файле
    void Seek(int pos) {
        file.seekg(pos);
    }

    // Чтение данных из файла
    bool Read(char* buffer, int size) {
        file.read(buffer, size);
        return file.gcount() == size;
    }

    // Запись данных в файл
    bool Write(const char* buffer, int size) {
        file.write(buffer, size);
        return file.good();
    }

    // Получение текущей позиции в файле
    int GetPosition() {
        return file.tellg();
    }

    // Получение длины файла
    int GetLength() {
        int currentPos = file.tellg();
        file.seekg(0, ios::end);
        int length = file.tellg();
        file.seekg(currentPos);
        return length;
    }
};

// Производный класс для работы с файлами определенного типа данных
class CMyDataFile : public CFile {
public:
    struct MyData {
        int value;
        char name[20];
    };

    struct Header {
        int count;
        int dataSize;
    };

    // Чтение данных MyData из файла
    bool ReadData(MyData& data) {
        return Read(reinterpret_cast<char*>(&data), sizeof(MyData));
    }

    // Запись данных MyData в файл
    bool WriteData(const MyData& data) {
        return Write(reinterpret_cast<const char*>(&data), sizeof(MyData));
    }

    // Чтение заголовка файла
    bool ReadHeader(Header& header) {
        return Read(reinterpret_cast<char*>(&header), sizeof(Header));
    }

    // Запись заголовка в файл
    bool WriteHeader(const Header& header) {
        return Write(reinterpret_cast<const char*>(&header), sizeof(Header));
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);

    CMyDataFile myFile;
    string filename = "data.bin";
    int choice;

    do {
        cout << "Выберите операцию:" << endl;
        cout << "1. Открыть файл" << endl;
        cout << "2. Закрыть файл" << endl;
        cout << "3. Записать данные в файл" << endl;
        cout << "4. Прочитать данные из файла" << endl;
        cout << "5. Получить текущую позицию в файле" << endl;
        cout << "6. Получить длину файла" << endl;
        cout << "7. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                ios_base::openmode mode;
                cout << "Выберите режим открытия (1 - чтение, 2 - запись, 3 - добавление): ";
                int modeChoice;
                cin >> modeChoice;
                switch (modeChoice) {
                    case 1:
                        mode = ios::in;
                        break;
                    case 2:
                        mode = ios::out | ios::trunc;
                        break;
                    case 3:
                        mode = ios::out | ios::app;
                        break;
                    default:
                        cout << "Неверный режим открытия файла." << endl;
                        break;
                }
                if (myFile.Open(filename, mode)) {
                    cout << "Файл успешно открыт." << endl;
                } else {
                    cout << "Ошибка при открытии файла." << endl;
                }
                break;
            }
            case 2:
                myFile.Close();
                cout << "Файл закрыт." << endl;
                break;
            case 3: {
                CMyDataFile::MyData data;
                cout << "Введите значение: ";
                cin >> data.value;
                cout << "Введите имя: ";
                cin >> data.name;
                if (myFile.WriteData(data)) {
                    cout << "Данные успешно записаны в файл." << endl;
                } else {
                    cout << "Ошибка при записи данных в файл." << endl;
                }
                break;
            }
            case 4: {
                CMyDataFile::MyData data;
                if (myFile.ReadData(data)) {
                    cout << "Данные успешно прочитаны из файла." << endl;
                    cout << "Значение: " << data.value << endl;
                    cout << "Имя: " << data.name << endl;
                } else {
                    cout << "Ошибка при чтении данных из файла." << endl;
                }
                break;
            }
            case 5:
                cout << "Текущая позиция в файле: " << myFile.GetPosition() << endl;
                break;
            case 6:
                cout << "Длина файла: " << myFile.GetLength() << " байт" << endl;
                break;
            case 7:
                cout << "Выход из программы." << endl;
                break;
            default:
                cout << "Неверный выбор операции." << endl;
                break;
        }
    } while (choice != 7);

    return 0;
}