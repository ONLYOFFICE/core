#ifndef _PDF_WRITER_SRC_PAGES_H
#define _PDF_WRITER_SRC_PAGES_H

#include "Objects.h"

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
	//----------------------------------------------------------------------------------------
	// CPageTree
	//----------------------------------------------------------------------------------------
	class CPageTree : public CDictObject
	{
	public:
		CPageTree(CXref* pXref);
		void AddPage(CDictObject* pPage);
		EDictType GetDictType() const
		{
			return dict_type_PAGES;
		}
	private:
		CNumberObject* m_pCount;
		CArrayObject*  m_pPages;
	};
	//----------------------------------------------------------------------------------------
	// CPage
	//----------------------------------------------------------------------------------------
	class CPage : public CDictObject
	{
	public:
		CPage(CXref* pXref, CPageTree* pParent, CDocument* pDocument);
		~CPage();

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
		void      SetExtGrState(CExtGrState* pExtGrState);
		void      AddAnnotation(CAnnotation* pAnnot);
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

		void      DrawTextArray(double dX, double dY, const BYTE** ppTexts, unsigned int* pLens, unsigned int unCount, double* pShifts);


		void      ExecuteXObject(CXObject* pXObject);
		void      DrawImage(CImageDict* pImage, double dX, double dY, double dWidth, double dHeight);
		void      SetPatternColorSpace(CImageTilePattern* pPattern);
		void      SetFilter(unsigned int unFiler);
		CMatrix*  GetTransform();
		void      AddGroup(CDictObject* pDict);

	private:

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
		CDictObject* m_pContents;
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
}

#endif // _PDF_WRITER_SRC_PAGES_H

