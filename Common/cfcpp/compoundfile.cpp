/*
* (c) Copyright Ascensio System SIA 2010-2019
*
* This program is a free software product. You can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License (AGPL)
* version 3 as published by the Free Software Foundation. In accordance with
* Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
* that Ascensio System SIA expressly excludes the warranty of non-infringement
* of any third-party rights.
*
* This program is distributed WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
* details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
*
* You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
* street, Riga, Latvia, EU, LV-1050.
*
* The  interactive user interfaces in modified source and object code versions
* of the Program must display Appropriate Legal Notices, as required under
* Section 5 of the GNU AGPL version 3.
*
* Pursuant to Section 7(b) of the License you must retain the original Product
* logo when distributing the program. Pursuant to Section 7(e) we decline to
* grant you any rights under trademark law for use of our trademarks.
*
* All the Product's GUI elements, including illustrations and icon sets, as
* well as technical writing content are licensed under the terms of the
* Creative Commons Attribution-ShareAlike 4.0 International. See the License
* terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
*
*/
#include "compoundfile_impl.h"
#include "directoryentry.h"
#include "cfexception.h"
#include "streamview.h"
#include "../../DesktopEditor/common/File.h"
#include <algorithm>
#include <cmath>
#include <array>
#include "Stream/fstream_utils.h"
#include "Stream/stream_utils.h"
#include "sector.h"

//--------------------------------------------------------------------------------
using namespace CFCPP;

CompoundFile::CompoundFile() : _impl(new CFCPP::CompoundFile_impl())
{
}
CompoundFile::CompoundFile(const std::wstring &fileName, CFSUpdateMode updateMode, int configParameters) :
    _impl(new CFCPP::CompoundFile_impl(fileName, updateMode, configParameters))
{
}
CompoundFile::CompoundFile(CFSVersion cfsVersion, CFSConfiguration configFlags) :
    _impl(new CFCPP::CompoundFile_impl(cfsVersion, configFlags))
{
}
CompoundFile::CompoundFile(const std::wstring &fileName) :
    _impl(new CFCPP::CompoundFile_impl(fileName))
{
}
CompoundFile::CompoundFile(Stream stream) :
    _impl(new CFCPP::CompoundFile_impl(stream))
{
}
std::shared_ptr<CFStorage> CompoundFile::RootStorage()
{
    return _impl->RootStorage();
}
void CompoundFile::Save(std::wstring wFileName)
{
    _impl->Save(wFileName);
}
void CompoundFile::Save(Stream stream)
{
    _impl->Save(stream);
}
void CompoundFile::Commit(bool releaseMemory)
{
    _impl->Commit(releaseMemory);
}
bool CompoundFile::HasSourceStream() const
{
    return _impl->HasSourceStream();
}
bool CompoundFile::ValidationExceptionEnabled() const
{
    return _impl->ValidationExceptionEnabled();
}
bool CompoundFile::IsClosed()const
{
    return _impl->IsClosed();
}
void CompoundFile::Close()
{
    _impl->Close();
}
std::vector<BYTE> CompoundFile::GetDataBySID(_INT32 sid)
{
    return _impl->GetDataBySID(sid);
}
_GUID_ CompoundFile::getGuidBySID(_INT32 sid)
{
    return _impl->getGuidBySID(sid);
}
_GUID_ CompoundFile::getGuidForStream(_INT32 sid)
{
    return _impl->getGuidForStream(sid);
}
//--------------------------------------------------------------------------------
CompoundFile_impl::~CompoundFile_impl()
{
}
CompoundFile_impl::CompoundFile_impl() :
    CompoundFile_impl(CFSVersion::Ver_3, CFSConfiguration::Default)
{}

CompoundFile_impl::CompoundFile_impl(const std::wstring &fileName, CFSUpdateMode updateMode, int configParameters)
{
    configuration = configParameters;
    isValidationExceptionEnabled = !(configParameters & CFSConfiguration::NoValidationException);
    sectorRecycle = configParameters & CFSConfiguration::SectorRecycle;
    this->updateMode = updateMode;
    eraseFreeSectors = configParameters & CFSConfiguration::EraseFreeSectors;

    LoadFile(fileName);

    DIFAT_SECTOR_FAT_ENTRIES_COUNT = (GetSectorSize() / 4) - 1;
    FAT_SECTOR_ENTRIES_COUNT = (GetSectorSize() / 4);
}

CompoundFile_impl::CompoundFile_impl(CFSVersion cfsVersion, CFSConfiguration configFlags) : header(new Header(cfsVersion))
{
    configuration = configFlags;

    sectorRecycle = configFlags & CFSConfiguration::SectorRecycle;
    eraseFreeSectors = configFlags & CFSConfiguration::EraseFreeSectors;

    if (cfsVersion == CFSVersion::Ver_4)
    {
        Ver3SizeLimitReached action = std::bind(&CompoundFile_impl::OnSizeLimitReached, this);
        sectors.OnVer3SizeLimitReached += action;
    }


    DIFAT_SECTOR_FAT_ENTRIES_COUNT = (GetSectorSize() / 4) - 1;
    FAT_SECTOR_ENTRIES_COUNT = (GetSectorSize() / 4);

    std::shared_ptr<IDirectoryEntry> rootDir = DirectoryEntry::New(L"Root Entry", StgType::StgRoot, directoryEntries);
    rootDir->setStgColor(StgColor::Black);

    rootStorage.reset(new CFStorage(this, rootDir));
}

CompoundFile_impl::CompoundFile_impl(const std::wstring &fileName)
{
    sectorRecycle = false;
    updateMode = CFSUpdateMode::ReadOnly;
    eraseFreeSectors = false;

    LoadFile(fileName);

    DIFAT_SECTOR_FAT_ENTRIES_COUNT = (GetSectorSize() / 4) - 1;
    FAT_SECTOR_ENTRIES_COUNT = (GetSectorSize() / 4);
}

CompoundFile_impl::CompoundFile_impl(Stream stream)
{
    LoadStream(stream);

    DIFAT_SECTOR_FAT_ENTRIES_COUNT = (GetSectorSize() / 4) - 1;
    FAT_SECTOR_ENTRIES_COUNT = (GetSectorSize() / 4);
}

void CompoundFile_impl::OnSizeLimitReached()
{
    std::shared_ptr<Sector> rangeLockSector(new Sector(GetSectorSize(), sourceStream));
    sectors.Add(rangeLockSector);

    rangeLockSector->type = SectorType::RangeLockSector;

    transactionLockAdded = true;
    lockSectorId = rangeLockSector->id;
}


void CompoundFile_impl::Commit(bool releaseMemory)
{
    if (isDisposed)
        throw CFDisposedException("Compound File closed: cannot commit data");

    if (updateMode != CFSUpdateMode::Update)
        throw CFInvalidOperation("Cannot commit data in Read-Only update mode");

    _INT32 sectorSize = GetSectorSize();

    if (header->majorVersion != (_UINT16)CFSVersion::Ver_3)
        CheckForLockSector();

    sourceStream->seek(0, std::ios::beg);

    std::vector<char> zeroArray(sectorSize, 0);
    sourceStream->write(zeroArray.data(), zeroArray.size());
    zeroArray.clear();

    CommitDirectory();

    bool gap = true;


    for (_INT32 i = 0; i < (int)sectors.largeArraySlices.size(); i++)
    {
        std::shared_ptr<Sector> sector = sectors[i];

        if (sector.get() != nullptr && sector->dirtyFlag)
        {
            if (gap)
                sourceStream->seek((long)((long)(sectorSize) + (long)i * (long)sectorSize), std::ios::beg);

            sourceStream->write(reinterpret_cast<char*>(sector->GetData().data()), sectorSize);
            sourceStream->flush();
            sector->dirtyFlag = false;
            gap = false;

        }
        else
        {
            gap = true;
        }

        if (sector.get() != nullptr && releaseMemory)
        {

            sector->ReleaseData();
            sector.reset();
            sectors[i].reset();
        }
    }

    sourceStream->seek(0, std::ios::beg);
    header->Write(sourceStream);

    sourceStream->flush();
}

bool CompoundFile_impl::HasSourceStream() const
{
    return sourceStream != nullptr;
}

bool CompoundFile_impl::ValidationExceptionEnabled() const
{
    return isValidationExceptionEnabled;
}

void CompoundFile_impl::Close()
{
    Close(true);
}

std::shared_ptr<RedBlackTree::RBTree> CompoundFile_impl::CreateNewTree()
{
    return std::shared_ptr<RedBlackTree::RBTree>(new RedBlackTree::RBTree);
}

std::shared_ptr<RedBlackTree::RBTree> CompoundFile_impl::GetChildrenTree(_INT32 sid)
{
    std::shared_ptr<RedBlackTree::RBTree> bst(new RedBlackTree::RBTree());

    DoLoadChildren(bst, directoryEntries[sid]);

    return bst;
}

bool CompoundFile_impl::IsClosed() const
{
    return isDisposed;
}

void CompoundFile_impl::Load(Stream stream)
{
    try
    {
        header.reset(new Header);
        directoryEntries.clear();

        this->sourceStream = stream;

        header->Read(stream);

        _INT32 countSectors = std::ceil(((double)(Length(stream) - GetSectorSize()) / (double)GetSectorSize()));

        if (Length(stream) > 0x7FFFFF0)
            this->isTransactionLockAllocated = true;


        sectors.Clear();

        for (_INT32 i = 0; i < countSectors; i++)
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

void CompoundFile_impl::Save(std::wstring wFileName)
{
    if (isDisposed)
        throw CFException("Compound File closed: cannot save data");

    Stream file = OpenFileStream(wFileName, true, true);
    file->seek(0, std::ios::beg);

    try
    {
        Save(file);

        if (file.get() != nullptr)
            file->flush();

        if (file.get() != nullptr)
            file->close();
    }
    catch (std::exception& ex)
    {
        if (file.get() != nullptr)
            file->flush();

        if (file.get() != nullptr)
            file->close();

        throw CFException("Error saving file [" + fileName + "]", ex);
    }
}


void CompoundFile_impl::Save(Stream stream)
{
    if (isDisposed)
        throw CFDisposedException("Compound File closed: cannot save data");

    // todo or not check seekable

    CheckForLockSector();
    _INT32 sectorSize = GetSectorSize();

    try
    {
        std::vector<char> zeroArray(sectorSize, 0);
        stream->write(zeroArray.data(), zeroArray.size());
        zeroArray.clear();

        CommitDirectory();

        for (_INT32 i = 0; i < sectors.Count(); i++)
        {
            auto sector = sectors[i];

            if (sector == nullptr)
            {
                sector.reset(new Sector(sectorSize, sourceStream));
                sector->id = i;
            }


            stream->write(reinterpret_cast<char*>(sector->GetData().data()), sectorSize);
        }

        stream->seek(0, std::ios::beg);
        header->Write(stream);
    }
    catch (std::exception &ex)
    {
        throw CFException("Internal error while saving compound file to Stream ", ex);
    }
}

SVector<Sector> CompoundFile_impl::GetFatSectorChain()
{
    _INT32 N_HEADER_FAT_ENTRY = 109;

    SVector<Sector> result;

    _INT32 nextSectorID = Sector::ENDOFCHAIN;

    SVector<Sector> difatSectors = GetDifatSectorChain();

    _INT32 index = 0;

    while (index < header->fatSectorsNumber && index < N_HEADER_FAT_ENTRY)
    {
        nextSectorID = header->difat[index];
        auto& sector = sectors[nextSectorID];

        if (sector.get() == nullptr)
        {
            sector.reset(new Sector(GetSectorSize(), sourceStream));
            sector->id = nextSectorID;
            sector->type = SectorType::FAT;
            sectors[nextSectorID] = sector;
        }

        result.push_back(sector);

        index++;
    }

    if (difatSectors.size() > 0)
    {
        std::unordered_set<_INT32>  processedSectors;
        _INT64 stLength = header->fatSectorsNumber > N_HEADER_FAT_ENTRY ?
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

        _INT32 i = 0;

        while ((int)result.size() < header->fatSectorsNumber)
        {
            difatStream->read(nextDIFATSectorBuffer, 4); // IsLittleEndian ?
            nextSectorID = *reinterpret_cast<int*>(nextDIFATSectorBuffer);

            EnsureUniqueSectorIndex(nextSectorID, processedSectors);

            auto& sector = sectors[nextSectorID];

            if (sector == nullptr)
            {
                sector.reset(new Sector(GetSectorSize(), sourceStream));
                sector->type = SectorType::FAT;
                sector->id = nextSectorID;
                sectors[nextSectorID] = sector;
            }

            result.push_back(sector);

            if (difatStream->getPosition() == ((GetSectorSize() - 4) + i * GetSectorSize()))
            {
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

SVector<Sector> CompoundFile_impl::GetDifatSectorChain()
{
    _INT32 validationCount = 0;

    SVector<Sector> result;

    _INT32 nextSectorID  = Sector::ENDOFCHAIN;

    std::unordered_set<_INT32>  processedSectors;

    if (header->difatSectorsNumber != 0)
    {
        validationCount = (int)header->difatSectorsNumber;

        auto sector = sectors[header->firstDIFATSectorID];

        if (sector == nullptr)
        {
            sector.reset(new Sector(GetSectorSize(), sourceStream));
            sector->type = SectorType::DIFAT;
            sector->id = header->firstDIFATSectorID;
            sectors[header->firstDIFATSectorID] = sector;
        }

        result.push_back(sector);

        while (true)
        {
            _INT32 startPos = GetSectorSize() - 4;
            nextSectorID = *reinterpret_cast<int*>(sector->GetData().data() + startPos);
            EnsureUniqueSectorIndex(nextSectorID, processedSectors);
            if (nextSectorID == Sector::FREESECT || nextSectorID == Sector::ENDOFCHAIN) break;

            validationCount--;

            if (validationCount < 0)
            {
                if (this->closeStream)
                    this->Close();

                if (this->isValidationExceptionEnabled)
                    throw CFCorruptedFileException("DIFAT sectors count mismatched. Corrupted compound file");
            }

            sector = sectors[nextSectorID];

            if (sector == nullptr)
            {
                sector.reset(new Sector(GetSectorSize(), sourceStream));
                sector->id = nextSectorID;
                sectors[nextSectorID] = sector;
            }

            result.push_back(sector);
        }
    }

    return result;
}

SVector<Sector> CompoundFile_impl::GetNormalSectorChain(_INT32 sectorID)
{
    SVector<Sector> result;

    _INT32 nextSectorID = sectorID;

    SVector<Sector> fatSectors = GetFatSectorChain();
    std::unordered_set<_INT32>  processedSectors;

    SList<Sector> zeroQueue;
    StreamView fatStream(fatSectors, GetSectorSize(), fatSectors.size() * GetSectorSize(), zeroQueue, sourceStream);

    while (true)
    {
        if (nextSectorID == Sector::ENDOFCHAIN) break;

        if (nextSectorID < 0)
            throw CFCorruptedFileException("Next Sector ID reference is below zero. NextID : " + std::to_string(nextSectorID));

        if (nextSectorID >= sectors.Count())
            throw CFCorruptedFileException("Next Sector ID reference an out of range sector. NextID : " + std::to_string(nextSectorID) +
                                               " while sector count " + std::to_string(sectors.Count()));

        auto& sector = sectors[nextSectorID];
        if (sector == nullptr)
        {
            sector.reset(new Sector(GetSectorSize(), sourceStream));
            sector->id = nextSectorID;
            sector->type = SectorType::Normal;
            sectors[nextSectorID] = sector;
        }

        result.push_back(sector);

        fatStream.seek(nextSectorID * 4, std::ios::beg);
        _INT32 next = fatStream.ReadInt32();

        EnsureUniqueSectorIndex(next, processedSectors);
        nextSectorID = next;
    }


    return result;
}

SVector<Sector> CompoundFile_impl::GetMiniSectorChain(_INT32 sectorID)
{
    SVector<Sector> result;

    if (sectorID != Sector::ENDOFCHAIN)
    {
        _INT32 nextSectorID = sectorID;

        SVector<Sector> miniFAT = GetNormalSectorChain(header->firstMiniFATSectorID);
        SVector<Sector> miniStream = GetNormalSectorChain(RootEntry()->getStartSetc());
        SList<Sector> zeroQueue;

        StreamView miniFATView(miniFAT, GetSectorSize(), header->miniFATSectorsNumber * Sector::MINISECTOR_SIZE, zeroQueue, sourceStream);

        StreamView miniStreamView(miniStream, GetSectorSize(), rootStorage->size(), zeroQueue, sourceStream);

        nextSectorID = sectorID;

        std::unordered_set<_INT32>  processedSectors;

        while (true)
        {
            if (nextSectorID == Sector::ENDOFCHAIN)
                break;

            std::shared_ptr<Sector> ms(new Sector(Sector::MINISECTOR_SIZE, sourceStream));

            ms->id = nextSectorID;
            ms->type = SectorType::Mini;

            miniStreamView.seek(nextSectorID * Sector::MINISECTOR_SIZE, std::ios::beg);
            miniStreamView.read(reinterpret_cast<char*>(ms->GetData().data()), Sector::MINISECTOR_SIZE);

            result.push_back(ms);

            miniFATView.seek(nextSectorID * 4, std::ios::beg);
            _INT32 next = miniFATView.ReadInt32();

            nextSectorID = next;
            EnsureUniqueSectorIndex(nextSectorID, processedSectors);
        }
    }

    return result;
}

SVector<Sector> CompoundFile_impl::GetSectorChain(_INT32 sectorID, SectorType chainType)
{
    switch (chainType)
    {
    case SectorType::DIFAT:
        return GetDifatSectorChain();

    case SectorType::FAT:
        return GetFatSectorChain();

    case SectorType::Normal:
        return GetNormalSectorChain(sectorID);

    case SectorType::Mini:
        return GetMiniSectorChain(sectorID);

    default:
        throw CFException("Unsupproted chain type");
    }
}

void CompoundFile_impl::EnsureUniqueSectorIndex(_INT32 nextSectorID, std::unordered_set<_INT32> & processedSectors)
{
    if (processedSectors.find(nextSectorID) != processedSectors.end() && this->isValidationExceptionEnabled)
    {
        throw CFCorruptedFileException("The file is corrupted.");
    }

    processedSectors.insert(nextSectorID);
}

void CompoundFile_impl::CommitDirectory()
{
    const _INT32 DIRECTORY_SIZE = 128;

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

    for (const auto& de : *directoryEntries)
    {
        de->Write(sv);
    }

    _INT32 delta = directoryEntries.size();

    while (delta % (GetSectorSize() / DIRECTORY_SIZE) != 0)
    {
        std::shared_ptr<IDirectoryEntry> dummy =
                DirectoryEntry::New(L"", StgType::StgInvalid, directoryEntries);
        dummy->Write(sv);
        delta++;
    }

    for (auto& sector : *directorySectors)
    {
        sector->type = SectorType::Directory;
    }

    AllocateSectorChain(directorySectors);

    header->firstDirectorySectorID = directorySectors[0]->id;

    if (header->majorVersion == 3)
    {
        header->directorySectorsNumber = 0;
    }
    else
    {
        header->directorySectorsNumber = directorySectors.size();
    }
}

void CompoundFile_impl::Close(bool closeStream)
{
    this->closeStream = closeStream;
    Dispose(closeStream);
}

std::shared_ptr<IDirectoryEntry> CompoundFile_impl::RootEntry()
{
    if (directoryEntries.empty())
        return {};
    return directoryEntries[0];
}

std::shared_ptr<CFStorage> CompoundFile_impl::RootStorage()
{
    return rootStorage;
}

SVector<IDirectoryEntry> CompoundFile_impl::FindDirectoryEntries(std::wstring entryName)
{
    SVector<IDirectoryEntry> result;

    for (auto& de : *directoryEntries)
    {
        if (de->GetEntryName() == entryName && de->getStgType() != StgType::StgInvalid)
            result.push_back(de);
    }

    return result;
}

std::shared_ptr<RedBlackTree::RBTree> CompoundFile_impl::DoLoadChildrenTrusted(std::shared_ptr<IDirectoryEntry> de)
{
    std::shared_ptr<RedBlackTree::RBTree> bst;

    if (de->getChild() != DirectoryEntry::NOSTREAM)
    {
        bst.reset(new RedBlackTree::RBTree(directoryEntries[de->getChild()]));
    }

    return bst;
}

void CompoundFile_impl::DoLoadChildren(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de)
{
    if (de->getChild() != DirectoryEntry::NOSTREAM)
    {
        if (directoryEntries[de->getChild()]->getStgType() == StgType::StgInvalid) return;

        LoadSiblings(bst, directoryEntries[de->getChild()]);
        NullifyChildNodes(std::static_pointer_cast<IDirectoryEntry>(directoryEntries[de->getChild()]));
        bst->Insert(std::static_pointer_cast<IDirectoryEntry>(directoryEntries[de->getChild()]));
    }
}

void CompoundFile_impl::NullifyChildNodes(std::shared_ptr<IDirectoryEntry> de)
{
    de->setParent({});
    de->setLeft({});
    de->setRight({});
}

void CompoundFile_impl::LoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de)
{
    levelSIDs.clear();

    if (de->getLeftSibling() != DirectoryEntry::NOSTREAM)
    {
        DoLoadSiblings(bst, directoryEntries[de->getLeftSibling()]);
    }

    if (de->getRightSibling() != DirectoryEntry::NOSTREAM)
    {
        levelSIDs.push_back(de->getRightSibling());
        DoLoadSiblings(bst, directoryEntries[de->getRightSibling()]);
    }
}

void CompoundFile_impl::DoLoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de)
{
    if (ValidateSibling(de->getLeftSibling()))
    {
        levelSIDs.push_back(de->getLeftSibling());
        DoLoadSiblings(bst, directoryEntries[de->getLeftSibling()]);
    }

    if (ValidateSibling(de->getRightSibling()))
    {
        levelSIDs.push_back(de->getRightSibling());
        DoLoadSiblings(bst, directoryEntries[de->getRightSibling()]);
    }

    NullifyChildNodes(de);
    bst->Insert(de);
}

bool CompoundFile_impl::ValidateSibling(_INT32 sid)
{
    if (sid != DirectoryEntry::NOSTREAM)
    {
        if (sid >= (int)directoryEntries.size())
        {
            if (this->isValidationExceptionEnabled)
            {
                throw CFCorruptedFileException("A Directory Entry references the non-existent sid number " + std::to_string(sid));
            }
            else
                return false;
        }

        if (directoryEntries[sid]->getStgType() == StgType::StgInvalid)
        {
            if (this->isValidationExceptionEnabled)
            {
                throw CFCorruptedFileException("A Directory Entry has a valid reference to an Invalid Storage Type directory [" + std::to_string(sid) + "]");
            }
            else
                return false;
        }

        _INT32 stgtype = directoryEntries[sid]->getStgType();
        if (false == (stgtype >= 0 && stgtype <= 5))
        {

            if (this->isValidationExceptionEnabled)
            {
                throw CFCorruptedFileException("A Directory Entry has an invalid Storage Type");
            }
            else
                return false;
        }

        if (std::find(levelSIDs.begin(), levelSIDs.end(), sid) != levelSIDs.end())
            throw CFCorruptedFileException("Cyclic reference of directory item");

        return true;
    }

    return false;
}

void CompoundFile_impl::LoadDirectories()
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


    while (dirReader->tell() < (_INT64)directoryChain.size() * sectorSize)
    {
        std::shared_ptr<IDirectoryEntry> de(DirectoryEntry::New(L"", StgType::StgInvalid, directoryEntries));
        de->Read(dirReader, getVersion());
    }
}

void CompoundFile_impl::FreeMiniChain(SVector<Sector> &sectorChain, bool zeroSector)
{
    FreeMiniChain(sectorChain,0, zeroSector);
}

void CompoundFile_impl::FreeMiniChain(SVector<Sector> &sectorChain, _INT32 nth_sector_to_remove, bool zeroSector)
{
    std::vector<char> ZEROED_MINI_SECTOR(Sector::MINISECTOR_SIZE, 0);

    SVector<Sector> miniFAT
            = GetSectorChain(header->firstMiniFATSectorID, SectorType::Normal);

    SVector<Sector> miniStream
            = GetSectorChain(RootEntry()->getStartSetc(), SectorType::Normal);

    SList<Sector> zeroQueue;
    StreamView miniFATView(miniFAT, GetSectorSize(), header->miniFATSectorsNumber * Sector::MINISECTOR_SIZE, zeroQueue, sourceStream);

    StreamView miniStreamView(miniStream, GetSectorSize(), rootStorage->size(), zeroQueue, sourceStream);

    if (zeroSector)
    {
        for (_INT32 i = nth_sector_to_remove; i < (int)sectorChain.size(); i++)
        {
            auto& s = sectorChain[i];

            if (s->id != -1)
            {
                miniStreamView.seek(Sector::MINISECTOR_SIZE * s->id, std::ios::beg);
                miniStreamView.write(ZEROED_MINI_SECTOR.data(), Sector::MINISECTOR_SIZE);
            }
        }
    }

    for (_INT32 i = nth_sector_to_remove; i < (int)sectorChain.size(); i++)
    {
        _INT32 currentId = sectorChain[i]->id;

        miniFATView.seek(currentId * 4, std::ios::beg);
        const _INT32 freesec = Sector::FREESECT;
        miniFATView.write(reinterpret_cast<const char*>(&freesec), 4);
    }

    if (nth_sector_to_remove > 0 && sectorChain.size() > 0)
    {
        miniFATView.seek(sectorChain[nth_sector_to_remove - 1]->id * 4, std::ios::beg);
        const _INT32 endofchain = Sector::ENDOFCHAIN;
        miniFATView.write(reinterpret_cast<const char*>(&endofchain), 4);
    }

    AllocateSectorChain(miniStreamView.BaseSectorChain());
    AllocateSectorChain(miniFATView.BaseSectorChain());

    if (miniFAT.size() > 0)
    {
        rootStorage->getDirEntry()->setStartSetc(miniStream[0]->id);
        header->miniFATSectorsNumber = miniFAT.size();
        header->firstMiniFATSectorID = miniFAT[0]->id;
    }
}

void CompoundFile_impl::FreeChain(SVector<Sector> &sectorChain, _INT32 nth_sector_to_remove, bool zeroSector)
{
    SVector<Sector> FAT = GetSectorChain(-1, SectorType::FAT);

    SList<Sector> zeroQueue;
    StreamView FATView(FAT, GetSectorSize(), FAT.size() * GetSectorSize(), zeroQueue, sourceStream);

    if (zeroSector)
    {
        for (_INT32 i = nth_sector_to_remove; i < (int)sectorChain.size(); i++)
        {
            sectorChain[i]->ZeroData();
        }
    }

    for (_INT32 i = nth_sector_to_remove; i < (int)sectorChain.size(); i++)
    {
        _INT32 currentId = sectorChain[i]->id;

        FATView.seek(currentId * 4, std::ios::beg);
        const _INT32 freesec = Sector::FREESECT;
        FATView.write(reinterpret_cast<const char*>(&freesec), 4);
    }

    if (nth_sector_to_remove > 0 && sectorChain.size() > 0)
    {
        FATView.seek(sectorChain[nth_sector_to_remove - 1]->id * 4, std::ios::beg);
        const _INT32 endofchain = Sector::ENDOFCHAIN;
        FATView.write(reinterpret_cast<const char*>(&endofchain), 4);
    }
}

void CompoundFile_impl::FreeChain(SVector<Sector> &sectorChain, bool zeroSector)
{
    FreeChain(sectorChain, 0, zeroSector);
}

void CompoundFile_impl::AllocateSectorChain(SVector<Sector> &sectorChain)
{
    for (auto& sector : *sectorChain)
    {
        if (sector->id == -1)
        {
            sectors.Add(sector);
            sector->id = sectors.Count() - 1;
        }
    }

    AllocateFATSectorChain(sectorChain);
}

void CompoundFile_impl::AllocateFATSectorChain(SVector<Sector> &sectorChain)
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


    for (_INT32 i = 0; i < (int)sectorChain.size() - 1; i++)
    {

        auto sN = sectorChain[i + 1];
        auto sC = sectorChain[i];

        fatStream.seek(sC->id * 4, std::ios::beg);
        fatStream.write(reinterpret_cast<const char*>(&(sN->id)), 4);
    }

    fatStream.seek(sectorChain[sectorChain.size() - 1]->id * 4, std::ios::beg);
    const _INT32 endofchain = Sector::ENDOFCHAIN;
    fatStream.write(reinterpret_cast<const char*>(&endofchain), 4);

    AllocateDIFATSectorChain(fatStream.BaseSectorChain());
}

void CompoundFile_impl::AllocateDIFATSectorChain(SVector<Sector> &FATsectorChain)
{
    header->fatSectorsNumber = FATsectorChain.size();

    for (auto& sector : *FATsectorChain)
    {
        if (sector->id == -1)
        {
            sectors.Add(sector);
            sector->id = sectors.Count() - 1;
            sector->type = SectorType::FAT;
        }
    }

    _INT32 nCurrentSectors = sectors.Count();

    _INT32 countDIFATSectors = (int)header->difatSectorsNumber;

    if ((int)FATsectorChain.size() > HEADER_DIFAT_ENTRIES_COUNT)
    {
        countDIFATSectors = std::ceil((double)(FATsectorChain.size() - HEADER_DIFAT_ENTRIES_COUNT) / DIFAT_SECTOR_FAT_ENTRIES_COUNT);
        countDIFATSectors = LowSaturation(countDIFATSectors - (int)header->difatSectorsNumber);
    }

    nCurrentSectors += countDIFATSectors;

    while (header->fatSectorsNumber * FAT_SECTOR_ENTRIES_COUNT < nCurrentSectors)
    {
        std::shared_ptr<Sector> extraFATSector (new Sector(GetSectorSize(), sourceStream));
        sectors.Add(extraFATSector);

        extraFATSector->id = sectors.Count() - 1;
        extraFATSector->type = SectorType::FAT;

        FATsectorChain.push_back(extraFATSector);

        header->fatSectorsNumber++;
        nCurrentSectors++;

        if (countDIFATSectors * DIFAT_SECTOR_FAT_ENTRIES_COUNT <
                (header->fatSectorsNumber > HEADER_DIFAT_ENTRIES_COUNT ?
                 header->fatSectorsNumber - HEADER_DIFAT_ENTRIES_COUNT :
                 0))
        {
            countDIFATSectors++;
            nCurrentSectors++;
        }
    }


    SVector<Sector> difatSectors = GetSectorChain(-1, SectorType::DIFAT);

    StreamView difatStream(difatSectors, GetSectorSize(), sourceStream);

    for (_INT32 i = 0; i < (int)FATsectorChain.size(); i++)
    {
        if (i < HEADER_DIFAT_ENTRIES_COUNT)
        {
            header->difat[i] = FATsectorChain[i]->id;
        }
        else
        {
            if (i != HEADER_DIFAT_ENTRIES_COUNT && (i - HEADER_DIFAT_ENTRIES_COUNT) % DIFAT_SECTOR_FAT_ENTRIES_COUNT == 0)
            {
                _INT32 zero = 0;
                difatStream.write(reinterpret_cast<const char*>(&zero), sizeof(int));
            }

            difatStream.write(reinterpret_cast<const char*>(&FATsectorChain[i]->id), sizeof(int));

        }
    }

    for (_INT32 i = 0; i < (int)difatStream.BaseSectorChain().size(); i++)
    {
        if (difatStream.BaseSectorChain()[i]->id == -1)
        {
            sectors.Add(difatStream.BaseSectorChain()[i]);
            difatStream.BaseSectorChain()[i]->id = sectors.Count() - 1;
            difatStream.BaseSectorChain()[i]->type = SectorType::DIFAT;
        }
    }

    header->difatSectorsNumber = (_UINT16)countDIFATSectors;


    if (difatStream.BaseSectorChain() != nullptr && difatStream.BaseSectorChain().size() > 0)
    {
        header->firstDIFATSectorID = difatStream.BaseSectorChain()[0]->id;

        header->difatSectorsNumber = (_UINT16)difatStream.BaseSectorChain().size();

        for (_INT32 i = 0; i < (int)difatStream.BaseSectorChain().size() - 1; i++)
        {
            _INT32 ID = difatStream.BaseSectorChain()[i + 1]->id;
            char* src = reinterpret_cast<char *>(&ID);
            char* dst = reinterpret_cast<char *>(difatStream.BaseSectorChain()[i]->GetData().data());
            _INT32 offsetDst = GetSectorSize() - sizeof(int);
            std::copy_n(src, sizeof(int), dst+offsetDst);
        }

        auto eoc = Sector::ENDOFCHAIN;
        char* src = reinterpret_cast<char*>(&eoc);
        char* dst =  reinterpret_cast<char *>(difatStream.BaseSectorChain()[difatStream.BaseSectorChain().size() - 1]->GetData().data());
        _INT32 offsetDst = GetSectorSize() - sizeof(int);
        std::copy_n(src, sizeof(int), dst+offsetDst);
    }
    else
    {
        header->firstDIFATSectorID = Sector::ENDOFCHAIN;
    }

    SList<Sector> zeroQueue;
    StreamView fatSv(FATsectorChain, GetSectorSize(), header->fatSectorsNumber * GetSectorSize(), zeroQueue, sourceStream);

    for (_INT32 i = 0; i < (int)header->difatSectorsNumber; i++)
    {
        fatSv.seek(difatStream.BaseSectorChain()[i]->id * 4, std::ios::beg);
        const _INT32 difsect = Sector::DIFSECT;
        fatSv.write(reinterpret_cast<const char*>(&difsect), 4);
    }

    for (_INT32 i = 0; i < header->fatSectorsNumber; i++)
    {
        fatSv.seek(fatSv.BaseSectorChain()[i]->id * 4, std::ios::beg);
        const _INT32 fatsect = Sector::FATSECT;
        fatSv.write(reinterpret_cast<const char*>(&fatsect), 4);
    }

    header->fatSectorsNumber = fatSv.BaseSectorChain().size();
}

void CompoundFile_impl::AllocateMiniSectorChain(SVector<Sector> &sectorChain)
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

    for (_INT32 i = 0; i < (int)sectorChain.size(); i++)
    {
        auto& sector = sectorChain[i];

        if (sector->id == -1)
        {
            miniStreamView.seek(rootStorage->size() + Sector::MINISECTOR_SIZE, std::ios::beg);
            sector->id = (int)(miniStreamView.getPosition() - Sector::MINISECTOR_SIZE) / Sector::MINISECTOR_SIZE;

            rootStorage->getDirEntry()->setSize(miniStreamView.getLength());
        }
    }

    for (_INT32 i = 0; i < (int)sectorChain.size() - 1; i++)
    {
        _INT32 currentId = sectorChain[i]->id;
        _INT32 nextId = sectorChain[i + 1]->id;

        miniFATView.seek(currentId * 4, std::ios::beg);
        miniFATView.write(reinterpret_cast<const char*>(&nextId), 4);
    }

    miniFATView.seek(sectorChain[sectorChain.size() - 1]->id * SIZE_OF_SID, std::ios::beg);
    const _INT32 endofchain = Sector::ENDOFCHAIN;
    miniFATView.write(reinterpret_cast<const char*>(&endofchain), 4);

    AllocateSectorChain(miniStreamView.BaseSectorChain());
    AllocateSectorChain(miniFATView.BaseSectorChain());

    if (miniFAT.size() > 0)
    {
        rootStorage->getDirEntry()->setStartSetc(miniStream[0]->id);
        header->miniFATSectorsNumber = miniFAT.size();
        header->firstMiniFATSectorID = miniFAT[0]->id;
    }
}

void CompoundFile_impl::PersistMiniStreamToStream(const SVector<Sector> &miniSectorChain)
{
    SVector<Sector> miniStream = GetSectorChain(RootEntry()->getStartSetc(), SectorType::Normal);

    SList<Sector> zeroQueue;
    StreamView miniStreamView(
                miniStream,
                GetSectorSize(),
                rootStorage->size(),
                zeroQueue,
                sourceStream);

    for (auto& sector : *miniSectorChain)
    {

        if (sector->id == -1)
            throw CFException("Invalid minisector index");

        miniStreamView.seek(Sector::MINISECTOR_SIZE * sector->id, std::ios::beg);
        miniStreamView.write(reinterpret_cast<const char*>(sector->GetData().data()), Sector::MINISECTOR_SIZE);
    }
}

_INT32 CompoundFile_impl::LowSaturation(_INT32 x)
{
    return x > 0 ? x : 0;
}

void CompoundFile_impl::SetSectorChain(SVector<Sector> sectorChain)
{
    if (sectorChain != nullptr && sectorChain.size() == 0)
        return;

    SectorType st = sectorChain[0]->type;

    if (st == SectorType::Normal)
    {
        AllocateSectorChain(sectorChain);
    }
    else if (st == SectorType::Mini)
    {
        AllocateMiniSectorChain(sectorChain);
    }
}

CFSVersion CompoundFile_impl::getVersion() const
{
    return (CFSVersion)header->majorVersion;
}

SVector<IDirectoryEntry> &CompoundFile_impl::GetDirectories()
{
    return directoryEntries;
}

void CompoundFile_impl::ResetDirectoryEntry(_INT32 sid)
{
    directoryEntries[sid]->SetEntryName(L"");
    directoryEntries[sid]->setLeft({});
    directoryEntries[sid]->setRight({});
    directoryEntries[sid]->setParent({});
    directoryEntries[sid]->setStgType(StgType::StgInvalid);
    directoryEntries[sid]->setStartSetc(DirectoryEntry::ZERO);
    directoryEntries[sid]->setStorageCLSID(_GUID_());
    directoryEntries[sid]->setSize(0);
    directoryEntries[sid]->setStateBits(0);
    directoryEntries[sid]->setColor(RedBlackTree::RED);
    directoryEntries[sid]->setCreationDate(0);
    directoryEntries[sid]->setModifyDate(0);
}

void CompoundFile_impl::InvalidateDirectoryEntry(_INT32 sid)
{
    if (sid >= (int)directoryEntries.size())
        throw CFException("Invalid SID of the directory entry to remove");

    ResetDirectoryEntry(sid);
}

void CompoundFile_impl::FreeAssociatedData(_INT32 sid)
{
    if (directoryEntries[sid]->getSize() > 0)
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

void CompoundFile_impl::FreeData(CFStream *stream)
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

void CompoundFile_impl::WriteData(std::shared_ptr<CFItem> cfItem, _INT64 position, const std::vector<BYTE> &buffer)
{
    WriteData(cfItem, buffer, position, 0, buffer.size());
}

void CompoundFile_impl::WriteData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE> &buffer)
{
    WriteData(cfItem, 0, buffer);
}

void CompoundFile_impl::AppendData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE> &buffer)
{
    WriteData(cfItem, cfItem->size(), buffer);
}

void CompoundFile_impl::SetStreamLength(std::shared_ptr<CFItem> cfItem, _INT64 length)
{
    if (cfItem->size() == length)
        return;

    SectorType newSectorType = SectorType::Normal;
    _INT32 newSectorSize = GetSectorSize();

    if (length < header->minSizeStandardStream)
    {
        newSectorType = SectorType::Mini;
        newSectorSize = Sector::MINISECTOR_SIZE;
    }

    SectorType oldSectorType = SectorType::Normal;
    _INT32 oldSectorSize = GetSectorSize();

    if (cfItem->size() < header->minSizeStandardStream)
    {
        oldSectorType = SectorType::Mini;
        oldSectorSize = Sector::MINISECTOR_SIZE;
    }


    _INT64 oldSize = cfItem->size();
    SVector<Sector> sectorChain = GetSectorChain(cfItem->dirEntry.lock()->getStartSetc(), oldSectorType);
    _INT64 delta = length - cfItem->size();

    bool transitionToMini = false;
    bool transitionToNormal = false;
    SVector<Sector> oldChain(false);

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
        }
    }


    SList<Sector> freeList;
    std::shared_ptr<StreamView> sv;

    if (!transitionToMini && !transitionToNormal)
    {
        if (delta > 0)
        {
            if (sectorRecycle)
                freeList = FindFreeSectors(newSectorType);

            sv.reset(new StreamView(sectorChain, newSectorSize, length, freeList, sourceStream));

            SetSectorChain(sectorChain);
        }
        else if (delta < 0)
        {

            _INT32 nSec = (int)std::floor(((double)(std::abs(delta)) / newSectorSize));

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
    else if (transitionToMini)
    {
        if (sectorRecycle)
            freeList = FindFreeSectors(SectorType::Mini);

        SList<Sector> zeroQueue;
        sv.reset(new StreamView(oldChain, oldSectorSize, oldSize, zeroQueue, sourceStream));

        cfItem->dirEntry.lock()->setStartSetc(Sector::ENDOFCHAIN);
        cfItem->dirEntry.lock()->setSize(0);

        SVector<Sector> newChain = GetMiniSectorChain(Sector::ENDOFCHAIN);
        StreamView destSv(newChain, Sector::MINISECTOR_SIZE, length, freeList, sourceStream);

        _INT32 cnt = 4096 < length ? 4096 : (int)length;

        std::array<char, 4096> buffer;
        buffer.fill(0);
        _INT64 toRead = length;

        while (toRead > cnt)
        {
            cnt = sv->read(buffer.data(), cnt);
            toRead -= cnt;
            destSv.write(buffer.data(), cnt);
        }

        sv->read(buffer.data(), (int)toRead);
        destSv.write(buffer.data(), (int)toRead);

        FreeChain(oldChain, eraseFreeSectors);

        AllocateMiniSectorChain(destSv.BaseSectorChain());

        PersistMiniStreamToStream(destSv.BaseSectorChain());

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
    else if (transitionToNormal)
    {
        if (sectorRecycle)
            freeList = FindFreeSectors(SectorType::Normal);

        SList<Sector> zeroQueue;
        sv.reset(new StreamView(oldChain, oldSectorSize, oldSize, zeroQueue, sourceStream));

        SVector<Sector> newChain = GetNormalSectorChain(Sector::ENDOFCHAIN);
        StreamView destSv(newChain, GetSectorSize(), length, freeList, sourceStream);

        _INT32 count = 256 < length ? 256 : (int)length;

        std::array<char, 256> buf;
        buf.fill(0);
        _INT64 toRead = (std::min)(length, cfItem->size());

        while (toRead > count)
        {
            count = sv->read(buf.data(), count);
            toRead -= count;
            destSv.write(buf.data(), count);
        }

        sv->read(buf.data(), (int)toRead);
        destSv.write(buf.data(), (int)toRead);

        FreeMiniChain(oldChain, eraseFreeSectors);

        AllocateSectorChain(destSv.BaseSectorChain());

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

SList<Sector> CompoundFile_impl::FindFreeSectors(SectorType sType)
{
    SList<Sector> freeList;
    SList<Sector> zeroQueue;

    if (sType == SectorType::Normal)
    {
        SVector<Sector> FatChain = GetSectorChain(-1, SectorType::FAT);

        StreamView fatStream(FatChain, GetSectorSize(), header->fatSectorsNumber * GetSectorSize(), zeroQueue, sourceStream);

        _INT32 index = 0;

        while (index < sectors.Count())
        {
            _INT32 id = fatStream.ReadInt32();

            if (id == Sector::FREESECT)
            {
                if (sectors[index] == nullptr)
                {
                    std::shared_ptr<Sector> sector(new Sector(GetSectorSize(), sourceStream));
                    sector->id = index;
                    sectors[index] = sector;
                }

                freeList.enqueue(sectors[index]);
            }

            index++;
        }
    }
    else
    {
        SVector<Sector> miniFAT = GetSectorChain(header->firstMiniFATSectorID, SectorType::Normal);

        StreamView miniFATView(miniFAT, GetSectorSize(), header->miniFATSectorsNumber * Sector::MINISECTOR_SIZE, zeroQueue, sourceStream);

        SVector<Sector> miniStream = GetSectorChain(RootEntry()->getStartSetc(), SectorType::Normal);

        StreamView miniStreamView(miniStream, GetSectorSize(), rootStorage->size(), zeroQueue, sourceStream);

        _INT32 index = 0;

        _INT32 countMinisectors = (int)(miniStreamView.getLength() / Sector::MINISECTOR_SIZE);

        while (index < countMinisectors)
        {
            _INT32 nextId = miniFATView.ReadInt32();

            if (nextId == Sector::FREESECT)
            {
                std::shared_ptr<Sector> ms(new Sector(Sector::MINISECTOR_SIZE, sourceStream));

                ms->id = index;
                ms->type = SectorType::Mini;

                miniStreamView.seek(ms->id * Sector::MINISECTOR_SIZE, std::ios::beg);
                miniStreamView.read(reinterpret_cast<char*>(ms->GetData().data()), Sector::MINISECTOR_SIZE);

                freeList.enqueue(ms);
            }

            index++;
        }
    }

    return freeList;
}

std::vector<BYTE> CompoundFile_impl::GetData(const CFStream *cFStream)
{
    if (isDisposed)
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

_INT32 CompoundFile_impl::ReadData(CFStream *cFStream, _INT64 position, std::vector<BYTE> &buffer, _INT32 count)
{
    if (count > (int)buffer.size())
        throw std::invalid_argument("count parameter exceeds buffer size");

    auto de = cFStream->dirEntry.lock();

    count = std::min((_INT64)(de->getSize() - position), (_INT64)count);

    std::shared_ptr<StreamView> sv;

    SList<Sector> zeroQueue;
    if (de->getSize() < header->minSizeStandardStream)
    {
        sv.reset(new StreamView(GetSectorChain(de->getStartSetc(), SectorType::Mini), Sector::MINISECTOR_SIZE, de->getSize(), zeroQueue, sourceStream));
    }
    else
    {
        sv.reset(new StreamView(GetSectorChain(de->getStartSetc(), SectorType::Normal), GetSectorSize(), de->getSize(), zeroQueue, sourceStream));
    }


    sv->seek(position, std::ios::beg);
    _INT32 result = sv->read(reinterpret_cast<char*>(buffer.data()), count);

    return result;
}

_INT32 CompoundFile_impl::ReadData(CFStream *cFStream, _INT64 position, std::vector<BYTE> &buffer, _INT32 offset, _INT32 count)
{
    auto de = cFStream->dirEntry.lock();

    count = std::min((_INT64)(buffer.size() - offset), (_INT64)count);

    std::shared_ptr<StreamView> sv;

    SList<Sector> zeroQueue;
    if (de->getSize() < header->minSizeStandardStream)
    {
        sv.reset(new StreamView(GetSectorChain(de->getStartSetc(), SectorType::Mini), Sector::MINISECTOR_SIZE, de->getSize(), zeroQueue, sourceStream));
    }
    else
    {
        sv.reset(new StreamView(GetSectorChain(de->getStartSetc(), SectorType::Normal), GetSectorSize(), de->getSize(), zeroQueue, sourceStream));
    }


    sv->seek(position, std::ios::beg);
    _INT32 result = sv->read(reinterpret_cast<char*>(buffer.data() + offset), count);

    return result;
}

std::vector<BYTE> CompoundFile_impl::GetDataBySID(_INT32 sid)
{
    if (isDisposed)
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
            StreamView sv(GetSectorChain(de->getStartSetc(), SectorType::Normal), GetSectorSize(), de->getSize(), zeroQueue, sourceStream);
            result.resize(de->getSize());
            sv.read(reinterpret_cast<char*>(result.data()), result.size());
        }
    }
    catch (...)
    {
        throw CFException("Cannot get data for SID");
    }

    return result;
}

_GUID_ CompoundFile_impl::getGuidBySID(_INT32 sid)
{
    if (isDisposed)
        throw CFDisposedException("Compound File closed: cannot access data");
    if (sid < 0)
        throw CFException("Invalid SID");
    std::shared_ptr<IDirectoryEntry> de = directoryEntries[sid];
    return de->getStorageCLSID();
}

_GUID_ CompoundFile_impl::getGuidForStream(_INT32 sid)
{
    if (isDisposed)
        throw CFDisposedException("Compound File closed: cannot access data");
    if (sid < 0)
        throw CFException("Invalid SID");

	_GUID_ guid;

    for (_INT32 i = sid - 1; i >= 0; i--)
    {
        if (directoryEntries[i]->getStorageCLSID() != guid && directoryEntries[i]->getStgType() == StgType::StgStorage)
        {
            return directoryEntries[i]->getStorageCLSID();
        }
    }

    return guid;
}

void CompoundFile_impl::WriteData(std::shared_ptr<CFItem> cfItem, const char* data, _INT64 position, _INT32 count)
{
    if (data == nullptr)
        throw CFInvalidOperation("Parameter [data] cannot be null");

    if (cfItem->dirEntry.expired())
        throw CFException("Internal error [cfItem->dirEntry] cannot be null");

    if (count == 0) return;

    _INT64 delta = (position + count) - cfItem->size() < 0 ? 0 : (position + count) - cfItem->size();
    _INT64 newLength = cfItem->size() + delta;

    SetStreamLength(cfItem, newLength);

    SectorType _st = SectorType::Normal;
    _INT32 _sectorSize = GetSectorSize();

    if (cfItem->size() < header->minSizeStandardStream)
    {
        _st = SectorType::Mini;
        _sectorSize = Sector::MINISECTOR_SIZE;
    }

    SVector<Sector> sectorChain = GetSectorChain(cfItem->dirEntry.lock()->getStartSetc(), _st);
    SList<Sector> zeroQueue;
    StreamView sv(sectorChain, _sectorSize, newLength, zeroQueue, sourceStream);

    sv.seek(position, std::ios::beg);
    sv.write(data, count);

    if (cfItem->size() < header->minSizeStandardStream)
    {
        PersistMiniStreamToStream(sv.BaseSectorChain());
    }
}

void CompoundFile_impl::WriteData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE> &buffer, _INT64 position, _INT32 offset, _INT32 count)
{
    WriteData(cfItem, reinterpret_cast<const char*>(buffer.data() + offset), position, count);
}

_INT32 CompoundFile_impl::GetSectorSize()
{
    return 2 << (header->sectorShift - 1);
}
void CompoundFile_impl::Dispose(bool disposing)
{
    try
    {
        if (!isDisposed)
        {
            std::lock_guard<std::mutex> lock(lockObject);
            {
                if (disposing)
                {
                    sectors.Clear();

                    rootStorage.reset();
                    header.reset();
                    directoryEntries.clear();
                    fileName.clear();
                }

                if (sourceStream && closeStream && !(configuration & CFSConfiguration::LeaveOpen))
                    sourceStream->close();

            }
        }
        isDisposed = true;
    }
    catch(...)
    {
        isDisposed = true;
    }
}

void CompoundFile_impl::CheckForLockSector()
{
    if (transactionLockAdded && !isTransactionLockAllocated)
    {
        StreamView fatStream(GetFatSectorChain(), GetSectorSize(), sourceStream);

        fatStream.seek(lockSectorId * 4, std::ios::beg);
        const _INT32 endOfChain = Sector::ENDOFCHAIN;
        fatStream.write(reinterpret_cast<const char*>(&endOfChain), 4);

        isTransactionLockAllocated = true;
    }
}

void CompoundFile_impl::LoadFile(std::wstring fileName)
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

void CompoundFile_impl::SetFileName(std::wstring fileName)
{
    BYTE* pUtf8 = NULL;
    LONG lLen = 0;
    NSFile::CUtf8Converter::GetUtf8StringFromUnicode(fileName.c_str(), fileName.length(), pUtf8, lLen, false);
    this->fileName = std::string(pUtf8, pUtf8 + lLen);
    delete [] pUtf8;
}

void CompoundFile_impl::LoadStream(Stream stream)
{
    if (stream.get() == nullptr)
        throw CFException("Stream parameter cannot be null");

    // todo or not: check seekable

    stream->seek(0, std::ios::beg);

    Load(stream);
}
