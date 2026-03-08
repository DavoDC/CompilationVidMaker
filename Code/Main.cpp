// Main.cpp

#include "Common.h"
#include "Processor.h"

using namespace std;

// ### Paths (relative to executable location)
string clipsPath  = "Clips\\";    // Drop input clips here
string exePath    = "FFMPEG\\";   // Drop ffmpeg.exe and ffprobe.exe here
string outputPath = "Output\\";   // Encoded videos and descriptions go here

int main()
{
    print("###### Welcome to CompilationVidMaker! ######");
    print("Automates batching, encoding, and YouTube description generation.");

    Processor proc(clipsPath, exePath, outputPath);
    proc.run();

    print("\nPress Enter to exit...");
    cin.get();
}
