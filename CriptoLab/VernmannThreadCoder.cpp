#pragma once

#include "VernmannThreadCoder.h"


string VernamThreadCoder::encode(const string& message, const vector<uint8_t> keystream) {
    if (message.size() != keystream.size()) {
        throw invalid_argument("Size of message and keystream must be equal!");
    }

    string result(message.size(), '\0'); // �������������� ������
    for (size_t i = 0; i < message.size(); ++i) {
        result[i] = message[i] ^ keystream[i]; // XOR ��� ������� �������
    }
    return result;
}

vector<uint8_t> VernamThreadCoder::generateKeystream(size_t length) {
    if (length == 0) {
        throw invalid_argument("Keystream size must be more then 0!");
    }

    // ������ ��� �������� ��������� ������
    vector<uint8_t> keystream(length);

    // ���������� ��������� ��������� �����
    random_device rd;   // �������� ��������� �����
    mt19937 gen(rd());  // ��������� �� ������ Mersenne Twister
    uniform_int_distribution<int> dist(0, 255); // �������� ������

    // ��������� ������ ���������� ����������
    for (size_t i = 0; i < length; ++i) {
        keystream[i] = dist(gen);
    }

    return keystream;
}