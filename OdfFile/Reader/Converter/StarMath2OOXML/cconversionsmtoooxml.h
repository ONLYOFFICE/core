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

#ifndef CCONVERSIONSMTOOOXML_H
#define CCONVERSIONSMTOOOXML_H
#include "cstarmathpars.h"
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"

namespace StarMath {
//delete XmlWrite
	class CConversionSMtoOOXML
	{
	public:
		CConversionSMtoOOXML();
		~CConversionSMtoOOXML();
		void StartConversion(std::vector<CElement*> arPars, const unsigned int& iAlignment = 1);
		static void StandartProperties(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute,const TypeConversion& enTypeConversion);
		static void PropertiesMFPR(const std::wstring& wsType,XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute,const TypeConversion &enTypeConversion);
		static void PropertiesNaryPr(const TypeElement& enTypeOp,bool bEmptySub,bool bEmptySup,XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute,const TypeConversion &enTypeConversion);
		static void PropertiesFuncPr(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute,const TypeConversion &enTypeConversion);
		static void WriteNodeConversion(const std::wstring& wsNameBlock,CElement* pValueBlock,XmlUtils::CXmlWriter* pXmlWrite);
		static void PropertiesDPr(XmlUtils::CXmlWriter* pXmlWrite,const std::wstring& wsOpenBracket,const std::wstring& wsCloseBracket,CAttribute* pAttribute,const TypeConversion &enTypeConversion,const TypeElement& enTypeBracket);
		static void PropertiesMPr(XmlUtils::CXmlWriter* pXmlWrite,const TypeElement& enTypeMatrix,CAttribute* pAttribute,const TypeConversion &enTypeConversion,const unsigned int& iDimension);
		static void NodeGrade(XmlUtils::CXmlWriter* pXmlWrite,CElement* pValueGrade,CAttribute* pAttribute);
		static void WriteCtrlPrNode(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute,const TypeConversion &enTypeConversion);
		static void WriteChrNode(const std::wstring& wsTypeOp,XmlUtils::CXmlWriter* pXmlWrite);
		static void WriteLimLocNode(const std::wstring& wsTypeLimLock,XmlUtils::CXmlWriter* pXmlWrite);
		static void WriteRPrFName(const TypeElement& enTypeOp,XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute,const std::wstring& wsNameOp,const TypeConversion &enTypeConversion);
		static void WriteStyNode(XmlUtils::CXmlWriter* pXmlWrite,const std::wstring& wsAttributeNode);
		static void WritePreserveBlock(XmlUtils::CXmlWriter* pXmlWrite, CAttribute *pAttribute,const TypeConversion &enTypeConversion);
		static void WriteLimUpOrLowNode(XmlUtils::CXmlWriter* pXmlWrite,const std::wstring& wsNameNode,CElement* pValue,const TypeElement& enType,CAttribute* pAttribute,const TypeConversion& enTypeConvers, const std::wstring& wsName = L"oper",CElement* pIndex = nullptr);
		static void ElementConversion(XmlUtils::CXmlWriter* pXmlWrite,CElement* pElement);
		void EndConversion();
		std::wstring GetOOXML();
	private:
		static void BracketTypeNotation(const std::wstring& wsOpenBracket,const std::wstring& wsCloseBracket, XmlUtils::CXmlWriter* pXmlWrite);
		XmlUtils::CXmlWriter* m_pXmlWrite;
	};
}
#endif // CCONVERSIONSMTOOOXML_H
