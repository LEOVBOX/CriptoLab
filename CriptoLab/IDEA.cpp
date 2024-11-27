#include "IDEA.h"

// Установка открытого текста для шифрования
void IDEA::setPlainText(byte in[]) {
    // Преобразование массива байтов в массив 16-битных слов
    for (int i = 0; i < 8; i += 2) {
        plainText[i / 2] = (in[i] << 8) + in[i + 1];
    }
}

// Установка ключа шифрования
void IDEA::setKey(byte in[]) {
    // Сохранение 128-битного ключа
    for (int i = 0; i < 16; i++) {
        key[i] = in[i];
    }
    // Генерация раундовых ключей шифрования и дешифрования
    getEncRoundKey(encRoundKey);
    getDecRoundKey(encRoundKey, decRoundKey);
}

// Генерация раундовых ключей шифрования
void IDEA::getEncRoundKey(word16* encRoundKey) {
    int i, j;
    // Первые 8 раундовых ключей берутся непосредственно из исходного ключа
    for (i = 0, j = 0; j < 8; j++) {
        encRoundKey[j] = (key[i] << 8) + key[i + 1]; // Комбинация двух байтов в 16-битное слово
        i += 2;
    }
    // Оставшиеся 44 ключа генерируются путем циклического сдвига
    for (i = 0; j < 52; j++) {
        i++;
        encRoundKey[i + 7] = encRoundKey[i & 7] << 9 | encRoundKey[(i + 1) & 7] >> 7;
        encRoundKey += (i & 8);
        i &= 7;
    }
}

// Генерация раундовых ключей дешифрования на основе ключей шифрования
void IDEA::getDecRoundKey(word16* EK, word16 DK[]) {
    int i;
    word16 temp[52];
    word16 t1, t2, t3;
    word16* p = temp + 52;

    // Генерация первых четырех дешифровочных ключей
    t1 = invMul(*EK++);
    t2 = -*EK++;
    t3 = -*EK++;
    *--p = invMul(*EK++);
    *--p = t3;
    *--p = t2;
    *--p = t1;

    // Генерация ключей для остальных раундов
    for (i = 0; i < 7; i++) {
        t1 = *EK++;
        *--p = *EK++;
        *--p = t1;
        t1 = invMul(*EK++);
        t2 = -*EK++;
        t3 = -*EK++;
        *--p = invMul(*EK++);
        *--p = t2;
        *--p = t3;
        *--p = t1;
    }

    // Завершающий набор дешифровочных ключей
    t1 = *EK++;
    *--p = *EK++;
    *--p = t1;
    t1 = invMul(*EK++);
    t2 = -*EK++;
    t3 = -*EK++;
    *--p = invMul(*EK++);
    *--p = t3;
    *--p = t2;
    *--p = t1;

    // Копирование временного массива в выходной массив DK
    for (i = 0, p = temp; i < 52; i++) {
        *EK++ = *p;
        *p++ = 0;
    }
}

// Вычисление мультипликативной инверсии
word16 IDEA::invMul(word16 x) {
    word16 t0, t1;
    word16 q, y;

    // Если x = 0 или x = 1, возвращаем x
    if (x <= 1) {
        return x;
    }

    // Инициализация значений
    t1 = word16(0x10001L / x);
    y = word16(0x10001L % x);

    if (y == 1) {
        return (1 - t1) & 0xFFFF;
    }

    t0 = 1;
    do {
        // Алгоритм расширенного евклидова алгоритма
        q = x / y;
        x = x % y;
        t0 += q * t1;
        if (x == 1) {
            return t0;
        }
        q = y / x;
        y = y % x;
        t1 += q * t0;
    } while (y != 1);

    return (1 - t1) & 0xFFFF;
}

// Шифрование одного блока данных
void IDEA::encryption(word16 in[], word16 out[], word16* EK) {
    word16 x1, x2, x3, x4, t1, t2;

    // Инициализация входных данных
    x1 = in[0];
    x2 = in[1];
    x3 = in[2];
    x4 = in[3];

    // 8 основных раундов шифрования
    int r = 8;
    do {
        x1 = mul(x1, *EK++);     // Умножение по модулю
        x2 += *EK++;             // Сложение по модулю
        x3 += *EK++;
        x4 = mul(x4, *EK++);
        t2 = x1 ^ x3;            // XOR
        t1 = x2 ^ x4;
        t2 = mul(t2, *EK++);
        t1 = t1 + t2;
        t1 = mul(t1, *EK++);
        t2 = t1 + t2;
        x1 ^= t1;
        x4 ^= t2;
        t2 ^= x2;
        x2 = x3 ^ t1;
        x3 = t2;
    } while (--r);

    // Вывод зашифрованных данных
    x1 = mul(x1, *EK++);
    *out++ = x1;
    *out++ = x3 + *EK++;
    *out++ = x2 + *EK++;
    x4 = mul(x4, *EK++);
    *out = x4;
}

// Вызов процесса шифрования и дешифрования
void IDEA::enc() {
    // Шифрование
    encryption(plainText, cipherText, encRoundKey);
    // Дешифрование
    encryption(cipherText, deCipherText, decRoundKey);
}

// Умножение двух 16-битных чисел по модулю 2^16 + 1
word16 IDEA::mul(word16 x, word16 y) {
    word32 p = (word32)x * y;
    if (p) {
        y = p & 0xFFFF;
        x = p >> 16;
        return (y - x) + (y < x);
    }
    else if (x) {
        return 1 - y;
    }
    else {
        return 1 - x;
    }
}

// Тестирование алгоритма IDEA
void IDEA::IDEATest(const string& outputFile) {
    ofstream fout(outputFile);
    if (!fout) {
        cerr << "Не удалось открыть файл для записи: " << outputFile << endl;
        return;
    }

    // Вывод ключа
    fout << "The input key is:" << endl;
    for (int i = 0; i < 16; i++) {
        fout << hex << setw(2) << setfill('0') << int(key[i]) << " ";
    }
    fout << endl;

    // Вывод открытого текста
    fout << "The plain text is:" << endl;
    for (int i = 0; i < 4; i++) {
        fout << hex << setw(4) << setfill('0') << plainText[i] << " ";
    }
    fout << endl;

    // Вывод зашифрованного текста
    fout << "The cipher text is:" << endl;
    for (int i = 0; i < 4; i++) {
        fout << hex << setw(4) << setfill('0') << cipherText[i] << " ";
    }
    fout << endl;

    // Вывод расшифрованного текста
    fout << "The deciphered text is:" << endl;
    for (int i = 0; i < 4; i++) {
        fout << hex << setw(4) << setfill('0') << deCipherText[i] << " ";
    }
    fout << endl;

    fout.close();
}
