#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include "reader.h"

using   std::string, 
        std::endl,
        std::ofstream,
        std::cout, 
        std::map , 
        std::pair,
        std::bitset;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <assembly-code-file> <machine-code-file>\n" ;
        exit(1);
    }
    
    FILE *inFilePtr, *outFilePtr;
    string  label, opcode, 
            arg0, arg1, arg2;
    map<string, int> labelFill;

    auto errorMessage = [](auto filePtr, char *fileString) {
        if (filePtr == NULL) {
            cout << "error in opening " <<  fileString << endl;
            exit(1);
        }
    };
    auto checkFile = [](auto arg, auto mode){
        auto filePtr = fopen(arg, mode);
        errorMessage(filePtr, arg);
        return filePtr;
    };
    inFilePtr = checkFile(argv[1], "r");
    outFilePtr = checkFile(argv[2], "w");

    ofstream machine; 
    machine.open(outFileString, ofstream::out);
    int i = 0;

    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
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

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);
    i = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        string slabel = label;
        if (!opcode.compare("add")) {    /* opcode "add" */
            string regA_add = bitset<3>(stoi(arg0)).to_string();
            string regB_add = bitset<3>(stoi(arg1)).to_string();
            string regDest_add = bitset<3>(stoi(arg2)).to_string();
            string fullcode_add = "0000000000" + regA_add + regB_add + "0000000000000" + regDest_add;
            machine << stoi(fullcode_add, nullptr, 2) << endl;
        }
        else if (!opcode.compare("nand")){  /* opcode "nand" */
            string fullcode_nand = "0000000001" + bitset<3>(stoi(arg0)).to_string() + bitset<3>(stoi(arg1)).to_string() + "0000000000000" +bitset<3>(stoi(arg2)).to_string();
            machine << stoi(fullcode_nand, nullptr, 2) << endl;
        }
        else if (!opcode.compare("lw")){     /* opcode "lw" */
            string regA = bitset<3>(stoi(arg0)).to_string();
            string regB = bitset<3>(stoi(arg1)).to_string();
            string offset ;
            /* if arg2 is label fill offset with its label value
             * else fill offset with its value. */
            int arg2_1stchar = int(arg2[0])-48;
            if(arg2_1stchar != -3 && arg2_1stchar < 0 || arg2_1stchar > 9){
                if(labelFill.find(arg2)==labelFill.end()){
                    cout << "error : use undefine label\n";
                    exit(1);
                }
                if((labelFill.find(arg2)->second >> 16) > 0){
                    cout << "error : use over size offset\n";
                    exit(1);
                }
                offset =  bitset<16>(labelFill.find(arg2)->second).to_string();
            }else{
                if((stoi(arg2)>>16) > 0){
                    cout << "error : use over size offset\n";
                    exit(1);
                }
                offset =  bitset<16>(stoi(arg2)).to_string();
            }
            string fullcode = "010" + regA + regB + offset;
            machine << stoi(fullcode, nullptr, 2) << endl;
        }else if (!opcode.compare("sw")){   /* opcode "sw" */
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
            string fullcode = "011" + regA + regB + offset;
            machine << stoi(fullcode, nullptr, 2) << endl;
        }
        else if (!opcode.compare("beq")){    /* opcode "beq" */
            int Arg2_beq;
            string off_beq;
            /* check arg0 is Number and check size of offset
             * else prtint error "use undefine label" */
            if (isNumber(arg2))
            {
                Arg2_beq = stoi(arg2);
                if((Arg2_beq >> 16) > 0){
                    cout << "error : use over size offset\n";
                    exit(1);
                }
                off_beq = bitset<16>(Arg2_beq).to_string();
            }
            else{
                if(labelFill.find(arg2)==labelFill.end()){
                    cout << "error : use undefine label\n";
                    exit(1);
                }
                int beq_jump = labelFill.find(arg2) -> second;
                int jump = beq_jump-i-1;
                if((jump>>16) > 0){
                    cout << "error : use over size offset\n";
                    exit(1);
                }
                off_beq  = IntegerToString2sComp(jump);
            } 
            string regA_beq = bitset<3>(stoi(arg0)).to_string();
            string regB_beq = bitset<3>(stoi(arg1)).to_string();
            string fullcode_beq = "0000000100" + regA_beq +regB_beq + off_beq ;
            machine  << stoi(fullcode_beq, nullptr, 2)  << endl;
        }
        else if (!opcode.compare("jalr")){  /* opcode "jalr" */
            string regA_j = bitset<3>(stoi(arg0)).to_string();
            string regB_j = bitset<3>(stoi(arg1)).to_string();
            string fullcode_j = "101" + regA_j +regB_j + "0000000000000000" ;
            machine  << stoi(fullcode_j, nullptr, 2)  << endl;
        }
        else if (!opcode.compare("halt")){   /* opcode "jalr" */
            machine<< 25165824 << endl;
        }
        else if (!opcode.compare("noop")){   /* opcode "noop" */
            machine << 29360128 << endl;
        }else if (!opcode.compare(".fill")){     /* opcode "fill */
            /* if .fill following with label insert that label value
             * else .fill following with num insert that number value. */
            if(isNumber(arg0)){
                string::size_type sz;
                int labelValue = stoi(arg0,&sz);
                machine << labelValue << '\n';
            }else{
                machine << labelFill.find(arg0)->second << '\n';
            }
        }else{
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