/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "RendererOutputDev.h"
#include "Adaptors.h"
#include "../lib/xpdf/ErrorCodes.h"
#include "../lib/xpdf/GfxState.h"
#include "../lib/xpdf/GfxFont.h"
#include "../lib/fofi/FoFiTrueType.h"
#include "../lib/fofi/FoFiType1C.h"
#include "../lib/fofi/FoFiIdentifier.h"
#include "../lib/xpdf/Page.h"
#include "../lib/xpdf/Dict.h"
#include "../lib/xpdf/Stream.h"
#include "../lib/xpdf/PDFDoc.h"
#include "../lib/xpdf/CharCodeToUnicode.h"
#include "../lib/xpdf/TextString.h"
#include "XmlUtils.h"

#include "../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../DesktopEditor/graphics/Image.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/common/Array.h"
#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/graphics/BaseThread.h"
#include "../../DesktopEditor/graphics/commands/DocInfo.h"
#include "../../DesktopEditor/graphics/AlphaMask.h"
#include "../Resources/BaseFonts.h"
#include <new>

#ifndef BUILDING_WASM_MODULE
#define FONTS_USE_AFM_SETTINGS
#else
#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#include "FontsWasm.h"
#define FONTS_USE_ONLY_MEMORY_STREAMS
#endif

#if defined(_MSC_VER)
#define OO_INLINE __forceinline
#else
#define OO_INLINE inline
#endif

namespace PdfReader
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
			if ((nPos > 0) && (sName2.at(nPos - 1) == '-' || sName2.at(nPos - 1) == ','))
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

	void RendererOutputDev::CheckFontStylePDF(std::wstring& sName, bool& bBold, bool& bItalic)
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

		CheckFontNameStyle(sName, L"condensedbold");
		CheckFontNameStyle(sName, L"semibold");
		CheckFontNameStyle(sName, L"regular");

		CheckFontNameStyle(sName, L"ultraexpanded");
		CheckFontNameStyle(sName, L"extraexpanded");
		CheckFontNameStyle(sName, L"semiexpanded");
		CheckFontNameStyle(sName, L"expanded");

		CheckFontNameStyle(sName, L"ultracondensed");
		CheckFontNameStyle(sName, L"extracondensed");
		CheckFontNameStyle(sName, L"semicondensed");
		CheckFontNameStyle(sName, L"condensedlight");
		CheckFontNameStyle(sName, L"condensed");
		//CheckFontNameStyle(sName, L"light");

		if (CheckFontNameStyle(sName, L"bold_italic"))  { bBold = true; bItalic = true; }
		if (CheckFontNameStyle(sName, L"bold_oblique")) { bBold = true; bItalic = true; }

		if (CheckFontNameStyle(sName, L"boldmt")) bBold = true;
		if (CheckFontNameStyle(sName, L"bold"))   bBold = true;

		if (CheckFontNameStyle(sName, L"italicmt")) bItalic = true;
		if (CheckFontNameStyle(sName, L"italic"))   bItalic = true;
		if (CheckFontNameStyle(sName, L"oblique"))  bItalic = true;

		//if (CheckFontNameStyle(sName, L"bolditalicmt")) { bBold = true; bItalic = true; }
		//if (CheckFontNameStyle(sName, L"bolditalic")) { bBold = true; bItalic = true; }
		//if (CheckFontNameStyle(sName, L"boldoblique")) { bBold = true; bItalic = true; }
	}

	void CheckFontNamePDF(std::wstring& sName, NSFonts::CFontSelectFormat* format)
	{
		bool bBold   = false;
		bool bItalic = false;

		RendererOutputDev::CheckFontStylePDF(sName, bBold, bItalic);

		if (format)
		{
			if (bBold)
				format->bBold = new INT(1);
			if (bItalic)
				format->bItalic = new INT(1);
		}
	}

	void Transform(double* pMatrix, double dUserX, double dUserY, double* pdDeviceX, double* pdDeviceY)
	{
		*pdDeviceX = dUserX * pMatrix[0] + dUserY * pMatrix[2] + pMatrix[4];
		*pdDeviceY = dUserX * pMatrix[1] + dUserY * pMatrix[3] + pMatrix[5];
	}
	inline int luminosity(BYTE* p)
	{
		return (p[2]*77 + p[1]*150 + p[0]*29) >> 8;
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
	CPdfFontList::CPdfFontList()
	{
		m_oCS.InitializeCriticalSection();
		m_oFontMap.clear();
	}
	CPdfFontList::~CPdfFontList()
	{
		m_oCS.DeleteCriticalSection();
		Clear();
	}
	bool CPdfFontList::Find(Ref oRef, TFontEntry* pEntry)
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
	bool CPdfFontList::Find2(Ref oRef, TFontEntry** ppEntry)
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
			(*ppEntry) = Add(oRef, std::wstring(), NULL, NULL, 0, 0);
			(*ppEntry)->bAvailable = false;
		}

		RELEASEOBJECT(pCS);

		return bResult;
	}
	TFontEntry* CPdfFontList::Add(Ref oRef, const std::wstring& wsFileName, int* pCodeToGID, int* pCodeToUnicode, unsigned int unLenGID, unsigned int unLenUnicode)
	{
		// Данная функция приходит только из Find2, поэтому проверять есть ли данный шрифт уже не надо
		CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

		TFontEntry* pNewEntry = new TFontEntry;
		pNewEntry->wsFilePath     = wsFileName;
		pNewEntry->pCodeToGID     = pCodeToGID;
		pNewEntry->pCodeToUnicode = pCodeToUnicode;
		pNewEntry->unLenGID       = unLenGID;
		pNewEntry->unLenUnicode   = unLenUnicode;

		Add(oRef, pNewEntry);

		RELEASEOBJECT(pCS);

		return pNewEntry;
	}
	void CPdfFontList::Remove(Ref oRef)
	{
		CRefFontMap::iterator oPos = m_oFontMap.find(oRef);
		if (m_oFontMap.end() != oPos)
		{
			TFontEntry* pEntry = oPos->second;
			if (NULL != pEntry)
			{
				MemUtilsFree(pEntry->pCodeToGID);
				MemUtilsFree(pEntry->pCodeToUnicode);
			}
			delete pEntry;
			m_oFontMap.erase(oPos);
		}
	}
	void CPdfFontList::Clear()
	{
		for (auto const &oIt : m_oFontMap)
		{
			TFontEntry* pEntry = oIt.second;
			if (NULL != pEntry)
			{
				MemUtilsFree(pEntry->pCodeToGID);
				MemUtilsFree(pEntry->pCodeToUnicode);
			}
			delete pEntry;
		}
		m_oFontMap.clear();
	}
	bool CPdfFontList::GetFont(Ref* pRef, TFontEntry* pEntry)
	{
		TFontEntry* pFindEntry = Lookup(*pRef);
		if (NULL == pFindEntry)
			return false;

		*pEntry = *pFindEntry;
		return true;
	}
	TFontEntry* CPdfFontList::Lookup(Ref& oRef)
	{
		CRefFontMap::const_iterator oPos = m_oFontMap.find(oRef);
		return m_oFontMap.end() == oPos ? NULL : oPos->second;
	}
	void CPdfFontList::Add(Ref& oRef, TFontEntry* pFontEntry)
	{
		// До вызова данной функции надо проверять есть ли элемент с данным ключом
		m_oFontMap.insert(std::pair<Ref, TFontEntry*>(oRef, pFontEntry));
	}
	//--------------------------------------------------------------------------------------
	// RendererOutputDev
	//--------------------------------------------------------------------------------------
	RendererOutputDev::RendererOutputDev(IRenderer* pRenderer, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList)
	{
		m_pFontManager  = pFontManager;
		m_pFontList     = pFontList;

		m_bTiling       = false;

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

		m_pXref = NULL;
		m_pbBreak = NULL;
		m_pSoftMask = NULL;

		m_bDrawOnlyText = false;
	}
	RendererOutputDev::~RendererOutputDev()
	{
		m_pRenderer = NULL;
		RELEASEINTERFACE(m_pSoftMask);
	}
	void RendererOutputDev::startPage(int nPageIndex, GfxState* pGState)
	{
		if (nPageIndex < 0)
			return;

		m_pRenderer->BeginCommand(c_nPageType);

		m_arrMatrix[0] = 1; m_arrMatrix[1] = 0;
		m_arrMatrix[2] = 0; m_arrMatrix[3] = 1;
		m_arrMatrix[4] = 0; m_arrMatrix[5] = 0;

		if (c_nHtmlRendrerer2 == m_lRendererType)
			m_bDrawOnlyText = S_OK == m_pRenderer->CommandLong(c_nCommandLongTypeOnlyText, 0);
		else if (c_nHtmlRendrererText == m_lRendererType)
			m_bDrawOnlyText = true;
		else
			m_bDrawOnlyText = false;
	}
	void RendererOutputDev::endPage()
	{
		m_pRenderer->EndCommand(c_nPageType);
	}
	void RendererOutputDev::saveState(GfxState* pGState)
	{
		m_sStates.push_back(GfxOutputState());
		m_sStates.back().pGState = pGState;
		if (m_pSoftMask)
		{
			m_pSoftMask->AddRef();
			m_sStates.back().pSoftMask = m_pSoftMask;
		}

		// Выходит дольше из-за копирования Clip, Pen, Brush,
		// но не имеет смысла, т.к. Restore всё равно перенакладывает все Clip с нуля
		//if (c_nGrRenderer == m_lRendererType)
		//{
		//	NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer);
		//	GRenderer->Save();
		//}
		//else
		updateAll(pGState);
	}
	void RendererOutputDev::restoreState(GfxState* pGState)
	{
		RELEASEINTERFACE(m_pSoftMask);
		if (m_sStates.empty())
		{ // Несбалансированный q/Q - сломанный файл
			updateAll(pGState);
			UpdateAllClip(pGState);
			return;
		}

		m_pSoftMask = m_sStates.back().pSoftMask;
		if (c_nGrRenderer == m_lRendererType)
		{
			if (NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer))
				GRenderer->SetSoftMask(m_pSoftMask);
		}

		bool bClipChanged = m_sStates.back().pClip || m_sStates.back().pTextClip;
		m_sStates.pop_back();

		updateAll(pGState);
		if (bClipChanged)
			UpdateAllClip(pGState);
	}
	void RendererOutputDev::updateCTM(GfxState* pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32)
	{
	}
	void RendererOutputDev::updateLineDash(GfxState* pGState)
	{
		double* pDash  = NULL;
		int     nSize  = 0;
		double  dStart = 0;
		pGState->getLineDash(&pDash, &nSize, &dStart);
		bool bOffCopy = nSize == 1;
		if (bOffCopy)
		{
			double* pDashTemp = new double[2];
			pDashTemp[0] = pDash[0];
			pDashTemp[1] = pDash[0];
			pDash = pDashTemp;
			nSize = 2;
		}

		if (0 == nSize) // Solid
		{
			m_pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
			m_pRenderer->put_PenDashOffset(0);
		}
		else
		{
			double* dDash = new double[nSize];
			for (int nIndex = 0; nIndex < nSize; ++nIndex)
				dDash[nIndex] = PDFCoordsToMM(pDash[nIndex]);

			m_pRenderer->PenDashPattern(dDash, (long)nSize);
			m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
			m_pRenderer->put_PenDashOffset(PDFCoordsToMM(dStart));
			RELEASEARRAYOBJECTS(dDash);
		}
		if (bOffCopy)
			delete[] pDash;
	}
	void RendererOutputDev::updateFlatness(GfxState* pGState)
	{
	}
	void RendererOutputDev::updateLineJoin(GfxState* pGState)
	{
		int nJoinStyle = pGState->getLineJoin();
		if (1 == nJoinStyle)
			nJoinStyle = 2;
		else if (2 == nJoinStyle)
			nJoinStyle = 1;

		m_pRenderer->put_PenLineJoin(nJoinStyle);
	}
	void RendererOutputDev::updateLineCap(GfxState* pGState)
	{
		int nCapStyle = pGState->getLineCap();
		if (1 == nCapStyle)
			nCapStyle = 2;
		else if (2 == nCapStyle)
			nCapStyle = 1;

		m_pRenderer->put_PenLineStartCap(nCapStyle);
		m_pRenderer->put_PenLineEndCap(nCapStyle);
	}
	void RendererOutputDev::updateMiterLimit(GfxState* pGState)
	{
		m_pRenderer->put_PenMiterLimit(PDFCoordsToMM(pGState->getMiterLimit()));
	}
	void RendererOutputDev::updateLineWidth(GfxState* pGState)
	{
		m_pRenderer->put_PenSize(PDFCoordsToMM(pGState->getLineWidth()));
	}
	void RendererOutputDev::updateStrokeAdjust(GfxState* pGState)
	{
	}
	void RendererOutputDev::updateFillColor(GfxState* pGState)
	{
		GfxColor* pColor = pGState->getFillColor();
		GfxColorSpace* pColorSpace = pGState->getFillColorSpace();

		GfxRGB c;

		pColorSpace->getRGB(pColor, &c, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
		DWORD dwColor = colToByte(c.r) + colToByte(c.g) * 0x100 + colToByte(c.b) * 0x100 * 0x100;
		m_pRenderer->put_BrushColor1(dwColor);
		m_pRenderer->put_BrushColor2(dwColor);
	}
	void RendererOutputDev::updateStrokeColor(GfxState* pGState)
	{
		GfxColor* pColor = pGState->getStrokeColor();
		GfxColorSpace* pColorSpace = pGState->getStrokeColorSpace();

		GfxRGB c;

		pColorSpace->getRGB(pColor, &c, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
		DWORD dwColor = colToByte(c.r) + colToByte(c.g) * 0x100 + colToByte(c.b) * 0x100 * 0x100;

		m_pRenderer->put_PenColor(dwColor);
	}
	void RendererOutputDev::updateBlendMode(GfxState* pGState)
	{
		if (((GlobalParamsAdaptor*)globalParams)->getDrawFormField())
			return;

		switch (pGState->getBlendMode())
		{
		case gfxBlendNormal:
			m_pRenderer->put_BlendMode(3);
			// agg::comp_op_src_over
			break;
		case gfxBlendMultiply:
			m_pRenderer->put_BlendMode(14);
			// agg::comp_op_multiply
			break;
		case gfxBlendScreen:
			m_pRenderer->put_BlendMode(15);
			// agg::comp_op_screen
			break;
		case gfxBlendOverlay:
			m_pRenderer->put_BlendMode(16);
			// agg::comp_op_overlay
			break;
		case gfxBlendDarken:
			m_pRenderer->put_BlendMode(17);
			// agg::comp_op_darken
			break;
		case gfxBlendLighten:
			m_pRenderer->put_BlendMode(18);
			// agg::comp_op_lighten
			break;
		case gfxBlendColorDodge:
			m_pRenderer->put_BlendMode(19);
			// agg::comp_op_color_dodge
			break;
		case gfxBlendColorBurn:
			m_pRenderer->put_BlendMode(20);
			// agg::comp_op_color_burn
			break;
		case gfxBlendHardLight:
			m_pRenderer->put_BlendMode(21);
			// agg::comp_op_hard_light
			break;
		case gfxBlendSoftLight:
			m_pRenderer->put_BlendMode(22);
			// agg::comp_op_soft_light
			break;
		case gfxBlendDifference:
			m_pRenderer->put_BlendMode(23);
			// agg::comp_op_difference
			break;
		case gfxBlendExclusion:
			m_pRenderer->put_BlendMode(24);
			// agg::comp_op_exclusion
			break;
		case gfxBlendHue:
		case gfxBlendSaturation:
		case gfxBlendColor:
		case gfxBlendLuminosity:
		default:
			m_pRenderer->put_BlendMode(3);
			// agg::comp_op_src_over
			break;
		}
	}
	void RendererOutputDev::updateFillOpacity(GfxState* pGState)
	{
		m_pRenderer->put_BrushAlpha1(std::min(255, std::max(0, int(pGState->getFillOpacity() * 255))));
		m_pRenderer->put_BrushAlpha2(std::min(255, std::max(0, int(pGState->getFillOpacity() * 255))));
	}
	void RendererOutputDev::updateStrokeOpacity(GfxState* pGState)
	{
		m_pRenderer->put_PenAlpha(std::min(255, std::max(0, int(pGState->getStrokeOpacity() * 255))));
	}
	void RendererOutputDev::updateAll(GfxState* pGState)
	{
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
	}
	void RendererOutputDev::updateRender(GfxState* pGState)
	{

	}
	NSFonts::CFontInfo* RendererOutputDev::GetFontByParams(XRef* pXref, NSFonts::IFontManager* pFontManager, GfxFont* pFont, std::wstring& wsFontBaseName)
	{
		NSFonts::CFontInfo* pFontInfo = NULL;
		if (!pFontManager)
			return pFontInfo;

		Ref* pRef = pFont->getID();
		Object oRefObject, oFontObject;
		oRefObject.initRef(pRef->num, pRef->gen);
		oRefObject.fetch(pXref, &oFontObject);
		oRefObject.free();

		NSFonts::CFontSelectFormat oFontSelect;
		CheckFontNamePDF(wsFontBaseName, &oFontSelect);
		if (oFontObject.isDict())
		{
			Dict* pFontDict = oFontObject.getDict();
			Object oFontDescriptor, oDescendantFonts;
			pFontDict->lookup("FontDescriptor", &oFontDescriptor);
			if (!oFontDescriptor.isDict() && pFontDict->lookup("DescendantFonts", &oDescendantFonts)->isArray())
			{
				oFontDescriptor.free(); oFontObject.free();
				if (oDescendantFonts.arrayGet(0, &oFontObject)->isDict())
					oFontObject.dictLookup("FontDescriptor", &oFontDescriptor);
			}
			if (oFontDescriptor.isDict())
			{
				Object oDictItem;
				oFontDescriptor.dictLookup("FontName", &oDictItem);
				if (oDictItem.isName())
					oFontSelect.wsName = AStringToPWString(oDictItem.getName());
				else
					oFontSelect.wsName = new std::wstring(wsFontBaseName);
				oDictItem.free();

				oFontDescriptor.dictLookup("FontFamily", &oDictItem);
				if (oDictItem.isString())
				{
					TextString* s = new TextString(oDictItem.getString());
					oFontSelect.wsAltName = new std::wstring(NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength()));
					delete s;
				}
				oDictItem.free();

				oFontDescriptor.dictLookup("FontStretch", &oDictItem);
				oDictItem.free();

				oFontDescriptor.dictLookup("FontWeight", &oDictItem);
				oDictItem.free();

				oFontDescriptor.dictLookup("FontBBox", &oDictItem);
				oDictItem.free();

				oFontDescriptor.dictLookup("Flags", &oDictItem);
				if (oDictItem.isInt() && 0 != oDictItem.getInt())
				{
					int nFlags = oDictItem.getInt();
					if (nFlags & 1) // моноширинный
						oFontSelect.bFixedWidth = new INT(1);
				}
				oDictItem.free();

				oFontDescriptor.dictLookup("ItalicAngle", &oDictItem);
				if (oDictItem.isInt() && 0 != oDictItem.getInt())
				{
					if (oFontSelect.bItalic) RELEASEOBJECT(oFontSelect.bItalic);
					oFontSelect.bItalic = new INT(1);
				}
				oDictItem.free();

				oFontDescriptor.dictLookup("Ascent", &oDictItem);
				if (oDictItem.isInt()) oFontSelect.shAscent = new SHORT(oDictItem.getInt());
				oDictItem.free();

				oFontDescriptor.dictLookup("Leading", &oDictItem);
				if (oDictItem.isInt()) oFontSelect.shLineGap = new SHORT(oDictItem.getInt());
				oDictItem.free();

				oFontDescriptor.dictLookup("CapHeight", &oDictItem);
				if (oDictItem.isInt()) oFontSelect.shCapHeight = new SHORT(oDictItem.getInt());
				oDictItem.free();

				oFontDescriptor.dictLookup("XHeight", &oDictItem);
				if (oDictItem.isInt()) oFontSelect.shXHeight = new SHORT(oDictItem.getInt());
				oDictItem.free();

				oFontDescriptor.dictLookup("StemV", &oDictItem);
				if (oDictItem.isNum())
				{
					double dStemV = oDictItem.getNum();
					if (dStemV > 50.5)
						oFontSelect.usWeight = new USHORT(sqrt(oDictItem.getNum() - 50.5) * 65);
				}
				oDictItem.free();

				oFontDescriptor.dictLookup("StemH", &oDictItem);
				oDictItem.free();

				oFontDescriptor.dictLookup("Descent", &oDictItem);
				if (oDictItem.isInt()) oFontSelect.shDescent = new SHORT(oDictItem.getInt());
				oDictItem.free();

				oFontDescriptor.dictLookup("AvgWidth", &oDictItem);
				if (oDictItem.isInt()) oFontSelect.shAvgCharWidth = new SHORT(oDictItem.getInt());
				oDictItem.free();

				oFontDescriptor.dictLookup("MaxWidth", &oDictItem);
				oDictItem.free();

				oFontDescriptor.dictLookup("MissingWidth", &oDictItem);
				oDictItem.free();
			}
			else
				oFontSelect.wsName = new std::wstring(wsFontBaseName);
			oFontDescriptor.free(); oDescendantFonts.free();
		}
		else
			oFontSelect.wsName = new std::wstring(wsFontBaseName);
		oFontObject.free();

		pFontInfo = pFontManager->GetFontInfoByParams(oFontSelect);
		return pFontInfo;
	}
	void RendererOutputDev::GetFont(XRef* pXref, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, GfxFont* pFont, std::wstring& wsFileName, std::wstring& wsFontName)
	{
		wsFileName = L"";
		wsFontName = L"";
		TFontEntry* pEntry = NULL;
		// MEMERR string dealocation pEntry
		if (!pFontList->Find2((*pFont->getID()), &pEntry))
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
			const BYTE* pData14 = NULL;
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
													   (wchar_t*)((GlobalParamsAdaptor*)globalParams)->GetTempFolder().c_str(), NULL))
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
				oReferenceObject.fetch(pXref, &oStreamObject);
				oReferenceObject.free();
				if (!oStreamObject.isStream())
				{
					// Внедренный шрифт неправильно записан
					oStreamObject.free();

#ifndef FONTS_USE_ONLY_MEMORY_STREAMS
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
						Ref* pRef = pFont->getID();
						Object oRefObject, oFontObject;
						oRefObject.initRef(pRef->num, pRef->gen);
						oRefObject.fetch(pXref, &oFontObject);
						oRefObject.free();

						if (oFontObject.isDict())
						{
							std::string sFontName, sFontFamily;
							int nFontWeight = 0, nItalicAngle = 0, nAscent = 0, nDescent = 0;
							int nCapHeight = 0, nXHeight = 0, nStemV = 0, nStemH = 0, nMissingWidth = 0;
							int arrBBox[4] = { 0, 0, 0, 0 };

							Object oFontDescriptor;
							if (oFontObject.dictLookup("FontDescriptor", &oFontDescriptor)->isDict())
							{
								Object oDictItem;
								oFontDescriptor.dictLookup("FontName", &oDictItem);
								if (oDictItem.isName()) sFontName = oDictItem.getName();
								oDictItem.free();

								oFontDescriptor.dictLookup("FontFamily", &oDictItem);
								if (oDictItem.isName()) sFontFamily = oDictItem.getName();
								oDictItem.free();

								oFontDescriptor.dictLookup("FontWeight", &oDictItem);
								if (oDictItem.isInt()) nFontWeight = oDictItem.getInt();
								oDictItem.free();

								if (oFontDescriptor.dictLookup("FontBBox", &oDictItem)->isArray() && oDictItem.arrayGetLength() == 4)
								{
									for (int nIndex = 0; nIndex < 4; nIndex++)
									{
										Object oArrayItem;
										if (oDictItem.arrayGet(nIndex, &oArrayItem)->isInt())
											arrBBox[nIndex] = oArrayItem.getInt();
										oArrayItem.free();
									}
								}
								oDictItem.free();

								oFontDescriptor.dictLookup("ItalicAngle", &oDictItem);
								if (oDictItem.isInt()) nItalicAngle = oDictItem.getInt();
								oDictItem.free();

								oFontDescriptor.dictLookup("Ascent", &oDictItem);
								if (oDictItem.isInt()) nAscent = oDictItem.getInt();
								oDictItem.free();

								oFontDescriptor.dictLookup("CapHeight", &oDictItem);
								if (oDictItem.isInt()) nCapHeight = oDictItem.getInt();
								oDictItem.free();

								oFontDescriptor.dictLookup("XHeight", &oDictItem);
								if (oDictItem.isInt()) nXHeight = oDictItem.getInt();
								oDictItem.free();

								oFontDescriptor.dictLookup("StemV", &oDictItem);
								if (oDictItem.isInt()) nStemV = oDictItem.getInt();
								oDictItem.free();

								oFontDescriptor.dictLookup("StemH", &oDictItem);
								if (oDictItem.isInt()) nStemH = oDictItem.getInt();
								oDictItem.free();

								oFontDescriptor.dictLookup("Descent", &oDictItem);
								if (oDictItem.isInt()) nDescent = oDictItem.getInt();
								oDictItem.free();

								oFontDescriptor.dictLookup("MissingWidth", &oDictItem);
								if (oDictItem.isInt()) nMissingWidth = oDictItem.getInt();
								oDictItem.free();

							}
							oFontDescriptor.free();

							fprintf(pFile, "StartFontMetrics 3.0\n");
							if (!sFontName.empty()) fprintf(pFile, "FontName %s\n", sFontName.c_str());
							if (!sFontFamily.empty()) fprintf(pFile, "FamilyName %s\n", sFontFamily.c_str());
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
							if (oFontObject.dictLookup("FirstChar", &oDictItem)->isInt()) nFirstChar = oDictItem.getInt();
							oDictItem.free();

							Gfx8BitFont* pT1Font = (Gfx8BitFont*)pFont;
							if (oFontObject.dictLookup("Widths", &oDictItem)->isArray())
							{
								int nWidthsCount = oDictItem.arrayGetLength();
								fprintf(pFile, "StartCharMetrics %d\n", nWidthsCount);
								for (int nIndex = 0; nIndex < nWidthsCount; nIndex++)
								{
									int nWidth = nMissingWidth;
									Object oArrayItem;
									if (oDictItem.arrayGet(nIndex, &oArrayItem)->isInt()) nWidth = oArrayItem.getInt();
									oArrayItem.free();

									char** ppEncoding = pT1Font->getEncoding();

									if (ppEncoding && ppEncoding[nIndex])
										fprintf(pFile, "C %d ; WX %d ; N %s ;\n", nIndex + nFirstChar, nWidth, ppEncoding[nIndex]);
									else
										fprintf(pFile, "C %d ; WX %d ;\n", nIndex + nFirstChar, nWidth);
								}
								fprintf(pFile, "EndCharMetrics\n");
							}
							oDictItem.free();
						}
						oFontObject.free();
					}
					fclose(pFile);
				}
#endif

				// Загрузим сам файл со шрифтом, чтобы точно определить его тип
				if (!pFontManager->LoadFontFromFile(wsFileName, 0, 10, 72, 72))
				{
					pEntry->bAvailable = true;
					return;
				}

				std::wstring wsFontType = pFontManager->GetFontType();
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
				const BYTE* pData14 = NULL;
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
			else if (!pFont->locateFont(pXref, false) ||
					 (wsFileName = NSStrings::GetStringFromUTF32(pFont->locateFont(pXref, false)->path)).length() == 0)
			{
				NSFonts::CFontInfo* pFontInfo = GetFontByParams(pXref, pFontManager, pFont, wsFontBaseName);

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
							pFontList->Remove(*pFont->getID());
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
			}
			// Здесь мы грузим кодировки
			int* pCodeToGID = NULL, *pCodeToUnicode = NULL;
			int nLen = 0;
			FoFiTrueType* pTTFontFile  = NULL;
			FoFiType1C*   pT1CFontFile = NULL;
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
						pCodeToGID = ((Gfx8BitFont*)pFont)->getCodeToGIDMap(pTTFontFile);
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
				else if (L"" != wsFileName && (pFont8bit = dynamic_cast<Gfx8BitFont*>(pFont)))
				{
					char** ppEncoding = pFont8bit->getEncoding();
					if (!ppEncoding)
						break;

					if (!pFontManager)
						break;

					pFontManager->LoadFontFromFile(wsFileName, 0, 1, 72, 72);
					pCodeToGID = (int*)MemUtilsMallocArray(256, sizeof(int));
					if (!pCodeToGID)
						break;

					nLen = 256;
					for (int nIndex = 0; nIndex < 256; ++nIndex)
					{
						pCodeToGID[nIndex] = 0;
						char* sName = NULL;
						if ((sName = ppEncoding[nIndex]))
						{
							unsigned short ushGID = pFontManager->GetNameIndex(AStringToWString(sName));
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
						char** ppEncoding = pFont8bit->getEncoding();
						if (!ppEncoding)
							break;

						if (!pFontManager)
							break;

						pFontManager->LoadFontFromFile(wsFileName, 0, 1, 72, 72);
						pCodeToGID = (int*)MemUtilsMallocArray(256, sizeof(int));
						if (!pCodeToGID)
							break;

						nLen = 256;
						for (int nIndex = 0; nIndex < 256; ++nIndex)
						{
							pCodeToGID[nIndex] = 0;
							char* sName = NULL;
							if ((sName = ppEncoding[nIndex]))
							{
								unsigned short ushGID = pFontManager->GetNameIndex(AStringToWString(sName));
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
					pCodeToGID = ((Gfx8BitFont*)pFont)->getCodeToGIDMap(pTTFontFile);
					nLen = 256;

					delete pTTFontFile;
					pTTFontFile = NULL;
				}
				else
				{
					pCodeToGID = NULL;
					nLen       = 0;

					if (pFontManager->LoadFontFromFile(wsFileName, 0, 10, 72, 72))
					{
						INT* pCodes = NULL;
						nLen = 256;
						pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
						for (int nCode = 0; nCode < nLen; ++nCode)
						{
							pCodeToGID[nCode] = pFontManager->GetGIDByUnicode(nCode);
						}
					}
				}
				break;
			}
			case fontCIDType0:
			case fontCIDType0C:
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
				/*
#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
				pT1CFontFile = FoFiType1C::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize);
#else
				pT1CFontFile = FoFiType1C::load((char*)U_TO_UTF8(wsFileName).c_str());
#endif
				if (pT1CFontFile)
				{
					pCodeToGID = pT1CFontFile->getCIDToGIDMap(&nLen);

					delete pT1CFontFile;
					pT1CFontFile = NULL;
				}
				else
				{
					pCodeToGID = NULL;
					nLen = 0;
				}
				*/
				pCodeToGID = NULL;
				nLen = 0;
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
					CharCodeToUnicode* pCodeToUnicode = NULL;
					if ((pCodeToUnicode = ((GfxCIDFont*)pFont)->getToUnicode()))
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
								pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
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
				else if (((GfxCIDFont*)pFont)->getCIDToGID())
				{
					nLen = ((GfxCIDFont*)pFont)->getCIDToGIDLen();
					pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
					if (!pCodeToGID)
						break;

					memcpy(pCodeToGID, ((GfxCIDFont*)pFont)->getCIDToGID(), nLen * sizeof(int));
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
				GfxCIDFont* pCIDFont = (GfxCIDFont*)pFont;
				CharCodeToUnicode* pToUnicode = pCIDFont->getToUnicode();
				if (NULL != pToUnicode)
				{
					nToUnicodeLen = pToUnicode->getLength();
					pCodeToUnicode = (int*)MemUtilsMallocArray(nToUnicodeLen, sizeof(int));

					if (pCodeToUnicode)
					{
						for (int nIndex = 0; nIndex < nToUnicodeLen; ++nIndex)
						{
							Unicode aUnicode[2];
							if (pToUnicode->mapToUnicode(nIndex, aUnicode, 2))
								pCodeToUnicode[nIndex] = aUnicode[0];
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

				CharCodeToUnicode* pToUnicode = ((Gfx8BitFont*)pFont)->getToUnicode();
				if (NULL != pToUnicode)
				{
					nToUnicodeLen = pToUnicode->getLength();
					pCodeToUnicode = (int*)MemUtilsMallocArray(nToUnicodeLen, sizeof(int));//literally here

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
	}
	void RendererOutputDev::updateFont(GfxState* pGState)
	{
		// Проверяем наличие списка со шрифтами
		if (!m_pFontList)
			return;

		GfxFont* pFont = pGState->getFont();
		if (!pFont)
			return;

		m_pRenderer->put_FontSize(pGState->getFontSize());

		std::wstring wsFileName = L"";
		std::wstring wsFontName = L"";
		GetFont(m_pXref, m_pFontManager, m_pFontList, pFont, wsFileName, wsFontName);

		if (!wsFileName.empty())
		{
			m_pRenderer->put_FontPath(wsFileName);
			m_pRenderer->put_FontName(wsFontName);
		}
	}
	void RendererOutputDev::stroke(GfxState* pGState)
	{
		if (m_bDrawOnlyText)
			return;

		DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
		m_pRenderer->DrawPath(c_nStroke);

		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::fill(GfxState* pGState)
	{
		if (m_bDrawOnlyText)
			return;

		DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
		m_pRenderer->DrawPath(c_nWindingFillMode);
		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::eoFill(GfxState* pGState)
	{
		if (m_bDrawOnlyText)
			return;

		DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
		m_pRenderer->DrawPath(c_nEvenOddFillMode);

		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::FillStroke(GfxState* pGState)
	{
		if (m_bDrawOnlyText)
			return;

		DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
		m_pRenderer->DrawPath(c_nStroke | c_nWindingFillMode);

		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::EoFillStroke(GfxState* pGState)
	{
		if (m_bDrawOnlyText)
			return;

		DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());
		m_pRenderer->DrawPath(c_nStroke | c_nEvenOddFillMode);

		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::tilingPatternFill(GfxState* pGState, Gfx* gfx, Object* pStream, int nPaintType, int nTilingType, Dict* pResourcesDict, double* pMatrix, double* pBBox,
											  int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep)
	{
		if (m_bDrawOnlyText)
			return;

		if (nX1 - nX0 == 1 && nY1 - nY0 == 1) // Одно изображение, tilingPattern не требуется
		{
			gfx->drawForm(pStream, pResourcesDict, pMatrix, pBBox);
			return;
		}

		if (fabs(pBBox[2] - pBBox[0] - dXStep) > 0.001 || fabs(pBBox[3] - pBBox[1] - dYStep) > 0.001)
			return;

		double dWidth, dHeight, dDpiX, dDpiY;
		m_pRenderer->get_Width(&dWidth);
		m_pRenderer->get_Height(&dHeight);
		m_pRenderer->get_DpiX(&dDpiX);
		m_pRenderer->get_DpiY(&dDpiY);
		dWidth  = dWidth  * dDpiX / 25.4;
		dHeight = dHeight * dDpiY / 25.4;

		dWidth  *= (dXStep / pGState->getPageWidth());
		dHeight *= (dYStep / pGState->getPageHeight());

		int nWidth  = round(dWidth);
		int nHeight = round(dHeight);

		BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
		memset(pBgraData, 0, nWidth * nHeight * 4);

		CBgraFrame* pFrame = new CBgraFrame();
		pFrame->put_Data(pBgraData);
		pFrame->put_Width(nWidth);
		pFrame->put_Height(nHeight);
		pFrame->put_Stride(-4 * nWidth);

		NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
		pRenderer->SetFontManager(m_pFontManager);
		pRenderer->CreateFromBgraFrame(pFrame);
		pRenderer->put_Width (dWidth  * 25.4 / 72.0);
		pRenderer->put_Height(dHeight * 25.4 / 72.0);
		pRenderer->CommandLong(c_nPenWidth0As1px, 1);
		pRenderer->SetSwapRGB(false);

		PDFRectangle box;
		box.x1 = pBBox[0];
		box.y1 = pBBox[1];
		box.x2 = pBBox[2];
		box.y2 = pBBox[3];

		RendererOutputDev* m_pRendererOut = new RendererOutputDev(pRenderer, m_pFontManager, m_pFontList);
		m_pRendererOut->NewPDF(gfx->getDoc()->getXRef());

		Gfx* m_gfx = new Gfx(gfx->getDoc(), m_pRendererOut, -1, pResourcesDict, dDpiX, dDpiY, &box, NULL, 0);
		m_gfx->display(pStream);

		pFrame->ClearNoAttack();
		RELEASEOBJECT(m_gfx);
		RELEASEOBJECT(pRenderer);
		RELEASEOBJECT(m_pRendererOut);
		RELEASEOBJECT(pFrame);

		Aggplus::CImage* oImage = new Aggplus::CImage();
		oImage->Create(pBgraData, nWidth, nHeight, 4 * nWidth);

		double xMin, yMin, xMax, yMax;
		xMin = nX0 * dXStep + pBBox[0];
		yMin = nY0 * dYStep + pBBox[1];
		xMax = nX1 * dXStep + pBBox[0];
		yMax = nY1 * dYStep + pBBox[1];
		Transform(pMatrix, xMin, yMin, &xMin, &yMin);
		Transform(pMatrix, xMax, yMax, &xMax, &yMax);
		pGState->moveTo(xMin, yMin);
		pGState->lineTo(xMax, yMin);
		pGState->lineTo(xMax, yMax);
		pGState->lineTo(xMin, yMax);
		pGState->closePath();

		DoPath(pGState, pGState->getPath(), pGState->getPageHeight(), pGState->getCTM());

		long brush;
		m_pRenderer->get_BrushType(&brush);

		int alpha = pGState->getFillOpacity() * 255;
		m_pRenderer->put_BrushType(c_BrushTypeTexture);
		m_pRenderer->put_BrushTextureImage(oImage);
		m_pRenderer->put_BrushTextureMode(c_BrushTextureModeTile);
		m_pRenderer->put_BrushTextureAlpha(alpha);
		m_pRenderer->BeginCommand(c_nImageType);

		m_pRenderer->DrawPath(c_nWindingFillMode);

		m_pRenderer->PathCommandEnd();
		m_pRenderer->EndCommand(c_nImageType);
		m_pRenderer->put_BrushType(brush);
		m_pRenderer->put_BrushTextureImage(NULL);

		pGState->clearPath();
		RELEASEINTERFACE(oImage);
	}
	void RendererOutputDev::StartTilingFill(GfxState* pGState)
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

	GBool RendererOutputDev::shadedFill(GfxState* pGState, GfxShading* pShading)
	{
		double x0, y0, x1, x2, y1, y2, r1, r2;
		double xmin, xmax, ymin, ymax, r;
		double* matrix;

		int nTriangles = 0, nPatches = 0;
		switch (pShading->getType())
		{
		case 1:
			((GfxFunctionShading*)pShading)->getDomain(&x0, &y0, &x1, &y1);
			matrix = ((GfxFunctionShading*)pShading)->getMatrix();
			pGState->moveTo(x0 * matrix[0] + y0 * matrix[2] + matrix[4],
					x0 * matrix[1] + y0 * matrix[3] + matrix[5]);
			pGState->lineTo(x1 * matrix[0] + y0 * matrix[2] + matrix[4],
					x1 * matrix[1] + y0 * matrix[3] + matrix[5]);
			pGState->lineTo(x1 * matrix[0] + y1 * matrix[2] + matrix[4],
					x1 * matrix[1] + y1 * matrix[3] + matrix[5]);
			pGState->lineTo(x0 * matrix[0] + y1 * matrix[2] + matrix[4],
					x0 * matrix[1] + y1 * matrix[3] + matrix[5]);
			pGState->closePath();
			FunctionShadedFill(pGState, (GfxFunctionShading*) pShading);
			return true;
		case 2:
			pGState->getUserClipBBox(&xmin, &ymin, &xmax, &ymax);

			pGState->moveTo(xmin, ymin);
			pGState->lineTo(xmin, ymax);
			pGState->lineTo(xmax, ymax);
			pGState->lineTo(xmax, ymin);
			pGState->closePath();
			AxialShadedFill(pGState, (GfxAxialShading* )pShading);
			return true;
		case 3:
			((GfxRadialShading*)pShading)->getCoords(&x1, &y1, &r1, &x2, &y2, &r2);

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
			nTriangles = ((GfxGouraudTriangleShading*) pShading)->getNTriangles();

			for (int i = 0; i < nTriangles; i++) {
				int nComps = ((GfxGouraudTriangleShading*) pShading)->getNComps();
				double x1,x2,x3,y1,y2,y3;
				double* c1 =  new double[nComps];
				double* c2 =  new double[nComps];
				double* c3 =  new double[nComps];

				GfxColor col1, col2, col3;

				((GfxGouraudTriangleShading*) pShading)->getTriangle(i, &x1, &y1, c1, &x2, &y2, c2, &x3, &y3, c3);
				((GfxGouraudTriangleShading*) pShading)->getColor(c1, &col1);
				((GfxGouraudTriangleShading*) pShading)->getColor(c2, &col2);
				((GfxGouraudTriangleShading*) pShading)->getColor(c3, &col3);

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
			// int nComps = ((GfxPatchMeshShading*)pShading)->getNComps();
			int nPatches = ((GfxPatchMeshShading*)pShading)->getNPatches();

			NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer);
			if (GRenderer)
				GRenderer->SetSoftMask(NULL);

			m_pRenderer->BeginCommand(c_nLayerType);

			for (int i = 0; i < nPatches; i++) {
				GfxPatch* patch = ((GfxPatchMeshShading*)pShading)->getPatch(i);
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
				PatchMeshFill(pGState, patch, (GfxPatchMeshShading*)pShading);
			}

			if (GRenderer)
				GRenderer->SetSoftMask(m_pSoftMask);
			m_pRenderer->EndCommand(c_nLayerType);

			return true;

		}
		return false;
	}

	bool RendererOutputDev::FunctionShadedFill(GfxState* pGState, GfxFunctionShading* pShading)
	{
		if (m_bDrawOnlyText)
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

		float cur_y = 0;
		float delta_x = (x2 - x1) / info.shading.function.get_resolution();
		float delta_y = (y2 - y1) / info.shading.function.get_resolution();

		GfxColorSpace* ColorSpace = pShading->getColorSpace();

		for (size_t i = 0; i < info.shading.function.get_resolution(); i++)
		{
			float cur_x = 0;
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

		m_pRenderer->put_BrushGradInfo(&info);
		m_pRenderer->DrawPath(c_nWindingFillMode);

		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->put_BrushType(brush);
		pGState->clearPath();
		return true;
	}
	bool RendererOutputDev::AxialShadedFill(GfxState* pGState, GfxAxialShading* pShading)
	{
		if (m_bDrawOnlyText)
			return true;

		double x1, x2, y1, y2;
		double t0, t1;

		long brush;
		m_pRenderer->get_BrushType(&brush);

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

		GfxColorSpace* ColorSpace = pShading->getColorSpace();
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

		m_pRenderer->put_BrushGradInfo(&info);
		m_pRenderer->DrawPath(c_nWindingFillMode);

		m_pRenderer->EndCommand(c_nPathType);

		m_pRenderer->put_BrushType(brush);

		pGState->clearPath();
		return true;
	}
	bool RendererOutputDev::RadialShadedFill(GfxState* pGState, GfxRadialShading* pShading)
	{
		if (m_bDrawOnlyText)
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

		GfxColorSpace* ColorSpace = pShading->getColorSpace();;
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

		m_pRenderer->put_BrushGradInfo(&info);
		m_pRenderer->DrawPath(c_nWindingFillMode);

		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->put_BrushType(brush);
		pGState->clearPath();
		return true;
	}
	bool RendererOutputDev::GouraundTriangleFill(GfxState* pGState, const std::vector<GfxColor*> &colors, const std::vector<NSStructures::Point> &points)
	{
		if (m_bDrawOnlyText)
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

		m_pRenderer->put_BrushGradInfo(&info);
		m_pRenderer->DrawPath(c_nWindingFillMode);

		pGState->clearPath();
		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->put_BrushType(brush);
		return true;
	}
	bool RendererOutputDev::PatchMeshFill(GfxState* pGState, GfxPatch* patch, GfxPatchMeshShading* pShading)
	{
		if (m_bDrawOnlyText)
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
		GfxColorSpace* ColorSpace = pShading->getColorSpace();
		for (int i = 0; i < 2; i ++)
		{
			for (int j = 0; j < 2; j++)
			{
				GfxColor c;
				pShading->getColor(patch->color[i][j], &c);
				GfxRGB draw_color;
				// RenderingIntent in this case does nothing but it's an obligatory arguments
				ColorSpace->getRGB(&c, &draw_color, gfxRenderingIntentAbsoluteColorimetric);
				colors[j][i] = {colToByte(draw_color.b), colToByte(draw_color.g), colToByte(draw_color.r), (unsigned)alpha};
			}
		}
		NSStructures::GradientInfo info = NSStructures::GInfoConstructor::get_tensor_curve(points,
																						   {},
																						   colors,
																						   false
																						   );

		m_pRenderer->put_BrushGradInfo(&info);
		m_pRenderer->DrawPath(c_nWindingFillMode);

		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->put_BrushType(brush);
		pGState->clearPath();
		return true;
	}
	void RendererOutputDev::StartShadedFill()
	{
		if (!m_bDrawOnlyText)
			m_pRenderer->BeginCommand(c_nComplexFigureType);
	}
	void RendererOutputDev::EndShadedFill()
	{
		if (!m_bDrawOnlyText)
			m_pRenderer->EndCommand(c_nComplexFigureType);
	}
	void RendererOutputDev::StartTilingFillIteration()
	{
		if (!m_bDrawOnlyText)
			m_pRenderer->BeginCommand(c_nPDFTilingFillIteration);
	}
	void RendererOutputDev::EndTilingFillIteration()
	{
		if (!m_bDrawOnlyText)
			m_pRenderer->EndCommand(c_nPDFTilingFillIteration);
	}
	void RendererOutputDev::StartSimpleTilingFill(GfxState* pGState, int  nX0, int nY0, int nX1, int nY1, double dStepX, double dStepY, double dXMin, double dYMin, double dXMax, double dYMax, double* pMatrix)
	{
		if (m_bDrawOnlyText)
			return;

		UpdateAllClip(pGState);

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
		if (!m_bDrawOnlyText)
			m_pRenderer->EndCommand(c_nPDFTilingFill);
	}
	void RendererOutputDev::clip(GfxState* pGState)
	{
		if (m_bDrawOnlyText || m_sStates.empty())
			return;

		if (!m_sStates.back().pClip)
			m_sStates.back().pClip = new GfxClip();
		int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeWinding;
		m_sStates.back().pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);
		AddClip(pGState, &m_sStates.back(), m_sStates.back().pClip->GetPathNum() - 1);
	}
	void RendererOutputDev::eoClip(GfxState* pGState)
	{
		if (m_bDrawOnlyText || m_sStates.empty())
			return;

		if (!m_sStates.back().pClip)
			m_sStates.back().pClip = new GfxClip();
		int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeEvenOdd;
		m_sStates.back().pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);
		AddClip(pGState, &m_sStates.back(), m_sStates.back().pClip->GetPathNum() - 1);
	}
	void RendererOutputDev::clipToStrokePath(GfxState* pGState)
	{
		if (m_bDrawOnlyText || m_sStates.empty())
			return;

		if (!m_sStates.back().pClip)
			m_sStates.back().pClip = new GfxClip();
		int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeWinding | c_nClipToStrokePath;
		m_sStates.back().pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);
		AddClip(pGState, &m_sStates.back(), m_sStates.back().pClip->GetPathNum() - 1);
	}
	void RendererOutputDev::clipToPath(GfxState* pGState, GfxPath* pPath, double* pMatrix, bool bEO)
	{
		if (m_bDrawOnlyText)
			return;

		int nClipFlag = bEO ? c_nClipRegionTypeEvenOdd : c_nClipRegionTypeWinding;
		nClipFlag |= c_nClipRegionIntersect;

		m_pRenderer->BeginCommand(c_nClipType);
		m_pRenderer->put_ClipMode(nClipFlag);
		DoPath(pGState, pPath, pGState->getPageHeight(), pMatrix);
		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->EndCommand(c_nClipType);
	}
	void RendererOutputDev::ClipToText(const std::wstring& wsFontName, const std::wstring& wsFontPath, double dFontSize, int nFontStyle, double* pMatrix, const std::wstring& wsText, double dX, double dY, double dWidth, double dHeight, double dBaseLineOffset)
	{
		if (m_bDrawOnlyText)
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
	void RendererOutputDev::endTextObject(GfxState* pGState)
	{
		if (!m_sStates.empty() && m_sStates.back().pTextClip && 4 <= pGState->getRender())
		{
			AddTextClip(pGState, &m_sStates.back());
			updateFont(pGState);
		}
	}
	void RendererOutputDev::beginStringOp(GfxState* pGState)
	{
		m_pRenderer->BeginCommand(c_nTextType);

		int nRenderMode = pGState->getRender();

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
	void RendererOutputDev::endStringOp(GfxState* pGState)
	{

		int nRenderMode = pGState->getRender();

		// Добавляем в Clipping Path текст
		if (nRenderMode >= 4)
		{
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
	void RendererOutputDev::drawString(GfxState* pGState, GString* seString)
	{
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

		unsigned int unGidsCount = seString->getLength();
		unsigned int* pGids = new unsigned int[unGidsCount];
		if (!pGids)
			return;

		std::wstring  wsUnicodeText;
		for (int nIndex = 0; nIndex < seString->getLength(); nIndex++)
		{
			int nChar = seString->getChar(nIndex);

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
	void RendererOutputDev::drawChar(GfxState* pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode* pUnicode, int nUnicodeLen)
	{
		// Проверяем наличие списка со шрифтами
		if (NULL == m_pFontList)
			return;

		// Проверяем наличие текущего шрифта
		TFontEntry oEntry;
		if (!m_pFontList->GetFont(pGState->getFont()->getID(), &oEntry))
			return;

		int nRenderMode = pGState->getRender();
		if (3 == nRenderMode && !m_bDrawOnlyText) // Невидимый текст
		{
			return;
		}

		double startX, startY, endX, endY;
		Transform(pGState->getCTM(), dX, dY, &startX, &startY);
		Transform(pGState->getCTM(), dX + dDx, dY + dDy, &endX, &endY);
		double dCenterX = (startX + endX) / 2;
		double dCenterY = (startY + endY) / 2;
		if (((GlobalParamsAdaptor*)globalParams)->InRedact(dCenterX, dCenterY))
			return;

		double* pCTM   = pGState->getCTM();
		double* pTm    = pGState->getTextMat();
		GfxFont* pFont = pGState->getFont();

		double pNewTm[6], arrMatrix[6];

		double dTextScale = std::min(sqrt(pTm[2] * pTm[2] + pTm[3] * pTm[3]), sqrt(pTm[0] * pTm[0] + pTm[1] * pTm[1]));
		double dITextScale = 1 / dTextScale;
		double dOldSize = 10.0, dOldWidth = 1.0;
		m_pRenderer->get_FontSize(&dOldSize);
		m_pRenderer->get_PenSize(&dOldWidth);
		if (dOldSize * dTextScale > 0)
		{
			m_pRenderer->put_FontSize(dOldSize * dTextScale);

			pNewTm[0] =  pTm[0] * dITextScale * pGState->getHorizScaling();
			pNewTm[1] =  pTm[1] * dITextScale * pGState->getHorizScaling();
			pNewTm[2] = -pTm[2] * dITextScale;
			pNewTm[3] = -pTm[3] * dITextScale;
			pNewTm[4] =  dX - dOriginX;
			pNewTm[5] =  dY - dOriginY;
		}
		else
		{
			m_pRenderer->put_FontSize(-dOldSize * dTextScale);

			pNewTm[0] = -pTm[0] * dITextScale * pGState->getHorizScaling();
			pNewTm[1] = -pTm[1] * dITextScale * pGState->getHorizScaling();
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

		double dSize = 1;
		if (true)
		{
			double dNorma = std::min(sqrt(arrMatrix[0] * arrMatrix[0] + arrMatrix[1] * arrMatrix[1]), sqrt(arrMatrix[2] * arrMatrix[2] + arrMatrix[3] * arrMatrix[3]));
			if (dNorma > 0.001)
			{
				arrMatrix[0] /= dNorma;
				arrMatrix[1] /= dNorma;
				arrMatrix[2] /= dNorma;
				arrMatrix[3] /= dNorma;

				m_pRenderer->get_FontSize(&dSize);
				dSize *= dNorma;
				m_pRenderer->put_FontSize(dSize);
				if (nRenderMode == 1 || nRenderMode == 2 || nRenderMode == 5 || nRenderMode == 6)
					m_pRenderer->put_PenSize(PDFCoordsToMM(pGState->getLineWidth() * dNorma));
			}
		}

		double dShiftX = 0, dShiftY = 0;
		DoTransform(arrMatrix, &dShiftX, &dShiftY, true);

		std::wstring wsUnicodeText;

		bool isCIDFont = pFont->isCIDFont();

		if (NULL != oEntry.pCodeToUnicode && nCode < oEntry.unLenUnicode)
		{
			int unUnicode = oEntry.pCodeToUnicode[nCode];
			wsUnicodeText = NSStringExt::CConverter::GetUnicodeFromUTF32((const unsigned int*)(&unUnicode), 1);
		}
		else
		{
			if (isCIDFont)
			{
				// Значит кодировка была Identity-H или Identity-V, что означает, что исходные коды и есть юникодные значения
				wsUnicodeText = (wchar_t(nCode));
			}
			else
			{
				// Договорились, что если нельзя точно составить юникодные значения, тогда отдаем NULL
				if (pFont->getType() == fontType3)
					wsUnicodeText = NSStringExt::CConverter::GetUnicodeFromUTF32(pUnicode, nUnicodeLen);
				else
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
			if ((isCIDFont && (((GfxCIDFont*)pFont)->usesIdentityEncoding() || ((GfxCIDFont*)pFont)->usesIdentityCIDToGID() || ((GfxCIDFont*)pFont)->ctuUsesCharCodeToUnicode() || pFont->getType() == fontCIDType0C))
					|| (!isCIDFont && wsUnicodeText.empty()))
			{
				int nCurCode = (0 == nCode ? 65534 : nCode);
				unGid       = (unsigned int)nCurCode;
				unGidsCount = 1;
			}
		}

		if (nRenderMode == 0 || nRenderMode == 4 || nRenderMode == 6 || m_bDrawOnlyText)
		{
			bool bReplace = false;
			std::wstring sFontPath;
#ifdef BUILDING_WASM_MODULE
			m_pRenderer->get_FontPath(&sFontPath);
			if (!unGid && !wsUnicodeText.empty() && !sFontPath.empty())
			{
				unsigned int lUnicode = (unsigned int)wsUnicodeText[0];
				long lStyle;
				double dDpiX, dDpiY;
				m_pRenderer->get_FontStyle(&lStyle);
				m_pRenderer->get_DpiX(&dDpiX);
				m_pRenderer->get_DpiY(&dDpiY);
				m_pFontManager->SetStringGID(FALSE);
				m_pFontManager->LoadFontFromFile(sFontPath, 0, dOldSize, dDpiX, dDpiY);

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
							m_pFontManager->LoadFontFromFile(wsFileName, 0, dSize, dDpiX, dDpiY);
							bReplace = true;
						}
					}
				}
			}
#endif
			m_pRenderer->CommandDrawTextEx(wsUnicodeText, &unGid, unGidsCount, PDFCoordsToMM(dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));
			if (bReplace)
				m_pRenderer->put_FontPath(sFontPath);
		}

		LONG lRendererType = 0;
		m_pRenderer->get_Type(&lRendererType);

		bool bIsEmulateBold = false;
		if (c_nDocxWriter == lRendererType && 2 == nRenderMode)
			bIsEmulateBold = (S_OK == m_pRenderer->CommandLong(c_nSupportPathTextAsText, 0)) ? true : false;

		if (bIsEmulateBold)
		{
			m_pRenderer->BeginCommand(c_nStrokeTextType);

			LONG lOldStyle = 0;
			m_pRenderer->get_FontStyle(&lOldStyle);
			LONG lNewStyle = lOldStyle;

			if ((lNewStyle & 0x01) == 0)
			{
				lNewStyle |= 0x01;
				m_pRenderer->put_FontStyle(lNewStyle);
			}

			if (unGid)
				m_pRenderer->CommandDrawTextEx(wsUnicodeText, &unGid, unGidsCount, PDFCoordsToMM(dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));
			else
				m_pRenderer->CommandDrawText(wsUnicodeText, PDFCoordsToMM(dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));

			if (lOldStyle != lNewStyle)
				m_pRenderer->put_FontStyle(lOldStyle);

			m_pRenderer->EndCommand(c_nStrokeTextType);
		}
		else if (nRenderMode == 1 || nRenderMode == 2 || nRenderMode == 5 || nRenderMode == 6)
		{
			m_pRenderer->BeginCommand(c_nStrokeTextType);

			m_pRenderer->PathCommandEnd();
			if (unGid)
				m_pRenderer->PathCommandTextEx(wsUnicodeText, &unGid, unGidsCount, PDFCoordsToMM(dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));
			else
				m_pRenderer->PathCommandText(wsUnicodeText, PDFCoordsToMM(dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));

			long lDrawPath = c_nStroke;
			if (nRenderMode == 2)
				lDrawPath |= c_nWindingFillMode;
			m_pRenderer->DrawPath(lDrawPath);

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
			// tmpchange
			if (!m_sStates.empty())
			{
				if (!m_sStates.back().pTextClip)
					m_sStates.back().pTextClip = new GfxTextClip();
				m_sStates.back().pTextClip->ClipToText(wsTempFontName, wsTempFontPath, dTempFontSize, (int)lTempFontStyle, arrMatrix, wsClipText, dShiftX, /*-fabs(pFont->getFontBBox()[3]) * dTfs + */ dShiftY, 0, 0, 0);

			}
		}

		m_pRenderer->put_FontSize(dOldSize);
		m_pRenderer->put_PenSize(dOldWidth);
	}

	GBool RendererOutputDev::beginType3Char(GfxState* state, double x, double y, double dx, double dy, CharCode code, Unicode* u, int uLen)
	{
		if (!m_bDrawOnlyText)
			return false;

		//drawChar(state, x, y, dx, dy, 0, 0, code, 1, u, uLen);

		return false;
	}
	void RendererOutputDev::endType3Char(GfxState* pGState)
	{
		return;
	}
	GBool RendererOutputDev::beginMarkedContent(GfxState* state, GString* s)
	{
		return gFalse;
	}
	GBool RendererOutputDev::beginMCOShapes(GfxState* state, GString* s, Object* ref)
	{
		IAdvancedCommand::AdvancedCommandType eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::ShapeStart;
		if (m_pRenderer->IsSupportAdvancedCommand(eAdvancedCommandType) == S_OK)
		{
			CShapeStart* pCommand = new CShapeStart();
			pCommand->SetShapeXML(s->getCString());

			Object oIm;
			if (ref && ref->isRef() && ref->fetch(m_pXref, &oIm)->isStream())
			{
				Dict* oImDict = oIm.streamGetDict();

				int nLength = 0;
				Object oLength;
				if (oImDict->lookup("Length", &oLength)->isInt())
					nLength = oLength.getInt();
				oLength.free();
				if (oImDict->lookup("DL", &oLength)->isInt())
					nLength = oLength.getInt();
				oLength.free();

				Stream* pImage = oIm.getStream()->getUndecodedStream();
				pImage->reset();

				BYTE* pBuffer = new BYTE[nLength];
				BYTE* pBufferPtr = pBuffer;
				for (int nI = 0; nI < nLength; ++nI)
					*pBufferPtr++ = (BYTE)pImage->getChar();

				CBgraFrame oFrame;
				if (oFrame.Decode(pBuffer, nLength))
				{
					pCommand->SetShapeImage(oFrame.get_Data(), oFrame.get_Width(), oFrame.get_Height());
					oFrame.ClearNoAttack();
				}
			}
			oIm.free();
			bool bRes = m_pRenderer->AdvancedCommand(pCommand) == S_OK;
			RELEASEOBJECT(pCommand);
			if (bRes)
				return gTrue;
		}
		return gFalse;
	}
	void RendererOutputDev::endMarkedContent(GfxState* state)
	{
		IAdvancedCommand::AdvancedCommandType eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::ShapeEnd;
		if (m_pRenderer->IsSupportAdvancedCommand(eAdvancedCommandType) == S_OK)
		{
			CEmptyComand* pCommand = new CEmptyComand(IAdvancedCommand::AdvancedCommandType::ShapeEnd);
			m_pRenderer->AdvancedCommand(pCommand);
			RELEASEOBJECT(pCommand);
		}
	}
	void RendererOutputDev::drawImageMask(GfxState* pGState, Object* pRef, Stream* pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate)
	{
		if (m_bDrawOnlyText || pGState->getFillColorSpace()->isNonMarking())
			return;

		int nBufferSize = 4 * nWidth * nHeight;
		if (nBufferSize < 1)
			return;

		BYTE* pBufferPtr = new(std::nothrow) BYTE[nBufferSize];
		if (!pBufferPtr)
			return;

		Aggplus::CImage oImage;
		oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);

		// Пишем данные в pBufferPtr
		ImageStream* pImageStream = new ImageStream(pStream, nWidth, 1, 1);
		pImageStream->reset();

		GfxColorSpace* pColorSpace = pGState->getFillColorSpace();
		GfxRGB oRGB;
		pColorSpace->getRGB(pGState->getFillColor(), &oRGB, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);

		BYTE r = colToByte(oRGB.r);
		BYTE g = colToByte(oRGB.g);
		BYTE b = colToByte(oRGB.b);
		BYTE unAlpha = std::min(255, std::max(0, int(pGState->getFillOpacity() * 255)));
		int nInvert = bInvert ? 1 : 0;

		for (int nY = nHeight - 1; nY >= 0; --nY)
		{
			BYTE* pMask = pImageStream->getLine();
			int nIndex = 4 * nY * nWidth;
			for (int nX = 0; nX < nWidth; ++nX)
			{
				BYTE unPixel = *pMask++ ^ nInvert;
				if (unPixel)
				{
					pBufferPtr[nIndex + 0] = 255;
					pBufferPtr[nIndex + 1] = 255;
					pBufferPtr[nIndex + 2] = 255;
					pBufferPtr[nIndex + 3] = 0;
				}
				else
				{
					pBufferPtr[nIndex + 0] = b;
					pBufferPtr[nIndex + 1] = g;
					pBufferPtr[nIndex + 2] = r;
					pBufferPtr[nIndex + 3] = unAlpha;
				}
				nIndex += 4;
			}
		}

		delete pImageStream;

		double arrMatrix[6];
		double* pCTM = pGState->getCTM();

		//  Исходное предобразование
		//              |1  0  0|   |pCTM[0] pCTM[1] 0|
		// arrMattrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
		//              |0  1  1|   |pCTM[4] pCTM[5] 1|

		arrMatrix[0] = pCTM[0];
		arrMatrix[1] = -pCTM[1];
		arrMatrix[2] = -pCTM[2];
		arrMatrix[3] = pCTM[3];
		arrMatrix[4] = pCTM[2] + pCTM[4];
		arrMatrix[5] = -(pCTM[3] + pCTM[5]) + pGState->getPageHeight();

		double dShiftX = 0, dShiftY = 0;
		DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
		m_pRenderer->DrawImage(&oImage, dShiftX, dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
	}
	void RendererOutputDev::setSoftMaskFromImageMask(GfxState* pGState, Object* pRef, Stream* pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate)
	{
		if (m_bDrawOnlyText || pGState->getFillColorSpace()->isNonMarking())
			return;

		int nBufferSize = 4 * nWidth * nHeight;
		if (nBufferSize < 1)
			return;

		BYTE* pBufferPtr = new(std::nothrow) BYTE[nBufferSize];
		if (!pBufferPtr)
			return;

		double bbox[4] = { 0, 0, 1, 1 };
		beginTransparencyGroup(pGState, bbox, pGState->getFillColorSpace(), true, false, true);

		Aggplus::CImage oImage;
		oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth, true);

		// Пишем данные в pBufferPtr
		ImageStream* pImageStream = new ImageStream(pStream, nWidth, 1, 1);
		pImageStream->reset();

		GfxColorSpace* pColorSpace = pGState->getFillColorSpace();
		GfxRGB oRGB;
		pColorSpace->getRGB(pGState->getFillColor(), &oRGB, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
		GfxPattern* pPattern = pGState->getFillPattern();
		if (pPattern && pPattern->getType() == 2)
		{
			GfxShading* pShading = ((GfxShadingPattern*)pPattern)->getShading();
			pColorSpace = pShading->getColorSpace();
			if (pShading->getHasBackground())
				pColorSpace->getRGB(pShading->getBackground(), &oRGB, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
		}

		BYTE r = colToByte(oRGB.r);
		BYTE g = colToByte(oRGB.g);
		BYTE b = colToByte(oRGB.b);
		BYTE unAlpha = std::min(255, std::max(0, int(pGState->getFillOpacity() * 255)));
		int nInvert = bInvert ? 1 : 0;

		for (int nY = nHeight - 1; nY >= 0; --nY)
		{
			BYTE* pMask = pImageStream->getLine();
			int nIndex = 4 * nY * nWidth;
			for (int nX = 0; nX < nWidth; ++nX)
			{
				BYTE unPixel = *pMask++ ^ nInvert;
				if (unPixel)
				{
					pBufferPtr[nIndex + 0] = 255;
					pBufferPtr[nIndex + 1] = 255;
					pBufferPtr[nIndex + 2] = 255;
					pBufferPtr[nIndex + 3] = 0;
				}
				else
				{
					pBufferPtr[nIndex + 0] = b;
					pBufferPtr[nIndex + 1] = g;
					pBufferPtr[nIndex + 2] = r;
					pBufferPtr[nIndex + 3] = unAlpha;
				}
				nIndex += 4;
			}
		}

		delete pImageStream;

		double arrMatrix[6];
		double* pCTM = pGState->getCTM();

		//  Исходное предобразование
		//              |1  0  0|   |pCTM[0] pCTM[1] 0|
		// arrMattrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
		//              |0  1  1|   |pCTM[4] pCTM[5] 1|

		arrMatrix[0] = pCTM[0];
		arrMatrix[1] = -pCTM[1];
		arrMatrix[2] = -pCTM[2];
		arrMatrix[3] = pCTM[3];
		arrMatrix[4] = pCTM[2] + pCTM[4];
		arrMatrix[5] = -(pCTM[3] + pCTM[5]) + pGState->getPageHeight();

		double dShiftX = 0, dShiftY = 0;
		DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
		m_pRenderer->DrawImage(&oImage, dShiftX, dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));

		setSoftMask(pGState, bbox, 0, NULL, NULL);
	}

	OO_INLINE bool CheckMask(const int& nComponentsCount, const int* pMaskColors, const BYTE* pLine)
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
	BYTE* BufferFromImageStream(GfxState* pGState, Stream* pStream, int nWidth, int nHeight, GfxImageColorMap* pColorMap, int* pMaskColors)
	{
		int nBufferSize = 4 * nWidth * nHeight;
		if (nBufferSize < 1)
			return NULL;

		BYTE* pBufferPtr = new(std::nothrow) BYTE[nBufferSize];
		if (!pBufferPtr)
			return NULL;

		int nComponentsCount = pColorMap->getNumPixelComps();
		BYTE unAlpha = std::min(255, std::max(0, int(pGState->getFillOpacity() * 255)));

		// Пишем данные в pBufferPtr
		ImageStream* pImageStream = new ImageStream(pStream, nWidth, nComponentsCount, pColorMap->getBits());
		pImageStream->reset();

		int nComps = pImageStream->getComps();
		int nCheckWidth = std::min(nWidth, pImageStream->getVals() / nComps);
		GfxRenderingIntent intent = pGState->getRenderingIntent();

		// fast realization for some colorspaces (for wasm module)
		int nColorMapType = pColorMap->getFillType();
		GfxColorComp** pColorMapLookup = pColorMap->getLookup();
		if (!pColorMapLookup)
			nColorMapType = 0;

		for (int nY = nHeight - 1; nY >= 0; --nY)
		{
			BYTE* pLine = pImageStream->getLine();
			BYTE* pLineDst = pBufferPtr + 4 * nWidth * nY;

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
				else if (3 == nColorMapType)
				{
					pLineDst[2] = colToByte(clip01(pColorMapLookup[0][pLine[0]]));
					pLineDst[1] = colToByte(clip01(pColorMapLookup[1][pLine[1]]));
					pLineDst[0] = colToByte(clip01(pColorMapLookup[2][pLine[2]]));
					pLineDst[3] = colToByte(clip01(pColorMapLookup[3][pLine[3]]));
				}
				else
				{
					GfxRGB oRGB;
					pColorMap->getRGB(pLine, &oRGB, intent);
					pLineDst[0] = colToByte(oRGB.b);
					pLineDst[1] = colToByte(oRGB.g);
					pLineDst[2] = colToByte(oRGB.r);
				}

				if (pMaskColors && CheckMask(nComponentsCount, pMaskColors, pLine))
					pLineDst[3] = 0;
				else if (3 != nColorMapType)
					pLineDst[3] = unAlpha;

				pLine += nComps;
				pLineDst += 4;
			}
		}

		pImageStream->close();
		delete pImageStream;

		return pBufferPtr;
	}
	bool RendererOutputDev::ReadImage(Aggplus::CImage* pImageRes, Object* pRef, Stream* pStream)
	{
		Object oIm;
		int nLength = 0;
		if (pRef && pRef->isRef() && pRef->fetch(m_pXref, &oIm)->isStream())
		{
			Object oLength;
			if (oIm.streamGetDict()->lookup("Length", &oLength)->isInt())
				nLength = oLength.getInt();
			oLength.free();
		}
		oIm.free();
		if (!nLength)
			return false;

		BYTE* pBuffer = new BYTE[nLength];
		Stream* pS = pStream->getUndecodedStream();
		pS->reset();
		nLength = pS->getBlock((char*)pBuffer, nLength);
		if (!nLength)
		{
			RELEASEARRAYOBJECTS(pBuffer);
			return false;
		}

		CBgraFrame oFrame;
		if (oFrame.Decode(pBuffer, nLength, _CXIMAGE_FORMAT_JPG))
		{
			pImageRes->Create(oFrame.get_Data(), oFrame.get_Width(), oFrame.get_Height(), -4 * oFrame.get_Width());
			oFrame.ClearNoAttack();
			RELEASEARRAYOBJECTS(pBuffer);
			return true;
		}

		RELEASEARRAYOBJECTS(pBuffer);
		return false;
	}
	void RendererOutputDev::drawImage(GfxState* pGState, Object* pRef, Stream* pStream, int nWidth, int nHeight, GfxImageColorMap* pColorMap, int* pMaskColors, GBool bInlineImg, GBool interpolate)
	{
		if (m_bDrawOnlyText)
			return;

		Aggplus::CImage oImage;
		StreamKind nSK = pStream->getKind();
		int nComponentsCount = pColorMap->getNumPixelComps();
		BYTE unAlpha = std::min(255, std::max(0, int(pGState->getFillOpacity() * 255)));

		// Чтение jpeg через cximage происходит быстрее чем через xpdf на ~40%
		if (pMaskColors || unAlpha != 255 || (nSK != strDCT || nComponentsCount != 3 || !ReadImage(&oImage, pRef, pStream)))
		{
			BYTE* pBufferPtr = BufferFromImageStream(pGState, pStream, nWidth, nHeight, pColorMap, pMaskColors);
			if (!pBufferPtr)
				return;

			oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);
		}

		double arrMatrix[6];
		double* pCTM = pGState->getCTM();
		//  Исходное предобразование
		//             |1  0  0|   |pCTM[0] pCTM[1] 0|
		// arrMatrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
		//             |0  1  1|   |pCTM[4] pCTM[5] 1|
		arrMatrix[0] = pCTM[0];
		arrMatrix[1] = -pCTM[1];
		arrMatrix[2] = -pCTM[2];
		arrMatrix[3] = pCTM[3];
		arrMatrix[4] = pCTM[2] + pCTM[4];
		arrMatrix[5] = -(pCTM[3] + pCTM[5]) + pGState->getPageHeight();

		double dShiftX = 0, dShiftY = 0;
		DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
		m_pRenderer->DrawImage(&oImage, dShiftX, dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
	}
	void RendererOutputDev::drawMaskedImage(GfxState* pGState, Object* pRef, Stream* pStream, int nWidth, int nHeight, GfxImageColorMap* pColorMap, Object* pStreamRef, Stream* pMaskStream, int nMaskWidth, int nMaskHeight, GBool bMaskInvert, GBool interpolate)
	{
		if (m_bDrawOnlyText)
			return;

		if (nMaskWidth <= 0 || nMaskHeight <= 0)
			drawImage(pGState, pRef, pStream, nWidth, nHeight, pColorMap, NULL, false, interpolate);

		if (nMaskWidth > nWidth || nMaskHeight > nHeight)
		{
			// If the mask is higher resolution than the image, use
			// drawSoftMaskedImage() instead.

			GfxImageColorMap* maskColorMap;
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

		BYTE* pBufferPtr = new(std::nothrow) BYTE[nBufferSize];
		if (!pBufferPtr)
			return;

		Aggplus::CImage oImage;
		oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);

		// Пишем данные в pBufferPtr
		ImageStream* pImageStream = new ImageStream(pStream, nWidth, pColorMap->getNumPixelComps(), pColorMap->getBits());
		ImageStream* pMask = new ImageStream(pMaskStream, nMaskWidth, 1, 1);
		pMask->reset();
		pImageStream->reset();

		BYTE unAlpha = std::min(255, std::max(0, int(pGState->getFillOpacity() * 255)));
		if (nWidth != nMaskWidth || nHeight != nMaskHeight)
		{
			BYTE* pMaskBuffer = new(std::nothrow) BYTE[nMaskWidth * nMaskHeight];
			if (!pMaskBuffer)
			{
				delete pMask;
				delete pImageStream;
				return;
			}

			BYTE unMask = 0;
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

			BYTE unPixel[4] ={ 0, 0, 0, 0 };
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
						pBufferPtr[nIndex + 3] = unAlpha;

					nIndex += 4;
				}
			}

			delete[] pMaskBuffer;
		}
		else
		{
			BYTE unPixel[4] = { 0, 0, 0, 0 };
			BYTE unMask = 0;
			for (int nY = nHeight - 1; nY >= 0; --nY)
			{
				int nIndex = 4 * nY * nWidth;
				for (int nX = 0; nX < nWidth; ++nX)
				{
					pImageStream->getPixel(unPixel);
					pMask->getPixel(&unMask);
					GfxRGB oRGB;
					pColorMap->getRGB(unPixel, &oRGB, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
					pBufferPtr[nIndex + 0] = colToByte(oRGB.b);
					pBufferPtr[nIndex + 1] = colToByte(oRGB.g);
					pBufferPtr[nIndex + 2] = colToByte(oRGB.r);
					pBufferPtr[nIndex + 3] = unMask && !bMaskInvert ? 0 : unAlpha;

					nIndex += 4;
				}
			}
		}

		delete pMask;
		delete pImageStream;

		double arrMatrix[6];
		double* pCTM = pGState->getCTM();
		//  Исходное предобразование
		//             |1  0  0|   |pCTM[0] pCTM[1] 0|
		// arrMatrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
		//             |0  1  1|   |pCTM[4] pCTM[5] 1|
		arrMatrix[0] = pCTM[0];
		arrMatrix[1] = -pCTM[1];
		arrMatrix[2] = -pCTM[2];
		arrMatrix[3] = pCTM[3];
		arrMatrix[4] = pCTM[2] + pCTM[4];
		arrMatrix[5] = -(pCTM[3] + pCTM[5]) + dPageHeight;

		double dShiftX = 0, dShiftY = 0;
		DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
		m_pRenderer->DrawImage(&oImage, dShiftX, dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
	}
	void RendererOutputDev::drawSoftMaskedImage(GfxState* pGState, Object* pRef, Stream* pStream, int nWidth, int nHeight, GfxImageColorMap* pColorMap, Object* maskRef, Stream* pMaskStream, int nMaskWidth, int nMaskHeight, GfxImageColorMap* pMaskColorMap, double* pMatteColor, GBool interpolate)
	{
		if (m_bDrawOnlyText)
			return;

		int nBufferSize = 4 * nWidth * nHeight;
		if (nBufferSize < 1)
			return;

		Aggplus::CImage oImage;
		int nComponentsCount = pColorMap->getNumPixelComps();

		if (nComponentsCount != 3 || pStream->getKind() != strDCT || !ReadImage(&oImage, pRef, pStream))
		{
			BYTE* pBufferPtr = BufferFromImageStream(pGState, pStream, nWidth, nHeight, pColorMap, NULL);
			if (!pBufferPtr)
				return;
			oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);
		}

		BYTE* pBufferPtr = oImage.GetData();
		double dAlphaKoef = pGState->getFillOpacity();
		if (nWidth != nMaskWidth || nHeight != nMaskHeight)
		{
			// TODO: Здесь сделан элементарный вариант масштабирования маски.
			//        Надо улучшить алгоритм.

			bool bResize = true;

			if (0 != nWidth && 0 != nMaskHeight && 0 != nHeight && 0 != nMaskWidth)
			{
				ImageStream* pSMaskStream = new ImageStream(pMaskStream, nMaskWidth, pMaskColorMap->getNumPixelComps(), pMaskColorMap->getBits());
				BYTE* pAlpha = new(std::nothrow) BYTE[nMaskWidth * nMaskHeight];
				if (pSMaskStream && pAlpha)
				{
					pSMaskStream->reset();

					BYTE unAlpha = 0;
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
						BYTE* pImageBuffer = pBufferPtr;
						pBufferPtr = new(std::nothrow) BYTE[4 * nMaxW * nMaxH];
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
								pBufferPtr[nIndex + 3] = (BYTE)(pAlpha[nNearestAlphaMatch] * dAlphaKoef);
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

								pBufferPtr[nIndex + 3] = (BYTE)(pAlpha[nNearestAlphaMatch] * dAlphaKoef);
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
					pBufferPtr[i] = (BYTE)(255.0 * dAlphaKoef);
				}
			}
		}
		else
		{
			ImageStream* pSMaskStream = new ImageStream(pMaskStream, nMaskWidth, pMaskColorMap->getNumPixelComps(), pMaskColorMap->getBits());
			pSMaskStream->reset();

			// Быстрая реализация для масок
			int nMaskColorMapType = pMaskColorMap->getFillType();
			GfxColorComp** pMaskColorMapLookup = pMaskColorMap->getLookup();
			if (!pMaskColorMapLookup)
				nMaskColorMapType = 0;
			int nMaskComps = pSMaskStream->getComps();
			int nMaskCheckWidth = std::min(nMaskWidth, pSMaskStream->getVals() / nMaskComps);

			BYTE unAlpha = 0;
			for (int nY = nMaskHeight - 1; nY >= 0; --nY)
			{
				BYTE* pMaskLine = pSMaskStream->getLine();
				int nIndex = 4 * nY * nMaskWidth;
				if (!pMaskLine)
				{
					// Заполняем прозрачностью, если линия не прочитана
					for (int nX = 0; nX < nMaskWidth; ++nX)
					{
						pBufferPtr[nIndex + 3] = 0;
						nIndex += 4;
					}
					continue;
				}

				for (int nX = 0; nX < nMaskCheckWidth; ++nX)
				{
					BYTE unAlpha = 0;

					// Оптимизированные случаи для разных цветовых пространств
					if (1 == nMaskColorMapType)
					{
						unAlpha = colToByte(clip01(pMaskColorMapLookup[0][pMaskLine[0]]));
					}
					else if (2 == nMaskColorMapType || 3 == nMaskColorMapType)
					{
						unAlpha = colToByte(clip01(0.3 * pMaskColorMapLookup[0][pMaskLine[0]] +
												  0.59 * pMaskColorMapLookup[1][pMaskLine[1]] +
												  0.11 * pMaskColorMapLookup[2][pMaskLine[2]] + 0.5));
					}
					else
					{
						GfxGray oGray;
						pMaskColorMap->getGray(pMaskLine, &oGray, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
						unAlpha = colToByte(oGray);
					}

					pBufferPtr[nIndex + 3] = (BYTE)(unAlpha * dAlphaKoef);
					pMaskLine += nMaskComps;
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
			for (int i = 0; i < pColorMap->getNumPixelComps(); ++i)
				oColor.c[i] = dblToCol(pMatteColor[i]);
			pColorMap->getColorSpace()->getRGB(&oColor, &oMatteRGB, gfxRenderingIntentAbsoluteColorimetric);

			BYTE unMatteR = colToByte(oMatteRGB.r);
			BYTE unMatteG = colToByte(oMatteRGB.g);
			BYTE unMatteB = colToByte(oMatteRGB.b);

			for (int nIndex = 0; nIndex < nBufferSize; nIndex += 4)
			{
				BYTE unA = pBufferPtr[nIndex + 3];
				if (unA)
				{
					double dK = 255.0 / unA;
					pBufferPtr[nIndex + 0] = std::max(0, std::min(255, int((pBufferPtr[nIndex + 0] - unMatteB) * dK + unMatteB)));
					pBufferPtr[nIndex + 1] = std::max(0, std::min(255, int((pBufferPtr[nIndex + 1] - unMatteG) * dK + unMatteG)));
					pBufferPtr[nIndex + 2] = std::max(0, std::min(255, int((pBufferPtr[nIndex + 2] - unMatteR) * dK + unMatteR)));
				}
				else
				{
					pBufferPtr[nIndex + 0] = 255;
					pBufferPtr[nIndex + 1] = 255;
					pBufferPtr[nIndex + 2] = 255;
				}
			}
		}

		double arrMatrix[6];
		double* pCTM = pGState->getCTM();
		//  Исходное предобразование
		//              |1  0  0|   |pCTM[0] pCTM[1] 0|
		// arrMattrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
		//              |0  1  1|   |pCTM[4] pCTM[5] 1|
		arrMatrix[0] = pCTM[0];
		arrMatrix[1] = -pCTM[1];
		arrMatrix[2] = -pCTM[2];
		arrMatrix[3] = pCTM[3];
		arrMatrix[4] = pCTM[2] + pCTM[4];
		arrMatrix[5] = -(pCTM[3] + pCTM[5]) + pGState->getPageHeight();

		double dShiftX = 0, dShiftY = 0;
		DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
		m_pRenderer->DrawImage(&oImage, dShiftX, dShiftY, PDFCoordsToMM(1), PDFCoordsToMM(1));
	}
	void RendererOutputDev::beginTransparencyGroup(GfxState* pGState, double* pBBox, GfxColorSpace* pBlendingColorSpace, GBool bIsolated, GBool bKnockout, GBool bForSoftMask)
	{
		if (c_nGrRenderer != m_lRendererType)
			return;

		m_pRenderer->BeginCommand(c_nLayerType);
		m_sCS.push_back(GfxOutputCS());
		m_sCS.back().bKnockout = bKnockout;
		m_sCS.back().pBlendingCS = pBlendingColorSpace;
		// TODO if (bKnockout)
	}
	void RendererOutputDev::endTransparencyGroup(GfxState* pGState)
	{
	}
	void RendererOutputDev::paintTransparencyGroup(GfxState* pGState, double* pBBox)
	{
		if (c_nGrRenderer != m_lRendererType)
			return;

		double dOpacity = std::min(1.0, std::max(0.0, pGState->getFillOpacity()));
		m_pRenderer->put_LayerOpacity(dOpacity);
		m_pRenderer->EndCommand(c_nLayerType);

		m_sCS.pop_back();
	}
	void RendererOutputDev::setSoftMask(GfxState* pGState, double* pBBox, GBool bAlpha, Function* pTransferFunc, GfxColor* pBackdropColor)
	{
		if (c_nGrRenderer != m_lRendererType)
			return;
		NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer);
		if (!GRenderer)
			return;

		RELEASEINTERFACE(m_pSoftMask);

		m_pSoftMask = GRenderer->CreateSoftMask(bAlpha);
		m_pSoftMask->AddRef();

		if (!bAlpha && m_sCS.back().pBlendingCS)
		{
			GfxRGB c;
			m_sCS.back().pBlendingCS->getRGB(pBackdropColor, &c, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
			DWORD dwColor = colToByte(c.r) + colToByte(c.g) * 0x100 + colToByte(c.b) * 0x100 * 0x100;
			// TODO цвет фона мягкой маски должен быть установлен в dwColor
		}

		if (pTransferFunc)
		{
			BYTE* pSource = m_pSoftMask->GetBuffer();
			int nWidth = m_pSoftMask->GetWidth();
			int nHeight = m_pSoftMask->GetHeight();

			for (int y = 0; y < nHeight; ++y)
			{
				for (int x = 0; x < nWidth; ++x)
				{
					int dLum = bAlpha ? pSource[y * nWidth * 4 + x * 4 + 3] : luminosity(pSource + y * nWidth * 4 + x * 4);

					double dLumIn, dLumOut;
					dLumIn = (double)dLum / 256.0;
					pTransferFunc->transform(&dLumIn, &dLumOut);
					dLum = (int)(dLumOut * 255.0 + 0.5);

					pSource[y * nWidth * 4 + x * 4 + 3] = dLum;
				}
			}

			// if (!bAlpha) // pTransferFunc преобразовала результат luminosity маски в alpha маску
			// 	m_pSoftMask->SetType(Aggplus::EMaskDataType::Alpha4Buffer);
		}

		m_sCS.pop_back();
	}
	void RendererOutputDev::clearSoftMask(GfxState* pGState)
	{
		if (c_nGrRenderer != m_lRendererType)
			return;
		if (NSGraphics::IGraphicsRenderer* GRenderer = dynamic_cast<NSGraphics::IGraphicsRenderer*>(m_pRenderer))
			GRenderer->SetSoftMask(NULL);
		RELEASEINTERFACE(m_pSoftMask);
	}
	void RendererOutputDev::NewPDF(XRef* pXref)
	{
		m_pXref = pXref;
	}
	void RendererOutputDev::DoPath(GfxState* pGState, GfxPath* pPath, double dPageHeight, double* pCTM, GfxClipMatrix* pCTM2)
	{
		if (m_bDrawOnlyText)
			return;

		double arrMatrix[6];
		if (pCTM2)
		{
			arrMatrix[0] =  pCTM2->dA;
			arrMatrix[1] = -pCTM2->dB;
			arrMatrix[2] =  pCTM2->dC;
			arrMatrix[3] = -pCTM2->dD;
			arrMatrix[4] =  pCTM2->dE;
			arrMatrix[5] = -pCTM2->dF + dPageHeight;
		}
		else
		{
			arrMatrix[0] =  pCTM[0];
			arrMatrix[1] = -pCTM[1];
			arrMatrix[2] =  pCTM[2];
			arrMatrix[3] = -pCTM[3];
			arrMatrix[4] =  pCTM[4];
			arrMatrix[5] = -pCTM[5] + dPageHeight;
		}

		double dShiftX = 0, dShiftY = 0;
		DoTransform(arrMatrix, &dShiftX, &dShiftY);

		m_pRenderer->BeginCommand(c_nPathType);
		m_pRenderer->PathCommandEnd();

		for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
		{
			GfxSubpath* pSubpath = pPath->getSubpath(nSubPathIndex);
			int nPointsCount = pSubpath->getNumPoints();

			m_pRenderer->PathCommandMoveTo(PDFCoordsToMM(pSubpath->getX(0) + dShiftX), PDFCoordsToMM(pSubpath->getY(0) + dShiftY));

			int nCurPointIndex = 1;
			while (nCurPointIndex < nPointsCount)
			{
				if (pSubpath->getCurve(nCurPointIndex))
				{
					m_pRenderer->PathCommandCurveTo(PDFCoordsToMM(pSubpath->getX(nCurPointIndex)     + dShiftX), PDFCoordsToMM(pSubpath->getY(nCurPointIndex)     + dShiftY),
													PDFCoordsToMM(pSubpath->getX(nCurPointIndex + 1) + dShiftX), PDFCoordsToMM(pSubpath->getY(nCurPointIndex + 1) + dShiftY),
													PDFCoordsToMM(pSubpath->getX(nCurPointIndex + 2) + dShiftX), PDFCoordsToMM(pSubpath->getY(nCurPointIndex + 2) + dShiftY));
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
	void RendererOutputDev::AddClip(GfxState* pGState, GfxOutputState* pState, int nIndex)
	{
		if (m_bDrawOnlyText || m_bTiling)
			return;

		GfxClip* pClip = pState->pClip;
		GfxPath* pPath = pClip->GetPath(nIndex);
		int  nClipFlag = pClip->GetClipFlag(nIndex);;

		m_pRenderer->BeginCommand(c_nClipType);
		m_pRenderer->put_ClipMode(nClipFlag);
		DoPath(pGState, pPath, pGState->getPageHeight(), pGState->getCTM(), &pClip->m_vMatrix[nIndex]);
		m_pRenderer->EndCommand(c_nClipType);
		m_pRenderer->PathCommandEnd();
	}
	void RendererOutputDev::AddTextClip(GfxState* pGState, GfxOutputState* pState)
	{
		if (m_bDrawOnlyText || m_bTiling)
			return;

		GfxTextClip* pTextClip = pState->pTextClip;
		m_pRenderer->BeginCommand(c_nClipType);
		m_pRenderer->put_ClipMode(c_nClipRegionTypeWinding | c_nClipRegionIntersect);
		m_pRenderer->StartConvertCoordsToIdentity();

		for (int nIndex = 0, nTextClipCount = pTextClip->GetTextsCount(); nIndex < nTextClipCount; nIndex++)
		{
			wchar_t* wsFontName, *wsFontPath;
			int lFontStyle;
			double dFontSize = 10, dX = 0, dY = 0, dWidth = 0, dHeight = 0, dBaseLineOffset = 0;
			wchar_t* wsText = pTextClip->GetText(nIndex, &dX, &dY, &dWidth, &dHeight, &dBaseLineOffset, &wsFontName, &wsFontPath, &dFontSize, &lFontStyle);

			m_pRenderer->put_FontName(wsFontName);
			m_pRenderer->put_FontPath(wsFontPath);
			m_pRenderer->put_FontSize(dFontSize);
			m_pRenderer->put_FontStyle(lFontStyle);

			double dShiftX = 0, dShiftY = 0;
			DoTransform(pTextClip->GetMatrix(nIndex), &dShiftX, &dShiftY, true);

			// TODO: нужна нормальная конвертация
			int nLen = 0;
			wchar_t* wsTextTmp = wsText;
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

		m_pRenderer->EndCommand(c_nClipType);
		m_pRenderer->PathCommandEnd();
		m_pRenderer->EndConvertCoordsToIdentity();
	}
	void RendererOutputDev::UpdateAllClip(GfxState* pGState)
	{
		if (m_bDrawOnlyText || m_bTiling)
			return;

		m_pRenderer->BeginCommand(c_nResetClipType);
		m_pRenderer->EndCommand(c_nResetClipType);

		for (int i = 0; i < m_sStates.size(); i++)
		{
			GfxClip* pClip = m_sStates[i].pClip;
			if (pClip)
				for (int nIndex = 0, nClipCount = pClip->GetPathNum(); nIndex < nClipCount; nIndex++)
					AddClip(pGState, &m_sStates[i], nIndex);

			GfxTextClip* pTextClip = m_sStates[i].pTextClip;
			if (pTextClip)
				AddTextClip(pGState, &m_sStates[i]);
		}

		updateFont(pGState);
	}
	void RendererOutputDev::DoTransform(double* pMatrix, double* pdShiftX, double* pdShiftY, bool bText)
	{
		if (1 == pMatrix[0] && 0 == pMatrix[1] && 0 == pMatrix[2] && 1 == pMatrix[3] && !bText)
		{
			if (pMatrix[4] || pMatrix[5])
			{
				*pdShiftX = pMatrix[4];
				*pdShiftY = pMatrix[5];
			}
			m_pRenderer->ResetTransform();
			m_arrMatrix[0] = 1; m_arrMatrix[1] = 0;
			m_arrMatrix[2] = 0; m_arrMatrix[3] = 1;
			m_arrMatrix[4] = 0; m_arrMatrix[5] = 0;
		}
		else if (m_arrMatrix[0] == pMatrix[0] && m_arrMatrix[1] == pMatrix[1] && m_arrMatrix[2] == pMatrix[2] && m_arrMatrix[3] == pMatrix[3]
				 && m_arrMatrix[4] == pMatrix[4]  && m_arrMatrix[5] == pMatrix[5] && !bText)
		{
			double dIDet = 1 / (pMatrix[0] * pMatrix[3] - pMatrix[1] * pMatrix[2]);
			*pdShiftX = ((pMatrix[4] - m_arrMatrix[4]) * m_arrMatrix[3] - (pMatrix[5] - m_arrMatrix[5]) * m_arrMatrix[1]) * dIDet;
			*pdShiftY = ((pMatrix[5] - m_arrMatrix[5]) * m_arrMatrix[0] - (pMatrix[4] - m_arrMatrix[4]) * m_arrMatrix[2]) * dIDet;
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
