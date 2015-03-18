#include "ApplicationFonts.h"
#include "ApplicationFontsWorker.h"
#include "../common/File.h"
#include "../common/Directory.h"
#include "../editor/MemoryStream.h"

namespace NSFontsApplication
{
    void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
    {
        size_t posn = 0;
        while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }
    
    class CTextItem
    {
    protected:
        wchar_t*	m_pData;
        size_t		m_lSize;
        
        wchar_t*	m_pDataCur;
        size_t		m_lSizeCur;
        
    public:
        CTextItem()
        {
            m_pData = NULL;
            m_lSize = 0;
            
            m_pDataCur	= m_pData;
            m_lSizeCur	= m_lSize;
        }
        CTextItem(const CTextItem& oSrc)
        {
            m_pData = NULL;
            *this = oSrc;
        }
        CTextItem& operator=(const CTextItem& oSrc)
        {
            RELEASEMEM(m_pData);
            
            m_lSize		= oSrc.m_lSize;
            m_lSizeCur	= oSrc.m_lSizeCur;
            m_pData		= (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
            
            memcpy(m_pData, oSrc.m_pData, m_lSizeCur * sizeof(wchar_t));
            
            m_pDataCur = m_pData + m_lSizeCur;
            
            return *this;
        }
        
        CTextItem(const size_t& nLen)
        {
            m_lSize = nLen;
            m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
            
            m_lSizeCur = 0;
            m_pDataCur = m_pData;
        }
        CTextItem(wchar_t* pData, const size_t& nLen)
        {
            m_lSize = nLen;
            m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
            
            memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
            
            m_lSizeCur = m_lSize;
            m_pDataCur = m_pData + m_lSize;
        }
        CTextItem(wchar_t* pData, BYTE* pUnicodeChecker = NULL)
        {
            size_t nLen = GetStringLen(pData);
            
            m_lSize = nLen;
            m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
            
            memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
            
            m_lSizeCur = m_lSize;
            m_pDataCur = m_pData + m_lSize;
            
            if (NULL != pUnicodeChecker)
            {
                wchar_t* pMemory = m_pData;
                while (pMemory < m_pDataCur)
                {
                    if (!pUnicodeChecker[*pMemory])
                        *pMemory = wchar_t(' ');
                    ++pMemory;
                }
            }
        }
        virtual ~CTextItem()
        {
            RELEASEMEM(m_pData);
        }
        
        inline void AddSize(const size_t& nSize)
        {
            if (NULL == m_pData)
            {
                m_lSize = 1000;
                if (nSize > m_lSize)
                    m_lSize = nSize;
                
                m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
                
                m_lSizeCur = 0;
                m_pDataCur = m_pData;
                return;
            }
            
            if ((m_lSizeCur + nSize) > m_lSize)
            {
                while ((m_lSizeCur + nSize) > m_lSize)
                {
                    m_lSize *= 2;
                }
                
                wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
                if (NULL != pRealloc)
                {
                    // реаллок сработал
                    m_pData		= pRealloc;
                    m_pDataCur	= m_pData + m_lSizeCur;
                }
                else
                {
                    wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
                    memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));
                    
                    free(m_pData);
                    m_pData		= pMalloc;
                    m_pDataCur	= m_pData + m_lSizeCur;
                }
            }
        }
        
    public:
        
        inline void operator+=(const std::wstring& oTemp)
        {
            WriteString(oTemp.c_str(), oTemp.length());
        }
        inline wchar_t operator[](const size_t& nIndex)
        {
            if (nIndex < m_lSizeCur)
                return m_pData[nIndex];
            
            return 0;
        }
        
        inline void AddSpace()
        {
            AddSize(1);
            *m_pDataCur = wchar_t(' ');
            
            ++m_lSizeCur;
            ++m_pDataCur;
        }
        inline void CorrectUnicode(const BYTE* pUnicodeChecker)
        {
            if (NULL != pUnicodeChecker)
            {
                wchar_t* pMemory = m_pData;
                while (pMemory < m_pDataCur)
                {
                    if (!pUnicodeChecker[*pMemory])
                        *pMemory = wchar_t(' ');
                    ++pMemory;
                }
            }
        }
        inline void RemoveLastSpaces()
        {
            wchar_t* pMemory = m_pDataCur - 1;
            while ((pMemory > m_pData) && (wchar_t(' ') == *pMemory))
            {
                --pMemory;
                --m_lSizeCur;
                --m_pDataCur;
            }
            
        }
        inline bool IsSpace()
        {
            if (1 != m_lSizeCur)
                return false;
            return (wchar_t(' ') == *m_pData);
        }
        
    public:
        inline void WriteString(const wchar_t* pString, const size_t& nLen)
        {
            AddSize(nLen);
            memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
            m_pDataCur += nLen;
            m_lSizeCur += nLen;
        }
        inline size_t GetCurSize()
        {
            return m_lSizeCur;
        }
        inline size_t GetSize()
        {
            return m_lSize;
        }
        inline void Clear()
        {
            RELEASEMEM(m_pData);
            
            m_pData = NULL;
            m_lSize = 0;
            
            m_pDataCur	= m_pData;
            m_lSizeCur	= 0;
        }
        inline void ClearNoAttack()
        {
            m_pDataCur	= m_pData;
            m_lSizeCur	= 0;
        }
        
        inline size_t GetStringLen(wchar_t* pData)
        {
            wchar_t* s = pData;
            for (; *s != 0; ++s);
            return (size_t)(s - pData);
        }
        
        inline std::wstring GetCString()
        {
            std::wstring str(m_pData, (int)m_lSizeCur);
            return str;
        }
        inline wchar_t* GetBuffer()
        {
            return m_pData;
        }
    };
    
    class CStringWriter : public CTextItem
    {
    public:
        CStringWriter() : CTextItem()
        {
        }
        virtual ~CStringWriter()
        {
        }
        
    public:
        
        inline void Write(CStringWriter& oWriter)
        {
            CTextItem::WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
        }
    };
    
    class CFontInfoJS
    {
    public:
        std::wstring m_sName;
        
        int	m_lIndexR;
        int	m_lFaceIndexR;
        int	m_lIndexI;
        int	m_lFaceIndexI;
        int	m_lIndexB;
        int	m_lFaceIndexB;
        int	m_lIndexBI;
        int	m_lFaceIndexBI;
        
        CFontInfoJS()
        {
            m_sName			= L"";
            
            m_lIndexR		= -1;
            m_lFaceIndexR	= -1;
            m_lIndexI		= -1;
            m_lFaceIndexI	= -1;
            m_lIndexB		= -1;
            m_lFaceIndexB	= -1;
            m_lIndexBI		= -1;
            m_lFaceIndexBI	= -1;
        }
        
        CFontInfoJS(const CFontInfoJS& oSrc)
        {
            *this = oSrc;
        }
        
        CFontInfoJS& operator=(const CFontInfoJS& oSrc)
        {
            m_sName			= oSrc.m_sName;
            
            m_lIndexR	= oSrc.m_lIndexR;
            m_lIndexI	= oSrc.m_lIndexI;
            m_lIndexB	= oSrc.m_lIndexB;
            m_lIndexBI	= oSrc.m_lIndexBI;
            
            m_lFaceIndexR	= oSrc.m_lFaceIndexR;
            m_lFaceIndexI	= oSrc.m_lFaceIndexI;
            m_lFaceIndexB	= oSrc.m_lFaceIndexB;
            m_lFaceIndexBI	= oSrc.m_lFaceIndexBI;
            
            return *this;
        }
    };
    
    static std::vector<std::wstring> SaveAllFontsJS(CApplicationFonts& applicationFonts, CStringWriter& oWriterJS)
    {
        std::vector<std::wstring> arrNames;
        
        CArray<CFontInfo*>* pList = applicationFonts.GetList()->GetFonts();
        
#ifdef _IOS
        
        int nOldCount = pList->GetCount();
        for (int i = 0; i < nOldCount; i++)
        {
            CFontInfo* pInfo = pList->operator [](i);
            
            if (pInfo->m_wsFontName.find(L".") == 0)
            {
                // странные шрифты какие-то есть в ios
                pList->RemoveAt(i);
                i--;
                nOldCount--;
                continue;
            }
        }
        
#endif
        
        int nCount = pList->GetCount();
        
        // сначала строим массив всех файлов шрифтов
        std::map<std::wstring, LONG> mapFontFiles;
        std::map<LONG, std::wstring> mapFontFiles2;
        LONG lFontFiles = 0;
        for (int i = 0; i < nCount; ++i)
        {
            CFontInfo* pInfo = pList->operator [](i);
            
            if (mapFontFiles.find(pInfo->m_wsFontPath) == mapFontFiles.end())
            {
                mapFontFiles.insert(std::pair<std::wstring, LONG>(pInfo->m_wsFontPath, lFontFiles));
                mapFontFiles2.insert(std::pair<LONG, std::wstring>(lFontFiles, pInfo->m_wsFontPath));
                ++lFontFiles;
            }
        }
        // -----------------------------------------
        
        // теперь строим массив всех шрифтов по имени
        std::map<std::wstring, CFontInfoJS> mapFonts;
        CArray<std::wstring> arrFonts;
        
        for (int i = 0; i < nCount; ++i)
        {
            CFontInfo* pInfo = (CFontInfo*)pList->operator [](i);
            std::wstring strPath = pInfo->m_wsFontPath;
            std::wstring strName = pInfo->m_wsFontName;
            
            int lFontIndex = 0;
            int lFaceIndex = 0;
            
            std::map<std::wstring, LONG>::iterator it = mapFontFiles.find(strPath);
            lFontIndex = (int)it->second;
            
            if (pInfo->m_lIndex >= 0)
                lFaceIndex = (int)pInfo->m_lIndex;
            
            std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(pInfo->m_wsFontName);
            if (mapFonts.end() != pPair)
            {
                pPair->second.m_sName = pInfo->m_wsFontName;
                
                if (pInfo->m_bBold && pInfo->m_bItalic)
                {
                    pPair->second.m_lIndexBI = lFontIndex;
                    pPair->second.m_lFaceIndexBI = lFaceIndex;
                }
                else if (pInfo->m_bBold)
                {
                    pPair->second.m_lIndexB = lFontIndex;
                    pPair->second.m_lFaceIndexB = lFaceIndex;
                }
                else if (pInfo->m_bItalic)
                {
                    pPair->second.m_lIndexI = lFontIndex;
                    pPair->second.m_lFaceIndexI = lFaceIndex;
                }
                else
                {
                    pPair->second.m_lIndexR = lFontIndex;
                    pPair->second.m_lFaceIndexR = lFaceIndex;
                }
            }
            else
            {
                CFontInfoJS fontInfo;
                
                fontInfo.m_sName = pInfo->m_wsFontName;
                
                if (pInfo->m_bBold && pInfo->m_bItalic)
                {
                    fontInfo.m_lIndexBI = lFontIndex;
                    fontInfo.m_lFaceIndexBI = lFaceIndex;
                }
                else if (pInfo->m_bBold)
                {
                    fontInfo.m_lIndexB = lFontIndex;
                    fontInfo.m_lFaceIndexB = lFaceIndex;
                }
                else if (pInfo->m_bItalic)
                {
                    fontInfo.m_lIndexI = lFontIndex;
                    fontInfo.m_lFaceIndexI = lFaceIndex;
                }
                else
                {
                    fontInfo.m_lIndexR = lFontIndex;
                    fontInfo.m_lFaceIndexR = lFaceIndex;
                }
                
                mapFonts.insert(std::pair<std::wstring, CFontInfoJS>(fontInfo.m_sName, fontInfo));
                arrFonts.Add(fontInfo.m_sName);
            }
        }
        // -------------------------------------------
        
        // теперь сортируем шрифты по имени ----------
        int nCountFonts = arrFonts.GetCount();
        for (int i = 0; i < nCountFonts; ++i)
        {
            for (int j = i + 1; j < nCountFonts; ++j)
            {
                if (arrFonts[i] > arrFonts[j])
                {
                    std::wstring temp = arrFonts[i];
                    arrFonts[i] = arrFonts[j];
                    arrFonts[j] = temp;
                }
            }
        }
        // -------------------------------------------
        
        // и самое главное. Здесь должен скидываться скрипт для работы со всеми шрифтами.
        // все объекты, которые позволят не знать о существующих фонтах
        if (true)
        {
            // сначала все файлы
            size_t nCountFiles = mapFontFiles.size();
            if (nCountFiles == 0)
                oWriterJS += (L"window[\"__fonts_files\"] = []; \n\n");
            else
            {
                std::wstring* pMassFiles = new std::wstring[nCountFiles];
                for ( std::map<std::wstring, LONG>::iterator pos = mapFontFiles.begin(); pos != mapFontFiles.end(); ++pos)
                {
                    std::wstring strFontId = pos->first;
                    
                    string_replace(strFontId, L"\\\\", L"\\");
                    string_replace(strFontId, L"\\", L"/");
                    
                    //int nStart = strFontId.find_last_of(wchar_t('\\'));
                    //strFontId = strFontId.substr(nStart + 1);
                    
                    pMassFiles[pos->second] = strFontId;
                }
                
                oWriterJS += (L"window[\"__fonts_files\"] = [\n");
                for (size_t nIndex = 0; nIndex < nCountFiles; ++nIndex)
                {
                    oWriterJS += (L"\"");
                    oWriterJS += (pMassFiles[nIndex]);
                    if (nIndex != (nCountFiles - 1))
                        oWriterJS += (L"\",\n");
                    else
                        oWriterJS += (L"\"");
                }
                oWriterJS += (L"\n];\n\n");
                
                delete [] pMassFiles;
            }
            
            oWriterJS += L"window[\"__fonts_infos\"] = [\n";
            
            for (int index = 0; index < nCountFonts; ++index)
            {
                std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[index]);
                
                char buffer[1000];
                sprintf(buffer, "\",%d,%d,%d,%d,%d,%d,%d,%d]", pPair->second.m_lIndexR, pPair->second.m_lFaceIndexR,
                        pPair->second.m_lIndexI, pPair->second.m_lFaceIndexI,
                        pPair->second.m_lIndexB, pPair->second.m_lFaceIndexB,
                        pPair->second.m_lIndexBI, pPair->second.m_lFaceIndexBI);
                
                std::string sBuffer(buffer);
                
                oWriterJS += L"[\"";
                oWriterJS += pPair->second.m_sName;
                oWriterJS += NSFile::CUtf8Converter::GetUnicodeFromCharPtr(sBuffer);
                
                if (index != (nCountFonts - 1))
                    oWriterJS += (L",\n");
                else
                    oWriterJS += (L"\n");
                
                arrNames.push_back(pPair->second.m_sName);
            }
            oWriterJS += (L"];\n\n");
            
            if (true)
            {
                BYTE* pData = NULL;
                LONG lLen = 0;
                applicationFonts.GetList()->ToBuffer(&pData, &lLen, L"", true);
                
                char* cData64 = NULL;
                int nData64Dst = 0;
                NSFile::CBase64Converter::Encode(pData, (int)lLen, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);
                
                std::wstring sData64 = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(cData64, (LONG)nData64Dst, FALSE);
                
                oWriterJS += (L"window[\"g_fonts_selection_bin\"] = \"");
                oWriterJS += sData64;
                oWriterJS += L"\";\n";
                
                RELEASEARRAYOBJECTS(pData);
                RELEASEARRAYOBJECTS(cData64);
            }
        }
        
        return arrNames;
    }
    
    class CStreamReader
    {
    private:
        BYTE*	Data;
        BYTE*	DataEnd;
        BYTE*	DataCur;
        
    public:
        CStreamReader()
        {
            Data	= NULL;
            DataEnd	= NULL;
            DataCur	= NULL;
        }
        CStreamReader(BYTE* pData, unsigned int nSize)
        {
            Data	= pData;
            DataEnd = Data + nSize;
            DataCur = Data;
        }
        
        inline bool IsNoEnd()
        {
            return (DataCur < DataEnd);
        }
        inline void Skip(int count)
        {
            DataCur += count;
        }
        inline BYTE* GetCur()
        {
            return DataCur;
        }
        
        inline int GetInt32()
        {
#ifdef _ARM_ALIGN_
            int ret = 0;
            memcpy(&ret, DataCur, sizeof(int));
            DataCur += 4;
            return ret;
#else
            int ret = *((int*)DataCur);
            DataCur += 4;
            return ret;
#endif
        }
        inline BYTE GetUChar()
        {
            BYTE ret = *DataCur;
            ++DataCur;
            return ret;
        }
        inline std::string GetString1()
        {
            int nLen = GetInt32();
            if (0 == nLen)
                return "";
            
            std::string s((char*)DataCur, nLen);
            DataCur += nLen;
            return s;
        }
    };
}

CApplicationFontsWorker::CApplicationFontsWorker()
{
    
}
CApplicationFontsWorker::~CApplicationFontsWorker()
{
    
}

std::vector<std::wstring> CApplicationFontsWorker::CheckApplication(bool bIsNeedSystemFonts,
                      unsigned char* pDataSrc, unsigned int nLenSrc,
                      unsigned char*& pDataDst, unsigned int& nLenDst)
{
    std::vector<std::wstring> arrNames;
    
    CArray<std::wstring> fonts;
    CApplicationFonts oFonts;
    
    pDataDst = NULL;
    nLenDst = 0;
    
    if (bIsNeedSystemFonts)
    {
        fonts = oFonts.GetSetupFontFiles();
    }
    
    for (std::vector<std::wstring>::iterator iter = m_arAdditionalFolders.begin(); iter != m_arAdditionalFolders.end(); ++iter)
    {
        NSDirectory::GetFiles2(*iter, fonts);
    }
    
    if (NULL != pDataSrc)
    {
        // проверяем, поменялось ли чего
        NSFontsApplication::CStreamReader oReader(pDataSrc, nLenSrc);
        int nLenAllFonts = oReader.GetInt32();
        oReader.Skip(nLenAllFonts);
        
        CArray<std::wstring> arrOld;
        
        int nCountCache = oReader.GetInt32();
        for (int i = 0; i < nCountCache; ++i)
        {
            int nLen = oReader.GetInt32();
            arrOld.Add(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(oReader.GetCur(), (LONG)nLen));
            oReader.Skip(nLen);
        }
        
        if (fonts.GetCount() == arrOld.GetCount())
        {
            int nCountFonts = fonts.GetCount();
            bool bIsBreak = false;
            for (int i = 0 ; i < nCountFonts; ++i)
            {
                if (fonts[i] != arrOld[i])
                {
                    bIsBreak = true;
                    break;
                }
            }
            
            if (bIsBreak)
            {
                // ничего не поменялось
                int nCountNames = oReader.GetInt32();
                for (int i = 0; i < nCountNames; ++i)
                {
                    int nLen = oReader.GetInt32();
                    arrNames.push_back(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(oReader.GetCur(), (LONG)nLen));
                    oReader.Skip(nLen);
                }
                
                return arrNames;
            }
        }
    }
    
    // произошли изменения
    oFonts.InitializeFromArrayFiles(fonts);
    
    NSFontsApplication::CStringWriter oWriterJS;
    arrNames = NSFontsApplication::SaveAllFontsJS(oFonts, oWriterJS);
    
    // теперь нужно записать новую дату
    NSMemoryStream::CMemoryStream oStream;
    std::string sAllFontsJS = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(oWriterJS.GetBuffer(), (LONG)oWriterJS.GetCurSize());
    oStream.WriteStringA2(sAllFontsJS.c_str(), (int)sAllFontsJS.length());

    int nCountF = fonts.GetCount();
    oStream.WriteLONG((LONG)nCountF);
    
    for (int i = 0; i < nCountF; ++i)
    {
        std::string sTmp = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(fonts[i]);
        oStream.WriteStringA2(sTmp.c_str(), (int)sTmp.length());
    }
    
    nCountF = (int)arrNames.size();
    oStream.WriteLONG((LONG)nCountF);
    for (std::vector<std::wstring>::iterator iter = arrNames.begin(); iter != arrNames.end(); ++iter)
    {
        std::string sTmp = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(*iter);
        oStream.WriteStringA2(sTmp.c_str(), (int)sTmp.length());
    }
    
    nLenDst = (unsigned int)oStream.GetSize();
    pDataDst = new unsigned char[nLenDst];
    memcpy(pDataDst, oStream.GetData(), (size_t)nLenDst);
    
    return arrNames;
}
