#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

bool CheckIfExtension(string::size_type n, string const& s, int Length) {
    if(string::npos == n) {
        cout << "\nThis file does not contain \".fa\"";
        return 1;
    } else {
        if(Length - 3 == n) {
            return 0;
        } else {
            cout << "\nThis file does not contain \".fa\"";
            return 1;
        }
    }
}

int main(int argc, char* argv[]) {
    string FilePath = argv[1];
    if(CheckIfExtension(FilePath.find(".fa"), FilePath, FilePath.length())) {
        return 1;
    }

    string FileContents;
    string OneLine;
    ifstream Program(FilePath);
    while(getline(Program, OneLine)) {
        FileContents += OneLine;
    }
    Program.close();

    cout << FileContents << endl;
    return 0;
}