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
#pragma once

#include "../CommonInclude.h"
#include "../Chart/ChartSerialize.h"
#include "../Styles/rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CSparkline : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSparkline)
            WritingElement_XlsbConstructors(CSparkline)
			CSparkline();
			virtual ~CSparkline();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring>										m_oRef;
			nullable<std::wstring>										m_oSqRef;
		};

		class CSparklines  : public WritingElementWithChilds<CSparkline>
		{
		public:
			WritingElement_AdditionConstructors(CSparklines)
            WritingElement_XlsbConstructors(CSparklines)
			CSparklines();
			virtual ~CSparklines();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class CSparklineGroup : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSparklineGroup)
            WritingElement_XlsbConstructors(CSparklineGroup)
			CSparklineGroup();
			virtual ~CSparklineGroup();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDouble>									m_oManualMax;
			nullable<SimpleTypes::CDouble>									m_oManualMin;
			nullable<SimpleTypes::CDouble>									m_oLineWeight;
			nullable<SimpleTypes::Spreadsheet::ST_SparklineType>			m_oType;
			nullable<SimpleTypes::COnOff>									m_oDateAxis;
			nullable<OOX::Spreadsheet::ST_DispBlanksAs>						m_oDisplayEmptyCellsAs;
			nullable<SimpleTypes::COnOff>									m_oMarkers;
			nullable<SimpleTypes::COnOff>									m_oHigh;
			nullable<SimpleTypes::COnOff>									m_oLow;
			nullable<SimpleTypes::COnOff>									m_oFirst;
			nullable<SimpleTypes::COnOff>									m_oLast;
			nullable<SimpleTypes::COnOff>									m_oNegative;
			nullable<SimpleTypes::COnOff>									m_oDisplayXAxis;
			nullable<SimpleTypes::COnOff>									m_oDisplayHidden;
			nullable<SimpleTypes::Spreadsheet::ST_SparklineAxisMinMax>		m_oMinAxisType;
			nullable<SimpleTypes::Spreadsheet::ST_SparklineAxisMinMax>		m_oMaxAxisType;
			nullable<SimpleTypes::COnOff>									m_oRightToLeft;

			nullable<OOX::Spreadsheet::CColor>					m_oColorSeries;
			nullable<OOX::Spreadsheet::CColor>					m_oColorNegative;
			nullable<OOX::Spreadsheet::CColor>					m_oColorAxis;
			nullable<OOX::Spreadsheet::CColor>					m_oColorMarkers;
			nullable<OOX::Spreadsheet::CColor>					m_oColorFirst;
			nullable<OOX::Spreadsheet::CColor>					m_oColorLast;
			nullable<OOX::Spreadsheet::CColor>					m_oColorHigh;
			nullable<OOX::Spreadsheet::CColor>					m_oColorLow;
			nullable<std::wstring>								m_oRef;
			nullable<CSparklines>								m_oSparklines;
		};

		class CSparklineGroups  : public WritingElementWithChilds<CSparklineGroup>
		{
		public:
			WritingElement_AdditionConstructors(CSparklineGroups)
            WritingElement_XlsbConstructors(CSparklineGroups)
			CSparklineGroups();
			virtual ~CSparklineGroups();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

	} //Spreadsheet
} // namespace OOX

