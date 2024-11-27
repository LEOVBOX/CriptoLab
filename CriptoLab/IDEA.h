#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

typedef unsigned char byte;
typedef unsigned short word16; // 16 бит
typedef unsigned long word32;  // 32 бит

// Класс для реализации шифрования алгоритмом IDEA
class IDEA {
public:
    // Установка ключа шифрования (128-битный ключ)
    void setKey(byte in[]);

    // Установка открытого текста для шифрования (64-битный блок)
    void setPlainText(byte in[]);

    // Вычисление мультипликативной инверсии числа по модулю 2^16 + 1
    word16 invMul(word16 x);

    // Умножение двух чисел по модулю 2^16 + 1
    word16 mul(word16 x, word16 y);

    // Шифрование 64-битного блока
    void encryption(word16 in[], word16 out[], word16* Ek);

    // Выполнение процесса шифрования и дешифрования
    void enc();

    // Тестирование работы алгоритма IDEA с выводом результатов в файл
    void IDEATest(const string& outputFile);

private:
    // Генерация ключей для раундов шифрования
    void getEncRoundKey(word16* encRoundKey);

    // Генерация ключей для раундов дешифрования
    void getDecRoundKey(word16* EK, word16 DK[]);

    // 128-битный ключ шифрования
    byte key[16];

    // Буфер для хранения зашифрованного текста (64 бита)
    word16 cipherText[4];

    // Буфер для хранения открытого текста (64 бита)
    word16 plainText[4];

    // Буфер для хранения расшифрованного текста (64 бита)
    word16 deCipherText[4];

    // Ключи для 52 раундов шифрования
    word16 encRoundKey[52];

    // Ключи для 52 раундов дешифрования
    word16 decRoundKey[52];
};
