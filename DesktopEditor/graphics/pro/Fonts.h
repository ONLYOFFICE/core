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
#ifndef _GRAPHICS_EXPORTS_FONT_H_
#define _GRAPHICS_EXPORTS_FONT_H_

#include "../config.h"
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include "../../common/Types.h"

#define NSFONTS_EMBEDDING_RIGHTS_ANY               0x00
#define NSFONTS_EMBEDDING_RIGHTS_PRINT_AND_PREVIEW 0x01
#define NSFONTS_EMBEDDING_RIGHTS_EDITABLE          0x02
#define NSFONTS_EMBEDDING_RIGHTS_INSTALLABLE       0x03

struct TBBox
{
    float fMinX;
    float fMaxX;
    float fMinY;
    float fMaxY;
};

struct TBBoxAdvance
{
    TBBox box;

    float fAdvanceX;
    float fAdvanceY;
};


namespace NSBase
{
    class GRAPHICS_DECL CBaseRefCounter
    {
    protected:
        volatile int m_lRef;

    public:
        CBaseRefCounter();
        virtual ~CBaseRefCounter();

        virtual int AddRef();
        virtual int Release();
    };

    void GRAPHICS_DECL Release(CBaseRefCounter* base);
}

namespace NSFonts
{
    enum EFontFormat
    {
        fontWindowsFNT = 0, // *.fon
        fontTrueType   = 1, // *.ttf
        fontOpenType   = 2, // *.ttf, *.otf (CFF формат)
        fontUnknown    = 3
    };

    class CFontInfo
    {
    public:
        CFontInfo(const std::wstring& wsFontName,
            const std::wstring& wsStyle,
            const std::wstring& wsFontPath,
            long lIndex,
            INT bBold,
            INT bItalic,
            INT bFixedWidth,
            BYTE *pPanose,
            UINT ulRange1,
            UINT ulRange2,
            UINT ulRange3,
            UINT ulRange4,
            UINT ulCodeRange1,
            UINT ulCodeRange2,
            USHORT usWeigth,
            USHORT usWidth,
            SHORT sFamilyClass,
            EFontFormat eFormat,
            SHORT shAvgCharWidth,
            SHORT shAscent,
            SHORT shDescent,
            SHORT shLineGap,
            SHORT shXHeight,
            SHORT shCapHeight,
            USHORT usType)
        {
            m_wsFontName = wsFontName;
            m_wsFontPath = wsFontPath;
            m_wsStyle    = wsStyle;
            m_lIndex     = lIndex;

            m_bBold      = bBold;
            m_bItalic    = bItalic;

            m_bIsFixed   = bFixedWidth;

            if ( pPanose )
                memcpy( (void*)m_aPanose, (const void *)pPanose, 10 );
            else
                memset( (void*)m_aPanose, 0x00, 10 );

            m_ulUnicodeRange1  = ulRange1;
            m_ulUnicodeRange2  = ulRange2;
            m_ulUnicodeRange3  = ulRange3;
            m_ulUnicodeRange4  = ulRange4;
            m_ulCodePageRange1 = ulCodeRange1;
            m_ulCodePageRange2 = ulCodeRange2;
            m_usWeigth         = usWeigth;
            m_usWidth          = usWidth;

            m_sFamilyClass     = sFamilyClass;

            m_eFontFormat      = eFormat;

            m_shAvgCharWidth   = shAvgCharWidth;
            m_shAscent         = shAscent;
            m_shDescent        = shDescent;
            m_shLineGap        = shLineGap;
            m_shXHeight        = shXHeight;
            m_shCapHeight      = shCapHeight;

            m_usType = usType;
        }

        ~CFontInfo()
        {
        }

        int IsEquals(const CFontInfo *pFontInfo)
        {
            return (m_wsFontName == pFontInfo->m_wsFontName &&
                m_wsStyle == pFontInfo->m_wsStyle &&
                m_wsFontPath == pFontInfo->m_wsFontPath &&
                m_bItalic == pFontInfo->m_bItalic &&
                m_bBold == pFontInfo->m_bBold);
        }
		static inline bool CanEmbedForPreviewAndPrint(const USHORT& usType)
		{
			return (2 != usType);
		}
		static inline bool CanEmbedForEdit(const USHORT& usType)
		{
			return (0 == usType || (2 != usType && (usType & 8 || !(usType & 4))));
		}
		static inline bool CanEmbedForInstall(const USHORT& usType)
		{
			return (0 == usType || !(2 == usType || (usType & 2) || (usType & 4) || (usType & 8) || (usType & 512)));
		}

    public:
        std::wstring m_wsFontName;   // Имя шрифта
        std::wstring m_wsFontPath;   // Путь к файлу с шрифтом
        long         m_lIndex;       // Номер шрифта в файле(если в файле больше 1 шрифта)
        std::wstring m_wsStyle;

        INT         m_bBold;            // Bold text
        INT         m_bItalic;          // Italic text
        INT         m_bIsFixed;         // Моноширинный шрифт?

        BYTE        m_aPanose[10];
        UINT        m_ulUnicodeRange1;  // Bits 0-31
        UINT        m_ulUnicodeRange2;  // Bits 32-63
        UINT        m_ulUnicodeRange3;  // Bits 64-95
        UINT        m_ulUnicodeRange4;  // Bits 96-127

        UINT        m_ulCodePageRange1; // Bits 0-31
        UINT        m_ulCodePageRange2; // Bits 32-63

        USHORT       m_usWeigth;
        USHORT       m_usWidth;

        SHORT        m_sFamilyClass;
        EFontFormat  m_eFontFormat;

        SHORT        m_shAvgCharWidth;   // Средняя ширина символов
        SHORT        m_shAscent;         // Ascent
        SHORT        m_shDescent;        // Descent
        SHORT        m_shLineGap;        // Межсимвольный интервал
        SHORT        m_shXHeight;        // Высота буквы 'x' (в нижнем регистре)
        SHORT        m_shCapHeight;      // Высота буквы 'H' (в верхнем регистре)

        USHORT       m_usType;

        std::vector<std::wstring> names;
    };

    class CFontSelectFormat
    {
    public:
        std::wstring*   wsName;
        std::wstring*   wsAltName;

        std::wstring*   wsDefaultName;

        std::wstring*   wsFamilyClass;
        SHORT*          sFamilyClass;

        INT*            bBold;
        INT*            bItalic;

        INT*            bFixedWidth;

        BYTE*           pPanose;

        UINT*           ulRange1;
        UINT*           ulRange2;
        UINT*           ulRange3;
        UINT*           ulRange4;
        UINT*           ulCodeRange1;
        UINT*           ulCodeRange2;

        USHORT*         usWeight;
        USHORT*         usWidth;

        int*            nFontFormat;
        BYTE*           unCharset;

        SHORT*          shAvgCharWidth;
        SHORT*          shAscent;
        SHORT*          shDescent;
        SHORT*          shLineGap;
        SHORT*          shXHeight;
        SHORT*          shCapHeight;

        // mask for allowed os2.fsType field
        // https://docs.microsoft.com/en-us/typography/opentype/spec/os2#fstype
        // 0: Installable embedding
        // 2: Restricted License embedding
        // 4: Preview & Print embedding
        // 8: Editable embedding
        // for examle, to exclude "Restricted License embedding" fonts - set up shType to (0 | 4 | 8)

        USHORT*         usType;

    public:
        CFontSelectFormat()
        {
            wsName = NULL;
            wsAltName = NULL;

            wsDefaultName = NULL;

            wsFamilyClass = NULL;
            sFamilyClass = NULL;

            bBold = NULL;
            bItalic = NULL;

            bFixedWidth = NULL;

            pPanose = NULL;

            ulRange1 = NULL;
            ulRange2 = NULL;
            ulRange3 = NULL;
            ulRange4 = NULL;
            ulCodeRange1 = NULL;
            ulCodeRange2 = NULL;

            usWeight = NULL;
            usWidth = NULL;

            nFontFormat = NULL;
            unCharset = NULL;

            shAvgCharWidth = NULL;
            shAscent = NULL;
            shDescent = NULL;
            shLineGap = NULL;
            shXHeight = NULL;
            shCapHeight = NULL;

            usType = NULL;
        }
        ~CFontSelectFormat()
        {
            Destroy();
        }
		void CreateDuplicate(CFontSelectFormat& oFormat) const
        {
            oFormat.Destroy();

            if (NULL != wsName)
                oFormat.wsName = new std::wstring(*wsName);

            if (NULL != wsAltName)
                oFormat.wsAltName = new std::wstring(*wsAltName);

            if (NULL != wsDefaultName)
                oFormat.wsDefaultName = new std::wstring(*wsDefaultName);

            if (NULL != wsFamilyClass)
                oFormat.wsFamilyClass = new std::wstring(*wsFamilyClass);

            if (NULL != sFamilyClass)
                oFormat.sFamilyClass = new SHORT(*sFamilyClass);

            if (NULL != bBold)
                oFormat.bBold = new INT(*bBold);
            if (NULL != bItalic)
                oFormat.bItalic = new INT(*bItalic);

            if (NULL != bFixedWidth)
                oFormat.bFixedWidth = new INT(*bFixedWidth);

            if (NULL != pPanose)
            {
                oFormat.pPanose = new BYTE[10];
                memcpy( (void*)oFormat.pPanose , (const void *)pPanose, 10 );
            }

            if (NULL != ulRange1)
                oFormat.ulRange1 = new UINT(*ulRange1);
            if (NULL != ulRange2)
                oFormat.ulRange2 = new UINT(*ulRange2);
            if (NULL != ulRange3)
                oFormat.ulRange3 = new UINT(*ulRange3);
            if (NULL != ulRange4)
                oFormat.ulRange4 = new UINT(*ulRange4);
            if (NULL != ulCodeRange1)
                oFormat.ulCodeRange1 = new UINT(*ulCodeRange1);
            if (NULL != ulCodeRange2)
                oFormat.ulCodeRange2 = new UINT(*ulCodeRange2);

            if (NULL != usWeight)
                oFormat.usWeight = new USHORT(*usWeight);
            if (NULL != usWidth)
                oFormat.usWidth = new USHORT(*usWidth);

            if (NULL != nFontFormat)
                oFormat.nFontFormat = new int(*nFontFormat);

            if (NULL != unCharset)
                oFormat.unCharset = new BYTE(*unCharset);

            if (NULL != shAvgCharWidth)
                oFormat.shAvgCharWidth = new SHORT(*shAvgCharWidth);
            if (NULL != shAscent)
                oFormat.shAscent = new SHORT(*shAscent);
            if (NULL != shDescent)
                oFormat.shDescent = new SHORT(*shDescent);
            if (NULL != shLineGap)
                oFormat.shLineGap = new SHORT(*shLineGap);
            if (NULL != shXHeight)
                oFormat.shXHeight = new SHORT(*shXHeight);
            if (NULL != shCapHeight)
                oFormat.shCapHeight = new SHORT(*shCapHeight);

            if (NULL != usType)
                oFormat.usType = new USHORT(*usType);
        }
        void Destroy(bool isDestroyBI = true)
        {
            RELEASEOBJECT(wsName);
            RELEASEOBJECT(wsAltName);

            RELEASEOBJECT(wsDefaultName);

            RELEASEOBJECT(wsFamilyClass);
            RELEASEOBJECT(sFamilyClass);

            if (isDestroyBI)
            {
                RELEASEOBJECT(bBold);
                RELEASEOBJECT(bItalic);
            }

            RELEASEOBJECT(bFixedWidth);

            RELEASEARRAYOBJECTS(pPanose);

            RELEASEOBJECT(ulRange1);
            RELEASEOBJECT(ulRange2);
            RELEASEOBJECT(ulRange3);
            RELEASEOBJECT(ulRange4);
            RELEASEOBJECT(ulCodeRange1);
            RELEASEOBJECT(ulCodeRange2);

            RELEASEOBJECT(usWeight);
            RELEASEOBJECT(usWidth);

            RELEASEOBJECT(nFontFormat);
            RELEASEOBJECT(unCharset);

            RELEASEOBJECT(shAvgCharWidth);
            RELEASEOBJECT(shAscent);
            RELEASEOBJECT(shDescent);
            RELEASEOBJECT(shLineGap);
            RELEASEOBJECT(shXHeight);
            RELEASEOBJECT(shCapHeight);

            RELEASEOBJECT(usType);
        }
        void Fill(CFontInfo* pFontInfo)
        {
            Destroy();

            wsName = new std::wstring(pFontInfo->m_wsFontName);

            bItalic     = new INT(pFontInfo->m_bItalic ? 1 : 0);
            bBold       = new INT(pFontInfo->m_bBold ? 1 : 0);
            bFixedWidth = new INT(pFontInfo->m_bIsFixed ? 1 : 0);

            pPanose = new BYTE[10];
            memcpy((void*)pPanose, pFontInfo->m_aPanose, 10);

            ulRange1 = new UINT(pFontInfo->m_ulUnicodeRange1);
            ulRange2 = new UINT(pFontInfo->m_ulUnicodeRange2);
            ulRange3 = new UINT(pFontInfo->m_ulUnicodeRange3);
            ulRange4 = new UINT(pFontInfo->m_ulUnicodeRange4);

            ulCodeRange1 = new UINT(pFontInfo->m_ulCodePageRange1);
            ulCodeRange2 = new UINT(pFontInfo->m_ulCodePageRange2);

            usWeight = new USHORT(pFontInfo->m_usWeigth);
            usWidth  = new USHORT(pFontInfo->m_usWidth);

            shAvgCharWidth = new SHORT(pFontInfo->m_shAvgCharWidth);
            shAscent       = new SHORT(pFontInfo->m_shAscent);
            shDescent      = new SHORT(pFontInfo->m_shDescent);
            shLineGap      = new SHORT(pFontInfo->m_shLineGap);
            shXHeight      = new SHORT(pFontInfo->m_shXHeight);
            shCapHeight    = new SHORT(pFontInfo->m_shCapHeight);

            usType = new USHORT(pFontInfo->m_usType);
        }

    };

    class CFontListToBufferSerializer
    {
    public:
        std::wstring m_strDirectory;
        bool m_bIsOnlynames;
        int m_nVersion;

    public:
        CFontListToBufferSerializer(const std::wstring& sDir, const bool& bIsOnlynames, const int& nVer)
        {
            m_strDirectory = sDir;
            m_bIsOnlynames = bIsOnlynames;
            m_nVersion = nVer;
        }
    };

    class CLibrary_private;
    class GRAPHICS_DECL CLibrary
    {
    public:
        CLibrary_private* m_internal;
    public:
        CLibrary();
        ~CLibrary();
    };
}

namespace NSFonts
{
    class GRAPHICS_DECL ISimpleGraphicsPath
    {
    public:
        virtual bool _MoveTo(double x, double y)                                                    = 0;
        virtual bool _LineTo(double x, double y)                                                    = 0;
        virtual bool _CurveTo(double x1, double y1, double x2, double y2, double x3, double y3)     = 0;
        virtual bool _Close()                                                                       = 0;
    };

    class GRAPHICS_DECL IFontPath : public NSBase::CBaseRefCounter
    {
    public:
        IFontPath();
        virtual ~IFontPath();

    public:
        virtual int ToInterface(ISimpleGraphicsPath* pPath) = 0;
    };

    namespace NSFontPath
    {
        GRAPHICS_DECL IFontPath* Create();
    }
}

namespace NSFonts
{
    class GRAPHICS_DECL IFontStream : public NSBase::CBaseRefCounter
    {
    public:
        IFontStream();
        virtual ~IFontStream();

    public:
        virtual int CreateFromFile(const std::wstring& strFileName, unsigned char* pDataUse = NULL) = 0;
        virtual int CreateFromMemory(BYTE* pData, LONG lSize, bool bClear) = 0;
        virtual void GetMemory(BYTE*& pData, LONG& lSize) = 0;
    };

    class GRAPHICS_DECL IFontsMemoryStorage : public NSBase::CBaseRefCounter
    {
    public:
        IFontsMemoryStorage();
        virtual ~IFontsMemoryStorage();

        virtual bool Add(const std::wstring& id, BYTE* data, LONG size, bool bClear = false) = 0;
        virtual bool Remove(const std::wstring& id) = 0;
        virtual IFontStream* Get(const std::wstring& id) = 0;

        virtual std::wstring GenerateId() = 0;
        virtual void Clear() = 0;
    };

    class GRAPHICS_DECL IApplicationFontStreams : public NSBase::CBaseRefCounter
    {
    public:
        IApplicationFontStreams();
        virtual ~IApplicationFontStreams();

    public:
        virtual IFontStream* GetStream(const std::wstring& strFile) = 0;
        virtual void CheckStreams(std::map<std::wstring, bool>& mapFiles) = 0;
        virtual void Clear() = 0;
    };

    namespace NSStream
    {
        GRAPHICS_DECL IFontStream* Create();        
    }

    namespace NSApplicationFontStream
    {
        GRAPHICS_DECL IApplicationFontStreams* Create();

        // create default
        GRAPHICS_DECL IFontsMemoryStorage* CreateDefaultGlobalMemoryStorage();

        // without AddRef!!!
        GRAPHICS_DECL IFontsMemoryStorage* GetGlobalMemoryStorage();
        GRAPHICS_DECL void SetGlobalMemoryStorage(IFontsMemoryStorage* pStorage);
    }
}

namespace NSFonts
{
    class GRAPHICS_DECL IFontFile : public NSBase::CBaseRefCounter
    {
    public:
        IFontFile();
        virtual ~IFontFile();

    public:
        virtual std::string GetStyleName() = 0;
        virtual void GetPanose(unsigned char* pData) = 0;

        virtual bool IsFixedWidth() = 0;

        virtual int GetAscender() = 0;
        virtual int GetDescender() = 0;
        virtual int GetHeight() = 0;
        virtual int Units_Per_Em() = 0;

        virtual std::wstring GetFontFormat() = 0;

        virtual bool IsItalic() = 0;
        virtual bool IsBold() = 0;


        virtual void SetItalic(const INT& value) = 0;
        virtual void SetNeedBold(const INT& value) = 0;

        virtual bool IsSymbolic(bool bIsOS2Check = false) = 0;

        virtual int IsUnicodeRangeAvailable(unsigned long ulBit, unsigned int un4ByteIndex) = 0;

        virtual void CheckTextMatrix() = 0;
        virtual bool SetTextMatrix(const double& fA, const double& fB, const double& fC, const double fD, double fE, double fF) = 0;
        virtual void SetFontMatrix(const double& fA, const double& fB, const double& fC, const double fD, double fE, double fF) = 0;

        virtual int SetCMapForCharCode(long lUnicode, int *pnCMapIndex) = 0;
        virtual double GetCharWidth(int gid) = 0;

        virtual int GetGIDByUnicode(int code) = 0;

        virtual int GetEmbeddingLicenceType() = 0;
        virtual void FillFontSelectFormat(CFontSelectFormat& oFormat) = 0;
    };

    namespace NSFontFile
    {
        GRAPHICS_DECL IFontFile* Create();
    }
}

namespace NSFonts
{
    class GRAPHICS_DECL IFontsCache : public NSBase::CBaseRefCounter
    {
    public:
        IFontsCache();
        virtual ~IFontsCache();

        virtual void Clear() = 0;
        virtual void SetCacheSize(const int& lMaxSize) = 0;

    public:
        virtual void SetStreams(IApplicationFontStreams* pStreams) = 0;
        virtual IFontFile* LockFont(CLibrary& library, const std::wstring& strFileName, const int& lFaceIndex, const double& dSize) = 0;
    };

    namespace NSFontCache
    {
        GRAPHICS_DECL IFontsCache* Create();
    }
}

namespace NSFonts
{
    class IApplicationFonts;
    class GRAPHICS_DECL IFontManager : public NSBase::CBaseRefCounter
    {
    public:
        IFontManager();
        virtual ~IFontManager();

        virtual IFontsCache* GetCache() = 0;
        virtual IApplicationFonts* GetApplication() = 0;
        virtual IFontFile* GetFile() = 0;

        virtual double GetCharSpacing() = 0;
        virtual void SetCharSpacing(const double &dCharSpacing) = 0;

        virtual int GetStringGID() = 0;
        virtual void SetStringGID(const int& bStringGID) = 0;

        virtual int GetUnitsPerEm() = 0;
        virtual int GetAscender() = 0;
        virtual int GetDescender() = 0;
        virtual int GetLineHeight() = 0;

        virtual std::wstring GetName() = 0;

        virtual void Initialize() = 0;
        virtual void SetOwnerCache(IFontsCache* pCache) = 0;
        virtual void CreateOwnerCache(const int& nCacheSize = -1);

        virtual void AfterLoad() = 0;

        virtual double UpdateSize(const double& dOldSize, const double& dDpi, const double& dNewDpi) = 0;

        virtual int LoadString1(const std::wstring& wsBuffer, const float& fX, const float& fY) = 0;
        virtual int LoadString2(const std::wstring& wsBuffer, const float& fX, const float& fY) = 0;
        virtual int LoadString1(const unsigned int* pGids, const unsigned int& nGidsCount, const float& fX, const float& fY) = 0;
        virtual int LoadString2(const unsigned int* pGids, const unsigned int& nGidsCount, const float& fX, const float& fY) = 0;
        virtual int LoadString3(const int& gid, const float& fX, const float& fY) = 0;
        virtual int LoadString3C(const int& gid, const float& fX, const float& fY) = 0;
        virtual int LoadString2C(const int& wsBuffer, const float& fX, const float& fY) = 0;

        virtual TBBoxAdvance MeasureChar2(const LONG& lUnicode) = 0;
        virtual TBBox MeasureString() = 0;
        virtual TBBox MeasureString2() = 0;

        virtual int SetTextMatrix(const double& fA, const double& fB, const double& fC, const double& fD, const double& fE, const double& fF) = 0;
        virtual int SetTextMatrix2(const double& fA, const double& fB, const double& fC, const double& fD, const double& fE, const double& fF) = 0;

        virtual int GetStringPath(ISimpleGraphicsPath* pPath) = 0;

        virtual CFontInfo* GetFontInfoByParams(CFontSelectFormat& oFormat, bool bIsDictionaryUse = true) = 0;
        virtual std::vector<CFontInfo*> GetAllStylesByFontName(const std::wstring& strName) = 0;

        virtual int LoadFontByName(const std::wstring& sName, const double& dSize, const int& lStyle, const double& dDpiX, const double& dDpiY) = 0;
        virtual int LoadFontFromFile(const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY) = 0;
        virtual int LoadFontFromFile2(IFontsCache* pCache, const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY) = 0;
        virtual void CloseFont() = 0;

        virtual std::wstring GetFontType() = 0;
        virtual void SetSubpixelRendering(const bool& hmul, const bool& vmul) = 0;

        virtual unsigned int GetNameIndex(const std::wstring& wsName) = 0;
		virtual unsigned int GetGIDByUnicode(const unsigned int& unCode) = 0;
        
        virtual void GetFace(double& d0, double& d1, double& d2) = 0;
        virtual void GetLimitsY(double& dMin, double& dMax) = 0;

        virtual int GetUnderline(float *pfStartX, float *pfStartY, float *pfEndX, float *pfEndY, float *pfSize) = 0;

		virtual void SetUseCorrentFontByName(const bool& use) = 0;
    public:
        static IFontFile* LoadFontFile(CLibrary& library, IFontStream* pStream, int lFaceIndex);
    };

    namespace NSFontManager
    {
        GRAPHICS_DECL IFontManager* Create();
    }
}

namespace NSFonts
{
    class GRAPHICS_DECL IFontList : public NSBase::CBaseRefCounter
    {
    public:
        IFontList();
        virtual ~IFontList();

    public:
        virtual std::vector<NSFonts::CFontInfo*>* GetFonts() = 0;
        virtual CFontInfo* GetByParams(CFontSelectFormat& oSelect, bool bIsDictionaryUse = true) = 0;
        virtual void ToBuffer(BYTE** pDstData, LONG* pLen, CFontListToBufferSerializer& oSerializer) = 0;
    };

    class GRAPHICS_DECL IApplicationFonts : public NSBase::CBaseRefCounter
    {
    public:
        IApplicationFonts();
        virtual ~IApplicationFonts();

    public:
        virtual IFontsCache* GetCache() = 0;
        virtual IFontList* GetList() = 0;
        virtual IApplicationFontStreams* GetStreams() = 0;

        virtual void InitializeFromFolder(std::wstring strFolder, bool bIsCheckSelection = true) = 0;
        virtual void Initialize(bool bIsCheckSelection = true) = 0;
        virtual void InitializeFromBin(BYTE* pData, unsigned int nLen) = 0;
        virtual void InitializeRanges(unsigned char* data) = 0;

        virtual std::vector<std::wstring> GetSetupFontFiles() = 0;
        virtual void InitializeFromArrayFiles(std::vector<std::wstring>& files, int nFlag = 0) = 0;

    #if defined(_WIN32) || defined (_WIN64)
        virtual void InitFromReg() = 0;
    #endif

    #ifdef _IOS
        virtual std::vector<std::wstring> GetSetupFontFiles_ios() = 0;
    #endif

        virtual IFontManager* GenerateFontManager() = 0;

        virtual std::wstring GetFontBySymbol(int symbol) = 0;
    };
    
    namespace NSApplication
    {
        GRAPHICS_DECL IApplicationFonts* Create();
    }
}

namespace NSFonts
{
    class GRAPHICS_DECL CApplicationFontsSymbolsChecker
    {
    public:
        virtual void Check(const int& nCode, const unsigned int& nIndex) = 0;
    };

    class CApplicationFontsSymbols_Private;
    class GRAPHICS_DECL CApplicationFontsSymbols
    {
    private:
        CApplicationFontsSymbols_Private* m_internal;

    public:
        CApplicationFontsSymbols();
        ~CApplicationFontsSymbols();

    public:
        void CheckSymbols(const std::wstring& sFile, const int& nFaceIndex, CApplicationFontsSymbolsChecker* pChecker);
    };
}

#endif // _GRAPHICS_EXPORTS_FONT_H_
