#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string extractContents(const std::string& fileContents) {
    std::string output;
    std::string line;
    std::istringstream stream(fileContents);
    while(std::getline(stream, line)) {
        // Check if the line ends with a semicolon
        if(!line.empty() && line.back() == ';') {
            size_t pos = line.find("print ");
            if(pos != std::string::npos) {
                pos += 6;
                size_t startQuote = line.find('"', pos);
                size_t endQuote = line.find('"', startQuote + 1);
                if(startQuote != std::string::npos && endQuote != std::string::npos) {
                    std::string content = line.substr(startQuote + 1, endQuote - startQuote - 1);
                    output += "printf(\"" + content + "\");\n";
                }
            } else if(line.find("return ") == 0) {
                size_t pos = line.find("return ") + 7;
                output += "return " + line.substr(pos, line.size() - pos - 1) + ";\n";
            }
        } else {
            // Handle error: Missing semicolon at the end of the statement
            std::cerr << "Error: Missing semicolon at the end of the statement: " << line << "\n";
            return "";
        }
    }
    return output;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return EXIT_FAILURE;
    }

    std::string FilePath = argv[1];
    std::ifstream inputFile(FilePath);
    if(!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file.\n";
        return EXIT_FAILURE;
    }

    std::string FileContents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    std::string Output = extractContents(FileContents);
    if(Output.substr(0, 4) == "ERROR") {
        return EXIT_FAILURE;
    }

    std::string OutputFileName = FilePath.substr(0, FilePath.find_last_of('.')) + ".c";
    std::ofstream OutputFile(OutputFileName);
    if(!OutputFile.is_open()) {
        std::cerr << "Error: Could not create output file.\n";
        return EXIT_FAILURE;
    }

    OutputFile << "#include <stdio.h>\nint main() {\n" << Output << "\nreturn 0;\n}";
    OutputFile.close();

    std::cout << "C program generated successfully.\n";

    return EXIT_SUCCESS;
}