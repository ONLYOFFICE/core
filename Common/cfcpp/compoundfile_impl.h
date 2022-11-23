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
#pragma once

#include "compoundfile.h"
#include "sectorcollection.h"
#include "cfstorage.h"
#include "slist.h"
#include <unordered_set>
#include "RBTree/rbtree.h"
#include "idirectoryentry.h"
#include <mutex>
#include "header.h"
#include "fstream"

namespace CFCPP
{
class DirectoryEntry;

class CompoundFile_impl
{
public:
    CompoundFile_impl(const std::wstring &fileName, CFSUpdateMode updateMode, CFSConfiguration configParameters = Default);
    CompoundFile_impl(CFSVersion cfsVersion, CFSConfiguration configFlags);
    CompoundFile_impl(const std::wstring &fileName);
    CompoundFile_impl(Stream stream);
    CompoundFile_impl();
	virtual ~CompoundFile_impl();

    // Main methods
    std::shared_ptr<CFStorage> RootStorage();

    void Save(std::wstring wFileName);
    void Save(Stream stream);


    void Commit(bool releaseMemory = false);
    bool HasSourceStream() const;
    bool ValidationExceptionEnabled() const;
    bool IsClosed()const;
    void Close();

    std::vector<BYTE> GetDataBySID(_INT32 sid);
    _GUID_ getGuidBySID(_INT32 sid);
    _GUID_ getGuidForStream(_INT32 sid);

    // internal methods
    static std::shared_ptr<RedBlackTree::RBTree> CreateNewTree();
    std::shared_ptr<RedBlackTree::RBTree> GetChildrenTree(_INT32 sid);
    SVector<IDirectoryEntry> &GetDirectories();
    void ResetDirectoryEntry(_INT32 sid);
    void InvalidateDirectoryEntry(_INT32 sid);
    void FreeAssociatedData(_INT32 sid);
    void FreeData(CFStream* stream);
    void WriteData(std::shared_ptr<CFItem> cfItem, const char* data, _INT64 position, _INT32 count);
    void WriteData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE>& buffer, _INT64 position, _INT32 offset, _INT32 count);
    void WriteData(std::shared_ptr<CFItem> cfItem, _INT64 position, const std::vector<BYTE>& buffer);
    void WriteData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE>& buffer);
    void AppendData(std::shared_ptr<CFItem> cfItem, const std::vector<BYTE>& buffer);
    void SetStreamLength(std::shared_ptr<CFItem> cfItem, _INT64 length);
    SList<Sector> FindFreeSectors(SectorType sType);
    std::vector<BYTE> GetData(const CFStream *cFStream);
    _INT32 ReadData(CFStream* cFStream, _INT64 position, std::vector<BYTE>& buffer, _INT32 count);
    _INT32 ReadData(CFStream* cFStream, _INT64 position, std::vector<BYTE>& buffer, _INT32 offset, _INT32 count);

protected:
    _INT32 GetSectorSize();
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
    SVector<Sector> GetNormalSectorChain(_INT32 sectorID);
    SVector<Sector> GetMiniSectorChain(_INT32 sectorID);
    SVector<Sector> GetSectorChain(_INT32 sectorID, SectorType chainType);
    void EnsureUniqueSectorIndex(_INT32 nextsectorID, std::unordered_set<_INT32>  &processedSectors);
    void CommitDirectory();
    void Close(bool closeStream);

    std::shared_ptr<IDirectoryEntry> RootEntry();
    SVector<IDirectoryEntry> FindDirectoryEntries(std::wstring entryName);

    std::shared_ptr<RedBlackTree::RBTree> DoLoadChildrenTrusted(std::shared_ptr<IDirectoryEntry> de);
    void DoLoadChildren(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de);
    void NullifyChildNodes(std::shared_ptr<IDirectoryEntry> de);
    void LoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de);
    void DoLoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de);
    bool ValidateSibling(_INT32 sid);
    void LoadDirectories();

    void FreeMiniChain(SVector<Sector>& sectorChain, bool zeroSector);
    void FreeMiniChain(SVector<Sector>& sectorChain, _INT32 nth_sector_to_remove, bool zeroSector);
    void FreeChain(SVector<Sector>& sectorChain, _INT32 nth_sector_to_remove, bool zeroSector);
    void FreeChain(SVector<Sector>& sectorChain, bool zeroSector);

    void AllocateSectorChain(SVector<Sector>& sectorChain);
    void AllocateFATSectorChain(SVector<Sector>& sectorChain);
    void AllocateDIFATSectorChain(SVector<Sector>& FATsectorChain);
    void AllocateMiniSectorChain(SVector<Sector>& sectorChain);
    void PersistMiniStreamToStream(const SVector<Sector>& miniSectorChain);
    static _INT32 LowSaturation(_INT32 x);
    void SetSectorChain(SVector<Sector> sectorChain);

    CFSVersion getVersion() const;

public:
    CFSConfiguration configuration = Default;
    std::unique_ptr<Header> header;
    Stream sourceStream;

private:
    const _INT32 HEADER_DIFAT_ENTRIES_COUNT = 109;
    _INT32 DIFAT_SECTOR_FAT_ENTRIES_COUNT = 127;
    _INT32 FAT_SECTOR_ENTRIES_COUNT = 128;
    const _INT32 SIZE_OF_SID = 4;
    bool sectorRecycle = false;
    bool eraseFreeSectors = false;
    static constexpr _INT32 FLUSHING_QUEUE_SIZE = 6000;
    static constexpr _INT32 FLUSHING_BUFFER_MAX_SIZE = 1024 * 1024 * 16;


    SectorCollection sectors;
    std::fstream stream;
    std::string fileName;
    std::shared_ptr<CFStorage> rootStorage;

    bool closeStream = true;
    bool transactionLockAdded = false;
    _INT32 lockSectorId = -1;
    bool isTransactionLockAllocated = false;
    bool isValidationExceptionEnabled = true;
    bool isDisposed = false;

    CFSUpdateMode updateMode;
    SVector<IDirectoryEntry> directoryEntries;
    std::list<_INT32>  levelSIDs;
    std::mutex lockObject;
};
}
