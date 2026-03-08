// Processor.cpp

#include "Processor.h"
#include "Command.h"
#include <chrono>

using namespace std;
using namespace chrono;

Processor::Processor(const string& clipsPath,
                     const string& exePath,
                     const string& outputPath)
    : clipsPath(clipsPath), exePath(exePath), outputPath(outputPath)
{
}

void Processor::run() {

    auto startTime = high_resolution_clock::now();

    string ffmpegPath  = exePath + "ffmpeg.exe";
    string ffprobePath = exePath + "ffprobe.exe";

    // Validate root clips path
    if (!isPathValid(clipsPath)) {
        printErr("Clips path not found: " + clipsPath, true);
    }

    // Create output folder if it doesn't exist
    if (!isPathValid(outputPath)) {
        fs::create_directories(outputPath);
        print("Created output folder: " + outputPath);
    }

    // Discover character subfolders (one level deep)
    vector<fs::path> charFolders;
    for (const auto& entry : fs::directory_iterator(clipsPath)) {
        if (entry.is_directory()) {
            charFolders.push_back(entry.path());
        }
    }
    sort(charFolders.begin(), charFolders.end());

    if (charFolders.empty()) {
        // No subfolders — treat clipsPath itself as single character
        charFolders.push_back(fs::path(clipsPath));
    }

    print("\nFound " + to_string(charFolders.size()) + " character folder(s).");

    Encoder encoder(ffmpegPath, outputPath);
    DescriptionWriter descWriter(outputPath);

    int totalBatches = 0;

    // Process each character folder independently
    for (const fs::path& charPath : charFolders) {
        string charName = charPath.filename().string();
        print("\n============================");
        print("Character: " + charName);
        print("============================");

        ClipList clipList(charPath.string(), ffprobePath);
        if (clipList.getCount() == 0) {
            print("No clips found, skipping.");
            continue;
        }

        Batcher batcher(clipList);

        int batchNum = 1;
        for (const Batch& batch : batcher.getBatches()) {
            print("\n--- " + charName + " Batch " + to_string(batchNum)
                + " of " + to_string(batcher.getCount()) + " ---");

            vector<KillEvent> kills = KillDetector::detect(batch);
            if (!kills.empty()) {
                print("Kill events: " + to_string(kills.size()));
                for (const KillEvent& ev : kills) {
                    print("  " + ev.tier + " at " + to_string(ev.timestampSeconds)
                        + "s — " + ev.clipName);
                }
            }

            encoder.encode(batch, charName);
            descWriter.write(batch, kills, charName);

            batchNum++;
            totalBatches++;
        }
    }

    // Summary
    auto stopTime = high_resolution_clock::now();
    double totalSecs = duration_cast<milliseconds>(stopTime - startTime).count() / 1000.0;

    print("\n=== Done ===");
    print("Characters processed: " + to_string(charFolders.size()));
    print("Total batches: " + to_string(totalBatches));
    print("Output folder: " + outputPath);
    print(Command::formatTimeTaken(totalSecs));
}
