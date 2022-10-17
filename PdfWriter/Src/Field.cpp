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
#include "Image.h"
#include "Font.h"
#include "Info.h"
#include "EncryptDictionary.h"

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

		m_pMK         = NULL;
		m_pParent     = NULL;
		m_pKids       = NULL;
		m_pAppearance = NULL;
		m_pFocus      = NULL;
		m_pBlur       = NULL;
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
	CDictObject* CFieldBase::GetAA()
	{
		CDictObject* pAA = dynamic_cast<CDictObject*>(this->Get("AA"));
		if (pAA)
			return pAA;

		pAA = new CDictObject();
		if (!pAA)
			return NULL;

		Add("AA", pAA);
		return pAA;
	}
	void CFieldBase::AddScriptToAA(const std::string& sKey, const std::string& sScript, CDictObject* pAA)
	{
		if (sScript.empty())
			return;

		CDictObject* _pAA = pAA;
		if (!_pAA)
			_pAA = GetAA();

		if (!_pAA)
			return;

		CDictObject* pKey = new CDictObject();
		if (!pKey)
			return;

		m_pXref->Add(pKey);
		pAA->Add(sKey.c_str(), pKey);
		pKey->Add("S", "JavaScript");
		pKey->Add("JS", new CStringObject(sScript.c_str(), true, true));
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

		Add("DA", new CStringObject(sDA.c_str(), false, true));
	}
	void CFieldBase::SetTextAppearance(const std::wstring& wsValue, unsigned short* pCodes, unsigned int unCount, CFontDict* pFont, const TRgb& oColor, const double& dAlpha, double dFontSize, double dX, double dY, CFontCidTrueType** ppFonts, double* pShifts)
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

		pNormal->DrawSimpleText(wsValue, pCodes, unCount, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop), ppFonts, pShifts);
	}
	void CFieldBase::StartTextAppearance(CFontDict* pFont, const double& dFontSize, const TRgb& oColor, const double& dAlpha)
	{
		m_pAppearance = new CAnnotAppearance(m_pXref, this);
		if (!m_pAppearance)
			return;

		Add("AP", m_pAppearance);

		CAnnotAppearanceObject* pNormal  = m_pAppearance->GetNormal();
		CResourcesDict* pFieldsResources = GetResourcesDict();

		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}

		pNormal->StartDrawText(pFont, dFontSize, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
	}
	void CFieldBase::AddLineToTextAppearance(const double& dX, const double& dY, unsigned short* pCodes, const unsigned int& unCodesCount, CFontCidTrueType** ppFonts, const double* pShifts)
	{
		if (!m_pAppearance)
			return;

		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->DrawTextLine(dX, dY, pCodes, unCodesCount, ppFonts, pShifts);
	}
	void CFieldBase::EndTextAppearance()
	{
		if (!m_pAppearance)
			return;

		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->EndDrawText();
	}
	void CFieldBase::SetTextValue(const std::wstring &wsValue)
	{
		std::string sValue = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsValue);
		Add("V", new CStringObject(sValue.c_str(), true));
	}
	void CFieldBase::SetFieldBorder(const EBorderSubtype& eSubtype, const TRgb& oColor, const double& dWidth, const unsigned short& nDashOn, const unsigned short& nDashOff, const unsigned short& nDashPhase)
	{
		if (dWidth < 0.01)
		{
			m_nBorderType = 0;
			Remove("BS");
			return;
		}

		CDictObject* pBorderStyleDict = new CDictObject();
		if (!pBorderStyleDict)
			return;

		Add("BS", pBorderStyleDict);

		// PDF ридеры воспринимают только целочисленные значения толщины линии
		int nWidth = (int)(dWidth + 0.5);
		if (nWidth < 1)
			nWidth = 1;

		pBorderStyleDict->Add("W", nWidth);

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
		m_dBorderSize  = nWidth;
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
		m_bShd = true;
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
	void CFieldBase::SetPlaceHolderText(const std::wstring& wsText, const TRgb& oNormalColor, const TRgb& oPlaceHolderColor)
	{
		m_wsPlaceHolderText = wsText;
		m_oNormalColor      = oNormalColor;
		m_oPlaceHolderColor = oPlaceHolderColor;

		SetPlaceHolderText(std::vector<std::wstring>{wsText}, {oNormalColor}, {oPlaceHolderColor});

		if (m_pParent)
			m_pParent->UpdateKidsPlaceHolder();
	}
	void CFieldBase::SetPlaceHolderText(const std::vector<std::wstring>& vPlaceHolders, const std::vector<TRgb>& vNormalColors, const std::vector<TRgb>& vPlaceHolderColors)
	{
		if (!vPlaceHolders.size() || !vNormalColors.size() || !vPlaceHolderColors.size())
			return;

		std::string sFocus = "\nvar	curColor = color.convert(event.target.textColor, \"RGB\");\nif ((";

		for (unsigned int unIndex = 0, unCount = vPlaceHolders.size(); unIndex < unCount; ++unIndex)
		{
			if (unIndex)
				sFocus += " || ";

			std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(vPlaceHolders[unIndex]);
			sFocus += "event.target.value==\"" + sText + "\"";
		}

		sFocus += ") && (";

		for (unsigned int unIndex = 0, unCount = vPlaceHolderColors.size(); unIndex < unCount; ++unIndex)
		{
			if (unIndex)
				sFocus += " || ";

			const TRgb& oColor = vPlaceHolderColors[unIndex];

			sFocus += "(Math.abs(curColor[1] - " + std::to_string(oColor.r) +
					") < 0.005 && Math.abs(curColor[2] - " + std::to_string(oColor.g) +
					") < 0.005 && Math.abs(curColor[3] - " + std::to_string(oColor.b) +
					") < 0.005)";
		}

		sFocus += "))\n{	event.target.value = \"\";\n	event.target.textColor =[\"RGB\", " +
				std::to_string(vNormalColors[vNormalColors.size() - 1].r) + ", " +
				std::to_string(vNormalColors[vNormalColors.size() - 1].g) + ", " +
				std::to_string(vNormalColors[vNormalColors.size() - 1].b) + "];\n}";



		std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(vPlaceHolders[vPlaceHolders.size() - 1]);
		const TRgb& oColor = vPlaceHolderColors[vPlaceHolderColors.size() - 1];
		std::string sBlur = "\nif (event.target.value == \"\")\n{\n	event.target.textColor =[\"RGB\", " +
			std::to_string(oColor.r) + ", " +
			std::to_string(oColor.g) + ", " +
			std::to_string(oColor.b) + "];\nevent.target.value = \"" + sText + "\";\n}";

		if (!m_pFocus || !m_pBlur)
		{
			CDictObject* pAA = GetAA();
			if (!pAA)
				return;

			CDictObject* pFocus = new CDictObject();
			CDictObject* pBlur = new CDictObject();
			if (!pFocus || !pBlur)
				return;

			m_pXref->Add(pFocus);
			m_pXref->Add(pBlur);

			m_pFocus = pFocus;
			m_pBlur  = pBlur;

			pAA->Add("Bl", pBlur);
			pAA->Add("Fo", pFocus);
		}

		m_pFocus->Add("S", "JavaScript");
		m_pFocus->Add("JS", new CStringObject(sFocus.c_str(), true, true));

		m_pBlur->Add("S", "JavaScript");
		m_pBlur->Add("JS", new CStringObject(sBlur.c_str(), true, true));
	}
	void CFieldBase::UpdateKidsPlaceHolder()
	{
		std::vector<std::wstring> vText;
		std::vector<TRgb> vPlaceHolderColor, vNormalColor;

		if (m_pKids)
		{
			for (unsigned int unKidIndex = 0, unKidsCount = GetKidsCount(); unKidIndex < unKidsCount; ++unKidIndex)
			{
				CFieldBase* pKid = (CFieldBase*)m_pKids->Get(unKidIndex);

				const std::wstring& wsText = pKid->GetPlaceHolderText();

				if (wsText.empty())
					continue;

				const TRgb& oPColor = pKid->GetPlaceHolderColor();
				const TRgb& oNColor = pKid->GetNormalColor();

				bool isAdd = true;
				for (unsigned int unIndex = 0, unCount = vText.size(); unIndex < unCount; ++unIndex)
				{
					if (vText[unIndex] == wsText)
					{
						isAdd = false;
						break;
					}
				}

				if (isAdd)
					vText.push_back(wsText);

				isAdd = true;
				for (unsigned int unIndex = 0, unCount = vPlaceHolderColor.size(); unIndex < unCount; ++unIndex)
				{
					if (vPlaceHolderColor[unIndex] == oPColor)
					{
						isAdd = false;
						break;
					}
				}

				if (isAdd)
					vPlaceHolderColor.push_back(oPColor);

				isAdd = true;
				for (unsigned int unIndex = 0, unCount = vNormalColor.size(); unIndex < unCount; ++unIndex)
				{
					if (vNormalColor[unIndex] == oNColor)
					{
						isAdd = false;
						break;
					}
				}

				if (isAdd)
					vNormalColor.push_back(oNColor);
			}

			for (unsigned int unKidIndex = 0, unKidsCount = GetKidsCount(); unKidIndex < unKidsCount; ++unKidIndex)
			{
				CFieldBase* pKid = (CFieldBase*)m_pKids->Get(unKidIndex);
				pKid->SetPlaceHolderText(vText, vNormalColor, vPlaceHolderColor);
			}
		}
	}
	const std::wstring& CFieldBase::GetPlaceHolderText()
	{
		return m_wsPlaceHolderText;
	}
	const TRgb& CFieldBase::GetNormalColor()
	{
		return m_oNormalColor;
	}
	const TRgb& CFieldBase::GetPlaceHolderColor()
	{
		return m_oPlaceHolderColor;
	}
	void CFieldBase::SetFormat(const CFormFieldInfo::CTextFormFormat* pFormat)
	{
		if (!pFormat || pFormat->IsEmpty())
			return;

		std::string scriptK, scriptF, scriptV;

		const TRgb& oNormalColor = GetNormalColor();

		const std::string prefix = "var curColor = color.convert(event.target.textColor, \"RGB\");\nif (Math.abs(curColor[1] - " + std::to_string(oNormalColor.r) +
			") < 0.005 && Math.abs(curColor[2] - " + std::to_string(oNormalColor.g) +
			") < 0.005 && Math.abs(curColor[3] - " + std::to_string(oNormalColor.b) +
			") < 0.005)\n{";
		const std::string postfix = "}";

		if (pFormat->GetSymbolsCount())
		{
			scriptK += "\nvar s=String.fromCharCode(";
			scriptV += "\nvar s=String.fromCharCode(";
			for (unsigned int unIndex = 0, unCount = pFormat->GetSymbolsCount(); unIndex < unCount; ++unIndex)
			{
				if (unIndex)
				{
					scriptK += ", ";
					scriptV += ", ";
				}

				scriptK += std::to_string(pFormat->GetSymbol(unIndex));
				scriptV += std::to_string(pFormat->GetSymbol(unIndex));
			}
			scriptK += ");";
			scriptV += ");";

			scriptK += "\nfor (var i = 0, l = event.change.length; i < l; ++i){\nvar c = event.change.charAt(i);\nif(-1 == s.indexOf(c)){\nevent.rc = false;\nbreak;\n}\n}";
			scriptV += "\nfor (var i = 0, l = event.value.length; i < l; ++i){\nvar c = event.value.charAt(i);\nif(-1 == s.indexOf(c)){\nevent.rc = false;\nbreak;\n}\n}";
		}

		if (pFormat->IsDigit())
		{
			scriptK += "\nif(false !== event.rc)\n{for(var i = 0, l = event.change.length; i < l; ++i){var c = event.change.charCodeAt(i);if(c < 48 || c > 57){event.rc = false;break;}}}";
			scriptV += "\nfor(var i = 0, l = event.value.length; i < l; ++i){var c = event.value.charCodeAt(i);if(c < 48 || c > 57){event.rc = false;break;}}";
		}
		else if (pFormat->IsLetter())
		{
			scriptK += "\nvar r=/^[^\\d.!?\\/\\\\+\\-\\x20'\":;\\(\\)\\[\\]\\{\\}=_@#$%^&*]+$/;\nif(false !== event.rc)\n{for(var i = 0, l = event.change.length; i < l; ++i){var c = event.change.charCodeAt(i);if(!r.test(String.fromCharCode(c))){event.rc = false;break;}}}";
			scriptV += "\nvar r=/^[^\\d.!?\\/\\\\+\\-\\x20'\":;\\(\\)\\[\\]\\{\\}=_@#$%^&*]+$/;\nfor(var i = 0, l = event.value.length; i < l; ++i){var c = event.value.charCodeAt(i);if(!r.test(String.fromCharCode(c))){event.rc = false;break;}}";
		}
		else if (pFormat->IsMask())
		{
			scriptK += "\nAFSpecial_KeystrokeEx(\"" + NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pFormat->GetMask()) + "\");";
		}
		else if (pFormat->IsRegExp())
		{
			scriptV += "\nvar r=/" + NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pFormat->GetRegExp()) + "/;";
			scriptV += "\nif(event.value)event.rc=!!event.value.match(r);";
		}

		CDictObject* pAA = GetAA();

		if (!scriptK.empty())
		{
			scriptK = prefix + scriptK + postfix;
			AddScriptToAA("K", scriptK, pAA);
		}

		if (!scriptV.empty())
		{
			scriptV = prefix + scriptV + postfix;
			AddScriptToAA("V", scriptV, pAA);
		}

		if (!scriptF.empty())
		{
			scriptV = prefix + scriptF + postfix;
			AddScriptToAA("F", scriptF, pAA);
		}
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
		{
			if (m_pParent)
				m_pParent->Add("MaxLen", nMaxLen);
			else
				Add("MaxLen", nMaxLen);
		}
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
	void CChoiceField::AddOption(const std::wstring& wsOption, const bool& bPushBack)
	{
		if (!m_pOpt)
		{
			m_pOpt = new CArrayObject();
			Add("Opt", m_pOpt);
		}

		if (!m_pOpt)
			return;

		std::string sOption = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsOption);
		m_pOpt->Add(new CStringObject(sOption.c_str(), true), bPushBack);
	}
	void CChoiceField::SetSelectedIndex(const unsigned int& unSelectedIndex)
	{
		if (m_pParent)
		{
			CChoiceField* pParent = dynamic_cast<CChoiceField*>(m_pParent);
			if (pParent)
			{
				pParent->SetSelectedIndex(unSelectedIndex);
				return;
			}
		}

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		pArray->Add(unSelectedIndex);
		Add("I", pArray);

		if (m_pKids)
		{
			for (unsigned int unIndex = 0, unCount = m_pKids->GetCount(); unIndex < unCount; ++unIndex)
			{
				CFieldBase* pKid = dynamic_cast<CFieldBase*>(m_pKids->Get(unIndex));
				if (pKid)
					pKid->Remove("I");
			}
		}
	}
	void CChoiceField::UpdateSelectedIndexToParent()
	{
		if (m_pParent)
		{
			CObjectBase* pSelectedArray = Get("I");
			if (pSelectedArray)
				m_pParent->Add("I", pSelectedArray->Copy());

			Remove("I");

			CObjectBase* pValue = Get("V");
			if (pValue)
				m_pParent->Add("V", pValue->Copy());

			Remove("V");

			CObjectBase* pOpt = Get("Opt");
			if (pOpt)
				m_pParent->Add("Opt", pOpt->Copy());

			Remove("Opt");
		}
	}
	void CChoiceField::SetPlaceHolderText(const std::wstring& wsText, const TRgb& oNormalColor, const TRgb& oPlaceHolderColor)
	{
		CDictObject* pAA = GetAA();
		if (!pAA)
			return;

		CDictObject* pFocus = new CDictObject();
		CDictObject* pBlur = new CDictObject();
		if (!pFocus || !pBlur)
			return;

		bool bCanEdit = GetFieldFlag() & (1 << 18);

		
		std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsText);
		std::string sFocus, sBlur;

		if (!bCanEdit)
		{
			CDictObject* pChange = new CDictObject();
			if (!pChange)
			{
				delete pFocus;
				delete pBlur;
				return;
			}

			// Если текст плейсхолдера не добавить как опцию, тогда AdobeAcrobat не дает выставлять такое текстовое значение
			AddOption(wsText, false);

			sFocus = "event.target.textColor = [\"RGB\", " +
				std::to_string(oNormalColor.r) + ", " +
				std::to_string(oNormalColor.g) + ", " +
				std::to_string(oNormalColor.b) + "];";

			sBlur = "\nif (event.target.value == \"" + sText + "\")\n	event.target.textColor = [\"RGB\", " +
				std::to_string(oPlaceHolderColor.r) + ", " +
				std::to_string(oPlaceHolderColor.g) + ", " +
				std::to_string(oPlaceHolderColor.b) + "];\nelse\n	event.target.textColor = [\"RGB\", " +
				std::to_string(oNormalColor.r) + ", " +
				std::to_string(oNormalColor.g) + ", " +
				std::to_string(oNormalColor.b) + "];";

			std::string sChange = "\n	if (event.value == \"" + sText + "\")\n		event.target.textColor =[\"RGB\", " +
				std::to_string(oPlaceHolderColor.r) + ", " +
				std::to_string(oPlaceHolderColor.g) + ", " +
				std::to_string(oPlaceHolderColor.b) + "];\n	else\n		event.target.textColor = [\"RGB\", " +
				std::to_string(oNormalColor.r) + ", " +
				std::to_string(oNormalColor.g) + ", " +
				std::to_string(oNormalColor.b) + "];\n}";

			m_pXref->Add(pChange);
			pChange->Add("S", "JavaScript");
			pChange->Add("JS", new CStringObject(sChange.c_str(), true, true));
			pAA->Add("K", pChange);
		}
		else
		{
			sFocus = "var curColor = color.convert(event.target.textColor, \"RGB\");\nevent.target.textColor = [\"RGB\", " +
				std::to_string(oNormalColor.r) + ", " +
				std::to_string(oNormalColor.g) + ", " +
				std::to_string(oNormalColor.b) + "];\nif (event.target.value == \"" + \
				sText + "\" && Math.abs(curColor[1] - " + std::to_string(oPlaceHolderColor.r) +
				") < 0.005 && Math.abs(curColor[2] - " + std::to_string(oPlaceHolderColor.g) +
				") < 0.005 && Math.abs(curColor[3] - " + std::to_string(oPlaceHolderColor.b) +
				") < 0.005)\n	event.target.value = \"\";";

			sBlur = "\nif (event.target.value == \"\")\n{\n	event.target.textColor = [\"RGB\", " +
				std::to_string(oPlaceHolderColor.r) + ", " +
				std::to_string(oPlaceHolderColor.g) + ", " +
				std::to_string(oPlaceHolderColor.b) + "];\nevent.target.value = \"" + sText + "\";\n}";
		}

		m_pXref->Add(pFocus);
		pFocus->Add("S", "JavaScript");
		pFocus->Add("JS", new CStringObject(sFocus.c_str(), true, true));
		pAA->Add("Fo", pFocus);

		m_pXref->Add(pBlur);
		pBlur->Add("S", "JavaScript");
		pBlur->Add("JS", new CStringObject(sBlur.c_str(), true, true));
		pAA->Add("Bl", pBlur);
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
	void CCheckBoxField::SetAppearance(const std::wstring& wsYesValue, unsigned short* pYesCodes, unsigned int unYesCount, CFontDict* pYesFont,
									   const std::wstring& wsOffValue, unsigned short* pOffCodes, unsigned int unOffCount, CFontDict* pOffFont,
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

		if (!m_nBorderType)
		{
			CArrayObject* pArray = new CArrayObject();
			pArray->Add(0);
			m_pMK->Add("BC", pArray);
		}

		if (!m_bShd)
		{
			CArrayObject* pArray = new CArrayObject();
			pArray->Add(1);
			m_pMK->Add("BG", pArray);
		}
		
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


		pAppearance->GetYesN()->DrawSimpleText(wsYesValue, pYesCodes, unYesCount, pYesFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
		pAppearance->GetOffN()->DrawSimpleText(wsOffValue, pOffCodes, unOffCount, pOffFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));

		pAppearance->GetYesD()->DrawSimpleText(wsYesValue, pYesCodes, unYesCount, pYesFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
		pAppearance->GetOffD()->DrawSimpleText(wsOffValue, pOffCodes, unOffCount, pOffFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
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

		SetFieldBorder(border_subtype_Solid, TRgb(0.0, 0.0, 0.0), 1, 0, 0, 0);
		Remove("BS");

		if (1 == nType)
		{
			m_pMK->Add("CA", new CStringObject("4"));

			pAppearance->GetYesN()->DrawSimpleText(L"4", NULL, 0, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
			pAppearance->GetOffN()->DrawSimpleText(L"", NULL, 0, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
		}
		else if (2 == nType)
		{
			m_pMK->Add("CA", new CStringObject("1"));

			pAppearance->GetYesN()->DrawSimpleText(L"1", NULL, 0, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
			pAppearance->GetOffN()->DrawSimpleText(L"", NULL, 0, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
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
	// CPictureField
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
		m_pMK->Add("TP", 1);

		m_pIF = new CDictObject();
		if (!m_pIF)
			return;

		m_pMK->Add("IF", m_pIF);

		SetScaleType(EScaleType::Always);
		SetRespectBorders(false);
		SetConstantProportions(true);
		SetShift(0.5, 0.5);

		m_pResources = NULL;
	}
	void CPictureField::SetFieldName(const std::string& sName, bool isSkipCheck)
	{
		std::string _sName = sName + "_af_image";
		CFieldBase::SetFieldName(_sName, isSkipCheck);
	}
	void CPictureField::SetFieldName(const std::wstring& wsName, bool isSkipCheck)
	{
		std::string sName = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsName) + "_af_image";
		CFieldBase::SetFieldName(sName, isSkipCheck);
	}
	void CPictureField::SetAppearance(CImageDict* pImage)
	{
		CAnnotAppearance* pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAppearance);

		CAnnotAppearanceObject* pNormal = pAppearance->GetNormal();
		CResourcesDict* pFieldsResources = GetResourcesDict();

		std::string sDA = "0.909 0.941 0.992 rg";
		Add("DA", new CStringObject(sDA.c_str()));

		if (pImage)
		{			
			TRect oRect = GetRect();

			double dH = fabs(oRect.fTop - oRect.fBottom);
			double dW = fabs(oRect.fRight - oRect.fLeft);

			double dOriginW = pImage->GetWidth() * 72 / 96.0;
			double dOriginH = pImage->GetHeight() * 72 / 96.0;

			bool bNeedScale = (EScaleType::Always == m_eScaleType
				|| (EScaleType::Bigger == m_eScaleType && (dOriginH > dH || dOriginW > dW))
				|| (EScaleType::Smaller == m_eScaleType && dOriginH < dH && dOriginW < dW));

			double dDstW = dOriginW;
			double dDstH = dOriginH;
			double dDstX = 0;
			double dDstY = 0;

			if (m_bRespectBorders && HaveBorder())
			{
				double dBorderSize = GetBorderSize();
				dDstX += 2 * dBorderSize;
				dDstY += 2 * dBorderSize;
				dH -= 4 * dBorderSize;
				dW -= 4 * dBorderSize;
			}

			if (bNeedScale)
			{
				if (!m_bConstantProportions)
				{
					dDstH = dH;
					dDstW = dW;
				}
				else
				{
					double dScaleKoef = fmin(dW / dOriginW, dH / dOriginH);
					dDstW = dScaleKoef * dOriginW;
					dDstH = dScaleKoef * dOriginH;
				}
			}

			dDstX += (dW - dDstW) * m_dShiftX;
			dDstY += (dH - dDstH) * m_dShiftY;

			CXObject* pForm = new CXObject();
			CStream* pStream = new CMemoryStream();
			pForm->SetStream(m_pXref, pStream);
			
#ifndef FILTER_FLATE_DECODE_DISABLED
			if (m_pDocument->GetCompressionMode() & COMP_TEXT)
				pForm->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
			CArrayObject* pBBox = new CArrayObject();
			pForm->Add("BBox", pBBox);
			pBBox->Add(0);
			pBBox->Add(0);
			pBBox->Add(dOriginW);
			pBBox->Add(dOriginH);
			pForm->Add("FormType", 1);
			CArrayObject* pFormMatrix = new CArrayObject();
			pForm->Add("Matrix", pFormMatrix);
			pFormMatrix->Add(1);
			pFormMatrix->Add(0);
			pFormMatrix->Add(0);
			pFormMatrix->Add(1);
			pFormMatrix->Add(0);
			pFormMatrix->Add(0);
			pForm->Add("Name", "FRM");

			CDictObject* pFormRes = new CDictObject();
			CArrayObject* pFormResProcset = new CArrayObject();
			pFormRes->Add("ProcSet", pFormResProcset);
			pFormResProcset->Add(new CNameObject("PDF"));
			pFormResProcset->Add(new CNameObject("ImageC"));
			CDictObject* pFormResXObject = new CDictObject();
			pFormRes->Add("XObject", pFormResXObject);
			pFormResXObject->Add("Img", pImage);
			pForm->Add("Resources", pFormRes);

			pForm->Add("Subtype", "Form");
			pForm->Add("Type", "XObject");			

			pStream->WriteStr("q\012");
			pStream->WriteReal(dOriginW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dOriginH);
			pStream->WriteStr(" 0 0 cm\012/Img Do\012Q");

			pFieldsResources->AddXObjectWithName("FRM", pForm);
			pNormal->DrawPicture("FRM", dDstX, dDstY, dDstW / dOriginW, dDstH / dOriginH, m_bRespectBorders);
		}
		else
		{
			pNormal->DrawPicture();
		}		
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

		m_eScaleType = eType;
	}
	void CPictureField::SetConstantProportions(const bool& bConstant)
	{
		if (!m_pIF)
			return;

		if (bConstant)
			m_pIF->Add("S", "P");
		else
			m_pIF->Add("S", "A");

		m_bConstantProportions = bConstant;
	}
	void CPictureField::SetRespectBorders(const bool& bRespectBorders)
	{
		if (!m_pIF)
			return;

		m_pIF->Add("FB", !bRespectBorders);

		m_bRespectBorders = bRespectBorders;
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

		m_dShiftX = dX;
		m_dShiftY = dY;
	}
	CResourcesDict* CPictureField::GetResourcesDict()
	{
		if (!m_pResources)
			m_pResources = new CResourcesDict(m_pXref, false, true);

		return m_pResources;
	}
	//----------------------------------------------------------------------------------------
	// CSignatureField
	//----------------------------------------------------------------------------------------
	CSignatureField::CSignatureField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		// Словарь сигнатур
		m_pSig = new CSignatureDict(pXref);
		if (!m_pSig)
			return;
		Add("V", m_pSig);
		Add("FT", "Sig");
		m_pResources = NULL;
	}
	void CSignatureField::SetName(const std::wstring& wsValue)
	{
		if (!m_pSig || wsValue.empty())
			return;
		m_pSig->SetName(U_TO_UTF8(wsValue));
	}
	void CSignatureField::SetReason(const std::wstring &wsValue)
	{
		if (!m_pSig || wsValue.empty())
			return;
		m_pSig->SetReason(U_TO_UTF8(wsValue));
	}
	void CSignatureField::SetContact(const std::wstring &wsValue)
	{
		if (!m_pSig || wsValue.empty())
			return;
		m_pSig->SetContact(U_TO_UTF8(wsValue));
	}
	void CSignatureField::SetCert()
	{

	}
	void CSignatureField::SetDate(bool bDate)
	{
		if (!m_pSig || !bDate)
			return;
		m_pSig->SetDate();
	}
	void CSignatureField::SetAppearance(CImageDict* pImage)
	{
		CAnnotAppearance* pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAppearance);

		CAnnotAppearanceObject* pNormal = pAppearance->GetNormal();
		CResourcesDict* pFieldsResources = GetResourcesDict();

		std::string sDA = "0.909 0.941 0.992 rg";
		Add("DA", new CStringObject(sDA.c_str()));

		if (pImage)
		{
			TRect oRect = GetRect();

			double dH = fabs(oRect.fTop - oRect.fBottom);
			double dW = fabs(oRect.fRight - oRect.fLeft);

			double dOriginW = pImage->GetWidth() * 72 / 96.0;
			double dOriginH = pImage->GetHeight() * 72 / 96.0;

			double dDstW = dOriginW;
			double dDstH = dOriginH;
			double dDstX = 0;
			double dDstY = 0;

			if (HaveBorder())
			{
				double dBorderSize = GetBorderSize();
				dDstX += 2 * dBorderSize;
				dDstY += 2 * dBorderSize;
				dH -= 4 * dBorderSize;
				dW -= 4 * dBorderSize;
			}

			double dScaleKoef = fmin(dW / dOriginW, dH / dOriginH);
			dDstW = dScaleKoef * dOriginW;
			dDstH = dScaleKoef * dOriginH;

			dDstX += (dW - dDstW) * 0.5;
			dDstY += (dH - dDstH) * 0.5;

			CXObject* pForm = new CXObject();
			CStream* pStream = new CMemoryStream();
			pForm->SetStream(m_pXref, pStream);

#ifndef FILTER_FLATE_DECODE_DISABLED
			if (m_pDocument->GetCompressionMode() & COMP_TEXT)
				pForm->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
			CArrayObject* pBBox = new CArrayObject();
			pForm->Add("BBox", pBBox);
			pBBox->Add(0);
			pBBox->Add(0);
			pBBox->Add(dOriginW);
			pBBox->Add(dOriginH);
			pForm->Add("FormType", 1);
			CArrayObject* pFormMatrix = new CArrayObject();
			pForm->Add("Matrix", pFormMatrix);
			pFormMatrix->Add(1);
			pFormMatrix->Add(0);
			pFormMatrix->Add(0);
			pFormMatrix->Add(1);
			pFormMatrix->Add(0);
			pFormMatrix->Add(0);
			pForm->Add("Name", "FRM");

			CDictObject* pFormRes = new CDictObject();
			CArrayObject* pFormResProcset = new CArrayObject();
			pFormRes->Add("ProcSet", pFormResProcset);
			pFormResProcset->Add(new CNameObject("PDF"));
			pFormResProcset->Add(new CNameObject("ImageC"));
			CDictObject* pFormResXObject = new CDictObject();
			pFormRes->Add("XObject", pFormResXObject);
			pFormResXObject->Add("Img", pImage);
			pForm->Add("Resources", pFormRes);

			pForm->Add("Subtype", "Form");
			pForm->Add("Type", "XObject");

			pStream->WriteStr("q\012");
			pStream->WriteReal(dOriginW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dOriginH);
			pStream->WriteStr(" 0 0 cm\012/Img Do\012Q");

			pFieldsResources->AddXObjectWithName("FRM", pForm);
			pNormal->DrawPicture("FRM", dDstX, dDstY, dDstW / dOriginW, dDstH / dOriginH, true);
		}
		else
		{
			pNormal->DrawPicture();
		}
	}
	CResourcesDict* CSignatureField::GetResourcesDict()
	{
		if (!m_pResources)
			m_pResources = new CResourcesDict(m_pXref, false, true);

		return m_pResources;
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
	// CCheckBoxAnnotAppearance
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
		m_pXref     = pXref;
		m_pStream   = new CMemoryStream();
		m_pField    = pField;
		m_pFont     = NULL;
		m_dFontSize = 10.0;

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
		//SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
	}
	void CAnnotAppearanceObject::DrawSimpleText(const std::wstring& wsText, unsigned short* pCodes, unsigned int unCount, CFontDict* pFont, double dFontSize, double dX, double dY, double dR, double dG, double dB, const char* sExtGStateName, double dWidth, double dHeight, CFontCidTrueType** ppFonts, double* pShifts)
	{
		if (!m_pStream || !pFont)
			return;

		StartDrawText(pFont, dFontSize, dR, dG, dB, sExtGStateName, dWidth, dHeight);

		if (pCodes)
			DrawTextLine(dX, dY, pCodes, unCount, ppFonts, pShifts);
		else
			DrawTextLine(dX, dY, wsText);

		EndDrawText();		
	}
	void CAnnotAppearanceObject::DrawPicture(const char* sImageName, const double& dX, const double& dY, const double& dImageW, const double& dImageH, const bool& bRespectBorder)
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

		m_pStream->WriteStr("q\012");

		if (m_pField->HaveShd())
		{
			TRgb oColor = m_pField->GetShdColor();
			m_pStream->WriteReal(oColor.r);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(oColor.g);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(oColor.b);
			m_pStream->WriteStr(" rg\012");

			m_pStream->WriteStr("1 0 0 1 0 0 cm\012");
			m_pStream->WriteStr("0 0 ");
			m_pStream->WriteReal(fmax(dW, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dH, 0.0));
			m_pStream->WriteStr(" re\012f\012");
		}
		else if (!sImageName)
		{
			m_pStream->WriteStr("0.909 0.941 0.992 rg\0121 0 0 1 0 0 cm\012");
			m_pStream->WriteStr("0 0 ");
			m_pStream->WriteReal(fmax(dW, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dH, 0.0));
			m_pStream->WriteStr(" re\012f\012");
		}

		if (m_pField->HaveBorder())
		{
			TRgb oColor = m_pField->GetBorderColor();

			m_pStream->WriteReal(oColor.r);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(oColor.g);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(oColor.b);
			m_pStream->WriteStr(" RG\012");

			double dBorderSize   = m_pField->GetBorderSize();
			double dBorderSize_2 = dBorderSize / 2;
			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteStr(" w\0120 j\0120 J\012");

			m_pStream->WriteReal(dBorderSize_2);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dBorderSize_2);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dW - dBorderSize, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dH - dBorderSize, 0.0));
			m_pStream->WriteStr(" re\012S\012");

			if (bRespectBorder && sImageName)
			{
				m_pStream->WriteReal(2 * dBorderSize);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(2 * dBorderSize);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(fmax(dW - 4 * dBorderSize, 0.0));
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(fmax(dH - 4 * dBorderSize, 0.0));
				m_pStream->WriteStr(" re\012W\012n\012");
			}

		}
		else if (!sImageName)
		{
			m_pStream->WriteStr("0.909 0.941 0.992 RG\012");
			m_pStream->WriteStr("0.5 0.5 ");
			m_pStream->WriteReal(fmax(dW - 1, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dH - 1, 0.0));
			m_pStream->WriteStr(" re\012s\012");
		}

		if (sImageName)
		{

			m_pStream->WriteReal(dImageW);
			m_pStream->WriteStr(" 0 0 ");
			m_pStream->WriteReal(dImageH);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dY);
			m_pStream->WriteStr(" cm\012");

			m_pStream->WriteEscapeName(sImageName);
			m_pStream->WriteStr(" Do\012");
		}

		m_pStream->WriteStr("Q");
	}	
	void CAnnotAppearanceObject::StartDrawText(CFontDict* pFont, const double& dFontSize, const double& dR, const double& dG, const double& dB, const char* sExtGStateName, const double& dWidth, const double& dHeight)
	{
		CResourcesDict* pResources = m_pField->GetResourcesDict();
		if (!m_pStream || !pFont || !pResources)
			return;

		m_pStream->WriteEscapeName("Tx");
		m_pStream->WriteStr(" BMC\012");

		if (m_pField->HaveShd())
		{
			m_pStream->WriteStr("q\012");
			TRgb oColor = m_pField->GetShdColor();
			m_pStream->WriteReal(oColor.r);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(oColor.g);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(oColor.b);
			m_pStream->WriteStr(" rg\012");

			m_pStream->WriteStr("1 0 0 1 0 0 cm\012");
			m_pStream->WriteStr("0 0 ");
			m_pStream->WriteReal(fmax(dWidth, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dHeight, 0.0));
			m_pStream->WriteStr(" re\012f\012");
			m_pStream->WriteStr("Q\012");
		}

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
			dBorderSize2  = dBorderSize * 2;
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

		double _dFontSize = std::min(1000.0, std::max(0.0, dFontSize));

		m_pStream->WriteEscapeName(pResources->GetFontName(pFont));
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(_dFontSize);
		m_pStream->WriteStr(" Tf\012");

		m_bStart    = true;
		m_pFont     = pFont;
		m_dFontSize = _dFontSize;
	}
	void CAnnotAppearanceObject::DrawTextLine(const double& dX, const double& dY, const unsigned short* pCodes, const unsigned int& unCount, CFontCidTrueType** ppFonts, const double* pShifts)
	{
		CResourcesDict* pResources = m_pField->GetResourcesDict();
		if (!pResources)
			return;

		if (pCodes && pShifts)
		{
			double _dX = dX + pShifts[0], _dY = dY;

			if (!m_bStart)
			{
				_dX -= m_dCurX;
				_dY -= m_dCurY;

				m_dCurX += _dX;
				m_dCurY += _dY;
			}
			else
			{
				m_dCurX = _dX;
				m_dCurY = _dY;
			}

			m_pStream->WriteReal(_dX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(_dY);
			m_pStream->WriteStr(" Td\012");

			for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
			{
				if (ppFonts)
				{
					CFontDict* pFont = (CFontDict*)(ppFonts[unIndex]);
					if (m_pFont != pFont)
					{
						m_pStream->WriteEscapeName(pResources->GetFontName((CFontDict*)(ppFonts[unIndex])));
						m_pStream->WriteChar(' ');
						m_pStream->WriteReal(m_dFontSize);
						m_pStream->WriteStr(" Tf\012");
						m_pFont = pFont;
					}
				}

				unsigned char unByte[2] = {static_cast<unsigned char>((pCodes[unIndex] >> 8) & 0xFF), static_cast<unsigned char>(pCodes[unIndex] & 0xFF)};
				m_pStream->WriteChar('[');
				m_pStream->WriteChar('<');
				m_pStream->WriteBinary(unByte, 2, NULL);
				m_pStream->WriteChar('>');
				m_pStream->WriteStr("]TJ\012");

				if (unIndex != unCount - 1)
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
			double _dX = dX, _dY = dY;

			if (!m_bStart)
			{
				_dX -= m_dCurX;
				_dY -= m_dCurY;

				m_dCurX += _dX;
				m_dCurY += _dY;
			}
			else
			{
				m_dCurX = _dX;
				m_dCurY = _dY;
			}

			m_pStream->WriteReal(_dX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(_dY);
			m_pStream->WriteStr(" Td\012");

			for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
			{
				if (ppFonts)
				{
					CFontDict* pFont = (CFontDict*)(ppFonts[unIndex]);
					if (m_pFont != pFont)
					{
						m_pStream->WriteEscapeName(pResources->GetFontName((CFontDict*)(ppFonts[unIndex])));
						m_pStream->WriteChar(' ');
						m_pStream->WriteReal(m_dFontSize);
						m_pStream->WriteStr(" Tf\012");
						m_pFont = pFont;
					}
				}

				unsigned char unByte[2] = {static_cast<unsigned char>((pCodes[unIndex] >> 8) & 0xFF), static_cast<unsigned char>(pCodes[unIndex] & 0xFF)};
				m_pStream->WriteChar('<');
				m_pStream->WriteBinary(unByte, 2, NULL);
				m_pStream->WriteChar('>');
				m_pStream->WriteStr(" Tj\012");
			}
		}

		m_bStart = false;
	}
	void CAnnotAppearanceObject::DrawTextLine(const double &dX, const double &dY, const std::wstring& wsText)
	{
		m_pStream->WriteReal(dX);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY);
		m_pStream->WriteStr(" Td\012");

		std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsText);
		m_pStream->WriteEscapeText((BYTE*)(sText.c_str()), sText.length());

		m_pStream->WriteStr(" Tj\012");
	}
	void CAnnotAppearanceObject::EndDrawText()
	{
		m_pStream->WriteStr("ET\012");
		m_pStream->WriteStr("Q\012EMC\012");
	}
}
