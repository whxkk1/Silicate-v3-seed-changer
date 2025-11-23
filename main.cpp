#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

std::string openFileDialog() //change this if you are not on windows
{
    OPENFILENAMEA ofn;
    char fileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = "";

    if (GetOpenFileNameA(&ofn))
        return std::string(fileName);

    return "";
}


void viewSeed(const std::string& input) {
    std::ifstream macro(input, std::ios::binary);
    if (!macro) {
        std::cout << "Error opening the macro.";
        return;
    }
    macro.seekg(0x12, std::ios::beg);

    uint64_t seed = 0;

    macro.read(reinterpret_cast<char*>(&seed), 8);
    std::cout << "Current seed: " << seed;
}

void changeSeed(const std::string& input, const uint64_t& seed) {
    std::fstream macro(input, std::ios::in | std::ios::out | std::ios::binary);
    if (!macro) {
        std::cout << "Error opening the macro.";
        return;
    }

    std::ofstream backup(input + ".bak", std::ios::binary); //backup just in case
    backup << macro.rdbuf();
    backup.close();

    macro.seekp(0x12, std::ios::beg);

    macro.write(reinterpret_cast<const char*>(&seed), 8);
}

int main() {
    std::cout << "Enter a slc3 macro: \n";
    std::string macro = openFileDialog();

    viewSeed(macro);

    std::cout << "\nEnter new seed (uint64_t): ";
    uint64_t seed;
    std::cin >> seed;

    changeSeed(macro, seed);

}
