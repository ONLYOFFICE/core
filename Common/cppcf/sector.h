#pragma once

#include <sstream>
#include <fstream>
#include <memory>
#include "../../DesktopEditor/common/Types.h"
#include <vector>
#include <mutex>

namespace CFCPP
{
enum SectorType
{
    Normal,
    Mini,
    FAT,
    DIFAT,
    RangeLockSector,
    Directory
};

class Sector
{
public:
    Sector(int size, std::unique_ptr<std::fstream> stream);
    Sector(int size, const std::vector<BYTE> &data);
    Sector(int size);

    bool IsStreamed();
    void ZeroData();
    void InitFATData();
    void ReleaseData();

    virtual void Dispose(bool disposing=false);

public:
    static int MINISECTOR_SIZE;
    const int FREESECT = 0xFFFFFFFF;
    const int ENDOFCHAIN = 0xFFFFFFFE;
    const int FATSECT = 0xFFFFFFFD;
    const int DIFSECT = 0xFFFFFFFC;

    int getSize() const;

private:
    bool dirtyFlag = false;
    int size = 0;
    std::unique_ptr<std::fstream> stream;
    SectorType type;
    int id = -1;
    std::vector<BYTE> data;
    std::mutex lockObject;
    bool _disposed;//false
};

}
