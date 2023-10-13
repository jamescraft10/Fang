#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> variables;

std::string getType(const std::string& variableType) {
    if (variableType == "int") {
        return "int";
    } else if (variableType == "float") {
        return "float";
    } else if (variableType == "double") {
        return "double";
    } else if (variableType == "char") {
        return "char";
    } else if (variableType == "bool") {
        return "bool";
    }
    return "";
}

std::string getTypeValue(const std::string& variableType) {
    if (variableType == "int") {
        return "%d";
    } else if (variableType == "float") {
        return "%f";
    } else if (variableType == "double") {
        return "%lf";
    } else if (variableType == "char") {
        return "%c";
    } else if (variableType == "bool") {
        return "%d";
    }
    return "";
}

std::string extractContents(const std::string& fileContents) {
    std::string output;
    std::string line;
    std::istringstream stream(fileContents);

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == ';') {
            size_t posPrint = line.find("print ");
            size_t posReturn = line.find("return ");
            size_t posLet = line.find("let ");

            if (posPrint != std::string::npos) {
                posPrint += 6;
                std::string content = line.substr(posPrint, line.size() - posPrint - 1);
                if (content.front() == '"' && content.back() == '"') {
                    // Handle string literals
                    content = content.substr(1, content.size() - 2);
                    output += "printf(\"%s\\n\", \"" + content + "\");\n";
                } else {
                    // Handle variables
                    if (variables.find(content) != variables.end()) {
                        std::string typeSpecifier = getTypeValue(variables[content]);
                        if (!typeSpecifier.empty()) {
                            output += "printf(\"" + typeSpecifier + "\\n\", " + content + ");\n";
                        } else {
                            std::cerr << "Error: Invalid variable type for variable: " << content << "\n";
                            return "ERROR: Invalid variable type";
                        }
                    } else {
                        std::cerr << "Error: Undefined variable: " << content << "\n";
                        return "ERROR: Undefined variable";
                    }
                }
            } else if (posReturn != std::string::npos) {
                // Handle return statement
                size_t pos = line.find("return ") + 7;
                std::string returnValue = line.substr(pos, line.size() - pos - 1);
                if (variables.find(returnValue) != variables.end()) {
                    returnValue = variables[returnValue];
                }
                output += "return " + returnValue + ";\n";
            } else if (posLet == 0) {
                // Handle let statement
                size_t posColon = line.find(": ");
                size_t posEquals = line.find(" = ");
                if (posColon != std::string::npos && posEquals != std::string::npos) {
                    std::string variableName = line.substr(4, posColon - 4);
                    std::string variableType = line.substr(posColon + 2, posEquals - posColon - 2);
                    std::string variableValue = line.substr(posEquals + 3, line.size() - posEquals - 4);
                    variables[variableName] = variableType;
                    output += getType(variableType) + " " + variableName + " = " + variableValue + ";\n";
                }
            } else {
                std::cerr << "Error: Invalid statement: " << line << "\n";
                return "ERROR: Invalid statement";
            }
        } else {
            std::cerr << "Error: Missing semicolon at the end of the statement: " << line << "\n";
            return "ERROR: Missing semicolon";
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
    if (Output.substr(0, 5) == "ERROR") {
        return EXIT_FAILURE;
    }

    std::string OutputFileName = FilePath.substr(0, FilePath.find_last_of('.')) + ".c";
    std::ofstream OutputFile(OutputFileName);
    if (!OutputFile.is_open()) {
        std::cerr << "Error: Could not create output file.\n";
        return EXIT_FAILURE;
    }

    OutputFile << "#include <stdio.h>\n#include <stdbool.h>\nint main() {\n" << Output << "\n}";
    OutputFile.close();

    std::cout << "C program generated successfully.\n";

    return EXIT_SUCCESS;
}