#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

bool CheckIfExtension(std::string::size_type n, std::string const& s, int Length) {
    if(std::string::npos == n) {
        std::cout << "\nThis file does not contain \".fa\"";
        return 1;
    } else {
        if(Length - 3 == n) {
            return 0;
        } else {
            std::cout << "\nThis file does not contain \".fa\"";
            return 1;
        }
    }
}

int main(int argc, char* argv[]) {
    std::string FilePath = argv[1];
    if(CheckIfExtension(FilePath.find(".fa"), FilePath, FilePath.length())) {
        return 1;
    }

    std::string FileContents;
    std::string OneLine;
    std::ifstream Program(FilePath);
    while(getline(Program, OneLine)) {
        FileContents += OneLine;
    }
    Program.close();

    std::cout << FileContents << std::endl;

    for(int i = 0; i <= FileContents.length(); ++i) {
        if(FileContents[i] == 'r' && FileContents[i+1] == 'e' && FileContents[i+2] == 't' && FileContents[i+3] == 'u' && FileContents[i+4] == 'r' && FileContents[i+5] == 'n' && FileContents[i+6] == ' ') {
            std::cout << "RETURN" << std::endl;
            int Value = FileContents[i+7] - 48; // The code for '0' is 48 and it goes up 0-9 so we subtract 48
            std::cout << Value << std::endl;
            if(FileContents[i+8] != ';') {
                std::cout << "Expected \';\'" << std::endl;
                return 1;
            } else {
                std::cout << "SEMI" << std::endl;
            }
            i += 8;
        }
    }

    return 0;
}