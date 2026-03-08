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

    // 1. Load clips
    print("\n=== Step 1: Loading Clips ===");
    ClipList clipList(clipsPath, ffprobePath);
    if (clipList.getCount() == 0) {
        printErr("No clips found in: " + clipsPath, true);
    }

    // 2. Batch into ~15min groups
    print("\n=== Step 2: Batching ===");
    Batcher batcher(clipList);
    if (batcher.getCount() == 0) {
        printErr("No batches created.", true);
    }

    // 3. Process each batch
    Encoder encoder(ffmpegPath, outputPath);
    DescriptionWriter descWriter(outputPath);

    int batchNum = 1;
    for (const Batch& batch : batcher.getBatches()) {
        print("\n=== Batch " + to_string(batchNum) + " of "
            + to_string(batcher.getCount()) + " ===");

        // Detect kills
        vector<KillEvent> kills = KillDetector::detect(batch);
        if (!kills.empty()) {
            print("Kill events detected: " + to_string(kills.size()));
            for (const KillEvent& ev : kills) {
                print("  " + ev.tier + " at " + to_string(ev.timestampSeconds) + "s — " + ev.clipName);
            }
        }

        // Encode
        encoder.encode(batch);

        // Write description
        descWriter.write(batch, kills);

        batchNum++;
    }

    // Summary
    auto stopTime = high_resolution_clock::now();
    double totalSecs = duration_cast<milliseconds>(stopTime - startTime).count() / 1000.0;

    print("\n=== Done ===");
    print("Batches processed: " + to_string(batcher.getCount()));
    print("Output folder: " + outputPath);
    print(Command::formatTimeTaken(totalSecs));
}
