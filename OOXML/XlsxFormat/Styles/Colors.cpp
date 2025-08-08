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

#include "Colors.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../XlsbFormat/Biff12_unions/COLORPALETTE.h"
#include "../../XlsbFormat/Biff12_unions/INDEXEDCOLORS.h"
#include "../../XlsbFormat/Biff12_unions/MRUCOLORS.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Palette.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CColors::CColors()
		{
		}
		CColors::~CColors()
		{
		}
		void CColors::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CColors::toXML() const
		{
			return _T("");
		}
		void CColors::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CColors::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("indexedColors") == sName )
					m_oIndexedColors = oReader;
				else if ( _T("mruColors") == sName )
					m_oMruColors = oReader;
			}
		}
		void CColors::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::COLORPALETTE*>(obj.get());

			if(ptr->m_INDEXEDCOLORS != nullptr)
			{
				auto oINDEXEDCOLORS = static_cast<XLSB::INDEXEDCOLORS*>(ptr->m_INDEXEDCOLORS.get());
				if(!oINDEXEDCOLORS->m_arIndexedColor.empty())
				{
					m_oIndexedColors = oINDEXEDCOLORS->m_arIndexedColor;
				}
			}

			if(ptr->m_MRUCOLORS != nullptr)
			{
				auto oMRUCOLORS = static_cast<XLSB::MRUCOLORS*>(ptr->m_MRUCOLORS.get());
				if(!oMRUCOLORS->m_arMRUColor.empty())
				{
					m_oMruColors = oMRUCOLORS->m_arMRUColor;
				}
			}
		}
		XLS::BaseObjectPtr CColors::toBin()
		{
			auto ptr(new XLSB::COLORPALETTE);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oIndexedColors.IsInit())
			{
			auto indexColors(new XLSB::INDEXEDCOLORS);
                ptr->m_INDEXEDCOLORS = XLS::BaseObjectPtr{indexColors};
			indexColors->m_arIndexedColor =	m_oIndexedColors->toBin();
			}

			if(m_oMruColors.IsInit())
			{
				auto mruColors(new XLSB::MRUCOLORS);
				ptr->m_MRUCOLORS = XLS::BaseObjectPtr{mruColors};
                mruColors->m_arMRUColor = m_oMruColors->toBin();
			}

			return objectPtr;
		}
		XLS::BaseObjectPtr CColors::toXLS()
		{
			auto ptr = new XLS::Palette;

			if(m_oIndexedColors.IsInit())
			{
				for(auto i : m_oIndexedColors->m_arrItems)
					ptr->rgColor.push_back(i->toXLS());
			}

			return XLS::BaseObjectPtr(ptr);
		}
		EElementType CColors::getType () const
		{
			return et_x_Colors;
		}
		void CColors::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}
		void CColors::initXLSColors()
		{
			m_oIndexedColors = CIndexedColors();
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF000000";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFFFFFF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFF0000";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF00FF00";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF0000FF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFFFF00";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFF00FF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF00FFFF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF800000";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF008000";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF000080";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF808000";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF800080";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF008080";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFC0C0C0";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF808080";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF9999FF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF993366";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFFFFCC";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFCCFFFF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF660066";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFF8080";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF0066CC";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFCCCCFF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF000080";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFF00FF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFFFF00";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF00FFFF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF800080";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF800000";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF008080";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF0000FF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF00CCFF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFCCFFFF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFCCFFCC";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFFFF99";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF99CCFF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFF99CC";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFCC99FF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFFCC99";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF3366FF";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF33CCCC";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF81D41A";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFFCC00";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFF9900";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFFF6600";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF666699";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF969696";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF003366";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF339966";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF003300";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF333300";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FFC9211E";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF993366";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF333399";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}
			{
				auto tempColor = new CRgbColor;
				tempColor->m_oRgb = L"FF333333";
				m_oIndexedColors->m_arrItems.push_back(tempColor);
			}

		}

	} //Spreadsheet
} // namespace OOX

