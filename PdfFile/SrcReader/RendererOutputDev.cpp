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

#include "Adaptors.h"
#include "../lib/xpdf/ErrorCodes.h"
#include "../lib/xpdf/GfxState.h"
#include "../lib/xpdf/GfxFont.h"
#include "../lib/fofi/FoFiTrueType.h"
#include "../lib/fofi/FoFiIdentifier.h"
//#include "../lib/xpdf/File.h"
#include "../lib/xpdf/CMap.h"
#include "../lib/xpdf/Dict.h"
#include "../lib/xpdf/Stream.h"
//#include "FontFileTrueType.h"
//#include "FontFileType1C.h"
#include "../lib/xpdf/CharCodeToUnicode.h"
#include "RendererOutputDev.h"
#include "XmlUtils.h"

#include "../../DesktopEditor/graphics/Image.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/common/Array.h"
#include "../../DesktopEditor/graphics/BaseThread.h"
#include "../Resources/BaseFonts.h"

#ifndef BUILDING_WASM_MODULE
#define FONTS_USE_AFM_SETTINGS
#else
#include "FontsWasm.h"
#define FONTS_USE_ONLY_MEMORY_STREAMS
#endif

#if defined(_MSC_VER)
#define OO_INLINE __forceinline
#else
#define OO_INLINE inline
#endif

namespace NSCorrectFontName
{
    bool CheckFontNameStyle(std::wstring& sName, const std::wstring& sStyle)
    {
        size_t nPos = 0;
        size_t nLenReplace = sStyle.length();
        bool bRet = false;

        std::wstring sName2 = sName;
        NSStringExt::ToLower(sName2);

        while (std::wstring::npos != (nPos = sName2.find(sStyle, nPos)))
        {
            size_t nOffset = 0;
            if ((nPos > 0) && sName2.at(nPos - 1) == '-')
            {
                --nPos;
                ++nOffset;
            }

            bRet = true;
            sName.erase(nPos, nLenReplace + nOffset);
            sName2.erase(nPos, nLenReplace + nOffset);
        }
        return bRet;
    }

    void CheckFontNamePDF(std::wstring& sName, NSFonts::CFontSelectFormat& format)
    {
        if (sName.length() > 7 && sName.at(6) == '+')
        {
            bool bIsRemove = true;
            for (int nIndex = 0; nIndex < 6; nIndex++)
            {
                wchar_t nChar = sName.at(nIndex);
                if (nChar < 'A' || nChar > 'Z')
                {
                    bIsRemove = false;
                    break;
                }
            }
            if (bIsRemove)
            {
                sName.erase(0, 7);
            }
        }

        bool bBold = false;
        bool bItalic = false;

        CheckFontNameStyle(sName, L"regular");
        CheckFontNameStyle(sName, L"condensed");
        CheckFontNameStyle(sName, L"condensedlight");
        //CheckFontNameStyle(sName, L"light");

        CheckFontNameStyle(sName, L"condensedbold");
        CheckFontNameStyle(sName, L"semibold");
        if (CheckFontNameStyle(sName, L"boldmt")) bBold = true;
        if (CheckFontNameStyle(sName, L"bold")) bBold = true;

        if (CheckFontNameStyle(sName, L"italicmt")) bItalic = true;
        if (CheckFontNameStyle(sName, L"italic")) bItalic = true;
        if (CheckFontNameStyle(sName, L"oblique")) bItalic = true;

        if (CheckFontNameStyle(sName, L"bolditalicmt")) { bBold = true; bItalic = true; }
        if (CheckFontNameStyle(sName, L"bolditalic")) { bBold = true; bItalic = true; }
        if (CheckFontNameStyle(sName, L"bold_italic")) { bBold = true; bItalic = true; }
        if (CheckFontNameStyle(sName, L"boldoblique")) { bBold = true; bItalic = true; }
        if (CheckFontNameStyle(sName, L"bold_oblique")) { bBold = true; bItalic = true; }

        if (bBold)
            format.bBold = new INT(1);
        if (bItalic)
            format.bItalic = new INT(1);
    }
}

class CMemoryFontStream
{
public:
    BYTE* m_pData;
    int m_nSize;
    int m_nPos;
    bool m_bIsAttach;

    CMemoryFontStream()
    {
        m_pData = NULL;
        m_nSize = 0;
        m_nPos = 0;
        m_bIsAttach = false;
    }
    ~CMemoryFontStream()
    {
        if (NULL != m_pData && !m_bIsAttach)
            RELEASEARRAYOBJECTS(m_pData);
    }

    void fromStream(std::wstring& sStreamName)
    {
        NSFonts::IFontStream* pStream = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Get(sStreamName);
        if (pStream)
        {
            LONG lSize = 0;
            pStream->GetMemory(m_pData, lSize);
            m_nSize = (int)lSize;
            m_nPos = 0;
            m_bIsAttach = true;
        }
    }

    void fromBuffer(BYTE* pData, int nSize)
    {
        if (pData)
        {
            m_pData = pData;
            m_nSize = nSize;
            m_nPos = 0;
            m_bIsAttach = true;
        }
    }

    void load(Object& oStreamObject)
    {
        int nCurrentSize = 0xFFFF;
        int nCurrentPos = 0;
        BYTE* pStream = new BYTE[nCurrentSize];

        int nChar;
        while ((nChar = oStreamObject.streamGetChar()) != EOF)
        {
            if (nCurrentPos >= nCurrentSize)
            {
                int nNewSize = 2 * nCurrentSize;
                BYTE* pNewBuffer = new BYTE[nNewSize];
                memcpy(pNewBuffer, pStream, nCurrentSize);
                RELEASEARRAYOBJECTS(pStream);
                pStream = pNewBuffer;
                nCurrentSize = nNewSize;
            }
            pStream[nCurrentPos++] = nChar;
        }

        m_pData = pStream;
        m_nSize = nCurrentPos;
        m_nPos = 0;
    }

    int getChar()
    {
        if (m_nPos >= m_nSize)
            return EOF;
        return m_pData[m_nPos++];
    }

    void toStart()
    {
        m_nPos = 0;
    }
};

static int readFromMemoryStream(void* data)
{
    return ((CMemoryFontStream*)data)->getChar();
}

// TODO: 1. Реализовать по-нормальному градиентные заливки (Axial и Radial)
//       2. m_pRenderer->SetAdditionalParam(L"TilingHtmlPattern", oWriter.GetXmlString());
//       3. Подбор шрифтов необходимо перенести в GlobalParams->FindFontFile
//       4. В идентефикацию шрифта к путю добавить номер шрифта в файле

namespace PdfReader
{
    //--------------------------------------------------------------------------------------
    // CFontList
    //--------------------------------------------------------------------------------------
    CFontList::CFontList()
    {
        m_oCS.InitializeCriticalSection();
        m_oFontMap.clear();
    }
    CFontList::~CFontList()
    {
        m_oCS.DeleteCriticalSection();
        Clear();
    }
    void        CFontList::LoadFromFile(std::wstring wsDirPath)
    {
        return;
        //Clear();

        //CStringW wsFilePath = wsDirPath + CStringW( _T("/FontList.rsc") );
        //XmlUtils::CXmlNode oMainNode;

        //if ( !oMainNode.FromXmlFile( wsFilePath ) )
        //    return;

        //if ( _T("PDFFontList") == oMainNode.getName() )
        //{
        //    XmlUtils::CXmlNodes oFonts;
        //    oMainNode.GetNodes( _T("Font"), oFonts );
        //    for ( int nIndex = 0; nIndex < oFonts.GetCount(); nIndex++ )
        //    {
        //        XmlUtils::CXmlNode oFont;
        //        Ref oRef;
        //        CStringW sFilePath;
        //        int nLength = 0;
        //        unsigned short *pCodeToGid = NULL, *pCodeToUnicode = NULL;

        //        if ( oFonts.GetAt( nIndex, oFont ) )
        //        {
        //            XmlUtils::CXmlNode oNode;
        //            CStringW sValue;

        //            if ( oFont.GetNode( _T("ID"), oNode ) )
        //            {
        //                sValue = oNode.GetAttribute( _T("num") );
        //                oRef.num = XmlUtils::GetInteger( sValue );
        //                sValue = oNode.GetAttribute( _T("gen") );
        //                oRef.gen = XmlUtils::GetInteger( sValue );
        //            }

        //            if ( oFont.GetNode( _T("FilePath"), oNode ) )
        //            {
        //                sFilePath = oNode.GetAttribute( _T("value") );
        //            }
        //            if ( oFont.GetNode( _T("CodeToGid"), oNode ) )
        //            {
        //                sValue = oNode.GetAttribute( _T("length") );
        //                nLength = XmlUtils::GetInteger( sValue );
        //                pCodeToGid = (unsigned short *)MemUtilsMalloc( sizeof(unsigned short) * nLength );
        //                if ( !pCodeToGid )
        //                    return;

        //                XmlUtils::CXmlNodes oArray;
        //                oNode.GetNodes( _T("Entry"), oArray );
        //                for ( int nCurIndex = 0; nCurIndex < oArray.GetCount() && nCurIndex < nLength; nCurIndex++ )
        //                {
        //                    XmlUtils::CXmlNode oArrayItem;
        //                    if ( oArray.GetAt( nCurIndex, oArrayItem ) )
        //                    {
        //                        sValue = oArrayItem.GetAttribute( _T("value") );
        //                        pCodeToGid[nCurIndex] = XmlUtils::GetInteger( sValue );
        //                    }
        //                    else
        //                        pCodeToGid[nCurIndex] = 0;
        //                }
        //            }
        //            if ( oFont.GetNode( _T("CodeToUnicode"), oNode ) )
        //            {
        //                sValue = oNode.GetAttribute( _T("length") );
        //                nLength = XmlUtils::GetInteger( sValue );
        //                pCodeToUnicode = (int *)MemUtilsMalloc( sizeof(int) * nLength );
        //                if ( !pCodeToUnicode )
        //                    return;

        //                XmlUtils::CXmlNodes oArray;
        //                oNode.GetNodes( _T("Entry"), oArray );
        //                for ( int nCurIndex = 0; nCurIndex < oArray.GetCount() && nCurIndex < nLength; nCurIndex++ )
        //                {
        //                    XmlUtils::CXmlNode oArrayItem;
        //                    if ( oArray.GetAt( nCurIndex, oArrayItem ) )
        //                    {
        //                        sValue = oArrayItem.GetAttribute( _T("value") );
        //                        pCodeToUnicode[nCurIndex] = XmlUtils::GetInteger( sValue );
        //                    }
        //                    else
        //                        pCodeToUnicode[nCurIndex] = 0;
        //                }
        //            }

        //        }

        //        Add( oRef, sFilePath, pCodeToGid, pCodeToUnicode, nLength );
        //    }
        //}
    }
    void        CFontList::SaveToFile(std::wstring wsDirPath)
    {
        return;
        //CStringW wsFilePath = wsDirPath + CStringW( _T("/FontList.rsc") );
        //XmlUtils::CXmlWriter oWriter;

        //oWriter.WriteNodeBegin( _T("PDFFontList") );

        //for ( int nIndex = 0; nIndex < (int)m_arrList.GetCount(); nIndex++ )
        //{
        //    TFontEntry oEntry = m_arrList.GetAt( nIndex );
        //    oWriter.WriteNodeBegin( _T("Font") );


        //    oWriter.WriteNodeBegin( _T("ID"), true );
        //    oWriter.WriteAttribute( _T("num"), oEntry.oRef.num );
        //    oWriter.WriteAttribute( _T("gen"), oEntry.oRef.gen );
        //    oWriter.WriteNodeEnd( _T("ID"), true, true );

        //    oWriter.WriteNodeBegin( _T("FilePath"), true );
        //    oWriter.WriteAttribute( _T("value"), oEntry.wsFilePath );
        //    oWriter.WriteNodeEnd( _T("FilePath"), true, true );

        //    if ( NULL != oEntry.pCodeToGID )
        //    {
        //        oWriter.WriteNodeBegin( _T("CodeToGid"), true );
        //        oWriter.WriteAttribute( _T("length"), oEntry.nLen );
        //        oWriter.WriteNodeEnd( _T("CodeToGid"), true, false );
        //        for ( int nCurIndex = 0; nCurIndex < oEntry.nLen; nCurIndex++ )
        //        {
        //            oWriter.WriteNodeBegin( _T("Entry"), true );
        //            oWriter.WriteAttribute( _T("value"), oEntry.pCodeToGID[nCurIndex] );
        //            oWriter.WriteNodeEnd( _T("Entry"), true, true );
        //        }
        //        oWriter.WriteNodeEnd( _T("CodeToGid") );
        //    }

        //    if ( NULL != oEntry.pCodeToUnicode )
        //    {
        //        oWriter.WriteNodeBegin( _T("CodeToUnicode"), true );
        //        oWriter.WriteAttribute( _T("length"), oEntry.nLen );
        //        oWriter.WriteNodeEnd( _T("CodeToUnicode"), true, false );
        //        for ( int nCurIndex = 0; nCurIndex < oEntry.nLen; nCurIndex++ )
        //        {
        //            oWriter.WriteNodeBegin( _T("Entry"), true );
        //            oWriter.WriteAttribute( _T("value"), oEntry.pCodeToUnicode[nCurIndex] );
        //            oWriter.WriteNodeEnd( _T("Entry"), true, true );
        //        }
        //        oWriter.WriteNodeEnd( _T("CodeToUnicode") );
        //    }

        //    oWriter.WriteNodeEnd( _T("Font") );
        //}
        //oWriter.WriteNodeEnd( _T("PDFFontList") );

        //oWriter.SaveToFile( wsFilePath );
    }
    bool        CFontList::Find(Ref oRef, TFontEntry *pEntry)
    {
        CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

        bool bResult = (NULL != (pEntry = Lookup(oRef)));

        if (bResult)
        {
            // Шрифт нашелся, но пока им пользоваться нельзя, потому что он загружается в параллельном потоке
            while (!pEntry->bAvailable)
                NSThreads::Sleep(10);
        }

        RELEASEOBJECT(pCS);

        return bResult;
    }
    bool        CFontList::Find2(Ref oRef, TFontEntry **ppEntry)
    {
        CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

        bool bResult = (NULL != ((*ppEntry) = Lookup(oRef)));

        if (bResult)
        {
            // Шрифт нашелся, но пока им пользоваться нельзя, потому что он загружается в параллельном потоке
            while (!(*ppEntry)->bAvailable)
                NSThreads::Sleep(10);
        }

        if (!bResult)
        {
            (*ppEntry) = Add(oRef, L"", NULL, NULL, 0, 0);
            (*ppEntry)->bAvailable = false;
        }

        RELEASEOBJECT(pCS);

        return bResult;
    }
    TFontEntry* CFontList::Add(Ref oRef, std::wstring wsFileName, int *pCodeToGID, int *pCodeToUnicode, unsigned int unLenGID, unsigned int unLenUnicode)
    {
        // Данная функция приходит только из Find2, поэтому проверять есть ли данный шрифт уже не надо
        CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

        TFontEntry *pNewEntry = new TFontEntry;
        pNewEntry->oRef.gen      = oRef.gen;
        pNewEntry->oRef.num      = oRef.num;
        pNewEntry->wsFilePath     = wsFileName;
        pNewEntry->pCodeToGID     = pCodeToGID;
        pNewEntry->pCodeToUnicode = pCodeToUnicode;
        pNewEntry->unLenGID       = unLenGID;
        pNewEntry->unLenUnicode   = unLenUnicode;

        Add(oRef, pNewEntry);

        RELEASEOBJECT(pCS);

        return pNewEntry;
    }
    void CFontList::Remove(Ref oRef)
    {
        CRefFontMap::iterator oPos = m_oFontMap.find(oRef);
        if (m_oFontMap.end() != oPos)
        {
            TFontEntry *pEntry = oPos->second;
            if (NULL != pEntry)
            {
                MemUtilsFree(pEntry->pCodeToGID);
                MemUtilsFree(pEntry->pCodeToUnicode);
            }
            delete pEntry;
            m_oFontMap.erase(oPos);
        }
    }
    void        CFontList::Clear()
    {
        for (auto const &oIt : m_oFontMap)
        {
            TFontEntry *pEntry = oIt.second;
            if (NULL != pEntry)
            {
                MemUtilsFree(pEntry->pCodeToGID);
                MemUtilsFree(pEntry->pCodeToUnicode);
            }
            delete pEntry;
        }
        m_oFontMap.clear();
    }
    bool        CFontList::GetFont(Ref *pRef, TFontEntry *pEntry)
    {
        TFontEntry* pFindEntry = Lookup(*pRef);
        if (NULL == pFindEntry)
            return false;

        *pEntry = *pFindEntry;
        return true;
    }
    //--------------------------------------------------------------------------------------
    // RendererOutputDev
    //--------------------------------------------------------------------------------------
    RendererOutputDev::RendererOutputDev(IRenderer *pRenderer, NSFonts::IFontManager* pFontManager, CFontList *pFontList)
    {
        m_pFontManager  = pFontManager;
        m_pFontManager  = pFontManager;
        m_pFontList     = pFontList;

        m_bTiling       = false;

        //m_pBufferTextClip = NULL; tmpchange
        //m_sClip           = NULL;

        m_lRendererType = c_nUnknownRenderer;
        m_pRenderer     = pRenderer;

        if (NULL != m_pRenderer)
        {
            m_pRenderer->put_PenColor(0);
            m_pRenderer->put_PenAlpha(255);
            m_pRenderer->put_PenSize(1);

            m_pRenderer->put_FontName(L"Arial");
            m_pRenderer->put_FontStyle(0);
            m_pRenderer->put_FontSize(10.0);

            m_pRenderer->get_Type(&m_lRendererType);
        }

        m_pbBreak                   = NULL;
        m_bTransparentGroup         = false;
        m_bIsolatedTransparentGroup = false;
        m_bTransparentGroupSoftMask = false;
        m_bTransparentGroupSoftMaskEnd = false;

        m_bDrawOnlyText = false;
        m_bClipChanged = true;

        //m_oFontList.LoadFromFile( (GlobalParamsAdaptor*)globalParams->GetTempFolder() );
        //// Тестовый пример
        //m_pRenderer->NewPage();
        //m_pRenderer->BeginCommand( c_nPageType );

        //m_pRenderer->SetWidth( 100.0f );
        //m_pRenderer->SetHeight( 100.0f );

        //m_oFont.SetFontSize( 1.0 );
        //m_oPen.SetSize( 0.3 );
        //m_oPen.SetAlpha( 128 );
        //m_pRenderer->SetFont( m_oFont.ToXmlString().AllocSysString() );
        //m_pRenderer->SetShadow( m_oShadow.ToXmlString().AllocSysString() );
        //m_pRenderer->SetPen( m_oPen.ToXmlString().AllocSysString() );

        //m_pRenderer->PathCommandMoveTo( 0, 50 );
        //m_pRenderer->PathCommandLineTo( 100, 50 );
        //m_pRenderer->DrawPath( c_nStroke );

        //m_pRenderer->PathCommandMoveTo( 50, 0 );
        //m_pRenderer->PathCommandLineTo( 50, 100 );
        //m_pRenderer->DrawPath( c_nStroke );
        //
        //m_oPen.SetSize( 0.3 );
        //m_oPen.SetAlpha( 255 );
        //m_pRenderer->SetPen( m_oPen.ToXmlString().AllocSysString() );

        //IAVSOfficeRendererTemplate2 *pRenderer2 = NULL;
        //m_pRenderer->QueryInterface( __uuidof(AVSOfficeEditor::IAVSOfficeRendererTemplate2), (void**)&pRenderer2 );

        //double pNewTm[6], arrMatrix[6];
        //double dAscent  = 0.905;
        //double dDescent = -0.211;
        //double dKoef = ( dAscent - fabs( dDescent ) ) * 1;

        //double pCTM[6] = { 10, 0, 0, 10, 50, 50};
        //pNewTm[0] =  1;//pTm[0];
        //pNewTm[1] =  0;//pTm[1];
        //pNewTm[2] = -0;//pTm[2];
        //pNewTm[3] = -1;//pTm[3];
        //pNewTm[4] =  0 + 0 * dKoef;
        //pNewTm[5] =  0 + 1 * dKoef;

        //arrMatrix[0] =   pNewTm[0] * pCTM[0] + pNewTm[1] * pCTM[2];
        //arrMatrix[1] = -(pNewTm[0] * pCTM[1] + pNewTm[1] * pCTM[3]);
        //arrMatrix[2] =   pNewTm[2] * pCTM[0] + pNewTm[3] * pCTM[2];
        //arrMatrix[3] = -(pNewTm[2] * pCTM[1] + pNewTm[3] * pCTM[3]);
        //arrMatrix[4] =   pNewTm[4] * pCTM[0] + pNewTm[5] * pCTM[2] + pCTM[4];
        //arrMatrix[5] = -(pNewTm[4] * pCTM[1] + pNewTm[5] * pCTM[3] + pCTM[5]) + 100;

        //double  dAcsentFactor = ( ( fabs(-0.324) + fabs(1.005) ) - ( dAscent + fabs( dDescent ) ) ) / 2 + dAscent;
        //double dAscentShiftX = -arrMatrix[2] * dAcsentFactor;
        //double dAscentShiftY = -arrMatrix[3] * dAcsentFactor;

        ////arrMatrix[4] += dAscentShiftX;
        ////arrMatrix[5] += dAscentShiftY;


        //pRenderer2->SetTransform( arrMatrix[0], arrMatrix[1], arrMatrix[2], arrMatrix[3], arrMatrix[4], arrMatrix[5] );

        //m_pRenderer->PathCommandMoveTo( -0.664, -0.324 );
        //m_pRenderer->PathCommandLineTo(  2.000, -0.324 );
        //m_pRenderer->PathCommandLineTo(  2.000,  1.005 );
        //m_pRenderer->PathCommandLineTo( -0.664,  1.005 );
        //m_pRenderer->PathCommandClose();
        //m_pRenderer->DrawPath( c_nStroke );


        //m_pRenderer->CommandDrawText( _T("A"), 0, 0, 0, 0, 0 );

        //double dAlpha =  0*(3.14 / 180.f) * 160;
        //pRenderer2->SetTransform( cos( dAlpha ), -sin( dAlpha ), -sin( dAlpha ), -cos( dAlpha ), 50, 50  );
        //m_pRenderer->PathCommandMoveTo( 0, 0 );
        //m_pRenderer->PathCommandLineTo( 10, 0 );
        //m_pRenderer->PathCommandLineTo( 10, 10 );
        //m_pRenderer->PathCommandLineTo( 0, 10 );
        //m_pRenderer->PathCommandClose();
        //m_pRenderer->DrawPath( c_nStroke );

        //m_pRenderer->EndCommand( c_nPageType );
        //RELEASEINTERFACE( pRenderer2 );
    }
    RendererOutputDev::~RendererOutputDev()
    {
        m_pRenderer = NULL;

//        if (m_sClip)  tmpchange
//            delete m_sClip;

//        if (m_pBufferTextClip) tmpchange
//            delete m_pBufferTextClip;

    }
    void RendererOutputDev::startPage(int nPageIndex, GfxState *pGState)
    {
        m_pRenderer->BeginCommand(c_nPageType);

        // Переводим пункты в миллиметры
        m_arrMatrix[0] = 1; m_arrMatrix[1] = 0;
        m_arrMatrix[2] = 0; m_arrMatrix[3] = 1;
        m_arrMatrix[4] = 0; m_arrMatrix[5] = 0;

        m_bTransparentGroup = false;
        m_bIsolatedTransparentGroup = false;
        m_bTransparentGroupSoftMask = false;
        m_bTransparentGroupSoftMaskEnd = false;

        if (c_nHtmlRendrerer2 == m_lRendererType)
            m_bDrawOnlyText = (S_OK == m_pRenderer->CommandLong(c_nCommandLongTypeOnlyText, 0)) ? true : false;
        else if (c_nHtmlRendrererText == m_lRendererType)
            m_bDrawOnlyText = true;
        else
            m_bDrawOnlyText = false;
    }
    void RendererOutputDev::endPage()
    {
        m_pRenderer->EndCommand(c_nPageType);
    }
    void RendererOutputDev::saveState(GfxState *pGState)
    {
        m_sClip.push_back(GfxClip());
        m_bClipChanged = true;
        updateAll(pGState);
    }
    void RendererOutputDev::restoreState(GfxState *pGState)
    {
        if (!m_sClip.empty())
            m_sClip.pop_back();
        m_bClipChanged = true;
        updateAll(pGState);
    }
    void RendererOutputDev::updateCTM(GfxState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32)
    {
    }
    void RendererOutputDev::updateLineDash(GfxState *pGState)
    {
        double *pDash  = NULL;
        int     nSize  = 0;
        double  dStart = 0;
        pGState->getLineDash(&pDash, &nSize, &dStart);

        if (0 == nSize) // Solid
        {
            m_pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
            m_pRenderer->put_PenDashOffset(0);
        }
        else
        {
            for (int nIndex = 0; nIndex < nSize; ++nIndex)
            {
                pDash[nIndex] = PDFCoordsToMM(pDash[nIndex]);
            }

            m_pRenderer->PenDashPattern(pDash, (long)nSize);
            m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
            m_pRenderer->put_PenDashOffset(PDFCoordsToMM(dStart));
        }
    }
    void RendererOutputDev::updateFlatness(GfxState *pGState)
    {
    }
    void RendererOutputDev::updateLineJoin(GfxState *pGState)
    {
        int nJoinStyle = pGState->getLineJoin();
        if (1 == nJoinStyle)
            nJoinStyle = 2;
        else if (2 == nJoinStyle)
            nJoinStyle = 1;

        m_pRenderer->put_PenLineJoin(nJoinStyle);
    }
    void RendererOutputDev::updateLineCap(GfxState *pGState)
    {
        int nCapStyle = pGState->getLineCap();
        if (1 == nCapStyle)
            nCapStyle = 2;
        else if (2 == nCapStyle)
            nCapStyle = 1;

        m_pRenderer->put_PenLineStartCap(nCapStyle);
        m_pRenderer->put_PenLineEndCap(nCapStyle);
    }
    void RendererOutputDev::updateMiterLimit(GfxState *pGState)
    {
    }
    void RendererOutputDev::updateLineWidth(GfxState *pGState)
    {
        m_pRenderer->put_PenSize(PDFCoordsToMM(pGState->getLineWidth()));
    }
    void RendererOutputDev::updateStrokeAdjust(GfxState *pGState)
    {
    }
    void RendererOutputDev::updateFillColor(GfxState *pGState)
    {
        GfxColor *pColor = pGState->getFillColor();
        GfxColorSpace *pColorSpace = pGState->getFillColorSpace();

        GfxRGB c;

        pColorSpace->getRGB(pColor, &c, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
        DWORD dwColor = colToByte(c.r) + colToByte(c.g) * 0x100 + colToByte(c.b) * 0x100 * 0x100;
        m_pRenderer->put_BrushColor1(dwColor);
        m_pRenderer->put_BrushColor2(dwColor);
    }
    void RendererOutputDev::updateStrokeColor(GfxState *pGState)
    {
        GfxColor *pColor = pGState->getStrokeColor();
        GfxColorSpace *pColorSpace = pGState->getStrokeColorSpace();

        GfxRGB c;

        pColorSpace->getRGB(pColor, &c, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
        DWORD dwColor = colToByte(c.r) + colToByte(c.g) * 0x100 + colToByte(c.b) * 0x100 * 0x100;

        m_pRenderer->put_PenColor(dwColor);
    }
    void RendererOutputDev::updateBlendMode(GfxState *pGState)
    {
        NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer);
        if (!GRenderer)
            return;

        switch (pGState->getBlendMode())
        {
        case gfxBlendNormal:
            GRenderer->put_BlendMode(3);
            // agg::comp_op_src_over
            break;
        case gfxBlendMultiply:
            GRenderer->put_BlendMode(14);
            // agg::comp_op_multiply
            break;
        case gfxBlendScreen:
            GRenderer->put_BlendMode(15);
            // agg::comp_op_screen
            break;
        case gfxBlendOverlay:
            GRenderer->put_BlendMode(16);
            // agg::comp_op_overlay
            break;
        case gfxBlendDarken:
            GRenderer->put_BlendMode(17);
            // agg::comp_op_darken
            break;
        case gfxBlendLighten:
            GRenderer->put_BlendMode(18);
            // agg::comp_op_lighten
            break;
        case gfxBlendColorDodge:
            GRenderer->put_BlendMode(19);
            // agg::comp_op_color_dodge
            break;
        case gfxBlendColorBurn:
            GRenderer->put_BlendMode(20);
            // agg::comp_op_color_burn
            break;
        case gfxBlendHardLight:
            GRenderer->put_BlendMode(21);
            // agg::comp_op_hard_light
            break;
        case gfxBlendSoftLight:
            GRenderer->put_BlendMode(22);
            // agg::comp_op_soft_light
            break;
        case gfxBlendDifference:
            GRenderer->put_BlendMode(23);
            // agg::comp_op_difference
            break;
        case gfxBlendExclusion:
            GRenderer->put_BlendMode(24);
            // agg::comp_op_exclusion
            break;
        case gfxBlendHue:
        case gfxBlendSaturation:
        case gfxBlendColor:
        case gfxBlendLuminosity:
        default:
            GRenderer->put_BlendMode(3);
            // agg::comp_op_src_over
            break;
        }
    }
    void RendererOutputDev::updateFillOpacity(GfxState *pGState)
    {
        m_pRenderer->put_BrushAlpha1(std::min(255, std::max(0, int(pGState->getFillOpacity() * 255))));
        m_pRenderer->put_BrushAlpha2(std::min(255, std::max(0, int(pGState->getFillOpacity() * 255))));
    }
    void RendererOutputDev::updateStrokeOpacity(GfxState *pGState)
    {
        m_pRenderer->put_PenAlpha(std::min(255, std::max(0, int(pGState->getStrokeOpacity() * 255))));
    }
    void RendererOutputDev::updateAll(GfxState *pGState)
    {
        updateCTM(pGState, pGState->getCTM()[0], pGState->getCTM()[1], pGState->getCTM()[2], pGState->getCTM()[3], pGState->getCTM()[4], pGState->getCTM()[5]);
        updateLineDash(pGState);
        updateFlatness(pGState);
        updateLineJoin(pGState);
        updateLineCap(pGState);
        updateMiterLimit(pGState);
        updateLineWidth(pGState);
        updateStrokeAdjust(pGState);
        updateFillColorSpace(pGState);
        updateFillColor(pGState);
        updateStrokeColorSpace(pGState);
        updateStrokeColor(pGState);
        updateBlendMode(pGState);
        updateFillOpacity(pGState);
        updateStrokeOpacity(pGState);
        updateFont(pGState);
        updateClip(pGState);
    }
    void RendererOutputDev::updateRender(GfxState *pGState)
    {

    }
    void RendererOutputDev::updateFont(GfxState *pGState)
    {


        // Проверяем наличие списка со шрифтами
        if (NULL == m_pFontList)
            return;

        GfxFont *pFont = pGState->getFont();
        if (NULL == pFont)
            return;

        m_pRenderer->put_FontSize(pGState->getFontSize());
        //m_oFont.Size = pGState->getFontSize();



        std::wstring wsFileName = L"";
        std::wstring wsFontName = L"";
        TFontEntry *pEntry = NULL;
        // MEMERR string dealocation pEntry
        if (!m_pFontList->Find2((*pFont->getID()), &pEntry))
        {
            GfxFontType eFontType = pFont->getType();
            if (fontType3 == eFontType) // FontType3 обрабатывается отдельной командой
            {
                pEntry->bAvailable = true;
                return;
            }

            std::wstring wsTempFileName = L"";
            Ref oEmbRef;
            bool bFontSubstitution = false;
            std::wstring wsFontBaseName = NSStrings::GetStringFromUTF32(pFont->getName());
            if (wsFontBaseName.empty())
                wsFontBaseName = L"Helvetica";
            const unsigned char* pData14 = NULL;
            unsigned int nSize14 = 0;
        #ifdef FONTS_USE_ONLY_MEMORY_STREAMS
            CMemoryFontStream oMemoryFontStream;
        #endif
            // 1. Если шрифт внедренный, тогда скидываем его в темповый файл.
            // 2. Если шрифт лежит вне пдф, а в самом пдф есть ссылка на него, тогда используем эту ссылку.
            // 3. В противном случае подбираем шрифт.

            if (pFont->getEmbeddedFontID(&oEmbRef))
            {
                std::wstring wsExt;
                switch (pFont->getType())
                {
                    case fontType1:       wsExt = L".pfb_t1";    break;
                    case fontType1C:      wsExt = L".pfb_t1c";   break;
                    case fontType1COT:    wsExt = L".pfb_t1cot"; break;
                    case fontTrueType:    wsExt = L".ttf";       break;
                    case fontTrueTypeOT:  wsExt = L".otf";       break;
                    case fontCIDType0:    wsExt = L".cid_0";     break;
                    case fontCIDType0C:   wsExt = L".cid_0c";    break;
                    case fontCIDType0COT: wsExt = L".cid_0cot";  break;
                    case fontCIDType2:    wsExt = L".cid_2";     break;
                    case fontCIDType2OT:  wsExt = L".cid_2ot";   break;
                }

            #ifdef FONTS_USE_ONLY_MEMORY_STREAMS
                if (NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage())
                {
                     wsTempFileName = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->GenerateId();
                }
            #else
                FILE* pTempFile = NULL;
                if (!NSFile::CFileBinary::OpenTempFile(&wsTempFileName, &pTempFile, L"wb", (wchar_t*)wsExt.c_str(),
                                                       (wchar_t*)((GlobalParamsAdaptor *)globalParams)->GetTempFolder().c_str(), NULL))
                {
                    if (L"" != wsTempFileName)
                        NSFile::CFileBinary::Remove(wsTempFileName);

                    pEntry->bAvailable = true;
                    return;
                }
                wsTempFileName = UTF8_TO_U(NSSystemPath::NormalizePath(U_TO_UTF8(wsTempFileName)));
            #endif

                Object oReferenceObject, oStreamObject;
                oReferenceObject.initRef(oEmbRef.num, oEmbRef.gen);
                oReferenceObject.fetch(m_pXref, &oStreamObject);
                oReferenceObject.free();
                if (!oStreamObject.isStream())
                {
                    // Внедренный шрифт неправильно записан
                    oStreamObject.free();

                    #ifndef BUILDING_WASM_MODULE
                    fclose(pTempFile);

                    if (L"" != wsTempFileName)
                        NSFile::CFileBinary::Remove(wsTempFileName);
                    #endif

                    pEntry->bAvailable = true;
                    return;
                }
                oStreamObject.streamReset();

            #ifdef FONTS_USE_ONLY_MEMORY_STREAMS
                oMemoryFontStream.load(oStreamObject);
                NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Add(wsTempFileName, oMemoryFontStream.m_pData, (LONG)oMemoryFontStream.m_nSize, true);
            #else
                int nChar;
                while ((nChar = oStreamObject.streamGetChar()) != EOF)
                {
                    fputc(nChar, pTempFile);
                }
                fclose(pTempFile);
            #endif

                oStreamObject.streamClose();
                oStreamObject.free();
                wsFileName = wsTempFileName;

            #ifdef FONTS_USE_AFM_SETTINGS
                // Для шрифтов типа Type1 нужно дописать Afm файл с метриками
                if (fontType1 == pFont->getType() || fontType1C == pFont->getType() || fontType1COT == pFont->getType())
                {
                    std::wstring wsSplitFileName, wsSplitFileExt;
                    SpitPathExt(wsFileName, &wsSplitFileName, &wsSplitFileExt);
                    std::wstring wsAfmPath = wsSplitFileName + L".afm";

                    FILE* pFile = NSFile::CFileBinary::OpenFileNative(wsAfmPath, L"wb");
                    if (pFile)
                    {
                        Ref *pRef = pFont->getID();
                        Object oRefObject, oFontObject;
                        oRefObject.initRef(pRef->num, pRef->gen);
                        oRefObject.fetch(m_pXref, &oFontObject);
                        oRefObject.free();

                        if (oFontObject.isDict())
                        {
                            char *sFontName = NULL, *sFontFamily = NULL, *sFontStretch = NULL;
                            int nFontWeight = 0, nItalicAngle = 0, nAscent = 0, nDescent = 0, nLeading = 0;
                            int nCapHeight = 0, nXHeight = 0, nStemV = 0, nStemH = 0, nAvgWidth = 0, nMaxWidth = 0, nMissingWidth = 0;
                            Array *pBBox = NULL;
                            int arrBBox[4] ={ 0, 0, 0, 0 };

                            Dict *pFontDict = oFontObject.getDict();
                            Object oFontDescriptor;
                            if (pFontDict->lookup("FontDescriptor", &oFontDescriptor)->isDict())
                            {
                                Object oDictItem;
                                // FontName
                                oFontDescriptor.dictLookup("FontName", &oDictItem);
                                if (oDictItem.isName()) sFontName = oDictItem.getName();
                                oDictItem.free();

                                // FontFamily
                                oFontDescriptor.dictLookup("FontFamily", &oDictItem);
                                if (oDictItem.isName()) sFontFamily = oDictItem.getName();
                                oDictItem.free();

                                // FontStretch
                                oFontDescriptor.dictLookup("FontStretch", &oDictItem);
                                if (oDictItem.isName()) sFontStretch = oDictItem.getName();
                                oDictItem.free();

                                // FontWeight
                                oFontDescriptor.dictLookup("FontWeight", &oDictItem);
                                if (oDictItem.isInt()) nFontWeight = oDictItem.getInt();
                                oDictItem.free();

                                // FontBBox
                                oFontDescriptor.dictLookup("FontBBox", &oDictItem);
                                if (oDictItem.isArray()) pBBox = oDictItem.getArray();
                                if (4 == pBBox->getLength())
                                {
                                    for (int nIndex = 0; nIndex < 4; nIndex++)
                                    {
                                        Object oArrayItem;
                                        pBBox->get(nIndex, &oArrayItem);
                                        if (oArrayItem.isInt())
                                            arrBBox[nIndex] = oArrayItem.getInt();

                                        oArrayItem.free();
                                    }
                                }
                                oDictItem.free();

                                // ItalicAngle
                                oFontDescriptor.dictLookup("ItalicAngle", &oDictItem);
                                if (oDictItem.isInt()) nItalicAngle = oDictItem.getInt();
                                oDictItem.free();

                                // Ascent
                                oFontDescriptor.dictLookup("Ascent", &oDictItem);
                                if (oDictItem.isInt()) nAscent = oDictItem.getInt();
                                oDictItem.free();

                                // Leading
                                oFontDescriptor.dictLookup("Leading", &oDictItem);
                                if (oDictItem.isInt()) nLeading = oDictItem.getInt();
                                oDictItem.free();

                                // CapHeight
                                oFontDescriptor.dictLookup("CapHeight", &oDictItem);
                                if (oDictItem.isInt()) nCapHeight = oDictItem.getInt();
                                oDictItem.free();

                                // XHeight
                                oFontDescriptor.dictLookup("XHeight", &oDictItem);
                                if (oDictItem.isInt()) nXHeight = oDictItem.getInt();
                                oDictItem.free();

                                // StemV
                                oFontDescriptor.dictLookup("StemV", &oDictItem);
                                if (oDictItem.isInt()) nStemV = oDictItem.getInt();
                                oDictItem.free();

                                // StemH
                                oFontDescriptor.dictLookup("StemH", &oDictItem);
                                if (oDictItem.isInt()) nStemH = oDictItem.getInt();
                                oDictItem.free();

                                // Descent
                                oFontDescriptor.dictLookup("Descent", &oDictItem);
                                if (oDictItem.isInt()) nDescent = oDictItem.getInt();
                                oDictItem.free();

                                // AvgWidth
                                oFontDescriptor.dictLookup("AvgWidth", &oDictItem);
                                if (oDictItem.isInt()) nAvgWidth = oDictItem.getInt();
                                oDictItem.free();

                                // MaxWidth
                                oFontDescriptor.dictLookup("MaxWidth", &oDictItem);
                                if (oDictItem.isInt()) nMaxWidth = oDictItem.getInt();
                                oDictItem.free();

                                // MissingWidth
                                oFontDescriptor.dictLookup("MissingWidth", &oDictItem);
                                if (oDictItem.isInt()) nMissingWidth = oDictItem.getInt();
                                oDictItem.free();

                            }
                            oFontDescriptor.free();

                            fprintf(pFile, "StartFontMetrics 3.0\n");
                            if (NULL != sFontName) fprintf(pFile, "FontName %s\n", sFontName);
                            if (NULL != sFontFamily) fprintf(pFile, "FamilyName %s\n", sFontFamily);
                            if (nFontWeight >= 550) fprintf(pFile, "Weight Bold\n");

                            fprintf(pFile, "ItalicAngle %d\n", nItalicAngle);

                            fprintf(pFile, "FontBBox %d %d %d %d\n", arrBBox[0], arrBBox[1], arrBBox[2], arrBBox[3]);

                            fprintf(pFile, "CapHeight %d\n", nCapHeight);
                            fprintf(pFile, "XHeight %d\n", nXHeight);
                            fprintf(pFile, "Ascender %d\n", nAscent);
                            fprintf(pFile, "Descender %d\n", nDescent);
                            fprintf(pFile, "StdHW %d\n", nStemH);
                            fprintf(pFile, "StdHV %d\n", nStemV);

                            int nFirstChar = 0;
                            Object oDictItem;
                            pFontDict->lookup("FirstChar", &oDictItem);
                            if (oDictItem.isInt()) nFirstChar = oDictItem.getInt();
                            oDictItem.free();

                            int nLastChar = nFirstChar;
                            pFontDict->lookup("LastChar", &oDictItem);
                            if (oDictItem.isInt()) nLastChar = oDictItem.getInt();
                            oDictItem.free();

                            Array *pWidths = NULL;
                            pFontDict->lookup("Widths", &oDictItem);
                            if (oDictItem.isArray()) pWidths = oDictItem.getArray();

                            int nCount = nLastChar - nFirstChar + 1;
                            Gfx8BitFont *pT1Font = (Gfx8BitFont *)pFont;

                            if (NULL != pWidths)
                            {
                                int nWidthsCount = pWidths->getLength();
                                fprintf(pFile, "StartCharMetrics %d\n", nWidthsCount);
                                for (int nIndex = 0; nIndex < nWidthsCount; nIndex++)
                                {
                                    int nWidth = nMissingWidth;
                                    Object oArrayItem;
                                    pWidths->get(nIndex, &oArrayItem);
                                    if (oArrayItem.isInt()) nWidth = oArrayItem.getInt();
                                    oArrayItem.free();

                                    char **ppEncoding = pT1Font->getEncoding();


                                    if (NULL != ppEncoding && NULL != ppEncoding[nIndex])
                                        fprintf(pFile, "C %d ; WX %d ; N %s ;\n", nIndex + nFirstChar, nWidth, ppEncoding[nIndex]);
                                    else
                                        fprintf(pFile, "C %d ; WX %d ;\n", nIndex + nFirstChar, nWidth);
                                }
                                fprintf(pFile, "EndCharMetrics\n");
                            }
                            oDictItem.free();
                        }
                    }
                    fclose(pFile);
                }
            #endif

                // Загрузим сам файл со шрифтом, чтобы точно определить его тип
                if (!m_pFontManager->LoadFontFromFile(wsFileName, 0, 10, 72, 72))
                {
                    pEntry->bAvailable = true;
                    return;
                }

                std::wstring wsFontType = m_pFontManager->GetFontType();
                if (L"TrueType" == wsFontType)
                {
                    if (eFontType != fontType1COT   && eFontType != fontTrueType
                        && eFontType != fontTrueTypeOT && eFontType != fontCIDType0COT
                        && eFontType != fontCIDType2   && eFontType != fontCIDType2OT)
                    {
                        if (eFontType == fontType1 || eFontType == fontType1C)
                            eFontType = fontType1COT;
                        else if (eFontType == fontCIDType0 || eFontType == fontCIDType0C)
                            eFontType = fontCIDType0COT;
                    }
                }
                else if (L"Type 1" == wsFontType)
                {
                    if (eFontType != fontType1 && eFontType != fontType1C)
                    {
                        eFontType = fontType1;
                    }
                }
                else if (L"CID Type 1" == wsFontType)
                {
                    if (eFontType != fontCIDType0   && eFontType != fontCIDType0C
                        && eFontType != fontCIDType2OT && eFontType != fontCIDType0COT)
                    {
                        eFontType = fontCIDType0;
                    }
                }
                else if (L"CFF" == wsFontType)
                {
                    if (eFontType != fontType1C      && eFontType != fontType1COT
                        && eFontType != fontTrueTypeOT  && eFontType != fontCIDType0C
                        && eFontType != fontCIDType0COT && eFontType != fontCIDType2OT
                        && eFontType != fontCIDType2)
                    {
                        if (eFontType == fontType1 || eFontType == fontTrueType)
                            eFontType = fontType1C;
                        else if (eFontType == fontCIDType0)
                            eFontType = fontCIDType0C;
                    }
                }
            }
        #ifndef FONTS_USE_ONLY_MEMORY_STREAMS
            else if (PdfReader::GetBaseFont(wsFontBaseName, pData14, nSize14))
            {
                FILE* pFile = NULL;
                if (!NSFile::CFileBinary::OpenTempFile(&wsTempFileName, &pFile, L"wb", L".base",
                                                      (wchar_t*)((GlobalParamsAdaptor*)globalParams)->GetTempFolder().c_str(), NULL))
                {
                    if (!wsTempFileName.empty())
                        NSFile::CFileBinary::Remove(wsTempFileName);

                    pEntry->bAvailable = true;
                    return;
                }
                fclose(pFile);
                NSFile::CFileBinary oFile;
                oFile.CreateFileW(wsTempFileName);
                oFile.WriteFile((BYTE*)pData14, nSize14);
                oFile.CloseFile();
                wsFileName = wsTempFileName;

                eFontType = fontTrueType;
            }
        #else
            else if ([&oMemoryFontStream, wsFontBaseName]()
            {
                const unsigned char* pData14 = NULL;
                unsigned int nSize14 = 0;
                if (PdfReader::GetBaseFont(wsFontBaseName, pData14, nSize14))
                {
                     oMemoryFontStream.fromBuffer((BYTE*)pData14, nSize14);
                     return true;
                }
                return false;
            }())
            {
                wsFileName = wsFontBaseName;
                NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Add(wsFileName, oMemoryFontStream.m_pData, (LONG)oMemoryFontStream.m_nSize, true);
            }
        #endif
            else if (!pFont->locateFont(m_pXref, false) ||
                (wsFileName = NSStrings::GetStringFromUTF32(pFont->locateFont(m_pXref, false)->path)).length() == 0)
            //else if (0)
            {
                // TODO: Сначала тут мы должны проверить, если ищется один из 14 стандартных шрифтов,
                //       тогда мы должны вернуть путь к стандартному шрифту.

                NSFonts::CFontInfo* pFontInfo = NULL;
                if (m_pFontManager)
                {
                    Ref *pRef = pFont->getID();
                    Object oRefObject, oFontObject;
                    oRefObject.initRef(pRef->num, pRef->gen);
                    oRefObject.fetch(m_pXref, &oFontObject);
                    oRefObject.free();

                    NSFonts::CFontSelectFormat oFontSelect;
                    NSCorrectFontName::CheckFontNamePDF(wsFontBaseName, oFontSelect);
                    if (oFontObject.isDict())
                    {
                        Dict *pFontDict = oFontObject.getDict();
                        Object oFontDescriptor;
                        if (pFontDict->lookup("FontDescriptor", &oFontDescriptor)->isDict())
                        {
                            Object oDictItem;
                            // FontName
                            oFontDescriptor.dictLookup("FontName", &oDictItem);
                            if (oDictItem.isName())
                                oFontSelect.wsName = AStringToPWString(oDictItem.getName());
                            else
                                oFontSelect.wsName = new std::wstring(wsFontBaseName);
                            oDictItem.free();

                            // FontFamily
                            oFontDescriptor.dictLookup("FontFamily", &oDictItem);
                            oDictItem.free();

                            // FontStretch
                            oFontDescriptor.dictLookup("FontStretch", &oDictItem);
                            oDictItem.free();

                            // FontWeight
                            oFontDescriptor.dictLookup("FontWeight", &oDictItem);
                            oDictItem.free();

                            // FontBBox
                            oFontDescriptor.dictLookup("FontBBox", &oDictItem);
                            oDictItem.free();

                            // ItalicAngle
                            oFontDescriptor.dictLookup("ItalicAngle", &oDictItem);
                            if (oDictItem.isInt() && 0 != oDictItem.getInt())
                            {
                                if (oFontSelect.bItalic) RELEASEOBJECT(oFontSelect.bItalic);
                                oFontSelect.bItalic = new INT(1);
                            }
                            oDictItem.free();

                            // Ascent
                            oFontDescriptor.dictLookup("Ascent", &oDictItem);
                            if (oDictItem.isInt()) oFontSelect.shAscent = new SHORT(oDictItem.getInt());
                            oDictItem.free();

                            // Leading
                            oFontDescriptor.dictLookup("Leading", &oDictItem);
                            if (oDictItem.isInt()) oFontSelect.shLineGap = new SHORT(oDictItem.getInt());
                            oDictItem.free();

                            // CapHeight
                            oFontDescriptor.dictLookup("CapHeight", &oDictItem);
                            if (oDictItem.isInt()) oFontSelect.shCapHeight = new SHORT(oDictItem.getInt());
                            oDictItem.free();

                            // XHeight
                            oFontDescriptor.dictLookup("XHeight", &oDictItem);
                            if (oDictItem.isInt()) oFontSelect.shXHeight = new SHORT(oDictItem.getInt());
                            oDictItem.free();

                            // StemV
                            oFontDescriptor.dictLookup("StemV", &oDictItem);
                            oDictItem.free();

                            // StemH
                            oFontDescriptor.dictLookup("StemH", &oDictItem);
                            oDictItem.free();

                            // Descent
                            oFontDescriptor.dictLookup("Descent", &oDictItem);
                            if (oDictItem.isInt()) oFontSelect.shDescent = new SHORT(oDictItem.getInt());
                            oDictItem.free();

                            // AvgWidth
                            oFontDescriptor.dictLookup("AvgWidth", &oDictItem);
                            if (oDictItem.isInt()) oFontSelect.shAvgCharWidth = new SHORT(oDictItem.getInt());
                            oDictItem.free();

                            // MaxWidth
                            oFontDescriptor.dictLookup("MaxWidth", &oDictItem);
                            oDictItem.free();

                            // MissingWidth
                            oFontDescriptor.dictLookup("MissingWidth", &oDictItem);
                            oDictItem.free();

                        }
                        else
                            oFontSelect.wsName = new std::wstring(wsFontBaseName);
                        oFontDescriptor.free();
                    }
                    else
                        oFontSelect.wsName = new std::wstring(wsFontBaseName);

                    pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelect);
                }

                if (pFontInfo && L"" != pFontInfo->m_wsFontPath)
                {
                    wsFileName = pFontInfo->m_wsFontPath;
                    eFontType  = pFont->isCIDFont() ? fontCIDType2 : fontTrueType;

                #ifdef FONTS_USE_ONLY_MEMORY_STREAMS
                    if (NSWasm::IsJSEnv())
                        wsFileName = pFontInfo->m_wsFontName;

                    if (!wsFileName.empty())
                    {
                        wsFileName = NSWasm::LoadFont(wsFileName, pFontInfo->m_bBold, pFontInfo->m_bItalic);
                        if (wsFileName.empty())
                        {
                            m_pFontList->Remove(*pFont->getID());
                            return;
                        }
                    }
                    oMemoryFontStream.fromStream(wsFileName);
                #endif

                    bFontSubstitution = true;
                }
                else // В крайнем случае, в данном шрифте просто не пишем ничего
                {
                    pEntry->bAvailable = true;
                    return;
                }

                // Записываем файл с кодировкой. (Специально для перезаписи в PDF)
                if (c_nPDFWriter == m_lRendererType)
                {
                    std::wstring wsExt;
                    if (!pFont->isCIDFont())
                    {
                        wsExt = L".non";
                    }
                    else if (pFont->isCIDFont())
                    {
                        wsExt = L".cid_non";
                    }

                    FILE* pTempFile = NULL;
                    if (!NSFile::CFileBinary::OpenTempFile(&wsTempFileName, &pTempFile, L"wb", (wchar_t*)wsExt.c_str(),
                                                           (wchar_t*)((GlobalParamsAdaptor*)globalParams)->GetTempFolder().c_str(), NULL))
                    {
                        if (L"" != wsTempFileName)
                            NSFile::CFileBinary::Remove(wsTempFileName);

                        pEntry->bAvailable = true;
                        return;
                    }
                    fclose(pTempFile);

                    // Копируем файл, для создания уникального имени, чтобы связать с файлом с кодировкой
                    if (NSFile::CFileBinary::Copy(wsFileName, wsTempFileName))
                    {
                        wsFileName = wsTempFileName;
                    }
                    else
                    {
                        NSFile::CFileBinary::Remove(wsTempFileName);
                        wsTempFileName = L"";
                    }

                    if (L"" != wsTempFileName)
                    {
                        std::wstring wsSplitFileName, wsSplitFileExt;
                        SpitPathExt(wsFileName, &wsSplitFileName, &wsSplitFileExt);
                        std::wstring wsEncodingPath = wsSplitFileName + L".enc";

                        CXmlWriter oXmlWriter;

                        Ref *pRef = pFont->getID();
                        Object oRefObject, oFontObject;
                        oRefObject.initRef(pRef->num, pRef->gen);
                        oRefObject.fetch(m_pXref, &oFontObject);
                        oRefObject.free();

                        if (oFontObject.isDict())
                        {
                            Dict *pFontDict = oFontObject.getDict();

                            int nEncodingType = -1; // Объекта Encoding нет
                            int nBaseEncoding = -1;

                            Object oDictItem;
                            pFontDict->lookup("Encoding", &oDictItem);
                            if (oDictItem.isDict())
                            {
                                nEncodingType = 1; // Encoding - идет отдельным объектом

                                Object oTemp;
                                oDictItem.dictLookup("BaseEncoding", &oTemp);

                                if (oTemp.isName("MacRomanEncoding"))
                                    nBaseEncoding = 0;
                                else if (oTemp.isName("MacExpertEncoding"))
                                    nBaseEncoding = 1;
                                else if (oTemp.isName("WinAnsiEncoding"))
                                    nBaseEncoding = 2;

                                oTemp.free();
                            }
                            else if (oDictItem.isName("MacRomanEncoding"))
                            {
                                nEncodingType = 0;
                                nBaseEncoding = 0;
                            }
                            else if (oDictItem.isName("MacExpertEncoding"))
                            {
                                nEncodingType = 0;
                                nBaseEncoding = 1;
                            }
                            else if (oDictItem.isName("WinAnsiEncoding"))
                            {
                                nEncodingType = 0;
                                nBaseEncoding = 2;
                            }
                            else
                            {
                                nEncodingType = -1;
                            }

                            oXmlWriter.WriteNodeBegin(L"Encoding", true);
                            oXmlWriter.WriteAttribute(L"type", nEncodingType);
                            oXmlWriter.WriteAttribute(L"base", nBaseEncoding);
                            oXmlWriter.WriteNodeEnd(L"Encoding", true, false);

                            // Differences
                            if (oDictItem.isDict())
                            {
                                Object oDifferences;
                                oDictItem.dictLookup("Differences", &oDifferences);
                                if (oDifferences.isArray())
                                {
                                    int nArrayLen = oDifferences.arrayGetLength();
                                    oXmlWriter.WriteNodeBegin(L"Differences", true);
                                    oXmlWriter.WriteAttribute(L"count", nArrayLen);
                                    oXmlWriter.WriteNodeEnd(L"Differences", true, false);

                                    for (int nIndex = 0; nIndex < nArrayLen; ++nIndex)
                                    {
                                        Object oTemp;
                                        oDifferences.arrayGet(nIndex, &oTemp);
                                        if (oTemp.isInt())
                                        {
                                            int nCode = oTemp.getInt();
                                            oXmlWriter.WriteNodeBegin(L"Code", true);
                                            oXmlWriter.WriteAttribute(L"value", nCode);
                                            oXmlWriter.WriteNodeEnd(L"Code", true, true);
                                        }
                                        else if (oTemp.isName())
                                        {
                                            char *sName = oTemp.getName();
                                            oXmlWriter.WriteNodeBegin(L"Name", true);
                                            oXmlWriter.WriteAttribute(L"value", AStringToWString(sName));
                                            oXmlWriter.WriteNodeEnd(L"Name", true, true);
                                        }
                                        else
                                        {
                                            // TODO: Неправильный тип записи
                                        }
                                        oTemp.free();
                                    }

                                    oXmlWriter.WriteNodeEnd(L"Differences");
                                }
                                oDifferences.free();
                            }
                            oDictItem.free();
                            oXmlWriter.WriteNodeEnd(L"Encoding");
                        }
                        oFontObject.free();
                        oXmlWriter.SaveToFile(wsEncodingPath);
                    }
                }
            }
            // Здесь мы грузим кодировки
            int *pCodeToGID = NULL, *pCodeToUnicode = NULL;
            int nLen = 0;
            FoFiTrueType *pTTFontFile  = NULL;
            FoFiType1C   *pT1CFontFile = NULL;
        #ifdef FONTS_USE_ONLY_MEMORY_STREAMS
            FoFiIdentifierType fofiType = FoFiIdentifier::identifyStream(&readFromMemoryStream, &oMemoryFontStream);
            oMemoryFontStream.toStart();
        #else
            FoFiIdentifierType fofiType = FoFiIdentifier::identifyFile((char*)U_TO_UTF8(wsFileName).c_str());
        #endif

            switch (eFontType)
            {
                case fontType1:
                case fontType1C:
                case fontType1COT:
                {
					Gfx8BitFont* pFont8bit = NULL;
                    if (fofiType == fofiIdTrueType)
                    {
                    #ifdef FONTS_USE_ONLY_MEMORY_STREAMS
                        pTTFontFile = FoFiTrueType::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize, 0);
                    #else
                        pTTFontFile = FoFiTrueType::load((char*)U_TO_UTF8(wsFileName).c_str(), 0);
                    #endif

                        if (pTTFontFile)
                        {
                            pCodeToGID = ((Gfx8BitFont *)pFont)->getCodeToGIDMap(pTTFontFile);
                            nLen = 256;

                            delete pTTFontFile;
                            pTTFontFile = NULL;
                        }
                        else
                        {
                            pCodeToGID = NULL;
                            nLen = 0;
                        }
                    }
					else if (L"" != wsFileName && (pFont8bit = dynamic_cast<Gfx8BitFont*>(pFont)) && pFont8bit->getHasEncoding())
                    {					
						char **ppEncoding = pFont8bit->getEncoding();
                        if (!ppEncoding)
                            break;

                        if (!m_pFontManager)
                            break;

                        m_pFontManager->LoadFontFromFile(wsFileName, 0, 1, 72, 72);
                        pCodeToGID = (int *)MemUtilsMallocArray(256, sizeof(int));
                        if (!pCodeToGID)
                            break;

                        nLen = 256;
                        for (int nIndex = 0; nIndex < 256; ++nIndex)
                        {
                            pCodeToGID[nIndex] = 0;
                            char* sName = NULL;
                            if ((sName = ppEncoding[nIndex]))
                            {
                                unsigned short ushGID = m_pFontManager->GetNameIndex(AStringToWString(sName));
                                pCodeToGID[nIndex] = ushGID;
                            }
                        }
                    }
                    break;
                }
                case fontTrueType:
                case fontTrueTypeOT:
				{
					if (fofiType == fofiIdType1PFB)
                    {						
						Gfx8BitFont* pFont8bit = dynamic_cast<Gfx8BitFont*>(pFont);
						if (L"" != wsFileName && pFont8bit && pFont8bit->getHasEncoding())
                        {
							char **ppEncoding = pFont8bit->getEncoding();
                            if (!ppEncoding)
                                break;

                            if (!m_pFontManager)
                                break;

                            m_pFontManager->LoadFontFromFile(wsFileName, 0, 1, 72, 72);
                            pCodeToGID = (int *)MemUtilsMallocArray(256, sizeof(int));
                            if (!pCodeToGID)
                                break;

                            nLen = 256;
                            for (int nIndex = 0; nIndex < 256; ++nIndex)
                            {
                                pCodeToGID[nIndex] = 0;
                                char* sName = NULL;
								if ((sName = ppEncoding[nIndex]))
                                {
                                    unsigned short ushGID = m_pFontManager->GetNameIndex(AStringToWString(sName));
                                    pCodeToGID[nIndex] = ushGID;
                                }
                            }
                        }
                        break;
                    }

                #ifdef FONTS_USE_ONLY_MEMORY_STREAMS
                    pTTFontFile = FoFiTrueType::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize, 0);
                #else
                    pTTFontFile = FoFiTrueType::load((char*)U_TO_UTF8(wsFileName).c_str(), 0);
                #endif
                    if (pTTFontFile)
                    {
                        pCodeToGID = ((Gfx8BitFont *)pFont)->getCodeToGIDMap(pTTFontFile);
                        nLen = 256;

                        delete pTTFontFile;
                        pTTFontFile = NULL;
                    }
                    else
                    {
						pCodeToGID = NULL;
						nLen       = 0;

						if (m_pFontManager->LoadFontFromFile(wsFileName, 0, 10, 72, 72))
						{
							INT* pCodes = NULL;
							nLen = 256;
							pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
							for (int nCode = 0; nCode < nLen; ++nCode)
							{
								pCodeToGID[nCode] = m_pFontManager->GetGIDByUnicode(nCode);
							}
						}
                    }
                    break;
                }
                case fontCIDType0:
                case fontCIDType0C:
                {
                    //// TODO: Проверить, почему получение данной кодировки было отключено
                    //if ((pT1CFontFile = CFontFileType1C::LoadFromFile((wchar_t*)wsFileName.c_str())))
                    //{
                    //    pCodeToGID = pT1CFontFile->getCIDToGIDMap(&nLen);
                    //    delete pT1CFontFile;
                    //}
                    //else
                    //{
                        pCodeToGID = NULL;
                        nLen = 0;
                    //}
                    break;
                }
                case fontCIDType0COT:
                {
                    GfxCIDFont* pFontCID = dynamic_cast<GfxCIDFont*>(pFont);
                    if (!bFontSubstitution && pFontCID && pFontCID->getCIDToGID())
                    {
                        nLen = pFontCID->getCIDToGIDLen();
                        if (!nLen)
                            break;
                        pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
                        if (!pCodeToGID)
                        {
                            nLen = 0;
                            break;
                        }
                        memcpy(pCodeToGID, ((GfxCIDFont*)pFont)->getCIDToGID(), nLen * sizeof(int));
                        break;
                    }
                #ifdef FONTS_USE_ONLY_MEMORY_STREAMS
                    pTTFontFile = FoFiTrueType::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize, 0);
                #else
                    pTTFontFile = FoFiTrueType::load((char*)U_TO_UTF8(wsFileName).c_str(), 0);
                #endif

                    if (pTTFontFile)
                    {
                        if (pTTFontFile->isOpenTypeCFF())
                        {
                            pCodeToGID = pTTFontFile->getCIDToGIDMap(&nLen);
                        }
                        else
                        {
                            pCodeToGID = NULL;
                            nLen = 0;
                        }
                        delete pTTFontFile;
                        pTTFontFile = NULL;
                    }
                    else
                    {
                        pCodeToGID = NULL;
                        nLen = 0;
                    }
                    break;
                }
                case fontCIDType2:
                case fontCIDType2OT:
                {
                    // Создаем карту CID-to-GID
                    // Если у нас шрифт был не встроен и подбирался и есть мап ToUnicode, тогда на основе его читаем из файла гиды по юникодным значениям.
                    // Для встроенных шрифтов используем мап CIDtoGID
                    pCodeToGID = NULL;
                    nLen = 0;
                    if (L"" != wsFileName && bFontSubstitution)
                    {
                        CharCodeToUnicode *pCodeToUnicode = NULL;
                        if ((pCodeToUnicode = ((GfxCIDFont *)pFont)->getToUnicode()))
                        {
                        #ifdef FONTS_USE_ONLY_MEMORY_STREAMS
                            pTTFontFile = FoFiTrueType::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize, 0);
                        #else
                            pTTFontFile = FoFiTrueType::load((char*)U_TO_UTF8(wsFileName).c_str(), 0);
                        #endif
                            if (pTTFontFile)
                            {
                                // Ищем Unicode Cmap
                                std::vector<int> arrCMapIndex;
                                for (int nCMapIndex = 0; nCMapIndex < pTTFontFile->getNumCmaps(); ++nCMapIndex)
                                {
                                    if ((pTTFontFile->getCmapPlatform(nCMapIndex) == 3 && pTTFontFile->getCmapEncoding(nCMapIndex) == 1) || pTTFontFile->getCmapPlatform(nCMapIndex) == 0)
                                    {
                                        arrCMapIndex.push_back(nCMapIndex);
                                    }
                                }
                                if (arrCMapIndex.size() > 0)
                                {
                                    // CID -> Unicode -> GID
                                    nLen = pCodeToUnicode->getLength();
                                    pCodeToGID = (int *)MemUtilsMallocArray(nLen, sizeof(int));
                                    for (int nCode = 0; nCode < nLen; ++nCode)
                                    {
                                        Unicode arrUnicodeBuffer[8];
                                        if (pCodeToUnicode->mapToUnicode(nCode, arrUnicodeBuffer, 8) > 0)
                                        {
                                            pCodeToGID[nCode] = pTTFontFile->mapCodeToGID(arrCMapIndex[0], arrUnicodeBuffer[0]);
                                            for (size_t nIndex = 1; nIndex < arrCMapIndex.size(); nIndex++)
                                            {
                                                if (0 == pCodeToGID[nCode])
                                                    pCodeToGID[nCode] = pTTFontFile->mapCodeToGID(arrCMapIndex[nIndex], arrUnicodeBuffer[0]);
                                                else
                                                    break;
                                            }
                                        }
                                        else
                                        {
                                            pCodeToGID[nCode] = 0;
                                        }
                                    }
                                }
                                delete pTTFontFile;
                                pTTFontFile = NULL;
                            }
                            pCodeToUnicode->decRefCnt();
                        }
                    }
                    else if (((GfxCIDFont *)pFont)->getCIDToGID())
                    {
                        nLen = ((GfxCIDFont *)pFont)->getCIDToGIDLen();
                        pCodeToGID = (int *)MemUtilsMallocArray(nLen, sizeof(int));
                        if (!pCodeToGID)
                            break;

                        memcpy(pCodeToGID, ((GfxCIDFont *)pFont)->getCIDToGID(), nLen * sizeof(int));
                    }

                    break;
                }
                default:
                {
                    // Такого не должно произойти
                #ifndef FONTS_USE_ONLY_MEMORY_STREAMS
                    if (L"" != wsTempFileName)
                        NSFile::CFileBinary::Remove(wsTempFileName);
                #endif

                    break;
                }
            }
            // Составляем таблицу Code -> Unicode
            int nToUnicodeLen = 0;
            if (pFont->isCIDFont())
            {
                GfxCIDFont *pCIDFont = (GfxCIDFont *)pFont;
                CharCodeToUnicode *pToUnicode = pCIDFont->getToUnicode();
                if (NULL != pToUnicode)
                {
                    nToUnicodeLen = pToUnicode->getLength();
                    pCodeToUnicode = (int *)MemUtilsMallocArray(nToUnicodeLen, sizeof(int));

                    if (pCodeToUnicode)
                    {
                        for (int nIndex = 0; nIndex < nToUnicodeLen; ++nIndex)
                        {
                            Unicode aUnicode[2];
                            if (pToUnicode->mapToUnicode(nIndex, aUnicode, 2))
                                pCodeToUnicode[nIndex] = (unsigned short)aUnicode[0];
                            else
                                pCodeToUnicode[nIndex] = 0;
                        }
                    }

                    pToUnicode->decRefCnt();
                }
            }
            else
            {
                // memory troubles here

                CharCodeToUnicode *pToUnicode = ((Gfx8BitFont *)pFont)->getToUnicode();
                if (NULL != pToUnicode)
                {
                    nToUnicodeLen = pToUnicode->getLength();
                    pCodeToUnicode = (int *)MemUtilsMallocArray(nToUnicodeLen, sizeof(int));//literally here

                    if (pCodeToUnicode)
                    {
                        for (int nIndex = 0; nIndex < nToUnicodeLen; ++nIndex)
                        {
                            Unicode nUnicode = 0;
                            if (pToUnicode->mapToUnicode(nIndex, &nUnicode, 1))
                                pCodeToUnicode[nIndex] = (unsigned short)nUnicode;
                            else
                                pCodeToUnicode[nIndex] = nIndex;
                        }
                    }
                    pToUnicode->decRefCnt();
                }
            }

            // Записываем файл с настройками шрифта (Специально для перезаписи в PDF)
            if (L"" != wsFileName && c_nPDFWriter == m_lRendererType)
            {
                std::wstring wsSplitFileName, wsSplitFileExt;
                SpitPathExt(wsFileName, &wsSplitFileName, &wsSplitFileExt);
                std::wstring wsEncodingPath = wsSplitFileName + L".enc";

                GfxFontType eFontType = pFont->getType();
                if (fontType1 == eFontType || fontType1C == eFontType || fontType1COT == eFontType || fontTrueType == eFontType || fontTrueTypeOT == eFontType)
                {
                    // Запись информации для простых шрифтов
                    CXmlWriter oXmlWriter;

                    Ref *pRef = pFont->getID();
                    Object oRefObject, oFontObject;
                    oRefObject.initRef(pRef->num, pRef->gen);
                    oRefObject.fetch(m_pXref, &oFontObject);
                    oRefObject.free();

                    if (oFontObject.isDict())
                    {
                        Dict *pFontDict = oFontObject.getDict();

                        int nEncodingType = -1; // Объекта Encoding нет
                        int nBaseEncoding = -1;

                        Object oDictItem;
                        pFontDict->lookup("Encoding", &oDictItem);
                        if (oDictItem.isDict())
                        {
                            nEncodingType = 1; // Encoding - идет отдельным объектом

                            Object oTemp;
                            oDictItem.dictLookup("BaseEncoding", &oTemp);

                            if (oTemp.isName("MacRomanEncoding"))
                                nBaseEncoding = 0;
                            else if (oTemp.isName("MacExpertEncoding"))
                                nBaseEncoding = 1;
                            else if (oTemp.isName("WinAnsiEncoding"))
                                nBaseEncoding = 2;

                            oTemp.free();
                        }
                        else if (oDictItem.isName("MacRomanEncoding"))
                        {
                            nEncodingType = 0;
                            nBaseEncoding = 0;
                        }
                        else if (oDictItem.isName("MacExpertEncoding"))
                        {
                            nEncodingType = 0;
                            nBaseEncoding = 1;
                        }
                        else if (oDictItem.isName("WinAnsiEncoding"))
                        {
                            nEncodingType = 0;
                            nBaseEncoding = 2;
                        }
                        else
                        {
                            nEncodingType = -1;
                        }

                        oXmlWriter.WriteNodeBegin(L"PDF-resources");
                        oXmlWriter.WriteNodeBegin(L"Encoding", true);
                        oXmlWriter.WriteAttribute(L"type", nEncodingType);
                        oXmlWriter.WriteAttribute(L"base", nBaseEncoding);
                        oXmlWriter.WriteNodeEnd(L"Encoding", true, false);

                        // Differences
                        if (oDictItem.isDict())
                        {
                            Object oDifferences;
                            oDictItem.dictLookup("Differences", &oDifferences);
                            if (oDifferences.isArray())
                            {
                                int nArrayLen = oDifferences.arrayGetLength();
                                oXmlWriter.WriteNodeBegin(L"Differences", true);
                                oXmlWriter.WriteAttribute(L"count", nArrayLen);
                                oXmlWriter.WriteNodeEnd(L"Differences", true, false);

                                for (int nIndex = 0; nIndex < nArrayLen; ++nIndex)
                                {
                                    Object oTemp;
                                    oDifferences.arrayGet(nIndex, &oTemp);
                                    if (oTemp.isInt())
                                    {
                                        int nCode = oTemp.getInt();
                                        oXmlWriter.WriteNodeBegin(L"Code", true);
                                        oXmlWriter.WriteAttribute(L"value", nCode);
                                        oXmlWriter.WriteNodeEnd(L"Code", true, true);
                                    }
                                    else if (oTemp.isName())
                                    {
                                        char* sName = oTemp.getName();
                                        oXmlWriter.WriteNodeBegin(L"Name", true);
                                        oXmlWriter.WriteAttribute(L"value", AStringToWString(sName));
                                        oXmlWriter.WriteNodeEnd(L"Name", true, true);
                                    }
                                    else
                                    {
                                        // TO DO: Error "Wrong type in font encoding resource differences"
                                    }
                                    oTemp.free();
                                }

                                oXmlWriter.WriteNodeEnd(L"Differences");
                            }
                            oDifferences.free();
                        }
                        oDictItem.free();
                        oXmlWriter.WriteNodeEnd(L"Encoding");

                        pFontDict->lookup("BaseFont", &oDictItem);
                        if (oDictItem.isName())
                        {
                            oXmlWriter.WriteNodeBegin(L"FontBase", true);
                            oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.getName()));
                            oXmlWriter.WriteNodeEnd(L"FontBase", true, true);
                        }
                        oDictItem.free();

                        pFontDict->lookup("ToUnicode", &oDictItem);
                        if (oDictItem.isStream())
                        {
                            oXmlWriter.WriteNodeBegin(L"ToUnicode");

                            std::string sBuffer;
                            if (true)
                            {
                                oDictItem.streamReset();
                                int nChar = 0;
                                while ((nChar = oDictItem.streamGetChar()) != EOF)
                                {
                                    sBuffer+=(char)nChar;
                                }
                                oDictItem.streamClose();

                                CBase64 oBase64;
                                oBase64.Encode((unsigned char*)sBuffer.c_str(), sBuffer.length());

                                oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));
                            }

                            oXmlWriter.WriteNodeEnd(L"ToUnicode");
                        }
                        oDictItem.free();


                        oXmlWriter.WriteNodeBegin(L"FontDescriptor");

                        Object oFontDescriptor;
                        if (pFontDict->lookup("FontDescriptor", &oFontDescriptor)->isDict())
                        {
                            // FontName
                            oFontDescriptor.dictLookup("FontName", &oDictItem);
                            if (oDictItem.isName())
                            {
                                oXmlWriter.WriteNodeBegin(L"FontName", true);
                                oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.getName()));
                                oXmlWriter.WriteNodeEnd(L"FontName", true);
                            }
                            oDictItem.free();

                            // FontFamily
                            oFontDescriptor.dictLookup("FontFamily", &oDictItem);
                            if (oDictItem.isName())
                            {
                                oXmlWriter.WriteNodeBegin(L"FontFamily", true);
                                oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.getName()));
                                oXmlWriter.WriteNodeEnd(L"FontFamily", true);
                            }
                            oDictItem.free();

                            // FontStretch
                            oFontDescriptor.dictLookup("FontStretch", &oDictItem);
                            if (oDictItem.isName())
                            {
                                oXmlWriter.WriteNodeBegin(L"FontStretch", true);
                                oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.getName()));
                                oXmlWriter.WriteNodeEnd(L"FontStretch", true);
                            }
                            oDictItem.free();

                            // FontWeight
                            oFontDescriptor.dictLookup("FontWeight", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"FontWeight", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"FontWeight", true);
                            }
                            oDictItem.free();

                            // FontWeight
                            oFontDescriptor.dictLookup("Flags", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"Flags", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"Flags", true);
                            }
                            oDictItem.free();

                            // FontBBox
                            oFontDescriptor.dictLookup("FontBBox", &oDictItem);
                            if (oDictItem.isArray())
                            {
                                Array *pBBox = oDictItem.getArray();
                                if (NULL != pBBox && 4 == pBBox->getLength())
                                {
                                    oXmlWriter.WriteNodeBegin(L"FontBBox", true);
                                    for (int nIndex = 0; nIndex < 4; nIndex++)
                                    {
                                        Object oArrayItem;
                                        pBBox->get(nIndex, &oArrayItem);
                                        if (oArrayItem.isInt())
                                        {
                                            std::wstring wsValue = L"value" + std::to_wstring(nIndex);
                                            oXmlWriter.WriteAttribute(wsValue, oArrayItem.getInt());
                                        }

                                        oArrayItem.free();
                                    }
                                    oXmlWriter.WriteNodeEnd(L"FontBBox", true);
                                }
                            }
                            oDictItem.free();

                            // ItalicAngle
                            oFontDescriptor.dictLookup("ItalicAngle", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"ItalicAngle", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"ItalicAngle", true);
                            }
                            oDictItem.free();

                            // Ascent
                            oFontDescriptor.dictLookup("Ascent", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"Ascent", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"Ascent", true);
                            }
                            oDictItem.free();

                            // Leading
                            oFontDescriptor.dictLookup("Leading", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"Leading", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"Leading", true);
                            }
                            oDictItem.free();

                            // CapHeight
                            oFontDescriptor.dictLookup("CapHeight", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"CapHeight", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"CapHeight", true);
                            }
                            oDictItem.free();

                            // XHeight
                            oFontDescriptor.dictLookup("XHeight", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"XHeight", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"XHeight", true);
                            }
                            oDictItem.free();

                            // StemV
                            oFontDescriptor.dictLookup("StemV", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"StemV", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"StemV", true);
                            }
                            oDictItem.free();

                            // StemH
                            oFontDescriptor.dictLookup("StemH", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"StemH", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"StemH", true);
                            }
                            oDictItem.free();

                            // Descent
                            oFontDescriptor.dictLookup("Descent", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"Descent", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"Descent", true);
                            }
                            oDictItem.free();

                            // AvgWidth
                            oFontDescriptor.dictLookup("AvgWidth", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"AvgWidth", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"AvgWidth", true);
                            }
                            oDictItem.free();

                            // MaxWidth
                            oFontDescriptor.dictLookup("MaxWidth", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"MaxWidth", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"MaxWidth", true);
                            }
                            oDictItem.free();

                            // MissingWidth
                            oFontDescriptor.dictLookup("MissingWidth", &oDictItem);
                            if (oDictItem.isInt())
                            {
                                oXmlWriter.WriteNodeBegin(L"MissingWidth", true);
                                oXmlWriter.WriteAttribute(L"value", oDictItem.getInt());
                                oXmlWriter.WriteNodeEnd(L"MissingWidth", true);
                            }
                            oDictItem.free();

                        }
                        oFontDescriptor.free();
                        oXmlWriter.WriteNodeEnd(L"FontDescriptor");

                        int nFirstChar = 0;
                        pFontDict->lookup("FirstChar", &oDictItem);
                        if (oDictItem.isInt()) nFirstChar = oDictItem.getInt();
                        oDictItem.free();

                        int nLastChar = 0;
                        pFontDict->lookup("LastChar", &oDictItem);
                        if (oDictItem.isInt()) nLastChar = oDictItem.getInt();
                        oDictItem.free();

                        Array *pWidths = NULL;
                        pFontDict->lookup("Widths", &oDictItem);
                        if (oDictItem.isArray())
                        {
                            oXmlWriter.WriteNodeBegin(L"Widths", true);
                            oXmlWriter.WriteAttribute(L"FirstChar", nFirstChar);
                            oXmlWriter.WriteAttribute(L"LastChar", nLastChar);
                            oXmlWriter.WriteNodeEnd(L"Widths", true, false);

                            Array *pWidths = oDictItem.getArray();
                            int nWidthsCount = pWidths->getLength();
                            for (int nIndex = 0; nIndex < nWidthsCount; nIndex++)
                            {
                                Object oArrayItem;
                                pWidths->get(nIndex, &oArrayItem);
                                if (oArrayItem.isInt())
                                {
                                    oXmlWriter.WriteNodeBegin(L"Width", true);
                                    oXmlWriter.WriteAttribute(L"value", oArrayItem.getInt());
                                    oXmlWriter.WriteNodeEnd(L"Width", true);
                                }
                                oArrayItem.free();
                            }
                            oXmlWriter.WriteNodeEnd(L"Widths");
                        }
                        oDictItem.free();
                        oXmlWriter.WriteNodeEnd(L"PDF-resources");
                    }
                    oFontObject.free();
                    oXmlWriter.SaveToFile(wsEncodingPath);
                }
                else if (fontCIDType0 == eFontType || fontCIDType0C == eFontType || fontCIDType0COT == eFontType || fontCIDType2 == eFontType || fontCIDType2OT == eFontType)
                {
                    // Пишем файл с кодировкой CMap
                    std::wstring wsCMapPath = wsSplitFileName + L".cmap";
                    if (pFont->isCIDFont())
                    {
                        GfxCIDFont *pCIDFont = (GfxCIDFont *)pFont;
//                        if (NULL != pCIDFont->GetCMap()) todo toxml
//                            pCIDFont->GetCMap()->ToXml(wsCMapPath);
                    }

                    CXmlWriter oXmlWriter;
                    Ref *pRef = pFont->getID();
                    Object oRefObject, oFontObject;
                    oRefObject.initRef(pRef->num, pRef->gen);
                    oRefObject.fetch(m_pXref, &oFontObject);
                    oRefObject.free();
                    if (oFontObject.isDict())
                    {
                        Dict *pFontDict = oFontObject.getDict();
                        oXmlWriter.WriteNodeBegin(L"PDF-resources");
                        Object oDictItem;
                        pFontDict->lookup("BaseFont", &oDictItem);
                        if (oDictItem.isName())
                        {
                            oXmlWriter.WriteNodeBegin(L"Type0", true);
                            oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.getName()));
                            oXmlWriter.WriteNodeEnd(L"Type0", true, false);
                        }
                        else
                        {
                            oXmlWriter.WriteNodeBegin(L"Type0");
                        }
                        oDictItem.free();

                        pFontDict->lookup("ToUnicode", &oDictItem);
                        if (oDictItem.isStream())
                        {
                            oXmlWriter.WriteNodeBegin(L"ToUnicode");

                            std::string sBuffer;
                            oDictItem.streamReset();
                            int nChar = 0;
                            while ((nChar = oDictItem.streamGetChar()) != EOF)
                            {
                                sBuffer+=(char)nChar;
                            }
                            oDictItem.streamClose();

                            CBase64 oBase64;
                            oBase64.Encode((unsigned char*)sBuffer.c_str(), sBuffer.length());

                            oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));
                            oXmlWriter.WriteNodeEnd(L"ToUnicode");
                        }
                        oDictItem.free();

                        pFontDict->lookup("Encoding", &oDictItem);
                        if (oDictItem.isName())
                        {
                            oXmlWriter.WriteNodeBegin(L"Encoding", true);
                            oXmlWriter.WriteAttribute(L"name", AStringToWString(oDictItem.getName()));
                            oXmlWriter.WriteNodeEnd(L"Encoding", true, true);
                        }
                        else if (oDictItem.isStream())
                        {
                            oXmlWriter.WriteNodeBegin(L"Encoding");

                            Dict *pEncodingDict = oDictItem.streamGetDict();
                            if (NULL != pEncodingDict)
                            {
                                Object oEncItem;
                                pEncodingDict->lookup("CMapName", &oEncItem);
                                if (oEncItem.isName())
                                {
                                    oXmlWriter.WriteNodeBegin(L"CMapName", true);
                                    oXmlWriter.WriteAttribute(L"name", AStringToWString(oEncItem.getName()));
                                    oXmlWriter.WriteNodeEnd(L"CMapName", true, true);
                                }
                                oEncItem.free();

                                pEncodingDict->lookup("CIDSystemInfo", &oEncItem);
                                if (oEncItem.isDict())
                                {
                                    Dict *pCIDInfo = oEncItem.getDict();

                                    if (NULL != pCIDInfo)
                                    {
                                        oXmlWriter.WriteNodeBegin(L"CIDSystemInfo");

                                        Object oCIDInfoItem;
                                        pCIDInfo->lookup("Registry", &oCIDInfoItem);
                                        if (oCIDInfoItem.isString())
                                        {
                                            oXmlWriter.WriteNodeBegin(L"Registry", true);
                                            oXmlWriter.WriteAttribute(L"string", AStringToWString(oCIDInfoItem.getString()->getCString()));
                                            oXmlWriter.WriteNodeEnd(L"Registry", true, true);
                                        }
                                        oCIDInfoItem.free();

                                        pCIDInfo->lookup("Ordering", &oCIDInfoItem);
                                        if (oCIDInfoItem.isString())
                                        {
                                            oXmlWriter.WriteNodeBegin(L"Ordering", true);
                                            oXmlWriter.WriteAttribute(L"string", AStringToWString(oCIDInfoItem.getString()->getCString()));
                                            oXmlWriter.WriteNodeEnd(L"Ordering", true, true);
                                        }
                                        oCIDInfoItem.free();

                                        pCIDInfo->lookup("Supplement", &oCIDInfoItem);
                                        if (oCIDInfoItem.isInt())
                                        {
                                            oXmlWriter.WriteNodeBegin(L"Supplement", true);
                                            oXmlWriter.WriteAttribute(L"integer", oCIDInfoItem.getInt());
                                            oXmlWriter.WriteNodeEnd(L"Supplement", true, true);
                                        }
                                        oCIDInfoItem.free();

                                        oXmlWriter.WriteNodeEnd(L"CIDSystemInfo");
                                    }
                                }
                                oEncItem.free();

                                pEncodingDict->lookup("WMode", &oEncItem);
                                if (oEncItem.isInt())
                                {
                                    oXmlWriter.WriteNodeBegin(L"WMode", true);
                                    oXmlWriter.WriteAttribute(L"integer", oEncItem.getInt());
                                    oXmlWriter.WriteNodeEnd(L"WMode", true, true);
                                }
                                oEncItem.free();

                                pEncodingDict->lookup("UseCMap", &oEncItem);
                                if (oEncItem.isName())
                                {
                                    oXmlWriter.WriteNodeBegin(L"UseCMap", true);
                                    oXmlWriter.WriteAttribute(L"name", AStringToWString(oEncItem.getName()));
                                    oXmlWriter.WriteNodeEnd(L"UseCMap", true, true);
                                }
                                else if (oEncItem.isStream())
                                {
                                    oXmlWriter.WriteNodeBegin(L"UseCMap");

                                    std::string sBuffer;
                                    oEncItem.streamReset();
                                    int nChar = 0;
                                    while ((nChar = oEncItem.streamGetChar()) != EOF)
                                    {
                                        sBuffer+=(char)nChar;
                                    }
                                    oEncItem.streamClose();

                                    CBase64 oBase64;
                                    oBase64.Encode((unsigned char*)sBuffer.c_str(), sBuffer.length());

                                    oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));

                                    oXmlWriter.WriteNodeEnd(L"UseCMap");
                                }
                                oEncItem.free();
                            }

                            oXmlWriter.WriteNodeBegin(L"Stream");
                            std::string sBuffer;
                            oDictItem.streamReset();
                            int nChar = 0;
                            while ((nChar = oDictItem.streamGetChar()) != EOF)
                            {
                                sBuffer+=(char)nChar;
                            }
                            oDictItem.streamClose();

                            CBase64 oBase64;
                            oBase64.Encode((unsigned char*)sBuffer.c_str(), sBuffer.length());

                            oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));

                            oXmlWriter.WriteNodeEnd(L"Stream");
                            oXmlWriter.WriteNodeEnd(L"Encoding");
                        }
                        oDictItem.free();

                        pFontDict->lookup("DescendantFonts", &oDictItem);
                        if (oDictItem.isArray())
                        {
                            Array *pArray =  oDictItem.getArray();
                            if (1 == pArray->getLength())
                            {
                                Object oDescFont;
                                pArray->get(0, &oDescFont);

                                if (oDescFont.isDict())
                                {
                                    Dict *pDescFont = oDescFont.getDict();
                                    if (NULL != pDescFont)
                                    {
                                        oXmlWriter.WriteNodeBegin(L"DescendantFonts");

                                        Object oFontItem;
                                        pDescFont->lookup("Subtype", &oFontItem);
                                        if (oFontItem.isName())
                                        {
                                            oXmlWriter.WriteNodeBegin(L"Subtype", true);
                                            oXmlWriter.WriteAttribute(L"name", AStringToWString(oFontItem.getName()));
                                            oXmlWriter.WriteNodeEnd(L"Subtype", true, true);
                                        }
                                        oFontItem.free();

                                        pDescFont->lookup("BaseFont", &oFontItem);
                                        if (oFontItem.isName())
                                        {
                                            oXmlWriter.WriteNodeBegin(L"BaseFont", true);
                                            oXmlWriter.WriteAttribute(L"name", AStringToWString(oFontItem.getName()));
                                            oXmlWriter.WriteNodeEnd(L"BaseFont", true, true);
                                        }
                                        oFontItem.free();

                                        pDescFont->lookup("CIDSystemInfo", &oFontItem);
                                        if (oFontItem.isDict())
                                        {
                                            Dict *pCIDInfo = oFontItem.getDict();
                                            if (NULL != pCIDInfo)
                                            {
                                                oXmlWriter.WriteNodeBegin(L"CIDSystemInfo");

                                                Object oCIDInfoItem;
                                                pCIDInfo->lookup("Registry", &oCIDInfoItem);
                                                if (oCIDInfoItem.isString())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"Registry", true);
                                                    oXmlWriter.WriteAttribute(L"string", AStringToWString(oCIDInfoItem.getString()->getCString()));
                                                    oXmlWriter.WriteNodeEnd(L"Registry", true, true);
                                                }
                                                oCIDInfoItem.free();

                                                pCIDInfo->lookup("Ordering", &oCIDInfoItem);
                                                if (oCIDInfoItem.isString())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"Ordering", true);
                                                    oXmlWriter.WriteAttribute(L"string", AStringToWString(oCIDInfoItem.getString()->getCString()));
                                                    oXmlWriter.WriteNodeEnd(L"Ordering", true, true);
                                                }
                                                oCIDInfoItem.free();

                                                pCIDInfo->lookup("Supplement", &oCIDInfoItem);
                                                if (oCIDInfoItem.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"Supplement", true);
                                                    oXmlWriter.WriteAttribute(L"integer", oCIDInfoItem.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"Supplement", true, true);
                                                }
                                                oCIDInfoItem.free();

                                                oXmlWriter.WriteNodeEnd(L"CIDSystemInfo");
                                            }
                                        }
                                        oFontItem.free();

                                        pDescFont->lookup("FontDescriptor", &oFontItem);
                                        if (oFontItem.isDict())
                                        {
                                            Dict *pFontDescriptor = oFontItem.getDict();
                                            if (NULL != pFontDescriptor)
                                            {
                                                oXmlWriter.WriteNodeBegin(L"FontDescriptor");
                                                Object oItemFD;

                                                pFontDescriptor->lookup("FontName", &oItemFD);
                                                if (oItemFD.isName())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"FontName", true);
                                                    oXmlWriter.WriteAttribute(L"name", AStringToWString(oItemFD.getName()));
                                                    oXmlWriter.WriteNodeEnd(L"FontName", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("FontStretch", &oItemFD);
                                                if (oItemFD.isName())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"FontStretch", true);
                                                    oXmlWriter.WriteAttribute(L"name", AStringToWString(oItemFD.getName()));
                                                    oXmlWriter.WriteNodeEnd(L"FontStretch", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("FontWeight", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"FontWeight", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"FontWeight", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("Flags", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"Flags", true);
                                                    oXmlWriter.WriteAttribute(L"integer", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"Flags", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("FontBBox", &oItemFD);
                                                if (oItemFD.isArray())
                                                {
                                                    Array *pBBox = oItemFD.getArray();
                                                    if (NULL != pBBox && 4 == pBBox->getLength())
                                                    {
                                                        oXmlWriter.WriteNodeBegin(L"FontBBox", true);
                                                        for (int nIndex = 0; nIndex < 4; nIndex++)
                                                        {
                                                            Object oArrayItem;
                                                            pBBox->get(nIndex, &oArrayItem);
                                                            if (oArrayItem.isInt())
                                                            {
                                                                std::wstring wsValue = L"value" + std::to_wstring(nIndex);
                                                                oXmlWriter.WriteAttribute(wsValue, oArrayItem.getInt());
                                                            }

                                                            oArrayItem.free();
                                                        }
                                                        oXmlWriter.WriteNodeEnd(L"FontBBox", true);
                                                    }
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("ItalicAngle", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"ItalicAngle", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"ItalicAngle", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("Ascent", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"Ascent", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"Ascent", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("Descent", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"Descent", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"Descent", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("Leading", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"Leading", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"Leading", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("CapHeight", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"CapHeight", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"CapHeight", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("XHeight", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"XHeight", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"XHeight", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("StemV", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"StemV", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"StemV", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("StemH", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"StemH", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"StemH", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("AvgWidth", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"AvgWidth", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"AvgWidth", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("MaxWidth", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"MaxWidth", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"MaxWidth", true, true);
                                                }
                                                oItemFD.free();

                                                pFontDescriptor->lookup("MissingWidth", &oItemFD);
                                                if (oItemFD.isInt())
                                                {
                                                    oXmlWriter.WriteNodeBegin(L"MissingWidth", true);
                                                    oXmlWriter.WriteAttribute(L"number", oItemFD.getInt());
                                                    oXmlWriter.WriteNodeEnd(L"MissingWidth", true, true);
                                                }
                                                oItemFD.free();

                                                // TODO: Тут надо реализовать чтени полей /Style, /Lang, /FD, /CIDSet

                                                oXmlWriter.WriteNodeEnd(L"FontDescriptor");
                                            }
                                        }
                                        oFontItem.free();

                                        pDescFont->lookup("DW", &oFontItem);
                                        if (oFontItem.isInt())
                                        {
                                            oXmlWriter.WriteNodeBegin(L"DW", true);
                                            oXmlWriter.WriteAttribute(L"integer", oFontItem.getInt());
                                            oXmlWriter.WriteNodeEnd(L"DW", true, true);
                                        }
                                        oFontItem.free();

                                        pDescFont->lookup("W", &oFontItem);
                                        if (oFontItem.isArray())
                                        {
                                            Array *pArray = oFontItem.getArray();
                                            if (NULL != pArray)
                                            {
                                                oXmlWriter.WriteNodeBegin(L"W");

                                                for (int nIndex = 0; nIndex < pArray->getLength(); nIndex++)
                                                {
                                                    Object oArrayItem;
                                                    pArray->get(nIndex, &oArrayItem);
                                                    if (oArrayItem.isInt())
                                                    {
                                                        oXmlWriter.WriteNodeBegin(L"Int", true);
                                                        oXmlWriter.WriteAttribute(L"value", oArrayItem.getInt());
                                                        oXmlWriter.WriteNodeEnd(L"Int", true, true);
                                                    }
                                                    else if (oArrayItem.isArray())
                                                    {
                                                        Array *pWArray = oArrayItem.getArray();
                                                        if (NULL != pWArray)
                                                        {
                                                            oXmlWriter.WriteNodeBegin(L"Array");
                                                            for (int nWIndex = 0; nWIndex < pWArray->getLength(); nWIndex++)
                                                            {
                                                                Object oWArrayItem;
                                                                pWArray->get(nWIndex, &oWArrayItem);
                                                                if (oWArrayItem.isInt())
                                                                {
                                                                    oXmlWriter.WriteNodeBegin(L"Int", true);
                                                                    oXmlWriter.WriteAttribute(L"value", oWArrayItem.getInt());
                                                                    oXmlWriter.WriteNodeEnd(L"Int", true, true);
                                                                }
                                                                oWArrayItem.free();
                                                            }
                                                            oXmlWriter.WriteNodeEnd(L"Array");
                                                        }
                                                    }
                                                    oArrayItem.free();
                                                }

                                                oXmlWriter.WriteNodeEnd(L"W");
                                            }
                                        }
                                        oFontItem.free();

                                        pDescFont->lookup("DW2", &oFontItem);
                                        if (oFontItem.isArray())
                                        {
                                            Array *pArray = oFontItem.getArray();
                                            if (NULL != pArray && 2 == pArray->getLength())
                                            {
                                                oXmlWriter.WriteNodeBegin(L"DW2", true);

                                                Object oArrayItem;
                                                pArray->get(0, &oArrayItem);
                                                if (oArrayItem.isInt())
                                                {
                                                    oXmlWriter.WriteAttribute(L"value0", oArrayItem.getInt());
                                                }
                                                oArrayItem.free();

                                                pArray->get(1, &oArrayItem);
                                                if (oArrayItem.isInt())
                                                {
                                                    oXmlWriter.WriteAttribute(L"value1", oArrayItem.getInt());
                                                }
                                                oArrayItem.free();

                                                oXmlWriter.WriteNodeEnd(L"DW2", true, true);
                                            }
                                        }
                                        oFontItem.free();

                                        pDescFont->lookup("W2", &oFontItem);
                                        if (oFontItem.isArray())
                                        {
                                            Array *pArray = oFontItem.getArray();
                                            if (NULL != pArray)
                                            {
                                                oXmlWriter.WriteNodeBegin(L"W2");

                                                for (int nIndex = 0; nIndex < pArray->getLength(); nIndex++)
                                                {
                                                    Object oArrayItem;
                                                    pArray->get(nIndex, &oArrayItem);
                                                    if (oArrayItem.isInt())
                                                    {
                                                        oXmlWriter.WriteNodeBegin(L"Int", true);
                                                        oXmlWriter.WriteAttribute(L"value", oArrayItem.getInt());
                                                        oXmlWriter.WriteNodeEnd(L"Int", true, true);
                                                    }
                                                    else if (oArrayItem.isArray())
                                                    {
                                                        Array *pWArray = oArrayItem.getArray();
                                                        if (NULL != pWArray)
                                                        {
                                                            oXmlWriter.WriteNodeBegin(L"Array");
                                                            for (int nWIndex = 0; nWIndex < pWArray->getLength(); nWIndex++)
                                                            {
                                                                Object oWArrayItem;
                                                                pWArray->get(nWIndex, &oWArrayItem);
                                                                if (oWArrayItem.isInt())
                                                                {
                                                                    oXmlWriter.WriteNodeBegin(L"Int", true);
                                                                    oXmlWriter.WriteAttribute(L"value", oWArrayItem.getInt());
                                                                    oXmlWriter.WriteNodeEnd(L"Int", true, true);
                                                                }
                                                                oWArrayItem.free();
                                                            }
                                                            oXmlWriter.WriteNodeEnd(L"Array");
                                                        }
                                                    }
                                                    oArrayItem.free();
                                                }

                                                oXmlWriter.WriteNodeEnd(L"W2");
                                            }
                                        }
                                        oFontItem.free();

                                        pDescFont->lookup("CIDToGIDMap", &oFontItem);
                                        if (oFontItem.isName())
                                        {
                                            oXmlWriter.WriteNodeBegin(L"CIDToGIDMap", true);
                                            oXmlWriter.WriteAttribute(L"name", AStringToWString(oFontItem.getName()));
                                            oXmlWriter.WriteNodeEnd(L"CIDToGIDMap", true, true);
                                        }
                                        else if (oFontItem.isStream())
                                        {
                                            oXmlWriter.WriteNodeBegin(L"CIDToGIDMap");

                                            std::string sBuffer;
                                            oFontItem.streamReset();
                                            int nChar = 0;
                                            while ((nChar = oFontItem.streamGetChar()) != EOF)
                                            {
                                                sBuffer+=(char)nChar;
                                            }
                                            oFontItem.streamClose();

                                            CBase64 oBase64;
                                            oBase64.Encode((unsigned char*)sBuffer.c_str(), sBuffer.length());

                                            oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));

                                            oXmlWriter.WriteNodeEnd(L"CIDToGIDMap");
                                        }
                                        oFontItem.free();


                                        oXmlWriter.WriteNodeEnd(L"DescendantFonts");
                                    }
                                }
                                oDescFont.free();
                            }
                        }
                        oDictItem.free();

                        oXmlWriter.WriteNodeEnd(L"Type0");
                        oXmlWriter.WriteNodeEnd(L"PDF-resources");
                    }
                    oFontObject.free();

                    oXmlWriter.SaveToFile(wsEncodingPath);
                }
            }

            // Обрежем индекс у FontName, если он есть
            if (wsFontName.empty())
                wsFontName = wsFontBaseName;
            if (wsFontName.length() > 7)
            {
                bool bIsIndex = true;
                if ('+' != wsFontName.at(6))
                    bIsIndex = false;

                if (bIsIndex)
                {
                    for (int nIndex = 0; nIndex < 6; nIndex++)
                    {
                        int nChar = wsFontName.at(nIndex);
                        if (nChar < 'A' || nChar > 'Z')
                        {
                            bIsIndex = false;
                            break;
                        }
                    }
                }

                if (bIsIndex)
                {
                    wsFontName.erase(0, 7);
                }
            }

            pEntry->wsFilePath     = wsFileName;
            pEntry->wsFontName     = wsFontName;
            pEntry->pCodeToGID     = pCodeToGID;
            pEntry->pCodeToUnicode = pCodeToUnicode;
            pEntry->unLenGID       = (unsigned int)nLen;
            pEntry->unLenUnicode   = (unsigned int)nToUnicodeLen;
            pEntry->bAvailable     = true;
        }
        else if (NULL != pEntry)
        {
            wsFileName = pEntry->wsFilePath;
            wsFontName = pEntry->wsFontName;
        }

        if (L"" != wsFileName)
        {
            m_pRenderer->put_FontPath(wsFileName);
            m_pRenderer->put_FontName(wsFontName);
        }
    }
    void RendererOutputDev::stroke(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_bTransparentGroupSoftMask)
            return;

        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
        m_pRenderer->DrawPath(c_nStroke);

        m_pRenderer->EndCommand(c_nPathType);
    }
    void RendererOutputDev::fill(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_bTransparentGroupSoftMask || (!m_arrTransparentGroupSoftMask.empty() && m_bTransparentGroupSoftMaskEnd))
            return;

        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
        m_pRenderer->DrawPath(c_nWindingFillMode);
        m_pRenderer->EndCommand(c_nPathType);
    }
    void RendererOutputDev::eoFill(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_bTransparentGroupSoftMask || (!m_arrTransparentGroupSoftMask.empty() && m_bTransparentGroupSoftMaskEnd))
            return;

        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
        m_pRenderer->DrawPath(c_nEvenOddFillMode);

        m_pRenderer->EndCommand(c_nPathType);
    }
    void RendererOutputDev::FillStroke(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_bTransparentGroupSoftMask || (!m_arrTransparentGroupSoftMask.empty() && m_bTransparentGroupSoftMaskEnd))
            return;

        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
        m_pRenderer->DrawPath(c_nStroke | c_nWindingFillMode);

        m_pRenderer->EndCommand(c_nPathType);
    }
    void RendererOutputDev::EoFillStroke(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_bTransparentGroupSoftMask || (!m_arrTransparentGroupSoftMask.empty() && m_bTransparentGroupSoftMaskEnd))
            return;

        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
        m_pRenderer->DrawPath(c_nStroke | c_nEvenOddFillMode);

        m_pRenderer->EndCommand(c_nPathType);
    }
    void RendererOutputDev::tilingPatternFill(GfxState *pGState, Object *pStream, int nPaintType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep)
    {

    }
    void RendererOutputDev::StartTilingFill(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        m_pRenderer->BeginCommand(c_nComplexFigureType);
        m_bTiling = true;
    }
    void RendererOutputDev::EndTilingFill()
    {
        if (m_bDrawOnlyText)
            return;

        m_pRenderer->EndCommand(c_nComplexFigureType);
        m_bTiling = false;
    }

    GBool RendererOutputDev::shadedFill(GfxState *pGState, GfxShading *pShading)
    {
        double x0, y0, x1, x2, y1, y2, r1, r2;
        double xmin, xmax, ymin, ymax, r;
        double *matrix;

        int nTriangles = 0, nPatches = 0;
        switch (pShading->getType())
        {
            case 1:
                ((GfxFunctionShading *)pShading)->getDomain(&x0, &y0, &x1, &y1);
                matrix = ((GfxFunctionShading *)pShading)->getMatrix();
                pGState->moveTo(x0 * matrix[0] + y0 * matrix[2] + matrix[4],
                                x0 * matrix[1] + y0 * matrix[3] + matrix[5]);
                pGState->lineTo(x1 * matrix[0] + y0 * matrix[2] + matrix[4],
                                x1 * matrix[1] + y0 * matrix[3] + matrix[5]);
                pGState->lineTo(x1 * matrix[0] + y1 * matrix[2] + matrix[4],
                                x1 * matrix[1] + y1 * matrix[3] + matrix[5]);
                pGState->lineTo(x0 * matrix[0] + y1 * matrix[2] + matrix[4],
                                x0 * matrix[1] + y1 * matrix[3] + matrix[5]);
                pGState->closePath();
                FunctionShadedFill(pGState, (GfxFunctionShading *) pShading);
                return true;
            case 2:
                pGState->getUserClipBBox(&xmin, &ymin, &xmax, &ymax);
                //pGState->clearPath();


                pGState->moveTo(xmin, ymin);
                pGState->lineTo(xmin, ymax);
                pGState->lineTo(xmax, ymax);
                pGState->lineTo(xmax, ymin);
                pGState->closePath();
                AxialShadedFill(pGState, (GfxAxialShading* )pShading);
                return true;
            case 3:
                ((GfxRadialShading *)pShading)->getCoords(&x1, &y1, &r1, &x2, &y2, &r2);

                r = std::max(r1,r2);
                xmin = std::min(x1, x2) - 2 * r;
                ymin = std::min(y1, y2) - 2 * r;
                xmax = std::max(x1, x2) + 2 * r;
                ymax = std::max(y1, y2) + 2 * r;

                pGState->moveTo(xmin, ymin);
                pGState->lineTo(xmin, ymax);
                pGState->lineTo(xmax, ymax);
                pGState->lineTo(xmax, ymin);
                pGState->closePath();
                RadialShadedFill(pGState, (GfxRadialShading*) pShading);
                return true;
            case 4:
            case 5:
                nTriangles = ((GfxGouraudTriangleShading *) pShading)->getNTriangles();

                for (int i = 0; i < nTriangles; i++) {
                    int nComps = ((GfxGouraudTriangleShading *) pShading)->getNComps();
                    double x1,x2,x3,y1,y2,y3;
                    double *c1 =  new double[nComps];
                    double *c2 =  new double[nComps];
                    double *c3 =  new double[nComps];

                    GfxColor col1, col2, col3;

                    ((GfxGouraudTriangleShading *) pShading)->getTriangle(i, &x1, &y1, c1, &x2, &y2, c2, &x3, &y3, c3);
                    ((GfxGouraudTriangleShading *) pShading)->getColor(c1, &col1);
                    ((GfxGouraudTriangleShading *) pShading)->getColor(c2, &col2);
                    ((GfxGouraudTriangleShading *) pShading)->getColor(c3, &col3);

                    pGState->clearPath();

                    pGState->moveTo(x1, y1);
                    pGState->lineTo(x2, y2);
                    pGState->lineTo(x3, y3);

                    pGState->closePath();

                    GouraundTriangleFill(pGState, {&col1, &col2, &col3}, {{x1,y1},{x2,y2},{x3,y3}});
                    delete[] c1;
                    delete[] c2;
                    delete[] c3;
                }
                return true;
            case 6:
            case 7:
                int nComps = ((GfxPatchMeshShading *) pShading)->getNComps();
                int nPatches = ((GfxPatchMeshShading *) pShading)->getNPatches();
                for (int i = 0; i < nPatches; i++) {
                    GfxPatch *patch = ((GfxPatchMeshShading *) pShading)->getPatch(i);
                    pGState->clearPath();
                    pGState->moveTo(patch->x[0][0], patch->y[0][0]);
                    pGState->curveTo(patch->x[0][1], patch->y[0][1],
                                   patch->x[0][2], patch->y[0][2],
                                   patch->x[0][3], patch->y[0][3]);
                    pGState->curveTo(patch->x[1][3], patch->y[1][3],
                                   patch->x[2][3], patch->y[2][3],
                                   patch->x[3][3], patch->y[3][3]);
                    pGState->curveTo(patch->x[3][2], patch->y[3][2],
                                   patch->x[3][1], patch->y[3][1],
                                   patch->x[3][0], patch->y[3][0]);
                    pGState->curveTo(patch->x[2][0], patch->y[2][0],
                                   patch->x[1][0], patch->y[1][0],
                                   patch->x[0][0], patch->y[0][0]);
                    pGState->closePath();
                    PatchMeshFill(pGState, patch, (GfxPatchMeshShading *) pShading);
                }
                return true;

        }
        return false;
    }

	bool RendererOutputDev::FunctionShadedFill(GfxState *pGState, GfxFunctionShading *pShading)
	{
		if (m_bDrawOnlyText)
			return true;

		if (m_bTransparentGroupSoftMask || (!m_arrTransparentGroupSoftMask.empty() && m_bTransparentGroupSoftMaskEnd))
			return true;

        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
		long brush;
		int alpha = pGState->getFillOpacity() * 255;
		m_pRenderer->get_BrushType(&brush);
		m_pRenderer->put_BrushType(c_BrushTypeMyTestGradient);
		double x1,x2,y1,y2;
		pShading->getDomain(&x1, &y1, &x2, &y2);
		std::vector<float> mapping(6);
		for (int i = 0; i < 6; i++)
		{
			mapping[i] = PDFCoordsToMM(pShading->getMatrix()[i]);
		}
		NSStructures::GradientInfo info = NSStructures::GInfoConstructor::get_functional(x1, x2, y1, y2, mapping);

		float cur_x = 0, cur_y = 0;
		float delta_x = (x2 - x1) / info.shading.function.get_resolution();
		float delta_y = (y2 - y1) / info.shading.function.get_resolution();

        GfxColorSpace *ColorSpace = pShading->getColorSpace();

		for (size_t i = 0; i < info.shading.function.get_resolution(); i++)
        {
		    cur_x = 0;
            for (size_t j = 0; j < info.shading.function.get_resolution(); j++)
            {
                GfxColor c;
                pShading->getColor(cur_x, cur_y, &c);

                GfxRGB draw_color;
                // RenderingIntent in this case does nothing but it's an obligatory arguments
                ColorSpace->getRGB(&c, &draw_color, gfxRenderingIntentAbsoluteColorimetric);
                info.shading.function.set_color(j, i, colToByte(draw_color.b),
                    colToByte(draw_color.g), colToByte(draw_color.r), alpha);
                cur_x += delta_x;
            }
            cur_y += delta_y;
        }

		if (NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer))
		{
			GRenderer->put_BrushGradInfo(info);
			m_pRenderer->DrawPath(c_nWindingFillMode);
		}

		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->put_BrushType(brush);
		pGState->clearPath();
		return true;
	}
	bool RendererOutputDev::AxialShadedFill(GfxState *pGState, GfxAxialShading *pShading)
	{
		if (m_bDrawOnlyText)
			return true;

		if (m_bTransparentGroupSoftMask || (!m_arrTransparentGroupSoftMask.empty() && m_bTransparentGroupSoftMaskEnd))
			return true;
		double x1, x2, y1, y2;
		double t0, t1;

        long brush;
        m_pRenderer->get_BrushType(&brush);

        /*
        IRenderer* pLastRenderer = m_pRenderer;
        BYTE* pBgraData = NULL;
        CBgraFrame* pFrame = NULL;
        double dpi, dWidth, dHeight;
        m_pRenderer->get_DpiX(&dpi);
        // TODO что если размеры не равны размерам m_pSoftMask
        pLastRenderer->get_Width(&dWidth);
        pLastRenderer->get_Height(&dHeight);
        int nWidth  = dWidth  * 96 / dpi;
        int nHeight = dHeight * 96 / dpi;
        if (m_pSoftMask)
        {
            NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
            m_pRenderer = pRenderer;

            pBgraData = new BYTE[nWidth * nHeight * 4];
            memset(pBgraData, 0xff, nWidth * nHeight * 4);

            pFrame = new CBgraFrame();
            pFrame->put_Data(pBgraData);
            pFrame->put_Width(nWidth);
            pFrame->put_Height(nHeight);
            // TODO bIsFlip = true ~ 4
            pFrame->put_Stride(-4 * nWidth);

            pRenderer->CreateFromBgraFrame(pFrame);
            // TODO проверить
            pRenderer->SetSwapRGB(true);

            pRenderer->put_Width(dWidth);
            pRenderer->put_Height(dHeight);
        }
        */

        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());

		double dAlphaKoef = pGState->getFillOpacity();
		m_pRenderer->put_BrushType(c_BrushTypePathNewLinearGradient);

        pShading->getCoords(&x1, &y1, &x2, &y2);
		t0 = pShading->getDomain0();
		t1 = pShading->getDomain1();

        x1 = PDFCoordsToMM(x1);
        x2 = PDFCoordsToMM(x2);
        y1 = PDFCoordsToMM(y1);
        y2 = PDFCoordsToMM(y2);

		NSStructures::GradientInfo info = NSStructures::GInfoConstructor::get_linear({x1, y1}, {x2, y2}, t0, t1,
															   pShading->getExtend0(), pShading->getExtend1());

		GfxColorSpace *ColorSpace = pShading->getColorSpace();
		float delta = (t1 - t0) / info.shading.function.get_resolution();
		float t = t0;
		for (size_t i = 0; i < info.shading.function.get_resolution(); i++)
		{
			GfxColor c;
			pShading->getColor(t, &c);

            GfxRGB  draw_color;
            GfxGray draw_alpha;

            // RenderingIntent in this case does nothing but it's an obligatory arguments
            ColorSpace->getRGB(&c, &draw_color, gfxRenderingIntentAbsoluteColorimetric);
            ColorSpace->getGray(&c, &draw_alpha, gfxRenderingIntentAbsoluteColorimetric);
            info.shading.function.set_color(i, colToByte(draw_color.b),
                                            colToByte(draw_color.g), colToByte(draw_color.r), dAlphaKoef * 255.0);
            t+=delta;
		}

		if (NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer))
		{
		    GRenderer->put_BrushGradInfo(info);
			m_pRenderer->DrawPath(c_nWindingFillMode);
		}

		m_pRenderer->EndCommand(c_nPathType);

        /*
        if (m_pSoftMask)
        {
            for (int nY = m_nSoftMaskHeight - 1; nY >= 0; nY--)
            {
                int nIndex = 4 * nY * m_nSoftMaskWidth;
                for (int nX = 0; nX < m_nSoftMaskWidth; nX++)
                {
                    if (m_pSoftMask[nIndex + 3])
                    {
                        m_pSoftMask[nIndex + 0] = pBgraData[nIndex + 0];
                        m_pSoftMask[nIndex + 1] = pBgraData[nIndex + 1];
                        m_pSoftMask[nIndex + 2] = pBgraData[nIndex + 2];
                    }
                    nIndex += 4;
                }
            }

            pFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res3.png", _CXIMAGE_FORMAT_PNG);
            RELEASEOBJECT(m_pRenderer);
            m_pRenderer = pLastRenderer;

            Aggplus::CImage oImage;
            oImage.Create(m_pSoftMask, m_nSoftMaskWidth, m_nSoftMaskHeight, -4 * m_nSoftMaskWidth, true);

            double arrMatrix[6];
            double* pCTM = m_pCTMSoftMask;

            //  Исходное предобразование
            //              |1  0  0|   |pCTM[0] pCTM[1] 0|
            // arrMattrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
            //              |0  1  1|   |pCTM[4] pCTM[5] 1|

            arrMatrix[0] =     pCTM[0];
            arrMatrix[1] =  -pCTM[1];
            arrMatrix[2] =    -pCTM[2];
            arrMatrix[3] =  -(-pCTM[3]);
            arrMatrix[4] =     pCTM[2] + pCTM[4];
            arrMatrix[5] =  -(pCTM[3] + pCTM[5]) + pGState->getPageHeight();

            double dShiftX = 0, dShiftY = 0;
            DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
            m_pRenderer->DrawImage(&oImage, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
            oImage.SaveFile(NSFile::GetProcessDirectory() + L"/res2.png", _CXIMAGE_FORMAT_PNG);
        }
        */

		m_pRenderer->put_BrushType(brush);

		pGState->clearPath();
		return true;
	}
	bool RendererOutputDev::RadialShadedFill(GfxState *pGState, GfxRadialShading *pShading)
	{
		if (m_bDrawOnlyText)
			return true;

		if (m_bTransparentGroupSoftMask || (!m_arrTransparentGroupSoftMask.empty() && m_bTransparentGroupSoftMaskEnd))
			return true;



        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());

		long brush;
		int alpha = pGState->getFillOpacity() * 255;
		m_pRenderer->get_BrushType(&brush);
		m_pRenderer->put_BrushType(c_BrushTypePathRadialGradient);

		double x1, x2, y1, y2, r1, r2;
		double t0, t1;
		pShading->getCoords(&x1, &y1, &r1, &x2, &y2, &r2);
		t0 = pShading->getDomain0();
		t1 = pShading->getDomain1();

		x1 = PDFCoordsToMM(x1);
        x2 = PDFCoordsToMM(x2);
        y1 = PDFCoordsToMM(y1);
        y2 = PDFCoordsToMM(y2);
        r1 = PDFCoordsToMM(r1);
        r2 = PDFCoordsToMM(r2);

		NSStructures::GradientInfo info = NSStructures::GInfoConstructor::get_radial({x1, y1}, {x2, y2}, r1, r2,
															   t0, t1, pShading->getExtend0(), pShading->getExtend1());

		GfxColorSpace *ColorSpace = pShading->getColorSpace();;
		float delta = (t1 - t0) / info.shading.function.get_resolution();
		float t = t0;
		for (size_t i = 0; i < info.shading.function.get_resolution(); i++)
		{
            GfxColor c;
            pShading->getColor(t, &c);
            GfxRGB draw_color;
            // RenderingIntent in this case does nothing but it's an obligatory arguments
            ColorSpace->getRGB(&c, &draw_color, gfxRenderingIntentAbsoluteColorimetric);
			info.shading.function.set_color(i, colToByte(draw_color.b),
                                            colToByte(draw_color.g), colToByte(draw_color.r), alpha);
            t+=delta;
		}

		if (NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer))
		{
			GRenderer->put_BrushGradInfo(info);
			m_pRenderer->DrawPath(c_nWindingFillMode);
		}
		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->put_BrushType(brush);
		pGState->clearPath();
		return true;
	}
	bool RendererOutputDev::GouraundTriangleFill(GfxState *pGState, const std::vector<GfxColor*> &colors, const std::vector<NSStructures::Point> &points)
	{
		if (m_bDrawOnlyText)
			return true;

		if (m_bTransparentGroupSoftMask || (!m_arrTransparentGroupSoftMask.empty() && m_bTransparentGroupSoftMaskEnd))
			return true;

        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());

		long brush;
		int alpha = pGState->getFillOpacity() * 255;
		m_pRenderer->get_BrushType(&brush);
		m_pRenderer->put_BrushType(c_BrushTypeTriagnleMeshGradient);

		std::vector<NSStructures::Point> pixel_points;
		std::vector<agg::rgba8> rgba8_colors;
		GfxCalRGBColorSpace ColorSpace;

		for (int i = 0; i < 3; i++)
		{
			GfxColor c = *colors[i];
            GfxRGB draw_color;
            // RenderingIntent in this case does nothing but it's an obligatory arguments
            ColorSpace.getRGB(&c, &draw_color, gfxRenderingIntentAbsoluteColorimetric);
			rgba8_colors.push_back({colToByte(draw_color.b), colToByte(draw_color.g), colToByte(draw_color.r), (unsigned)alpha});
			double x = points[i].x;
			double y = points[i].y;
			x = PDFCoordsToMM(x);
            y = PDFCoordsToMM(y);
			pixel_points.push_back({x, y});
		}

		NSStructures::GradientInfo info = NSStructures::GInfoConstructor::get_triangle(pixel_points, rgba8_colors, {}, false);

		if (NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer))
		{
			GRenderer->put_BrushGradInfo(info);
			m_pRenderer->DrawPath(c_nWindingFillMode);
		}
		pGState->clearPath();
		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->put_BrushType(brush);
		return true;
	}
	bool RendererOutputDev::PatchMeshFill(GfxState *pGState, GfxPatch *patch, GfxPatchMeshShading *pShading)
	{
		if (m_bDrawOnlyText)
			return true;

		if (m_bTransparentGroupSoftMask || (!m_arrTransparentGroupSoftMask.empty() && m_bTransparentGroupSoftMaskEnd))
			return true;

        DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());

		long brush;
		int alpha = pGState->getFillOpacity() * 255;
		m_pRenderer->get_BrushType(&brush);
		m_pRenderer->put_BrushType(c_BrushTypeTensorCurveGradient);

		std::vector<std::vector<NSStructures::Point>> points(4, std::vector<NSStructures::Point>(4));
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				double x = patch->x[i][j];
				double y = patch->y[i][j];

				x = PDFCoordsToMM(x);
                y = PDFCoordsToMM(y);
				points[i][j].x = x;
				points[i][j].y = y;
			}
		}
		std::vector<std::vector<agg::rgba8>> colors(2, std::vector<agg::rgba8>(2));
		GfxDeviceRGBColorSpace ColorSpace;
		for (int i = 0; i < 2; i ++)
		{
			for (int j = 0; j < 2; j++)
			{
			    GfxColor c;
			    pShading->getColor(patch->color[i][j], &c);
                GfxRGB draw_color;
                // RenderingIntent in this case does nothing but it's an obligatory arguments
                ColorSpace.getRGB(&c, &draw_color, gfxRenderingIntentAbsoluteColorimetric);
				colors[j][i] = {colToByte(draw_color.b), colToByte(draw_color.g), colToByte(draw_color.r), (unsigned)alpha};
			}
		}
		NSStructures::GradientInfo info = NSStructures::GInfoConstructor::get_tensor_curve(points,
		{},
																	 colors,
																	 false
																	 );

		if (NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer))
		{
			GRenderer->put_BrushGradInfo(info);
			m_pRenderer->DrawPath(c_nWindingFillMode);
		}

		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->put_BrushType(brush);
		pGState->clearPath();
		return true;
	}
	void RendererOutputDev::StartShadedFill(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        m_pRenderer->BeginCommand(c_nComplexFigureType);
    }
    void RendererOutputDev::EndShadedFill()
    {
        if (m_bDrawOnlyText)
            return;

        m_pRenderer->EndCommand(c_nComplexFigureType);
    }
    void RendererOutputDev::StartTilingFillIteration()
    {
        if (m_bDrawOnlyText)
            return;

        m_pRenderer->BeginCommand(c_nPDFTilingFillIteration);
    }
    void RendererOutputDev::EndTilingFillIteration()
    {
        if (m_bDrawOnlyText)
            return;

        m_pRenderer->EndCommand(c_nPDFTilingFillIteration);
    }
    void RendererOutputDev::StartSimpleTilingFill(GfxState *pGState, int  nX0, int nY0, int nX1, int nY1, double dStepX, double dStepY, double dXMin, double dYMin, double dXMax, double dYMax, double* pMatrix)
    {
        if (m_bDrawOnlyText)
            return;

        this->clipAttack(pGState);

        m_pRenderer->BeginCommand(c_nPDFTilingFill);

        CXmlWriter oWriter;
        oWriter.WriteNodeBegin(L"htmltiling", true);
        oWriter.WriteAttribute(L"x", nX0);
        oWriter.WriteAttribute(L"y", nY0);
        oWriter.WriteAttribute(L"countx", nX1 - nX0);
        oWriter.WriteAttribute(L"county", nY1 - nY0);
        oWriter.WriteAttribute(L"stepx", dStepX);
        oWriter.WriteAttribute(L"stepy", dStepY);
        oWriter.WriteNodeEnd(L"htmltiling", true, false);

        oWriter.WriteNodeBegin(L"bbox", true);
        oWriter.WriteAttribute(L"x", dXMin);
        oWriter.WriteAttribute(L"y", dYMin);
        oWriter.WriteAttribute(L"r", dXMax);
        oWriter.WriteAttribute(L"b", dYMax);
        oWriter.WriteNodeEnd(L"bbox", true, true);

        oWriter.WriteNodeBegin(L"transform", true);
        oWriter.WriteAttribute(L"m1", pMatrix[0]);
        oWriter.WriteAttribute(L"m2", pMatrix[1]);
        oWriter.WriteAttribute(L"m3", pMatrix[2]);
        oWriter.WriteAttribute(L"m4", pMatrix[3]);
        oWriter.WriteAttribute(L"m5", pMatrix[4]);
        oWriter.WriteAttribute(L"m6", pMatrix[5]);
        oWriter.WriteNodeEnd(L"transform", true, true);

        oWriter.WriteNodeEnd(L"htmltiling", false);
        // TODO: m_pRenderer->SetAdditionalParam(L"TilingHtmlPattern", oWriter.GetXmlString());
    }
    void RendererOutputDev::EndSimpleTilingFill()
    {
        if (m_bDrawOnlyText)
            return;

        m_pRenderer->EndCommand(c_nPDFTilingFill);
    }
    void RendererOutputDev::clip(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_sClip.empty())
        {
            m_sClip.push_back(GfxClip());
        }
        m_sClip.back().AddPath(pGState->getPath(), pGState->getCTM(), false);
        m_bClipChanged = true;
        updateClip(pGState);
    }
    void RendererOutputDev::eoClip(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_sClip.empty())
        {
            m_sClip.push_back(GfxClip());
        }
        m_sClip.back().AddPath(pGState->getPath(), pGState->getCTM(), true);
        m_bClipChanged = true;
        updateClip(pGState);
    }
    void RendererOutputDev::clipToStrokePath(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_sClip.empty())
        {
            m_sClip.push_back(GfxClip());
        }
        m_sClip.back().AddPath(pGState->getPath(), pGState->getCTM(), false);
        m_bClipChanged = true;
        updateClip(pGState);
    }
    void RendererOutputDev::clipToPath(GfxState *pGState, GfxPath *pPath, double *pMatrix, bool bEO)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_bTransparentGroupSoftMask)
            return;

        int nClipFlag = bEO ? c_nClipRegionTypeEvenOdd : c_nClipRegionTypeWinding;
        nClipFlag |= c_nClipRegionIntersect;

        m_pRenderer->BeginCommand(c_nClipType);
        m_pRenderer->put_ClipMode(nClipFlag);
        DoPath(pGState, pPath, pGState->getPageHeight(), pMatrix);
        m_pRenderer->EndCommand(c_nPathType);
        m_pRenderer->EndCommand(c_nClipType);
    }
    void RendererOutputDev::ClipToText(const std::wstring& wsFontName, const std::wstring& wsFontPath, double dFontSize, int nFontStyle, double *pMatrix, const std::wstring& wsText, double dX, double dY, double dWidth, double dHeight, double dBaseLineOffset)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_bTransparentGroupSoftMask)
            return;

        m_pRenderer->put_FontName(wsFontName);
        m_pRenderer->put_FontPath(wsFontPath);
        m_pRenderer->put_FontSize(dFontSize);
        m_pRenderer->put_FontStyle((long)nFontStyle);

        double dShiftX = 0, dShiftY = 0;
        DoTransform(pMatrix, &dShiftX, &dShiftY, true);

        // TODO: нужна нормальная конвертация
        int nLen = (int)wsText.length();
        const wchar_t* pDataSrc = wsText.c_str();
        if (1 == wsText.length())
            m_pRenderer->PathCommandTextExCHAR(0, (LONG)pDataSrc[0], PDFCoordsToMM(dX), PDFCoordsToMM(dY), PDFCoordsToMM(dWidth), PDFCoordsToMM(dHeight));
        else if (0 != nLen)
        {
            unsigned int* pGids = new unsigned int[nLen];
            for (int nIndex = 0; nIndex < nLen; ++nIndex)
                pGids[nIndex] = (unsigned int)pDataSrc[nIndex];

            m_pRenderer->PathCommandTextEx(L"", pGids, nLen, PDFCoordsToMM(dX), PDFCoordsToMM(dY), PDFCoordsToMM(dWidth), PDFCoordsToMM(dHeight));

            RELEASEARRAYOBJECTS(pGids);
        }
    }
    void RendererOutputDev::endTextObject(GfxState *pGState)
    {
//        if (NULL != m_pBufferTextClip) tmpchange
//        {
//            updateClip(pGState);
//
//            RELEASEOBJECT(m_pBufferTextClip);
//        }
    }
    void RendererOutputDev::beginStringOp(GfxState *pGState)
    {
        if (m_bTransparentGroupSoftMask)
            return;

        m_pRenderer->BeginCommand(c_nTextType);

        int nRenderMode = pGState->getRender();

        // Обработка Clip
//        if (nRenderMode >= 4) tmpchange
//        {
//            RELEASEOBJECT(m_pBufferTextClip);
//            m_pBufferTextClip = new GfxTextClip();
//        }

        // Обработка Stroke
        if (1 == nRenderMode || 2 == nRenderMode || 5 == nRenderMode || 6 == nRenderMode)
        {
//            Painter::CPen oPen;
//
//            m_pRenderer->put_PenSize( PDFCoordsToMM( pGState->getFontSize() ) * 0.05 );
//            m_pRenderer->put_PenAlpha(  255 );
//            oPen.SetColor( m_oPen.GetColor() );
//
//            BSTR bsPen = oPen.ToXmlString().AllocSysString();
//            m_pRenderer->SetPen( bsPen );
//            ::SysFreeString( bsPen );
        }
    }
    void RendererOutputDev::endStringOp(GfxState *pGState)
    {
        if (m_bTransparentGroupSoftMask)
            return;

        int nRenderMode = pGState->getRender();

        // Добавляем в Clipping Path текст
        if (nRenderMode >= 4)
        {
//            if (m_pBufferTextClip) tmpchange
//                pGState->GetClip()->AppendTextClip(m_pBufferTextClip);

            updateFont(pGState);
        }

        // Возвращаем параметры для Stroke
        if (1 == nRenderMode || 2 == nRenderMode || 5 == nRenderMode || 6 == nRenderMode)
        {
            //BSTR bsPen = m_oPen.ToXmlString().AllocSysString();
            //m_pRenderer->SetPen( bsPen );
            //::SysFreeString( bsPen );
        }

        m_pRenderer->EndCommand(c_nTextType);
    }
    void RendererOutputDev::drawString(GfxState *pGState, GString *seString)
    {
        if (m_bTransparentGroupSoftMask)
            return;

        // Проверяем наличие списка со шрифтами
        if (NULL == m_pFontList)
            return;

        // Проверяем наличие текущего шрифта
        TFontEntry oEntry;
        if (!m_pFontList->GetFont(pGState->getFont()->getID(), &oEntry))
            return;

        int nRendererMode = pGState->getRender();

        if (3 == nRendererMode) // Невидимый текст
            return;

        double *pCTM  = pGState->getCTM();
        double *pTm   = pGState->getTextMat();
        GfxFont *pFont = pGState->getFont();

        unsigned int unGidsCount = seString->getLength();
        unsigned int* pGids = new unsigned int[unGidsCount];
        if (!pGids)
            return;

        std::wstring  wsUnicodeText;
        for (int nIndex = 0; nIndex < seString->getLength(); nIndex++)
        {
            char nChar = seString->getChar(nIndex);

            if (NULL != oEntry.pCodeToUnicode)
            {
                unsigned short unUnicode = oEntry.pCodeToUnicode[nChar];
                wsUnicodeText += (wchar_t(unUnicode));
            }

            if (NULL != oEntry.pCodeToGID)
                pGids[nIndex] = oEntry.pCodeToGID[nChar];
            else
                pGids[nIndex] = (0 == nChar ? 65534 : nChar);

        }

        m_pRenderer->CommandDrawTextEx(wsUnicodeText, pGids, unGidsCount, PDFCoordsToMM(100), PDFCoordsToMM(100), 0, PDFCoordsToMM(0));
        RELEASEARRAYOBJECTS(pGids);
    }
    void RendererOutputDev::drawChar(GfxState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen)
    {
        if (m_bTransparentGroupSoftMask)
            return;

        // Проверяем наличие списка со шрифтами
        if (NULL == m_pFontList)
            return;

        // Проверяем наличие текущего шрифта
        TFontEntry oEntry;
        if (!m_pFontList->GetFont(pGState->getFont()->getID(), &oEntry))
            return;

        int   nRenderMode = pGState->getRender();

        if (3 == nRenderMode) // Невидимый текст
        {
            return;
        }

        double *pCTM      = pGState->getCTM();
        double *pTm       = pGState->getTextMat();
        GfxFont *pFont     = pGState->getFont();

        double pNewTm[6], arrMatrix[6];

        double dTextScale = std::min(sqrt(pTm[2] * pTm[2] + pTm[3] * pTm[3]), sqrt(pTm[0] * pTm[0] + pTm[1] * pTm[1]));
        double dITextScale = 1 / dTextScale;
        double dOldSize = 10.0;
        m_pRenderer->get_FontSize(&dOldSize);
        if (dOldSize * dTextScale > 0)
        {
            m_pRenderer->put_FontSize(dOldSize * dTextScale);

            pNewTm[0] =  pTm[0] * dITextScale;
            pNewTm[1] =  pTm[1] * dITextScale;
            pNewTm[2] = -pTm[2] * dITextScale;
            pNewTm[3] = -pTm[3] * dITextScale;
            pNewTm[4] =  dX;
            pNewTm[5] =  dY;
        }
        else
        {
            m_pRenderer->put_FontSize(-dOldSize * dTextScale);

            pNewTm[0] = pTm[0] * dITextScale;
            pNewTm[1] = pTm[1] * dITextScale;
            pNewTm[2] = pTm[2] * dITextScale;
            pNewTm[3] = pTm[3] * dITextScale;
            pNewTm[4] = dX;
            pNewTm[5] = dY;
        }


        arrMatrix[0] =   pNewTm[0] * pCTM[0] + pNewTm[1] * pCTM[2];
        arrMatrix[1] = -(pNewTm[0] * pCTM[1] + pNewTm[1] * pCTM[3]);
        arrMatrix[2] =   pNewTm[2] * pCTM[0] + pNewTm[3] * pCTM[2];
        arrMatrix[3] = -(pNewTm[2] * pCTM[1] + pNewTm[3] * pCTM[3]);
        arrMatrix[4] =   pNewTm[4] * pCTM[0] + pNewTm[5] * pCTM[2] + pCTM[4];
        arrMatrix[5] = -(pNewTm[4] * pCTM[1] + pNewTm[5] * pCTM[3] + pCTM[5]) + pGState->getPageHeight();

        if (true)
        {
            double dNorma = std::min(sqrt(arrMatrix[0] * arrMatrix[0] + arrMatrix[1] * arrMatrix[1]), sqrt(arrMatrix[2] * arrMatrix[2] + arrMatrix[3] * arrMatrix[3]));
            if (dNorma > 0.001)
            {
                arrMatrix[0] /= dNorma;
                arrMatrix[1] /= dNorma;
                arrMatrix[2] /= dNorma;
                arrMatrix[3] /= dNorma;

                double dSize = 1;
                m_pRenderer->get_FontSize(&dSize);
                m_pRenderer->put_FontSize(dSize * dNorma);
            }
        }

        double dShiftX = 0, dShiftY = 0;
        DoTransform(arrMatrix, &dShiftX, &dShiftY, true);

        // Здесь мы посылаем координаты текста в пунктах
        double dPageHeight = pGState->getPageHeight();

        std::wstring wsUnicodeText;

        bool isCIDFont = pFont->isCIDFont();

        if (NULL != oEntry.pCodeToUnicode && nCode < oEntry.unLenUnicode)
        {
            unsigned short unUnicode = oEntry.pCodeToUnicode[nCode];
            wsUnicodeText = (wchar_t(unUnicode));
        }
        else
        {
            if (isCIDFont)
            {
                // Значит кодировка была Identity-H или Identity-V, что означает, что иходные коды и есть юникодные значения
                wsUnicodeText = (wchar_t(nCode));
            }
            else
            {
                // Договорились, что если нельзя точно составить юникодные значения, тогда отдаем NULL
                wsUnicodeText = L"";
            }
        }

        unsigned int unGidsCount = 0;
        unsigned int unGid       = 0;
        if (NULL != oEntry.pCodeToGID && nCode < oEntry.unLenGID)
        {
            if (0 == (unGid = oEntry.pCodeToGID[nCode]))
                unGidsCount = 0;
            else
                unGidsCount = 1;
        }
        else
        {
            if ((isCIDFont && (((GfxCIDFont*)pFont)->usesIdentityEncoding() || ((GfxCIDFont*)pFont)->usesIdentityCIDToGID() || ((GfxCIDFont*)pFont)->ctuUsesCharCodeToUnicode()))
                || (!isCIDFont && wsUnicodeText.empty()))
            {
                int nCurCode = (0 == nCode ? 65534 : nCode);
                unGid       = (unsigned int)nCurCode;
                unGidsCount = 1;
            }
        }

        if (nRenderMode == 0 || nRenderMode == 2 || nRenderMode == 4 || nRenderMode == 6)
        {
        #ifdef BUILDING_WASM_MODULE
            std::wstring sFontPath;
            m_pRenderer->get_FontPath(&sFontPath);
            if (!unGid && !wsUnicodeText.empty() && !sFontPath.empty())
            {
                unsigned int lUnicode = (unsigned int)wsUnicodeText[0];
                long lStyle;
                m_pRenderer->get_FontStyle(&lStyle);
                m_pFontManager->LoadFontFromFile(sFontPath, 0, 10, 72, 72);

                NSFonts::IFontFile* pFontFile = m_pFontManager->GetFile();
                if (pFontFile)
                {
                    int nCMapIndex = 0;
                    int GID = pFontFile->SetCMapForCharCode(lUnicode, &nCMapIndex);
                    if (GID <= 0 && lUnicode < 0xF000)
                        GID = pFontFile->SetCMapForCharCode(lUnicode + 0xF000, &nCMapIndex);

                    if (GID <= 0)
                    {
                        std::wstring sName = m_pFontManager->GetApplication()->GetFontBySymbol(lUnicode);
                        int bBold   = lStyle & 0x01 ? 1 : 0;
                        int bItalic = lStyle & 0x02 ? 1 : 0;

						if (!sName.empty())
						{
							if (!NSWasm::IsJSEnv())
							{
								NSFonts::CFontSelectFormat oFormat;
								oFormat.wsName  = new std::wstring(sName);
								oFormat.bBold   = new INT(bBold);
								oFormat.bItalic = new INT(bItalic);
								NSFonts::CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFormat);

								sName = pFontInfo->m_wsFontPath;
							}

							std::wstring wsFileName = NSWasm::LoadFont(sName, bBold, bItalic);
							if (wsFileName.empty())
							{
								m_pFontList->Remove(*pGState->getFont()->getID());
								return;
							}
							m_pRenderer->put_FontPath(wsFileName);
						}
                    }
                }
            }
        #endif

            m_pRenderer->CommandDrawTextEx(wsUnicodeText, &unGid, unGidsCount, PDFCoordsToMM(0 + dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));
        }

        if (nRenderMode == 1 || nRenderMode == 2 || nRenderMode == 5 || nRenderMode == 6)
        {
            m_pRenderer->BeginCommand(c_nStrokeTextType);

            m_pRenderer->PathCommandEnd();
            if (unGid)
                m_pRenderer->PathCommandTextEx(wsUnicodeText, &unGid, unGidsCount, PDFCoordsToMM(dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));
            else
                m_pRenderer->PathCommandText(wsUnicodeText, PDFCoordsToMM(dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));
            m_pRenderer->DrawPath(c_nStroke);

            m_pRenderer->EndCommand(c_nStrokeTextType);
        }

        if (4 <= nRenderMode)
        {
            std::wstring wsTempFontName, wsTempFontPath;
            std::wstring wsClipText; wsClipText += (wchar_t)(unGid);
            double dTempFontSize;
            long lTempFontStyle;
            m_pRenderer->get_FontName(&wsTempFontName);
            m_pRenderer->get_FontPath(&wsTempFontPath);
            m_pRenderer->get_FontSize(&dTempFontSize);
            m_pRenderer->get_FontStyle(&lTempFontStyle);
//            tmpchange
            if (m_sClip.empty())
            {
                m_sClip.push_back(GfxClip());
            }
            m_sClip.back().GetTextClip()->ClipToText(wsTempFontName, wsTempFontPath, dTempFontSize, (int)lTempFontStyle, arrMatrix, wsClipText, 0 + dShiftX, /*-fabs(pFont->getFontBBox()[3]) * dTfs*/ + dShiftY, 0, 0, 0);
            m_bClipChanged = true;
        }

        m_pRenderer->put_FontSize(dOldSize);
    }

    void RendererOutputDev::endType3Char(GfxState *pGState)
    {
        return;
    }
    void RendererOutputDev::Type3D0(GfxState *pGState, double dWx, double dWy)
    {
        return;
    }
    void RendererOutputDev::Type3D1(GfxState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy)
    {
        return;
    }
    void RendererOutputDev::drawImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight,GBool bInvert, GBool bInlineImage, GBool interpolate)
    {
        if (m_bDrawOnlyText)
            return;

        if (pGState->getFillColorSpace()->isNonMarking())
        {
            return;
        }

        double dPageHeight = pGState->getPageHeight();

        int nBufferSize = 4 * nWidth * nHeight;
        if (nBufferSize < 1)
            return;

        unsigned char *pBufferPtr = new unsigned char[nBufferSize];
        if (!pBufferPtr)
            return;

        Aggplus::CImage oImage;
        oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);

        // Пишем данные в pBufferPtr
        ImageStream *pImageStream = new ImageStream(pStream, nWidth, 1, 1);

        pImageStream->reset();

        GfxColorSpace* pColorSpace = pGState->getFillColorSpace();
        GfxRGB oRGB;
        pColorSpace->getRGB(pGState->getFillColor(), &oRGB, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);

        unsigned char r = colToByte(oRGB.r);
        unsigned char g = colToByte(oRGB.g);
        unsigned char b = colToByte(oRGB.b);

        unsigned char unAlpha = m_bTransparentGroup ? (m_bIsolatedTransparentGroup ? 0 : 255.0 * pGState->getFillOpacity()) : 255;
        int nInvert = (bInvert ? 1 : 0);
        for (int nY = nHeight - 1; nY >= 0; nY--)
        {
            unsigned char *pMask = NULL;
            int nX = 0;
            for (nX = 0, pMask = pImageStream->getLine(); nX < nWidth; nX++)
            {
                int nIndex = 4 * (nX + nY * nWidth);
                unsigned char unPixel = *pMask++ ^ nInvert;
                pBufferPtr[nIndex + 0] = unPixel ? 255 : b;
                pBufferPtr[nIndex + 1] = unPixel ? 255 : g;
                pBufferPtr[nIndex + 2] = unPixel ? 255 : r;
                pBufferPtr[nIndex + 3] = unPixel ? 0 : unAlpha;
            }
        }

        delete pImageStream;

        double arrMatrix[6];
        double *pCTM = pGState->getCTM();

        //  Исходное предобразование
        //              |1  0  0|   |pCTM[0] pCTM[1] 0|
        // arrMattrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
        //              |0  1  1|   |pCTM[4] pCTM[5] 1|

        arrMatrix[0] =     pCTM[0];
        arrMatrix[1] =  -pCTM[1];
        arrMatrix[2] =    -pCTM[2];
        arrMatrix[3] =  -(-pCTM[3]);
        arrMatrix[4] =     pCTM[2] + pCTM[4];
        arrMatrix[5] =  -(pCTM[3] + pCTM[5]) + dPageHeight;

        double dShiftX = 0, dShiftY = 0;
        DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
        m_pRenderer->DrawImage(&oImage, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
    }
    void RendererOutputDev::setSoftMaskFromImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate)
    {
        if (m_bDrawOnlyText)
            return;

        if (pGState->getFillColorSpace()->isNonMarking())
        {
            return;
        }

        double dPageHeight = pGState->getPageHeight();

        int nBufferSize = 4 * nWidth * nHeight;
        if (nBufferSize < 1)
            return;

        unsigned char *pBufferPtr = new unsigned char[nBufferSize];
        if (!pBufferPtr)
            return;
        /*
        RELEASEARRAYOBJECTS(m_pSoftMask);
        m_pSoftMask = pBufferPtr;
        m_nSoftMaskWidth  = nWidth;
        m_nSoftMaskHeight = nHeight;
        */

        Aggplus::CImage oImage;
        oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth, true);

        // Пишем данные в pBufferPtr
        ImageStream *pImageStream = new ImageStream(pStream, nWidth, 1, 1);

        pImageStream->reset();

        GfxColorSpace* pColorSpace = pGState->getFillColorSpace();
        GfxRGB oRGB;
        pColorSpace->getRGB(pGState->getFillColor(), &oRGB, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
        GfxPattern* pPattern = pGState->getFillPattern();
        if (pPattern && pPattern->getType() == 2)
        {
            GfxShading *pShading = ((GfxShadingPattern*)pPattern)->getShading();
            pColorSpace = pShading->getColorSpace();
            if (pShading->getHasBackground())
                pColorSpace->getRGB(pShading->getBackground(), &oRGB, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
        }

        unsigned char r = colToByte(oRGB.r);
        unsigned char g = colToByte(oRGB.g);
        unsigned char b = colToByte(oRGB.b);

        double dAlphaKoef = m_bTransparentGroup ? (m_bIsolatedTransparentGroup ? 0 : pGState->getFillOpacity()) : 1;
        int nInvert = (bInvert ? 1 : 0);
        for (int nY = nHeight - 1; nY >= 0; nY--)
        {
            unsigned char *pMask = NULL;
            int nX = 0;
            int nIndex = 4 * nY * nWidth;
            for (nX = 0, pMask = pImageStream->getLine(); nX < nWidth; nX++)
            {
                unsigned char unPixel = *pMask++ ^ nInvert;
                pBufferPtr[nIndex + 0] = unPixel ? 255 : b;
                pBufferPtr[nIndex + 1] = unPixel ? 255 : g;
                pBufferPtr[nIndex + 2] = unPixel ? 255 : r;
                pBufferPtr[nIndex + 3] = unPixel ? 0 : (unsigned char)(255.0 * dAlphaKoef);
                nIndex += 4;
            }
        }

        delete pImageStream;

        double arrMatrix[6];
        double *pCTM = pGState->getCTM();

        //  Исходное предобразование
        //              |1  0  0|   |pCTM[0] pCTM[1] 0|
        // arrMattrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
        //              |0  1  1|   |pCTM[4] pCTM[5] 1|

        arrMatrix[0] =     pCTM[0];
        arrMatrix[1] =  -pCTM[1];
        arrMatrix[2] =    -pCTM[2];
        arrMatrix[3] =  -(-pCTM[3]);
        arrMatrix[4] =     pCTM[2] + pCTM[4];
        arrMatrix[5] =  -(pCTM[3] + pCTM[5]) + dPageHeight;

        double dShiftX = 0, dShiftY = 0;
        DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
        m_pRenderer->DrawImage(&oImage, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
    }

    OO_INLINE bool CheckMask(const int& nComponentsCount, const int* pMaskColors, const unsigned char* pLine)
    {
        bool isMask = true;
        for (int nCompIndex = 0; nCompIndex < nComponentsCount; ++nCompIndex)
        {
            if (pMaskColors[nCompIndex * 2] > pLine[nCompIndex] || pLine[nCompIndex] > pMaskColors[nCompIndex * 2 + 1])
            {
                isMask = false;
                break;
            }
        }

        return isMask;
    }

    void RendererOutputDev::drawImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, int *pMaskColors, GBool bInlineImg, GBool interpolate)
    {
        if (m_bDrawOnlyText)
            return;

        double dPageHeight = pGState->getPageHeight();

        int nBufferSize = 4 * nWidth * nHeight;
        if (nBufferSize < 1)
            return;

        unsigned char *pBufferPtr = new unsigned char[nBufferSize];
        if (!pBufferPtr)
            return;

        int nComponentsCount = pColorMap->getNumPixelComps();

        // Пишем данные в pBufferPtr
        ImageStream *pImageStream = new ImageStream(pStream, nWidth, nComponentsCount, pColorMap->getBits());
        pImageStream->reset();

        bool bTransperent = false;
        for (const bool& b : m_arrTransparentGroupSoftMask)
            bTransperent = b || bTransperent;
        unsigned char unAlpha = m_bTransparentGroup ? ((m_bIsolatedTransparentGroup && bTransperent) ? 0 : 255.0 * pGState->getFillOpacity()) : 255;

        int nStride = pImageStream->getVals();
        int nComps = pImageStream->getComps();
        int nWidthMax = nStride / nComps;
        int nCheckWidth = std::min(nWidth, nWidthMax);

        // fast realization for some colorspaces (for wasm module)
        int nColorMapType = pColorMap->getFillType();
        GfxColorComp** pColorMapLookup = pColorMap->getLookup();
        if (!pColorMapLookup)
            nColorMapType = 0;

        for (int nY = nHeight - 1; nY >= 0; --nY)
        {
            unsigned char* pLine = pImageStream->getLine();
            unsigned char* pLineDst = pBufferPtr + 4 * nWidth * nY;

            if (!pLine)
            {
                memset(pLineDst, 0, 4 * nWidth);
                continue;
            }

            for (int nX = 0; nX < nCheckWidth; ++nX)
            {
                if (2 == nColorMapType)
                {
                    pLineDst[2] = colToByte(clip01(pColorMapLookup[0][pLine[0]]));
                    pLineDst[1] = colToByte(clip01(pColorMapLookup[1][pLine[1]]));
                    pLineDst[0] = colToByte(clip01(pColorMapLookup[2][pLine[2]]));
                }
                else if (1 == nColorMapType)
                {
                    pLineDst[0] = pLineDst[1] = pLineDst[2] = colToByte(clip01(pColorMapLookup[0][pLine[0]]));
                }
                else
                {
                    GfxRGB oRGB;
                    pColorMap->getRGB(pLine, &oRGB, gfxRenderingIntentAbsoluteColorimetric);
                    pLineDst[0] = colToByte(oRGB.b);
                    pLineDst[1] = colToByte(oRGB.g);
                    pLineDst[2] = colToByte(oRGB.r);
                }

                if (pMaskColors && CheckMask(nComponentsCount, pMaskColors, pLine))
                    pLineDst[3] = 0;
                else
                    pLineDst[3] = unAlpha;

                pLine += nComps;
                pLineDst += 4;
            }
        }

        Aggplus::CImage oImage;
		oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);

        pImageStream->close();
		delete pImageStream;

        double arrMatrix[6];
        double *pCTM = pGState->getCTM();
        //  Исходное предобразование
        //             |1  0  0|   |pCTM[0] pCTM[1] 0|
        // arrMatrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
        //             |0  1  1|   |pCTM[4] pCTM[5] 1|
        arrMatrix[0] =     pCTM[0];
        arrMatrix[1] =  -pCTM[1];
        arrMatrix[2] =    -pCTM[2];
        arrMatrix[3] =  -(-pCTM[3]);
        arrMatrix[4] =     pCTM[2] + pCTM[4];
        arrMatrix[5] =  -(pCTM[3] + pCTM[5]) + dPageHeight;

        double dShiftX = 0, dShiftY = 0;
        DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
        m_pRenderer->DrawImage(&oImage, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
    }
	void RendererOutputDev::drawMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, Object* pStreamRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GBool bMaskInvert, GBool interpolate)
    {
        if (m_bDrawOnlyText)
            return;

        if (nMaskWidth <= 0 || nMaskHeight <= 0)
            drawImage(pGState, pRef, pStream, nWidth, nHeight, pColorMap, NULL, false, interpolate);

        if (nMaskWidth > nWidth || nMaskHeight > nHeight)
        {
            // If the mask is higher resolution than the image, use
            // drawSoftMaskedImage() instead.

            GfxImageColorMap *maskColorMap;
            Object maskDecode, decodeLow, decodeHigh;

            decodeLow.initInt(bMaskInvert ? 0 : 1);
            decodeHigh.initInt(bMaskInvert ? 1 : 0);
            maskDecode.initArray(m_pXref);
            maskDecode.arrayAdd(&decodeLow);
            maskDecode.arrayAdd(&decodeHigh);
            maskColorMap = new GfxImageColorMap(1, &maskDecode, new GfxDeviceGrayColorSpace());
            maskDecode.free();
            drawSoftMaskedImage(pGState, pRef, pStream, nWidth, nHeight,
                                pColorMap, pStreamRef, pMaskStream, nMaskWidth, nMaskHeight, maskColorMap, NULL, interpolate);
            delete maskColorMap;
            return;
        }

        double dPageHeight = pGState->getPageHeight();

        int nBufferSize = 4 * nWidth * nHeight;
        if (nBufferSize < 1)
            return;

        unsigned char *pBufferPtr = new unsigned char[nBufferSize];
        if (!pBufferPtr)
            return;

        Aggplus::CImage oImage;
        oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);

        // Пишем данные в pBufferPtr
        ImageStream *pImageStream = new ImageStream(pStream, nWidth, pColorMap->getNumPixelComps(), pColorMap->getBits());
        ImageStream *pMask = new ImageStream(pMaskStream, nMaskWidth, 1, 1);

        pMask->reset();
        pImageStream->reset();

        if (nWidth != nMaskWidth || nHeight != nMaskHeight)
        {
            unsigned char *pMaskBuffer = new unsigned char[nMaskWidth * nMaskHeight];
            if (!pMaskBuffer)
            {
                delete pMask;
                delete pImageStream;
                return;
            }

            unsigned char unMask = 0;
            for (int nY = nMaskHeight - 1; nY >= 0; nY--)
            {
                int nIndex = nY * nMaskWidth;
                for (int nX = 0; nX < nMaskWidth; nX++)
                {
                    pMask->getPixel(&unMask);
                    pMaskBuffer[nIndex++] = unMask;
                }
            }

            double dScaleWidth  = (double)nWidth / (double)nMaskWidth;
            double dScaleHeight = (double)nHeight / (double)nMaskHeight;

            unsigned char unPixel[4] ={ 0, 0, 0, 0 };
            for (int nY = nHeight - 1; nY >= 0; nY--)
            {
                int nIndex = 4 * nY * nWidth;
                for (int nX = 0; nX < nWidth; nX++)
                {
                    pImageStream->getPixel(unPixel);

                    int nNearestY = (std::min)((int)(nY / dScaleHeight), nMaskHeight - 1);
                    int nNearestX = (std::min)((int)(nX / dScaleWidth), nMaskWidth - 1);
                    unMask = pMaskBuffer[nNearestY * nMaskWidth + nNearestX];

                    GfxRGB oRGB;
                    pColorMap->getRGB(unPixel, &oRGB, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
                    pBufferPtr[nIndex + 0] = colToByte(oRGB.b);
                    pBufferPtr[nIndex + 1] = colToByte(oRGB.g);
                    pBufferPtr[nIndex + 2] = colToByte(oRGB.r);

                    if (unMask && !bMaskInvert)
                        pBufferPtr[nIndex + 3] = 0;
                    else
                        pBufferPtr[nIndex + 3] = 255;

                    nIndex += 4;
                }
            }

            delete[] pMaskBuffer;
        }
        else
        {
            unsigned char unPixel[4] ={ 0, 0, 0, 0 };
            unsigned char unMask = 0;
            for (int nY = nHeight - 1; nY >= 0; nY--)
            {
                int nIndex = 4 * nY * nWidth;
                for (int nX = 0; nX < nWidth; nX++)
                {
                    pImageStream->getPixel(unPixel);
                    pMask->getPixel(&unMask);
                    GfxRGB oRGB;
                    pColorMap->getRGB(unPixel, &oRGB, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
                    pBufferPtr[nIndex + 0] = colToByte(oRGB.b);
                    pBufferPtr[nIndex + 1] = colToByte(oRGB.g);
                    pBufferPtr[nIndex + 2] = colToByte(oRGB.r);

                    if (unMask && !bMaskInvert)
                        pBufferPtr[nIndex + 3] = 0;
                    else
                        pBufferPtr[nIndex + 3] = 255;

                    nIndex += 4;
                }
            }
        }

        delete pMask;
        delete pImageStream;

        double arrMatrix[6];
        double *pCTM = pGState->getCTM();
        //  Исходное предобразование
        //             |1  0  0|   |pCTM[0] pCTM[1] 0|
        // arrMatrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
        //             |0  1  1|   |pCTM[4] pCTM[5] 1|
        arrMatrix[0] =     pCTM[0];
        arrMatrix[1] =  -pCTM[1];
        arrMatrix[2] =    -pCTM[2];
        arrMatrix[3] =  -(-pCTM[3]);
        arrMatrix[4] =     pCTM[2] + pCTM[4];
        arrMatrix[5] =  -(pCTM[3] + pCTM[5]) + dPageHeight;

        double dShiftX = 0, dShiftY = 0;
        DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
        m_pRenderer->DrawImage(&oImage, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
    }
    void RendererOutputDev::drawSoftMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream,
                                                int nWidth, int nHeight,
                                                GfxImageColorMap *pColorMap,
                                                Object *maskRef, Stream *pMaskStream,
                                                int nMaskWidth, int nMaskHeight,
                                                GfxImageColorMap *pMaskColorMap,
                                                double *pMatteColor, GBool interpolate)
    {
        if (m_bDrawOnlyText)
            return;

        double dPageHeight = pGState->getPageHeight();

		int nBufferSize = 4 * nWidth * nHeight;
        if (nBufferSize < 1)
            return;

        unsigned char *pBufferPtr = new unsigned char[nBufferSize];
        if (!pBufferPtr)
			return;

		Aggplus::CImage oImage;
		oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);

        // Пишем данные в pBufferPtr
        ImageStream *pImageStream = new ImageStream(pStream, nWidth, pColorMap->getNumPixelComps(), pColorMap->getBits());
        pImageStream->reset();

        double dAlphaKoef = m_bTransparentGroup ? (m_bIsolatedTransparentGroup ? 0 : pGState->getFillOpacity()) : 1;
        unsigned char unPixel[4] = { 0, 0, 0, 0 };
        for (int nY = nHeight - 1; nY >= 0; nY--)
        {
            int nIndex = 4 * nY * nWidth;
            for (int nX = 0; nX < nWidth; nX++)
            {
                pImageStream->getPixel(unPixel);
                GfxRGB oRGB;
                pColorMap->getRGB(unPixel, &oRGB, gfxRenderingIntentAbsoluteColorimetric);
                pBufferPtr[nIndex + 0] = colToByte(oRGB.b);
                pBufferPtr[nIndex + 1] = colToByte(oRGB.g);
                pBufferPtr[nIndex + 2] = colToByte(oRGB.r);
                pBufferPtr[nIndex + 3] = 255;

                nIndex += 4;
            }
        }
        delete pImageStream;

		if (nWidth != nMaskWidth || nHeight != nMaskHeight)
        {
            // TO DO: Здесь сделан элементарный вариант масштабирования маски.
            //        Надо улучшить алгоритм.

            bool bResize = true;

            if (0 != nWidth && 0 != nMaskHeight && 0 != nHeight && 0 != nMaskWidth)
            {
                ImageStream *pSMaskStream = new ImageStream(pMaskStream, nMaskWidth, pMaskColorMap->getNumPixelComps(), pMaskColorMap->getBits());
                unsigned char *pAlpha = new unsigned char[nMaskWidth * nMaskHeight];
                if (pSMaskStream && pAlpha)
                {
                    pSMaskStream->reset();

                    unsigned char unAlpha = 0;
                    for (int i = 0, nCount = nMaskWidth * nMaskHeight; i < nCount; ++i)
                    {
                        pSMaskStream->getPixel(&unAlpha);
                        GfxGray oGray;
                        pMaskColorMap->getGray(&unAlpha, &oGray, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
                        pAlpha[i] = colToByte(oGray);
                    }
                    delete pSMaskStream;

                    int nMaxW = (std::max)(nWidth, nMaskWidth);
                    int nMaxH = (std::max)(nHeight, nMaskHeight);

                    double dAlphaScaleWidth  = (double)nMaskWidth / (double)nMaxW;
                    double dAlphaScaleHeight = (double)nMaskHeight / (double)nMaxH;

					if (nWidth != nMaxW || nHeight != nMaxH)
					{
                        unsigned char* pImageBuffer = pBufferPtr;
                        pBufferPtr = new unsigned char[4 * nMaxW * nMaxH];
                        if (!pBufferPtr)
                        {
                            delete[] pImageBuffer;
                            delete[] pAlpha;
                            return;
                        }

						oImage.Create(pBufferPtr, nMaxW, nMaxH, -4 * nMaxW);

                        double dImageScaleWidth  = (double)nWidth / (double)nMaxW;
                        double dImageScaleHeight = (double)nHeight / (double)nMaxH;

                        for (int nY = nMaxH - 1; nY >= 0; nY--)
                        {
                            int nIndex = 4 * nY * nMaxW;
                            for (int nX = 0; nX < nMaxW; nX++)
                            {
                                int nNearestAlphaMatch =  (((int)((nMaxH - 1 - nY) * dAlphaScaleHeight) * nMaskWidth) + ((int)(nX * dAlphaScaleWidth)));
								int nNearestImageMatch =  4 * (((int)(nY * dImageScaleHeight) * nWidth) + ((int)(nX * dImageScaleWidth)));

                                pBufferPtr[nIndex + 0] = pImageBuffer[nNearestImageMatch + 0];
                                pBufferPtr[nIndex + 1] = pImageBuffer[nNearestImageMatch + 1];
                                pBufferPtr[nIndex + 2] = pImageBuffer[nNearestImageMatch + 2];
                                pBufferPtr[nIndex + 3] = (unsigned char)(pAlpha[nNearestAlphaMatch] * dAlphaKoef);
                                nIndex += 4;
                            }
						}

                        delete[] pImageBuffer;
                    }
                    else
                    {
                        for (int nY = nHeight - 1; nY >= 0; nY--)
                        {
                            int nIndex = 4 * nY * nWidth;
                            for (int nX = 0; nX < nWidth; nX++)
                            {
                                int nNearestAlphaMatch =  (((int)((nHeight - 1 - nY) * dAlphaScaleHeight) * nMaskWidth) + ((int)(nX * dAlphaScaleWidth)));

                                pBufferPtr[nIndex + 3] = (unsigned char)(pAlpha[nNearestAlphaMatch] * dAlphaKoef);
                                nIndex += 4;
                            }
                        }
                    }

                    delete[] pAlpha;
                }
                else
                {
                    if (pAlpha)
                        delete[] pAlpha;

                    if (pSMaskStream)
                        delete pSMaskStream;

                    bResize = false;
                }
            }
            else
                bResize = false;

            if (!bResize && dAlphaKoef < 1.0)
            {
                for (int i = 3, nCount = nWidth * nHeight * 4; i < nCount; i += 4)
                {
                    pBufferPtr[i] = (unsigned char)(255.0 * dAlphaKoef);
                }
            }
        }
		else
        {
            ImageStream *pSMaskStream = new ImageStream(pMaskStream, nMaskWidth, pMaskColorMap->getNumPixelComps(), pMaskColorMap->getBits());
            pSMaskStream->reset();

            unsigned char unAlpha = 0;
            for (int nY = nHeight - 1; nY >= 0; nY--)
            {
                int nIndex = 4 * nY * nWidth;
                for (int nX = 0; nX < nWidth; nX++)
                {
                    pSMaskStream->getPixel(&unAlpha);
                    GfxGray oGray;
                    pMaskColorMap->getGray(&unAlpha, &oGray, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
                    pBufferPtr[nIndex + 3] = (unsigned char)(colToByte(oGray) * dAlphaKoef);
                    nIndex += 4;
                }
            }
            delete pSMaskStream;
        }

        // Undo preblend
		if (pMatteColor)
        {
            GfxRGB oMatteRGB;
            GfxColor oColor;
            for (int i = 0; i < pColorMap->getNumPixelComps(); ++i) {
                oColor.c[i] = dblToCol(pMatteColor[i]);
            }
            pColorMap->getColorSpace()->getRGB(&oColor, &oMatteRGB, gfxRenderingIntentAbsoluteColorimetric);

            unsigned char unMatteR = colToByte(oMatteRGB.r);
            unsigned char unMatteG = colToByte(oMatteRGB.g);
            unsigned char unMatteB = colToByte(oMatteRGB.b);

            for (int nIndex = 0; nIndex < nHeight * nWidth * 4; nIndex += 4)
            {
                unsigned char unA = pBufferPtr[nIndex + 3];

                if (0 == unA)
                {
                    pBufferPtr[nIndex + 0] = 255;
                    pBufferPtr[nIndex + 1] = 255;
                    pBufferPtr[nIndex + 2] = 255;
                }
                else
                {
                    double dK = 255.0 / unA;

                    pBufferPtr[nIndex + 0] = std::max(0, std::min(255, int((pBufferPtr[nIndex + 0] - unMatteB) * dK + unMatteB)));
                    pBufferPtr[nIndex + 1] = std::max(0, std::min(255, int((pBufferPtr[nIndex + 1] - unMatteG) * dK + unMatteG)));
                    pBufferPtr[nIndex + 2] = std::max(0, std::min(255, int((pBufferPtr[nIndex + 2] - unMatteR) * dK + unMatteR)));
                }
            }
        }

        double arrMatrix[6];
        double *pCTM = pGState->getCTM();
        //  Исходное предобразование
        //              |1  0  0|   |pCTM[0] pCTM[1] 0|
        // arrMattrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
        //              |0  1  1|   |pCTM[4] pCTM[5] 1|
        arrMatrix[0] =     pCTM[0];
        arrMatrix[1] =  -pCTM[1];
        arrMatrix[2] =    -pCTM[2];
        arrMatrix[3] =  -(-pCTM[3]);
        arrMatrix[4] =     pCTM[2] + pCTM[4];
        arrMatrix[5] =  -(pCTM[3] + pCTM[5]) + dPageHeight;

        double dShiftX = 0, dShiftY = 0;
        DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
        m_pRenderer->DrawImage(&oImage, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
    }
    void RendererOutputDev::beginTransparencyGroup(GfxState *pGState, double *pBBox, GfxColorSpace *pBlendingColorSpace, GBool bIsolated, GBool bKnockout, GBool bForSoftMask)
    {
        m_bTransparentGroup = true;
        m_bIsolatedTransparentGroup = bIsolated;
        m_bTransparentGroupSoftMask = bForSoftMask;
        m_arrTransparentGroupSoftMask.push_back(bForSoftMask);
    }
    void RendererOutputDev::endTransparencyGroup(GfxState *pGState)
    {
        if (m_bTransparentGroupSoftMask)
            m_bTransparentGroupSoftMaskEnd = true;

        m_arrTransparentGroupSoftMask.pop_back();
        m_bTransparentGroup = false;
        m_bTransparentGroupSoftMask = m_arrTransparentGroupSoftMask.empty() ? false : m_arrTransparentGroupSoftMask.back();
    }
    void RendererOutputDev::paintTransparencyGroup(GfxState *pGState, double *pBBox)
    {
        m_bIsolatedTransparentGroup = false;
        m_bTransparentGroupSoftMaskEnd = false;
        /*
        if (!m_pTransparentGroupSoftMask)
            return;

        double arrMatrix[6];
        double *pCTM = pGState->getCTM();
        double dPageHeight = pGState->getPageHeight();

        arrMatrix[0] =     pCTM[0];
        arrMatrix[1] =  -pCTM[1];
        arrMatrix[2] =    -pCTM[2];
        arrMatrix[3] =  -(-pCTM[3]);
        arrMatrix[4] =     pCTM[2] + pCTM[4];
        arrMatrix[5] =  -(pCTM[3] + pCTM[5]) + dPageHeight;

        double dShiftX = pBBox[0], dShiftY = pBBox[1];
        DoTransform(arrMatrix, &dShiftX, &dShiftY, true);

        Aggplus::CImage oImage;
        oImage.Create(m_pTransparentGroupSoftMask->get_Data(), m_pTransparentGroupSoftMask->get_Width(), m_pTransparentGroupSoftMask->get_Height(), m_pTransparentGroupSoftMask->get_Stride());
        m_pTransparentGroupSoftMask->ClearNoAttack();

        m_pRenderer->DrawImage(&oImage, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));

        RELEASEOBJECT(m_pTransparentGroupSoftMask);
        */
    }
    void RendererOutputDev::setSoftMask(GfxState *pGState, double *pBBox, GBool bAlpha, Function *pTransferFunc, GfxColor *pBackdropColor)
    {
        m_bIsolatedTransparentGroup = false;
        //m_bTransparentGroupSoftMaskEnd = false;
    }
    void RendererOutputDev::clearSoftMask(GfxState *pGState)
    {
    }
    void RendererOutputDev::NewPDF(XRef *pXref)
    {
        m_pXref = pXref;
    }
    void RendererOutputDev::Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY)
    {
        *pdDeviceX = dUserX * pMatrix[0] + dUserY * pMatrix[2] + pMatrix[4];
        *pdDeviceY = dUserX * pMatrix[1] + dUserY * pMatrix[3] + pMatrix[5];
    }
    void RendererOutputDev::DoPath(GfxState *pGState, GfxPath *pPath, double dPageHeight, double *pCTM, GfxClipMatrix* pCTM2)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_bTransparentGroupSoftMask)
            return;

        double arrMatrix[6];
        //double *pCTM = pGState->getCTM();
        arrMatrix[0] =  pCTM[0];
        arrMatrix[1] = -pCTM[1];
        arrMatrix[2] =  pCTM[2];
        arrMatrix[3] = -pCTM[3];
        arrMatrix[4] =  pCTM[4];
        arrMatrix[5] = -pCTM[5] + dPageHeight;
        if (pCTM2)
        {
            arrMatrix[0] =  pCTM2->dA;
            arrMatrix[1] = -pCTM2->dB;
            arrMatrix[2] =  pCTM2->dC;
            arrMatrix[3] = -pCTM2->dD;
            arrMatrix[4] =  pCTM2->dE;
            arrMatrix[5] = -pCTM2->dF + dPageHeight;
        }

        double dShiftX = 0, dShiftY = 0;
        DoTransform(arrMatrix, &dShiftX, &dShiftY);

        m_pRenderer->BeginCommand(c_nPathType);
        m_pRenderer->PathCommandEnd();

        for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
        {
            GfxSubpath *pSubpath = pPath->getSubpath(nSubPathIndex);
            int nPointsCount = pSubpath->getNumPoints();

            m_pRenderer->PathCommandMoveTo(PDFCoordsToMM(pSubpath->getX(0) + dShiftX), PDFCoordsToMM(pSubpath->getY(0) + dShiftY));

            int nCurPointIndex = 1;
            while (nCurPointIndex < nPointsCount)
            {
                if (pSubpath->getCurve(nCurPointIndex))
                {
                    m_pRenderer->PathCommandCurveTo(PDFCoordsToMM(pSubpath->getX(nCurPointIndex) + dShiftX), PDFCoordsToMM(pSubpath->getY(nCurPointIndex) + dShiftY), PDFCoordsToMM(pSubpath->getX(nCurPointIndex + 1) + dShiftX), PDFCoordsToMM(pSubpath->getY(nCurPointIndex + 1) + dShiftY), PDFCoordsToMM(pSubpath->getX(nCurPointIndex + 2) + dShiftX), PDFCoordsToMM(pSubpath->getY(nCurPointIndex + 2) + dShiftY));
                    nCurPointIndex += 3;
                }
                else
                {
                    m_pRenderer->PathCommandLineTo(PDFCoordsToMM(pSubpath->getX(nCurPointIndex) + dShiftX), PDFCoordsToMM(pSubpath->getY(nCurPointIndex) + dShiftY));
                    ++nCurPointIndex;
                }
            }
            if (pSubpath->isClosed())
            {
                m_pRenderer->PathCommandClose();
            }
        }
    }
    void RendererOutputDev::updateClip(GfxState *pGState)
    {
        if (m_bDrawOnlyText)
            return;

        if (m_bTransparentGroupSoftMask)
            return;

        if (m_bTiling)
            return;

        updateClipAttack(pGState);
    }
    void RendererOutputDev::updateClipAttack(GfxState *pGState)
    {
		if (!m_bClipChanged)
			return;

		m_pRenderer->BeginCommand(c_nResetClipType);
        m_pRenderer->EndCommand(c_nResetClipType);

		if (m_sClip.empty())
			return;

        //for (int i = m_sClip.size() - 1; i >= 0; i--)
        for (int i = 0; i < m_sClip.size(); i++)
        {
            for (int nIndex = 0; nIndex < m_sClip[i].GetPathNum(); nIndex++)
            {
                GfxPath *pPath  = m_sClip[i].GetPath(nIndex);
                bool    bFlag   = m_sClip[i].GetClipEo(nIndex);

                int     nClipFlag = bFlag ? c_nClipRegionTypeEvenOdd : c_nClipRegionTypeWinding;
                nClipFlag |= c_nClipRegionIntersect;

                m_pRenderer->BeginCommand(c_nClipType);
                m_pRenderer->put_ClipMode(nClipFlag);
                DoPath(pGState, pPath, pGState->getPageHeight(), pGState->getCTM(), &m_sClip[i].m_vMatrix[nIndex]);
                m_pRenderer->EndCommand(c_nPathType);
                m_pRenderer->EndCommand(c_nClipType);
                m_pRenderer->PathCommandEnd();
            }

            int nTextClipCount = m_sClip[i].GetTextClip()->GetTextsCount();
            if (nTextClipCount > 0)
            {
                m_pRenderer->BeginCommand(c_nClipType);
                m_pRenderer->put_ClipMode(c_nClipRegionTypeWinding | c_nClipRegionIntersect);
                m_pRenderer->StartConvertCoordsToIdentity();

                for (int nIndex = 0; nIndex < nTextClipCount; nIndex++)
                {
                    wchar_t *wsFontName, *wsFontPath;
                    int lFontStyle;
                    double dFontSize = 10, dX = 0, dY = 0, dWidth = 0, dHeight = 0, dBaseLineOffset = 0;
                    wchar_t *wsText = m_sClip[i].GetTextClip()->GetText(nIndex, &dX, &dY, &dWidth, &dHeight, &dBaseLineOffset, &wsFontName, &wsFontPath, &dFontSize, &lFontStyle);

                    m_pRenderer->put_FontName(wsFontName);
                    m_pRenderer->put_FontPath(wsFontPath);
                    m_pRenderer->put_FontSize(dFontSize);
                    m_pRenderer->put_FontStyle(lFontStyle);

                    double dShiftX = 0, dShiftY = 0;
                    DoTransform(m_sClip[i].GetTextClip()->GetMatrix(nIndex), &dShiftX, &dShiftY, true);

                    // TODO: нужна нормальная конвертация
                    int nLen = 0;
                    wchar_t *wsTextTmp = wsText;
                    if (wsTextTmp)
                    {
                        while (*wsTextTmp)
                            ++wsTextTmp;

                        nLen = (int)(wsTextTmp - wsText);
                    }

                    if (1 == nLen)
                        m_pRenderer->PathCommandTextExCHAR(0, (LONG)wsText[0], PDFCoordsToMM(dX), PDFCoordsToMM(dY), PDFCoordsToMM(dWidth), PDFCoordsToMM(dHeight));
                    else if (0 != nLen)
                    {
                        unsigned int* pGids = new unsigned int[nLen];
                        for (int nIndex = 0; nIndex < nLen; ++nIndex)
                            pGids[nIndex] = (unsigned int)wsText[nIndex];

                        m_pRenderer->PathCommandTextEx(L"", pGids, nLen, PDFCoordsToMM(dX), PDFCoordsToMM(dY), PDFCoordsToMM(dWidth), PDFCoordsToMM(dHeight));

                        RELEASEARRAYOBJECTS(pGids);
                    }

                }

                m_pRenderer->EndCommand(c_nPathType);
                m_pRenderer->EndCommand(c_nClipType);
                m_pRenderer->PathCommandEnd();
                m_pRenderer->EndConvertCoordsToIdentity();
            }
        }

        m_bClipChanged = false;


        updateFont(pGState);
    }
    void RendererOutputDev::DoTransform(double *pMatrix, double *pdShiftX, double *pdShiftY, bool bText)
    {
        if (1 == pMatrix[0] && 0 == pMatrix[1] && 0 == pMatrix[2] && 1 == pMatrix[3] && !bText)
        {
            if (0 == pMatrix[4] && 0 == pMatrix[5])
            {
                m_pRenderer->ResetTransform();
                m_arrMatrix[0] = 1; m_arrMatrix[1] = 0;
                m_arrMatrix[2] = 0; m_arrMatrix[3] = 1;
                m_arrMatrix[4] = 0; m_arrMatrix[5] = 0;
            }
            else
            {
                *pdShiftX = pMatrix[4];
                *pdShiftY = pMatrix[5];
                m_pRenderer->ResetTransform();
                m_arrMatrix[0] = 1;          m_arrMatrix[1] = 0;
                m_arrMatrix[2] = 0;          m_arrMatrix[3] = 1;
                m_arrMatrix[4] = 0;          m_arrMatrix[5] = 0;
            }
        }
        else if (m_arrMatrix[0] == pMatrix[0] && m_arrMatrix[1] == pMatrix[1] && m_arrMatrix[2] == pMatrix[2] && m_arrMatrix[3] == pMatrix[3]
        && m_arrMatrix[4] == pMatrix[4]  && m_arrMatrix[5] == pMatrix[5] && !bText)
        {
            double dNewX = pMatrix[4], dNewY = pMatrix[5];
            double dIDet = 1 / (pMatrix[0] * pMatrix[3] - pMatrix[1] * pMatrix[2]);

            *pdShiftX = ((dNewX - m_arrMatrix[4]) * m_arrMatrix[3] - (dNewY - m_arrMatrix[5]) * m_arrMatrix[1]) * dIDet;
            *pdShiftY = ((dNewY - m_arrMatrix[5]) * m_arrMatrix[0] - (dNewX - m_arrMatrix[4]) * m_arrMatrix[2]) * dIDet;
        }
        else
        {
            m_pRenderer->SetTransform(pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3], PDFCoordsToMM(pMatrix[4]), PDFCoordsToMM(pMatrix[5]));
            m_arrMatrix[0] = pMatrix[0]; m_arrMatrix[1] = pMatrix[1];
            m_arrMatrix[2] = pMatrix[2]; m_arrMatrix[3] = pMatrix[3];
            m_arrMatrix[4] = pMatrix[4]; m_arrMatrix[5] = pMatrix[5];
        }
        return;
    }
}
