// DescriptionWriter.cpp

#include "DescriptionWriter.h"
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

DescriptionWriter::DescriptionWriter(const string& outputPath)
    : outputPath(outputPath)
{
}

void DescriptionWriter::write(const Batch& batch, const vector<KillEvent>& kills) {

    string outFile = outputPath + "\\batch" + to_string(batch.batchNumber) + "_description.txt";
    ofstream file(outFile);

    if (!file.is_open()) {
        printErr("Could not write description file: " + outFile);
        return;
    }

    // --- Clips section ---
    file << "Clips:\n";
    int index = 1;
    for (const Clip& clip : batch.clips) {
        file << index++ << ". " << clip.getFileName() << "\n";
    }

    // --- Timestamps section (only if kills found) ---
    if (!kills.empty()) {
        file << "\nTimestamps:\n";
        for (const KillEvent& ev : kills) {
            file << formatTimestamp(ev.timestampSeconds) << " " << ev.tier << "\n";
        }
    }

    file.close();
    printSuccess("Description written: " + outFile);
}

string DescriptionWriter::formatTimestamp(int seconds) {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;

    ostringstream oss;
    if (h > 0) {
        oss << h << ":" << setw(2) << setfill('0') << m << ":" << setw(2) << setfill('0') << s;
    } else {
        oss << m << ":" << setw(2) << setfill('0') << s;
    }
    return oss.str();
}
