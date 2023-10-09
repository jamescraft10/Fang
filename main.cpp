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
    if(argv[1] == 0) {
        std::cout << "Must pass in a file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string FilePath = argv[1];
    if(CheckIfExtension(FilePath.find(".fa"), FilePath, FilePath.length())) {
        return EXIT_FAILURE;
    }

    std::string FileContents;
    std::string OneLine;
    std::ifstream Program(FilePath);
    while(getline(Program, OneLine)) {
        FileContents += OneLine;
    }
    Program.close();

    std::cout << FileContents << std::endl;

    std::string Output = "";
    for(int i = 0; i <= FileContents.length(); ++i) {
        // Return
        if(FileContents[i] == 'p' && FileContents[i+1] == 'r' && FileContents[i+2] == 'i' && FileContents[i+3] == 'n' && FileContents[i+4] == 't' && FileContents[i+5] == ' ') {
            Output += "printf(\"";
            int j = 6;
            while(true) {
                ++j;
                if(FileContents[i+j] != '"') {
                    Output += FileContents[i+j];
                } else if(FileContents[i+j] == '"') {
                    break;
                }
            }
            Output += "\");\n";
        }
        if(FileContents[i] == 'r' && FileContents[i+1] == 'e' && FileContents[i+2] == 't' && FileContents[i+3] == 'u' && FileContents[i+4] == 'r' && FileContents[i+5] == 'n' && FileContents[i+6] == ' ') {
            Output += "return ";
            int j = 6;
            while(true) {
                ++j;
                if(FileContents[i+j] != ';') {
                    Output += FileContents[i+j];
                } else if(FileContents[i+j] == ';') {
                    break;
                }
            }
            Output += ";";
        }
    }

    std::string Name = FilePath;
    Name[Name.length() - 1] = ' ';
    Name[Name.length() - 2] = 'c';
    std::ofstream OutputFile(Name);
    OutputFile << "#include <stdio.h>\nint main() {\n" << Output << "\n}";
    OutputFile.close();

    std::cout << Output << std::endl;

    return EXIT_SUCCESS;
}