#include <string>

using std::string;

enum Opcode {
    add, nand,
    lw, sw, beq,
    jalr,
    halt, noop,
    fill, 
    non_support_opcode
};

Opcode encodeOpcode(string s){
    if(s == "add") return add;
    if(s == "nand") return nand;
    if(s == "lw") return lw;
    if(s == "sw") return sw;
    if(s == "beq") return beq;
    if(s == "jalr") return jalr;
    if(s == "halt") return halt;
    if(s == "noop") return noop;
    if(s == ".fill") return fill;
    return non_support_opcode;
}