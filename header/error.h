#include<functional>
#include<fstream>
#include<string>
#include<map>

using   std::function,
        std::fstream,
        std::map,std::string,
        std::cout;

void error(const bool& condition, function<void()> callback){
    if( condition ){
        callback();
        exit(1);
    }
}

void detectErrorFileOpening(fstream& fileStream, char* fileString){
    error(!fileStream, [&](){
        cout << "error in opening " << fileString << '\n';
    });
}

void detectErrorOversizeOffset(int number){
    error(number >> 16 > 0 , [](){
        cout << "error: use oversize offset !\n";
    });
}

void detectErrorUndefineLabel(map<string, int> label, string arg){
    error(label.find(arg) == label.end(), [](){
        cout << "error: use undefine label !\n";
    });
}

void detectErrorUseSameLabel(map<string, int> label, string l){
    error(label.find(l) != label.end(), [](){
        cout << "error: use same label\n";
    });
}