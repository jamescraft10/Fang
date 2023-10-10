#include <iostream>
#include <fstream>
#include <string>

std::string extractContents(const std::string& fileContents) {
    std::string output;
    size_t i = 0;
    while (i < fileContents.size()) {
        if (fileContents.substr(i, 6) == "print ") {
            i += 6;
            size_t startQuote = fileContents.find('"', i);
            size_t endQuote = fileContents.find('"', startQuote + 1);
            if (startQuote != std::string::npos && endQuote != std::string::npos) {
                std::string content = fileContents.substr(startQuote + 1, endQuote - startQuote - 1);
                output += "printf(\"" + content + "\");\n";
                i = endQuote + 1;
            }
        } else if (fileContents.substr(i, 7) == "return ") {
            output += "return ";
            i += 7;
            while (i < fileContents.size() && fileContents[i] != ';') {
                output += fileContents[i++];
            }
            output += ";\n";
        } else {
            ++i;
        }
    }
    return output;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return EXIT_FAILURE;
    }

    std::string FilePath = argv[1];
    std::ifstream inputFile(FilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file.\n";
        return EXIT_FAILURE;
    }

    std::string FileContents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    std::string Output = extractContents(FileContents);

    std::string OutputFileName = FilePath.substr(0, FilePath.find_last_of('.')) + ".c";
    std::ofstream OutputFile(OutputFileName);
    if (!OutputFile.is_open()) {
        std::cerr << "Error: Could not create output file.\n";
        return EXIT_FAILURE;
    }

    OutputFile << "#include <stdio.h>\nint main() {\n" << Output << "\nreturn 0;\n}";
    OutputFile.close();

    std::cout << "C program generated successfully.\n";

    return EXIT_SUCCESS;
}