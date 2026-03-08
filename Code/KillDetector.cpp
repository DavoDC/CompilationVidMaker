// KillDetector.cpp

#include "KillDetector.h"
#include <algorithm>

using namespace std;

vector<KillEvent> KillDetector::detect(const Batch& batch) {
    vector<KillEvent> events;
    int runningSeconds = 0;

    for (const Clip& clip : batch.clips) {
        string tier = detectTier(clip.getFileName());
        if (!tier.empty()) {
            KillEvent ev;
            ev.tier = tier;
            ev.timestampSeconds = runningSeconds;
            ev.clipName = clip.getFileName();
            events.push_back(ev);
        }
        runningSeconds += clip.getDuration();
    }

    return events;
}

string KillDetector::detectTier(const string& filename) {
    // Check highest tier first
    if (containsCI(filename, "hexa"))   return "Hexa Kill";
    if (containsCI(filename, "penta"))  return "Penta Kill";
    if (containsCI(filename, "quadra")) return "Quadra Kill";
    if (containsCI(filename, "quad"))   return "Quadra Kill";
    return "";
}

bool KillDetector::containsCI(const string& source, const string& query) {
    string srcLower = source;
    string qLower = query;
    transform(srcLower.begin(), srcLower.end(), srcLower.begin(), ::tolower);
    transform(qLower.begin(), qLower.end(), qLower.begin(), ::tolower);
    return srcLower.find(qLower) != string::npos;
}
