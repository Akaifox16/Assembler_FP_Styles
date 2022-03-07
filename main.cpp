#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <scn/scn.h>
#include "header/opcode.h"
#include "header/compile.h"
#include "header/error.h"

using   std::string, std::stringstream,
        std::fstream,
        std::vector, std::map, std::pair,
        std::getline, std::cout, std::endl;

int main(int argc, char *argv[]) {
    fstream assembly, machine; 
    string line, label, opcode, arg0, arg1, arg2;
    map<string, int> labelFill;
    
    auto parse = [&](){
        auto res = scn::scan(line, "{:s} {:s}", label);
        if(encodeOpcode(label) == non_support_opcode){
            scn::scan_default(res, opcode, arg0, arg1, arg2);
        }else{
            label = "";
            scn::scan_default(line, opcode, arg0, arg1, arg2);
        }
    };

    auto openFileStream = [&](fstream& file, auto arg, auto mode){
        file.open(arg, mode);
        detectErrorFileOpening( file, arg );
    };

    auto readFile = [&](auto callback){
        for(int i= 0; getline(assembly, line); i++){
            parse();
            callback(i);
        }
    };

    auto map = [&](int i){        
        auto label_finder = [&](auto& arg, bool isBeq=false){
            if(isNumber(arg)){
                return stoi(arg);
            }
            return isBeq 
                    ? labelFill.find(arg)->second -i -1
                    : labelFill.find(arg)->second;
        };

        auto op = encodeOpcode(opcode);
        switch(op){
            case add: 
            case nand: return r_type(op, arg0, arg1, arg2); 
            case lw: 
            case sw: return s_type(op, arg0, arg1, label_finder(arg2)); 
            case beq: return s_type(op,arg0, arg1, label_finder(arg2, true)); 
            case jalr: return jalr_type(op, arg0, arg1); 
            case halt: 
            case noop: return n_type(op); 
            case fill: return label_finder(arg0); 
            default: 
                error(true, [](){
                    cout << "error: use undefined opcode !\n";
                });
        }

        return 0;
    };
    
    error( argc != 3, [&](){
        cout << "error: usage: " << argv[0] << " <assembly-code-file> <machine-code-file>\n" ;
    });

    openFileStream(assembly, argv[1], fstream::in);
    openFileStream(machine, argv[2], fstream::out);

    // find label
    readFile([&](int i){
        if(label.compare("")!=0){
            detectErrorUseSameLabel(labelFill, label);
            labelFill.insert(pair<string, int>(label, i));
        }
        label = "";
    });

    // reopen file
    assembly.close(); 
    openFileStream(assembly, argv[1], fstream::in);

    // mapping form assembly to machine code
    readFile([&](int i){
        machine << map(i) << '\n';   
    });

    assembly.close();
    machine.close();
    cout << "compile complete\n";

    return(0);
}