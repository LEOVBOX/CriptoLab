#pragma once

#include <iostream>
#include <random>
#include <string>

using namespace std;

class VernamThreadCoder {
public:
    string encode(const string& message, const vector<uint8_t> keystream);

    string decode(const string& code, const vector<uint8_t> keystream) {
        return encode(code, keystream);
    };

    vector<uint8_t> generateKeystream(size_t length);
};