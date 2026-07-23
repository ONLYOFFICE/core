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
#pragma once

#include <vector>
#include <string>

#include "typeConversion.h"
#include "TextDirection.h"
#include "typeselements.h"

namespace XmlUtils
{
class CXmlWriter;
}
namespace StarMath
{
    class CElement;
    class CAttribute;

    class CConversionSMtoOOXML
	{
	public:
		CConversionSMtoOOXML();
		~CConversionSMtoOOXML();
		void StartConversion(const std::vector<CElement*> arPars, const unsigned int& iAlignment = 1);
        static void StandartProperties(XmlUtils::CXmlWriter* pXmlWriter, CAttribute* pAttribute,const TypeConversion& enTypeConversion, const TextDirection& enTypeLang = TextDirection::LeftToRight);
		static void PropertiesMFPR(const std::wstring& wsType,XmlUtils::CXmlWriter* pXmlWriter,CAttribute* pAttribute,const TypeConversion &enTypeConversion);
		static void PropertiesNaryPr(const TypeElement& enTypeOp,bool bEmptySub,bool bEmptySup,XmlUtils::CXmlWriter* pXmlWriter,CAttribute* pAttribute,const TypeConversion &enTypeConversion,const bool& bEQN = false);
		static void PropertiesFuncPr(XmlUtils::CXmlWriter* pXmlWriter,CAttribute* pAttribute,const TypeConversion &enTypeConversion);
		static void WriteNodeConversion(const std::wstring& wsNameBlock,CElement* pValueBlock,XmlUtils::CXmlWriter* pXmlWriter);
		static void PropertiesDPr(XmlUtils::CXmlWriter* pXmlWriter,const std::wstring& wsOpenBracket,const std::wstring& wsCloseBracket,CAttribute* pAttribute,const TypeConversion &enTypeConversion,const TypeElement& enTypeBracket);
		static void PropertiesMPr(XmlUtils::CXmlWriter* pXmlWriter,const TypeElement& enTypeMatrix,CAttribute* pAttribute,const TypeConversion &enTypeConversion,const unsigned int& iDimension);
		static void NodeGrade(XmlUtils::CXmlWriter* pXmlWriter,CElement* pValueGrade,CAttribute* pAttribute);
		static void WriteCtrlPrNode(XmlUtils::CXmlWriter* pXmlWriter,CAttribute* pAttribute,const TypeConversion &enTypeConversion);
		static void WriteChrNode(const std::wstring& wsTypeOp,XmlUtils::CXmlWriter* pXmlWriter);
		static void WriteLimLocNode(const std::wstring& wsTypeLimLock,XmlUtils::CXmlWriter* pXmlWriter);
		static void WriteRPrFName(const TypeElement& enTypeOp,XmlUtils::CXmlWriter* pXmlWriter,CAttribute* pAttribute,const std::wstring& wsNameOp,const TypeConversion &enTypeConversion);
		static void WriteStyNode(XmlUtils::CXmlWriter* pXmlWriter,const std::wstring& wsAttributeNode);
		static void WritePreserveBlock(XmlUtils::CXmlWriter* pXmlWriter, CAttribute *pAttribute,const TypeConversion &enTypeConversion);
		static void WriteLimUpOrLowNode(XmlUtils::CXmlWriter* pXmlWriter,const std::wstring& wsNameNode,CElement* pValue,const TypeElement& enType,CAttribute* pAttribute,const TypeConversion& enTypeConvers, const std::wstring& wsName = L"oper",CElement* pIndex = nullptr);
		static void ElementConversion(XmlUtils::CXmlWriter* pXmlWriter,CElement* pElement);
		void EndConversion();
		std::wstring GetOOXML();
	private:
		static void BracketTypeNotation(const std::wstring& wsOpenBracket,const std::wstring& wsCloseBracket, XmlUtils::CXmlWriter* pXmlWriter);
		XmlUtils::CXmlWriter* m_pXmlWriter = NULL;
	};
}
