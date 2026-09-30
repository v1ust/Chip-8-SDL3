#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H
#include <fstream>
#include <vector>
#include <iostream>
#include <string.h>
#include <string>
#include <cstdint>
#include <stack>
// 00E0	Очистить экран
// 1NNN	Перейти по адресу NNN (записать NNN в PC)
// 6XNN	Записать NN в регистр VX
// 7XNN	Прибавить NN к VX (флаг переноса не трогать)
// ANNN	Записать NNN в регистр I
// DXYN	Нарисовать спрайт (подробности ниже)


class Chip8 {
private:
    uint8_t memory[4096] = {};
    uint16_t pc = 0x200;
    uint8_t V[16] = {};  // регистры V0–VF
    uint16_t I = 0;      // адресный регистр
    std::stack <uint16_t> stack = {};  // адреса возврата

public:
    uint8_t screen[64][32] = {};
    int loadFromFile(std::string path);
    void step();
    void printScreen();
};


#endif //CHIP8_CHIP8_H
