#include "Chip8.h"
#include <string.h>
#include <errno.h>
#include <SDL3/SDL_render.h>
using namespace std;

int Chip8::loadFromFile(string path)
{
    cout<<"Chip8::loadFromFile"<<path<<endl;

    ifstream in(path, std::ios::binary);

    if (!(in.is_open()))
    {

        return -1;
    }
    char c;
    uint16_t i = 0x200;
    while (in.get(c) && (int)i < 4096)
    {
        memory[i++] = (uint8_t)c;
    }
    if ((int)i == 4096) {
        if (in.get(c)) {
            cerr << "В файле остались данные массив заполнен" << " "<<strerror(errno) << path <<endl;
            return -1;
        }
        else cout << "Файл полностью записан"<<path<<endl;
    }
    return 0;
}

void Chip8::step() {
    cout<<"Chip8::step"<<endl;
    uint16_t sk;
    uint16_t type,X,Y,N = 0;
    uint16_t NNN,NN = 0;
    sk = ((uint16_t)(this->memory[this->pc] << 8) | this->memory[this->pc + 1]);
    type = (sk & 0xf000)>>12;
    X = (sk & 0x0f00)>>8;
    Y = (sk & 0x00f0)>>4;
    N = (sk & 0x000f);
    NN = (sk & 0x00ff);
    NNN = (sk & 0x0fff);
    pc += 2;
    switch (type) {
        case 0x0: {
            for (int i = 0; i < 64; i++) {
                for (int j = 0; j < 32; j++) {
                    screen[i][j] = 0;
                }
            }
        }
        break;
        case 0x1: {
            pc = NNN;
        }
        break;
        case 0x6: {
            V[X] = NN;
        }
        break;
        case 0x7: {
            V[X] += NN;
        }
        break;
        case 0xA: {
            I = NNN;
        }
        break;
        case 0xD: {
            // DXYN	Нарисовать спрайт высотой N строк из памяти по адресу I в точке (V[X], V[Y])
            uint16_t sx = 0, sy = 0;
            uint8_t tmp = 0;
            uint8_t tmp2 = 0;
            int px = 0;
            int py = 0;
            V[0xF] = 0;
            sx = V[X] % 64;
            sy = V[Y] % 32;
            for (int i = 0; i < N; i++) {
                tmp = memory[I + i];
                for (int j = 0; j < 8; j++) {
                    if (tmp & (0x80 >> j)) {
                        px = sx + j;
                        py = sy + i;
                        if (px < 64 && py < 32) {
                            if (screen[px][py] == 1) {
                                V[0xF] = 1;
                            }
                            screen[px][py] ^= 1;
                        }
                    }
                }
            }

        }
        break;
        default: cout << "Неизвестный опкод: " << hex << sk << endl;
            break;
    }
    cout << "opcode = " << hex << sk << endl;
}

void Chip8::printScreen() {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            std::cout << (screen[x][y] ? '#' : '.');
        }
        std::cout << '\n';
    }
}