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
#include "GState.h"
#include "GFont.h"
#include "File.h"
#include "CMap.h"
#include "Dict.h"
#include "Stream.h"
#include "FontFileTrueType.h"
#include "FontFileType1C.h"
#include "CharCodeToUnicode.h"
#include "RendererOutputDev.h"
#include "XmlUtils.h"

#include "../../DesktopEditor/graphics/Image.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Array.h"
#include "../../DesktopEditor/graphics/BaseThread.h"

#include "../../HtmlRenderer/include/HTMLRenderer3.h"

#include "../../PdfWriter/PdfRenderer.h"

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
		//	return;

		//if ( _T("PDFFontList") == oMainNode.GetName() )
		//{
		//	XmlUtils::CXmlNodes oFonts;
		//	oMainNode.GetNodes( _T("Font"), oFonts );
		//	for ( int nIndex = 0; nIndex < oFonts.GetCount(); nIndex++ )
		//	{
		//		XmlUtils::CXmlNode oFont;
		//		Ref oRef;
		//		CStringW sFilePath;
		//		int nLength = 0;
		//		unsigned short *pCodeToGid = NULL, *pCodeToUnicode = NULL;

		//		if ( oFonts.GetAt( nIndex, oFont ) )
		//		{
		//			XmlUtils::CXmlNode oNode;
		//			CStringW sValue;

		//			if ( oFont.GetNode( _T("ID"), oNode ) )
		//			{
		//				sValue = oNode.GetAttribute( _T("num") );
		//				oRef.nNum = XmlUtils::GetInteger( sValue );
		//				sValue = oNode.GetAttribute( _T("gen") );
		//				oRef.nGen = XmlUtils::GetInteger( sValue );
		//			}

		//			if ( oFont.GetNode( _T("FilePath"), oNode ) ) 
		//			{
		//				sFilePath = oNode.GetAttribute( _T("value") );
		//			}
		//			if ( oFont.GetNode( _T("CodeToGid"), oNode ) )
		//			{
		//				sValue = oNode.GetAttribute( _T("length") );
		//				nLength = XmlUtils::GetInteger( sValue );
		//				pCodeToGid = (unsigned short *)MemUtilsMalloc( sizeof(unsigned short) * nLength );
		//				if ( !pCodeToGid )
		//					return;

		//				XmlUtils::CXmlNodes oArray;
		//				oNode.GetNodes( _T("Entry"), oArray );
		//				for ( int nCurIndex = 0; nCurIndex < oArray.GetCount() && nCurIndex < nLength; nCurIndex++ )
		//				{
		//					XmlUtils::CXmlNode oArrayItem;
		//					if ( oArray.GetAt( nCurIndex, oArrayItem ) )
		//					{
		//						sValue = oArrayItem.GetAttribute( _T("value") );
		//						pCodeToGid[nCurIndex] = XmlUtils::GetInteger( sValue );
		//					}
		//					else
		//						pCodeToGid[nCurIndex] = 0;
		//				}
		//			}
		//			if ( oFont.GetNode( _T("CodeToUnicode"), oNode ) )
		//			{
		//				sValue = oNode.GetAttribute( _T("length") );
		//				nLength = XmlUtils::GetInteger( sValue );
		//				pCodeToUnicode = (unsigned short *)MemUtilsMalloc( sizeof(unsigned short) * nLength );
		//				if ( !pCodeToUnicode )
		//					return;

		//				XmlUtils::CXmlNodes oArray;
		//				oNode.GetNodes( _T("Entry"), oArray );
		//				for ( int nCurIndex = 0; nCurIndex < oArray.GetCount() && nCurIndex < nLength; nCurIndex++ )
		//				{
		//					XmlUtils::CXmlNode oArrayItem;
		//					if ( oArray.GetAt( nCurIndex, oArrayItem ) )
		//					{
		//						sValue = oArrayItem.GetAttribute( _T("value") );
		//						pCodeToUnicode[nCurIndex] = XmlUtils::GetInteger( sValue );
		//					}
		//					else
		//						pCodeToUnicode[nCurIndex] = 0;
		//				}
		//			}

		//		}

		//		Add( oRef, sFilePath, pCodeToGid, pCodeToUnicode, nLength );
		//	}
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
		//	TFontEntry oEntry = m_arrList.GetAt( nIndex );
		//	oWriter.WriteNodeBegin( _T("Font") );


		//	oWriter.WriteNodeBegin( _T("ID"), true );
		//	oWriter.WriteAttribute( _T("num"), oEntry.oRef.nNum );
		//	oWriter.WriteAttribute( _T("gen"), oEntry.oRef.nGen );
		//	oWriter.WriteNodeEnd( _T("ID"), true, true );

		//	oWriter.WriteNodeBegin( _T("FilePath"), true );
		//	oWriter.WriteAttribute( _T("value"), oEntry.wsFilePath );
		//	oWriter.WriteNodeEnd( _T("FilePath"), true, true );

		//	if ( NULL != oEntry.pCodeToGID )
		//	{
		//		oWriter.WriteNodeBegin( _T("CodeToGid"), true );
		//		oWriter.WriteAttribute( _T("length"), oEntry.nLen );
		//		oWriter.WriteNodeEnd( _T("CodeToGid"), true, false );
		//		for ( int nCurIndex = 0; nCurIndex < oEntry.nLen; nCurIndex++ )
		//		{
		//			oWriter.WriteNodeBegin( _T("Entry"), true );
		//			oWriter.WriteAttribute( _T("value"), oEntry.pCodeToGID[nCurIndex] );
		//			oWriter.WriteNodeEnd( _T("Entry"), true, true );
		//		}
		//		oWriter.WriteNodeEnd( _T("CodeToGid") );
		//	}

		//	if ( NULL != oEntry.pCodeToUnicode )
		//	{
		//		oWriter.WriteNodeBegin( _T("CodeToUnicode"), true );
		//		oWriter.WriteAttribute( _T("length"), oEntry.nLen );
		//		oWriter.WriteNodeEnd( _T("CodeToUnicode"), true, false );
		//		for ( int nCurIndex = 0; nCurIndex < oEntry.nLen; nCurIndex++ )
		//		{
		//			oWriter.WriteNodeBegin( _T("Entry"), true );
		//			oWriter.WriteAttribute( _T("value"), oEntry.pCodeToUnicode[nCurIndex] );
		//			oWriter.WriteNodeEnd( _T("Entry"), true, true );
		//		}
		//		oWriter.WriteNodeEnd( _T("CodeToUnicode") );
		//	}

		//	oWriter.WriteNodeEnd( _T("Font") );
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
	TFontEntry* CFontList::Add(Ref oRef, std::wstring wsFileName, unsigned short *pCodeToGID, unsigned short *pCodeToUnicode, unsigned int unLenGID, unsigned int unLenUnicode)
	{
		// Данная функция приходит только из Find2, поэтому проверять есть ли данный шрифт уже не надо
		CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

		TFontEntry *pNewEntry = new TFontEntry;
		pNewEntry->oRef.nGen      = oRef.nGen;
		pNewEntry->oRef.nNum      = oRef.nNum;
		pNewEntry->wsFilePath     = wsFileName;
		pNewEntry->pCodeToGID     = pCodeToGID;
		pNewEntry->pCodeToUnicode = pCodeToUnicode;
		pNewEntry->unLenGID       = unLenGID;
		pNewEntry->unLenUnicode   = unLenUnicode;

		Add(oRef, pNewEntry);

		RELEASEOBJECT(pCS);

		return pNewEntry;
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
    RendererOutputDev::RendererOutputDev(GlobalParams *pGlobalParams, IRenderer *pRenderer, NSFonts::IFontManager* pFontManager, CFontList *pFontList)
	{
		m_pFontManager  = pFontManager;
		m_pGlobalParams = pGlobalParams;
		m_pFontList     = pFontList;

		m_bTiling       = false;

		m_pBufferTextClip = NULL;
		m_pClip           = NULL;

		m_lRendererType = c_nUnknownRenderer;
		m_pRenderer     = pRenderer;

		m_bUseAxialShaded  = false;
		m_bUseRadialShaded = false;

		if (NULL != m_pRenderer)
		{
			m_pRenderer->put_PenColor(0);
			m_pRenderer->put_PenAlpha(255);
			m_pRenderer->put_PenSize(1);

			m_pRenderer->put_FontName(L"Arial");
			m_pRenderer->put_FontStyle(0);
			m_pRenderer->put_FontSize(10.0);

			m_pRenderer->get_Type(&m_lRendererType);
			//if (c_nSWFRenderer == m_lRendererType)
			//{
			//	m_bUseAxialShaded  = true;
			//	m_bUseRadialShaded = true;
			//}
		}

		m_pbBreak                   = NULL;
		m_bTransparentGroup         = false;
		m_bTransparentGroupSoftMask = false;
		m_pTransparentGroupSoftMask = NULL;

		m_bDrawOnlyText = false;

		//m_oFontList.LoadFromFile( m_pGlobalParams->GetTempFolder() );
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

		if (m_pClip)
			delete m_pClip;

		if (m_pBufferTextClip)
			delete m_pBufferTextClip;

		if (m_pTransparentGroupSoftMask)
			delete[]m_pTransparentGroupSoftMask;
	}
	void RendererOutputDev::StartPage(int nPageIndex, GrState *pGState)
	{
		m_pRenderer->BeginCommand(c_nPageType);

		// Переводим пункты в миллиметры
		m_arrMatrix[0] = 1; m_arrMatrix[1] = 0;
		m_arrMatrix[2] = 0; m_arrMatrix[3] = 1;
		m_arrMatrix[4] = 0; m_arrMatrix[5] = 0;

		m_bTransparentGroup = false;
		m_bTransparentGroupSoftMask = false;

		if (m_pTransparentGroupSoftMask)
			delete[]m_pTransparentGroupSoftMask;
		m_pTransparentGroupSoftMask = NULL;

        if (c_nHtmlRendrerer2 == m_lRendererType)
            m_bDrawOnlyText = ((NSHtmlRenderer::CASCHTMLRenderer3*)m_pRenderer)->GetOnlyTextMode();
        else
			m_bDrawOnlyText = false;
	}
	void RendererOutputDev::EndPage()
	{
		m_pRenderer->EndCommand(c_nPageType);
	}
	void RendererOutputDev::SaveGState(GrState *pGState)
	{
		UpdateAll(pGState);
	}
	void RendererOutputDev::RestoreGState(GrState *pGState)
	{
		UpdateAll(pGState);
	}
	void RendererOutputDev::UpdateCTM(GrState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32)
	{
	}
	void RendererOutputDev::UpdateLineDash(GrState *pGState)
	{
		double *pDash  = NULL;
		int     nSize  = 0;
		double  dStart = 0;
		pGState->GetLineDash(&pDash, &nSize, &dStart);

		if (0 == nSize) // Solid
		{
			m_pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
			m_pRenderer->put_PenDashOffset(0);
		}
		else
		{
			m_pRenderer->PenDashPattern(pDash, (long)nSize);
			m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
			m_pRenderer->put_PenDashOffset(dStart);
		}
	}
	void RendererOutputDev::UpdateFlatness(GrState *pGState)
	{
	}
	void RendererOutputDev::UpdateLineJoin(GrState *pGState)
	{
		int nJoinStyle = pGState->GetLineJoin();
		if (1 == nJoinStyle)
			nJoinStyle = 2;
		else if (2 == nJoinStyle)
			nJoinStyle = 1;

		m_pRenderer->put_PenLineJoin(nJoinStyle);
	}
	void RendererOutputDev::UpdateLineCap(GrState *pGState)
	{
		int nCapStyle = pGState->GetLineCap();
		if (1 == nCapStyle)
			nCapStyle = 2;
		else if (2 == nCapStyle)
			nCapStyle = 1;

		m_pRenderer->put_PenLineStartCap(nCapStyle);
		m_pRenderer->put_PenLineEndCap(nCapStyle);
	}
	void RendererOutputDev::UpdateMiterLimit(GrState *pGState)
	{
	}
	void RendererOutputDev::UpdateLineWidth(GrState *pGState)
	{
		m_pRenderer->put_PenSize(PDFCoordsToMM(pGState->GetLineWidth()));
	}
	void RendererOutputDev::UpdateStrokeAdjust(GrState *pGState)
	{
	}
	void RendererOutputDev::UpdateFillColor(GrState *pGState)
	{
		GrColor *pColor = pGState->GetFillColor();
		GrColorSpace *pColorSpace = pGState->GetFillColorSpace();

		DWORD dwColor = pColorSpace->GetDwordColor(pColor);

		m_pRenderer->put_BrushColor1(dwColor);
		m_pRenderer->put_BrushColor2(dwColor);
	}
	void RendererOutputDev::UpdateStrokeColor(GrState *pGState)
	{
		GrColor *pColor = pGState->GetStrokeColor();
		GrColorSpace *pColorSpace = pGState->GetStrokeColorSpace();

		DWORD dwColor = pColorSpace->GetDwordColor(pColor);

		m_pRenderer->put_PenColor(dwColor);
	}
	void RendererOutputDev::UpdateBlendMode(GrState *pGState)
	{
	}
	void RendererOutputDev::UpdateFillOpacity(GrState *pGState)
	{
		m_pRenderer->put_BrushAlpha1(min(255, max(0, int(pGState->GetFillOpacity() * 255))));
		m_pRenderer->put_BrushAlpha2(min(255, max(0, int(pGState->GetFillOpacity() * 255))));
	}
	void RendererOutputDev::UpdateStrokeOpacity(GrState *pGState)
	{
		m_pRenderer->put_PenAlpha(min(255, max(0, int(pGState->GetStrokeOpacity() * 255))));
	}
	void RendererOutputDev::UpdateAll(GrState *pGState)
	{
		UpdateCTM(pGState, pGState->GetCTM()[0], pGState->GetCTM()[1], pGState->GetCTM()[2], pGState->GetCTM()[3], pGState->GetCTM()[4], pGState->GetCTM()[5]);
		UpdateLineDash(pGState);
		UpdateFlatness(pGState);
		UpdateLineJoin(pGState);
		UpdateLineCap(pGState);
		UpdateMiterLimit(pGState);
		UpdateLineWidth(pGState);
		UpdateStrokeAdjust(pGState);
		UpdateFillColorSpace(pGState);
		UpdateFillColor(pGState);
		UpdateStrokeColorSpace(pGState);
		UpdateStrokeColor(pGState);
		UpdateBlendMode(pGState);
		UpdateFillOpacity(pGState);
		UpdateStrokeOpacity(pGState);
		UpdateFont(pGState);
		UpdateClip(pGState);
	}
	void RendererOutputDev::UpdateRender(GrState *pGState)
	{

	}
	void RendererOutputDev::UpdateFont(GrState *pGState)
	{
		// Проверяем наличие списка со шрифтами
		if (NULL == m_pFontList)
			return;

		GrFont *pFont = pGState->GetFont();
		if (NULL == pFont)
			return;

		m_pRenderer->put_FontSize(pGState->GetFontSize());
		//m_oFont.Size = pGState->GetFontSize();

		std::wstring wsFileName = L"";
		std::wstring wsFontName = L"";
		TFontEntry *pEntry = NULL;
		if (!m_pFontList->Find2((*pFont->GetID()), &pEntry))
		{
			GrFontType eFontType = pFont->GetType();
			if (fontType3 == eFontType) // FontType3 обрабатывается отдельной командой
			{
				pEntry->bAvailable = true;
				return;
			}

			std::wstring wsTempFileName = L"";
			Ref oEmbRef;
			// 1. Если шрифт внедренный, тогда скидываем его в темповый файл.
			// 2. Если шрифт лежит вне пдф, а в самом пдф есть ссылка на него, тогда используем эту ссылку.
			// 3. В противном случае подбираем шрифт.			
			if (pFont->GetEmbeddedFontFileRef(&oEmbRef))
			{
				std::wstring wsExt;
				switch (pFont->GetType())
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

				FILE* pTempFile = NULL;
				if (!NSFile::CFileBinary::OpenTempFile(&wsTempFileName, &pTempFile, L"wb", (wchar_t*)wsExt.c_str(), (wchar_t*)m_pGlobalParams->GetTempFolder().c_str(), NULL))
				{
					if (L"" != wsTempFileName)
						NSFile::CFileBinary::Remove(wsTempFileName);

					pEntry->bAvailable = true;
					return;
				}

				Object oReferenceObject, oStreamObject;
				oReferenceObject.InitRef(oEmbRef.nNum, oEmbRef.nGen);
				oReferenceObject.Fetch(m_pXref, &oStreamObject);
				oReferenceObject.Free();
				if (!oStreamObject.IsStream())
				{
					// Внедренный шрифт неправильно записан
					oStreamObject.Free();
					fclose(pTempFile);

					if (L"" != wsTempFileName)
						NSFile::CFileBinary::Remove(wsTempFileName);

					pEntry->bAvailable = true;
					return;
				}
				oStreamObject.StreamReset();
				int nChar;
				while ((nChar = oStreamObject.StreamGetChar()) != EOF)
				{
					fputc(nChar, pTempFile);
				}
				oStreamObject.StreamClose();
				oStreamObject.Free();
				fclose(pTempFile);
				wsFileName = wsTempFileName;

				// Для шрифтов типа Type1 нужно дописать Afm файл с метриками
				if (fontType1 == pFont->GetType() || fontType1C == pFont->GetType() || fontType1COT == pFont->GetType())
				{
					std::wstring wsSplitFileName, wsSplitFileExt;
					SpitPathExt(wsFileName, &wsSplitFileName, &wsSplitFileExt);
					std::wstring wsAfmPath = wsSplitFileName + L".afm";
					
					FILE* pFile = NSFile::CFileBinary::OpenFileNative(wsAfmPath, L"wb");
					if (pFile)
					{
						Ref *pRef = pFont->GetID();
						Object oRefObject, oFontObject;
						oRefObject.InitRef(pRef->nNum, pRef->nGen);
						oRefObject.Fetch(m_pXref, &oFontObject);
						oRefObject.Free();

						if (oFontObject.IsDict())
						{
							char *sFontName = NULL, *sFontFamily = NULL, *sFontStretch = NULL;
							int nFontWeight = 0, nItalicAngle = 0, nAscent = 0, nDescent = 0, nLeading = 0;
							int nCapHeight = 0, nXHeight = 0, nStemV = 0, nStemH = 0, nAvgWidth = 0, nMaxWidth = 0, nMissingWidth = 0;
							Array *pBBox = NULL;
							int arrBBox[4] ={ 0, 0, 0, 0 };

							Dict *pFontDict = oFontObject.GetDict();
							Object oFontDescriptor;
							if (pFontDict->Search("FontDescriptor", &oFontDescriptor)->IsDict())
							{
								Object oDictItem;
								// FontName
								oFontDescriptor.DictLookup("FontName", &oDictItem);
								if (oDictItem.IsName()) sFontName = oDictItem.GetName();
								oDictItem.Free();

								// FontFamily
								oFontDescriptor.DictLookup("FontFamily", &oDictItem);
								if (oDictItem.IsName()) sFontFamily = oDictItem.GetName();
								oDictItem.Free();

								// FontStretch
								oFontDescriptor.DictLookup("FontStretch", &oDictItem);
								if (oDictItem.IsName()) sFontStretch = oDictItem.GetName();
								oDictItem.Free();

								// FontWeight
								oFontDescriptor.DictLookup("FontWeight", &oDictItem);
								if (oDictItem.IsInt()) nFontWeight = oDictItem.GetInt();
								oDictItem.Free();

								// FontBBox
								oFontDescriptor.DictLookup("FontBBox", &oDictItem);
								if (oDictItem.IsArray()) pBBox = oDictItem.GetArray();
								if (4 == pBBox->GetCount())
								{
									for (int nIndex = 0; nIndex < 4; nIndex++)
									{
										Object oArrayItem;
										pBBox->Get(nIndex, &oArrayItem);
										if (oArrayItem.IsInt())
											arrBBox[nIndex] = oArrayItem.GetInt();

										oArrayItem.Free();
									}
								}
								oDictItem.Free();

								// ItalicAngle
								oFontDescriptor.DictLookup("ItalicAngle", &oDictItem);
								if (oDictItem.IsInt()) nItalicAngle = oDictItem.GetInt();
								oDictItem.Free();

								// Ascent
								oFontDescriptor.DictLookup("Ascent", &oDictItem);
								if (oDictItem.IsInt()) nAscent = oDictItem.GetInt();
								oDictItem.Free();

								// Leading
								oFontDescriptor.DictLookup("Leading", &oDictItem);
								if (oDictItem.IsInt()) nLeading = oDictItem.GetInt();
								oDictItem.Free();

								// CapHeight
								oFontDescriptor.DictLookup("CapHeight", &oDictItem);
								if (oDictItem.IsInt()) nCapHeight = oDictItem.GetInt();
								oDictItem.Free();

								// XHeight
								oFontDescriptor.DictLookup("XHeight", &oDictItem);
								if (oDictItem.IsInt()) nXHeight = oDictItem.GetInt();
								oDictItem.Free();

								// StemV
								oFontDescriptor.DictLookup("StemV", &oDictItem);
								if (oDictItem.IsInt()) nStemV = oDictItem.GetInt();
								oDictItem.Free();

								// StemH
								oFontDescriptor.DictLookup("StemH", &oDictItem);
								if (oDictItem.IsInt()) nStemH = oDictItem.GetInt();
								oDictItem.Free();

								// Descent
								oFontDescriptor.DictLookup("Descent", &oDictItem);
								if (oDictItem.IsInt()) nDescent = oDictItem.GetInt();
								oDictItem.Free();

								// AvgWidth
								oFontDescriptor.DictLookup("AvgWidth", &oDictItem);
								if (oDictItem.IsInt()) nAvgWidth = oDictItem.GetInt();
								oDictItem.Free();

								// MaxWidth
								oFontDescriptor.DictLookup("MaxWidth", &oDictItem);
								if (oDictItem.IsInt()) nMaxWidth = oDictItem.GetInt();
								oDictItem.Free();

								// MissingWidth
								oFontDescriptor.DictLookup("MissingWidth", &oDictItem);
								if (oDictItem.IsInt()) nMissingWidth = oDictItem.GetInt();
								oDictItem.Free();

							}
							oFontDescriptor.Free();

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
							pFontDict->Search("FirstChar", &oDictItem);
							if (oDictItem.IsInt()) nFirstChar = oDictItem.GetInt();
							oDictItem.Free();

							int nLastChar = nFirstChar;
							pFontDict->Search("LastChar", &oDictItem);
							if (oDictItem.IsInt()) nLastChar = oDictItem.GetInt();
							oDictItem.Free();

							Array *pWidths = NULL;
							pFontDict->Search("Widths", &oDictItem);
							if (oDictItem.IsArray()) pWidths = oDictItem.GetArray();

							int nCount = nLastChar - nFirstChar + 1;
							Gr8BitFont *pT1Font = (Gr8BitFont *)pFont;

							if (NULL != pWidths)
							{
								int nWidthsCount = pWidths->GetCount();
								fprintf(pFile, "StartCharMetrics %d\n", nWidthsCount);
								for (int nIndex = 0; nIndex < nWidthsCount; nIndex++)
								{
									int nWidth = nMissingWidth;
									Object oArrayItem;
									pWidths->Get(nIndex, &oArrayItem);
									if (oArrayItem.IsInt()) nWidth = oArrayItem.GetInt();
									oArrayItem.Free();

									char **ppEncoding = pT1Font->GetEncoding();


									if (NULL != ppEncoding && NULL != ppEncoding[nIndex])
										fprintf(pFile, "C %d ; WX %d ; N %s ;\n", nIndex + nFirstChar, nWidth, ppEncoding[nIndex]);
									else
										fprintf(pFile, "C %d ; WX %d ;\n", nIndex + nFirstChar, nWidth);
								}
								fprintf(pFile, "EndCharMetrics\n");
							}
							oDictItem.Free();
						}
					}
					fclose(pFile);
				}

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
			else if (L"" == (wsFileName = pFont->GetExternalFontFilePath()))
			{
				// TODO: Сначала тут мы должны проверить, если ищется один из 14 стандартных шрифтов,
				//       тогда мы должны вернуть путь к стандартному шрифту.

                NSFonts::CFontInfo* pFontInfo = NULL;
				if (m_pFontManager)
				{
					Ref *pRef = pFont->GetID();
					Object oRefObject, oFontObject;
					oRefObject.InitRef(pRef->nNum, pRef->nGen);
					oRefObject.Fetch(m_pXref, &oFontObject);
					oRefObject.Free();

                    NSFonts::CFontSelectFormat oFontSelect;
					std::wstring wsFontBaseName = pFont->GetBaseName()->GetWString();
					if (oFontObject.IsDict())
					{
						Dict *pFontDict = oFontObject.GetDict();
						Object oFontDescriptor;
						if (pFontDict->Search("FontDescriptor", &oFontDescriptor)->IsDict())
						{
							Object oDictItem;
							// FontName
							oFontDescriptor.DictLookup("FontName", &oDictItem);
							if (oDictItem.IsName())
								oFontSelect.wsName = AStringToPWString(oDictItem.GetName());
							else
								oFontSelect.wsName = new std::wstring(wsFontBaseName);
							oDictItem.Free();

							// FontFamily
							oFontDescriptor.DictLookup("FontFamily", &oDictItem);
							oDictItem.Free();

							// FontStretch
							oFontDescriptor.DictLookup("FontStretch", &oDictItem);
							oDictItem.Free();

							// FontWeight
							oFontDescriptor.DictLookup("FontWeight", &oDictItem);
							oDictItem.Free();

							// FontBBox
							oFontDescriptor.DictLookup("FontBBox", &oDictItem);
							oDictItem.Free();

							// ItalicAngle
							oFontDescriptor.DictLookup("ItalicAngle", &oDictItem);
							if (oDictItem.IsInt() && 0 != oDictItem.GetInt()) oFontSelect.bItalic = new INT(1);
							oDictItem.Free();

							// Ascent
							oFontDescriptor.DictLookup("Ascent", &oDictItem);
							if (oDictItem.IsInt()) oFontSelect.shAscent = new SHORT(oDictItem.GetInt());
							oDictItem.Free();

							// Leading
							oFontDescriptor.DictLookup("Leading", &oDictItem);
							if (oDictItem.IsInt()) oFontSelect.shLineGap = new SHORT(oDictItem.GetInt());
							oDictItem.Free();

							// CapHeight
							oFontDescriptor.DictLookup("CapHeight", &oDictItem);
							if (oDictItem.IsInt()) oFontSelect.shCapHeight = new SHORT(oDictItem.GetInt());
							oDictItem.Free();

							// XHeight
							oFontDescriptor.DictLookup("XHeight", &oDictItem);
							if (oDictItem.IsInt()) oFontSelect.shXHeight = new SHORT(oDictItem.GetInt());
							oDictItem.Free();

							// StemV
							oFontDescriptor.DictLookup("StemV", &oDictItem);
							oDictItem.Free();

							// StemH
							oFontDescriptor.DictLookup("StemH", &oDictItem);
							oDictItem.Free();

							// Descent
							oFontDescriptor.DictLookup("Descent", &oDictItem);
							if (oDictItem.IsInt()) oFontSelect.shDescent = new SHORT(oDictItem.GetInt());
							oDictItem.Free();

							// AvgWidth
							oFontDescriptor.DictLookup("AvgWidth", &oDictItem);
							if (oDictItem.IsInt()) oFontSelect.shAvgCharWidth = new SHORT(oDictItem.GetInt());
							oDictItem.Free();

							// MaxWidth
							oFontDescriptor.DictLookup("MaxWidth", &oDictItem);
							oDictItem.Free();

							// MissingWidth
							oFontDescriptor.DictLookup("MissingWidth", &oDictItem);
							oDictItem.Free();

						}
						else
							oFontSelect.wsName = new std::wstring(wsFontBaseName);
						oFontDescriptor.Free();
					}
					else
						oFontSelect.wsName = new std::wstring(wsFontBaseName);

					pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelect);
				}

				if (pFontInfo && L"" != pFontInfo->m_wsFontPath)
				{
					wsFileName = pFontInfo->m_wsFontPath;
					eFontType  = pFont->IsCIDFont() ? fontCIDType2 : fontTrueType;
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
					if (!pFont->IsCIDFont())
					{
						wsExt = L".non";
					}
					else if (pFont->IsCIDFont())
					{
						wsExt = L".cid_non";
					}

					FILE* pTempFile = NULL;
					if (!NSFile::CFileBinary::OpenTempFile(&wsTempFileName, &pTempFile, L"wb", (wchar_t*)wsExt.c_str(), (wchar_t*)m_pGlobalParams->GetTempFolder().c_str(), NULL))
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

						Ref *pRef = pFont->GetID();
						Object oRefObject, oFontObject;
						oRefObject.InitRef(pRef->nNum, pRef->nGen);
						oRefObject.Fetch(m_pXref, &oFontObject);
						oRefObject.Free();

						if (oFontObject.IsDict())
						{
							Dict *pFontDict = oFontObject.GetDict();

							int nEncodingType = -1; // Объекта Encoding нет
							int nBaseEncoding = -1;

							Object oDictItem;
							pFontDict->Search("Encoding", &oDictItem);
							if (oDictItem.IsDict())
							{
								nEncodingType = 1; // Encoding - идет отдельным объектом

								Object oTemp;
								oDictItem.DictLookup("BaseEncoding", &oTemp);

								if (oTemp.IsName("MacRomanEncoding"))
									nBaseEncoding = 0;
								else if (oTemp.IsName("MacExpertEncoding"))
									nBaseEncoding = 1;
								else if (oTemp.IsName("WinAnsiEncoding"))
									nBaseEncoding = 2;

								oTemp.Free();
							}
							else if (oDictItem.IsName("MacRomanEncoding"))
							{
								nEncodingType = 0;
								nBaseEncoding = 0;
							}
							else if (oDictItem.IsName("MacExpertEncoding"))
							{
								nEncodingType = 0;
								nBaseEncoding = 1;
							}
							else if (oDictItem.IsName("WinAnsiEncoding"))
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
							if (oDictItem.IsDict())
							{
								Object oDifferences;
								oDictItem.DictLookup("Differences", &oDifferences);
								if (oDifferences.IsArray())
								{
									int nArrayLen = oDifferences.ArrayGetLength();
									oXmlWriter.WriteNodeBegin(L"Differences", true);
									oXmlWriter.WriteAttribute(L"count", nArrayLen);
									oXmlWriter.WriteNodeEnd(L"Differences", true, false);

									for (int nIndex = 0; nIndex < nArrayLen; ++nIndex)
									{
										Object oTemp;
										oDifferences.ArrayGet(nIndex, &oTemp);
										if (oTemp.IsInt())
										{
											int nCode = oTemp.GetInt();
											oXmlWriter.WriteNodeBegin(L"Code", true);
											oXmlWriter.WriteAttribute(L"value", nCode);
											oXmlWriter.WriteNodeEnd(L"Code", true, true);
										}
										else if (oTemp.IsName())
										{
											char *sName = oTemp.GetName();
											oXmlWriter.WriteNodeBegin(L"Name", true);
											oXmlWriter.WriteAttribute(L"value", AStringToWString(sName));
											oXmlWriter.WriteNodeEnd(L"Name", true, true);
										}
										else
										{
											// TODO: Неправильный тип записи
										}
										oTemp.Free();
									}

									oXmlWriter.WriteNodeEnd(L"Differences");
								}
								oDifferences.Free();
							}
							oDictItem.Free();
							oXmlWriter.WriteNodeEnd(L"Encoding");
						}
						oFontObject.Free();
						oXmlWriter.SaveToFile(wsEncodingPath);
					}
				}
			}

			// Здесь мы грузим кодировки
			unsigned short *pCodeToGID = NULL, *pCodeToUnicode = NULL;
			int nLen = 0;
			CFontFileTrueType *pTTFontFile  = NULL;
			CFontFileType1C   *pT1CFontFile = NULL;
			switch (eFontType)
			{
				case fontType1:
				case fontType1C:
				case fontType1COT:
				{
					if (L"" != wsFileName)
					{
						char **ppEncoding = ((Gr8BitFont *)pFont)->GetEncoding();
						if (!ppEncoding)
							break;

						if (!m_pFontManager)
							break;

						m_pFontManager->LoadFontFromFile(wsFileName, 0, 1, 72, 72);
						pCodeToGID = (unsigned short *)MemUtilsMallocArray(256, sizeof(unsigned short));
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
					if ((pTTFontFile = CFontFileTrueType::LoadFromFile((wchar_t*)wsFileName.c_str())))
					{
						pCodeToGID = ((Gr8BitFont *)pFont)->GetCodeToGIDMap(pTTFontFile);
						nLen = 256;

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
				case fontCIDType0:
				case fontCIDType0C:
				{
					//// TODO: Проверить, почему получение данной кодировки было отключено
					//if ((pT1CFontFile = CFontFileType1C::LoadFromFile((wchar_t*)wsFileName.c_str())))
					//{
					//	pCodeToGID = pT1CFontFile->GetCIDToGIDMap(&nLen);
					//	delete pT1CFontFile;
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
					if ((pTTFontFile = CFontFileTrueType::LoadFromFile((wchar_t*)wsFileName.c_str())))
					{
						if (pTTFontFile->IsOpenTypeCFF())
						{
							pCodeToGID = pTTFontFile->GetCIDToGIDMap(&nLen);
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
					// Если у нас есть мап ToUnicode, тогда на основе его читаем из файла гиды по юникодным значениям, 
					// если не нашли, но у нас есть мап CIDtoGID, тогда строим по последнему.

					pCodeToGID = NULL;
					nLen = 0;
					if (L"" != wsFileName)
					{
						CharCodeToUnicode *pCodeToUnicode = NULL;
						if ((pCodeToUnicode = ((GrCIDFont *)pFont)->GetToUnicode()))
						{
							if ((pTTFontFile = CFontFileTrueType::LoadFromFile((wchar_t*)wsFileName.c_str())))
							{
								// Ищем Unicode Cmap
								CArray<int> arrCMapIndex;
								for (int nCMapIndex = 0; nCMapIndex < pTTFontFile->GetCmapsCount(); ++nCMapIndex)
								{
									if ((pTTFontFile->GetCmapPlatform(nCMapIndex) == 3 && pTTFontFile->GetCmapEncoding(nCMapIndex) == 1) || pTTFontFile->GetCmapPlatform(nCMapIndex) == 0)
									{
										arrCMapIndex.Add(nCMapIndex);
									}
								}
								if (arrCMapIndex.GetSize() > 0)
								{
									// CID -> Unicode -> GID
									nLen = pCodeToUnicode->GetLength();
									pCodeToGID = (unsigned short *)MemUtilsMallocArray(nLen, sizeof(unsigned short));
									for (int nCode = 0; nCode < nLen; ++nCode)
									{
										Unicode arrUnicodeBuffer[8];
										if (pCodeToUnicode->MapToUnicode(nCode, arrUnicodeBuffer, 8) > 0)
										{
											pCodeToGID[nCode] = pTTFontFile->MapCodeToGID(arrCMapIndex[0], arrUnicodeBuffer[0]);
											for (int nIndex = 1; nIndex < arrCMapIndex.GetSize(); nIndex++)
											{
												if (0 == pCodeToGID[nCode])
													pCodeToGID[nCode] = pTTFontFile->MapCodeToGID(arrCMapIndex[nIndex], arrUnicodeBuffer[0]);
												else
													break;
											}
										}
										else
										{
											pCodeToGID[nCode] = nCode;
										}
									}
								}
								delete pTTFontFile;
								pTTFontFile = NULL;
							}
							pCodeToUnicode->Release();
						}
					}

					if (!pCodeToGID && ((GrCIDFont *)pFont)->GetCIDToGID())
					{
						nLen = ((GrCIDFont *)pFont)->GetCIDToGIDLen();
						pCodeToGID = (unsigned short *)MemUtilsMallocArray(nLen, sizeof(unsigned short));
						if (!pCodeToGID)
							break;

						memcpy(pCodeToGID, ((GrCIDFont *)pFont)->GetCIDToGID(), nLen * sizeof(unsigned short));
					}

					break;
				}
				default:
				{
					// Такого не должно произойти
					if (L"" != wsTempFileName)
						NSFile::CFileBinary::Remove(wsTempFileName);

					break;
				}
			}

			// Составляем таблицу Code -> Unicode 
			int nToUnicodeLen = 0;
			if (pFont->IsCIDFont())
			{
				GrCIDFont *pCIDFont = (GrCIDFont *)pFont;
				CharCodeToUnicode *pToUnicode = pCIDFont->GetToUnicode();
				if (NULL != pToUnicode)
				{
					nToUnicodeLen = pToUnicode->GetLength();
					pCodeToUnicode = (unsigned short *)MemUtilsMallocArray(nToUnicodeLen, sizeof(unsigned short));

					if (pCodeToUnicode)
					{
						for (int nIndex = 0; nIndex < nToUnicodeLen; ++nIndex)
						{
							Unicode aUnicode[2];
							if (pToUnicode->MapToUnicode(nIndex, aUnicode, 2))
								pCodeToUnicode[nIndex] = (unsigned short)aUnicode[0];
							else
								pCodeToUnicode[nIndex] = 0;
						}
					}

					pToUnicode->Release();
				}
			}
			else
			{
				CharCodeToUnicode *pToUnicode = ((Gr8BitFont *)pFont)->GetToUnicode();
				if (NULL != pToUnicode)
				{
					nToUnicodeLen = pToUnicode->GetLength();
					pCodeToUnicode = (unsigned short *)MemUtilsMallocArray(nToUnicodeLen, sizeof(unsigned short));

					if (pCodeToUnicode)
					{
						for (int nIndex = 0; nIndex < nToUnicodeLen; ++nIndex)
						{
							Unicode nUnicode = 0;
							if (pToUnicode->MapToUnicode(nIndex, &nUnicode, 1))
								pCodeToUnicode[nIndex] = (unsigned short)nUnicode;
							else
								pCodeToUnicode[nIndex] = nIndex;
						}
					}
					pToUnicode->Release();
				}
			}

			// Записываем файл с настройками шрифта (Специально для перезаписи в PDF)
			if (L"" != wsFileName && c_nPDFWriter == m_lRendererType)
			{			
				std::wstring wsSplitFileName, wsSplitFileExt;
				SpitPathExt(wsFileName, &wsSplitFileName, &wsSplitFileExt);
				std::wstring wsEncodingPath = wsSplitFileName + L".enc";

				GrFontType eFontType = pFont->GetType();
				if (fontType1 == eFontType || fontType1C == eFontType || fontType1COT == eFontType || fontTrueType == eFontType || fontTrueTypeOT == eFontType)
				{
					// Запись информации для простых шрифтов
					CXmlWriter oXmlWriter;

					Ref *pRef = pFont->GetID();
					Object oRefObject, oFontObject;
					oRefObject.InitRef(pRef->nNum, pRef->nGen);
					oRefObject.Fetch(m_pXref, &oFontObject);
					oRefObject.Free();

					if (oFontObject.IsDict())
					{
						Dict *pFontDict = oFontObject.GetDict();

						int nEncodingType = -1; // Объекта Encoding нет
						int nBaseEncoding = -1;

						Object oDictItem;
						pFontDict->Search("Encoding", &oDictItem);
						if (oDictItem.IsDict())
						{
							nEncodingType = 1; // Encoding - идет отдельным объектом

							Object oTemp;
							oDictItem.DictLookup("BaseEncoding", &oTemp);

							if (oTemp.IsName("MacRomanEncoding"))
								nBaseEncoding = 0;
							else if (oTemp.IsName("MacExpertEncoding"))
								nBaseEncoding = 1;
							else if (oTemp.IsName("WinAnsiEncoding"))
								nBaseEncoding = 2;

							oTemp.Free();
						}
						else if (oDictItem.IsName("MacRomanEncoding"))
						{
							nEncodingType = 0;
							nBaseEncoding = 0;
						}
						else if (oDictItem.IsName("MacExpertEncoding"))
						{
							nEncodingType = 0;
							nBaseEncoding = 1;
						}
						else if (oDictItem.IsName("WinAnsiEncoding"))
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
						if (oDictItem.IsDict())
						{
							Object oDifferences;
							oDictItem.DictLookup("Differences", &oDifferences);
							if (oDifferences.IsArray())
							{
								int nArrayLen = oDifferences.ArrayGetLength();
								oXmlWriter.WriteNodeBegin(L"Differences", true);
								oXmlWriter.WriteAttribute(L"count", nArrayLen);
								oXmlWriter.WriteNodeEnd(L"Differences", true, false);

								for (int nIndex = 0; nIndex < nArrayLen; ++nIndex)
								{
									Object oTemp;
									oDifferences.ArrayGet(nIndex, &oTemp);
									if (oTemp.IsInt())
									{
										int nCode = oTemp.GetInt();
										oXmlWriter.WriteNodeBegin(L"Code", true);
										oXmlWriter.WriteAttribute(L"value", nCode);
										oXmlWriter.WriteNodeEnd(L"Code", true, true);
									}
									else if (oTemp.IsName())
									{
										char* sName = oTemp.GetName();
										oXmlWriter.WriteNodeBegin(L"Name", true);
										oXmlWriter.WriteAttribute(L"value", AStringToWString(sName));
										oXmlWriter.WriteNodeEnd(L"Name", true, true);
									}
									else
									{
										// TO DO: Error "Wrong type in font encoding resource differences"
									}
									oTemp.Free();
								}

								oXmlWriter.WriteNodeEnd(L"Differences");
							}
							oDifferences.Free();
						}
						oDictItem.Free();
						oXmlWriter.WriteNodeEnd(L"Encoding");

						pFontDict->Search("BaseFont", &oDictItem);
						if (oDictItem.IsName())
						{
							oXmlWriter.WriteNodeBegin(L"FontBase", true);
							oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.GetName()));
							oXmlWriter.WriteNodeEnd(L"FontBase", true, true);
						}
						oDictItem.Free();

						pFontDict->Search("ToUnicode", &oDictItem);
						if (oDictItem.IsStream())
						{
							oXmlWriter.WriteNodeBegin(L"ToUnicode");

							StringExt *seBuffer = new StringExt();
							if (NULL != seBuffer)
							{
								oDictItem.StreamReset();
								int nChar = 0;
								while ((nChar = oDictItem.StreamGetChar()) != EOF)
								{
									seBuffer->Append(nChar);
								}
								oDictItem.StreamClose();

								CBase64 oBase64;
								oBase64.Encode((unsigned char *)seBuffer->GetBuffer(), seBuffer->GetLength());
								oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));

								delete seBuffer;
							}

							oXmlWriter.WriteNodeEnd(L"ToUnicode");
						}
						oDictItem.Free();


						oXmlWriter.WriteNodeBegin(L"FontDescriptor");

						Object oFontDescriptor;
						if (pFontDict->Search("FontDescriptor", &oFontDescriptor)->IsDict())
						{
							// FontName
							oFontDescriptor.DictLookup("FontName", &oDictItem);
							if (oDictItem.IsName())
							{
								oXmlWriter.WriteNodeBegin(L"FontName", true);
								oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.GetName()));
								oXmlWriter.WriteNodeEnd(L"FontName", true);
							}
							oDictItem.Free();

							// FontFamily
							oFontDescriptor.DictLookup("FontFamily", &oDictItem);
							if (oDictItem.IsName())
							{
								oXmlWriter.WriteNodeBegin(L"FontFamily", true);
								oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.GetName()));
								oXmlWriter.WriteNodeEnd(L"FontFamily", true);
							}
							oDictItem.Free();

							// FontStretch
							oFontDescriptor.DictLookup("FontStretch", &oDictItem);
							if (oDictItem.IsName())
							{
								oXmlWriter.WriteNodeBegin(L"FontStretch", true);
								oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.GetName()));
								oXmlWriter.WriteNodeEnd(L"FontStretch", true);
							}
							oDictItem.Free();

							// FontWeight
							oFontDescriptor.DictLookup("FontWeight", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"FontWeight", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"FontWeight", true);
							}
							oDictItem.Free();

							// FontWeight
							oFontDescriptor.DictLookup("Flags", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"Flags", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"Flags", true);
							}
							oDictItem.Free();

							// FontBBox
							oFontDescriptor.DictLookup("FontBBox", &oDictItem);
							if (oDictItem.IsArray())
							{
								Array *pBBox = oDictItem.GetArray();
								if (NULL != pBBox && 4 == pBBox->GetCount())
								{
									oXmlWriter.WriteNodeBegin(L"FontBBox", true);
									for (int nIndex = 0; nIndex < 4; nIndex++)
									{
										Object oArrayItem;
										pBBox->Get(nIndex, &oArrayItem);
										if (oArrayItem.IsInt())
										{
											std::wstring wsValue = L"value" + std::to_wstring(nIndex);
											oXmlWriter.WriteAttribute(wsValue, oArrayItem.GetInt());
										}

										oArrayItem.Free();
									}
									oXmlWriter.WriteNodeEnd(L"FontBBox", true);
								}
							}
							oDictItem.Free();

							// ItalicAngle
							oFontDescriptor.DictLookup("ItalicAngle", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"ItalicAngle", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"ItalicAngle", true);
							}
							oDictItem.Free();

							// Ascent
							oFontDescriptor.DictLookup("Ascent", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"Ascent", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"Ascent", true);
							}
							oDictItem.Free();

							// Leading
							oFontDescriptor.DictLookup("Leading", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"Leading", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"Leading", true);
							}
							oDictItem.Free();

							// CapHeight
							oFontDescriptor.DictLookup("CapHeight", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"CapHeight", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"CapHeight", true);
							}
							oDictItem.Free();

							// XHeight
							oFontDescriptor.DictLookup("XHeight", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"XHeight", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"XHeight", true);
							}
							oDictItem.Free();

							// StemV
							oFontDescriptor.DictLookup("StemV", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"StemV", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"StemV", true);
							}
							oDictItem.Free();

							// StemH
							oFontDescriptor.DictLookup("StemH", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"StemH", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"StemH", true);
							}
							oDictItem.Free();

							// Descent
							oFontDescriptor.DictLookup("Descent", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"Descent", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"Descent", true);
							}
							oDictItem.Free();

							// AvgWidth
							oFontDescriptor.DictLookup("AvgWidth", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"AvgWidth", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"AvgWidth", true);
							}
							oDictItem.Free();

							// MaxWidth
							oFontDescriptor.DictLookup("MaxWidth", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"MaxWidth", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"MaxWidth", true);
							}
							oDictItem.Free();

							// MissingWidth
							oFontDescriptor.DictLookup("MissingWidth", &oDictItem);
							if (oDictItem.IsInt())
							{
								oXmlWriter.WriteNodeBegin(L"MissingWidth", true);
								oXmlWriter.WriteAttribute(L"value", oDictItem.GetInt());
								oXmlWriter.WriteNodeEnd(L"MissingWidth", true);
							}
							oDictItem.Free();

						}
						oFontDescriptor.Free();
						oXmlWriter.WriteNodeEnd(L"FontDescriptor");

						int nFirstChar = 0;
						pFontDict->Search("FirstChar", &oDictItem);
						if (oDictItem.IsInt()) nFirstChar = oDictItem.GetInt();
						oDictItem.Free();

						int nLastChar = 0;
						pFontDict->Search("LastChar", &oDictItem);
						if (oDictItem.IsInt()) nLastChar = oDictItem.GetInt();
						oDictItem.Free();

						Array *pWidths = NULL;
						pFontDict->Search("Widths", &oDictItem);
						if (oDictItem.IsArray())
						{
							oXmlWriter.WriteNodeBegin(L"Widths", true);
							oXmlWriter.WriteAttribute(L"FirstChar", nFirstChar);
							oXmlWriter.WriteAttribute(L"LastChar", nLastChar);
							oXmlWriter.WriteNodeEnd(L"Widths", true, false);

							Array *pWidths = oDictItem.GetArray();
							int nWidthsCount = pWidths->GetCount();
							for (int nIndex = 0; nIndex < nWidthsCount; nIndex++)
							{
								Object oArrayItem;
								pWidths->Get(nIndex, &oArrayItem);
								if (oArrayItem.IsInt())
								{
									oXmlWriter.WriteNodeBegin(L"Width", true);
									oXmlWriter.WriteAttribute(L"value", oArrayItem.GetInt());
									oXmlWriter.WriteNodeEnd(L"Width", true);
								}
								oArrayItem.Free();
							}
							oXmlWriter.WriteNodeEnd(L"Widths");
						}
						oDictItem.Free();
						oXmlWriter.WriteNodeEnd(L"PDF-resources");
					}
					oFontObject.Free();
					oXmlWriter.SaveToFile(wsEncodingPath);
				}
				else if (fontCIDType0 == eFontType || fontCIDType0C == eFontType || fontCIDType0COT == eFontType || fontCIDType2 == eFontType || fontCIDType2OT == eFontType)
				{
					// Пишем файл с кодировкой CMap
					std::wstring wsCMapPath = wsSplitFileName + L".cmap";					
					if (pFont->IsCIDFont())
					{
						GrCIDFont *pCIDFont = (GrCIDFont *)pFont;
						if (NULL != pCIDFont->GetCMap())
							pCIDFont->GetCMap()->ToXml(wsCMapPath);
					}

					CXmlWriter oXmlWriter;
					Ref *pRef = pFont->GetID();
					Object oRefObject, oFontObject;
					oRefObject.InitRef(pRef->nNum, pRef->nGen);
					oRefObject.Fetch(m_pXref, &oFontObject);
					oRefObject.Free();
					if (oFontObject.IsDict())
					{
						Dict *pFontDict = oFontObject.GetDict();
						oXmlWriter.WriteNodeBegin(L"PDF-resources");
						Object oDictItem;
						pFontDict->Search("BaseFont", &oDictItem);
						if (oDictItem.IsName())
						{
							oXmlWriter.WriteNodeBegin(L"Type0", true);
							oXmlWriter.WriteAttribute(L"value", AStringToWString(oDictItem.GetName()));
							oXmlWriter.WriteNodeEnd(L"Type0", true, false);
						}
						else
						{
							oXmlWriter.WriteNodeBegin(L"Type0");
						}
						oDictItem.Free();

						pFontDict->Search("ToUnicode", &oDictItem);
						if (oDictItem.IsStream())
						{
							oXmlWriter.WriteNodeBegin(L"ToUnicode");

							StringExt *seBuffer = new StringExt();
							oDictItem.StreamReset();
							int nChar = 0;
							while ((nChar = oDictItem.StreamGetChar()) != EOF)
							{
								seBuffer->Append(nChar);
							}
							oDictItem.StreamClose();

							CBase64 oBase64;
							oBase64.Encode((unsigned char *)seBuffer->GetBuffer(), seBuffer->GetLength());
							oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));
							oXmlWriter.WriteNodeEnd(L"ToUnicode");
							
							delete seBuffer;
						}
						oDictItem.Free();

						pFontDict->Search("Encoding", &oDictItem);
						if (oDictItem.IsName())
						{
							oXmlWriter.WriteNodeBegin(L"Encoding", true);
							oXmlWriter.WriteAttribute(L"name", AStringToWString(oDictItem.GetName()));
							oXmlWriter.WriteNodeEnd(L"Encoding", true, true);
						}
						else if (oDictItem.IsStream())
						{
							oXmlWriter.WriteNodeBegin(L"Encoding");

							Dict *pEncodingDict = oDictItem.StreamGetDict();
							if (NULL != pEncodingDict)
							{
								Object oEncItem;
								pEncodingDict->Search("CMapName", &oEncItem);
								if (oEncItem.IsName())
								{
									oXmlWriter.WriteNodeBegin(L"CMapName", true);
									oXmlWriter.WriteAttribute(L"name", AStringToWString(oEncItem.GetName()));
									oXmlWriter.WriteNodeEnd(L"CMapName", true, true);
								}
								oEncItem.Free();

								pEncodingDict->Search("CIDSystemInfo", &oEncItem);
								if (oEncItem.IsDict())
								{
									Dict *pCIDInfo = oEncItem.GetDict();

									if (NULL != pCIDInfo)
									{
										oXmlWriter.WriteNodeBegin(L"CIDSystemInfo");

										Object oCIDInfoItem;
										pCIDInfo->Search("Registry", &oCIDInfoItem);
										if (oCIDInfoItem.IsString())
										{
											oXmlWriter.WriteNodeBegin(L"Registry", true);
											oXmlWriter.WriteAttribute(L"string", AStringToWString(oCIDInfoItem.GetString()->GetBuffer()));
											oXmlWriter.WriteNodeEnd(L"Registry", true, true);
										}
										oCIDInfoItem.Free();

										pCIDInfo->Search("Ordering", &oCIDInfoItem);
										if (oCIDInfoItem.IsString())
										{
											oXmlWriter.WriteNodeBegin(L"Ordering", true);
											oXmlWriter.WriteAttribute(L"string", AStringToWString(oCIDInfoItem.GetString()->GetBuffer()));
											oXmlWriter.WriteNodeEnd(L"Ordering", true, true);
										}
										oCIDInfoItem.Free();

										pCIDInfo->Search("Supplement", &oCIDInfoItem);
										if (oCIDInfoItem.IsInt())
										{
											oXmlWriter.WriteNodeBegin(L"Supplement", true);
											oXmlWriter.WriteAttribute(L"integer", oCIDInfoItem.GetInt());
											oXmlWriter.WriteNodeEnd(L"Supplement", true, true);
										}
										oCIDInfoItem.Free();

										oXmlWriter.WriteNodeEnd(L"CIDSystemInfo");
									}
								}
								oEncItem.Free();

								pEncodingDict->Search("WMode", &oEncItem);
								if (oEncItem.IsInt())
								{
									oXmlWriter.WriteNodeBegin(L"WMode", true);
									oXmlWriter.WriteAttribute(L"integer", oEncItem.GetInt());
									oXmlWriter.WriteNodeEnd(L"WMode", true, true);
								}
								oEncItem.Free();

								pEncodingDict->Search("UseCMap", &oEncItem);
								if (oEncItem.IsName())
								{
									oXmlWriter.WriteNodeBegin(L"UseCMap", true);
									oXmlWriter.WriteAttribute(L"name", AStringToWString(oEncItem.GetName()));
									oXmlWriter.WriteNodeEnd(L"UseCMap", true, true);
								}
								else if (oEncItem.IsStream())
								{
									oXmlWriter.WriteNodeBegin(L"UseCMap");

									StringExt *seBuffer = new StringExt();
									oEncItem.StreamReset();
									int nChar = 0;
									while ((nChar = oEncItem.StreamGetChar()) != EOF)
									{
										seBuffer->Append(nChar);
									}
									oEncItem.StreamClose();

									CBase64 oBase64;
									oBase64.Encode((unsigned char *)seBuffer->GetBuffer(), seBuffer->GetLength());
									oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));

									oXmlWriter.WriteNodeEnd(L"UseCMap");
									
									delete seBuffer;
								}
								oEncItem.Free();
							}

							oXmlWriter.WriteNodeBegin(L"Stream");
							StringExt *seBuffer = new StringExt();
							oDictItem.StreamReset();
							int nChar = 0;
							while ((nChar = oDictItem.StreamGetChar()) != EOF)
							{
								seBuffer->Append(nChar);
							}
							oDictItem.StreamClose();
							CBase64 oBase64;
							oBase64.Encode((unsigned char *)seBuffer->GetBuffer(), seBuffer->GetLength());
							oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));
							oXmlWriter.WriteNodeEnd(L"Stream");
							oXmlWriter.WriteNodeEnd(L"Encoding");
							
							delete seBuffer;
						}
						oDictItem.Free();

						pFontDict->Search("DescendantFonts", &oDictItem);
						if (oDictItem.IsArray())
						{
							Array *pArray =  oDictItem.GetArray();
							if (1 == pArray->GetCount())
							{
								Object oDescFont;
								pArray->Get(0, &oDescFont);

								if (oDescFont.IsDict())
								{
									Dict *pDescFont = oDescFont.GetDict();
									if (NULL != pDescFont)
									{
										oXmlWriter.WriteNodeBegin(L"DescendantFonts");

										Object oFontItem;
										pDescFont->Search("Subtype", &oFontItem);
										if (oFontItem.IsName())
										{
											oXmlWriter.WriteNodeBegin(L"Subtype", true);
											oXmlWriter.WriteAttribute(L"name", AStringToWString(oFontItem.GetName()));
											oXmlWriter.WriteNodeEnd(L"Subtype", true, true);
										}
										oFontItem.Free();

										pDescFont->Search("BaseFont", &oFontItem);
										if (oFontItem.IsName())
										{
											oXmlWriter.WriteNodeBegin(L"BaseFont", true);
											oXmlWriter.WriteAttribute(L"name", AStringToWString(oFontItem.GetName()));
											oXmlWriter.WriteNodeEnd(L"BaseFont", true, true);
										}
										oFontItem.Free();

										pDescFont->Search("CIDSystemInfo", &oFontItem);
										if (oFontItem.IsDict())
										{
											Dict *pCIDInfo = oFontItem.GetDict();
											if (NULL != pCIDInfo)
											{
												oXmlWriter.WriteNodeBegin(L"CIDSystemInfo");

												Object oCIDInfoItem;
												pCIDInfo->Search("Registry", &oCIDInfoItem);
												if (oCIDInfoItem.IsString())
												{
													oXmlWriter.WriteNodeBegin(L"Registry", true);
													oXmlWriter.WriteAttribute(L"string", AStringToWString(oCIDInfoItem.GetString()->GetBuffer()));
													oXmlWriter.WriteNodeEnd(L"Registry", true, true);
												}
												oCIDInfoItem.Free();

												pCIDInfo->Search("Ordering", &oCIDInfoItem);
												if (oCIDInfoItem.IsString())
												{
													oXmlWriter.WriteNodeBegin(L"Ordering", true);
													oXmlWriter.WriteAttribute(L"string", AStringToWString(oCIDInfoItem.GetString()->GetBuffer()));
													oXmlWriter.WriteNodeEnd(L"Ordering", true, true);
												}
												oCIDInfoItem.Free();

												pCIDInfo->Search("Supplement", &oCIDInfoItem);
												if (oCIDInfoItem.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"Supplement", true);
													oXmlWriter.WriteAttribute(L"integer", oCIDInfoItem.GetInt());
													oXmlWriter.WriteNodeEnd(L"Supplement", true, true);
												}
												oCIDInfoItem.Free();

												oXmlWriter.WriteNodeEnd(L"CIDSystemInfo");
											}
										}
										oFontItem.Free();

										pDescFont->Search("FontDescriptor", &oFontItem);
										if (oFontItem.IsDict())
										{
											Dict *pFontDescriptor = oFontItem.GetDict();
											if (NULL != pFontDescriptor)
											{
												oXmlWriter.WriteNodeBegin(L"FontDescriptor");
												Object oItemFD;

												pFontDescriptor->Search("FontName", &oItemFD);
												if (oItemFD.IsName())
												{
													oXmlWriter.WriteNodeBegin(L"FontName", true);
													oXmlWriter.WriteAttribute(L"name", AStringToWString(oItemFD.GetName()));
													oXmlWriter.WriteNodeEnd(L"FontName", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("FontStretch", &oItemFD);
												if (oItemFD.IsName())
												{
													oXmlWriter.WriteNodeBegin(L"FontStretch", true);
													oXmlWriter.WriteAttribute(L"name", AStringToWString(oItemFD.GetName()));
													oXmlWriter.WriteNodeEnd(L"FontStretch", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("FontWeight", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"FontWeight", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"FontWeight", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("Flags", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"Flags", true);
													oXmlWriter.WriteAttribute(L"integer", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"Flags", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("FontBBox", &oItemFD);
												if (oItemFD.IsArray())
												{
													Array *pBBox = oItemFD.GetArray();
													if (NULL != pBBox && 4 == pBBox->GetCount())
													{
														oXmlWriter.WriteNodeBegin(L"FontBBox", true);
														for (int nIndex = 0; nIndex < 4; nIndex++)
														{
															Object oArrayItem;
															pBBox->Get(nIndex, &oArrayItem);
															if (oArrayItem.IsInt())
															{
																std::wstring wsValue = L"value" + std::to_wstring(nIndex);
																oXmlWriter.WriteAttribute(wsValue, oArrayItem.GetInt());
															}

															oArrayItem.Free();
														}
														oXmlWriter.WriteNodeEnd(L"FontBBox", true);
													}
												}
												oItemFD.Free();

												pFontDescriptor->Search("ItalicAngle", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"ItalicAngle", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"ItalicAngle", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("Ascent", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"Ascent", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"Ascent", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("Descent", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"Descent", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"Descent", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("Leading", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"Leading", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"Leading", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("CapHeight", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"CapHeight", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"CapHeight", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("XHeight", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"XHeight", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"XHeight", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("StemV", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"StemV", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"StemV", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("StemH", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"StemH", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"StemH", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("AvgWidth", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"AvgWidth", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"AvgWidth", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("MaxWidth", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"MaxWidth", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"MaxWidth", true, true);
												}
												oItemFD.Free();

												pFontDescriptor->Search("MissingWidth", &oItemFD);
												if (oItemFD.IsInt())
												{
													oXmlWriter.WriteNodeBegin(L"MissingWidth", true);
													oXmlWriter.WriteAttribute(L"number", oItemFD.GetInt());
													oXmlWriter.WriteNodeEnd(L"MissingWidth", true, true);
												}
												oItemFD.Free();

												// TODO: Тут надо реализовать чтени полей /Style, /Lang, /FD, /CIDSet

												oXmlWriter.WriteNodeEnd(L"FontDescriptor");
											}
										}
										oFontItem.Free();

										pDescFont->Search("DW", &oFontItem);
										if (oFontItem.IsInt())
										{
											oXmlWriter.WriteNodeBegin(L"DW", true);
											oXmlWriter.WriteAttribute(L"integer", oFontItem.GetInt());
											oXmlWriter.WriteNodeEnd(L"DW", true, true);
										}
										oFontItem.Free();

										pDescFont->Search("W", &oFontItem);
										if (oFontItem.IsArray())
										{
											Array *pArray = oFontItem.GetArray();
											if (NULL != pArray)
											{
												oXmlWriter.WriteNodeBegin(L"W");

												for (int nIndex = 0; nIndex < pArray->GetCount(); nIndex++)
												{
													Object oArrayItem;
													pArray->Get(nIndex, &oArrayItem);
													if (oArrayItem.IsInt())
													{
														oXmlWriter.WriteNodeBegin(L"Int", true);
														oXmlWriter.WriteAttribute(L"value", oArrayItem.GetInt());
														oXmlWriter.WriteNodeEnd(L"Int", true, true);
													}
													else if (oArrayItem.IsArray())
													{
														Array *pWArray = oArrayItem.GetArray();
														if (NULL != pWArray)
														{
															oXmlWriter.WriteNodeBegin(L"Array");
															for (int nWIndex = 0; nWIndex < pWArray->GetCount(); nWIndex++)
															{
																Object oWArrayItem;
																pWArray->Get(nWIndex, &oWArrayItem);
																if (oWArrayItem.IsInt())
																{
																	oXmlWriter.WriteNodeBegin(L"Int", true);
																	oXmlWriter.WriteAttribute(L"value", oWArrayItem.GetInt());
																	oXmlWriter.WriteNodeEnd(L"Int", true, true);
																}
																oWArrayItem.Free();
															}
															oXmlWriter.WriteNodeEnd(L"Array");
														}
													}
													oArrayItem.Free();
												}

												oXmlWriter.WriteNodeEnd(L"W");
											}
										}
										oFontItem.Free();

										pDescFont->Search("DW2", &oFontItem);
										if (oFontItem.IsArray())
										{
											Array *pArray = oFontItem.GetArray();
											if (NULL != pArray && 2 == pArray->GetCount())
											{
												oXmlWriter.WriteNodeBegin(L"DW2", true);

												Object oArrayItem;
												pArray->Get(0, &oArrayItem);
												if (oArrayItem.IsInt())
												{
													oXmlWriter.WriteAttribute(L"value0", oArrayItem.GetInt());
												}
												oArrayItem.Free();

												pArray->Get(1, &oArrayItem);
												if (oArrayItem.IsInt())
												{
													oXmlWriter.WriteAttribute(L"value1", oArrayItem.GetInt());
												}
												oArrayItem.Free();

												oXmlWriter.WriteNodeEnd(L"DW2", true, true);
											}
										}
										oFontItem.Free();

										pDescFont->Search("W2", &oFontItem);
										if (oFontItem.IsArray())
										{
											Array *pArray = oFontItem.GetArray();
											if (NULL != pArray)
											{
												oXmlWriter.WriteNodeBegin(L"W2");

												for (int nIndex = 0; nIndex < pArray->GetCount(); nIndex++)
												{
													Object oArrayItem;
													pArray->Get(nIndex, &oArrayItem);
													if (oArrayItem.IsInt())
													{
														oXmlWriter.WriteNodeBegin(L"Int", true);
														oXmlWriter.WriteAttribute(L"value", oArrayItem.GetInt());
														oXmlWriter.WriteNodeEnd(L"Int", true, true);
													}
													else if (oArrayItem.IsArray())
													{
														Array *pWArray = oArrayItem.GetArray();
														if (NULL != pWArray)
														{
															oXmlWriter.WriteNodeBegin(L"Array");
															for (int nWIndex = 0; nWIndex < pWArray->GetCount(); nWIndex++)
															{
																Object oWArrayItem;
																pWArray->Get(nWIndex, &oWArrayItem);
																if (oWArrayItem.IsInt())
																{
																	oXmlWriter.WriteNodeBegin(L"Int", true);
																	oXmlWriter.WriteAttribute(L"value", oWArrayItem.GetInt());
																	oXmlWriter.WriteNodeEnd(L"Int", true, true);
																}
																oWArrayItem.Free();
															}
															oXmlWriter.WriteNodeEnd(L"Array");
														}
													}
													oArrayItem.Free();
												}

												oXmlWriter.WriteNodeEnd(L"W2");
											}
										}
										oFontItem.Free();

										pDescFont->Search("CIDToGIDMap", &oFontItem);
										if (oFontItem.IsName())
										{
											oXmlWriter.WriteNodeBegin(L"CIDToGIDMap", true);
											oXmlWriter.WriteAttribute(L"name", AStringToWString(oFontItem.GetName()));
											oXmlWriter.WriteNodeEnd(L"CIDToGIDMap", true, true);
										}
										else if (oFontItem.IsStream())
										{
											oXmlWriter.WriteNodeBegin(L"CIDToGIDMap");

											StringExt *seBuffer = new StringExt();
											oFontItem.StreamReset();
											int nChar = 0;
											while ((nChar = oFontItem.StreamGetChar()) != EOF)
											{
												seBuffer->Append(nChar);
											}
											oFontItem.StreamClose();

											CBase64 oBase64;
											oBase64.Encode((unsigned char *)seBuffer->GetBuffer(), seBuffer->GetLength());
											oXmlWriter.WriteString(AStringToWString(oBase64.GetCString()));

											oXmlWriter.WriteNodeEnd(L"CIDToGIDMap");
											
											delete seBuffer;
										}
										oFontItem.Free();


										oXmlWriter.WriteNodeEnd(L"DescendantFonts");
									}
								}
								oDescFont.Free();
							}
						}
						oDictItem.Free();

						oXmlWriter.WriteNodeEnd(L"Type0");
						oXmlWriter.WriteNodeEnd(L"PDF-resources");
					}
					oFontObject.Free();

					oXmlWriter.SaveToFile(wsEncodingPath);
				}
			}

			// Обрежем индекс у FontName, если он есть
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
	void RendererOutputDev::Stroke(GrState *pGState)
	{
        if (m_bDrawOnlyText)
            return;

		if (m_bTransparentGroup)
			return;

		DoPath(pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM());
		m_pRenderer->DrawPath(c_nStroke);

		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::Fill(GrState *pGState)
	{
        if (m_bDrawOnlyText)
            return;

		if (m_bTransparentGroup)
			return;

		DoPath(pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM());
		m_pRenderer->DrawPath(c_nWindingFillMode);

		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::EoFill(GrState *pGState)
	{
        if (m_bDrawOnlyText)
            return;

		if (m_bTransparentGroup)
			return;

		DoPath(pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM());
		m_pRenderer->DrawPath(c_nEvenOddFillMode);

		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::FillStroke(GrState *pGState)
	{
        if (m_bDrawOnlyText)
            return;

		if (m_bTransparentGroup)
			return;

		DoPath(pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM());
		m_pRenderer->DrawPath(c_nStroke | c_nWindingFillMode);

		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::EoFillStroke(GrState *pGState)
	{
        if (m_bDrawOnlyText)
            return;

		if (m_bTransparentGroup)
			return;

		DoPath(pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM());
		m_pRenderer->DrawPath(c_nStroke | c_nEvenOddFillMode);

		m_pRenderer->EndCommand(c_nPathType);
	}
	void RendererOutputDev::TilingPatternFill(GrState *pGState, Object *pStream, int nPaintType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep)
	{

	}
	void RendererOutputDev::StartTilingFill(GrState *pGState)
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
	bool RendererOutputDev::FunctionShadedFill(GrState *pGState, GrFunctionShading *pShading)
	{
		return false;
	}
	bool RendererOutputDev::AxialShadedFill(GrState *pGState, GrAxialShading    *pShading)
	{
		// TODO: Реализовать линейный градиент
		return false;
		//m_pRenderer->BeginCommand(c_nPDFGradientType);

		//double arrMatrix[6];
		//double *pCTM = pGState->GetCTM();
		//arrMatrix[0] =  pCTM[0];
		//arrMatrix[1] = -pCTM[1];
		//arrMatrix[2] =  pCTM[2];
		//arrMatrix[3] = -pCTM[3];
		//arrMatrix[4] =  pCTM[4];
		//arrMatrix[5] = -pCTM[5] + pGState->GetPageHeight();

		//double dShiftX = 0, dShiftY = 0;
		//DoTransform(arrMatrix, &dShiftX, &dShiftY);

		//double dFirstX, dFirstY, dSecondX, dSecondY;
		//pShading->GetCoords(&dFirstX, &dFirstY, &dSecondX, &dSecondY);

		//bool bExtendFirst  = pShading->GetExtendStart();
		//bool bExtendSecond = pShading->GetExtendEnd();

		//GrColorSpace *pColorSpace = pGState->GetFillColorSpace();

		//LONG lOldType;
		//m_pRenderer->get_BrushType(&lOldType);

		////m_pRenderer->put_B
		//NSStructures::CBrush oBrush;
		//oBrush.Rect.X      = PDFCoordsToMM(dFirstX + dShiftX);
		//oBrush.Rect.Y      = PDFCoordsToMM(dFirstY + dShiftY);
		//oBrush.Rect.Width  = PDFCoordsToMM(dSecondX - dFirstX);
		//oBrush.Rect.Height = PDFCoordsToMM(dSecondY - dFirstY);
		//oBrush.Type        = 2;
		//oBrush.TextureMode = bExtendFirst ? 1 : 0 + bExtendSecond ? 2 : 0;

		//BSTR bsXml = oBrush.ToXmlString().AllocSysString();
		//m_pRenderer->SetBrush(bsXml);
		//::SysFreeString(bsXml);

		//double dXMin, dYMin, dXMax, dYMax;
		//pGState->GetUserClipBBox(&dXMin, &dYMin, &dXMax, &dYMax);

		//oBrush.Type        = 2;
		//oBrush.TextureMode = 5;

		//m_pRenderer->BrushBounds(PDFCoordsToMM(dXMin + dShiftX), PDFCoordsToMM(dYMin + dShiftY), PDFCoordsToMM(dXMax - dXMin), PDFCoordsToMM(dYMax - dYMin));
		//Aggplus::BrushType::
		//m_pRenderer->put_BrushType(2);
		//m_pRenderer->put_BrushTextureMode(Aggplus::);

		//bsXml = oBrush.ToXmlString().AllocSysString();
		//m_pRenderer->SetBrush(bsXml);
		//::SysFreeString(bsXml);

		//// Присылаем 16 цветов
		//double dT0 = pShading->GetDomain0();
		//double dT1 = pShading->GetDomain1();
		//GrColor oColor;
		//for (int nIndex = 0; nIndex < 16; nIndex++)
		//{
		//	double dT = dT0 + nIndex * (dT1 - dT0) / 15;
		//	pShading->GetColor(dT, &oColor);

		//	oBrush.Color1      = pColorSpace->GetDwordColor(&oColor);
		//	oBrush.Type        = 2;
		//	oBrush.TextureMode = 10;

		//	bsXml = oBrush.ToXmlString().AllocSysString();
		//	m_pRenderer->SetBrush(bsXml);
		//	::SysFreeString(bsXml);
		//}

		//m_pRenderer->EndCommand(c_nPDFGradientType);

		//UpdateFillColor(pGState);
		//UpdateFillOpacity(pGState);

		//m_pRenderer->put_BrushType(lOldType);

		//return true;
	}
	bool RendererOutputDev::RadialShadedFill(GrState *pGState, GrRadialShading   *pShading)
	{
		// TODO: Реализовать радиальный градиент
		return false;

		//m_pRenderer->BeginCommand(c_nPDFGradientType);

		//double arrMatrix[6];
		//double *pCTM = pGState->GetCTM();
		//arrMatrix[0] =  pCTM[0];
		//arrMatrix[1] = -pCTM[1];
		//arrMatrix[2] =  pCTM[2];
		//arrMatrix[3] = -pCTM[3];
		//arrMatrix[4] =  pCTM[4];
		//arrMatrix[5] = -pCTM[5] + pGState->GetPageHeight();

		//double dShiftX = 0, dShiftY = 0;
		//DoTransform(arrMatrix, &dShiftX, &dShiftY);

		//double dFirstX, dFirstY, dFirstRad, dSecondX, dSecondY, dSecondRad;
		//pShading->GetCoords(&dFirstX, &dFirstY, &dFirstRad, &dSecondX, &dSecondY, &dSecondRad);

		//bool bExtendFirst  = pShading->GetExtendFirst();
		//bool bExtendSecond = pShading->GetExtendSecond();

		//LONG lOldType;
		//m_pRenderer->get_BrushType(&lOldType);

		//NSStructures::CBrush oBrush;
		//oBrush.Rect.X      = PDFCoordsToMM(dFirstX + dShiftX);
		//oBrush.Rect.Y      = PDFCoordsToMM(dFirstY + dShiftY);
		//oBrush.Rect.Width  = PDFCoordsToMM(dFirstRad);
		//oBrush.Rect.Height = PDFCoordsToMM(dFirstRad);
		//oBrush.Type        = 1;
		//oBrush.TextureMode = bExtendFirst ? 1 : 0;

		//BSTR bsXml = oBrush.ToXmlString().AllocSysString();
		//m_pRenderer->SetBrush(bsXml);
		//::SysFreeString(bsXml);

		//oBrush.Rect.X      = PDFCoordsToMM(dSecondX + dShiftX);
		//oBrush.Rect.Y      = PDFCoordsToMM(dSecondY + dShiftY);
		//oBrush.Rect.Width  = PDFCoordsToMM(dSecondRad);
		//oBrush.Rect.Height = PDFCoordsToMM(dSecondRad);
		//oBrush.Type        = 1;
		//oBrush.TextureMode = bExtendSecond ? 1 : 0;

		//bsXml = oBrush.ToXmlString().AllocSysString();
		//m_pRenderer->SetBrush(bsXml);
		//::SysFreeString(bsXml);

		//double dXMin, dYMin, dXMax, dYMax;
		//pGState->GetUserClipBBox(&dXMin, &dYMin, &dXMax, &dYMax);

		//oBrush.Rect.X      = PDFCoordsToMM(dXMin + dShiftX);
		//oBrush.Rect.Y      = PDFCoordsToMM(dYMin + dShiftY);
		//oBrush.Rect.Width  = PDFCoordsToMM(dXMax - dXMin);
		//oBrush.Rect.Height = PDFCoordsToMM(dYMax - dYMin);
		//oBrush.Type        = 1;
		//oBrush.TextureMode = 5;

		//bsXml = oBrush.ToXmlString().AllocSysString();
		//m_pRenderer->SetBrush(bsXml);
		//::SysFreeString(bsXml);

		//// Присылаем 15 цветов
		//double dT0 = pShading->GetDomain0();
		//double dT1 = pShading->GetDomain1();
		//GrColor oColor;
		//GrColorSpace *pColorSpace = pGState->GetFillColorSpace();
		//for (int nIndex = 0; nIndex < 15; nIndex++)
		//{
		//	double dT = dT0 + nIndex * (dT1 - dT0) / 14;
		//	pShading->GetColor(dT, &oColor);

		//	oBrush.Color1      = pColorSpace->GetDwordColor(&oColor);
		//	oBrush.Type        = 1;
		//	oBrush.TextureMode = 10;

		//	bsXml = oBrush.ToXmlString().AllocSysString();
		//	m_pRenderer->SetBrush(bsXml);
		//	::SysFreeString(bsXml);
		//}

		//m_pRenderer->EndCommand(c_nPDFGradientType);

		//UpdateFillColor(pGState);
		//UpdateFillOpacity(pGState);

		//m_pRenderer->put_BrushType(lOldType);

		//return true;
	}
	void RendererOutputDev::StartShadedFill(GrState *pGState)
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
	void RendererOutputDev::StartSimpleTilingFill(GrState *pGState, int  nX0, int nY0, int nX1, int nY1, double dStepX, double dStepY, double dXMin, double dYMin, double dXMax, double dYMax, double* pMatrix)
	{
        if (m_bDrawOnlyText)
            return;

		this->ClipAttack(pGState);

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
	void RendererOutputDev::Clip(GrState *pGState)
	{
        if (m_bDrawOnlyText)
            return;

		UpdateClip(pGState);
	}
	void RendererOutputDev::EoClip(GrState *pGState)
	{
        if (m_bDrawOnlyText)
            return;

		UpdateClip(pGState);
	}
	void RendererOutputDev::ClipToStrokePath(GrState *pGState)
	{
        if (m_bDrawOnlyText)
            return;

		UpdateClip(pGState);
	}
	void RendererOutputDev::ClipToPath(GrState *pGState, GrPath *pPath, double *pMatrix, bool bEO)
	{
        if (m_bDrawOnlyText)
            return;

		if (m_bTransparentGroup)
			return;

		int nClipFlag = bEO ? c_nClipRegionTypeEvenOdd : c_nClipRegionTypeWinding;
		nClipFlag |= c_nClipRegionIntersect;

		m_pRenderer->BeginCommand(c_nClipType);
		m_pRenderer->put_ClipMode(nClipFlag);
		DoPath(pGState, pPath, pGState->GetPageHeight(), pMatrix);
		m_pRenderer->EndCommand(c_nPathType);
		m_pRenderer->EndCommand(c_nClipType);
	}
	void RendererOutputDev::ClipToText(const std::wstring& wsFontName, const std::wstring& wsFontPath, double dFontSize, int nFontStyle, double *pMatrix, const std::wstring& wsText, double dX, double dY, double dWidth, double dHeight, double dBaseLineOffset)
	{
        if (m_bDrawOnlyText)
            return;

		if (m_bTransparentGroup)
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
    void RendererOutputDev::EndTextObject(GrState *pGState)
    {
        if (NULL != m_pBufferTextClip)
        {
            UpdateClip(pGState);

            RELEASEOBJECT(m_pBufferTextClip);
        }
    }
	void RendererOutputDev::BeginStringOperator(GrState *pGState)
	{
		if (m_bTransparentGroup)
			return;

		m_pRenderer->BeginCommand(c_nTextType);

		int nRenderMode = pGState->GetRenderMode();

		// Обработка Clip
		if (nRenderMode >= 4)
		{
            RELEASEOBJECT(m_pBufferTextClip);
			m_pBufferTextClip = new GrTextClip();
		}

		// Обработка Stroke
		if (1 == nRenderMode || 2 == nRenderMode || 5 == nRenderMode || 6 == nRenderMode)
		{
			//Painter::CPen oPen;

			//m_pRenderer->put_PenSize( PDFCoordsToMM( pGState->GetFontSize() ) * 0.05 );
			//m_pRenderer->put_PenAlpha(  255 );
			//oPen.SetColor( m_oPen.GetColor() );

			//BSTR bsPen = oPen.ToXmlString().AllocSysString();
			//m_pRenderer->SetPen( bsPen );
			//::SysFreeString( bsPen );
		}
	}
	void RendererOutputDev::EndStringOperator(GrState *pGState)
	{
		if (m_bTransparentGroup)
			return;

		int nRenderMode = pGState->GetRenderMode();

		// Добавляем в Clipping Path текст
		if (nRenderMode >= 4)
		{
			if (m_pBufferTextClip)
				pGState->GetClip()->AppendTextClip(m_pBufferTextClip);

            UpdateFont(pGState);
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
	void RendererOutputDev::DrawString(GrState *pGState, StringExt *seString)
	{
		if (m_bTransparentGroup)
			return;

		// Проверяем наличие списка со шрифтами
		if (NULL == m_pFontList)
			return;

		// Проверяем наличие текущего шрифта 
		TFontEntry oEntry;
		if (!m_pFontList->GetFont(pGState->GetFont()->GetID(), &oEntry))
			return;

		int nRendererMode = pGState->GetRenderMode();

		if (3 == nRendererMode) // Невидимый текс
			return;

		double *pCTM  = pGState->GetCTM();
		double *pTm   = pGState->GetTextMatrix();
		GrFont *pFont = pGState->GetFont();

		unsigned int unGidsCount = seString->GetLength();
		unsigned int* pGids = new unsigned int[unGidsCount];
		if (!pGids)
			return;

		std::wstring  wsUnicodeText;
		for (int nIndex = 0; nIndex < seString->GetLength(); nIndex++)
		{
			char nChar = seString->GetAt(nIndex);

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
	void RendererOutputDev::DrawChar(GrState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen)
	{
		if (m_bTransparentGroup)
			return;

		// Проверяем наличие списка со шрифтами
		if (NULL == m_pFontList)
			return;

		// Проверяем наличие текущего шрифта 
		TFontEntry oEntry;
		if (!m_pFontList->GetFont(pGState->GetFont()->GetID(), &oEntry))
			return;

		int   nRenderMode = pGState->GetRenderMode();

		if (3 == nRenderMode) // Невидимый текст
		{
			return;
		}

		double *pCTM      = pGState->GetCTM();
		double *pTm       = pGState->GetTextMatrix();
		GrFont *pFont     = pGState->GetFont();

		double pNewTm[6], arrMatrix[6];

		double dTextScale = min(sqrt(pTm[2] * pTm[2] + pTm[3] * pTm[3]), sqrt(pTm[0] * pTm[0] + pTm[1] * pTm[1]));
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
		arrMatrix[5] = -(pNewTm[4] * pCTM[1] + pNewTm[5] * pCTM[3] + pCTM[5]) + pGState->GetPageHeight();

		if (true)
		{
			double dNorma = min(sqrt(arrMatrix[0] * arrMatrix[0] + arrMatrix[1] * arrMatrix[1]), sqrt(arrMatrix[2] * arrMatrix[2] + arrMatrix[3] * arrMatrix[3]));
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
		double dPageHeight = pGState->GetPageHeight();

		std::wstring wsUnicodeText;
		if (NULL != oEntry.pCodeToUnicode && nCode < oEntry.unLenUnicode)
		{
			unsigned short unUnicode = oEntry.pCodeToUnicode[nCode];
			wsUnicodeText = (wchar_t(unUnicode));
		}
		else
		{
			if (pGState->GetFont()->IsCIDFont())
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
			int nCurCode = (0 == nCode ? 65534 : nCode);
			unGid       = (unsigned int)nCurCode;
			unGidsCount = 1;
		}

		std::wstring wsSrcCodeText;
		if (c_nPDFWriter == m_lRendererType)
		{
			int nCurCode = (0 == nCode ? 65534 : nCode);
			if (pGState->GetFont()->IsCIDFont())
			{
				// Мы посылаем и сам CID и внутренний Code с его длинной
				CXmlWriter oWriter;
				oWriter.WriteNodeBegin(L"PDF-Text", true);
				oWriter.WriteAttribute(L"cid", nCurCode);
				oWriter.WriteAttribute(L"code", (unsigned short)(pUnicode[0]));
				oWriter.WriteAttribute(L"len", nUnicodeLen);
				oWriter.WriteNodeEnd(L"PDF-Text", true, true);
				wsSrcCodeText = oWriter.GetXmlString();
			}
			else
			{
				// Мы посылаем и сам CID и внутренний Code с его длинной
				CXmlWriter oWriter;
				oWriter.WriteNodeBegin(L"PDF-Text", true);
				oWriter.WriteAttribute(L"code", nCurCode);
				oWriter.WriteNodeEnd(L"PDF-Text", true, true);
				wsSrcCodeText = oWriter.GetXmlString();
			}
		}

		float fAscent = pGState->GetFontSize();
		if (nRenderMode == 0 || nRenderMode == 2 || nRenderMode == 4 || nRenderMode == 6)
		{
			if (c_nPDFWriter == m_lRendererType)
			{
				CPdfRenderer* pPdfRenderer = (CPdfRenderer*)m_pRenderer;
				//pPdfRenderer->CommandDrawTextPdf(wsUnicodeText, &unGid, unGidsCount, wsSrcCodeText, PDFCoordsToMM(0 + dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));
			}
			else
			{
				m_pRenderer->CommandDrawTextEx(wsUnicodeText, &unGid, unGidsCount, PDFCoordsToMM(0 + dShiftX), PDFCoordsToMM(dShiftY), PDFCoordsToMM(dDx), PDFCoordsToMM(dDy));
			}
		}

		if (nRenderMode == 1 || nRenderMode == 2 || nRenderMode == 5 || nRenderMode == 6)
		{
			m_pRenderer->BeginCommand(c_nStrokeTextType);

			//m_pRenderer->PathCommandEnd();
			//m_pRenderer->PathCommandText( bsText, PDFCoordsToMM( 0 + dShiftX ), PDFCoordsToMM( /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/ + dShiftY ), PDFCoordsToMM( 0 ), PDFCoordsToMM( 0 ), PDFCoordsToMM( 0 ) );


			// Временно
			//m_pRenderer->PathCommandTextEx( bsText, PDFCoordsToMM( 0 + dShiftX ), PDFCoordsToMM( /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/ + dShiftY ), PDFCoordsToMM( 0 ), PDFCoordsToMM( 0 ), PDFCoordsToMM( 0 ), 0, bsStringGID );
			//m_pRenderer->PathCommandTextEx( bsUnicodeText, bsGIDText, bsSrcCodeText, PDFCoordsToMM( 0 + dShiftX ), PDFCoordsToMM( /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/0 + dShiftY ), PDFCoordsToMM( dDx ), PDFCoordsToMM( dDy ), PDFCoordsToMM( 0 ), 0 );
			//-----------


			//m_pRenderer->PathCommandText( bsUnicodeText, PDFCoordsToMM( 0 + dShiftX ), PDFCoordsToMM( /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/ + dShiftY ), PDFCoordsToMM( dDx ), PDFCoordsToMM( dDy ), PDFCoordsToMM( 0 ) );
			//m_pRenderer->DrawPath( c_nStroke );

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
			m_pBufferTextClip->ClipToText(wsTempFontName, wsTempFontPath, dTempFontSize, (int)lTempFontStyle, arrMatrix, wsClipText, 0 + dShiftX, /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/ +dShiftY, 0, 0, 0);
		}

		m_pRenderer->put_FontSize(dOldSize);
	}
	bool RendererOutputDev::BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pUnicode, int nUnicodeLen)
	{
		return false;
	}
	void RendererOutputDev::EndType3Char(GrState *pGState)
	{
		return;
	}
	void RendererOutputDev::Type3D0(GrState *pGState, double dWx, double dWy)
	{
		return;
	}
	void RendererOutputDev::Type3D1(GrState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy)
	{
		return;
	}
	void RendererOutputDev::DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, bool bInvert, bool bInlineImage)
	{
        if (m_bDrawOnlyText)
            return;

		if (pGState->GetFillColorSpace()->IsNonMarking())
		{
			return;
		}

		double dPageHeight = pGState->GetPageHeight();

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

		pImageStream->Reset();

		unsigned char unAlpha = m_bTransparentGroup ? 255.0 * pGState->GetFillOpacity() : 255;
		unsigned char unPixel = 0;
		int nInvert = (bInvert ? 1 : 0);
		for (int nY = nHeight - 1; nY >= 0; nY--)
		{
			unsigned char *pMask = NULL;
			int nX = 0;
			for (nX = 0, pMask = pImageStream->GetNextLine(); nX < nWidth; nX++)
			{
				int nIndex = 4 * (nX + nY * nWidth);
				unsigned char unPixel = *pMask++ ^ nInvert;
				pBufferPtr[nIndex + 0] = unPixel ? 255 : 0;
				pBufferPtr[nIndex + 1] = unPixel ? 255 : 0;
				pBufferPtr[nIndex + 2] = unPixel ? 255 : 0;
				pBufferPtr[nIndex + 3] = unPixel ? 0 : unAlpha;
			}
		}

		delete pImageStream;

		double arrMatrix[6];
		double *pCTM = pGState->GetCTM();

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
	void RendererOutputDev::DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pMaskColors, bool bInlineImg)
	{
        if (m_bDrawOnlyText)
            return;

		double dPageHeight = pGState->GetPageHeight();

		int nBufferSize = 4 * nWidth * nHeight;
		if (nBufferSize < 1)
			return;

		unsigned char *pBufferPtr = new unsigned char[nBufferSize];
		if (!pBufferPtr)
			return;

		Aggplus::CImage oImage;
		oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);

		// Пишем данные в pBufferPtr
		ImageStream *pImageStream = new ImageStream(pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent());

		pImageStream->Reset();

		unsigned char unAlpha = m_bTransparentGroup ? 255.0 * pGState->GetFillOpacity() : 255;

		unsigned char unPixel[32] ={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		for (int nY = nHeight - 1; nY >= 0; nY--)
		{
			for (int nX = 0; nX < nWidth; nX++)
			{
				int nIndex = 4 * (nX + nY * nWidth);
				pImageStream->GetPixel(unPixel);
				GrRGB oRGB;
				pColorMap->GetRGB(unPixel, &oRGB);
				pBufferPtr[nIndex + 0] = ColorToByte(oRGB.b);
				pBufferPtr[nIndex + 1] = ColorToByte(oRGB.g);
				pBufferPtr[nIndex + 2] = ColorToByte(oRGB.r);
				pBufferPtr[nIndex + 3] = unAlpha;
			}
		}

		delete pImageStream;

		double arrMatrix[6];
		double *pCTM = pGState->GetCTM();
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
	void RendererOutputDev::DrawMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, bool bMaskInvert)
	{
		if (m_bDrawOnlyText)
			return;

		if (nMaskWidth <= 0 || nMaskHeight <= 0)
			DrawImage(pGState, pRef, pStream, nWidth, nHeight, pColorMap, NULL, false);

		double dPageHeight = pGState->GetPageHeight();

		int nBufferSize = 4 * nWidth * nHeight;
		if (nBufferSize < 1)
			return;

		unsigned char *pBufferPtr = new unsigned char[nBufferSize];
		if (!pBufferPtr)
			return;

		Aggplus::CImage oImage;
		oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);

		// Пишем данные в pBufferPtr
		ImageStream *pImageStream = new ImageStream(pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent());
		ImageStream *pMask = new ImageStream(pMaskStream, nMaskWidth, 1, 1);

		pMask->Reset();
		pImageStream->Reset();

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
				for (int nX = 0; nX < nMaskWidth; nX++)
				{
					int nIndex = nX + nY * nMaskWidth;
					pMask->GetPixel(&unMask);
					pMaskBuffer[nIndex] = unMask;
				}
			}

			double dScaleWidth  = (double)nWidth / (double)nMaskWidth;
			double dScaleHeight = (double)nHeight / (double)nMaskHeight;

			unsigned char unPixel[4] ={ 0, 0, 0, 0 };
			for (int nY = nHeight - 1; nY >= 0; nY--)
			{
				for (int nX = 0; nX < nWidth; nX++)
				{
					int nIndex = 4 * (nX + nY * nWidth);
					pImageStream->GetPixel(unPixel);

					int nNearestY = (std::min)((int)(nY / dScaleHeight), nMaskHeight - 1);
					int nNearestX = (std::min)((int)(nX / dScaleWidth), nMaskWidth - 1);
					unMask = pMaskBuffer[nNearestY * nMaskWidth + nNearestX];

					GrRGB oRGB;
					pColorMap->GetRGB(unPixel, &oRGB);
					pBufferPtr[nIndex + 0] = ColorToByte(oRGB.b);
					pBufferPtr[nIndex + 1] = ColorToByte(oRGB.g);
					pBufferPtr[nIndex + 2] = ColorToByte(oRGB.r);

					if (unMask && !bMaskInvert)
						pBufferPtr[nIndex + 3] = 0;
					else
						pBufferPtr[nIndex + 3] = 255;
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
				for (int nX = 0; nX < nWidth; nX++)
				{
					int nIndex = 4 * (nX + nY * nWidth);
					pImageStream->GetPixel(unPixel);
					pMask->GetPixel(&unMask);
					GrRGB oRGB;
					pColorMap->GetRGB(unPixel, &oRGB);
					pBufferPtr[nIndex + 0] = ColorToByte(oRGB.b);
					pBufferPtr[nIndex + 1] = ColorToByte(oRGB.g);
					pBufferPtr[nIndex + 2] = ColorToByte(oRGB.r);

					if (unMask && !bMaskInvert)
						pBufferPtr[nIndex + 3] = 0;
					else
						pBufferPtr[nIndex + 3] = 255;
				}
			}
		}

		delete pMask;
		delete pImageStream;

		double arrMatrix[6];
		double *pCTM = pGState->GetCTM();
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
	void RendererOutputDev::DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap)
	{
        if (m_bDrawOnlyText)
            return;

		double dPageHeight = pGState->GetPageHeight();

		int nBufferSize = 4 * nWidth * nHeight;
		if (nBufferSize < 1)
			return;

		unsigned char *pBufferPtr = new unsigned char[nBufferSize];
		if (!pBufferPtr)
			return;

		Aggplus::CImage oImage;
		oImage.Create(pBufferPtr, nWidth, nHeight, -4 * nWidth);

		// Пишем данные в pBufferPtr
		ImageStream *pImageStream = new ImageStream(pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent());
		pImageStream->Reset();

		double dAlphaKoef = m_bTransparentGroup ? pGState->GetFillOpacity() : 1;
		unsigned char unPixel[4] ={ 0, 0, 0, 0 };
		for (int nY = nHeight - 1; nY >= 0; nY--)
		{
			for (int nX = 0; nX < nWidth; nX++)
			{
				int nIndex = 4 * (nX + nY * nWidth);
				pImageStream->GetPixel(unPixel);
				GrRGB oRGB;
				pColorMap->GetRGB(unPixel, &oRGB);
				pBufferPtr[nIndex + 0] = ColorToByte(oRGB.b);
				pBufferPtr[nIndex + 1] = ColorToByte(oRGB.g);
				pBufferPtr[nIndex + 2] = ColorToByte(oRGB.r);
				pBufferPtr[nIndex + 3] = 255;
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
				ImageStream *pSMaskStream = new ImageStream(pMaskStream, nMaskWidth, pMaskColorMap->GetComponentsCount(), pMaskColorMap->GetBitsPerComponent());
				unsigned char *pAlpha = new unsigned char[nMaskWidth * nMaskHeight];
				if (pSMaskStream && pAlpha)
				{
					pSMaskStream->Reset();

					unsigned char unAlpha = 0;
					for (int nY = 0; nY < nMaskHeight; nY++)
					{
						for (int nX = 0; nX < nMaskWidth; nX++)
						{
							int nIndex = (nX + nY * nMaskWidth);
							pSMaskStream->GetPixel(&unAlpha);
							GrGray oGray;
							pMaskColorMap->GetGray(&unAlpha, &oGray);
							pAlpha[nIndex] = ColorToByte(oGray);
						}
					}
					delete pSMaskStream;

					int nMaxW = (std::max)(nWidth, nMaskWidth);
					int nMaxH = (std::max)(nHeight, nMaskHeight);
					if (nWidth != nMaxW || nHeight != nMaxH)
					{
						unsigned char* pImageBuffer = pBufferPtr;
						int nNewBufferSize = 4 * nMaxW * nMaxH;
						pBufferPtr = new unsigned char[nNewBufferSize];
						if (!pBufferPtr)
						{
							delete[] pImageBuffer;
							delete[] pAlpha;
							return;
						}

						oImage.Create(pBufferPtr, nMaxW, nMaxH, -4 * nMaxW);

						double dImageScaleWidth  = (double)nWidth / (double)nMaxW;
						double dImageScaleHeight = (double)nHeight / (double)nMaxH;

						double dAlphaScaleWidth  = (double)nMaskWidth / (double)nMaxW;
						double dAlphaScaleHeight = (double)nMaskHeight / (double)nMaxH;

						for (int nY = nMaxH - 1; nY >= 0; nY--)
						{
							for (int nX = 0; nX < nMaxW; nX++)
							{
								int nIndex = 4 * (nY * nMaxW + nX);

								int nNearestAlphaMatch =  (((int)((nMaxH - 1 - nY) * dAlphaScaleHeight) * nMaskWidth) + ((int)(nX * dAlphaScaleWidth)));
								int nNearestImageMatch =  4 * (((int)((nMaxH - 1 - nY) * dImageScaleHeight) * nWidth) + ((int)(nX * dImageScaleWidth)));

								pBufferPtr[nIndex + 0] = pImageBuffer[nNearestImageMatch + 0];
								pBufferPtr[nIndex + 1] = pImageBuffer[nNearestImageMatch + 1];
								pBufferPtr[nIndex + 2] = pImageBuffer[nNearestImageMatch + 2];
								pBufferPtr[nIndex + 3] = (unsigned char)(pAlpha[nNearestAlphaMatch] * dAlphaKoef);
							}
						}

						delete[] pImageBuffer;
					}
					else
					{
						double dAlphaScaleWidth  = (double)nMaskWidth / (double)nWidth;
						double dAlphaScaleHeight = (double)nMaskHeight / (double)nHeight;

						for (int nY = nHeight - 1; nY >= 0; nY--)
						{
							for (int nX = 0; nX < nWidth; nX++)
							{
								int nIndex = 4 * (nY * nWidth + nX);

								int nNearestAlphaMatch =  (((int)((nHeight - 1 - nY) * dAlphaScaleHeight) * nMaskWidth) + ((int)(nX * dAlphaScaleWidth)));

								pBufferPtr[nIndex + 3] = (unsigned char)(pAlpha[nNearestAlphaMatch] * dAlphaKoef);
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

			if (!bResize)
			{
				for (int nY = nHeight - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nY * nWidth + nX);
						pBufferPtr[nIndex + 3] = (unsigned char)(255.0 * dAlphaKoef);
					}
				}
			}
		}
		else
		{
			ImageStream *pSMaskStream = new ImageStream(pMaskStream, nMaskWidth, pMaskColorMap->GetComponentsCount(), pMaskColorMap->GetBitsPerComponent());
			pSMaskStream->Reset();

			unsigned char unAlpha = 0;
			for (int nY = nHeight - 1; nY >= 0; nY--)
			{
				for (int nX = 0; nX < nWidth; nX++)
				{
					int nIndex = 4 * (nX + nY * nWidth);
					pSMaskStream->GetPixel(&unAlpha);
					GrGray oGray;
					pMaskColorMap->GetGray(&unAlpha, &oGray);
					pBufferPtr[nIndex + 3] = ColorToByte(oGray) * dAlphaKoef;
				}
			}
			delete pSMaskStream;
		}

		double arrMatrix[6];
		double *pCTM = pGState->GetCTM();
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
	void RendererOutputDev::BeginTransparencyGroup(GrState *pGState, double *pBBox, GrColorSpace *pBlendingColorSpace, bool bIsolated, bool bKnockout, bool bForSoftMask)
	{
		m_bTransparentGroup = true;
		m_bTransparentGroupSoftMask = bForSoftMask;
	}
	void RendererOutputDev::EndTransparencyGroup(GrState *pGState)
	{
		m_bTransparentGroup = false;
		m_bTransparentGroupSoftMask = false;

		if (m_pTransparentGroupSoftMask)
			delete[]m_pTransparentGroupSoftMask;

		m_pTransparentGroupSoftMask = NULL;
	}
	void RendererOutputDev::PaintTransparencyGroup(GrState *pGState, double *pBBox)
	{
	}
	void RendererOutputDev::SetSoftMask(GrState *pGState, double *pBBox, bool bAlpha, Function *pTransferFunc, GrColor *pBackdropColor)
	{
	}
	void RendererOutputDev::ClearSoftMask(GrState *pGState)
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
	void RendererOutputDev::DoPath(GrState *pGState, GrPath *pPath, double dPageHeight, double *pCTM)
	{
        if (m_bDrawOnlyText)
            return;

		if (m_bTransparentGroup)
			return;

		double arrMatrix[6];
		//double *pCTM = pGState->GetCTM();
		arrMatrix[0] =  pCTM[0];
		arrMatrix[1] = -pCTM[1];
		arrMatrix[2] =  pCTM[2];
		arrMatrix[3] = -pCTM[3];
		arrMatrix[4] =  pCTM[4];
		arrMatrix[5] = -pCTM[5] + dPageHeight;

		double dShiftX = 0, dShiftY = 0;
		DoTransform(arrMatrix, &dShiftX, &dShiftY);

		m_pRenderer->BeginCommand(c_nPathType);
		m_pRenderer->PathCommandEnd();

		int nSubPathCount = pPath->GetSubpathsCount();

		for (int nSubPathIndex = 0; nSubPathIndex < nSubPathCount; ++nSubPathIndex)
		{
			GrSubpath *pSubpath = pPath->GetSubpath(nSubPathIndex);
			int nPointsCount = pSubpath->GetPointsCount();

			m_pRenderer->PathCommandMoveTo(PDFCoordsToMM(pSubpath->GetX(0) + dShiftX), PDFCoordsToMM(pSubpath->GetY(0) + dShiftY));

			int nCurPointIndex = 1;
			while (nCurPointIndex < nPointsCount)
			{
				if (pSubpath->GetCurve(nCurPointIndex))
				{
					m_pRenderer->PathCommandCurveTo(PDFCoordsToMM(pSubpath->GetX(nCurPointIndex) + dShiftX), PDFCoordsToMM(pSubpath->GetY(nCurPointIndex) + dShiftY), PDFCoordsToMM(pSubpath->GetX(nCurPointIndex + 1) + dShiftX), PDFCoordsToMM(pSubpath->GetY(nCurPointIndex + 1) + dShiftY), PDFCoordsToMM(pSubpath->GetX(nCurPointIndex + 2) + dShiftX), PDFCoordsToMM(pSubpath->GetY(nCurPointIndex + 2) + dShiftY));
					nCurPointIndex += 3;
				}
				else
				{
					m_pRenderer->PathCommandLineTo(PDFCoordsToMM(pSubpath->GetX(nCurPointIndex) + dShiftX), PDFCoordsToMM(pSubpath->GetY(nCurPointIndex) + dShiftY));
					++nCurPointIndex;
				}
			}
			if (pSubpath->IsClosed())
			{
				m_pRenderer->PathCommandClose();
			}
		}
	}
	void RendererOutputDev::UpdateClip(GrState *pGState)
	{
        if (m_bDrawOnlyText)
            return;

		if (m_bTransparentGroup)
			return;

		if (m_bTiling)
			return;

		UpdateClipAttack(pGState);
	}
	void RendererOutputDev::UpdateClipAttack(GrState *pGState)
	{
		GrClip *pClip = pGState->GetClip();

		int nPathIndex = -1;
		//if ( m_pClip && m_pClip->IsEqual( pClip, nPathIndex ) )
		//{
		//	return;
		//}

        int nPathIndexStart = (-1 == nPathIndex) ? 0 : nPathIndex;

        m_pRenderer->BeginCommand(c_nResetClipType);
        m_pRenderer->EndCommand(c_nResetClipType);

        for (int nIndex = nPathIndexStart; nIndex < pClip->GetPathsCount(); nIndex++)
        {
            GrPath *pPath   = pClip->GetPath(nIndex);
            int     nFlag   = pClip->GetFlag(nIndex);
            double *pMatrix = pClip->GetMatrix(nIndex);

            int     nClipFlag = GrClipEOFlag == nFlag ? c_nClipRegionTypeEvenOdd : c_nClipRegionTypeWinding;
            nClipFlag |= c_nClipRegionIntersect;

            m_pRenderer->BeginCommand(c_nClipType);
            m_pRenderer->put_ClipMode(nClipFlag);
            DoPath(pGState, pPath, pGState->GetPageHeight(), pMatrix);
            m_pRenderer->EndCommand(c_nPathType);
            m_pRenderer->EndCommand(c_nClipType);
            m_pRenderer->PathCommandEnd();
        }

        int nTextClipCount = pClip->GetTextsCount();
        if (-1 == nPathIndex && nTextClipCount > 0)
        {
            m_pRenderer->BeginCommand(c_nClipType);
            m_pRenderer->put_ClipMode(c_nClipRegionTypeWinding | c_nClipRegionIntersect);
            m_pRenderer->StartConvertCoordsToIdentity();

            for (int nIndex = 0; nIndex < nTextClipCount; nIndex++)
            {
                WString wsFontName, wsFontPath;
                int lFontStyle;
                double dFontSize = 10, dX = 0, dY = 0, dWidth = 0, dHeight = 0, dBaseLineOffset = 0;
                WString wsText = pClip->GetText(nIndex, &dX, &dY, &dWidth, &dHeight, &dBaseLineOffset, &wsFontName, &wsFontPath, &dFontSize, &lFontStyle);
                int     nFlag   = pClip->GetFlag(nIndex);

                m_pRenderer->put_FontName(wsFontName);
                m_pRenderer->put_FontPath(wsFontPath);
                m_pRenderer->put_FontSize(dFontSize);
                m_pRenderer->put_FontStyle(lFontStyle);

                double dShiftX = 0, dShiftY = 0;
                DoTransform(pClip->GetTextMatrix(nIndex), &dShiftX, &dShiftY, true);

                // TODO: нужна нормальная конвертация
                int nLen = 0;
                WString wsTextTmp = wsText;
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

		if (m_pClip)
			delete m_pClip;

		m_pClip = pClip->Copy();

		UpdateFont(pGState);
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
		else if (m_arrMatrix[0] == pMatrix[0] && m_arrMatrix[1] == pMatrix[1] && m_arrMatrix[2] == pMatrix[2] && m_arrMatrix[3] == pMatrix[3] && !bText)
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
