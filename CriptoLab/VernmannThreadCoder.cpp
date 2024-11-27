#pragma once

#include "VernmannThreadCoder.h"


string VernamThreadCoder::encode(const string& message, const vector<uint8_t> keystream) {
    if (message.size() != keystream.size()) {
        throw invalid_argument("Size of message and keystream must be equal!");
    }

    string result(message.size(), '\0'); // Результирующая строка
    for (size_t i = 0; i < message.size(); ++i) {
        result[i] = message[i] ^ keystream[i]; // XOR для каждого символа
    }
    return result;
}

vector<uint8_t> VernamThreadCoder::generateKeystream(size_t length) {
    if (length == 0) {
        throw invalid_argument("Keystream size must be more then 0!");
    }

    // Вектор для хранения ключевого потока
    vector<uint8_t> keystream(length);

    // Используем генератор случайных чисел
    random_device rd;   // Источник случайных чисел
    mt19937 gen(rd());  // Генератор на основе Mersenne Twister
    uniform_int_distribution<int> dist(0, 255); // Диапазон байтов

    // Заполняем вектор случайными значениями
    for (size_t i = 0; i < length; ++i) {
        keystream[i] = dist(gen);
    }

    return keystream;
}