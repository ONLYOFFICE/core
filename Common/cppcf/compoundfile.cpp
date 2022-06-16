#include "compoundfile.h"
#include "cfexception.h"
#include "streamview.h"
#include "../../DesktopEditor/common/File.h"
#include <cmath>

using namespace CFCPP;

CompoundFile::CompoundFile() :
    CompoundFile(CFSVersion::Ver_3, CFSConfiguration::Default)
{}

CompoundFile::CompoundFile(const std::wstring &fileName, CFSUpdateMode updateMode, CFSConfiguration configParameters)
{
    configuration = configParameters;
    validationExceptionEnabled = !configParameters.HasFlag(CFSConfiguration.NoValidationException);
    sectorRecycle = configParameters.HasFlag(CFSConfiguration.SectorRecycle);
    updateMode = updateMode;
    eraseFreeSectors = configParameters.HasFlag(CFSConfiguration.EraseFreeSectors);

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
    IDirectoryEntry rootDir = DirectoryEntry.New("Root Entry", StgType.StgRoot, directoryEntries);
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
    Sector rangeLockSector(GetSectorSize(), sourceStream);

    rangeLockSector.type = SectorType::RangeLockSector;

    _transactionLockAdded = true;
    _lockSectorId = rangeLockSector.Id();
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

    sourceStream.Seek(0, SeekOrigin.Begin);
    sourceStream.Write((byte[])Array.CreateInstance(typeof(byte), GetSectorSize()), 0, sSize);

    CommitDirectory();

    bool gap = true;


    for (int i = 0; i < sectors.largeArraySlices.size(); i++)
    {
#if FLAT_WRITE

        //Note:
        //Here sectors should not be loaded dynamically because
        //if they are null it means that no change has involved them;

        Sector s = (Sector)sectors[i];

        if (s != null && s.DirtyFlag)
        {
            if (gap)
                sourceStream.Seek((long)((long)(sSize) + (long)i * (long)sSize), SeekOrigin.Begin);

            sourceStream.Write(s.GetData(), 0, sSize);
            sourceStream.Flush();
            s.DirtyFlag = false;
            gap = false;

        }
        else
        {
            gap = true;
        }

        if (s != null && releaseMemory)
        {

            s.ReleaseData();
            s = null;
            sectors[i] = null;
        }



#else


        Sector s = sectors[i] as Sector;


        if (s != null && s.DirtyFlag && flushingQueue.Count < (int)(buffer.Length / sSize))
        {
            //First of a block of contiguous sectors, mark id, start enqueuing

            if (gap)
            {
                sId = s.Id;
                gap = false;
            }

            flushingQueue.Enqueue(s);


        }
        else
        {
            //Found a gap, stop enqueuing, flush a write operation

            gap = true;
            sCount = flushingQueue.Count;

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
    sourceStream.Seek(0, SeekOrigin.Begin);
    header.Write(sourceStream);

    sourceStream.SetLength((long)(sectors.Count + 1) * sSize);
    sourceStream.Flush();

    if (releaseMemory)
        GC.Collect();

    //}
    //catch (Exception ex)
    //{
    //    throw new CFException("Internal error while committing data", ex);
    //}
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
            sectors.Add(null);
        }

        LoadDirectories();

        this.rootStorage
                = new CFStorage(this, directoryEntries[0]);
    }
    catch (Exception)
    {
        if (stream != null && closeStream)
            stream.Close();

        throw;
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

        fatStream.Seek(_lockSectorId * 4, SeekOrigin.Begin);
        fatStream.Write(BitConverter.GetBytes(Sector.ENDOFCHAIN), 0, 4);

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

}
