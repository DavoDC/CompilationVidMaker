#pragma once

// DescriptionWriter.h

// ### Headers
#include "Common.h"
#include "Batcher.h"
#include "KillDetector.h"

/**
 * @brief Generates a YouTube description .txt file for a batch.
 *        Includes clip filenames and kill timestamps.
*/
class DescriptionWriter {
public:
    // ### Constructor
    explicit DescriptionWriter(const std::string& outputPath);

    // ### Public methods

    /**
     * @brief Write description file for a batch.
    */
    void write(const Batch& batch, const std::vector<KillEvent>& kills);

private:
    std::string outputPath;

    /**
     * @return Timestamp formatted as MM:SS or H:MM:SS.
    */
    static std::string formatTimestamp(int seconds);
};
