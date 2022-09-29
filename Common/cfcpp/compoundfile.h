#pragma once

#include "sectorcollection.h"
#include "cfstorage.h"
#include "slist.h"
#include <unordered_set>
#include "RBTree/rbtree.h"
#include "idirectoryentry.h"
#include <mutex>
#include "header.h"

namespace CFCPP
{
class DirectoryEntry;

enum CFSConfiguration
{
    Default = 1,                // No other flags
    SectorRecycle = 2,          // Rewrite unused sectors
    EraseFreeSectors = 4,       // Free sectors are erased to avoid information leakage
    NoValidationException = 8,  // Ignore some file reading errors to read broken files
    LeaveOpen = 16              // file will not be closed
};

enum CFSUpdateMode
{
    ReadOnly,
    Update
};

class CompoundFile
{
public:
    CompoundFile(const std::wstring &fileName, CFSUpdateMode updateMode, CFSConfiguration configParameters = Default);
    CompoundFile(CFSVersion cfsVersion, CFSConfiguration configFlags);
    CompoundFile(const std::wstring &fileName);
    CompoundFile(Stream stream);
    CompoundFile();


    // Main methods
    std::shared_ptr<CFStorage> RootStorage();

    void Save(std::wstring wFileName);
    void Save(Stream stream);


    void Commit(bool releaseMemory = false);
    bool HasSourceStream() const;
    bool ValidationExceptionEnabled() const;
    bool IsClosed()const;
    void Close();

    std::vector<BYTE> GetDataBySID(int sid);
    GUID getGuidBySID(int sid);
    GUID getGuidForStream(int sid);

    // internal methods
    static std::shared_ptr<RedBlackTree::RBTree> CreateNewTree();
    std::shared_ptr<RedBlackTree::RBTree> GetChildrenTree(int sid);
    SVector<IDirectoryEntry> &GetDirectories();
    void ResetDirectoryEntry(int sid);
    void InvalidateDirectoryEntry(int sid);
    void FreeAssociatedData(int sid);
    void FreeData(CFStream* stream);
    void WriteData(std::shared_ptr<CFItem> cfItem, const char* data, std::streamsize position, int count);
    void WriteData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE>& buffer, std::streamsize position, int offset, int count);
    void WriteData(std::shared_ptr<CFItem> cfItem, std::streamsize position, const std::vector<BYTE>& buffer);
    void WriteData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE>& buffer);
    void AppendData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE>& buffer);
    void SetStreamLength(std::shared_ptr<CFItem> cfItem, std::streamsize length);
    SList<Sector> FindFreeSectors(SectorType sType);
    std::vector<BYTE> GetData(const CFStream *cFStream);
    int ReadData(CFStream* cFStream, std::streamsize position, std::vector<BYTE>& buffer, int count);
    int ReadData(CFStream* cFStream, std::streamsize position, std::vector<BYTE>& buffer, int offset, int count);

protected:
    int GetSectorSize();
    void Dispose(bool disposing);

private:
    void CheckForLockSector();
    void OnSizeLimitReached();
    void LoadFile(std::wstring fileName);
    void SetFileName(std::wstring fileName);
    void LoadStream(Stream stream);
    void Load(Stream stream);

    SVector<Sector> GetFatSectorChain();
    SVector<Sector> GetDifatSectorChain();
    SVector<Sector> GetNormalSectorChain(int secID);
    SVector<Sector> GetMiniSectorChain(int secID);
    SVector<Sector> GetSectorChain(int secID, SectorType chainType);
    void EnsureUniqueSectorIndex(int nextSecID, std::unordered_set<int> &processedSectors);
    void CommitDirectory();
    void Close(bool closeStream);

    std::shared_ptr<IDirectoryEntry> RootEntry();
    SVector<IDirectoryEntry> FindDirectoryEntries(std::wstring entryName);

    std::shared_ptr<RedBlackTree::RBTree> DoLoadChildrenTrusted(std::shared_ptr<IDirectoryEntry> de);
    void DoLoadChildren(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de);
    void NullifyChildNodes(std::shared_ptr<IDirectoryEntry> de);
    void LoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de);
    void DoLoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de);
    bool ValidateSibling(int sid);
    void LoadDirectories();

    void FreeMiniChain(SVector<Sector>& sectorChain, bool zeroSector);
    void FreeMiniChain(SVector<Sector>& sectorChain, int nth_sector_to_remove, bool zeroSector);
    void FreeChain(SVector<Sector>& sectorChain, int nth_sector_to_remove, bool zeroSector);
    void FreeChain(SVector<Sector>& sectorChain, bool zeroSector);

    void AllocateSectorChain(SVector<Sector>& sectorChain);
    void AllocateFATSectorChain(SVector<Sector>& sectorChain);
    void AllocateDIFATSectorChain(SVector<Sector>& FATsectorChain);
    void AllocateMiniSectorChain(SVector<Sector>& sectorChain);
    void PersistMiniStreamToStream(const SVector<Sector>& miniSectorChain);
    static int LowSaturation(int i);
    void SetSectorChain(SVector<Sector> sectorChain);

    CFSVersion getVersion() const;

public:
    CFSConfiguration configuration = Default;
    std::unique_ptr<Header> header;
    Stream sourceStream;

private:
    const int HEADER_DIFAT_ENTRIES_COUNT = 109;
    int DIFAT_SECTOR_FAT_ENTRIES_COUNT = 127;
    int FAT_SECTOR_ENTRIES_COUNT = 128;
    const int SIZE_OF_SID = 4;
    bool sectorRecycle = false;
    bool eraseFreeSectors = false;
    static constexpr int FLUSHING_QUEUE_SIZE = 6000;
    static constexpr int FLUSHING_BUFFER_MAX_SIZE = 1024 * 1024 * 16;


    SectorCollection sectors;
    std::fstream stream;
    std::string fileName;
    std::shared_ptr<CFStorage> rootStorage;

    bool closeStream = true;
    bool _transactionLockAdded = false;
    int _lockSectorId = -1;
    bool _transactionLockAllocated = false;
    bool validationExceptionEnabled = true;
    bool _disposed = false;

    CFSUpdateMode updateMode;
    SVector<IDirectoryEntry> directoryEntries;
    std::list<int> levelSIDs;
    std::mutex lockObject;
};
}
