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
	//----------------------------------------------------------------------------------------
	// CPageTree
	//----------------------------------------------------------------------------------------
	class CPageTree : public CDictObject
	{
	public:
		CPageTree(CXref* pXref);
		CPageTree(CXref* pXref, bool bEmpty);
		void Fix();
		void AddPage(CDictObject* pPage);
		CObjectBase* GetObj(int nPageIndex);
		CPage* GetPage(int nPageIndex);
		CObjectBase* RemovePage(int nPageIndex);
		bool InsertPage(int nPageIndex, CPage* pPage);
		bool Join(CPageTree* pPageTree);
		unsigned int GetCount()
		{
			return m_pCount ? m_pCount->Get() : 0;
		}
		EDictType GetDictType() const
		{
			return dict_type_PAGES;
		}
	private:
		CObjectBase* GetFromPageTree(int nPageIndex, int& nI, bool bRemove = false, bool bInsert = false, CPage* pPage = NULL);

		CNumberObject* m_pCount;
		CArrayObject*  m_pPages;
		CXref*         m_pXref;
	};
	//----------------------------------------------------------------------------------------
	// CPage
	//----------------------------------------------------------------------------------------
	class CPage : public CDictObject
	{
	public:
		CPage(CXref* pXref, CDocument* pDocument);
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
		void      SetFillColor(unsigned char unR, unsigned char unG, unsigned char unB);
		void      Concat(double dM11, double dM12, double dM21, double dM22, double dX, double dY);
		void      SetTransform(double dM11, double dM12, double dM21, double dM22, double dX, double dY);
		void      SetExtGrState(CExtGrState* pExtGrState);
		void      AddAnnotation(CDictObject* pAnnot);
		void      DrawShading(CShading* pShading);
		void      SetStrokeAlpha(unsigned char unAlpha);
		void      SetFillAlpha(unsigned char unAlpha);

		void      BeginText();
		void      EndText();
		void      MoveTextPos(double dX, double dY);
		void      ShowText(const BYTE* sText, unsigned int unLen);
		void      DrawText(double dX, double dY, const BYTE* sText, unsigned int unLen);
		void      SetCharSpace(double dValue);
		void      SetHorizontalScalling(double dValue);
		void      SetFontAndSize(CFontDict* pFont, double dSize);
		void      SetTextRenderingMode(ETextRenderingMode eMode);
		void      SetTextMatrix(double dM11, double dM12, double dM21, double dM22, double dX, double dY);
		void      DrawTextLine(const CTextLine* pTextLine);

		void      ExecuteXObject(CXObject* pXObject);
		void      DrawImage(CImageDict* pImage, double dX, double dY, double dWidth, double dHeight);
		void      SetPatternColorSpace(CImageTilePattern* pPattern);
		void      SetFilter(unsigned int unFiler);
		CMatrix*  GetTransform();
		void      AddGroup(CDictObject* pDict);

		void      AddContents(CXref* pXref);
        void      SetRotate(int nRotate);
        int       GetRotate();

	private:

		void          Init(CXref* pXref, CDocument* pDocument);
		void          EllipseArc(double dX, double dY, double dXRad, double dYRad, double dAngle1, double dAngle2, bool bClockDirection);
		CArrayObject* GetMediaBoxItem();
		CDictObject*  GetResourcesItem();
		CObjectBase*  GetCropBoxItem();
        CObjectBase*  GetRotateItem();
		TBox          GetMediaBox();
		void          SetMediaBoxValue(unsigned int unIndex, double dValue);
		void          AddResource();
		void          SetGrMode(EGrMode eMode);
		void          CheckGrMode(EGrMode eMode);
		void          WriteText(const BYTE* sText, unsigned int unLen);
		const char*   GetExtGrStateName(CExtGrState* pState);
		const char*   GetLocalFontName(CFontDict* pFont);
		const char*   GetXObjectName(CXObject* pObject);
		const char*   GetLocalShadingName(CShading* pShading);
		const char*   GetLocalPatternName(CImageTilePattern* pPattern);

	private:

		CDocument*   m_pDocument;
		CPageTree*   m_pParent;
		CXref*       m_pXref;
		CPoint       m_oStartPos;          // Позиция начала текущего пата
		CPoint       m_oCurPos;            // Текущая позиция пата
		CPoint       m_oTextPos;           // Текущая позиция текста
		CMatrix      m_oTextMatrix;
		CArrayObject* m_pContents;
		CStream*     m_pStream;
		unsigned int m_unCompressionMode;
		CDictObject* m_pExtGStates;
		unsigned int m_unExtGStatesCount;
		EGrMode      m_eGrMode;
		CGrState*    m_pGrState;
		CDictObject* m_pFonts;
		unsigned int m_unFontsCount;
		CFontDict*   m_pFont;              // Текущий шрифт
		CDictObject* m_pXObjects;
		unsigned int m_unXObjectsCount;
		CDictObject* m_pShadings;
		unsigned int m_unShadingsCount;
		CDictObject* m_pPatterns;
		unsigned int m_unPatternsCount;
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

