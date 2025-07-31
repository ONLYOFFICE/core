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

#include "Hyperlinks.h"
#include "../../XlsbFormat/Biff12_records/HLink.h"

#include "../../Common/SimpleTypes_Shared.h"

#include "../../XlsbFormat/Biff12_unions/HLINKS.h"
#include "../../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheWriter.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/HLINK.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/HLink.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CHyperlink::CHyperlink(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CHyperlink::~CHyperlink()
		{
		}
		void CHyperlink::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CHyperlink::toXML() const
		{
			return (L"");
		}
		void CHyperlink::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<hyperlink"));
			WritingStringNullableAttrEncodeXmlString(L"display", m_oDisplay, m_oDisplay.get());
			WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
			WritingStringNullableAttrEncodeXmlString(L"location", m_oLocation, m_oLocation.get());
			WritingStringNullableAttrEncodeXmlString(L"ref", m_oRef, m_oRef.get());
			WritingStringNullableAttrEncodeXmlString(L"tooltip", m_oTooltip, m_oTooltip.get());
			writer.WriteString((L"/>"));
		}
		void CHyperlink::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CHyperlink::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CHyperlink::toBin()
		{
			auto castedPtr(new XLSB::HLink);
			XLS::BaseObjectPtr ptr(castedPtr);

			if(m_oDisplay.IsInit())
				castedPtr->display = m_oDisplay.get();
            else
                castedPtr->display = L"";
			if(m_oRid.IsInit())
				castedPtr->relId.value = m_oRid->GetValue();
            else
                castedPtr->relId.value = L"";
			if(m_oLocation.IsInit())
				castedPtr->location = m_oLocation.get();
            else
                castedPtr->location = L"";
			if(m_oRef.IsInit())
				castedPtr->rfx = m_oRef.get();
			if(m_oTooltip.IsInit())
				castedPtr->tooltip = m_oTooltip.get();
            else
                castedPtr->tooltip = L"";

			return ptr;
		}
		XLS::BaseObjectPtr CHyperlink::toXLS()
		{
			auto unionPtr = new XLS::HLINK;
			auto ptr = new XLS::HLink;
			unionPtr->m_HLink = XLS::BaseObjectPtr(ptr);
			if(m_oRef.IsInit())
				ptr->ref8.fromString(m_oRef.get());
			if(m_oDisplay.IsInit())
			{
				ptr->hyperlink.hlstmfHasDisplayName = true;
				ptr->hyperlink.displayName = m_oDisplay.get();
			}
			if(m_oLocation.IsInit())
			{
				ptr->hyperlink.hlstmfHasLocationStr = true;
				ptr->hyperlink.location = m_oLocation.get();
			}
			if(m_oRef.IsInit())
			{
				ptr->hyperlink.hlstmfHasMoniker = true;
				ptr->hyperlink.hlstmfMonikerSavedAsStr = true;
				ptr->hyperlink.moniker = m_oRef.get();
			}
			return XLS::BaseObjectPtr(unionPtr);
		}
        void CHyperlink::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_HLink);
            {
                XLSB::UncheckedRfX rfx;
                if(m_oRef.IsInit())
                    rfx = m_oRef.get();
                *record <<rfx;
            }
            {
                XLSB::XLWideString rellId;
                if(m_oRid.IsInit())
                    rellId = m_oRid->GetValue();
                else
                    rellId = L"";
                *record << rellId;
            }
            {
                XLSB::XLWideString loc;
                if(m_oLocation.IsInit())
                    loc = m_oLocation.get();
                else
                    loc = L"";
                *record << loc;
            }
            {
                XLSB::XLWideString tooltip ;
                if(m_oTooltip.IsInit())
                    tooltip = m_oTooltip.get();
                else
                    tooltip = L"";
                *record << tooltip;
            }
            {
                XLSB::XLWideString display  ;
                if(m_oDisplay.IsInit())
                    display = m_oDisplay.get();
                else
                    display = L"";
                *record << display ;
            }
            writer->storeNextRecord(record);

        }
		EElementType CHyperlink::getType () const
		{
			return et_x_Hyperlink;
		}
		void CHyperlink::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, (L"display"),	m_oDisplay)
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"r:id"),		m_oRid )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"relationships:id"), m_oRid )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"location"),	m_oLocation )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"ref"),		m_oRef )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"tooltip"),	m_oTooltip )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CHyperlink::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::HLink*>(obj.get());
			m_oDisplay          = ptr->display.value();
			m_oRid              = ptr->relId.value.value();
			m_oLocation         = ptr->location.value();
			m_oRef              = ptr->rfx.toString(true, true);
			m_oTooltip          = ptr->tooltip.value();

		}

		CHyperlinks::CHyperlinks(OOX::Document *pMain) : WritingElementWithChilds<CHyperlink>(pMain)
		{
		}
		CHyperlinks::~CHyperlinks()
		{
		}
		void CHyperlinks::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CHyperlinks::toXML() const
		{
			return (L"");
		}
		void CHyperlinks::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_arrItems.empty()) return;

			writer.WriteString((L"<hyperlinks>"));

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString((L"</hyperlinks>"));
		}
		void CHyperlinks::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( (L"hyperlink") == sName )
				{
					CHyperlink *pHyperlink = new CHyperlink();
					m_arrItems.push_back(pHyperlink);

					pHyperlink->fromXML(oReader);
				}
			}
		}
		void CHyperlinks::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			if (obj.empty())
				return;

			for(auto &hyperlink : obj)
			{
				CHyperlink *pHyperlink = new CHyperlink(m_pMainDocument);
				m_arrItems.push_back(pHyperlink);

				pHyperlink->fromBin(hyperlink);
			}
		}
		XLS::BaseObjectPtr CHyperlinks::toBin()
		{

			auto castedPtr(new XLSB::HLINKS);
			XLS::BaseObjectPtr ptr(castedPtr);

			for(auto i:m_arrItems)
			{
				castedPtr->m_arHlinks.push_back(i->toBin());
			}
			return ptr;
		}
		std::vector<XLS::BaseObjectPtr> CHyperlinks::toXLS()
		{
			std::vector<XLS::BaseObjectPtr> objectVector;
			for(auto i:m_arrItems)
			{
				objectVector.push_back(i->toXLS());
			}
			return objectVector;
		}
        void CHyperlinks::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            for(auto i : m_arrItems)
            {
                i->toBin(writer);
            }
        }

		EElementType CHyperlinks::getType () const
			{
				return et_x_Hyperlinks;
			}

	} //Spreadsheet
} // namespace OOX
