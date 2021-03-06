#pragma once

#include "PsxPatchGroup.h"
#include "Sequence.h"

#include <map>
#include <set>

class InputStream;
class OutputStream;

BEGIN_NAMESPACE(AudioTools)

struct Sequence;

//------------------------------------------------------------------------------------------------------------------------------------------
// Container representing the entire of a Williams module file (.WMD file).
// Note: I'm only supporting data structures relating to the PSX sound driver, other formats (N64, PC) will be ignored.
//------------------------------------------------------------------------------------------------------------------------------------------
struct Module {
    uint8_t                 maxActiveSequences;     // The number of sequence work areas to allocate, or the maximum number of active sequences
    uint8_t                 maxActiveTracks;        // The number of track work areas to allocate, or the maximum number of active tracks
    uint8_t                 maxGatesPerSeq;         // Maximum number of 'gates' or on/off conditional jump switches per sequence
    uint8_t                 maxItersPerSeq;         // Maximum number of iteration counts (iters) per sequence; these are used to do jumps a certain number of times
    uint8_t                 maxCallbacks;           // Maximum number of user sequencer callbacks that can be used
    PsxPatchGroup           psxPatchGroup;          // PSX sound driver: patches
    std::vector<Sequence>   sequences;              // All of the sequences in the module file

    void readFromJson(const rapidjson::Document& doc) THROWS;
    void writeToJson(rapidjson::Document& doc) const noexcept;
    void readFromWmdFile(InputStream& in) THROWS;
    void writeToWmdFile(OutputStream& out) const THROWS;
    void resetToDefault() noexcept;

    void copyPatchSamples(
        const std::set<uint16_t>& srcPatchSampleIndexes,
        Module& intoModule,
        std::map<uint16_t, uint16_t>& oldAndNewPatchSampleIndexes
    ) const noexcept;

    void copyPatchVoices(
        const std::set<uint16_t>& srcPatchVoiceIndexes,
        Module& intoModule,
        std::map<uint16_t, uint16_t>& oldAndNewPatchVoiceIndexes,
        std::map<uint16_t, uint16_t>& oldAndNewPatchSampleIndexes
    ) const noexcept;

    void copyPatches(
        const std::set<uint16_t>& srcPatchIndexes,
        Module& intoModule,
        std::map<uint16_t, uint16_t>& oldAndNewPatchIndexes,
        std::map<uint16_t, uint16_t>& oldAndNewPatchVoiceIndexes,
        std::map<uint16_t, uint16_t>& oldAndNewPatchSampleIndexes
    ) const noexcept;

    void copySequences(
        const std::set<uint16_t>& srcSequenceIndexes,
        Module& intoModule,
        std::map<uint16_t, uint16_t>& oldAndNewSequenceIndexes,
        std::map<uint16_t, uint16_t>& oldAndNewPatchIndexes,
        std::map<uint16_t, uint16_t>& oldAndNewPatchVoiceIndexes,
        std::map<uint16_t, uint16_t>& oldAndNewPatchSampleIndexes
    ) const noexcept;

    // WMD file reading utilities
    static void skipReadingWmdPatchGroup(InputStream& in, const WmdPatchGroupHdr& patchGroupHdr) THROWS;
};

END_NAMESPACE(AudioTools)
