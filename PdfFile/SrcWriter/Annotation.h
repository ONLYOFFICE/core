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
#include "Field.h"

namespace PdfWriter
{
	class CDestination;

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

		void SetDestination(CArrayObject* pDest);
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

		struct CBorderType
		{
			CBorderType()
			{
				bHave  = false;
				nType  = 0;
				dWidth = 0;
			}

			bool bHave;
			BYTE nType;
			double dWidth;
			std::vector<double> arrDash;
		};

		CBorderType m_oBorder;

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
		void SetBorder(BYTE nType, double dWidth, const std::vector<double>& arrDash);
		void SetEmptyBorder();
		void SetAnnotFlag(const int& nAnnotFlag);
		void SetPage(CPage* pPage);
		void SetBE(BYTE nType, const double& dBE);
		void SetContents(const std::wstring& wsText);
		void SetNM(const std::wstring& wsNM);
		void SetLM(const std::wstring& wsLM);
		void SetC(const std::vector<double>& arrC);

		TRect& GetRect() { return m_oRect; }
		void SetXref(CXref* pXref) { m_pXref = pXref; }
		void SetDocument(CDocument* pDocument);
		CDocument* GetDocument();
		bool HaveBorder()       { return m_oBorder.bHave; }
		BYTE GetBorderType()    { return m_oBorder.nType; }
		double GetBorderWidth() { return m_oBorder.dWidth; }
		std::string GetBorderDash();
		double GetWidth()  { return abs(m_oRect.fRight - m_oRect.fLeft); }
		double GetHeight() { return abs(m_oRect.fTop - m_oRect.fBottom); }
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

		void SetRT(BYTE nRT);
		void SetCA(const double& dCA);
		void SetT(const std::wstring& wsT);
		void SetRC(const std::wstring& wsRC);
		void SetCD(const std::wstring& wsCD);
		void SetSubj(const std::wstring& wsSubj);

		void RemoveAP();
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

		void SetAP();
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
		void SetIT(BYTE nIT);
		void SetCP(BYTE nCP);
		void SetLL(const double& dLL);
		void SetLLE(const double& dLLE);
		void SetLLO(const double& dLLO);
		void SetLE(BYTE nLE1, BYTE nLE2);
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

		void SetSubtype(BYTE nSubtype);
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

		void SetSubtype(BYTE nSubtype);
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

		void SetIT(BYTE nIT);
		void SetSubtype(BYTE nSubtype);
		void SetLE(BYTE nLE1, BYTE nLE2);
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

		void SetQ(BYTE nQ);
		void SetIT(BYTE nIT);
		void SetLE(BYTE nLE);
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

		void SetSy(BYTE nSy);
		void SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4);
	};
	class CWidgetAnnotation : public CAnnotation
	{
	protected:
		EWidgetType m_nSubtype;
		CDictObject* m_pMK;
		CDictObject* m_pParent;

		CFontCidTrueType* m_pFont;
		double m_dFontSize;
		bool m_bBold;
		bool m_bItalic;

		CAnnotAppearance* m_pAppearance;
		double m_dFontSizeAP;
		std::vector<double> m_arrTC;
		std::vector<double> m_arrBC;
		std::vector<double> m_arrBG;
		BYTE m_nQ;

		void CheckMK();

	public:
		CWidgetAnnotation(CXref* pXref, EAnnotType eType);
		EAnnotType GetAnnotationType() const override
		{
			return AnnotWidget;
		}
		EWidgetType GetWidgetType() const
		{
			return m_nSubtype;
		}

		void SetSubtype(BYTE nSubtype);
		void SetDA(CFontDict* pFont, const double& dFontSize, const double& dFontSizeAP, const std::vector<double>& arrTC);
		void SetFont(CFontCidTrueType* pFont, double dFontSize, bool bBold, bool bItalic);
		CDictObject* GetObjOwnValue(const std::string& sV);
		CObjectBase* GetObjValue(const std::string& sV);

		void SetQ(BYTE nQ);
		void SetH(BYTE nH);
		void SetR(const int& nR);
		virtual void SetFlag (const int& nFlag);
		void SetParent(CDictObject* pParent);
		void SetTU(const std::wstring& wsTU);
		void SetDS(const std::wstring& wsDS);
		void SetDV(const std::wstring& wsDV);
		void SetT (const std::wstring& wsT);
		void SetBC(const std::vector<double>& arrBC);
		void SetBG(const std::vector<double>& arrBG);
		void AddAction(CAction* pAction);

		std::string GetDAforAP(CFontDict* pFont);
		std::string GetBGforAP(double dDiff = 0);
		std::string GetBCforAP();
		CFontCidTrueType* GetFont() { return m_pFont; }
		double GetFontSize()   { return m_dFontSize; }
		bool GetFontIsBold()   { return m_bBold; }
		bool GetFontIsItalic() { return m_bItalic; }
		bool HaveBG() { return !m_arrBG.empty(); }
		bool HaveBC() { return !m_arrBC.empty(); }
		BYTE GetQ() { return m_nQ; }

		void SetEmptyAP();
		void SetAP(const std::wstring& wsValue, unsigned short* pCodes, unsigned int unCount, double dX, double dY, CFontCidTrueType** ppFonts, double* pShifts);
		void StartAP();
		void AddLineToAP(const double& dX, const double& dY, unsigned short* pCodes, const unsigned int& unCodesCount, CFontCidTrueType** ppFonts = NULL, const double* pShifts = NULL);
		void EndAP();
	};
	class CPushButtonWidget : public CWidgetAnnotation
	{
	private:
		bool m_bRespectBorders;
		bool m_bConstantProportions;
		BYTE m_nScaleType;
		BYTE m_nTP;
		double m_dShiftX;
		double m_dShiftY;
		std::wstring m_wsCA;
		std::wstring m_wsRC;
		std::wstring m_wsAC;
		CDictObject* m_pIF;

		void CheckIF();

	public:
		CPushButtonWidget(CXref* pXref);

		void SetV(const std::wstring& wsV);
		void SetS(BYTE nS);
		void SetTP(BYTE nTP);
		void SetSW(BYTE nSW);
		void SetIFFlag(const int& nIFFlag);
		virtual void SetFlag (const int& nFlag) override;
		void SetI(const int& nI);
		void SetRI(const int& nRI);
		void SetIX(const int& nIX);
		void SetA(const double& dA1, const double& dA2);
		void SetCA(const std::wstring& wsCA);
		void SetRC(const std::wstring& wsRC);
		void SetAC(const std::wstring& wsAC);

		void SetAP(CXObject* pForm, BYTE nAP, unsigned short* pCodes, unsigned int unCount, double dX, double dY, double dLineW, double dLineH, CFontCidTrueType** ppFonts);
		const std::wstring& GetCA() { return m_wsCA; }
		const std::wstring& GetRC() { return m_wsRC; }
		const std::wstring& GetAC() { return m_wsAC; }
		BYTE GetTP() { return m_nTP; }
		bool GetRespectBorder() { return m_bRespectBorders; }

		int m_nI, m_nRI, m_nIX;
	};
	class CCheckBoxWidget : public CWidgetAnnotation
	{
	private:
		std::string m_sAP_N_Yes;

	public:
		CCheckBoxWidget(CXref* pXref);

		void SetV(const std::wstring& wsV);
		std::wstring SetStyle(BYTE nStyle);
		void SetAP_N_Yes(const std::wstring& wsAP_N_Yes);
		virtual void SetFlag (const int& nFlag);

		void SwitchAP(const std::string& sV);
	};
	class CTextWidget : public CWidgetAnnotation
	{
	private:
		std::string m_sV;
		bool m_bAPV;

	public:
		CTextWidget(CXref* pXref);

		void SetMaxLen(const int& nMaxLen);
		void SetV (const std::wstring& wsV);
		void SetRV(const std::wstring& wsRV);
		void SetAPV() { m_bAPV = true; }

		bool IsCombFlag();
		bool IsMultiLine();
		unsigned int GetMaxLen();
		bool HaveAPV() { return m_bAPV; }
	};
	class CChoiceWidget : public CWidgetAnnotation
	{
	private:
		std::vector< std::pair<std::wstring, std::wstring> > m_arrOpt;
		double m_dHeight;
		int m_nTI;
		std::vector<int> m_arrIndex;
	public:
		CChoiceWidget(CXref* pXref);

		virtual void SetFlag (const int& nFlag) override;
		void SetTI(const int& nTI);
		void SetV(const std::wstring& wsV);
		void SetI(const std::vector<int>& arrI);
		void SetV(const std::vector<std::wstring>& arrV);
		void SetOpt(const std::vector< std::pair<std::wstring, std::wstring> >& arrOpt);

		std::wstring GetValue(const std::wstring& wsExportV);
		void SetListBoxHeight(double dHeight) { m_dHeight = dHeight; }
		double GetListBoxHeight() { return m_dHeight; }
		std::wstring SetListBoxIndex(const std::vector<std::wstring>& arrV);
		std::vector<int> GetListBoxIndex() { return m_arrIndex; }
	};
	class CSignatureWidget : public CWidgetAnnotation
	{
	public:
		CSignatureWidget(CXref* pXref);
	};
}
#endif // _PDF_WRITER_SRC_ANNOTATION_H
