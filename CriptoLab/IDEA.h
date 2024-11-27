#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

typedef unsigned char byte;
typedef unsigned short word16; // 16 ���
typedef unsigned long word32;  // 32 ���

// ����� ��� ���������� ���������� ���������� IDEA
class IDEA {
public:
    // ��������� ����� ���������� (128-������ ����)
    void setKey(byte in[]);

    // ��������� ��������� ������ ��� ���������� (64-������ ����)
    void setPlainText(byte in[]);

    // ���������� ����������������� �������� ����� �� ������ 2^16 + 1
    word16 invMul(word16 x);

    // ��������� ���� ����� �� ������ 2^16 + 1
    word16 mul(word16 x, word16 y);

    // ���������� 64-������� �����
    void encryption(word16 in[], word16 out[], word16* Ek);

    // ���������� �������� ���������� � ������������
    void enc();

    // ������������ ������ ��������� IDEA � ������� ����������� � ����
    void IDEATest(const string& outputFile);

private:
    // ��������� ������ ��� ������� ����������
    void getEncRoundKey(word16* encRoundKey);

    // ��������� ������ ��� ������� ������������
    void getDecRoundKey(word16* EK, word16 DK[]);

    // 128-������ ���� ����������
    byte key[16];

    // ����� ��� �������� �������������� ������ (64 ����)
    word16 cipherText[4];

    // ����� ��� �������� ��������� ������ (64 ����)
    word16 plainText[4];

    // ����� ��� �������� ��������������� ������ (64 ����)
    word16 deCipherText[4];

    // ����� ��� 52 ������� ����������
    word16 encRoundKey[52];

    // ����� ��� 52 ������� ������������
    word16 decRoundKey[52];
};
