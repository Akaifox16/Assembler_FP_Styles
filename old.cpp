#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>

using namespace std;

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int convertNum(int num);
string IntegerToString2sComp(int Integer);

int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    map<string, int> labelFill;

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }
    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    ofstream machine; 
    machine.open(outFileString, ofstream::out);
    int i = 0;

    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        string slabel = label;
        if(slabel.compare("")!=0){//if contain label
            if(labelFill.find(label)!=labelFill.end()){
                cout << "error : use same label\n";
                exit(1);
            }
            labelFill.insert(pair<string, int>(label,i));
        }
        strcpy(label,""); // clear label
        i++;
    }

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);
    i = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        string slabel = label;
        if (!strcmp(opcode, "add")) {    /* opcode "add" */
            string regA_add = bitset<3>(atoi(arg0)).to_string();
            string regB_add = bitset<3>(atoi(arg1)).to_string();
            string regDest_add = bitset<3>(atoi(arg2)).to_string();
            string fullcode_add = "0000000000" + regA_add + regB_add + "0000000000000" + regDest_add;
            machine << stoi(fullcode_add, nullptr, 2) << endl;
        }
        else if (!strcmp(opcode, "nand")){  /* opcode "nand" */
            string fullcode_nand = "0000000001" + bitset<3>(atoi(arg0)).to_string() + bitset<3>(atoi(arg1)).to_string() + "0000000000000" +bitset<3>(atoi(arg2)).to_string();
            machine << stoi(fullcode_nand, nullptr, 2) << endl;
        }
        else if (!strcmp(opcode, "lw")){     /* opcode "lw" */
            string regA = bitset<3>(atoi(arg0)).to_string();
            string regB = bitset<3>(atoi(arg1)).to_string();
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
                if((atoi(arg2)>>16) > 0){
                    cout << "error : use over size offset\n";
                    exit(1);
                }
                offset =  bitset<16>(atoi(arg2)).to_string();
            }
            string fullcode = "010" + regA + regB + offset;
            machine << stoi(fullcode, nullptr, 2) << endl;
        }else if (!strcmp(opcode, "sw")){   /* opcode "sw" */
            string regA = bitset<3>(atoi(arg0)).to_string();
            string regB = bitset<3>(atoi(arg1)).to_string();
            string offset ;
            /*check if arg2 is string or number
             *else print error and check type of error */
            if(isNumber(arg2)){
                if((atoi(arg2)>>16) > 0){
                    cout << "error : use over size offset\n";
                    exit(1);
                }
                offset = bitset<16>(atoi(arg2)).to_string();
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
        else if (!strcmp(opcode, "beq")){    /* opcode "beq" */
            int Arg2_beq;
            string off_beq;
            /* check arg0 is Number and check size of offset
             * else prtint error "use undefine label" */
            if (isNumber(arg2))
            {
                Arg2_beq = atoi(arg2);
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
            string regA_beq = bitset<3>(atoi(arg0)).to_string();
            string regB_beq = bitset<3>(atoi(arg1)).to_string();
            string fullcode_beq = "0000000100" + regA_beq +regB_beq + off_beq ;
            machine  << stoi(fullcode_beq, nullptr, 2)  << endl;
        }
        else if (!strcmp(opcode, "jalr")){  /* opcode "jalr" */
            string regA_j = bitset<3>(atoi(arg0)).to_string();
            string regB_j = bitset<3>(atoi(arg1)).to_string();
            string fullcode_j = "101" + regA_j +regB_j + "0000000000000000" ;
            machine  << stoi(fullcode_j, nullptr, 2)  << endl;
        }
        else if (!strcmp(opcode, "halt")){   /* opcode "jalr" */
            machine<< 25165824 << endl;
        }
        else if (!strcmp(opcode, "noop")){   /* opcode "noop" */
            machine << 29360128 << endl;
        }else if (!strcmp(opcode, ".fill")){     /* opcode "fill */
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
            cout << "error : use undefine opcode\n";
            exit(1);
        } 
        i++;
    }
    cout << "compile complete\n";
    machine.close();
    exit(0);
    return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
	printf("error: line too long\n");
	exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}

 int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit integer */
    if (num & (1<<15) ){
        num -= (1<<16);
    }
    return(num);
}
// 2'complement int-> binary function
string IntegerToString2sComp(int Integer)
{  
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