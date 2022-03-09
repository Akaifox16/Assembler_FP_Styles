#include <iostream>
#include <string>
#include <fstream>
#include "../../header/error.h"

using std::cout, std::fstream, std::getline, std::string, std::endl;

int main(int argc, char *argv[]){
    fstream func, old;
    string funcLine, oldLine;
    bool isDifference = false;    

    error( argc != 3, [&](){
        cout << "error: usage: " << argv[0] << " <new-machine-code-file> <old-machine-code-file>\n" ;
    });

    func.open(argv[1], fstream::in);
    old.open(argv[2], fstream::in);

    for(int i= 1; getline(func, funcLine) && getline(old, oldLine) ; i++){
        if(funcLine.compare(oldLine) != 0){
            cout << "file difference in line " << i << endl;
            isDifference = true; break;
        }
    }
    
    if(!isDifference) cout << argv[1] << " and " << argv[2] << " is looking the same[100%].\n";
}