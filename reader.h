#include <cstdlib>
#include <cstdio>
#include <string>
#include <algorithm>

using std::string;

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
int readAndParse(FILE *inFilePtr, auto label, auto opcode, auto arg0,
    auto arg1, auto arg2)
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

bool isNumber(const string& s)
{
    return s.find_first_not_of("0123456789") == string::npos;
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