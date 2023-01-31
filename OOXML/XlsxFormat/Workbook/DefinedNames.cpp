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

#include "DefinedNames.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CDefinedName::CDefinedName(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CDefinedName::~CDefinedName()
		{
		}
		void CDefinedName::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDefinedName::toXML() const
		{
			return L"";
		}
		void CDefinedName::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<definedName");
			WritingStringNullableAttrEncodeXmlString(L"name", m_oName, *m_oName);
			WritingStringNullableAttrInt(L"localSheetId", m_oLocalSheetId, m_oLocalSheetId->GetValue());
			WritingStringNullableAttrBool(L"hidden", m_oHidden);
			writer.WriteString(L">");
			if(m_oRef.IsInit())
				writer.WriteEncodeXmlString(*m_oRef);
			writer.WriteString(L"</definedName>");
		}
		void CDefinedName::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			m_oRef = oReader.GetText3();
		}
		void CDefinedName::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CDefinedName::getType () const
		{
			return et_x_DefinedName;
		}
        // void CDefinedName::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
        // ->sheetData
        void CDefinedName::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::Name*>(obj.get());
			m_oComment                  = ptr->comment.value();
		   // m_oCustomMenu               = ptr->.value();
			m_oDescription              = ptr->description.value();
			m_oFunction                 = ptr->fFunc;
			m_oFunctionGroupId          = ptr->fGrp;
			m_oHelp                     = ptr->helpTopic.value();
			m_oHidden                   = ptr->fHidden;

			if(ptr->itab != 0xFFFFFFFF)
				m_oLocalSheetId         = ptr->itab;

			m_oName                     = ptr->name.value();
			m_oPublishToServer          = ptr->fPublished;
			m_oShortcutKey              = std::to_wstring(ptr->chKey);
			//m_oStatusBar                = ;
			m_oVbProcedure              = ptr->fOB;
			m_oWorkbookParameter        = ptr->fWorkbookParam;
			m_oXlm                      = ptr->fFutureFunction;
			m_oRef                      = ptr->rgce.getAssembledFormula();

		}

		CDefinedNames::CDefinedNames(OOX::Document *pMain) : WritingElementWithChilds<CDefinedName>(pMain)
		{
		}
		CDefinedNames::~CDefinedNames()
		{
		}
		void CDefinedNames::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDefinedNames::toXML() const
		{
			return L"";
		}
		void CDefinedNames::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_arrItems.empty()) return;

			writer.WriteString(L"<definedNames>");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(L"</definedNames>");
		}
		void CDefinedNames::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"definedName" == sName || L"NamedRange" == sName)
				{
					CDefinedName *pDefinedName = new CDefinedName(m_pMainDocument);
					m_arrItems.push_back( pDefinedName);

					pDefinedName->fromXML(oReader);
				}
			}
		}
		void CDefinedNames::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			//ReadAttributes(obj);

			if (obj.empty())
				return;

			for(auto &definedName : obj)
			{
				CDefinedName *pDefinedName = new CDefinedName(m_pMainDocument);
				m_arrItems.push_back( pDefinedName);

				pDefinedName->fromBin(definedName);
			}
		}
		EElementType CDefinedNames::getType () const
		{
			return et_x_DefinedNames;
		}
		void CDefinedNames::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
