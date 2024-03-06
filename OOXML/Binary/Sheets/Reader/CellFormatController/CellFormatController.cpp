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
#include "DigitReader.h"

#include "../../../../XlsxFormat/Styles/NumFmts.h"
#include "../../../../XlsxFormat/Styles/Xfs.h"

#include <regex>
#include <chrono>
#include <iomanip>
#include <cctype>


const std::wstring DefaultDateFormat = L"dd.mm.yyyy";
const std::wstring DefaultPercentFormat = L"0.0%";
const std::wstring DefaultDollarFormat = L"#,##0.00$";

std::map<std::wstring, std::uint16_t> defaultDataFormats
{
	{DefaultDateFormat, 14},
	{L"d-mmm-yy", 15},
	{L"d-mmm", 16},
	{L"mmm-yy", 17},
	{L"m/d/yy h:mm", 22},
	{L"h:mm", 20}
};

CellFormatController::CellFormatController(OOX::Spreadsheet::CStyles *styles):
	m_pStyles{styles}
{
	// Добавим стили для wrap-а
	m_pStyles->m_oCellXfs.Init();
	m_pStyles->m_oCellXfs->m_oCount.Init();
	m_pStyles->m_oCellXfs->m_oCount->SetValue(2);

	// Normall default
	auto pXfs = new OOX::Spreadsheet::CXfs();
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

	createFormatStyle(DefaultDateFormat);
	createFormatStyle(DefaultPercentFormat);
}

void CellFormatController::ProcessCellType(OOX::Spreadsheet::CCell *pCell, const std::wstring &value, bool bIsWrap)
{

	pCell_ = pCell;
	/// формат для булева значения в верхнем регистре
	if(value == L"true" || value == L"false")
	{
		pCell_->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeInlineStr);
		pCell_->m_oRichText.Init();
		OOX::Spreadsheet::CText *pText = new OOX::Spreadsheet::CText();
		auto tempValue = value;
		std::transform(tempValue.begin(), tempValue.end(), tempValue.begin(),
                 [](unsigned char c) { return std::toupper(c); });
		pText->m_sText = tempValue;
		pCell_->m_oRichText->m_arrItems.push_back(pText);
		return;
	}
	DigitReader digits = {};
	std::wstring digitFormat = {};
	std::wstring digitValue = {};
	if(digits.ReadDigit(value, digitValue, digitFormat))
	{
		if(!pCell_->m_oValue.IsInit())
		{
			pCell_->m_oValue.Init();
		}
		pCell_->m_oValue->m_sText = digitValue;
		std::map<std::wstring, unsigned int>::iterator pFind = mapDataNumber_.find(digitFormat);
		if (pFind != mapDataNumber_.end())
		{
			pCell_->m_oStyle = pFind->second;
		}
		else
		{

			if (!m_pStyles->m_oNumFmts.IsInit())
			{
				m_pStyles->m_oNumFmts.Init();
			}
			if(!digitFormat.empty())
			{
				createFormatStyle(digitFormat);
				pCell_->m_oStyle = mapDataNumber_.at(digitFormat);
			}
		}
		if (bIsWrap)
		{
			pCell_->m_oStyle = 1;
		}
		return;
	}

	DateReader dateReader = {};
	_INT32 digitalDate  = 0;
	auto validDate = dateReader.GetDigitalDate(value, digitalDate);
	if(validDate)
	{
		if(!pCell_->m_oValue.IsInit())
		{
			pCell_->m_oValue.Init();
		}
		pCell_->m_oValue->m_sText = std::to_wstring(digitalDate);
		std::map<std::wstring, unsigned int>::iterator pFind = mapDataNumber_.find(DefaultDateFormat);
		pCell_->m_oStyle = pFind->second;
	}
	else
	{
		if (value[0] == L'='/* && bCalcFormulas*/)
		{
			pCell_->m_oFormula.Init();
			pCell_->m_oFormula->m_sText = value;
		}
		else
		{
			pCell_->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeInlineStr);
			pCell_->m_oRichText.Init();
			OOX::Spreadsheet::CText *pText = new OOX::Spreadsheet::CText();
			pText->m_sText = value;
			pCell_->m_oRichText->m_arrItems.push_back(pText);
		}
	}

	if (bIsWrap)
	{
		pCell_->m_oStyle = 1;
	}

}

void CellFormatController::createFormatStyle(const std::wstring &format)
{
	auto prepareFormat = defaultDataFormats.find(format);
	if(prepareFormat == defaultDataFormats.end())
	{
		if (!m_pStyles->m_oNumFmts.IsInit())
		{
			m_pStyles->m_oNumFmts.Init();
		}
		m_pStyles->m_oNumFmts->m_arrItems.push_back(new OOX::Spreadsheet::CNumFmt());
		m_pStyles->m_oNumFmts->m_arrItems.back()->m_oFormatCode = format;
		m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId.Init();
		m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId->SetValue(164 + m_pStyles->m_oNumFmts->m_arrItems.size());
	}
	// Normal + data format
	OOX::Spreadsheet::CXfs* pXfs = new OOX::Spreadsheet::CXfs();

	pXfs->m_oBorderId.Init();	pXfs->m_oBorderId->SetValue(0);
	pXfs->m_oFillId.Init();		pXfs->m_oFillId->SetValue(0);
	pXfs->m_oFontId.Init();		pXfs->m_oFontId->SetValue(0);
	pXfs->m_oNumFmtId.Init();
	if(prepareFormat == defaultDataFormats.end())
		pXfs->m_oNumFmtId->SetValue(m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId->GetValue());
	else
		pXfs->m_oNumFmtId->SetValue(prepareFormat->second);
	m_pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);

	auto styleNum = (unsigned int)(m_pStyles->m_oCellXfs->m_arrItems.size() - 1);
	mapDataNumber_.insert(std::make_pair(format, styleNum));
}

