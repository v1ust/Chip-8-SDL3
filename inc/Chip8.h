#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H
#include <fstream>
#include <vector>
#include <iostream>
#include <string.h>
#include <string>
#include <cstdint>
#include <stack>

// 1 2 3 C        1 2 3 4
// 4 5 6 D   →    Q W E R
// 7 8 9 E        A S D F
// A 0 B F        Z X C V


// Опкод	Что делает
// EX9E	Пропустить следующую инструкцию, если клавиша с номером V[X] нажата
// EXA1	Пропустить следующую инструкцию, если клавиша с номером V[X] не нажата
// FX0A	Ждать нажатия любой клавиши, номер записать в V[X]
// FX07	V[X] = значение таймера задержки
// FX15	таймер задержки = V[X]
// FX18	таймер звука = V[X]

class Chip8 {
private:
    uint8_t memory[4096] = {};
    uint16_t pc = 0x200;
    uint8_t V[16] = {};  // регистры V0–VF
    uint16_t I = 0;      // адресный регистр
    std::stack <uint16_t> stack = {};  // адреса возврата
    bool keys[16] = {};
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;

public:
    uint8_t screen[64][32] = {};
    int loadFromFile(std::string path);
    void step();
    void setKey(uint8_t key, bool pressed);
    void tickTimers();
};


#endif //CHIP8_CHIP8_H
