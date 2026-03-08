// Main.cpp

#include "Common.h"
#include "Processor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

// Read a key=value config file, return map of keys to values
map<string, string> readConfig(const string& configPath) {
    map<string, string> cfg;
    ifstream file(configPath);
    if (!file.is_open()) {
        printErr("config.txt not found at: " + configPath, true);
    }
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto eq = line.find('=');
        if (eq == string::npos) continue;
        cfg[line.substr(0, eq)] = line.substr(eq + 1);
    }
    return cfg;
}

int main()
{
    print("###### Welcome to CompilationVidMaker! ######");
    print("Automates batching, encoding, and YouTube description generation.");

    auto cfg = readConfig("config.txt");
    string clipsPath  = cfg["ClipsPath"];
    string outputPath = cfg["OutputPath"];
    string ffmpegPath = cfg["FFMPEGPath"];

    Processor proc(clipsPath, ffmpegPath, outputPath);
    proc.run();

    print("\nPress Enter to exit...");
    cin.get();
}
