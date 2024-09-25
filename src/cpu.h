#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
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

    OPCODE_ADDR = 0x10,
    OPCODE_SUBR = 0x11,

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

    // float registers
    float regf[3] = {0};

    // ram or memory
    uint8_t mem[16384] = {0};

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
        {"addr", OPCODE_ADDR},
        {"subr", OPCODE_SUBR},
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

        if (components.size() < 2)
        {
            std::cerr << "Invalid command format" << std::endl;
            return -1; // Error code for invalid command format
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
                    // Assuming register names are a8, b8, c8, d8, etc.
                    mem[sc++] = components[i][0] - 'a'; // Store register index (0 for a, 1 for b, etc.)
                }
                else
                {
                    try
                    {
                        mem[sc++] = std::stoi(components[i]); // Convert immediate value to integer and store in memory
                    }
                    catch (const std::invalid_argument &e)
                    {
                        std::cerr << "Invalid immediate value: " << components[i] << std::endl;
                        return -1; // Error code for invalid immediate value
                    }
                }
            }
        }
        else
        {
            std::cerr << "Invalid command: " << components[0] << std::endl;
            return -1; // Error code for invalid command
        }

        return 0; // Return appropriate value based on your logic
    }

    // Execute the instruction pointed by the program counter
    int execute()
    {
        int op = static_cast<int>(mem[pc++]); // Read the last opcode

        switch (op)
        {
        case OPCODE_ADDI:
        {                                      // Add immediate
            uint16_t regIndex1 = static_cast<int>(mem[pc++]);         // First register
            uint16_t regIndex2 = static_cast<int>(mem[pc++]);     // Second register
            int immediateValue = static_cast<int>(mem[pc++]); // Immediate value
            reg8[regIndex1] = reg8[regIndex2] + immediateValue; // Add immediate value to register
            break;
        }
        case OPCODE_SUBB:
        {                                      // Subtract immediate
            int regIndex1 = mem[pc++];         // First register index
            int immediateValue = mem[pc++];    // Immediate value
            reg8[regIndex1] -= immediateValue; // Subtract immediate value from register
            std::cout << std::endl <<"Executed SUBB: reg8[" << regIndex1 << "] = " << static_cast<int>(reg8[regIndex1]) << std::endl;
            break;
        }
        // Handle other opcodes...
        default:
            std::cerr << "Unknown opcode: " << std::hex << op << std::endl;
            return -1;
        }

        return op; // Execution successful
    }
    const char *dump(int line)
    {
        static char buffer[50]; // Static buffer to hold the result

        std::stringstream ss;

        // Ensure we don't go out of bounds
        for (int i = 0; i < 4; ++i)
        {
            if (line + i < 16384)
            { // Check if within bounds
                ss << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(mem[line + i]) << " ";
            }
            else
            {
                ss << "0x-- "; // Indicate out of bounds
            }
        }

        // Copy the resulting string to the buffer
        std::strncpy(buffer, ss.str().c_str(), sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

        return buffer; // Return pointer to the buffer
    }

    int getREG8(int index)
    {
        return reg8[index];
    }
};