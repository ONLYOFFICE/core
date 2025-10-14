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
#ifndef _PDF_WRITER_SRC_PAGES_H
#define _PDF_WRITER_SRC_PAGES_H

#include "Objects.h"
#include <vector>

#ifdef DrawText
#undef DrawText
#endif

namespace PdfWriter
{
	class CXref;
	class CGrState;
	class CExtGrState;
	class CAnnotation;
	class CFontDict;
	class CXObject;
	class CImageDict;
	class CShading;
	class CImageTilePattern;
	class CDocument;
	class CTextLine;
	class CTextWord;
	class CFieldBase;
	class CPage;
	class CResourcesDict;
	//----------------------------------------------------------------------------------------
	// CPageTree
	//----------------------------------------------------------------------------------------
	class CPageTree : public CDictObject
	{
	public:
		CPageTree(CXref* pXref);
		CPageTree();
		void Fix();
		void AddPage(CObjectBase* pPage);
		CObjectBase* GetObj(int nPageIndex);
		CPage* GetPage(int nPageIndex);
		CObjectBase* RemovePage(int nPageIndex);
		bool InsertPage(int nPageIndex, CObjectBase* pPage);
		bool ReplacePage(int nPageIndex, CPage* pPage);
		bool Join(CPageTree* pPageTree);
		bool Find(CPage* pPage, int& nI);
		void CreateFakePages(int nPages, int nPageIndex = -1);
		void ClearFakePages();
		unsigned int GetCount()
		{
			return m_pCount ? m_pCount->Get() : 0;
		}
		EDictType GetDictType() const
		{
			return dict_type_PAGES;
		}
	private:
		CObjectBase* GetFromPageTree(int nPageIndex, int& nI, bool bRemove = false, bool bInsert = false, CObjectBase* pPage = NULL);

		CNumberObject* m_pCount;
		CArrayObject*  m_pPages;
	};
	//----------------------------------------------------------------------------------------
	// CPage
	//----------------------------------------------------------------------------------------
	class CPage : public CDictObject
	{
	public:
		CPage(CDocument* pDocument);
		CPage(CXref* pXref, CPageTree* pParent, CDocument* pDocument);
		~CPage();

		void      Fix();
		void      SetHeight(double dHeight);
		void      SetWidth(double dWidth);
		double    GetWidth();
		double    GetHeight();

		EDictType GetDictType() const
		{
			return dict_type_PAGE;
		}
		CStream* GetStream() const
		{
			return m_pStream;
		}
		void SetStream(CStream* pStream)
		{
			m_pStream = pStream;
		}
		void      BeforeWrite();

		void      MoveTo(double dX, double dY);
		void      LineTo(double dX, double dY);
		void      CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe);
		void      Ellipse(double dX, double dY, double dXRay, double dYRay);
		void      EllipseArcTo(double dX, double dY, double dXRad, double dYRad, double dAngle1, double dAngle2, bool bClockDirection = false);
		void      ClosePath();		
		void      Stroke();
		void      Fill();
		void      EoFill();
		void      FillStroke();
		void      EoFillStroke();
		void      EndPath();
		void      Clip();
		void      Eoclip();
		void      SetLineWidth(double dLineWidth);
		void      SetLineCap(ELineCapStyle eLineCap);
		void      SetLineJoin(ELineJoinStyle eLineJoin);
		void      SetMiterLimit(double dMiterLimit);
		void      SetDash(const double* pPattern, unsigned int unCount, double dPhase);
		void      SetFlat(double dFlatness);
		void      GrSave();
		void      GrRestore();
		void      SetStrokeColor(unsigned char unR, unsigned char unG, unsigned char unB);
		void      SetStrokeG(double dG);
		void      SetStrokeRGB(double dR, double dG, double dB);
		void      SetStrokeCMYK(double dC, double dM, double dY, double dK);
		void      SetFillColor(unsigned char unR, unsigned char unG, unsigned char unB);
		void      SetFillG(double dG);
		void      SetFillRGB(double dR, double dG, double dB);
		void      SetFillCMYK(double dC, double dM, double dY, double dK);
		void      Concat(double dM11, double dM12, double dM21, double dM22, double dX, double dY);
		void      StartTransform(double dM11, double dM12, double dM21, double dM22, double dX, double dY);
		void      SetTransform(double dM11, double dM12, double dM21, double dM22, double dX, double dY);
		void      SetExtGrState(CExtGrState* pExtGrState);
		void      SetExtGrStateKey(const char* sKey);
		void      AddAnnotation(CDictObject* pAnnot);
		bool      DeleteAnnotation(unsigned int nID);
		void      DrawShading(CShading* pShading);
		void      SetStrokeAlpha(unsigned char unAlpha);
		void      SetFillAlpha(unsigned char unAlpha);
		void      BeginMarkedContent(const std::string& sName);
		void      BeginMarkedContentDict(const std::string& sName, CDictObject* pBDC);
		void      EndMarkedContent();
		void      SetRenderingIntent(ERenderingIntent eRenderingIntent);

		void      BeginText();
		void      EndText();
		void      MoveTextPos(double dX, double dY);
		void      ShowText(const BYTE* sText, unsigned int unLen);
		void      DrawText(double dX, double dY, const BYTE* sText, unsigned int unLen);
		void      SetCharSpace(double dValue);
		void      SetWordSpace(double dValue);
		void      SetHorizontalScaling(double dValue);
		void      SetFontAndSize(CFontDict* pFont, double dSize);
		void      SetFontKeyAndSize(const char* sKey, double dSize);
		void      SetFontType(EFontType nType);
		void      SetTextRenderingMode(ETextRenderingMode eMode);
		void      SetTextMatrix(double dM11, double dM12, double dM21, double dM22, double dX, double dY);
		void      DrawTextLine(const CTextLine* pTextLine);
		void      SetTextRise(double dS);

		void      ExecuteXObject(CXObject* pXObject);
		void      ExecuteXObject(const char* sXObjectName);
		void      DrawImage(CImageDict* pImage, double dX, double dY, double dWidth, double dHeight);
		void      SetPatternColorSpace(CImageTilePattern* pPattern);
		void      SetFilter(unsigned int unFiler);
		CMatrix*  GetTransform();
		void      AddGroup(CDictObject* pDict);

		void      AddContents(CXref* pXref);
        void      SetRotate(int nRotate);
        int       GetRotate();
		void      ClearContent(CXref* pXref);
		void      ClearContentFull(CXref* pXref);
		CResourcesDict* GetResourcesItem();

	private:

		void          Init(CDocument* pDocument);
		void          EllipseArc(double dX, double dY, double dXRad, double dYRad, double dAngle1, double dAngle2, bool bClockDirection);
		CArrayObject* GetMediaBoxItem();
		CObjectBase*  GetCropBoxItem();
        CObjectBase*  GetRotateItem();
		TBox          GetMediaBox();
		void          SetMediaBoxValue(unsigned int unIndex, double dValue);
		void          AddResource(CXref* pXref = NULL);
		void          SetGrMode(EGrMode eMode);
		void          CheckGrMode(EGrMode eMode);
		void          WriteText(const BYTE* sText, unsigned int unLen);
		const char*   GetLocalShadingName(CShading* pShading);
		const char*   GetLocalPatternName(CImageTilePattern* pPattern);

	private:

		CDocument*   m_pDocument;
		CPageTree*   m_pParent;
		CPoint       m_oStartPos;          // Позиция начала текущего пата
		CPoint       m_oCurPos;            // Текущая позиция пата
		CPoint       m_oTextPos;           // Текущая позиция текста
		CMatrix      m_oTextMatrix;
		CArrayObject* m_pContents;
		CStream*     m_pStream;
		unsigned int m_unCompressionMode;
		EGrMode      m_eGrMode;
		CGrState*    m_pGrState;
		CFontDict*   m_pFont;              // Текущий шрифт
		CDictObject* m_pShadings;
		unsigned int m_unShadingsCount;
		CDictObject* m_pPatterns;
		unsigned int m_unPatternsCount;
		EFontType    m_eType;
	};
	class CFakePage : public CObjectBase
	{
	public:
		CFakePage(int nOriginIndex);
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			return pOut ? pOut : new CFakePage(m_nOriginIndex);
		}

		int GetOriginIndex();

	private:
		int m_nOriginIndex;
	};
	//----------------------------------------------------------------------------------------
	// CTextWord
	//----------------------------------------------------------------------------------------
	class CTextWord
	{
	public:

		CTextWord();
		~CTextWord();
		bool Add(unsigned char* pCodes, unsigned int unLen, double dX, double dY, double dWidth);

	private:

		void CheckBuffer();

	private:

		unsigned char*m_pText;
		int           m_nSize;
		int           m_nIndex;
		double        m_dStartX;
		double        m_dStartY;

		double        m_dCurX;

		friend class CTextLine;
		friend class CPage;
	};
	//----------------------------------------------------------------------------------------
	// CTextLine
	//----------------------------------------------------------------------------------------
	class CTextLine
	{
	public:

		CTextLine();
		~CTextLine();
		bool Add(unsigned char* pCodes, unsigned int unLen, double dX, double dY, double dWidth, double dFontSize);
		void Flush(CPage* pPage);

	private:

		void Clear();

	private:

		std::vector<CTextWord*> m_vWords;
		std::vector<double>     m_vShifts;
		double                  m_dX;
		double                  m_dY;

		friend class CPage;
	};
}

#endif // _PDF_WRITER_SRC_PAGES_H

