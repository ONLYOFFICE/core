﻿/*
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

	class CAnnotation : public CDictObject
	{
	protected:
		CAnnotation(CXref* pXref, EAnnotType eType);

		CXref* m_pXref;
		TRect  m_oRect;
		double m_dPageHeight = 0;

	public:
		int m_nID; // Идентификатор сопоставления

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
		void SetHeight(double dHeight);

		void SetRect(const TRect& oRect);
		void SetBorder(BYTE nType, double dWidth, double dDashesAlternating = 0.0, double dGaps = 0.0);
		void SetID(const int& nID);
		void SetAnnotFlag(const int& nAnnotFlag);
		void SetPage(CPage* pPage);
		void SetBE(BYTE nType, const double& dBE);
		void SetContents(const std::wstring& wsText);
		void SetNM(const std::wstring& wsNM);
		void SetLM(const std::wstring& wsLM);
		void SetC(const std::vector<double>& arrC);
		// TODO AP Необходимо генерировать внешний вид аннотации как у Widget
	};
	class CPopupAnnotation : public CAnnotation
	{
	private:
		int m_nParentID;

	public:
		CPopupAnnotation(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotPopup;
		}

		void SetOpen(bool bOpen);
		void SetParentID(const int& nParentID);

		int GetParentID() const
		{
			return m_nParentID;
		}
		void SetParentID(CAnnotation* pAnnot);
	};
	class CMarkupAnnotation : public CAnnotation
	{
	protected:
		CMarkupAnnotation(CXref* pXref, EAnnotType eType);

		int m_nIRTID;

	public:
		bool isMarkup() const override
		{
			return true;
		}

		void SetRT(const BYTE& nRT);
		void SetIRTID(const int& nIRTID);
		void SetCA(const double& dCA);
		void SetT(const std::wstring& wsT);
		void SetRC(const std::wstring& wsRC);
		void SetCD(const std::wstring& wsCD);
		void SetSubj(const std::wstring& wsSubj);

		int GetIRTID() const
		{
			return m_nIRTID;
		}
		void SetIRTID(CAnnotation* pAnnot);
		CPopupAnnotation* CreatePopup();
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
		CDocument* m_pDocument;

		void CheckMK();

	public:
		CWidgetAnnotation(CXref* pXref, EAnnotType eType);

		void SetDocument(CDocument* pDocument);
		void SetDA(CFontDict* pFont, const double& dFontSize, const std::vector<double>& arrTC);

		void SetQ(const BYTE& nQ);
		void SetH(const BYTE& nH);
		void SetR(const int& nR);
		void SetFlag    (const int& nFlag);
		void SetParentID(const int& nParentID);
		void SetTU(const std::wstring& wsTU);
		void SetDS(const std::wstring& wsDS);
		virtual void SetDV(const std::wstring& wsDV);
		void SetT (const std::wstring& wsT);
		void SetBC(const std::vector<double>& arrBC);
		void SetBG(const std::vector<double>& arrBG);
	};
	class CButtonWidget : public CWidgetAnnotation
	{
	private:
		EAnnotType m_nSubtype;
		CDictObject* m_pIF;

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

	public:
		CTextWidget(CXref* pXref);
		EAnnotType GetAnnotationType() const override
		{
			return m_nSubtype;
		}

		void SetMaxLen(const int& nMaxLen);
		void SetV (const std::wstring& wsV);
		void SetRV(const std::wstring& wsRV);
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
}
#endif // _PDF_WRITER_SRC_ANNOTATION_H
