#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <bitset>
#include <scn/scn.h>

using   std::string, 
        std::fstream,
        std::stringstream,
        std::vector,
        std::map , 
        std::pair,
        std::bitset,
        std::endl,
        std::getline,
        std::cout ;

enum Opcode {
    add, nand,
    lw, sw, beq,
    jalr,
    halt, noop,
    fill, 
    non_support_opcode
};

string IntegerToString2sComp(int Integer){  
    /* check Int input is over 16 bits */
    if (Integer < -32768 || Integer>32767)
		return "";
	string result;
    /* forloop convert */
	for (int bit = 0; bit < sizeof(int) * 4; ++bit)
	{
		int bit_val = 1 & Integer;
		result = (bit_val ? "1" : "0") + result;
		Integer = Integer >> 1;
	}
	return result;
}

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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <assembly-code-file> <machine-code-file>\n" ;
        exit(1);
    }
    fstream assembly, machine; 
    string line;
    string  label, opcode, 
            arg0, arg1, arg2;
    map<string, int> labelFill;
    auto errorMessage = [](fstream& fileStream, char *fileString) {
            if (!fileStream) {
                cout << "error in opening " <<  fileString << endl;
                exit(1);
            }
    };
    auto parse = [&](){
        auto res = scn::scan(line, "{:s} {:s}", label);
        if(encodeOpcode(label) == non_support_opcode){
            scn::scan_default(res, opcode, arg0, arg1, arg2);
        }else{
            label = "";
            scn::scan_default(line, opcode, arg0, arg1, arg2);
        }
        cout << label << " " << opcode << " " << arg0 << " " << arg1 << " " << arg2 << endl;
    };
    auto isNumber = [](const string& s){
        return s.find_first_not_of("-0123456789") == string::npos;
    };
    auto toInt = [](string num){
        return stoi(num, nullptr, 2);
    };
    auto r_type = [](const string& opcode, const auto& arg0, const auto& arg1, const auto& arg2 ){
        string regA = bitset<3>(stoi(arg0)).to_string();
        string regB = bitset<3>(stoi(arg1)).to_string();
        string regDest = bitset<3>(stoi(arg2)).to_string();
        return opcode + regA + regB + "0000000000000" + regDest;
    };
    auto s_type = [&](const string& opcode, const auto& arg0, const auto& arg1, const auto& arg2 ){
        string regA = bitset<3>(stoi(arg0)).to_string();
        string regB = bitset<3>(stoi(arg1)).to_string();
        string offset ;
        /*check if arg2 is string or number
            *else print error and check type of error */
        if(isNumber(arg2)){
            if((stoi(arg2)>>16) > 0){
                cout << "error : use over size offset\n";
                exit(1);
            }
            offset = bitset<16>(stoi(arg2)).to_string();
        }else{
            if(labelFill.find(arg2)==labelFill.end()){
                cout << "error : use undefine label\n";
                exit(1);
            }
            if((labelFill.find(arg2)->second>>16) > 0){
                cout << "error : use over size offset\n";
                exit(1);
            }
            offset = bitset<16>(labelFill.find(arg2)->second).to_string();
        }
        return opcode + regA + regB + offset;
    };
    auto beq_type = [&](const string& opcode,auto i){
        string regA = bitset<3>(stoi(arg0)).to_string();
        string regB = bitset<3>(stoi(arg1)).to_string();
        string offset ;
        /*check if arg2 is string or number
            *else print error and check type of error */
        if(isNumber(arg2)){
            if((stoi(arg2)>>16) > 0){
                cout << "error : use over size offset\n";
                exit(1);
            }
            offset = bitset<16>(stoi(arg2)).to_string();
        }else{
            if(labelFill.find(arg2)==labelFill.end()){
                cout << "error : use undefine label\n";
                exit(1);
            }
            if((labelFill.find(arg2)->second>>16) > 0){
                cout << "error : use over size offset\n";
                exit(1);
            }
            auto jump = labelFill.find(arg2)->second -i -1 ;
            offset = bitset<16>(IntegerToString2sComp(jump)).to_string();
        }
        return opcode + regA + regB + offset;
    };
    auto jalr_type = [](const string& opcode, const auto& arg0, const auto& arg1, const auto& arg2 ){
        string regA_j = bitset<3>(stoi(arg0)).to_string();
        string regB_j = bitset<3>(stoi(arg1)).to_string();
        return opcode + regA_j +regB_j + "0000000000000000";
    };
    auto n_type = [](const string& opcode){
        return opcode + "0000000000000000000000";
    };
    auto fill_op = [&](auto arg0){
        if(isNumber(arg0)){
            string::size_type sz;
            int labelValue = stoi(arg0,&sz);
            return labelValue ;
        }
        return labelFill.find(arg0)->second ;
    };

    assembly.open(argv[1], fstream::in);
    errorMessage(assembly, argv[1]);
    machine.open(argv[2], fstream::out);
    errorMessage(machine, argv[2]);

    int i = 0;
    while(getline(assembly, line)){
        parse();
        if(label.compare("")!=0){//if contain label
            if(labelFill.find(label)!=labelFill.end()){
                cout << "error : use same label\n";
                exit(1);
            }
            labelFill.insert(pair<string, int>(label,i));
        }
        label = ""; // clear label
        i++;
    }
    // machine.close();
    assembly.close();
    
    assembly.open(argv[1], fstream::in);
    i = 0;
    while(getline(assembly, line)){
        parse();
        switch(encodeOpcode(opcode)){
            case add: 
                machine << toInt(r_type("000", arg0, arg1, arg2)) << '\n'; 
                break;
            case nand: 
                machine << toInt(r_type("001", arg0, arg1, arg2)) << '\n'; 
                break;
            case lw: 
                machine << toInt(s_type("010", arg0, arg1, arg2)) << '\n'; 
                break;
            case sw: 
                machine << toInt(s_type("011", arg0, arg1, arg2)) << '\n'; 
                break;
            case beq: 
                machine << toInt(beq_type("100", i)) << '\n'; 
                break;
            case jalr: 
                machine << toInt(jalr_type("101", arg0, arg1, arg2)) << '\n'; 
                break;
            case halt: 
                machine << toInt(n_type("110")) << '\n'; 
                break;
            case noop: 
                machine << toInt(n_type("111")) << '\n'; 
                break;
            case fill: 
                machine << fill_op(arg0) << '\n'; 
                break;
            default: 
                cout << "error : use undefine opcode\n" << opcode;
                exit(1);
        }
        i++;
    }
    cout << "compile complete\n";
    machine.close();
    exit(0);
    return(0);
}
