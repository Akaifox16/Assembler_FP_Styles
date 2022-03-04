// I/O 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// containers
#include <vector>
#include <map>

// others
#include <scn/scn.h>
#include "header/opcode.h"
#include "header/compile.h"
#include "header/error.h"

using   std::string, 
        std::stringstream,
        std::fstream,
        std::vector,
        std::map , 
        std::pair,
        std::endl,
        std::getline,
        std::cout ;

int main(int argc, char *argv[]) {
    fstream assembly, machine; 
    string line, label, opcode, arg0, arg1, arg2;
    map<string, int> labelFill;
    
    error( argc != 3, [&](){
        cout << "error: usage: " << argv[0] << " <assembly-code-file> <machine-code-file>\n" ;
    });
    
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

    openFileStream(assembly, argv[1], fstream::in);
    openFileStream(machine, argv[2], fstream::out);

    for(int i= 0; getline(assembly, line); i++){
        parse();
        if(label.compare("")!=0){// if contain label
            detectErrorUseSameLabel(labelFill, label);
            labelFill.insert(pair<string, int>(label, i));
        }
        label = ""; // clear label
    }
    
    // reopen file
    assembly.close(); 
    openFileStream(assembly, argv[1], fstream::in);
    
    auto fill_op = [&](auto arg0){
        if(isNumber(arg0)){
            string::size_type sz;
            int labelValue = stoi(arg0,&sz);
            return labelValue ;
        }
        return labelFill.find(arg0)->second ;
    };

    auto map = [&](int i){
        auto op = encodeOpcode(opcode);
        switch(op){
            case add: 
                // return r_type("000", arg0, arg1, arg2); 
            case nand: return r_type(op, arg0, arg1, arg2); 
            case lw: 
            // return s_type("010", arg0, arg1, isNumber(arg2) 
            //                 ? stoi(arg2) 
            //                 : labelFill.find(arg2)->second); 
            case sw: return s_type(op, arg0, arg1, isNumber(arg2) 
                            ? stoi(arg2) 
                            : labelFill.find(arg2)->second); 
            case beq: return s_type(op,arg0, arg1, 
                            isNumber(arg2) 
                            ? stoi(arg2) 
                            : labelFill.find(arg2)->second -i -1); 
            // case beq: return beq_type("100",arg0, arg1, arg2, labelFill, i); 
            case jalr: return jalr_type(op, arg0, arg1); 
            case halt: 
            // return n_type(op); 
            case noop: return n_type(op); 
            case fill: return fill_op(arg0); 
            default: 
                error(true, [](){
                    cout << "error: use undefined opcode !\n";
                });
        }
    };

    for(int i = 0; getline(assembly, line); i++){
        parse();
        machine << map(i) << '\n';        
    }

    assembly.close();
    machine.close();
    cout << "compile complete\n";

    exit(0);
    return(0);
}
