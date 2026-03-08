// ClipList.cpp

#include "ClipList.h"
#include "Command.h"
#include <algorithm>

using namespace std;

// Supported video extensions
static const StringV VIDEO_EXTS = { ".mp4", ".mov", ".mkv", ".avi", ".webm" };

ClipList::ClipList(const string& clipsPath, const string& ffprobePath) {

    if (!isPathValid(clipsPath)) {
        printErr("Clips folder not found: " + clipsPath, true);
    }

    print("Scanning clips folder: " + clipsPath);

    // Collect and sort clip files alphabetically
    vector<fs::path> clipPaths;
    for (const auto& entry : fs::directory_iterator(clipsPath)) {
        if (!entry.is_regular_file()) continue;
        string ext = entry.path().extension().string();
        // Lowercase ext for comparison
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (find(VIDEO_EXTS.begin(), VIDEO_EXTS.end(), ext) != VIDEO_EXTS.end()) {
            clipPaths.push_back(entry.path());
        }
    }
    sort(clipPaths.begin(), clipPaths.end());

    // Build Clip objects
    for (const auto& path : clipPaths) {
        string fp = path.string();
        int dur = getDurationSeconds(fp, ffprobePath);
        if (dur > 0) {
            clips.emplace_back(fp, dur);
            print("  [" + to_string(dur) + "s] " + path.filename().string());
        } else {
            printErr("Could not read duration, skipping: " + path.filename().string());
        }
    }

    print("Loaded " + to_string(clips.size()) + " clips.");
}

const vector<Clip>& ClipList::getClips() const {
    return clips;
}

int ClipList::getCount() const {
    return static_cast<int>(clips.size());
}

int ClipList::getDurationSeconds(const string& filePath, const string& ffprobePath) {
    // ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 "file"
    Command cmd(ffprobePath, {
        "-v", "error",
        "-show_entries", "format=duration",
        "-of", "default=noprint_wrappers=1:nokey=1",
        quoteD(filePath)
    });
    cmd.run();
    string output = cmd.getOutput();
    try {
        return static_cast<int>(stod(output));
    } catch (...) {
        return -1;
    }
}
