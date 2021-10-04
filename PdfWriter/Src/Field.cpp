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
#include "Field.h"
#include "Pages.h"
#include "Streams.h"
#include "Document.h"
#include "ResourcesDictionary.h"
#include "Types.h"

#include <algorithm>
#include <math.h>

#include "../../DesktopEditor/common/SystemUtils.h"
#include "../../DesktopEditor/common/File.h"

#ifndef BS_DEF_WIDTH
#define BS_DEF_WIDTH 1
#endif

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CFieldBase
	//----------------------------------------------------------------------------------------
	CFieldBase::CFieldBase(CXref* pXref, CDocument* pDocument)
	{
		pXref->Add(this);
		Add("Ff", (unsigned int)0);

		m_pXref     = pXref;
		m_pDocument = pDocument;

		m_nBorderType = 0;
		m_dBorderSize = 0;

		m_bAutoFit = false;

		m_bShd = false;

		m_pMK = NULL;
		
		m_pParent = NULL;
		m_pKids   = NULL;
	}
	void CFieldBase::SetReadOnlyFlag(bool isReadOnly)
	{
		SetFlag(isReadOnly, 1 << 0);
	}
	void CFieldBase::SetRequiredFlag(bool isRequired)
	{
		SetFlag(isRequired, 1 << 1);
	}
	void CFieldBase::SetNoExportFlag(bool isNoExport)
	{
		SetFlag(isNoExport, 1 << 2);
	}
	void CFieldBase::SetFlag(bool isFlag, int nBit)
	{
		int nFlags = ((CNumberObject*)this->Get("Ff"))->Get();

		if ((nFlags & nBit) && !isFlag)
		{
			nFlags &= 0xffffffff ^ nBit;
		}
		else if (!(nFlags & nBit) && isFlag)
		{
			nFlags |= nBit;
		}

		Add("Ff", nFlags);
	}
	void CFieldBase::AddPageRect(CPage* pPage, const TRect& oRect)
	{
		if (!pPage)
			return;

		pPage->AddAnnotation(this);

		Add("Type", "Annot");
		Add("Subtype", "Widget");

		Add("P", pPage);

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("Rect", pArray);

		if (oRect.fTop < oRect.fBottom)
		{
			pArray->Add(oRect.fLeft);
			pArray->Add(oRect.fTop);
			pArray->Add(oRect.fRight);
			pArray->Add(oRect.fBottom);

			m_oRect.fLeft = oRect.fLeft;
			m_oRect.fTop = oRect.fTop;
			m_oRect.fRight = oRect.fRight;
			m_oRect.fBottom = oRect.fBottom;
		}
		else
		{
			pArray->Add(oRect.fLeft);
			pArray->Add(oRect.fBottom);
			pArray->Add(oRect.fRight);
			pArray->Add(oRect.fTop);

			m_oRect.fLeft = oRect.fLeft;
			m_oRect.fTop = oRect.fBottom;
			m_oRect.fRight = oRect.fRight;
			m_oRect.fBottom = oRect.fTop;
		}

		Add("F", 4);		
	}
	void CFieldBase::SetFieldName(const std::string& sName, bool isSkipCheck)
	{
		if (isSkipCheck || !m_pDocument->CheckFieldName(this, sName))
			Add("T", new CStringObject(sName.c_str()));
	}
	void CFieldBase::SetFieldName(const std::wstring& wsName, bool isSkipCheck) 
	{
		std::string sName = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsName);
		if (isSkipCheck || !m_pDocument->CheckFieldName(this, sName))
			Add("T", new CStringObject(sName.c_str(), true));
	}
	void CFieldBase::ClearKidRecords()
	{
		Remove("T");
		Remove("FT");
		Remove("Ff");
	}
	void CFieldBase::SetFieldHint(const std::wstring& wsHint)
	{
		if (wsHint != L"")
		{
			std::string sHint = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsHint);
			Add("TU", new CStringObject(sHint.c_str(), true));
		}
		else
		{
			Remove("TU");
		}
	}
	TRect& CFieldBase::GetRect()
	{
		return m_oRect;
	}
	CResourcesDict* CFieldBase::GetResourcesDict()
	{
		return m_pDocument->GetFieldsResources();
	}
	void CFieldBase::SetDefaultAppearance(CFontDict* pFont, const double& dFontSize, const TRgb& oColor)
	{
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();

		const char* sFontName = pFieldsResources->GetFontName(pFont);
		if (!sFontName)
			return;

		std::string sDA;
		sDA.append(std::to_string(oColor.r));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.g));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.b));
		sDA.append(" rg /");
		sDA.append(sFontName);

		if (IsAutoFit())
		{
			sDA.append(" 0 Tf");
		}
		else
		{
			sDA.append(" ");
			sDA.append(std::to_string(dFontSize));
			sDA.append(" Tf");
		}

		Add("DA", new CStringObject(sDA.c_str()));
	}
	void CFieldBase::SetTextAppearance(const std::wstring& wsValue, unsigned char* pCodes, unsigned int unCount, CFontDict* pFont, const TRgb& oColor, const double& dAlpha, double dFontSize, double dX, double dY, double* pShifts, unsigned int unShiftsCount)
	{
		CAnnotAppearance* pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAppearance);

		CAnnotAppearanceObject* pNormal = pAppearance->GetNormal();

		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();
		
		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}

		pNormal->DrawSimpleText(wsValue, pCodes, unCount, pFieldsResources->GetFontName(pFont), dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop), pShifts, unShiftsCount);
	}
	void CFieldBase::SetTextValue(const std::wstring &wsValue)
	{
		std::string sValue = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsValue);
		Add("V", new CStringObject(sValue.c_str(), true));
	}
	void CFieldBase::SetFieldBorder(const EBorderSubtype& eSubtype, const TRgb& oColor, const double& dWidth, const unsigned short& nDashOn, const unsigned short& nDashOff, const unsigned short& nDashPhase)
	{		
		CDictObject* pBorderStyleDict = new CDictObject();
		if (!pBorderStyleDict)
			return;

		Add("BS", pBorderStyleDict);

		// Почему то некоторые PDF-ридеры плохо воспринимают значения от 0 до 1, поэтому ставим 1
		if (dWidth < 0.01)
			pBorderStyleDict->Add("W", 0);
		else if (dWidth < 1.01)
			pBorderStyleDict->Add("W", 1);
		else
			pBorderStyleDict->Add("W", dWidth);
		if (dWidth < 0.01)
			return;

		if (border_subtype_Dashed == eSubtype)
		{
			CArrayObject* pDash = new CArrayObject();
			if (!pDash)
				return;

			pBorderStyleDict->Add("D", pDash);
			pBorderStyleDict->Add("Type", "Border");
			pDash->Add(nDashOn);
			pDash->Add(nDashOff);

			if (0 != nDashPhase)
				pDash->Add(nDashOff);
		}

		switch (eSubtype)
		{
			case border_subtype_Solid:      pBorderStyleDict->Add("S", "S"); break;
			case border_subtype_Dashed:     pBorderStyleDict->Add("S", "D"); break;
			case border_subtype_Beveled:    pBorderStyleDict->Add("S", "B"); break;
			case border_subtype_Inset:      pBorderStyleDict->Add("S", "I"); break;
			case border_subtype_Underlined: pBorderStyleDict->Add("S", "U"); break;
		}

		if (!m_pMK)
		{
			m_pMK = new CDictObject();
			Add("MK", m_pMK);
		}

		if (!m_pMK)
			return;

		CArrayObject* pColor = new CArrayObject();
		pColor->Add(oColor.r);
		pColor->Add(oColor.g);
		pColor->Add(oColor.b);
		m_pMK->Add("BC", pColor);

		m_nBorderType  = 1;
		m_dBorderSize  = dWidth;
		m_oBorderColor = oColor;
	}
	bool CFieldBase::HaveBorder() const
	{
		return m_nBorderType == 1;
	}
	const double& CFieldBase::GetBorderSize() const
	{
		return m_dBorderSize;
	}
	const TRgb& CFieldBase::GetBorderColor() const
	{
		return m_oBorderColor;
	}
	const bool& CFieldBase::IsAutoFit() const
	{
		return m_bAutoFit;
	}
	void CFieldBase::SetAutoFit(const bool& isAutoFit)
	{
		m_bAutoFit = isAutoFit;
	}
	bool CFieldBase::HaveShd() const
	{
		return m_bShd;
	}
	void CFieldBase::SetShd(const TRgb& oRgb)
	{
		m_oShdColor = oRgb;

		if (!m_pMK)
		{
			m_pMK = new CDictObject();
			Add("MK", m_pMK);
		}

		if (!m_pMK)
			return;

		CArrayObject* pBG = new CArrayObject();
		if (pBG)
		{
			pBG->Add(oRgb.r);
			pBG->Add(oRgb.g);
			pBG->Add(oRgb.b);
			m_pMK->Add("BG", pBG);
		}
	}
	const TRgb& CFieldBase::GetShdColor() const
	{
		return m_oShdColor;
	}
	void CFieldBase::SetParent(CFieldBase* pParent)
	{
		m_pParent = pParent;
		Add("Parent", pParent);
	}
	void CFieldBase::AddKid(CFieldBase* pChild)
	{
		if (!m_pKids)
		{
			m_pKids = new CArrayObject();
			Add("Kids", m_pKids);
		}

		m_pKids->Add(pChild);
	}
	int CFieldBase::GetKidsCount() const
	{
		if (!m_pKids)
			return 0;

		return m_pKids->GetCount();
	}
	int CFieldBase::GetFieldFlag() const
	{
		return ((CNumberObject*)Get("Ff"))->Get();
	}
	const char* CFieldBase::GetFieldType() const
	{
		return ((CNameObject*)Get("FT"))->Get();
	}
	void CFieldBase::SetAlign(const EFieldAlignType& eType)
	{
		Add("Q", (int)eType);
	}
	//----------------------------------------------------------------------------------------
	// CTextField
	//----------------------------------------------------------------------------------------
	CTextField::CTextField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		Add("FT", "Tx");

		m_bAutoFit = false;
	}
	void CTextField::SetMultilineFlag(bool isMultiLine)
	{
		SetFlag(isMultiLine, 1 << 12);
	}
	void CTextField::SetPasswordFlag(bool isPassword) 
	{
		SetFlag(isPassword, 1 << 13);
	}
	void CTextField::SetFileSelectFlag(bool isFileSelect)
	{
		SetFlag(isFileSelect, 1 << 20);
	}
	void CTextField::SetDoNotSpellCheckFlag(bool isDoNotSpellCheck)
	{
		SetFlag(isDoNotSpellCheck, 1 << 22);
	}
	void CTextField::SetDoNotScrollFlag(bool isDoNotScroll)
	{
		SetFlag(isDoNotScroll, 1 << 23);
	}
	void CTextField::SetCombFlag(bool isComb)
	{
		SetFlag(isComb, 1 << 24);
	}
	void CTextField::SetRichTextFlag(bool isRichText)
	{
		SetFlag(isRichText, 1 << 25);
	}
	void CTextField::SetMaxLen(int nMaxLen)
	{
		if (nMaxLen > 0)
			Add("MaxLen", nMaxLen);
	}
	int CTextField::GetMaxLen() const
	{
		CNumberObject* oMaxLen = (CNumberObject*)Get("MaxLen");
		if (oMaxLen)
			return oMaxLen->Get();

		return 0;
	}
	bool CTextField::IsCombFlag() const
	{
		int nFlags = ((CNumberObject*)this->Get("Ff"))->Get();
		return (nFlags & (1 << 24));
	}
	//----------------------------------------------------------------------------------------
	// CChoiceField
	//----------------------------------------------------------------------------------------
	CChoiceField::CChoiceField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		m_pOpt = NULL;

		Add("FT", "Ch");
	}
	void CChoiceField::SetComboFlag(const bool& isCombo)
	{
		SetFlag(isCombo, 1 << 17);
	}
	void CChoiceField::SetEditFlag(const bool& isEdit)
	{
		SetFlag(isEdit, 1 << 18);
	}
	void CChoiceField::SetSortFlag(const bool& isSort)
	{
		SetFlag(isSort, 1 << 19);
	}
	void CChoiceField::SetMultiSelectFlag(const bool& isMultiSelect)
	{
		SetFlag(isMultiSelect, 1 << 21);
	}
	void CChoiceField::SetDoNotSpellCheck(const bool& isDoNotSpellCheck)
	{
		SetFlag(isDoNotSpellCheck, 1 << 22);
	}
	void CChoiceField::AddOption(const std::wstring& wsOption)
	{
		if (!m_pOpt)
		{
			m_pOpt = new CArrayObject();
			Add("Opt", m_pOpt);
		}

		if (!m_pOpt)
			return;

		std::string sOption = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsOption);
		m_pOpt->Add(new CStringObject(sOption.c_str(), true));
	}
	//----------------------------------------------------------------------------------------
	// CCheckBoxField
	//----------------------------------------------------------------------------------------
	CCheckBoxField::CCheckBoxField(CXref* pXref, CDocument* pDocument, CRadioGroupField* pGroup, const char* sYesName) : CFieldBase(pXref, pDocument)
	{
		Add("FT", "Btn");

		m_pGroup = pGroup;

		if (pGroup)
			Add("Parent", pGroup);

		m_sYesName = sYesName ? sYesName : "Yes";
		Add("AS", "Off");
	}
	void CCheckBoxField::SetAppearance(const std::wstring& wsYesValue, unsigned char* pYesCodes, unsigned int unYesCount, CFontDict* pYesFont,
									   const std::wstring& wsOffValue, unsigned char* pOffCodes, unsigned int unOffCount, CFontDict* pOffFont,
									   const TRgb& oColor, const double& dAlpha, double dFontSize, double dX, double dY)
	{
		CCheckBoxAnnotAppearance* pAppearance = new CCheckBoxAnnotAppearance(m_pXref, this, m_sYesName.c_str());
		Add("AP", pAppearance);

		if (!m_pMK)
		{
			m_pMK = new CDictObject();
			m_pXref->Add(m_pMK);
			Add("MK", m_pMK);
		}

		CArrayObject* pArray = new CArrayObject();
		pArray->Add(0);
		m_pMK->Add("BC", pArray);

		pArray = new CArrayObject();
		pArray->Add(1);
		m_pMK->Add("BG", pArray);
		
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();

		const char* sFontName = pFieldsResources->GetFontName(pYesFont);
		if (!sFontName)
			return;

		std::string sDA;
		sDA.append(std::to_string(oColor.r));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.g));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.b));
		sDA.append(" rg /");
		sDA.append(sFontName);
		sDA.append(" ");
		sDA.append(std::to_string(0));
		sDA.append(" Tf");
		Add("DA", new CStringObject(sDA.c_str()));

		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}


		pAppearance->GetYesN()->DrawSimpleText(wsYesValue, pYesCodes, unYesCount, pFieldsResources->GetFontName(pYesFont), dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
		pAppearance->GetOffN()->DrawSimpleText(wsOffValue, pOffCodes, unOffCount, pFieldsResources->GetFontName(pOffFont), dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));

		pAppearance->GetYesD()->DrawSimpleText(wsYesValue, pYesCodes, unYesCount, pFieldsResources->GetFontName(pYesFont), dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
		pAppearance->GetOffD()->DrawSimpleText(wsOffValue, pOffCodes, unOffCount, pFieldsResources->GetFontName(pOffFont), dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
	}
	void CCheckBoxField::SetAppearance(const int& nType, const TRgb& oColor, const double& dAlpha, double dFontSize, double dX, double dY)
	{
		if (!m_pMK)
		{
			m_pMK = new CDictObject();
			m_pXref->Add(m_pMK);
			Add("MK", m_pMK);
		}

		CArrayObject* pArray = new CArrayObject();
		pArray->Add(3);
		m_pMK->Add("BC", pArray);

		pArray = new CArrayObject();
		pArray->Add(1);
		pArray->Add(1);
		pArray->Add(1);
		m_pMK->Add("BG", pArray);

		CCheckBoxAnnotAppearance* pAppearance = new CCheckBoxAnnotAppearance(m_pXref, this, m_sYesName.c_str());
		Add("AP", pAppearance);

		CFontDict* pFont = (CFontDict*)m_pDocument->GetDefaultCheckboxFont();
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();

		const char* sFontName = pFieldsResources->GetFontName(pFont);
		if (!sFontName)
			return;

		std::string sDA;
		sDA.append(std::to_string(oColor.r));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.g));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.b));
		sDA.append(" rg /");
		sDA.append(sFontName);
		sDA.append(" ");
		sDA.append(std::to_string(0));
		sDA.append(" Tf");
		Add("DA", new CStringObject(sDA.c_str()));

		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}

		double dW = fabs(m_oRect.fRight - m_oRect.fLeft);
		double dH = fabs(m_oRect.fBottom - m_oRect.fTop);

		dFontSize = dH * 31.014 / 33.8712;
		dX        = 5.4407 * dW / 37.119000;
		dY        = 6.4375 * dH / 33.8712;

		SetFieldBorder(border_subtype_Solid, TRgb(0, 0, 0), 1, 0, 0, 0);
		Remove("BS");

		if (1 == nType)
		{
			m_pMK->Add("CA", new CStringObject("4"));

			pAppearance->GetYesN()->DrawSimpleText(L"4", NULL, 0, pFieldsResources->GetFontName(pFont), dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
			pAppearance->GetOffN()->DrawSimpleText(L"", NULL, 0, pFieldsResources->GetFontName(pFont), dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
		}
		else if (2 == nType)
		{
			m_pMK->Add("CA", new CStringObject("1"));

			pAppearance->GetYesN()->DrawSimpleText(L"1", NULL, 0, pFieldsResources->GetFontName(pFont), dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
			pAppearance->GetOffN()->DrawSimpleText(L"", NULL, 0, pFieldsResources->GetFontName(pFont), dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
		}

	}
	void CCheckBoxField::SetValue(const bool& isYes)
	{
		const char* sValue = (isYes ? m_sYesName.c_str() : "Off");
		Add("AS", sValue);
		Add("V", sValue);
	}
	//----------------------------------------------------------------------------------------
	// CRadioGroupField
	//----------------------------------------------------------------------------------------
	CRadioGroupField::CRadioGroupField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		Add("FT", "Btn");
		SetFlag(true, 1 << 14);
		SetFlag(true, 1 << 15);

		m_pKids = new CArrayObject();

		if (m_pKids)
			Add("Kids", m_pKids);
	}
	CCheckBoxField* CRadioGroupField::CreateKid(const wchar_t* wsChoiceName)
	{
		if (!m_pKids)
			return NULL;

		std::string sName = wsChoiceName ? NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsChoiceName) : "Choice" + std::to_string(m_pKids->GetCount() + 1);
		CCheckBoxField* pKid = new CCheckBoxField(m_pXref, m_pDocument, this, sName.c_str());
		m_pKids->Add(pKid);
		return pKid;
	}
	void CRadioGroupField::SetFieldName(const std::wstring& wsFieldName, bool isSkipCheck)
	{
		m_wsFieldName = wsFieldName;
		CFieldBase::SetFieldName(wsFieldName, true);
	}
	const std::wstring& CRadioGroupField::GetFieldName() const
	{
		return m_wsFieldName;
	}
	//----------------------------------------------------------------------------------------
	// CAnnotAppearance
	//----------------------------------------------------------------------------------------
	CPictureField::CPictureField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		m_pMK = NULL;
		m_pIF = NULL;

		Add("FT", "Btn");
		SetFlag(true, 1 << 16);
		Add("H", "I");

		CDictObject* pAction = new CDictObject();		
		if (pAction)
		{
			pXref->Add(pAction);

			Add("A", pAction);

			pAction->Add("Type", "Action");
			pAction->Add("S", "JavaScript");
			pAction->Add("JS", new CStringObject("event.target.buttonImportIcon();"));
		}

		m_pMK = new CDictObject();
		if (!m_pMK)
			return;

		pXref->Add(m_pMK);
		Add("MK", m_pMK);

		m_pMK->Add("R", 0);

		CArrayObject* pBG = new CArrayObject();
		if (pBG)
		{
			pBG->Add(0.909);
			pBG->Add(0.941);
			pBG->Add(0.992);
			m_pMK->Add("BG", pBG);
		}

		m_pMK->Add("TP", 1);

		m_pIF = new CDictObject();
		if (!m_pIF)
			return;

		m_pMK->Add("IF", m_pIF);

		SetScaleType(EScaleType::Always);
		SetRespectBorders(false);
		SetConstantProportions(true);
		SetShift(0.5, 0.5);
	}
	void CPictureField::SetAppearance()
	{
		CAnnotAppearance* pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAppearance);


		CAnnotAppearanceObject* pNormal = pAppearance->GetNormal();

		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();

		std::string sDA = "0.909 0.941 0.992 rg";
		Add("DA", new CStringObject(sDA.c_str()));

		pNormal->DrawPicturePlaceholder();
	}
	void CPictureField::SetScaleType(const EScaleType& eType)
	{
		if (!m_pIF)
			return;

		switch (eType)
		{
			case EScaleType::Always: m_pIF->Add("SW", "A"); break;
			case EScaleType::Bigger: m_pIF->Add("SW", "B"); break;
			case EScaleType::Smaller: m_pIF->Add("SW", "S"); break;
			case EScaleType::Never: m_pIF->Add("SW", "N"); break;
		}
	}
	void CPictureField::SetConstantProportions(const bool& bConstant)
	{
		if (!m_pIF)
			return;

		if (bConstant)
			m_pIF->Add("S", "P");
		else
			m_pIF->Add("S", "A");
	}
	void CPictureField::SetRespectBorders(const bool& bRespectBorders)
	{
		if (!m_pIF)
			return;

		m_pIF->Add("FB", !bRespectBorders);
	}
	void CPictureField::SetShift(const double& dX, const double& dY)
	{
		if (!m_pIF)
			return;

		CArrayObject* pA = new CArrayObject();
		if (pA)
		{
			m_pIF->Add("A", pA);
			pA->Add(dX);
			pA->Add(dY);
		}
	}
	//----------------------------------------------------------------------------------------
	// CAnnotAppearance
	//----------------------------------------------------------------------------------------
	CAnnotAppearance::CAnnotAppearance(CXref* pXref, CFieldBase* pField)
	{
		m_pXref  = pXref;
		m_pField = pField;

		m_pNormal   = new CAnnotAppearanceObject(pXref, pField);
		m_pRollover = NULL;
		m_pDown     = NULL;

		Add("N", m_pNormal);
	}
	CAnnotAppearanceObject* CAnnotAppearance::GetNormal()
	{
		return m_pNormal;
	}
	CAnnotAppearanceObject* CAnnotAppearance::GetRollover()
	{
		if (!m_pRollover)
		{
			m_pRollover = new CAnnotAppearanceObject(m_pXref, m_pField);
			Add("R", m_pRollover);
		}

		return m_pRollover;
	}
	CAnnotAppearanceObject* CAnnotAppearance::GetDown()
	{
		if (!m_pDown)
		{
			m_pDown = new CAnnotAppearanceObject(m_pXref, m_pField);
			Add("D", m_pDown);
		}

		return m_pDown;
	}
	//----------------------------------------------------------------------------------------
	// CAnnotAppearance
	//----------------------------------------------------------------------------------------
	CCheckBoxAnnotAppearance::CCheckBoxAnnotAppearance(CXref* pXref, CFieldBase* pField, const char* sYesName)
	{
		m_pXref  = pXref;
		m_pField = pField;

		m_pYesN = new CAnnotAppearanceObject(pXref, pField);
		m_pOffN = new CAnnotAppearanceObject(pXref, pField);
		m_pYesD = new CAnnotAppearanceObject(pXref, pField);
		m_pOffD = new CAnnotAppearanceObject(pXref, pField);

		CDictObject* pDictN = new CDictObject();
		Add("N", pDictN);
		pDictN->Add(sYesName ? sYesName : "Yes", m_pYesN);
		pDictN->Add("Off", m_pOffN);
		
		CDictObject* pDictD = new CDictObject();
		Add("D", pDictD);
		pDictD->Add(sYesName ? sYesName : "Yes", m_pYesD);
		pDictD->Add("Off", m_pOffD);
	}
	CAnnotAppearanceObject* CCheckBoxAnnotAppearance::GetYesN()
	{
		return m_pYesN;
	}
	CAnnotAppearanceObject* CCheckBoxAnnotAppearance::GetOffN()
	{
		return m_pOffN;
	}
	CAnnotAppearanceObject* CCheckBoxAnnotAppearance::GetYesD()
	{
		return m_pYesD;
	}
	CAnnotAppearanceObject* CCheckBoxAnnotAppearance::GetOffD()
	{
		return m_pOffD;
	}
	//----------------------------------------------------------------------------------------
	// CAnnotAppearanceObject
	//----------------------------------------------------------------------------------------
	CAnnotAppearanceObject::CAnnotAppearanceObject(CXref* pXref, CFieldBase* pField)
	{
		m_pXref   = pXref;
		m_pStream = new CMemoryStream();
		m_pField  = pField;

		SetStream(m_pXref, m_pStream);

		Add("Type", "XObject");
		Add("Subtype", "Form");

		TRect oRect = pField->GetRect();

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("BBox", pArray);
		pArray->Add(0);
		pArray->Add(0);
		pArray->Add(oRect.fRight - oRect.fLeft);
		pArray->Add(oRect.fBottom - oRect.fTop);

		Add("Resources", pField->GetResourcesDict());

#ifndef FILTER_FLATE_DECODE_DISABLED
		SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
	}
	void CAnnotAppearanceObject::DrawSimpleText(const std::wstring& wsText, unsigned char* pCodes, unsigned int unCount, const char* sFontName, double dFontSize, double dX, double dY, double dR, double dG, double dB, const char* sExtGStateName, double dWidth, double dHeight, double* pShifts, unsigned int unShiftsCount)
	{
		if (!m_pStream || !sFontName)
			return;

		m_pStream->WriteEscapeName("Tx");
		m_pStream->WriteStr(" BMC\012");

		double dBorderSize   = 0;
		double dBorderSize_2 = 0;
		double dBorderSize2  = 0;
		if (m_pField && m_pField->HaveBorder())
		{
			TRgb oColor = m_pField->GetBorderColor();
			m_pStream->WriteStr("q\012");

			m_pStream->WriteReal(oColor.r);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(oColor.g);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(oColor.b);
			m_pStream->WriteStr(" RG\012");

			dBorderSize   = m_pField->GetBorderSize();
			dBorderSize_2 = dBorderSize / 2;
			dBorderSize_2 = dBorderSize * 2;
			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteStr(" w\0120 j\0120 J\012");

			m_pStream->WriteReal(dBorderSize_2);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dBorderSize_2);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(std::max(dWidth - dBorderSize, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(std::max(dHeight - dBorderSize, 0.0));
			m_pStream->WriteStr(" re\012S\012");

			CTextField* pTextField = dynamic_cast<CTextField*>(m_pField);
			if (pTextField && pTextField->IsCombFlag())
			{
				int nMaxLen = pTextField->GetMaxLen();
				if (nMaxLen > 1)
				{
					double dStep = dWidth / nMaxLen;
					double dX = dStep;
					for (int nIndex = 0; nIndex < nMaxLen - 1; ++nIndex)
					{
						m_pStream->WriteReal(dX);
						m_pStream->WriteChar(' ');
						m_pStream->WriteInt(0);
						m_pStream->WriteChar(' ');
						m_pStream->WriteStr(" m\012");
						m_pStream->WriteReal(dX);
						m_pStream->WriteChar(' ');
						m_pStream->WriteReal(dHeight);
						m_pStream->WriteChar(' ');
						m_pStream->WriteStr(" l\012S\012");

						dX += dStep;
					}
				}
			}

			m_pStream->WriteStr("Q\012");
		}

		m_pStream->WriteStr("q\012");

		m_pStream->WriteReal(dBorderSize);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dBorderSize);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(std::max(dWidth - dBorderSize2, 0.0));
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(std::max(dHeight - dBorderSize2, 0.0));
		m_pStream->WriteStr(" re\012W\012n\012");

		m_pStream->WriteStr("BT\012");

		m_pStream->WriteReal(dR);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dG);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dB);
		m_pStream->WriteStr(" rg\012");

		if (sExtGStateName)
		{
			m_pStream->WriteEscapeName(sExtGStateName);
			m_pStream->WriteStr(" gs\012");
		}

		dFontSize = std::min(1000.0, std::max(0.0, dFontSize));

		m_pStream->WriteEscapeName(sFontName);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dFontSize);
		m_pStream->WriteStr(" Tf\012");

		if (pCodes && pShifts && unShiftsCount > 0 && unShiftsCount == unCount / 2)
		{
			m_pStream->WriteReal(dX + pShifts[0]);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dY);
			m_pStream->WriteStr(" Td\012");

			for (unsigned int unIndex = 0; unIndex < unShiftsCount; ++unIndex)
			{
				m_pStream->WriteChar('[');
				m_pStream->WriteChar('<');
				m_pStream->WriteBinary(pCodes + 2 * unIndex, 2, NULL);
				m_pStream->WriteChar('>');
				m_pStream->WriteStr("]TJ\012");

				if (unIndex != unShiftsCount - 1)
				{
					m_pStream->WriteReal(pShifts[unIndex + 1]);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(0.0);
					m_pStream->WriteStr(" Td\012");
				}
			}
		}
		else
		{
			m_pStream->WriteReal(dX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dY);
			m_pStream->WriteStr(" Td\012");

			if (pCodes)
			{
				m_pStream->WriteChar('<');
				m_pStream->WriteBinary(pCodes, unCount, NULL);
				m_pStream->WriteChar('>');
			}
			else
			{
				std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsText);
				m_pStream->WriteEscapeText((BYTE*)(sText.c_str()), sText.length());
			}

			m_pStream->WriteStr(" Tj\012");
		}

		m_pStream->WriteStr("ET\012");
		
		m_pStream->WriteStr("Q\012EMC\012");
	}
	void CAnnotAppearanceObject::DrawPicturePlaceholder()
	{
		if (!m_pStream)
			return;

		double dW = 0, dH = 0;
		if (m_pField)
		{
			TRect oRect = m_pField->GetRect();
			dW = fabs(oRect.fRight - oRect.fLeft);
			dH = fabs(oRect.fBottom - oRect.fTop);
		}

		m_pStream->WriteStr("Q\0120.909 0.941 0.992 rg\0121 0 0 1 0 0 cm\012");
		m_pStream->WriteStr("0 0 ");
		m_pStream->WriteReal(std::max(dW, 0.0));
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(std::max(dH, 0.0));
		m_pStream->WriteStr(" re\012f\012q");
	}	
}
