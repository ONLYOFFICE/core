/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _PDF_WRITER_SRC_DOCUMENT_H
#define _PDF_WRITER_SRC_DOCUMENT_H

#include <vector>
#include <string>
#include "Types.h"

#include "../../DesktopEditor/fontengine/FontManager.h"
#include "../../DesktopEditor/common/File.h"
#include FT_TRUETYPE_TABLES_H

#ifdef CreateFont
#undef CreateFont
#endif

class CFontManager;

namespace PdfWriter
{
	class CCatalog;
	class COutline;
	class CXref;
	class CPageTree;
	class CPage;
	class CInfoDict;
	class CDictObject;
	class CEncryptDict;
	class CStream;
	class CDestination;
	class CExtGrState;
	class CAnnotation;
	class CImageDict;
	class CFontDict;
	class CFont14;
	class CFontCidTrueType;
	class CJbig2Global;
	class CShading;
	class CImageTilePattern;
	class CPattern;
	//----------------------------------------------------------------------------------------
	// CDocument
	//----------------------------------------------------------------------------------------
	class CDocument
	{
	public:

		CDocument();
		~CDocument();

		bool              CreateNew();
		void              Close();
		bool              SaveToFile(const std::wstring& wsPath);
			              
		void              SetEncryptionMode(EEncryptMode eMode, unsigned int unKeyLen = 0);
		void              SetPassword(const char* sOwnerPassword, const char* sUserPassword);
		void              SetPermission(unsigned int unPermission);
		void              SetCompressionMode(unsigned int unMode);
			              
		CPage*            AddPage();
		void              AddPageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix);
		void              AddPageLabel(unsigned int unPageIndex, EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix);
		COutline*         CreateOutline(COutline* pParent, const char* sTitle);
		CDestination*     CreateDestination(unsigned int unPageIndex);
					      
		CExtGrState*      GetExtGState(double dAlphaStroke = -1, double dAlphaFill = -1, EBlendMode eMode = blendmode_Unknown, int nStrokeAdjustment = -1);
		CExtGrState*      GetStrokeAlpha(double dAlpha);
		CExtGrState*      GetFillAlpha(double dAlpha);
		CJbig2Global*     GetJbig2Global();
					      
		CAnnotation*      CreateTextAnnot(unsigned int unPageNum, TRect oRect, const char* sText);
		CAnnotation*      CreateLinkAnnot(unsigned int unPageNum, TRect oRect, CDestination* pDest);
		CAnnotation*      CreateUriLinkAnnot(unsigned int unPageNum, TRect oRect, const char* sUri);
					      
		CImageDict*       CreateImage();
		CFont14*          CreateFont14(EStandard14Fonts eType);
		CFontCidTrueType* CreateTrueTypeFont(const std::wstring& wsFontPath, unsigned int unIndex);

		CImageTilePattern*CreateImageTilePattern(double dW, double dH, CImageDict* pImageDict, CMatrix* pMatrix = NULL, EImageTilePatternType eType = imagetilepatterntype_Default);
		CImageTilePattern*CreateHatchPattern(double dW, double dH, const BYTE& nR1, const BYTE& nG1, const BYTE& nB1, const BYTE& nAlpha1, const BYTE& nR2, const BYTE& nG2, const BYTE& nB2, const BYTE& nAlpha2, const std::wstring& wsHatch);
		CShading*         CreateAxialShading(CPage* pPage, double dX0, double dY0, double dX1, double dY1, unsigned char* pColors, unsigned char* pAlphas, double* pPoints, int nCount, CExtGrState*& pExtGrState);
		CShading*         CreateRadialShading(CPage* pPage, double dX0, double dY0, double dR0, double dX1, double dY1, double dR1, unsigned char* pColors, unsigned char* pAlphas, double* pPoints, int nCount, CExtGrState*& pExtGrState);
					  
	private:		  
					  
		char*             GetTTFontTag();
		void              AddFreeTypeFont(CFontCidTrueType* pFont);
		FT_Library        GetFreeTypeLibrary();
		CExtGrState*      FindExtGrState(double dAlphaStroke = -1, double dAlphaFill = -1, EBlendMode eMode = blendmode_Unknown, int nStrokeAdjustment = -1);
		void              SaveToStream(CStream* pStream);
		void              PrepareEncryption();
		CDictObject*      CreatePageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix);
		CShading*         CreateShading(CPage* pPage, double *pPattern, bool bAxial, unsigned char* pColors, unsigned char* pAlphas, double* pPoints, int nCount, CExtGrState*& pExtGrState);
		CShading*         CreateAxialShading(double dX0, double dY0, double dX1, double dY1, unsigned char* pColors, double* pPoints, int nCount);
		CShading*         CreateRadialShading(double dX0, double dY0, double dR0, double dX1, double dY1, double dR1, unsigned char* pColors, double* pPoints, int nCount);

	private:

		struct TFontInfo
		{
			TFontInfo(const std::wstring& path, const unsigned int& index, CFontCidTrueType* font)
			{
				wsPath  = path;
				unIndex = index;
				pFont   = font;
			}

			std::wstring      wsPath;
			unsigned int      unIndex;
			CFontCidTrueType* pFont;
		};

		CCatalog*                      m_pCatalog;
		COutline*                      m_pOutlines;
		CXref*                         m_pXref;
		CPageTree*                     m_pPageTree;
		CPage*                         m_pCurPage;
		int                            m_nCurPageNum;
		CInfoDict*                     m_pInfo;
		CDictObject*                   m_pTrailer;
		CDictObject*                   m_pResources;
		bool                           m_bEncrypt;
		CEncryptDict*                  m_pEncryptDict;
		unsigned int                   m_unCompressMode;
		std::vector<CPage*>            m_vPages;
		std::vector<CExtGrState*>      m_vExtGrStates;
		std::vector<CExtGrState*>      m_vStrokeAlpha;
		std::vector<CExtGrState*>      m_vFillAlpha;
		char                           m_sTTFontTag[8]; // 6 символов + '+' + 0x00 ("BAAAAA+/0")
		CJbig2Global*                  m_pJbig2;
		std::vector<CShading*>         m_vShadings;
		std::vector<TFontInfo>         m_vTTFonts;
		CDictObject*                   m_pTransparencyGroup;
		std::vector<CFontCidTrueType*> m_vFreeTypeFonts;
		FT_Library                     m_pFreeTypeLibrary;

		friend class CFontCidTrueType;
	};
}

#endif // _PDF_WRITER_SRC_DOCUMENT_H

