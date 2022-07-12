#include "compoundfile.h"
#include "cfexception.h"
#include "streamview.h"
#include "../../DesktopEditor/common/File.h"
#include <cmath>
#include <algorithm>


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

CompoundFile::CompoundFile(CFSVersion cfsVersion, CFSConfiguration configFlags) : header((ushort)cfsVersion)
{
    configuration = configFlags;

    sectorRecycle = configFlags & CFSConfiguration::SectorRecycle;
    bool eraseFreeSectors = configFlags & CFSConfiguration::EraseFreeSectors;

    //    if (cfsVersion == CFSVersion::Ver_4)
    //        sectors.OnVer3SizeLimitReached += new Ver3SizeLimitReached(OnSizeLimitReached);


    DIFAT_SECTOR_FAT_ENTRIES_COUNT = (GetSectorSize() / 4) - 1;
    FAT_SECTOR_ENTRIES_COUNT = (GetSectorSize() / 4);

    //Root --
    std::shared_ptr<IDirectoryEntry> rootDir = DirectoryEntry::New("Root Entry", StgType.StgRoot, directoryEntries);
    rootDir.StgColor = StgColor.Black;
    //InsertNewDirectoryEntry(rootDir);

    rootStorage = new CFStorage(this, rootDir);
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
    std::shared_ptr<Sector> rangeLockSector( new Sector(GetSectorSize(), sourceStream));

    rangeLockSector->type = SectorType::RangeLockSector;

    _transactionLockAdded = true;
    _lockSectorId = rangeLockSector->id;
    sectors.Add(rangeLockSector);
}

void CompoundFile::Commit()
{
    Commit(false);
}

void CompoundFile::Commit(bool releaseMemory)
{
    if (_disposed)
        throw new CFDisposedException("Compound File closed: cannot commit data");

    if (updateMode != CFSUpdateMode::Update)
        throw new CFInvalidOperation("Cannot commit data in Read-Only update mode");
#if !defined(FLAT_WRITE)

    int sId = -1;
    int sCount = 0;
    int bufOffset = 0;
#endif
    int sSize = GetSectorSize();

    if (header.majorVersion != (ushort)CFSVersion::Ver_3)
        CheckForLockSector();

    sourceStream->seekp(0, std::ios::beg);

    std::vector<char> zeroArray(sSize, 0);
    sourceStream->write(zeroArray.data(), zeroArray.size());
    zeroArray.clear();

    CommitDirectory();

    bool gap = true;


    for (int i = 0; i < (int)sectors.largeArraySlices.size(); i++)
    {
#if defined (FLAT_WRITE)

        //Note:
        //Here sectors should not be loaded dynamically because
        //if they are null it means that no change has involved them;

        std::shared_ptr<Sector> s = sectors[i];

        if (s.get() != nullptr && s->dirtyFlag)
        {
            if (gap)
                sourceStream->seekp((long)((long)(sSize) + (long)i * (long)sSize), std::ios::beg);

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



#else


        std::shared_ptr<Sector> s = sectors[i];


        if (s.get() != nullptr && s->dirtyFlag && flushingQueue.size() < (int)(buffer.Length / sSize))
        {
            //First of a block of contiguous sectors, mark id, start enqueuing

            if (gap)
            {
                sId = s->id;
                gap = false;
            }

            flushingQueue.push(s);


        }
        else
        {
            //Found a gap, stop enqueuing, flush a write operation

            gap = true;
            sCount = flushingQueue.size();

            if (sCount == 0) continue;

            bufOffset = 0;
            while (flushingQueue.Count > 0)
            {
                Sector r = flushingQueue.Dequeue();
                Buffer.BlockCopy(r.GetData(), 0, buffer, bufOffset, sSize);
                r.DirtyFlag = false;

                if (releaseMemory)
                {
                    r.ReleaseData();
                }

                bufOffset += sSize;
            }

            sourceStream.Seek(((long)sSize + (long)sId * (long)sSize), SeekOrigin.Begin);
            sourceStream.Write(buffer, 0, sCount * sSize);



            //Console.WriteLine("W - " + (int)(sCount * sSize ));

        }
#endif
    }

#if !FLAT_WRITE
    sCount = flushingQueue.Count;
    bufOffset = 0;

    while (flushingQueue.Count > 0)
    {
        Sector r = flushingQueue.Dequeue();
        Buffer.BlockCopy(r.GetData(), 0, buffer, bufOffset, sSize);
        r.DirtyFlag = false;

        if (releaseMemory)
        {
            r.ReleaseData();
            r = null;
        }

        bufOffset += sSize;
    }

    if (sCount != 0)
    {
        sourceStream.Seek((long)sSize + (long)sId * (long)sSize, SeekOrigin.Begin);
        sourceStream.Write(buffer, 0, sCount * sSize);
        //Console.WriteLine("W - " + (int)(sCount * sSize));
    }

#endif

    // Seek to beginning position and save header (first 512 or 4096 bytes)
    sourceStream->seekg(0, std::ios::beg);
    header.Write(sourceStream);

    sourceStream-> SetLength((long)(sectors.Count + 1) * sSize);
    sourceStream->flush();

    if (releaseMemory)
        GC.Collect();

    //}
    //catch (Exception ex)
    //{
    //    throw new CFException("Internal error while committing data", ex);
    //}
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

/// <summary>
/// Load compound file from an existing stream.
/// </summary>
/// <param name="stream">Stream to load compound file from</param>
void CompoundFile::Load(Stream stream)
{
    try
    {
        this->header = Header();
        this.directoryEntries = new List<IDirectoryEntry>();

        this->sourceStream = stream;

        header.Read(stream);

        int n_sector = std::ceil(((double)(Length(stream) - GetSectorSize()) / (double)GetSectorSize()));

        if (Length(stream) > 0x7FFFFF0)
            this->_transactionLockAllocated = true;


        sectors.Clear();
        //sectors = new ArrayList();
        for (int i = 0; i < n_sector; i++)
        {
            sectors.Add({});
        }

        LoadDirectories();

        this.rootStorage
                = new CFStorage(this, directoryEntries[0]);
    }
    catch (...)
    {
        if (stream.get() != nullptr && closeStream)
            //            stream->clear(); // close

            throw;
    }
}

void CompoundFile::Save(std::wstring wFileName)
{
    if (_disposed)
        throw new CFException("Compound File closed: cannot save data");

    BYTE* pUtf8 = NULL;
    LONG lLen = 0;
    NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wFileName.c_str(), wFileName.length(), pUtf8, lLen, false);
    std::string utf8FileName = std::string(pUtf8, pUtf8 + lLen);
    delete [] pUtf8;
    Stream fs(new std::fstream(utf8FileName, std::ios::out));

    try
    {
        Save(fs);
    }
    catch (Exception ex)
    {
        throw new CFException("Error saving file [" + fileName + "]", ex);
    }
    //    finally
    {
        if (fs.get() != nullptr)
            fs->flush();

        if (fs.get() != nullptr)
            static_cast<std::fstream*>(fs.get())->close();

    }
}

void CompoundFile::Save(Stream stream)
{
    if (_disposed)
        throw new CFDisposedException("Compound File closed: cannot save data");

    //    if (!stream.CanSeek)
    //        throw new CFException("Cannot save on a non-seekable stream");

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

        stream->seekp(0, std::ios::beg);
        header.Write(stream);
    }
    catch (std::exception &ex)
    {
        throw new CFException("Internal error while saving compound file to stream ", ex);
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
    while (idx < header.fatSectorsNumber && idx < N_HEADER_FAT_ENTRY)
    {
        nextSecID = header.difat[idx];
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
        std::streamsize stLength = header.fatSectorsNumber > N_HEADER_FAT_ENTRY ?
                    (header.fatSectorsNumber - N_HEADER_FAT_ENTRY) * 4 : 0;
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

        while ((int)result.size() < header.fatSectorsNumber)
        {
            difatStream->Read(nextDIFATSectorBuffer, 0, 4); // IsLittleEndian ?
            nextSecID = *reinterpret_cast<int*>(nextDIFATSectorBuffer);

            EnsureUniqueSectorIndex(nextSecID, processedSectors);

            auto s = sectors[nextSecID];

            if (s.get() == nullptr)
            {
                s.reset(new Sector(GetSectorSize(), sourceStream));
                s->type = SectorType::FAT;
                s->id = nextSecID;
                sectors[nextSecID] = s;//UUU
            }

            result.push_back(s);

            //difatStream.Read(nextDIFATSectorBuffer, 0, 4);
            //nextSecID = BitConverter.ToInt32(nextDIFATSectorBuffer, 0);


            if (difatStream->position == ((GetSectorSize() - 4) + i * GetSectorSize()))
            {
                // Skip DIFAT chain fields considering the possibility that the last FAT entry has been already read
                difatStream->Read(nextDIFATSectorBuffer, 0, 4);
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

    if (header.difatSectorsNumber != 0)
    {
        validationCount = (int)header.difatSectorsNumber;

        std::shared_ptr<Sector> s = sectors[header.firstDIFATSectorID];

        if (s == nullptr) //Lazy loading
        {
            s.reset(new Sector(GetSectorSize(), sourceStream));
            s->type = SectorType::DIFAT;
            s->id = header.firstDIFATSectorID;
            sectors[header.firstDIFATSectorID] = s;
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
                    throw new CFCorruptedFileException("DIFAT sectors count mismatched. Corrupted compound file");
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
            throw new CFCorruptedFileException("Next Sector ID reference is below zero. NextID : " + std::to_string(nextSecID));

        if (nextSecID >= sectors.Count())
            throw new CFCorruptedFileException("Next Sector ID reference an out of range sector. NextID : " + std::to_string(nextSecID) +
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

        fatStream.Seek(nextSecID * 4, std::ios::beg);
        int next = fatStream.ReadInt32();

        EnsureUniqueSectorIndex(next, processedSectors);
        nextSecID = next;

    }


    return result;
}

// TODO
SVector<Sector> CompoundFile::GetMiniSectorChain(int secID)
{
    SVector<Sector> result;

    if (secID != Sector::ENDOFCHAIN)
    {
        int nextSecID = secID;

        SVector<Sector> miniFAT = GetNormalSectorChain(header.firstMiniFATSectorID);
        SVector<Sector> miniStream = GetNormalSectorChain(RootEntry()->getStartSetc());
        SList<Sector> zeroQueue;

        StreamView miniFATView(miniFAT, GetSectorSize(), header.miniFATSectorsNumber * Sector::MINISECTOR_SIZE, zeroQueue, sourceStream);

        // TODO here
        StreamView miniStreamView(miniStream, GetSectorSize(), rootStorage.Size, zeroQueue, sourceStream);

        BinaryReader miniFATReader = new BinaryReader(miniFATView);

        nextSecID = secID;

        std::unordered_set<int> processedSectors;

        while (true)
        {
            if (nextSecID == Sector::ENDOFCHAIN)
                break;

            std::shared_ptr<Sector> ms(new Sector(Sector::MINISECTOR_SIZE, sourceStream));

            ms->id = nextSecID;
            ms->type = SectorType::Mini;

            miniStreamView.Seek(nextSecID * Sector::MINISECTOR_SIZE, std::ios::beg);
            miniStreamView.Read(reinterpret_cast<char*>(ms->GetData().data()), 0, Sector::MINISECTOR_SIZE);

            result.push_back(ms);

            miniFATView.Seek(nextSecID * 4, std::ios::beg);
            int next = miniFATReader.ReadInt32();

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
        throw new CFException("Unsupproted chain type");
    }
}

void CompoundFile::EnsureUniqueSectorIndex(int nextSecID, std::unordered_set<int>& processedSectors)
{
    if (processedSectors.find(nextSecID) != processedSectors.end() && this->validationExceptionEnabled)
    {
        throw new CFCorruptedFileException("The file is corrupted.");
    }

    processedSectors.insert(nextSecID);
}

void CompoundFile::CommitDirectory()
{
    const int DIRECTORY_SIZE = 128;

    auto directorySectors
            = GetSectorChain(header.firstDirectorySectorID, SectorType::Normal);

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

    for (const auto& di : directoryEntries)
    {
        di->Write(sv);
    }

    int delta = directoryEntries.size();

    while (delta % (GetSectorSize() / DIRECTORY_SIZE) != 0)
    {
        std::shared_ptr<IDirectoryEntry> dummy =
                DirectoryEntry::New(L"", StgType::StgInvalid, directoryEntries.cast<IDirectoryEntry>());
        dummy->Write(sv);
        delta++;
    }

    for (auto s : directorySectors)
    {
        s->type = SectorType::Directory;
    }

    AllocateSectorChain(directorySectors);

    header.firstDirectorySectorID = directorySectors[0]->id;

    //Version 4 supports directory sectors count
    if (header.majorVersion == 3)
    {
        header.directorySectorsNumber = 0;
    }
    else
    {
        header.directorySectorsNumber = directorySectors.size();
    }
}

void CompoundFile::Close(bool closeStream)
{

}

std::shared_ptr<IDirectoryEntry> CompoundFile::RootEntry()
{
    if (directoryEntries.empty())
        return {};
    return directoryEntries[0];
}

SVector<IDirectoryEntry> CompoundFile::FindDirectoryEntries(std::wstring entryName)
{
    SVector<IDirectoryEntry> result;

    for (auto d : directoryEntries)
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
    de->setParent({});
    de->setParent({});
}

void CompoundFile::LoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de)
{
    levelSIDs.clear();

    if (de->getLeftSibling() != DirectoryEntry::NOSTREAM)
    {
        // If there're more left siblings load them...
        DoLoadSiblings(bst, directoryEntries[de->getLeftSibling()]);
        //NullifyChildNodes(directoryEntries[de.LeftSibling]);
    }

    if (de->getRightSibling() != DirectoryEntry::NOSTREAM)
    {
        levelSIDs.push_back(de->getRightSibling());

        // If there're more right siblings load them...
        DoLoadSiblings(bst, directoryEntries[de->getRightSibling()]);
        //NullifyChildNodes(directoryEntries[de.RightSibling]);
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
                //this.Close();
                throw new CFCorruptedFileException("A Directory Entry references the non-existent sid number " + std::to_string(sid));
            }
            else
                return false;
        }

        //if this sibling is valid...
        if (directoryEntries[sid]->getStgType() == StgType::StgInvalid)
        {
            if (this->validationExceptionEnabled)
            {
                //this.Close();
                throw new CFCorruptedFileException("A Directory Entry has a valid reference to an Invalid Storage Type directory [" + std::to_string(sid) + "]");
            }
            else
                return false;
        }

        int stgtype = directoryEntries[sid]->getStgType();
        if (false == (stgtype >= 0 && stgtype <= 5))
        {

            if (this->validationExceptionEnabled)
            {
                //this.Close();
                throw new CFCorruptedFileException("A Directory Entry has an invalid Storage Type");
            }
            else
                return false;
        }

        if (std::find(levelSIDs.begin(), levelSIDs.end(), sid) != levelSIDs.end())
            throw new CFCorruptedFileException("Cyclic reference of directory item");

        return true; //No fault condition encountered for sid being validated
    }

    return false;
}

void CompoundFile::LoadDirectories()
{
    SVector<Sector> directoryChain
            = GetSectorChain(header.firstDirectorySectorID, SectorType::Normal);

    if (!(directoryChain.size() > 0))
        throw new CFCorruptedFileException("Directory sector chain MUST contain at least 1 sector");

    if (header.firstDirectorySectorID == Sector::ENDOFCHAIN)
        header.firstDirectorySectorID = directoryChain[0]->id;

    SList<Sector> zeroQueue;
    StreamView dirReader(directoryChain, GetSectorSize(), directoryChain.size() * GetSectorSize(), zeroQueue, sourceStream);


    while (dirReader.position < directoryChain.size() * GetSectorSize())
    {
        std::shared_ptr<IDirectoryEntry> de
                = DirectoryEntry::New(L"", StgType::StgInvalid, directoryEntries.cast<IDirectoryEntry>());

        //We are not inserting dirs. Do not use 'InsertNewDirectoryEntry'
        de->Read(dirReader, getVersion());

    }
}

void CompoundFile::FreeMiniChain(SVector<Sector> &sectorChain, bool zeroSector)
{

}

void CompoundFile::FreeChain(SVector<Sector> &sectorChain, int nth_sector_to_remove, bool zeroSector)
{

}

SVector<DirectoryEntry> CompoundFile::GetDirectories()
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
    directoryEntries[sid]->stateBits = 0;
    directoryEntries[sid]->setColor(RedBlackTree::RED);
    directoryEntries[sid]->setCreationDate(0);
    directoryEntries[sid]->setModifyDate(0);
}

void CompoundFile::InvalidateDirectoryEntry(int sid)
{
    if (sid >= (int)directoryEntries.size())
        throw new CFException("Invalid SID of the directory entry to remove");

    ResetDirectoryEntry(sid);
}

void CompoundFile::FreeAssociatedData(int sid)
{
    // Clear the associated stream (or ministream) if required
    if (directoryEntries[sid]->size > 0) //thanks to Mark Bosold for this !
    {
        if (directoryEntries[sid]->size < header.minSizeStandardStream)
        {
            SVector<Sector> miniChain
                    = GetSectorChain(directoryEntries[sid]->startSetc, SectorType::Mini);
            FreeMiniChain(miniChain, eraseFreeSectors);
        }
        else
        {
            SVector<Sector> chain
                    = GetSectorChain(directoryEntries[sid]->startSetc, SectorType::Normal);
            FreeChain(chain, eraseFreeSectors);
        }
    }
}

int CompoundFile::GetSectorSize()
{
    return 2 << (header.sectorShift - 1);
}

void CompoundFile::CheckForLockSector()
{
    //If transaction lock has been added and not yet allocated in the FAT...
    if (_transactionLockAdded && !_transactionLockAllocated)
    {
        StreamView fatStream(GetFatSectorChain(), GetSectorSize(), sourceStream);

        fatStream.Seek(_lockSectorId * 4, std::ios::beg);
        fatStream.Write(reinterpret_cast<const char*>(&Sector::ENDOFCHAIN), 0, 4);

        _transactionLockAllocated = true;
    }
}

void CompoundFile::LoadFile(std::wstring fileName)
{
    SetFileName(fileName);
    Stream fs;

    try
    {
        NSFile::CFileBinary file;
        file.OpenFile(fileName);
        if (updateMode == CFSUpdateMode::ReadOnly)
        {
            fs.reset(new std::fstream(this->fileName, std::ios::in | std::ios::out));

        }
        else
        {
            fs.reset(new std::fstream(this->fileName, std::ios::in | std::ios::out));
        }

        Load(fs);

    }
    catch(...)
    {
        if (fs.get() != nullptr)
            fs->clear(); // close

        throw;
    }
}

void CompoundFile::SetFileName(std::wstring fileName)
{
    BYTE* pUtf8 = NULL;
    LONG lLen = 0;
    NSFile::CUtf8Converter::GetUtf8StringFromUnicode(fileName.c_str(), fileName.length(), pUtf8, lLen, false);
    this->fileName = std::string(pUtf8, pUtf8 + lLen);
    delete [] pUtf8;

}

void CompoundFile::LoadStream(Stream stream)
{
    if (stream.get() == nullptr)
        throw new CFException("Stream parameter cannot be null");

    if (/*!stream.CanSeek*/false)
        throw new CFException("Cannot load a non-seekable Stream");


    stream->seekp(0, std::ios::beg);

    Load(stream);
}
