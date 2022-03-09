#include <bitset>
#include <map>
#include <string>
#include <vector>
#include "error.h"

#define REG_LENGTH 3
#define OFFSET_LENGTH 16

using   std::string, std::bitset, std::vector, std::map,
        std::stoi ;

bool isNumber(const string& s){
    return s.find_first_not_of("-0123456789") == string::npos;
}

int toInt(string num){
    return stoi(num, nullptr, 2);
}

string int2bin(int n, int bit){
    return  bit == REG_LENGTH 
            ? bitset<REG_LENGTH>(n).to_string()
            : bitset<OFFSET_LENGTH>(n).to_string();
}

auto op2mach(const int& opcode, const string& arg0, const string& arg1, const string& arg2){
    return toInt(int2bin(opcode, REG_LENGTH)
            + int2bin(stoi(arg0), REG_LENGTH)
            + int2bin(stoi(arg1), REG_LENGTH)
            + arg2) ;
}

auto r_type(const int& opcode, const string& arg0, const string& arg1, const string& arg2 ){
    int reg = stoi(arg2);
    detectErrorOversizeOffset(reg);
    return op2mach(opcode, arg0, arg1, "0000000000000" + int2bin(reg, REG_LENGTH));
}

auto s_type(const int& opcode, const string& arg0, const string& arg1, const int& arg2){
    detectErrorOversizeOffset(arg2);
    return op2mach(opcode, arg0, arg1, int2bin(arg2, OFFSET_LENGTH));
}

auto jalr_type(const int& opcode, const string& arg0, const string& arg1){
    return op2mach(opcode, arg0, arg1, int2bin(0, OFFSET_LENGTH));
}

auto n_type(const int& opcode){
    return op2mach(opcode, string("0"), string("0"), int2bin(0, OFFSET_LENGTH));
}