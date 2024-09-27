#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

enum opcode
{
    OPCODE_A8 = 0x00,
    OPCODE_B8 = 0x01,
    OPCODE_C8 = 0x02,
    OPCODE_D8 = 0x03,

    OPCODE_A16 = 0x04,
    OPCODE_B16 = 0x05,

    OPCODE_AF = 0x06,
    OPCODE_BF = 0x07,
    OPCODE_CF = 0x08,

    OPCODE_ADDI = 0x09,
    OPCODE_SUBB = 0x0A,
    OPCODE_LOAD = 0x0B,
    OPCODE_STORE = 0x0C,
    OPCODE_PUSH = 0x0D,
    OPCODE_POP = 0x0E,
    OPCODE_CLEAR = 0x0F,

    OPCODE_ADDF = 0x10,
    OPCODE_SUBF = 0x11,

    OP_HALT = 0xF0,
    OPCODE_NULL = 0xFF,
};

class CPU
{
private:
    // registers
    uint8_t reg8[4] = {0};

    // 16 bit registers
    uint16_t reg16[2] = {0};
    uint16_t pc = 0;
    uint16_t sc = 0;
    uint16_t sp = 0;

    // float registers
    float regf[3] = {0};

    // ram or memory
    uint8_t mem[16384] = {0};
    uint8_t stack[256] = {0};

    std::unordered_map<std::string, opcode> command_to_reg = {
        {"a8", OPCODE_A8},
        {"b8", OPCODE_B8},
        {"c8", OPCODE_C8},
        {"d8", OPCODE_D8},
        {"a16", OPCODE_A16},
        {"b16", OPCODE_B16},
        {"af", OPCODE_AF},
        {"bf", OPCODE_BF},
        {"cf", OPCODE_CF},
        {"addi", OPCODE_ADDI},
        {"subb", OPCODE_SUBB},
        {"load", OPCODE_LOAD},
        {"store", OPCODE_STORE},
        {"push", OPCODE_PUSH},
        {"pop", OPCODE_POP},
        {"clear", OPCODE_CLEAR},
        {"addf", OPCODE_ADDF},
        {"subf", OPCODE_SUBF},
        {"halt", OP_HALT},
        {"null", OPCODE_NULL},
    };

public:
    int getMemSize()
    {
        return sizeof(mem);
    }

    int load(std::string program)
    {
        std::stringstream ss(program);
        std::string item;
        std::vector<std::string> components;

        while (ss >> item)
        {
            components.push_back(item);
        }

        auto it = command_to_reg.find(components[0]);
        if (it != command_to_reg.end())
        {
            opcode op = it->second;
            mem[sc++] = op; // Store the opcode in memory

            for (size_t i = 1; i < components.size(); ++i)
            {
                if (components[i][0] == 'a' || components[i][0] == 'b' || components[i][0] == 'c' || components[i][0] == 'd')
                {
                    mem[sc++] = components[i][0] - 'a';
                }
                else
                {
                    mem[sc++] = std::stoi(components[i]);
                }
            }

            // Pad the remaining memory slots with zeros if the instruction has fewer than 4 components
            while (sc % 4 != 0)
            {
                mem[sc++] = 0;
            }
        }

        return 0; // Return appropriate value
    }

    // Execute the instruction pointed by the program counter
    int execute()
    {
        int op = static_cast<int>(mem[pc++]); // Read the last opcode

        switch (op)
        {
        case OPCODE_ADDI:
        {
            uint16_t regIndex1 = static_cast<int>(mem[pc++]);
            uint16_t regIndex2 = static_cast<int>(mem[pc++]);
            uint16_t immediateValue = static_cast<int>(mem[pc++]);
            reg8[regIndex1] = reg8[regIndex2] + immediateValue;
            break;
        }
        case OPCODE_SUBB:
        {
            uint16_t regIndex1 = static_cast<int>(mem[pc++]);
            uint16_t regIndex2 = static_cast<int>(mem[pc++]);
            uint16_t immediateValue = static_cast<int>(mem[pc++]);
            reg8[regIndex1] = reg8[regIndex2] - immediateValue;
            break;
        }
        case OP_HALT:
            return 1; // Halt the CPU
            break;
        case OPCODE_LOAD:
        {
            uint16_t regIndex = static_cast<int>(mem[pc++]);
            uint16_t imidiate = static_cast<int>(mem[pc++]);
            pc++;
            reg8[regIndex] = imidiate;
            break;
        }
        case OPCODE_STORE:
        {
            uint16_t regIndex = static_cast<int>(mem[pc++]);
            uint16_t imidiate = static_cast<int>(mem[pc++]);
            pc++;
            mem[imidiate] = reg8[regIndex];
            break;
        }
        case OPCODE_PUSH:
        {
            uint16_t regIndex = static_cast<int>(mem[pc++]); // Register index
            pc += 2;
            stack[sp++] = reg8[regIndex];
            break;
        }
        case OPCODE_POP:
        {
            uint16_t regIndex = static_cast<int>(mem[pc++]); // Register index
            pc += 2;
            sp--;
            reg8[regIndex] = stack[sp];
            break;
        }
        case OPCODE_ADDF:
        {
            int regIndex1 = static_cast<float>(mem[pc++]);
            int regIndex2 = static_cast<float>(mem[pc++]);
            float immediateValue = static_cast<float>(mem[pc++]);
            regf[regIndex1] = regf[regIndex2] + immediateValue;
            break;
        }
        default:
            std::cerr << "Unknown opcode: " << std::hex << op << std::endl;
            return -1;
        }

        return 0; // Continue execution
    }

    void run()
    {
        while (1 != execute())
        {
        }
    }
    const char *dump(int line)
    {
        bool first = true; // Static variable to indicate first
        static char buffer[50];

        std::stringstream ss;

        for (int i = 0; i < 4; ++i)
        {
            if (first) // Print memory address only once
            {
                ss << std::setw(2) << std::setfill('0') << std::hex << line + i << ": "; // Print memory address
                first = false;
            }

            if (line + i < 16384)
            {
                ss << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(mem[line + i]) << " ";
            }
            else
            {
                ss << "0x-- "; // Indicate out of bounds
            }
        }

        strncpy(buffer, ss.str().c_str(), sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0'; // null-termination

        return buffer; // Return pointer to the buffer
    }

    int getREG8(int index)
    {
        return reg8[index];
    }

    int getREG16(int index)
    {
        return reg16[index];
    }

    float getREGF(int index)
    {
        return regf[index];
    }

    const char *getStack(int line)
    {
        bool first = true;
        static char buffer[50];

        std::stringstream ss;

        for (int i = 0; i < 4; ++i)
        {
            if (first)
            {
                ss << std::setw(2) << std::setfill('0') << std::hex << line + i << ": "; // Print memory address
                first = false;
            }

            if (line + i < 256)
            {
                ss << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(stack[line + i]) << " ";
            }
            else
            {
                ss << "0x-- "; // Indicate out of bounds
            }
        }

        strncpy(buffer, ss.str().c_str(), sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

        return buffer; // Return pointer to the buffer
    }

    uint16_t getPC()
    {
        return pc;
    }

    uint16_t getSC()
    {
        return sc;
    }

    uint16_t getSP()
    {
        return sp;
    }
};