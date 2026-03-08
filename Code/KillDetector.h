#pragma once

// KillDetector.h

// ### Headers
#include "Common.h"
#include "Batcher.h"

/**
 * @brief A detected multi-kill event within a batch.
*/
struct KillEvent {
    std::string tier;           // e.g. "Quadra Kill"
    int timestampSeconds = 0;   // Offset from start of the batch
    std::string clipName;       // Filename of the clip it was found in
};

/**
 * @brief Scans clip filenames for multi-kill keywords and records timestamps.
*/
class KillDetector {
public:
    /**
     * @brief Scan a batch and return all detected kill events with timestamps.
    */
    static std::vector<KillEvent> detect(const Batch& batch);

private:
    /**
     * @brief Return kill tier label if found in filename, empty string if not.
     *        Case-insensitive. Checks hexa > penta > quadra (highest first).
    */
    static std::string detectTier(const std::string& filename);

    /**
     * @brief Case-insensitive substring check.
    */
    static bool containsCI(const std::string& source, const std::string& query);
};
