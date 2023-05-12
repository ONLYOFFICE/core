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

#include "CellFormatController.h"
#include "DateReader.h"

#include "../../../../XlsxFormat/Styles/NumFmts.h"
#include "../../../../XlsxFormat/Styles/Xfs.h"

#include <regex>
#include <chrono>
#include <iomanip>

const std::wstring DefaultDateFormat = L"dd/mm/yyyy";

CellFormatController::CellFormatController(OOX::Spreadsheet::CStyles *styles):
	m_pStyles{styles}
{
	// Добавим стили для wrap-а
	m_pStyles->m_oCellXfs.Init();
	m_pStyles->m_oCellXfs->m_oCount.Init();
	m_pStyles->m_oCellXfs->m_oCount->SetValue(2);

	// Normall default
	OOX::Spreadsheet::CXfs* pXfs = NULL;
	pXfs = new OOX::Spreadsheet::CXfs();
	pXfs->m_oBorderId.Init();		pXfs->m_oBorderId->SetValue(0);
	pXfs->m_oFillId.Init();			pXfs->m_oFillId->SetValue(0);
	pXfs->m_oFontId.Init();			pXfs->m_oFontId->SetValue(0);
	pXfs->m_oNumFmtId.Init();		pXfs->m_oNumFmtId->SetValue(0);

	m_pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);

	// Wrap style
	pXfs = new OOX::Spreadsheet::CXfs();
	pXfs->m_oBorderId.Init();		pXfs->m_oBorderId->SetValue(0);
	pXfs->m_oFillId.Init();			pXfs->m_oFillId->SetValue(0);
	pXfs->m_oFontId.Init();			pXfs->m_oFontId->SetValue(0);
	pXfs->m_oNumFmtId.Init();		pXfs->m_oNumFmtId->SetValue(0);

	pXfs->m_oApplyAlignment.Init();	pXfs->m_oApplyAlignment->SetValue(SimpleTypes::onoffTrue);
	pXfs->m_oAligment.Init();		pXfs->m_oAligment->m_oWrapText.Init();
	pXfs->m_oAligment->m_oWrapText->SetValue(SimpleTypes::onoffTrue);


	m_pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);

    m_pStyles->m_oNumFmts.Init();
	m_pStyles->m_oNumFmts->m_arrItems.push_back(new OOX::Spreadsheet::CNumFmt());
	m_pStyles->m_oNumFmts->m_arrItems.back()->m_oFormatCode = DefaultDateFormat;
	m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId.Init();
	m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId->SetValue(164 + m_pStyles->m_oNumFmts->m_arrItems.size());

	// Normal + data format
	pXfs = new OOX::Spreadsheet::CXfs();

	pXfs->m_oBorderId.Init();	pXfs->m_oBorderId->SetValue(0);
	pXfs->m_oFillId.Init();		pXfs->m_oFillId->SetValue(0);
	pXfs->m_oFontId.Init();		pXfs->m_oFontId->SetValue(0);
	pXfs->m_oNumFmtId.Init();	pXfs->m_oNumFmtId->SetValue(m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId->GetValue());

	m_pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);
	auto styleId = (unsigned int)(m_pStyles->m_oCellXfs->m_arrItems.size() - 1);
	mapDataNumber.insert(std::make_pair(DefaultDateFormat, styleId));

}

void CellFormatController::ProcessCellType(OOX::Spreadsheet::CCell *pCell, const std::wstring &value, bool bIsWrap)
{

	pCell_ = pCell;
	/// формат для булева значения в верхнем регистре
	if(value == L"true" || value == L"false")
	{
		pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeInlineStr);
		pCell->m_oRichText.Init();
		OOX::Spreadsheet::CText *pText = new OOX::Spreadsheet::CText();
		auto tempValue = value;
		std::transform(tempValue.begin(), tempValue.end(), tempValue.begin(),
                 [](unsigned char c) { return std::toupper(c); });
		pText->m_sText = tempValue;
		pCell->m_oRichText->m_arrItems.push_back(pText);
		return;
	}

    size_t length = value.length();
    wchar_t *pEndPtr;
	double dValue = wcstod(value.c_str(), &pEndPtr);
	value_ = &value;

	if (std::isnan(dValue) || std::isinf(dValue))
		pEndPtr = (wchar_t *)value.c_str();

	if ((0 == *pEndPtr) || (pEndPtr != value.c_str() && (value.c_str() + length  - pEndPtr) < 3))
	{
		processNumberValue(dValue, pEndPtr);
		if (bIsWrap)
		{
			pCell->m_oStyle = 1;
		}
		return;
    }

	DateReader dateReader = {};
	auto digitalDate = dateReader.GetDigitalDate(value);
	if(digitalDate != 0)
	{
		pCell->m_oValue.Init();
		pCell->m_oValue->m_sText = std::to_wstring(digitalDate);
		std::map<std::wstring, unsigned int>::iterator pFind = mapDataNumber.find(DefaultDateFormat);
		pCell_->m_oStyle = pFind->second;
	}
	else
	{
		if (value[0] == L'='/* && bCalcFormulas*/)
		{
			pCell->m_oFormula.Init();
			pCell->m_oFormula->m_sText = value;
		}
		else
		{
			pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeInlineStr);
			pCell->m_oRichText.Init();
			OOX::Spreadsheet::CText *pText = new OOX::Spreadsheet::CText();
			pText->m_sText = value;
			pCell->m_oRichText->m_arrItems.push_back(pText);
		}
	}

	if (bIsWrap)
	{
		pCell->m_oStyle = 1;
	}

}

void CellFormatController::processNumberValue(double dValue, wchar_t *pEndPtr)
{
	std::wstring data_format;
	std::wstring postfix;
	auto length = value_->length();

		if (0 != *pEndPtr)
		{
			size_t sz = length - (pEndPtr - value_->c_str());

			while (sz > 0)
			{
				if (pEndPtr[sz - 1] != L' ')
					break;
				sz--;
			}

			if (sz > 0)
			{
				postfix = std::wstring(pEndPtr, sz);
			}
		}

		size_t pos = value_->find(L".");
		if (pos != std::wstring::npos)
		{
			size_t fraction = length - pos - ((0 != *pEndPtr) ? 2 : 1);
			for (size_t i = 0; i < fraction && fraction != std::wstring::npos; ++i)
				data_format += L"0";
		}
		if (false == data_format.empty()) data_format = L"." + data_format;

		pCell_->m_oValue.Init();

		if (0 != *pEndPtr)
		{
			if (false == postfix.empty())
			{
				if (postfix[0] == L'%')
				{
					pCell_->m_oValue->m_sText = std::to_wstring(dValue / 100.);
				}
				else
				{
					pCell_->m_oValue->m_sText = value_->substr(0, length - 1);

					for (size_t i = 0; i < postfix.size(); ++i)
					{
						data_format += std::wstring(L"\\") + postfix[i];
					}
				}
			}
		}
		else
        {
			pCell_->m_oValue->m_sText = *value_;
        }
		if (false == data_format.empty())
		{
			data_format = L"0" + data_format;

			std::map<std::wstring, unsigned int>::iterator pFind = mapDataNumber.find(data_format);
			if (pFind != mapDataNumber.end())
			{
				pCell_->m_oStyle = pFind->second;
			}
			else
			{
				if (!m_pStyles->m_oNumFmts.IsInit()) m_pStyles->m_oNumFmts.Init();
				m_pStyles->m_oNumFmts->m_arrItems.push_back(new OOX::Spreadsheet::CNumFmt());
				m_pStyles->m_oNumFmts->m_arrItems.back()->m_oFormatCode = data_format;
				m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId.Init();
				m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId->SetValue(164 + m_pStyles->m_oNumFmts->m_arrItems.size());

				// Normal + data format
				OOX::Spreadsheet::CXfs* pXfs = new OOX::Spreadsheet::CXfs();

				pXfs->m_oBorderId.Init();	pXfs->m_oBorderId->SetValue(0);
				pXfs->m_oFillId.Init();		pXfs->m_oFillId->SetValue(0);
				pXfs->m_oFontId.Init();		pXfs->m_oFontId->SetValue(0);
				pXfs->m_oNumFmtId.Init();	pXfs->m_oNumFmtId->SetValue(m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId->GetValue());

				m_pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);

				pCell_->m_oStyle = (unsigned int)(m_pStyles->m_oCellXfs->m_arrItems.size() - 1);
				mapDataNumber.insert(std::make_pair(data_format, *pCell_->m_oStyle));
			}
		}
}

SimpleTypes::Spreadsheet::ECellTypeType CellFormatController::getCellType(const std::wstring &Cellvalue)
{
	if(Cellvalue == L"true" || Cellvalue == L"false")
	{
		return SimpleTypes::Spreadsheet::ECellTypeType::celltypeBool;
	}
	wchar_t *pEndPtr;
	double dValue = wcstod(Cellvalue.c_str(), &pEndPtr);

	if (std::isnan(dValue) || std::isinf(dValue))
		pEndPtr = (wchar_t *)Cellvalue.c_str();

	if ((0 == *pEndPtr) || (pEndPtr != Cellvalue.c_str() && (Cellvalue.c_str() + Cellvalue.length()  - pEndPtr) < 3))
	{
		return SimpleTypes::Spreadsheet::ECellTypeType::celltypeNumber;
	}

	if(isValidDate(Cellvalue))
	{
		return SimpleTypes::Spreadsheet::ECellTypeType::celltypeDate;
	}

	return SimpleTypes::Spreadsheet::ECellTypeType::celltypeInlineStr;
}

bool CellFormatController::isValidDate(const std::wstring &Cellvalue)
{
	std::wregex format1(L"(0[1-9]|[1-2][0-9]|3[0-1])\\.(0[1-9]|1[0-2])\\.[0-9]{4}");
	std::wregex format2(L"[0-9]{4}\\.(0[1-9]|1[0-2])\\.(0[1-9]|[1-2][0-9]|3[0-1])");
	std::wregex format3(L"(0[1-9]|1[0-2])\\.(0[1-9]|[1-2][0-9]|3[0-1])\\.[0-9]{4}");

	std::wregex format4(L"(0[1-9]|[1-2][0-9]|3[0-1])\\-(0[1-9]|1[0-2])\\-[0-9]{4}");
	std::wregex format5(L"[0-9]{4}\\-(0[1-9]|1[0-2])\\-(0[1-9]|[1-2][0-9]|3[0-1])");
	std::wregex format6(L"(0[1-9]|1[0-2])\\-(0[1-9]|[1-2][0-9]|3[0-1])\\-[0-9]{4}");

	std::wregex format7(L"(0[1-9]|[1-2][0-9]|3[0-1])\\/(0[1-9]|1[0-2])\\/[0-9]{4}");
	std::wregex format8(L"[0-9]{4}\\/(0[1-9]|1[0-2])\\/(0[1-9]|[1-2][0-9]|3[0-1])");
	std::wregex format9(L"(0[1-9]|1[0-2])\\/(0[1-9]|[1-2][0-9]|3[0-1])\\/[0-9]{4}");

	std::vector<std::wregex> formatList{format1, format2, format3, format4, format5, format6,
		format7, format8, format9};
	for(auto i:formatList)
	{
		if(std::regex_match(Cellvalue, i))
		{
			return true;
		}
	}
	return false;
}
