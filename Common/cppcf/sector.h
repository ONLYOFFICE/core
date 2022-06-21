#pragma once

#include "Stream.h"
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
    Sector(int size, const Stream &stream);
    Sector(int size, const std::vector<BYTE> &data);
    Sector(int size);

    bool IsStreamed();
    void ZeroData();
    void InitFATData();
    void ReleaseData();

    virtual void Dispose(bool disposing=false);
    std::vector<BYTE> &GetData();

public:
    static int MINISECTOR_SIZE;
    const static int FREESECT = 0xFFFFFFFF;
    const static int ENDOFCHAIN = 0xFFFFFFFE;
    const static int FATSECT = 0xFFFFFFFD;
    const static int DIFSECT = 0xFFFFFFFC;

    int getSize() const;

    SectorType type;
    bool dirtyFlag = false;
    int id = -1;

private:
    int size = 0;
    Stream stream;
    std::vector<BYTE> data;
    std::mutex lockObject;
    bool _disposed;//false
};

}
