#include "../HtmlFile.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/graphics/BaseThread.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

class CCounter
{
private:
    NSCriticalSection::CRITICAL_SECTION m_oCS;
    int m_lCounter;

public:
    CCounter()
    {
        m_oCS.InitializeCriticalSection();
        m_lCounter = 0;
    }
    ~CCounter()
    {
        m_oCS.DeleteCriticalSection();
    }

public:
    void Increment(int nCount = 1)
    {
        CTemporaryCS oCS(&m_oCS);
        m_lCounter += nCount;
    }
    void Decrement()
    {
        CTemporaryCS oCS(&m_oCS);
        --m_lCounter;
    }
    int GetCounter()
    {
        CTemporaryCS oCS(&m_oCS);
        return m_lCounter;
    }
};

class CThreadConverter : public NSThreads::CBaseThread
{
public:
    CCounter* m_pCounter;
    int m_nDstId;

public:
    CThreadConverter() : NSThreads::CBaseThread()
    {
        m_pCounter = NULL;
        m_nDstId = 1;
    }
    void SetCounter(CCounter* pCounter)
    {
        m_pCounter = pCounter;
    }

protected:
    DWORD ThreadProc()
    {
        std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/linux/Release/";
        std::wstring sSrc = L"/home/oleg/activex/37898EB";
        std::wstring sDstFolder = L"/home/oleg/activex/1/" + std::to_wstring(m_nDstId) + L"/";

        CHtmlFile oFile;
        std::wstring sMetaInfo;
        int nResult = oFile.ConvertEpub(sSrc, sMetaInfo, sDstFolder, sPath);
        nResult;

        m_pCounter->Decrement();
        return 0;
    }
};

int main(int argc, char *argv[])
{
#if 0

    std::vector<std::wstring> arFiles;
#ifdef WIN32
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/windows/Release/";

    arFiles.push_back(L"file:///C:/Users/oleg.korshul/Desktop/original_message%20(5).html");
    std::wstring sDstFolder = L"D:/test/Document";
#else
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/linux/Release/";

    arFiles.push_back(L"/home/oleg/activex/test.html");
    std::wstring sDstFolder = L"/home/oleg/activex/1/";
#endif

    CHtmlFile oFile;
    int nResult = oFile.Convert(arFiles, sDstFolder, sPath);
    nResult;

#else

#ifdef WIN32
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/windows/Release/";

    std::wstring sSrc = L"D:\\37898EB";
    std::wstring sDstFolder = L"D:/test/Document";
#else
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/linux/Release/";

    std::wstring sSrc = L"/home/oleg/activex/37898EB";
    std::wstring sDstFolder = L"/home/oleg/activex/1/";
#endif

#if 1
    CHtmlFile oFile;
    std::wstring sMetaInfo;
    int nResult = oFile.ConvertEpub(sSrc, sMetaInfo, sDstFolder, sPath);
    nResult;
#else

    int nCountTC = 1;
    CCounter oCounter;
    oCounter.Increment(nCountTC);

    for (int i = 0; i < nCountTC; i++)
    {
        CThreadConverter* pThread = new CThreadConverter();
        pThread->SetCounter(&oCounter);
        pThread->m_nDstId = i;
        pThread->Start(0);
    }

    while (0 != oCounter.GetCounter())
        NSThreads::Sleep(100);

#endif

#endif

    return 0;
}
