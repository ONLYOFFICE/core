/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "conversionmathformula.h"
#include "cconversionsmtoooxml.h"
#include "cstarmathpars.h"
#include "TFormulaSize.h"

namespace StarMath
{

	CStarMathConverter::CStarMathConverter():m_pParser(nullptr)
	{
		m_pParser = new CParserStarMathString;
	}
	CStarMathConverter::~CStarMathConverter()
	{
		if(m_pParser != nullptr)
			delete m_pParser;
	}

	void CStarMathConverter::SetBaseFont(const std::wstring &wsNameFont)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseFont(wsNameFont);
	}

	void CStarMathConverter::SetBaseSize(const unsigned int &nSize)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseSize(nSize);
	}

	void CStarMathConverter::SetBaseAlignment(const unsigned int &nAlignment)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseAlignment(nAlignment);
	}

	void CStarMathConverter::SetBaseItalic(bool bItal)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseItalic(bItal);
	}

	void CStarMathConverter::SetBaseBold(bool bBold)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseBold(bBold);
	}

	std::queue<TFormulaSize> CStarMathConverter::GetFormulaSize()
	{
		if(m_pParser != nullptr)
			return m_pParser->GetFormulaSize();
		return std::queue<TFormulaSize>{};
	}

	std::wstring CStarMathConverter::ConvertStarMathToOOXml(const std::wstring &wsFormula, const unsigned int &iTypeConversion)
	{
		StarMath::CConversionSMtoOOXML oConverterSM;
		oConverterSM.StartConversion(m_pParser->Parse(wsFormula,iTypeConversion),m_pParser->GetAlignment());
		return oConverterSM.GetOOXML();
	}
	std::wstring CStarMathConverter::ConvertEQNToOOXml(const std::wstring& wsFormula)
	{
		StarMath::CConversionSMtoOOXML oConverterEQN;
		oConverterEQN.StartConversion(StarMath::CParserStarMathString().ParseEQN(wsFormula));
		return oConverterEQN.GetOOXML();
	}

}
