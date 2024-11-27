#include "VernmannThreadCoder.h"
#include "IDEA.h"
#include <iostream>
#include "fstream"
#include <string>

using namespace std;

void writeFile(string& data, string filename) {
        ofstream outfile(filename);

        // Проверяем, успешно ли файл открыт
        if (!outfile) {
            std::cerr << "Open file error: " << filename << std::endl;
            return;
        }

        outfile << data << std::endl;

        outfile.close();

        std::cout << "Success: " << filename << std::endl;
}

string readFromFile(const std::string& filename) {
    ifstream infile(filename); // Открываем файл для чтения
    if (!infile) {
        std::cerr << "Open file error: " << filename << std::endl;
        exit(1); // Возвращаем пустую строку в случае ошибки
    }

    std::string content;
    std::string line;

    // Считываем файл построчно и добавляем в строку
    while (std::getline(infile, line)) {
        content += line + '\n';
    }

    infile.close();
    return content;
}

string readInputFromConsole(string warningMessage) {
    std::string input;
    std::cout << warningMessage;
    std::getline(std::cin, input); // Считываем строку до нажатия Enter
    return input;
}

void threadCoderMode() {
    // Потоковое шифрование с помощью алгоритма "Шифр Вернама"
    auto thread_coder = VernamThreadCoder();

    readInputFromConsole("");


    // Кодирование
    string message = readInputFromConsole("Type message:");

    auto keystream = thread_coder.generateKeystream(message.size());
    string threadEncodedMessage = thread_coder.encode(message, keystream);
    cout << "Enecoded cipher: ";
    cout << threadEncodedMessage << endl;

    // Декодирование
    string threadDecodedMessage = thread_coder.decode(threadEncodedMessage, keystream);
    cout << "Decoded cipher: " << threadDecodedMessage << endl;
}

// Простая хэш-функция для тестирования лавинного эффекта
uint32_t simpleHash(const string& input) {
    uint32_t hash = 0xABCDEF01; // Начальное значение хэша

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        hash ^= (hash << 5) + (hash >> 2) + static_cast<uint32_t>(c); // Битовые операции с текущим символом
    }

    return hash;
}

void hashMode() {
    string input = readFromFile("hashInput.txt");
    cout << "hashInput data: \n";
    cout << input << endl;
    cout << "Hash: ";
    cout << simpleHash(input);
}

void blockCoderMode() {
    IDEA idea;

    // Чтение входных данных из файла
    ifstream fin("blockInput.txt");
    if (!fin) {
        cerr << "Cannot open input file: blockInput.txt" << endl;
        return;
    }

    byte key[16];
    byte plainText[8];

    // Считываем ключ
    for (int i = 0; i < 16; i++) {
        int value;
        fin >> hex >> value;
        key[i] = static_cast<byte>(value);
    }

    // Считываем открытый текст
    for (int i = 0; i < 8; i++) {
        int value;
        fin >> hex >> value;
        plainText[i] = static_cast<byte>(value);
    }
    fin.close();

    // Установка данных и выполнение шифрования/расшифрования
    idea.setKey(key);
    idea.setPlainText(plainText);
    idea.enc();

    // Запись результатов в файл
    idea.IDEATest("blockOutput.txt");

    cout << "Result saved in output.txt" << endl;
}

void run() {
    int choice;
    std::cout << "Type number \n1 : thread cipher, console input\n2 : block cipher, file input\n3: hash avalanch test mode\n";
    std::cin >> choice;

    switch (choice) {
    case 1:
        threadCoderMode();
        break;
    case 2:
        blockCoderMode();
        break;
    case 3:
        hashMode();
        break;
    default:
        std::cout << "Error input. Try again" << std::endl;
    }
}

int main()
{
    run();
    return 0;
}