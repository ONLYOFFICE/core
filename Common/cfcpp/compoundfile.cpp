#include "compoundfile.h"
#include "cfstorage.h"
#include "header.h"
#include "directoryentry.h"
#include "cfexception.h"
#include "streamview.h"
#include "../../DesktopEditor/common/File.h"
#include <algorithm>
#include <cmath>
#include "sector.h"


using namespace CFCPP;

CompoundFile::CompoundFile() :
    CompoundFile(CFSVersion::Ver_3, CFSConfiguration::Default)
{}

CompoundFile::CompoundFile(const std::wstring &fileName, CFSUpdateMode updateMode, CFSConfiguration configParameters)
{
    configuration = configParameters;
    validationExceptionEnabled = !(configParameters & CFSConfiguration::NoValidationException);
    sectorRecycle = configParameters & CFSConfiguration::SectorRecycle;
    this->updateMode = updateMode;
    eraseFreeSectors = configParameters & CFSConfiguration::EraseFreeSectors;

    LoadFile(fileName);

    DIFAT_SECTOR_FAT_ENTRIES_COUNT = (GetSectorSize() / 4) - 1;
    FAT_SECTOR_ENTRIES_COUNT = (GetSectorSize() / 4);
}

CompoundFile::CompoundFile(CFSVersion cfsVersion, CFSConfiguration configFlags) : header(new Header(cfsVersion))
{
    configuration = configFlags;

    sectorRecycle = configFlags & CFSConfiguration::SectorRecycle;
    eraseFreeSectors = configFlags & CFSConfiguration::EraseFreeSectors;

    if (cfsVersion == CFSVersion::Ver_4)
    {
        Ver3SizeLimitReached action = std::bind(&CompoundFile::OnSizeLimitReached, this);
        sectors.OnVer3SizeLimitReached += action;
    }


    DIFAT_SECTOR_FAT_ENTRIES_COUNT = (GetSectorSize() / 4) - 1;
    FAT_SECTOR_ENTRIES_COUNT = (GetSectorSize() / 4);

    // Root
    std::shared_ptr<IDirectoryEntry> rootDir = DirectoryEntry::New(L"Root Entry", StgType::StgRoot, directoryEntries);
    rootDir->setStgColor(StgColor::Black);

    rootStorage.reset(new CFStorage(this, rootDir));
}

CompoundFile::CompoundFile(const std::wstring &fileName)
{
    sectorRecycle = false;
    updateMode = CFSUpdateMode::ReadOnly;
    eraseFreeSectors = false;

    LoadFile(fileName);

    DIFAT_SECTOR_FAT_ENTRIES_COUNT = (GetSectorSize() / 4) - 1;
    FAT_SECTOR_ENTRIES_COUNT = (GetSectorSize() / 4);
}

CompoundFile::CompoundFile(Stream stream)
{
    LoadStream(stream);

    DIFAT_SECTOR_FAT_ENTRIES_COUNT = (GetSectorSize() / 4) - 1;
    FAT_SECTOR_ENTRIES_COUNT = (GetSectorSize() / 4);
}

void CompoundFile::OnSizeLimitReached()
{
    std::shared_ptr<Sector> rangeLockSector(new Sector(GetSectorSize(), sourceStream));
    sectors.Add(rangeLockSector);

    rangeLockSector->type = SectorType::RangeLockSector;

    _transactionLockAdded = true;
    _lockSectorId = rangeLockSector->id;
}


void CompoundFile::Commit(bool releaseMemory)
{
    if (_disposed)
        throw CFDisposedException("Compound File closed: cannot commit data");

    if (updateMode != CFSUpdateMode::Update)
        throw CFInvalidOperation("Cannot commit data in Read-Only update mode");

    int sSize = GetSectorSize();

    if (header->majorVersion != (ushort)CFSVersion::Ver_3)
        CheckForLockSector();

    sourceStream->seek(0, std::ios::beg);

    std::vector<char> zeroArray(sSize, 0);
    sourceStream->write(zeroArray.data(), zeroArray.size());
    zeroArray.clear();

    CommitDirectory();

    bool gap = true;


    for (int i = 0; i < (int)sectors.largeArraySlices.size(); i++)
    {
        // Note:
        // Here sectors should not be loaded dynamically because
        // if they are null it means that no change has involved them;

        std::shared_ptr<Sector> s = sectors[i];

        if (s.get() != nullptr && s->dirtyFlag)
        {
            if (gap)
                sourceStream->seek((long)((long)(sSize) + (long)i * (long)sSize), std::ios::beg);

            sourceStream->write(reinterpret_cast<char*>(s->GetData().data()), sSize);
            sourceStream->flush();
            s->dirtyFlag = false;
            gap = false;

        }
        else
        {
            gap = true;
        }

        if (s.get() != nullptr && releaseMemory)
        {

            s->ReleaseData();
            s.reset();
            sectors[i].reset();
        }
    }


    // Seek to beginning position and save header (first 512 or 4096 bytes)
    sourceStream->seek(0, std::ios::beg);
    header->Write(sourceStream);

    sourceStream->flush();
}

bool CompoundFile::HasSourceStream() const
{
    return sourceStream != nullptr;
}

bool CompoundFile::ValidationExceptionEnabled() const
{
    return validationExceptionEnabled;
}

void CompoundFile::Close()
{
    Close(true);
}

std::shared_ptr<RedBlackTree::RBTree> CompoundFile::CreateNewTree()
{
    return std::shared_ptr<RedBlackTree::RBTree>(new RedBlackTree::RBTree);
}

std::shared_ptr<RedBlackTree::RBTree> CompoundFile::GetChildrenTree(int sid)
{
    std::shared_ptr<RedBlackTree::RBTree> bst(new RedBlackTree::RBTree());

    DoLoadChildren(bst, directoryEntries[sid]);

    return bst;
}

bool CompoundFile::IsClosed() const
{
    return _disposed;
}

void CompoundFile::Load(Stream stream)
{
    try
    {
        header.reset(new Header);
        directoryEntries.clear();

        this->sourceStream = stream;

        header->Read(stream);

        int n_sector = std::ceil(((double)(Length(stream) - GetSectorSize()) / (double)GetSectorSize()));

        if (Length(stream) > 0x7FFFFF0)
            this->_transactionLockAllocated = true;


        sectors.Clear();

        for (int i = 0; i < n_sector; i++)
        {
            sectors.Add({});
        }

        LoadDirectories();

        rootStorage.reset(new CFStorage(this, directoryEntries[0]));
    }
    catch (...)
    {
        if (stream && closeStream)
        {
            stream->close();
        }

        throw;
    }
}

void CompoundFile::Save(std::wstring wFileName)
{
    if (_disposed)
        throw CFException("Compound File closed: cannot save data");

    Stream fs = OpenFileStream(wFileName, true, true);
    fs->seek(0, std::ios::beg);

    try
    {
        Save(fs);
    }
    catch (std::exception& ex)
    {
        throw CFException("Error saving file [" + fileName + "]", ex);
    }
    //    finally
    {
        if (fs.get() != nullptr)
            fs->flush();

        if (fs.get() != nullptr)
            fs->close();

    }
}

void CompoundFile::Save(Stream stream)
{
    if (_disposed)
        throw CFDisposedException("Compound File closed: cannot save data");

    // Check seekable

    CheckForLockSector();
    int sSize = GetSectorSize();

    try
    {
        std::vector<char> zeroArray(sSize, 0);
        stream->write(zeroArray.data(), zeroArray.size());
        zeroArray.clear();

        CommitDirectory();

        for (int i = 0; i < sectors.Count(); i++)
        {
            auto s = sectors[i];

            if (s == nullptr)
            {
                // Load source (unmodified) sectors
                // Here we have to ignore "Dirty flag" of
                // sectors because we are NOT modifying the source
                // in a differential way but ALL sectors need to be
                // persisted on the destination stream
                s.reset(new Sector(sSize, sourceStream));
                s->id = i;

                //sectors[i] = s;
            }


            stream->write(reinterpret_cast<char*>(s->GetData().data()), sSize);

            //s.ReleaseData();

        }

        auto writePosition = stream->seek(0, std::ios::beg);
        header->Write(stream);
    }
    catch (std::exception &ex)
    {
        throw CFException("Internal error while saving compound file to stream ", ex);
    }
}

SVector<Sector> CompoundFile::GetFatSectorChain()
{
    int N_HEADER_FAT_ENTRY = 109; //Number of FAT sectors id in the header

    SVector<Sector> result;

    int nextSecID = Sector::ENDOFCHAIN;

    SVector<Sector> difatSectors = GetDifatSectorChain();

    int idx = 0;

    // Read FAT entries from the header Fat entry array (max 109 entries)
    while (idx < header->fatSectorsNumber && idx < N_HEADER_FAT_ENTRY)
    {
        nextSecID = header->difat[idx];
        auto s = sectors[nextSecID];

        if (s.get() == nullptr)
        {
            s.reset(new Sector(GetSectorSize(), sourceStream));
            s->id = nextSecID;
            s->type = SectorType::FAT;
            sectors[nextSecID] = s;
        }

        result.push_back(s);

        idx++;
    }

    //Is there any DIFAT sector containing other FAT entries ?
    if (difatSectors.size() > 0)
    {
        std::unordered_set<int> processedSectors;
        std::streamsize stLength = header->fatSectorsNumber > N_HEADER_FAT_ENTRY ?
                    (header->fatSectorsNumber - N_HEADER_FAT_ENTRY) * 4 : 0;
        SList<Sector> zeroQueue;

        std::shared_ptr<StreamView> difatStream(
                    new StreamView
                    (
                        difatSectors,
                        GetSectorSize(),
                        stLength,
                        zeroQueue,
                        sourceStream
                        )
                    );

        char nextDIFATSectorBuffer[4];

        int i = 0;

        while ((int)result.size() < header->fatSectorsNumber)
        {
            difatStream->read(nextDIFATSectorBuffer, 4); // IsLittleEndian ?
            nextSecID = *reinterpret_cast<int*>(nextDIFATSectorBuffer);

            EnsureUniqueSectorIndex(nextSecID, processedSectors);

            auto s = sectors[nextSecID];

            if (s.get() == nullptr)
            {
                s.reset(new Sector(GetSectorSize(), sourceStream));
                s->type = SectorType::FAT;
                s->id = nextSecID;
                sectors[nextSecID] = s;
            }

            result.push_back(s);

            if (difatStream->getPosition() == ((GetSectorSize() - 4) + i * GetSectorSize()))
            {
                // Skip DIFAT chain fields considering the possibility that the last FAT entry has been already read
                difatStream->read(nextDIFATSectorBuffer, 4);
                if (*reinterpret_cast<const int*>(nextDIFATSectorBuffer) == Sector::ENDOFCHAIN)
                    break;
                else
                {
                    i++;
                    continue;
                }
            }
        }
    }

    return result;
}

SVector<Sector> CompoundFile::GetDifatSectorChain()
{
    int validationCount = 0;

    SVector<Sector> result;

    int nextSecID  = Sector::ENDOFCHAIN;

    std::unordered_set<int> processedSectors;

    if (header->difatSectorsNumber != 0)
    {
        validationCount = (int)header->difatSectorsNumber;

        std::shared_ptr<Sector> s = sectors[header->firstDIFATSectorID];

        if (s == nullptr) //Lazy loading
        {
            s.reset(new Sector(GetSectorSize(), sourceStream));
            s->type = SectorType::DIFAT;
            s->id = header->firstDIFATSectorID;
            sectors[header->firstDIFATSectorID] = s;
        }

        result.push_back(s);

        while (validationCount >= 0)
        {
            int startPos = GetSectorSize() - 4;
            nextSecID = *reinterpret_cast<int*>(s->GetData().data() + startPos);
            EnsureUniqueSectorIndex(nextSecID, processedSectors);

            // Strictly speaking, the following condition is not correct from
            // a specification point of view:
            // only ENDOFCHAIN should break DIFAT chain but
            // a lot of existing compound files use FREESECT as DIFAT chain termination
            if (nextSecID == Sector::FREESECT || nextSecID == Sector::ENDOFCHAIN) break;

            validationCount--;

            if (validationCount < 0)
            {
                if (this->closeStream)
                    this->Close();

                if (this->validationExceptionEnabled)
                    throw CFCorruptedFileException("DIFAT sectors count mismatched. Corrupted compound file");
            }

            s = sectors[nextSecID];

            if (s == nullptr)
            {
                s.reset(new Sector(GetSectorSize(), sourceStream));
                s->id = nextSecID;
                sectors[nextSecID] = s;
            }

            result.push_back(s);
        }
    }

    return result;
}

SVector<Sector> CompoundFile::GetNormalSectorChain(int secID)
{
    SVector<Sector> result;

    int nextSecID = secID;

    SVector<Sector> fatSectors = GetFatSectorChain();
    std::unordered_set<int> processedSectors;

    SList<Sector> zeroQueue;
    StreamView fatStream(fatSectors, GetSectorSize(), fatSectors.size() * GetSectorSize(), zeroQueue, sourceStream);

    while (true)
    {
        if (nextSecID == Sector::ENDOFCHAIN) break;

        if (nextSecID < 0)
            throw CFCorruptedFileException("Next Sector ID reference is below zero. NextID : " + std::to_string(nextSecID));

        if (nextSecID >= sectors.Count())
            throw CFCorruptedFileException("Next Sector ID reference an out of range sector. NextID : " + std::to_string(nextSecID) +
                                               " while sector count " + std::to_string(sectors.Count()));

        std::shared_ptr<Sector> s = sectors[nextSecID];
        if (s == nullptr)
        {
            s.reset(new Sector(GetSectorSize(), sourceStream));
            s->id = nextSecID;
            s->type = SectorType::Normal;
            sectors[nextSecID] = s;
        }

        result.push_back(s);

        fatStream.seek(nextSecID * 4, std::ios::beg);
        int next = fatStream.ReadInt32();

        EnsureUniqueSectorIndex(next, processedSectors);
        nextSecID = next;

    }


    return result;
}

SVector<Sector> CompoundFile::GetMiniSectorChain(int secID)
{
    SVector<Sector> result;

    if (secID != Sector::ENDOFCHAIN)
    {
        int nextSecID = secID;

        SVector<Sector> miniFAT = GetNormalSectorChain(header->firstMiniFATSectorID);
        SVector<Sector> miniStream = GetNormalSectorChain(RootEntry()->getStartSetc());
        SList<Sector> zeroQueue;

        StreamView miniFATView(miniFAT, GetSectorSize(), header->miniFATSectorsNumber * Sector::MINISECTOR_SIZE, zeroQueue, sourceStream);

        StreamView miniStreamView(miniStream, GetSectorSize(), rootStorage->size(), zeroQueue, sourceStream);

        nextSecID = secID;

        std::unordered_set<int> processedSectors;

        while (true)
        {
            if (nextSecID == Sector::ENDOFCHAIN)
                break;

            std::shared_ptr<Sector> ms(new Sector(Sector::MINISECTOR_SIZE, sourceStream));

            ms->id = nextSecID;
            ms->type = SectorType::Mini;

            miniStreamView.seek(nextSecID * Sector::MINISECTOR_SIZE, std::ios::beg);
            miniStreamView.read(reinterpret_cast<char*>(ms->GetData().data()), Sector::MINISECTOR_SIZE);

            result.push_back(ms);

            miniFATView.seek(nextSecID * 4, std::ios::beg);
            int next = miniFATView.ReadInt32();

            nextSecID = next;
            EnsureUniqueSectorIndex(nextSecID, processedSectors);
        }
    }
    return result;
}

SVector<Sector> CompoundFile::GetSectorChain(int secID, SectorType chainType)
{
    switch (chainType)
    {
    case SectorType::DIFAT:
        return GetDifatSectorChain();

    case SectorType::FAT:
        return GetFatSectorChain();

    case SectorType::Normal:
        return GetNormalSectorChain(secID);

    case SectorType::Mini:
        return GetMiniSectorChain(secID);

    default:
        throw CFException("Unsupproted chain type");
    }
}

void CompoundFile::EnsureUniqueSectorIndex(int nextSecID, std::unordered_set<int>& processedSectors)
{
    if (processedSectors.find(nextSecID) != processedSectors.end() && this->validationExceptionEnabled)
    {
        throw CFCorruptedFileException("The file is corrupted.");
    }

    processedSectors.insert(nextSecID);
}

void CompoundFile::CommitDirectory()
{
    const int DIRECTORY_SIZE = 128;

    auto directorySectors
            = GetSectorChain(header->firstDirectorySectorID, SectorType::Normal);

    SList<Sector> zeroQueue;
    std::shared_ptr<StreamView> sv(
                new StreamView(
                    directorySectors,
                    GetSectorSize(),
                    0,
                    zeroQueue,
                    sourceStream
                    )
                );

    for (const auto& di : *directoryEntries)
    {
        di->Write(sv);
    }

    int delta = directoryEntries.size();

    while (delta % (GetSectorSize() / DIRECTORY_SIZE) != 0)
    {
        std::shared_ptr<IDirectoryEntry> dummy =
                DirectoryEntry::New(L"", StgType::StgInvalid, directoryEntries);
        dummy->Write(sv);
        delta++;
    }

    for (auto& s : *directorySectors)
    {
        s->type = SectorType::Directory;
    }

    AllocateSectorChain(directorySectors);

    header->firstDirectorySectorID = directorySectors[0]->id;

    //Version 4 supports directory sectors count
    if (header->majorVersion == 3)
    {
        header->directorySectorsNumber = 0;
    }
    else
    {
        header->directorySectorsNumber = directorySectors.size();
    }
}

void CompoundFile::Close(bool closeStream)
{
    this->closeStream = closeStream;
    Dispose(closeStream);
}

std::shared_ptr<IDirectoryEntry> CompoundFile::RootEntry()
{
    if (directoryEntries.empty())
        return {};
    return directoryEntries[0];
}

std::shared_ptr<CFStorage> CompoundFile::RootStorage()
{
    return rootStorage;
}

SVector<IDirectoryEntry> CompoundFile::FindDirectoryEntries(std::wstring entryName)
{
    SVector<IDirectoryEntry> result;

    for (auto& d : *directoryEntries)
    {
        if (d->GetEntryName() == entryName && d->getStgType() != StgType::StgInvalid)
            result.push_back(d);
    }

    return result;
}

std::shared_ptr<RedBlackTree::RBTree> CompoundFile::DoLoadChildrenTrusted(std::shared_ptr<IDirectoryEntry> de)
{
    std::shared_ptr<RedBlackTree::RBTree> bst;

    if (de->getChild() != DirectoryEntry::NOSTREAM)
    {
        bst.reset(new RedBlackTree::RBTree(directoryEntries[de->getChild()]));
    }

    return bst;
}

void CompoundFile::DoLoadChildren(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de)
{
    if (de->getChild() != DirectoryEntry::NOSTREAM)
    {
        if (directoryEntries[de->getChild()]->getStgType() == StgType::StgInvalid) return;

        LoadSiblings(bst, directoryEntries[de->getChild()]);
        NullifyChildNodes(std::static_pointer_cast<IDirectoryEntry>(directoryEntries[de->getChild()]));
        bst->Insert(std::static_pointer_cast<IDirectoryEntry>(directoryEntries[de->getChild()]));
    }
}

void CompoundFile::NullifyChildNodes(std::shared_ptr<IDirectoryEntry> de)
{
    de->setParent({});
    de->setLeft({});
    de->setRight({});
}

void CompoundFile::LoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de)
{
    levelSIDs.clear();

    if (de->getLeftSibling() != DirectoryEntry::NOSTREAM)
    {
        // If there're more left siblings load them...
        DoLoadSiblings(bst, directoryEntries[de->getLeftSibling()]);
    }

    if (de->getRightSibling() != DirectoryEntry::NOSTREAM)
    {
        levelSIDs.push_back(de->getRightSibling());

        // If there're more right siblings load them...
        DoLoadSiblings(bst, directoryEntries[de->getRightSibling()]);
    }
}

void CompoundFile::DoLoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de)
{
    if (ValidateSibling(de->getLeftSibling()))
    {
        levelSIDs.push_back(de->getLeftSibling());

        // If there're more left siblings load them...
        DoLoadSiblings(bst, directoryEntries[de->getLeftSibling()]);
    }

    if (ValidateSibling(de->getRightSibling()))
    {
        levelSIDs.push_back(de->getRightSibling());

        // If there're more right siblings load them...
        DoLoadSiblings(bst, directoryEntries[de->getRightSibling()]);
    }

    NullifyChildNodes(de);
    bst->Insert(de);
}

bool CompoundFile::ValidateSibling(int sid)
{
    if (sid != DirectoryEntry::NOSTREAM)
    {
        // if this siblings id does not overflow current list
        if (sid >= (int)directoryEntries.size())
        {
            if (this->validationExceptionEnabled)
            {
                //Close();
                throw CFCorruptedFileException("A Directory Entry references the non-existent sid number " + std::to_string(sid));
            }
            else
                return false;
        }

        //if this sibling is valid...
        if (directoryEntries[sid]->getStgType() == StgType::StgInvalid)
        {
            if (this->validationExceptionEnabled)
            {
                //Close();
                throw CFCorruptedFileException("A Directory Entry has a valid reference to an Invalid Storage Type directory [" + std::to_string(sid) + "]");
            }
            else
                return false;
        }

        int stgtype = directoryEntries[sid]->getStgType();
        if (false == (stgtype >= 0 && stgtype <= 5))
        {

            if (this->validationExceptionEnabled)
            {
                //Close();
                throw CFCorruptedFileException("A Directory Entry has an invalid Storage Type");
            }
            else
                return false;
        }

        if (std::find(levelSIDs.begin(), levelSIDs.end(), sid) != levelSIDs.end())
            throw CFCorruptedFileException("Cyclic reference of directory item");

        return true; //No fault condition encountered for sid being validated
    }

    return false;
}

void CompoundFile::LoadDirectories()
{
    SVector<Sector> directoryChain
            = GetSectorChain(header->firstDirectorySectorID, SectorType::Normal);

    if (!(directoryChain.size() > 0))
        throw CFCorruptedFileException("Directory sector chain MUST contain at least 1 sector");

    if (header->firstDirectorySectorID == Sector::ENDOFCHAIN)
        header->firstDirectorySectorID = directoryChain[0]->id;

    SList<Sector> zeroQueue;
    const auto sectorSize = GetSectorSize();
    Stream dirReader(new StreamView(directoryChain, sectorSize, directoryChain.size() * sectorSize, zeroQueue, sourceStream));


    while (dirReader->tell() < (std::streamsize)directoryChain.size() * sectorSize)
    {
        std::shared_ptr<IDirectoryEntry> de(DirectoryEntry::New(L"", StgType::StgInvalid, directoryEntries));

        //We are not inserting dirs. Do not use 'InsertNewDirectoryEntry'
        de->Read(dirReader, getVersion());

    }
}

void CompoundFile::FreeMiniChain(SVector<Sector> &sectorChain, bool zeroSector)
{
    FreeMiniChain(sectorChain,0, zeroSector);
}

void CompoundFile::FreeMiniChain(SVector<Sector> &sectorChain, int nth_sector_to_remove, bool zeroSector)
{
    std::vector<char> ZEROED_MINI_SECTOR(Sector::MINISECTOR_SIZE);

    SVector<Sector> miniFAT
            = GetSectorChain(header->firstMiniFATSectorID, SectorType::Normal);

    SVector<Sector> miniStream
            = GetSectorChain(RootEntry()->getStartSetc(), SectorType::Normal);

    SList<Sector> zeroQueue;
    StreamView miniFATView(miniFAT, GetSectorSize(), header->miniFATSectorsNumber * Sector::MINISECTOR_SIZE, zeroQueue, sourceStream);

    StreamView miniStreamView(miniStream, GetSectorSize(), rootStorage->size(), zeroQueue, sourceStream);

    // Set updated/new sectors within the ministream ----------
    if (zeroSector)
    {
        for (int i = nth_sector_to_remove; i < (int)sectorChain.size(); i++)
        {
            auto s = sectorChain[i];

            if (s->id != -1)
            {
                // Overwrite
                miniStreamView.seek(Sector::MINISECTOR_SIZE * s->id, std::ios::beg);
                miniStreamView.write(ZEROED_MINI_SECTOR.data(), Sector::MINISECTOR_SIZE);
            }
        }
    }

    // Update miniFAT                ---------------------------------------
    for (int i = nth_sector_to_remove; i < (int)sectorChain.size(); i++)
    {
        int currentId = sectorChain[i]->id;

        miniFATView.seek(currentId * 4, std::ios::beg);
        const int freesec = Sector::FREESECT;
        miniFATView.write(reinterpret_cast<const char*>(&freesec), 4);
    }

    // Write End of Chain in MiniFAT ---------------------------------------
    if (nth_sector_to_remove > 0 && sectorChain.size() > 0)
    {
        miniFATView.seek(sectorChain[nth_sector_to_remove - 1]->id * 4, std::ios::beg);
        const int endofchain = Sector::ENDOFCHAIN;
        miniFATView.write(reinterpret_cast<const char*>(&endofchain), 4);
    }

    // Update sector chains           ---------------------------------------
    AllocateSectorChain(miniStreamView.BaseSectorChain());
    AllocateSectorChain(miniFATView.BaseSectorChain());

    //Update HEADER and root storage when ministream changes
    if (miniFAT.size() > 0)
    {
        rootStorage->getDirEntry()->setStartSetc(miniStream[0]->id);
        header->miniFATSectorsNumber = miniFAT.size();
        header->firstMiniFATSectorID = miniFAT[0]->id;
    }
}

void CompoundFile::FreeChain(SVector<Sector> &sectorChain, int nth_sector_to_remove, bool zeroSector)
{
    // Dummy zero buffer
    std::vector<char> ZEROED_SECTOR;

    SVector<Sector> FAT = GetSectorChain(-1, SectorType::FAT);

    SList<Sector> zeroQueue;
    StreamView FATView(FAT, GetSectorSize(), FAT.size() * GetSectorSize(), zeroQueue, sourceStream);

    // Zeroes out sector data (if required)-------------
    if (zeroSector)
    {
        for (int i = nth_sector_to_remove; i < (int)sectorChain.size(); i++)
        {
            auto s = sectorChain[i];
            s->ZeroData();
        }
    }

    // Update FAT marking unallocated sectors ----------
    for (int i = nth_sector_to_remove; i < (int)sectorChain.size(); i++)
    {
        int currentId = sectorChain[i]->id;

        FATView.seek(currentId * 4, std::ios::beg);
        const int freesec = Sector::FREESECT;
        FATView.write(reinterpret_cast<const char*>(&freesec), 4);
    }

    // Write new end of chain if partial free ----------
    if (nth_sector_to_remove > 0 && sectorChain.size() > 0)
    {
        FATView.seek(sectorChain[nth_sector_to_remove - 1]->id * 4, std::ios::beg);
        const int endofchain = Sector::ENDOFCHAIN;
        FATView.write(reinterpret_cast<const char*>(&endofchain), 4);
    }
}

void CompoundFile::FreeChain(SVector<Sector> &sectorChain, bool zeroSector)
{
    FreeChain(sectorChain, 0, zeroSector);
}

void CompoundFile::AllocateSectorChain(SVector<Sector> &sectorChain)
{
    for (auto& s : *sectorChain)
    {
        if (s->id == -1)
        {
            sectors.Add(s);
            s->id = sectors.Count() - 1;
        }
    }

    AllocateFATSectorChain(sectorChain);
}

void CompoundFile::AllocateFATSectorChain(SVector<Sector> &sectorChain)
{
    SVector<Sector> fatSectors = GetSectorChain(-1, SectorType::FAT);

    SList<Sector> zeroQueue;
    StreamView fatStream(
                fatSectors,
                GetSectorSize(),
                header->fatSectorsNumber * GetSectorSize(),
                zeroQueue,
                sourceStream,
                true
                );

    // Write FAT chain values --

    for (int i = 0; i < (int)sectorChain.size() - 1; i++)
    {

        auto sN = sectorChain[i + 1];
        auto sC = sectorChain[i];

        fatStream.seek(sC->id * 4, std::ios::beg);
        fatStream.write(reinterpret_cast<const char*>(&(sN->id)), 4);
    }

    fatStream.seek(sectorChain[sectorChain.size() - 1]->id * 4, std::ios::beg);
    const int endofchain = Sector::ENDOFCHAIN;
    fatStream.write(reinterpret_cast<const char*>(&endofchain), 4);

    // Merge chain to CFS
    AllocateDIFATSectorChain(fatStream.BaseSectorChain());
}

void CompoundFile::AllocateDIFATSectorChain(SVector<Sector> &FATsectorChain)
{
    // Get initial sector's count
    header->fatSectorsNumber = FATsectorChain.size();

    // Allocate Sectors
    for (auto& s : *FATsectorChain)
    {
        if (s->id == -1)
        {
            sectors.Add(s);
            s->id = sectors.Count() - 1;
            s->type = SectorType::FAT;
        }
    }

    // Sector count...
    int nCurrentSectors = sectors.Count();

    // Temp DIFAT count
    int nDIFATSectors = (int)header->difatSectorsNumber;

    if ((int)FATsectorChain.size() > HEADER_DIFAT_ENTRIES_COUNT)
    {
        nDIFATSectors = std::ceil((double)(FATsectorChain.size() - HEADER_DIFAT_ENTRIES_COUNT) / DIFAT_SECTOR_FAT_ENTRIES_COUNT);
        nDIFATSectors = LowSaturation(nDIFATSectors - (int)header->difatSectorsNumber); //required DIFAT
    }

    // ...sum with new required DIFAT sectors count
    nCurrentSectors += nDIFATSectors;

    // ReCheck FAT bias
    while (header->fatSectorsNumber * FAT_SECTOR_ENTRIES_COUNT < nCurrentSectors)
    {
        std::shared_ptr<Sector> extraFATSector (new Sector(GetSectorSize(), sourceStream));
        sectors.Add(extraFATSector);

        extraFATSector->id = sectors.Count() - 1;
        extraFATSector->type = SectorType::FAT;

        FATsectorChain.push_back(extraFATSector);

        header->fatSectorsNumber++;
        nCurrentSectors++;

        //... so, adding a FAT sector may induce DIFAT sectors to increase by one
        // and consequently this may induce ANOTHER FAT sector (TO-THINK: May this condition occure ?)
        if (nDIFATSectors * DIFAT_SECTOR_FAT_ENTRIES_COUNT <
                (header->fatSectorsNumber > HEADER_DIFAT_ENTRIES_COUNT ?
                 header->fatSectorsNumber - HEADER_DIFAT_ENTRIES_COUNT :
                 0))
        {
            nDIFATSectors++;
            nCurrentSectors++;
        }
    }


    SVector<Sector> difatSectors = GetSectorChain(-1, SectorType::DIFAT);

    StreamView difatStream(difatSectors, GetSectorSize(), sourceStream);

    // Write DIFAT Sectors (if required)
    // Save room for the following chaining
    for (int i = 0; i < (int)FATsectorChain.size(); i++)
    {
        if (i < HEADER_DIFAT_ENTRIES_COUNT)
        {
            header->difat[i] = FATsectorChain[i]->id;
        }
        else
        {
            // room for DIFAT chaining at the end of any DIFAT sector (4 bytes)
            if (i != HEADER_DIFAT_ENTRIES_COUNT && (i - HEADER_DIFAT_ENTRIES_COUNT) % DIFAT_SECTOR_FAT_ENTRIES_COUNT == 0)
            {
                difatStream.write(reinterpret_cast<const char*>(0L), sizeof(int));
            }

            difatStream.write(reinterpret_cast<const char*>(&FATsectorChain[i]->id), sizeof(int));

        }
    }

    // Allocate room for DIFAT sectors
    for (int i = 0; i < (int)difatStream.BaseSectorChain().size(); i++)
    {
        if (difatStream.BaseSectorChain()[i]->id == -1)
        {
            sectors.Add(difatStream.BaseSectorChain()[i]);
            difatStream.BaseSectorChain()[i]->id = sectors.Count() - 1;
            difatStream.BaseSectorChain()[i]->type = SectorType::DIFAT;
        }
    }

    header->difatSectorsNumber = (uint)nDIFATSectors;


    // Chain first sector
    if (difatStream.BaseSectorChain().size() && difatStream.BaseSectorChain().size() > 0)
    {
        header->firstDIFATSectorID = difatStream.BaseSectorChain()[0]->id;

        // Update header information
        header->difatSectorsNumber = (uint)difatStream.BaseSectorChain().size();

        // Write chaining information at the end of DIFAT Sectors
        for (int i = 0; i < (int)difatStream.BaseSectorChain().size() - 1; i++)
        {
            int ID = difatStream.BaseSectorChain()[i + 1]->id;
            char* src = reinterpret_cast<char *>(&ID);
            char* dst = reinterpret_cast<char *>(difatStream.BaseSectorChain()[i]->GetData().data());
            int offsetDst = GetSectorSize() - sizeof(int);
            std::copy_n(src, sizeof(int), dst+offsetDst);
        }

        char* src = const_cast<char*>(reinterpret_cast<const char*>(Sector::ENDOFCHAIN));
        char* dst =  reinterpret_cast<char *>(difatStream.BaseSectorChain()[difatStream.BaseSectorChain().size() - 1]->GetData().data());
        int offsetDst = GetSectorSize() - sizeof(int);
        std::copy_n(src, sizeof(int), dst+offsetDst);
    }
    else
        header->firstDIFATSectorID = Sector::ENDOFCHAIN;

    // Mark DIFAT Sectors in FAT
    SList<Sector> zeroQueue;
    StreamView fatSv(FATsectorChain, GetSectorSize(), header->fatSectorsNumber * GetSectorSize(), zeroQueue, sourceStream);

    for (int i = 0; i < (int)header->difatSectorsNumber; i++)
    {
        fatSv.seek(difatStream.BaseSectorChain()[i]->id * 4, std::ios::beg);
        const int difsect = Sector::DIFSECT;
        fatSv.write(reinterpret_cast<const char*>(&difsect), 4);
    }

    for (int i = 0; i < header->fatSectorsNumber; i++)
    {
        fatSv.seek(fatSv.BaseSectorChain()[i]->id * 4, std::ios::beg);
        const int fatsect = Sector::FATSECT;
        fatSv.write(reinterpret_cast<const char*>(&fatsect), 4);
    }

    header->fatSectorsNumber = fatSv.BaseSectorChain().size();
}

void CompoundFile::AllocateMiniSectorChain(SVector<Sector> &sectorChain)
{
    SVector<Sector> miniFAT
            = GetSectorChain(header->firstMiniFATSectorID, SectorType::Normal);

    SVector<Sector> miniStream
            = GetSectorChain(RootEntry()->getStartSetc(), SectorType::Normal);

    SList<Sector> zeroQueue;
    StreamView miniFATView(
                miniFAT,
                GetSectorSize(),
                header->miniFATSectorsNumber * Sector::MINISECTOR_SIZE,
                zeroQueue,
                sourceStream,
                true
                );

    StreamView miniStreamView(
                miniStream,
                GetSectorSize(),
                rootStorage->size(),
                zeroQueue,
                sourceStream);


    // Set updated/new sectors within the ministream
    // We are writing data in a NORMAL Sector chain.
    for (int i = 0; i < (int)sectorChain.size(); i++)
    {
        std::shared_ptr<Sector> s = sectorChain[i];

        if (s->id == -1)
        {
            // Allocate, position ministream at the end of already allocated
            // ministream's sectors

            miniStreamView.seek(rootStorage->size() + Sector::MINISECTOR_SIZE, std::ios::beg);
            //miniStreamView.Write(s.GetData(), 0, Sector.MINISECTOR_SIZE);
            s->id = (int)(miniStreamView.getPosition() - Sector::MINISECTOR_SIZE) / Sector::MINISECTOR_SIZE;

            rootStorage->getDirEntry()->setSize(miniStreamView.getLength());
        }
    }

    // Update miniFAT
    for (int i = 0; i < (int)sectorChain.size() - 1; i++)
    {
        int currentId = sectorChain[i]->id;
        int nextId = sectorChain[i + 1]->id;

        miniFATView.seek(currentId * 4, std::ios::beg);
        miniFATView.write(reinterpret_cast<const char*>(&nextId), 4);
    }

    // Write End of Chain in MiniFAT
    miniFATView.seek(sectorChain[sectorChain.size() - 1]->id * SIZE_OF_SID, std::ios::beg);
    const int endofchain = Sector::ENDOFCHAIN;
    miniFATView.write(reinterpret_cast<const char*>(&endofchain), 4);

    // Update sector chains
    AllocateSectorChain(miniStreamView.BaseSectorChain());
    AllocateSectorChain(miniFATView.BaseSectorChain());

    //Update HEADER and root storage when ministream changes
    if (miniFAT.size() > 0)
    {
        rootStorage->getDirEntry()->setStartSetc(miniStream[0]->id);
        header->miniFATSectorsNumber = miniFAT.size();
        header->firstMiniFATSectorID = miniFAT[0]->id;
    }
}

void CompoundFile::PersistMiniStreamToStream(const SVector<Sector> &miniSectorChain)
{
    SVector<Sector> miniStream = GetSectorChain(RootEntry()->getStartSetc(), SectorType::Normal);

    SList<Sector> zeroQueue;
    StreamView miniStreamView(
                miniStream,
                GetSectorSize(),
                rootStorage->size(),
                zeroQueue,
                sourceStream);

    for (auto& s : *miniSectorChain)
    {

        if (s->id == -1)
            throw CFException("Invalid minisector index");

        // Ministream sectors already allocated
        miniStreamView.seek(Sector::MINISECTOR_SIZE * s->id, std::ios::beg);
        miniStreamView.write(reinterpret_cast<const char*>(s->GetData().data()), Sector::MINISECTOR_SIZE);
    }
}

int CompoundFile::LowSaturation(int i)
{
    return i > 0 ? i : 0;
}

void CompoundFile::SetSectorChain(SVector<Sector> sectorChain)
{
    if (sectorChain.size() == 0)
        return;

    SectorType _st = sectorChain[0]->type;

    if (_st == SectorType::Normal)
    {
        AllocateSectorChain(sectorChain);
    }
    else if (_st == SectorType::Mini)
    {
        AllocateMiniSectorChain(sectorChain);
    }
}

CFSVersion CompoundFile::getVersion() const
{
    return (CFSVersion)header->majorVersion;
}

SVector<IDirectoryEntry> &CompoundFile::GetDirectories()
{
    return directoryEntries;
}

void CompoundFile::ResetDirectoryEntry(int sid)
{
    directoryEntries[sid]->SetEntryName(L"");
    directoryEntries[sid]->setLeft({});
    directoryEntries[sid]->setRight({});
    directoryEntries[sid]->setParent({});
    directoryEntries[sid]->setStgType(StgType::StgInvalid);
    directoryEntries[sid]->setStartSetc(DirectoryEntry::ZERO);
    directoryEntries[sid]->setStorageCLSID(GUID());
    directoryEntries[sid]->setSize(0);
    directoryEntries[sid]->setStateBits(0);
    directoryEntries[sid]->setColor(RedBlackTree::RED);
    directoryEntries[sid]->setCreationDate(0);
    directoryEntries[sid]->setModifyDate(0);
}

void CompoundFile::InvalidateDirectoryEntry(int sid)
{
    if (sid >= (int)directoryEntries.size())
        throw CFException("Invalid SID of the directory entry to remove");

    ResetDirectoryEntry(sid);
}

void CompoundFile::FreeAssociatedData(int sid)
{
    // Clear the associated stream (or ministream) if required
    if (directoryEntries[sid]->getSize() > 0) //thanks to Mark Bosold for this !
    {
        if (directoryEntries[sid]->getSize() < header->minSizeStandardStream)
        {
            SVector<Sector> miniChain
                    = GetSectorChain(directoryEntries[sid]->getStartSetc(), SectorType::Mini);
            FreeMiniChain(miniChain, eraseFreeSectors);
        }
        else
        {
            SVector<Sector> chain
                    = GetSectorChain(directoryEntries[sid]->getStartSetc(), SectorType::Normal);
            FreeChain(chain, eraseFreeSectors);
        }
    }
}

void CompoundFile::FreeData(CFStream *stream)
{
    if (stream == nullptr || stream->size() == 0)
        return;

    SVector<Sector> sectorChain;

    if (stream->size() < header->minSizeStandardStream)
    {
        sectorChain = GetSectorChain(stream->dirEntry.lock()->getStartSetc(), SectorType::Mini);
        FreeMiniChain(sectorChain, eraseFreeSectors);
    }
    else
    {
        sectorChain = GetSectorChain(stream->dirEntry.lock()->getStartSetc(), SectorType::Normal);
        FreeChain(sectorChain, 0, eraseFreeSectors);
    }

    stream->dirEntry.lock()->setStartSetc(Sector::ENDOFCHAIN);
    stream->dirEntry.lock()->setSize(0);
}

void CompoundFile::WriteData(std::shared_ptr<CFItem> cfItem, std::streamsize position, const std::vector<BYTE> &buffer)
{
    WriteData(cfItem, buffer, position, 0, buffer.size());
}

void CompoundFile::WriteData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE> &buffer)
{
    WriteData(cfItem, 0, buffer);
}

void CompoundFile::AppendData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE> &buffer)
{
    WriteData(cfItem, cfItem->size(), buffer);
}

void CompoundFile::SetStreamLength(std::shared_ptr<CFItem> cfItem, std::streamsize length)
{
    if (cfItem->size() == length)
        return;

    SectorType newSectorType = SectorType::Normal;
    int newSectorSize = GetSectorSize();

    if (length < header->minSizeStandardStream)
    {
        newSectorType = SectorType::Mini;
        newSectorSize = Sector::MINISECTOR_SIZE;
    }

    SectorType oldSectorType = SectorType::Normal;
    int oldSectorSize = GetSectorSize();

    if (cfItem->size() < header->minSizeStandardStream)
    {
        oldSectorType = SectorType::Mini;
        oldSectorSize = Sector::MINISECTOR_SIZE;
    }

    std::streamsize oldSize = cfItem->size();


    // Get Sector chain and delta size induced by client
    SVector<Sector> sectorChain = GetSectorChain(cfItem->dirEntry.lock()->getStartSetc(), oldSectorType);
    std::streamsize delta = length - cfItem->size();

    // Check for transition ministream -> stream:
    // Only in this case we need to free old sectors,
    // otherwise they will be overwritten.

    bool transitionToMini = false;
    bool transitionToNormal = false;
    SVector<Sector> oldChain;

    if (cfItem->dirEntry.lock()->getStartSetc() != Sector::ENDOFCHAIN)
    {
        if (
                (length < header->minSizeStandardStream && cfItem->dirEntry.lock()->getSize() >= header->minSizeStandardStream)
                || (length >= header->minSizeStandardStream && cfItem->dirEntry.lock()->getSize() < header->minSizeStandardStream)
                )
        {
            if (cfItem->dirEntry.lock()->getSize() < header->minSizeStandardStream)
            {
                transitionToNormal = true;
                oldChain = sectorChain;
            }
            else
            {
                transitionToMini = true;
                oldChain = sectorChain;
            }

            // No transition caused by size change
        }
    }


    SList<Sector> freeList;
    std::shared_ptr<StreamView> sv;

    if (!transitionToMini && !transitionToNormal)   //############  NO TRANSITION
    {
        if (delta > 0) // Enlarging stream...
        {
            if (sectorRecycle)
                freeList = FindFreeSectors(newSectorType); // Collect available free sectors

            sv.reset(new StreamView(sectorChain, newSectorSize, length, freeList, sourceStream));

            //Set up  destination chain
            SetSectorChain(sectorChain);
        }
        else if (delta < 0)  // Reducing size...
        {

            int nSec = (int)std::floor(((double)(std::abs(delta)) / newSectorSize)); //number of sectors to mark as free

            if (newSectorSize == Sector::MINISECTOR_SIZE)
                FreeMiniChain(sectorChain, nSec, eraseFreeSectors);
            else
                FreeChain(sectorChain, nSec, eraseFreeSectors);
        }

        if (sectorChain.size() > 0)
        {
            cfItem->dirEntry.lock()->setStartSetc(sectorChain[0]->id);
            cfItem->dirEntry.lock()->setSize(length);
        }
        else
        {
            cfItem->dirEntry.lock()->setStartSetc(Sector::ENDOFCHAIN);
            cfItem->dirEntry.lock()->setSize(0);
        }

    }
    else if (transitionToMini)                          //############## TRANSITION TO MINISTREAM
    {
        // Transition Normal chain -> Mini chain

        // Collect available MINI free sectors

        if (sectorRecycle)
            freeList = FindFreeSectors(SectorType::Mini);

        SList<Sector> zeroQueue;
        sv.reset(new StreamView(oldChain, oldSectorSize, oldSize, zeroQueue, sourceStream));

        // Reset start sector and size of dir entry
        cfItem->dirEntry.lock()->setStartSetc(Sector::ENDOFCHAIN);
        cfItem->dirEntry.lock()->setSize(0);

        SVector<Sector> newChain = GetMiniSectorChain(Sector::ENDOFCHAIN);
        StreamView destSv(newChain, Sector::MINISECTOR_SIZE, length, freeList, sourceStream);

        // Buffered trimmed copy from old (larger) to new (smaller)
        int cnt = 4096 < length ? 4096 : (int)length;

        std::array<char, 4096> buf;
        buf.fill(0);
        std::streamsize toRead = length;

        //Copy old to new chain
        while (toRead > cnt)
        {
            cnt = sv->read(buf.data(), cnt);
            toRead -= cnt;
            destSv.write(buf.data(), cnt);
        }

        sv->read(buf.data(), (int)toRead);
        destSv.write(buf.data(), (int)toRead);

        //Free old chain
        FreeChain(oldChain, eraseFreeSectors);

        //Set up destination chain
        AllocateMiniSectorChain(destSv.BaseSectorChain());

        // Persist to normal strea
        PersistMiniStreamToStream(destSv.BaseSectorChain());

        //Update dir item
        if (destSv.BaseSectorChain().size() > 0)
        {
            cfItem->dirEntry.lock()->setStartSetc(destSv.BaseSectorChain()[0]->id);
            cfItem->dirEntry.lock()->setSize(length);
        }
        else
        {
            cfItem->dirEntry.lock()->setStartSetc(Sector::ENDOFCHAIN);
            cfItem->dirEntry.lock()->setSize(0);
        }
    }
    else if (transitionToNormal)                        //############## TRANSITION TO NORMAL STREAM
    {
        // Transition Mini chain -> Normal chain

        if (sectorRecycle)
            freeList = FindFreeSectors(SectorType::Normal); // Collect available Normal free sectors

        SList<Sector> zeroQueue;
        sv.reset(new StreamView(oldChain, oldSectorSize, oldSize, zeroQueue, sourceStream));

        SVector<Sector> newChain = GetNormalSectorChain(Sector::ENDOFCHAIN);
        StreamView destSv(newChain, GetSectorSize(), length, freeList, sourceStream);

        int cnt = 256 < length ? 256 : (int)length;

        std::array<char, 256> buf;
        buf.fill(0);
        std::streamsize toRead = std::min(length, cfItem->size());

        //Copy old to new chain
        while (toRead > cnt)
        {
            cnt = sv->read(buf.data(), cnt);
            toRead -= cnt;
            destSv.write(buf.data(), cnt);
        }

        sv->read(buf.data(), (int)toRead);
        destSv.write(buf.data(), (int)toRead);

        //Free old mini chain
        FreeMiniChain(oldChain, eraseFreeSectors);

        //Set up normal destination chain
        AllocateSectorChain(destSv.BaseSectorChain());

        //Update dir item
        if (destSv.BaseSectorChain().size() > 0)
        {
            cfItem->dirEntry.lock()->setStartSetc(destSv.BaseSectorChain()[0]->id);
            cfItem->dirEntry.lock()->setSize(length);
        }
        else
        {
            cfItem->dirEntry.lock()->setStartSetc(Sector::ENDOFCHAIN);
            cfItem->dirEntry.lock()->setSize(0);
        }
    }
}

SList<Sector> CompoundFile::FindFreeSectors(SectorType sType)
{
    SList<Sector> freeList;
    SList<Sector> zeroQueue;

    if (sType == SectorType::Normal)
    {

        SVector<Sector> FatChain = GetSectorChain(-1, SectorType::FAT);

        StreamView fatStream(FatChain, GetSectorSize(), header->fatSectorsNumber * GetSectorSize(), zeroQueue, sourceStream);

        int idx = 0;

        while (idx < sectors.Count())
        {
            int id = fatStream.ReadInt32();

            if (id == Sector::FREESECT)
            {
                if (sectors[idx] == nullptr)
                {
                    std::shared_ptr<Sector> s(new Sector(GetSectorSize(), sourceStream));
                    s->id = idx;
                    sectors[idx] = s;

                }

                freeList.enqueue(sectors[idx]);
            }

            idx++;
        }
    }
    else
    {
        SVector<Sector> miniFAT = GetSectorChain(header->firstMiniFATSectorID, SectorType::Normal);

        StreamView miniFATView(miniFAT, GetSectorSize(), header->miniFATSectorsNumber * Sector::MINISECTOR_SIZE, zeroQueue, sourceStream);

        SVector<Sector> miniStream = GetSectorChain(RootEntry()->getStartSetc(), SectorType::Normal);

        StreamView miniStreamView(miniStream, GetSectorSize(), rootStorage->size(), zeroQueue, sourceStream);

        int idx = 0;

        int nMinisectors = (int)(miniStreamView.getLength() / Sector::MINISECTOR_SIZE);

        while (idx < nMinisectors)
        {
            int nextId = miniFATView.ReadInt32();

            if (nextId == Sector::FREESECT)
            {
                std::shared_ptr<Sector> ms(new Sector(Sector::MINISECTOR_SIZE, sourceStream));

                ms->id = idx;
                ms->type = SectorType::Mini;

                miniStreamView.seek(ms->id * Sector::MINISECTOR_SIZE, std::ios::beg);
                miniStreamView.read(reinterpret_cast<char*>(ms->GetData().data()), Sector::MINISECTOR_SIZE);

                freeList.enqueue(ms);
            }

            idx++;
        }
    }

    return freeList;
}

std::vector<BYTE> CompoundFile::GetData(const CFStream *cFStream)
{
    if (_disposed)
        throw CFDisposedException("Compound File closed: cannot access data");

    std::vector<BYTE> result;

    auto de = cFStream->dirEntry;

    SList<Sector> zeroQueue;
    if (de.lock()->getSize() < header->minSizeStandardStream)
    {
        StreamView miniView(GetSectorChain(
                                de.lock()->getStartSetc(),
                                SectorType::Mini),
                            Sector::MINISECTOR_SIZE,
                            de.lock()->getSize(),
                            zeroQueue,
                            sourceStream);

        result.reserve(de.lock()->getSize());
        miniView.read(reinterpret_cast<char*>(result.data()), result.size());
    }
    else
    {
        StreamView sView(GetSectorChain(de.lock()->getStartSetc(), SectorType::Normal), GetSectorSize(), de.lock()->getSize(), zeroQueue, sourceStream);

        result.reserve((int)de.lock()->getSize());

        sView.read(reinterpret_cast<char*>(result.data()), result.size());

    }

    return result;
}

int CompoundFile::ReadData(CFStream *cFStream, std::streamsize position, std::vector<BYTE> &buffer, int count)
{
    if (count > (int)buffer.size())
        throw std::invalid_argument("count parameter exceeds buffer size");

    auto de = cFStream->dirEntry.lock();

    count = std::min((std::streamsize)(de->getSize() - position), (std::streamsize)count);

    std::shared_ptr<StreamView> sView;

    SList<Sector> zeroQueue;
    if (de->getSize() < header->minSizeStandardStream)
    {
        sView.reset(new StreamView(GetSectorChain(de->getStartSetc(), SectorType::Mini), Sector::MINISECTOR_SIZE, de->getSize(), zeroQueue, sourceStream));
    }
    else
    {
        sView.reset(new StreamView(GetSectorChain(de->getStartSetc(), SectorType::Normal), GetSectorSize(), de->getSize(), zeroQueue, sourceStream));
    }


    sView->seek(position, std::ios::beg);
    int result = sView->read(reinterpret_cast<char*>(buffer.data()), count);

    return result;
}

int CompoundFile::ReadData(CFStream *cFStream, std::streamsize position, std::vector<BYTE> &buffer, int offset, int count)
{
    auto de = cFStream->dirEntry.lock();

    count = std::min((std::streamsize)(buffer.size() - offset), (std::streamsize)count);

    std::shared_ptr<StreamView> sView;

    SList<Sector> zeroQueue;
    if (de->getSize() < header->minSizeStandardStream)
    {
        sView.reset(new StreamView(GetSectorChain(de->getStartSetc(), SectorType::Mini), Sector::MINISECTOR_SIZE, de->getSize(), zeroQueue, sourceStream));
    }
    else
    {
        sView.reset(new StreamView(GetSectorChain(de->getStartSetc(), SectorType::Normal), GetSectorSize(), de->getSize(), zeroQueue, sourceStream));
    }


    sView->seek(position, std::ios::beg);
    int result = sView->read(reinterpret_cast<char*>(buffer.data() + offset), count);

    return result;
}

std::vector<BYTE> CompoundFile::GetDataBySID(int sid)
{
    if (_disposed)
        throw CFDisposedException("Compound File closed: cannot access data");
    if (sid < 0)
        return {};
    std::vector<BYTE> result;
    try
    {
        std::shared_ptr<IDirectoryEntry> de = directoryEntries[sid];
        SList<Sector> zeroQueue;
        if (de->getSize() < header->minSizeStandardStream)
        {
            StreamView miniView(GetSectorChain(de->getStartSetc(), SectorType::Mini), Sector::MINISECTOR_SIZE, de->getSize(), zeroQueue, sourceStream);
            result.resize(de->getSize());
            miniView.read(reinterpret_cast<char*>(result.data()), result.size());
        }
        else
        {
            StreamView sView(GetSectorChain(de->getStartSetc(), SectorType::Normal), GetSectorSize(), de->getSize(), zeroQueue, sourceStream);
            result.resize(de->getSize());
            sView.read(reinterpret_cast<char*>(result.data()), result.size());
        }
    }
    catch (...)
    {
        throw CFException("Cannot get data for SID");
    }
    return result;
}

GUID CompoundFile::getGuidBySID(int sid)
{
    if (_disposed)
        throw CFDisposedException("Compound File closed: cannot access data");
    if (sid < 0)
        throw CFException("Invalid SID");
    std::shared_ptr<IDirectoryEntry> de = directoryEntries[sid];
    return de->getStorageCLSID();
}

GUID CompoundFile::getGuidForStream(int sid)
{
    if (_disposed)
        throw CFDisposedException("Compound File closed: cannot access data");
    if (sid < 0)
        throw CFException("Invalid SID");
    GUID g;
    //find first storage containing a non-zero CLSID before SID in directory structure
    for (int i = sid - 1; i >= 0; i--)
    {
        if (directoryEntries[i]->getStorageCLSID() != g && directoryEntries[i]->getStgType() == StgType::StgStorage)
        {
            return directoryEntries[i]->getStorageCLSID();
        }
    }
    return g;
}

void CompoundFile::WriteData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE> &buffer, std::streamsize position, int offset, int count)
{
    if (cfItem->dirEntry.expired())
        throw CFException("Internal error [cfItem->dirEntry] cannot be null");

    if (buffer.size() == 0) return;

    // Get delta size induced by client
    std::streamsize delta = (position + count) - cfItem->size() < 0 ? 0 : (position + count) - cfItem->size();
    std::streamsize newLength = cfItem->size() + delta;

    SetStreamLength(cfItem, newLength);

    // Calculate NEW sectors SIZE
    SectorType _st = SectorType::Normal;
    int _sectorSize = GetSectorSize();

    if (cfItem->size() < header->minSizeStandardStream)
    {
        _st = SectorType::Mini;
        _sectorSize = Sector::MINISECTOR_SIZE;
    }

    SVector<Sector> sectorChain = GetSectorChain(cfItem->dirEntry.lock()->getStartSetc(), _st);
    SList<Sector> zeroQueue;
    StreamView sv(sectorChain, _sectorSize, newLength, zeroQueue, sourceStream);

    sv.seek(position, std::ios::beg);
    sv.write(reinterpret_cast<const char*>(buffer.data() + offset), count);

    if (cfItem->size() < header->minSizeStandardStream)
    {
        PersistMiniStreamToStream(sv.BaseSectorChain());
    }
}

int CompoundFile::GetSectorSize()
{
    return 2 << (header->sectorShift - 1);
}

void CompoundFile::Dispose(bool disposing)
{
    try
    {
        if (!_disposed)
        {
            std::lock_guard<std::mutex> lock(lockObject);
            {
                if (disposing)
                {
                    // Call from user code...
                    sectors.Clear();

                    rootStorage.reset(); // Some problem releasing resources...
                    header.reset();
                    directoryEntries.clear();
                    fileName.clear();
                }

                if (sourceStream && closeStream && !(configuration & CFSConfiguration::LeaveOpen))
                    sourceStream->close();

            }
        }
        //finally
        _disposed = true;
    }
    catch(...)
    {
        //finally
        _disposed = true;
    }
}

void CompoundFile::CheckForLockSector()
{
    //If transaction lock has been added and not yet allocated in the FAT...
    if (_transactionLockAdded && !_transactionLockAllocated)
    {
        StreamView fatStream(GetFatSectorChain(), GetSectorSize(), sourceStream);

        fatStream.seek(_lockSectorId * 4, std::ios::beg);
        const int endofchain = Sector::ENDOFCHAIN;
        fatStream.write(reinterpret_cast<const char*>(&endofchain), 4);

        _transactionLockAllocated = true;
    }
}

void CompoundFile::LoadFile(std::wstring fileName)
{
    SetFileName(fileName);
    Stream fs;

    try
    {
        fs = OpenFileStream(fileName, updateMode != CFSUpdateMode::ReadOnly);

        Load(fs);

    }
    catch(...)
    {
        if (fs.get() != nullptr)
            fs->close();

        throw;
    }
}

void CompoundFile::SetFileName(std::wstring fileName)
{
    BYTE* pUtf8 = NULL;
    std::streamsize lLen = 0;
    NSFile::CUtf8Converter::GetUtf8StringFromUnicode(fileName.c_str(), fileName.length(), pUtf8, lLen, false);
    this->fileName = std::string(pUtf8, pUtf8 + lLen);
    delete [] pUtf8;

}

void CompoundFile::LoadStream(Stream stream)
{
    if (stream.get() == nullptr)
        throw CFException("Stream parameter cannot be null");

    // Check seekable

    stream->seek(0, std::ios::beg);

    Load(stream);
}
