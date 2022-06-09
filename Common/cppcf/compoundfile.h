#pragma once

#include "header.h"
#include "sectorcollection.h"

namespace CFCPP
{
enum CFSConfiguration
{

    Default = 1,
    SectorRecycle = 2,
    EraseFreeSectors = 4,
    NoValidationException = 8,
    LeaveOpen = 16
};

enum CFSVersion : int
{
    /// Compound file version 3 - The default and most common version available. Sector size 512 bytes, 2GB max file size.
    Ver_3 = 3,
    /// Compound file version 4 - Sector size is 4096 bytes. Using this version could bring some compatibility problem with existing applications.
    Ver_4 = 4
};
enum CFSUpdateMode
{
    /// ReadOnly update mode prevents overwriting
    /// of the opened file.
    /// Data changes are allowed but they have to be
    /// persisted on a different file when required
    ReadOnly,

    /// Update mode allows subsequent data changing operations
    /// to be persisted directly on the opened file or stream
    /// method when required. Warning: this option may cause existing data loss if misused.
    Update
};

class CompoundFile
{
public:
    CompoundFile();
    CompoundFile(CFSVersion cfsVersion, CFSConfiguration configFlags);
    void OnSizeLimitReached();
protected:
    int GetSectorSize();

public:
    CFSConfiguration configuration = Default;
    Header header;

private:
    const int HEADER_DIFAT_ENTRIES_COUNT = 109;
    const int DIFAT_SECTOR_FAT_ENTRIES_COUNT = 127;
    const int FAT_SECTOR_ENTRIES_COUNT = 128;
    const int SIZE_OF_SID = 4;
    bool sectorRecycle = false;
    bool eraseFreeSectors = false;
    const int FLUSHING_QUEUE_SIZE = 6000;
    static constexpr int FLUSHING_BUFFER_MAX_SIZE = 1024 * 1024 * 16;
    SectorCollection sectors;
    std::fstream stream;
};
}
