#include "Chip8.h"
#include <string.h>
#include <errno.h>
#include <random>
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
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 255);
    int rnd = dis(gen);
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
            if (sk == 0x00E0) {
                for (int i = 0; i < 64; i++) {
                    for (int j = 0; j < 32; j++) {
                        screen[i][j] = 0;
                    }
                }
            }
            if (sk == 0x00EE) {
                if (!stack.empty()) {
                    pc = stack.top();
                    stack.pop();
                }
                else {
                    cerr<<"stack empty"<<endl;
                }
            }
        }
        break;
        case 0x1: {
            pc = NNN;
        }
        break;
        case 0x2: {
            if (stack.size() <16) {
                stack.push(pc);
                pc = NNN;
            }
            else {
                cerr<<"stack overflow"<<endl;
            }
        }
        break;
        case 0x3: {
            if (V[X] == NN) {
                pc += 2;
            }
        }
        break;
        case 0x4: {
            if (V[X]!= NN) {
                pc += 2;
            }
        }
        break;
        case 0x5: {
            if (V[X] == V[Y]) {
                pc += 2;
            }
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
        case 0x8: {
            if (N == 0) {
                V[X] = V[Y];
            }
            else if (N == 1) {
                V[X] = V[X] | V[Y];
            }
            else if (N == 2) {
                V[X] = V[X] & V[Y];
            }
            else if (N == 3) {
                V[X] = V[X] ^ V[Y];
            }
            else if (N == 4) {
                int tmpres = V[X] + V[Y];
                uint8_t tmpflag = (tmpres > 255);
                V[X] = tmpres;
                V[0xF] = tmpflag;
            }
            else if (N == 5) {
                int tmpres = V[X] - V[Y];
                uint8_t tmpflag = (V[X] >= V[Y]);
                V[X] = tmpres;
                V[0xF] = tmpflag;
            }
            else if (N == 6) {
                uint8_t tmp = (V[X] & 0x01);
                V[X] = V[X] >> 1;
                V[15] = tmp;
            }
            else if (N == 7) {
                int tmpres = V[Y] - V[X];
                uint8_t tmpflag = (V[Y] >= V[X]);
                V[X] = tmpres;
                V[0xF] = tmpflag;
            }
            else if (N == 0xE) {
                uint8_t tmp = (V[X] & 0x80) >> 7;
                V[X] = V[X] << 1;
                V[15] = tmp;
            }
        }
        break;
        case 0x9: {
            if (V[X] != V[Y]) {
                pc += 2;
            }
        }
        break;
        case 0xA: {
            I = NNN;
        }
        break;
        case 0xB: {
                pc = NNN + V[0];
        }
        break;
        case 0xC: {
            V[X] = rnd & NN;
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
        // case 0xE: {
        //
        // }
        break;
        case 0xF: {
            if (NN == 0x001e) {
                I = I + V[X];
            }
            else if (NN == 0x0033) {
                memory[I + 0] = V[X] / 100;
                memory[I + 1] = (V[X] / 10)%10;
                memory[I + 2] = V[X] % 10;
            }
            else if (NN == 0x0055) {
                for (int i = 0; i <= X; i++) {
                    memory[I + i] = V[i];
                }
            }
            else if (NN == 0x0065) {
                for (int i = 0; i <= X; i++) {
                    V[i] = memory[I+i];
                }
            }
        }
        break;
        default: cout << "Неизвестный опкод: " << hex << sk << endl;
            break;
    }
}

// void Chip8::printScreen() {
//     int i = 0;
//     for (int y = 0; y < 32; y++) {
//         for (int x = 0; x < 64; x++) {
//             std::cout << (screen[x][y] ? '#' : '.');
//         }
//         std::cout << '\n';
//     }
// }