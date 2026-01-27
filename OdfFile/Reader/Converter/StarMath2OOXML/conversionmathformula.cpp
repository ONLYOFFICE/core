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
