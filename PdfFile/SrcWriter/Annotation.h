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
#ifndef _PDF_WRITER_SRC_ANNOTATION_H
#define _PDF_WRITER_SRC_ANNOTATION_H

#include "Objects.h"
#include "Types.h"
#include "Pages.h"
#include "Document.h"

namespace PdfWriter
{
	class CDestination;

	enum EBorderSubtype
	{
		border_subtype_Solid,
		border_subtype_Beveled,
		border_subtype_Dashed,
		border_subtype_Inset,
		border_subtype_Underlined
	};
	enum EAnnotType
	{
		AnnotUnknown        = -1,
		AnnotText           = 0,
		AnnotLink           = 1,
		AnnotSound          = 2,
		AnnotFreeText       = 3,
		AnnotStamp          = 4,
		AnnotSquare         = 5,
		AnnotCircle         = 6,
		AnnotStrikeOut      = 7,
		AnnotHighLight      = 8,
		AnnotUnderline      = 9,
		AnnotInk            = 10,
		AnnotFileAttachment = 11,
		AnnotPopup          = 12,
		AnnotLine           = 13,
		AnnotSquiggly       = 14,
		AnnotPolygon        = 15,
		AnnotPolyLine       = 16,
		AnnotCaret          = 17,
		AnnotWidget         = 18
	};
	enum EAnnotHighlightMode
	{
		AnnotNoHighlight = 0,
		AnnotInvertBox,
		AnnotInvertBorder,
		AnnotDownAppearance,
		AnnotHighlightModeEOF
	};
	enum EAnnotIcon
	{
		AnnotIconComment      = 0,
		AnnotIconKey          = 1,
		AnnotIconNote         = 2,
		AnnotIconHelp         = 3,
		AnnotIconNewParagraph = 4,
		AnnotIconParagraph    = 5,
		AnnotIconInsert       = 6,

		AnnotIconMin          = 0,
		AnnotIconMax          = 6
	};

	class CAction : public CDictObject
	{
	public:
		std::string m_sType;
		CAction(CXref* pXref);

		void SetType(const std::wstring& wsType);
		void SetNext(CAction* pNext);
	};
	class CActionResetForm : public CAction
	{
	public:
		CActionResetForm(CXref* pXref);

		void SetFlags(int nFlag);
		void SetFields(const std::vector<std::wstring>& arrFileds);
	};
	class CActionJavaScript : public CAction
	{
	public:
		CActionJavaScript(CXref* pXref);

		void SetJS(const std::wstring& wsJS);
	};
	class CActionGoTo : public CAction
	{
	public:
		CActionGoTo(CXref* pXref);

		void SetDestination(CDestination* pDest);
	};
	class CActionURI : public CAction
	{
	public:
		CActionURI(CXref* pXref);

		void SetURI(const std::wstring& wsURI);
	};
	class CActionHide : public CAction
	{
	public:
		CActionHide(CXref* pXref);

		void SetH(BYTE nH);
		void SetT(const std::vector<std::wstring>& arrT);
	};
	class CActionNamed : public CAction
	{
	public:
		CActionNamed(CXref* pXref);

		void SetN(const std::wstring& wsN);
	};

	class CAnnotation : public CDictObject
	{
	protected:
		CAnnotation(CXref* pXref, EAnnotType eType);

		CXref* m_pXref;
		TRect  m_oRect;
		double m_dPageWidth  = 0;
		double m_dPageHeight = 0;
		CDocument* m_pDocument;

	public:
		EDictType GetDictType() const
		{
			return dict_type_ANNOTATION;
		}
		virtual EAnnotType GetAnnotationType() const
		{
			return AnnotUnknown;
		}

		virtual bool isMarkup() const
		{
			return false;
		}

		void SetRect(const TRect& oRect);
		void SetBorder(BYTE nType, double dWidth, double dDashesAlternating = 0.0, double dGaps = 0.0);
		void SetAnnotFlag(const int& nAnnotFlag);
		void SetPage(CPage* pPage);
		void SetBE(BYTE nType, const double& dBE);
		void SetContents(const std::wstring& wsText);
		void SetNM(const std::wstring& wsNM);
		void SetLM(const std::wstring& wsLM);
		void SetC(const std::vector<double>& arrC);
		// TODO AP Необходимо генерировать внешний вид аннотации как у Widget
		virtual void CreateAP();
		TRect GetRect() { return m_oRect; }
		void SetXref(CXref* pXref) { m_pXref = pXref; }
		void SetDocument(CDocument* pDocument);
		CDocument* GetDocument();
	};
	class CPopupAnnotation : public CAnnotation
	{
	public:
		CPopupAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotPopup;
		}

		void SetOpen(bool bOpen);

		void SetParentID(CAnnotation* pAnnot);
	};
	class CMarkupAnnotation : public CAnnotation
	{
	protected:
		CMarkupAnnotation(CXref* pXref, EAnnotType eType);

	public:
		bool isMarkup() const override
		{
			return true;
		}

		void SetRT(const BYTE& nRT);
		void SetCA(const double& dCA);
		void SetT(const std::wstring& wsT);
		void SetRC(const std::wstring& wsRC);
		void SetCD(const std::wstring& wsCD);
		void SetSubj(const std::wstring& wsSubj);

		void SetIRTID(CAnnotation* pAnnot);
		CPopupAnnotation* CreatePopup();
		virtual void CreateAP() override;
	};
	class CLinkAnnotation : public CAnnotation
	{
	public:
		CLinkAnnotation(CXref* pXref, CDestination* pDestination);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotLink;
		}
		void SetBorderStyle  (float fWidth, unsigned short nDashOn, unsigned short nDashOff);
		void SetHighlightMode(EAnnotHighlightMode eMode);
	};
	class CTextAnnotation : public CMarkupAnnotation
	{
	private:
		BYTE m_nName;
	public:
		CTextAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotText;
		}

		void SetOpen(bool bOpen);
		void SetName(BYTE nName);
		void SetState(BYTE nState);
		void SetStateModel(BYTE nStateModel);

		void CreateAP() override;
	};
	class CUriLinkAnnotation : public CAnnotation
	{
	public:
		CUriLinkAnnotation(CXref* pXref, const char* sUri);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotLink;
		}
	};
	class CInkAnnotation : public CMarkupAnnotation
	{
	public:
		CInkAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotInk;
		}

		void SetInkList(const std::vector< std::vector<double> >& arrInkList);
	};
	class CLineAnnotation : public CMarkupAnnotation
	{
	public:
		CLineAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotLine;
		}

		void SetCap(bool bCap);
		void SetIT(const BYTE& nIT);
		void SetCP(const BYTE& nCP);
		void SetLL(const double& dLL);
		void SetLLE(const double& dLLE);
		void SetLLO(const double& dLLO);
		void SetLE(const BYTE& nLE1, const BYTE& nLE2);
		void SetL(const double& dL1, const double& dL2, const double& dL3, const double& dL4);
		void SetCO(const double& dCO1, const double& dCO2);
		void SetIC(const std::vector<double>& arrIC);
	};
	class CTextMarkupAnnotation : public CMarkupAnnotation
	{
	private:
		EAnnotType m_nSubtype;
	public:
		CTextMarkupAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return m_nSubtype;
		}

		void SetSubtype(const BYTE& nSubtype);
		void SetQuadPoints(const std::vector<double>& arrQuadPoints);
	};
	class CSquareCircleAnnotation : public CMarkupAnnotation
	{
	private:
		EAnnotType m_nSubtype;
	public:
		CSquareCircleAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return m_nSubtype;
		}

		void SetSubtype(const BYTE& nSubtype);
		void SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4);
		void SetIC(const std::vector<double>& arrIC);
	};
	class CPolygonLineAnnotation : public CMarkupAnnotation
	{
	private:
		EAnnotType m_nSubtype;
	public:
		CPolygonLineAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return m_nSubtype;
		}

		void SetIT(const BYTE& nIT);
		void SetSubtype(const BYTE& nSubtype);
		void SetLE(const BYTE& nLE1, const BYTE& nLE2);
		void SetIC(const std::vector<double>& arrIC);
		void SetVertices(const std::vector<double>& arrVertices);
	};
	class CFreeTextAnnotation : public CMarkupAnnotation
	{
	public:
		CFreeTextAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotFreeText;
		}

		void SetQ(const BYTE& nQ);
		void SetIT(const BYTE& nIT);
		void SetLE(const BYTE& nLE);
		void SetDS(const std::wstring& wsDS);
		void SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4);
		void SetCL(const std::vector<double>& arrCL);
	};
	class CCaretAnnotation : public CMarkupAnnotation
	{
	public:
		CCaretAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotCaret;
		}

		void SetSy(const BYTE& nSy);
		void SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4);
	};
	class CWidgetAnnotation : public CAnnotation
	{
	protected:
		CDictObject* m_pMK;
		CDictObject* m_pParent;
		CDictObject* m_pAA;
		CDictObject* m_pA;
		std::string m_sDAforAP;

		void CheckMK();

	public:
		CWidgetAnnotation(CXref* pXref, EAnnotType eType);

		void SetDA(CFontDict* pFont, const double& dFontSize, const double& dFontSizeAP, const std::vector<double>& arrTC);
		CDictObject* GetObjOwnValue(const std::string& sV);

		void SetQ(const BYTE& nQ);
		void SetH(const BYTE& nH);
		void SetR(const int& nR);
		void SetFlag    (const int& nFlag);
		void SetParent(CDictObject* pParent);
		void SetTU(const std::wstring& wsTU);
		void SetDS(const std::wstring& wsDS);
		virtual void SetDV(const std::wstring& wsDV);
		void SetT (const std::wstring& wsT);
		void SetBC(const std::vector<double>& arrBC);
		void SetBG(const std::vector<double>& arrBG);
		void AddAction(CAction* pAction);

		std::string GetDAforAP() { return m_sDAforAP; }
	};
	class CButtonWidget : public CWidgetAnnotation
	{
	private:
		EAnnotType m_nSubtype;
		CDictObject* m_pIF;
		std::string m_sAP_N_Yes;

		void CheckIF();

	public:
		CButtonWidget(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return m_nSubtype;
		}

		void SetV(bool bV);
		void SetDV(const std::wstring& wsDV) override;
		void SetS(const BYTE& nS);
		void SetTP(const BYTE& nTP);
		void SetSW(const BYTE& nSW);
		void SetStyle(const BYTE& nStyle);
		void SetIFFlag(const int& nIFFlag);
		void SetA(const double& dA1, const double& dA2);
		void SetCA(const std::wstring& wsCA);
		void SetRC(const std::wstring& wsRC);
		void SetAC(const std::wstring& wsAC);
		void SetAP_N_Yes(const std::wstring& wsAP_N_Yes);
	};
	class CTextWidget : public CWidgetAnnotation
	{
	private:
		EAnnotType m_nSubtype;
		std::string m_sV;

	public:
		CTextWidget(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return m_nSubtype;
		}

		void SetMaxLen(const int& nMaxLen);
		void SetV (const std::wstring& wsV);
		void SetRV(const std::wstring& wsRV);

		void CreateAP() override;
		std::string GetV() { return m_sV; }
	};
	class CChoiceWidget : public CWidgetAnnotation
	{
	private:
		EAnnotType m_nSubtype;

	public:
		CChoiceWidget(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return m_nSubtype;
		}

		void SetTI(const int& nTI);
		void SetV(const std::wstring& wsV);
		void SetOpt(const std::vector< std::pair<std::wstring, std::wstring> >& arrOpt);
	};
	class CSignatureWidget : public CWidgetAnnotation
	{
	private:
		EAnnotType m_nSubtype;

	public:
		CSignatureWidget(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return m_nSubtype;
		}
	};

	class CAnnotationAppearance : public CDictObject
	{
	public:
		CAnnotationAppearance(CXref* pXref, CAnnotation* pAnnot);

		void DrawTextComment();
		void DrawTextWidget();

	private:
		CXref*   m_pXref;
		CStream* m_pStream;
		CAnnotation* m_pAnnot;
	};
}
#endif // _PDF_WRITER_SRC_ANNOTATION_H
