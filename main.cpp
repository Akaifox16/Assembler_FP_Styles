#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <scn/scn.h>
#include "header/opcode.h"
#include "header/compile.h"
#include "header/parser.h"

using   std::string, 
        std::fstream,
        std::stringstream,
        std::vector,
        std::map , 
        std::pair,
        std::function,
        std::endl,
        std::getline,
        std::cout ;

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
    
    assembly.close(); 
    assembly.open(argv[1], fstream::in);
    
    auto add_op = [&](){
        return toInt(r_type("000", arg0, arg1, arg2));
    };
    auto nand_op = [&](){
        return toInt(r_type("001", arg0, arg1, arg2));
    };
    auto fill_op = [&](auto arg0){
        if(isNumber(arg0)){
            string::size_type sz;
            int labelValue = stoi(arg0,&sz);
            return labelValue ;
        }
        return labelFill.find(arg0)->second ;
    };

    i = 0;
    while(getline(assembly, line)){
        parse();
        switch(encodeOpcode(opcode)){
            case add: 
                machine << add_op() << '\n'; 
                break;
            case nand: 
                machine << nand_op() << '\n'; 
                break;
            case lw: 
                machine << toInt(s_type("010", arg0, arg1, arg2, labelFill)) << '\n'; 
                break;
            case sw: 
                machine << toInt(s_type("011", arg0, arg1, arg2, labelFill)) << '\n'; 
                break;
            case beq: 
                machine << toInt(beq_type("100",arg0, arg1, arg2, labelFill, i)) << '\n'; 
                break;
            case jalr: 
                machine << toInt(jalr_type("101", arg0, arg1)) << '\n'; 
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
