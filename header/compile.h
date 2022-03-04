#include <bitset>
#include <map>
#include <string>
#include <vector>

#define REG_LENGTH 3
#define OFFSET_LENGTH 16

using   std::bitset,
        std::string,
        std::vector,
        std::map,
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

string op_a_b(const int& opcode, const string& arg0, const string& arg1){
    return int2bin(opcode, REG_LENGTH)
            + int2bin(stoi(arg0), REG_LENGTH)
            + int2bin(stoi(arg1), REG_LENGTH) ;
}

int r_type(const int& opcode, const string& arg0, const string& arg1, const string& arg2 ){
    auto op_reg = op_a_b(opcode, arg0, arg1);
    string regDest = int2bin(stoi(arg2), REG_LENGTH);
    return toInt(op_reg + "0000000000000" + regDest);
}

auto s_type(const int& opcode, const string& arg0, const string& arg1, const int& arg2){
    auto op_reg = op_a_b(opcode, arg0, arg1);
    string offset = int2bin(arg2, OFFSET_LENGTH);
    return toInt(op_reg + offset);
}

auto jalr_type(const int& opcode, const string& arg0, const string& arg1){
    auto op_reg = op_a_b(opcode, arg0, arg1);
    return toInt(op_reg + "0000000000000000");
}

auto n_type(const int& opcode){
    string op = int2bin(opcode, REG_LENGTH);
    return toInt(op + "0000000000000000000000");
}