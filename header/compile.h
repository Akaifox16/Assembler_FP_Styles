#include <bitset>
#include <map>
#include <string>

#define REG_LENGTH 3
#define OFFSET_LENGTH 16

using   std::bitset,
        std::string,
        std::map ;

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

string r_type(const string& opcode, const string& arg0, const string& arg1, const string& arg2 ){
    string regA = int2bin(stoi(arg0), REG_LENGTH);
    string regB = int2bin(stoi(arg1), REG_LENGTH);
    string regDest = int2bin(stoi(arg2), REG_LENGTH);
    return opcode + regA + regB + "0000000000000" + regDest;
}

auto s_type(const string& opcode, const string& arg0, const string& arg1, const string& arg2, const map<string, int>& labelFill ){
    string regA = int2bin(stoi(arg0), REG_LENGTH);
    string regB = int2bin(stoi(arg1), REG_LENGTH);
    string offset = (isNumber(arg2))
                    ? int2bin(stoi(arg2), OFFSET_LENGTH)
                    : int2bin(labelFill.find(arg2)->second, OFFSET_LENGTH);
    // if(isNumber(arg2)){
    //     if((stoi(arg2)>>16) > 0){
    //         cout << "error : use over size offset\n";
    //         exit(1);
    //     }
    // }else{
    //     if(labelFill.find(arg2)==labelFill.end()){
    //         cout << "error : use undefine label\n";
    //         exit(1);
    //     }
    //     if((labelFill.find(arg2)->second>>16) > 0){
    //         cout << "error : use over size offset\n";
    //         exit(1);
    //     }
    // }
    return opcode + regA + regB + offset;
}

auto beq_type(const string& opcode, const string& arg0, const string& arg1, const string& arg2, const map<string, int>& labelFill, const int i){
    string regA = int2bin(stoi(arg0), REG_LENGTH);
    string regB = int2bin(stoi(arg1), REG_LENGTH);
    string offset = (isNumber(arg2))
                    ? int2bin(stoi(arg2), OFFSET_LENGTH)
                    : int2bin(labelFill.find(arg2)->second -i -1, OFFSET_LENGTH);
    /*check if arg2 is string or number
        *else print error and check type of error */
    // if(isNumber(arg2)){
    //     if((stoi(arg2)>>16) > 0){
    //         cout << "error : use over size offset\n";
    //         exit(1);
    //     }
    //     offset = bitset<16>(stoi(arg2)).to_string();
    // }else{
    //     if(labelFill.find(arg2)==labelFill.end()){
    //         cout << "error : use undefine label\n";
    //         exit(1);
    //     }
    //     if((labelFill.find(arg2)->second>>16) > 0){
    //         cout << "error : use over size offset\n";
    //         exit(1);
    //     }
    //     auto jump = labelFill.find(arg2)->second -i -1 ;
    //     offset = bitset<16>(IntegerToString2sComp(jump)).to_string();
    // }
    return opcode + regA + regB + offset;
}

auto jalr_type(const string& opcode, const string& arg0, const string& arg1){
    string regA = int2bin(stoi(arg0), REG_LENGTH);
    string regB = int2bin(stoi(arg1), REG_LENGTH);
    return opcode + regA +regB + "0000000000000000";
}

auto n_type(const string& opcode){
    return opcode + "0000000000000000000000";
}