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

#include "DefinedNames.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"

#include "../../Common/SimpleTypes_Shared.h"

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
			if (false == m_oRef.IsInit() && false == m_oName.IsInit()) return;

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
			 if(m_oName.IsInit())
				XLS::GlobalWorkbookInfo::arDefineNames_static.push_back(m_oName.get());
		}
		XLS::BaseObjectPtr CDefinedName::toBin()
		{
			auto ptr(new XLSB::Name);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oComment.IsInit())
                ptr->comment = m_oComment.get();
            else
                ptr->comment.setSize(0xFFFFFFFF);

			if(m_oDescription.IsInit())
                ptr->description = m_oDescription.get();
            else
                ptr->description.setSize(0xFFFFFFFF);
			if(m_oFunction.IsInit())
                ptr->fFunc = m_oFunction->GetValue();
            else
                ptr->fFunc = false;
			if(m_oFunctionGroupId.IsInit())
                ptr->fGrp = m_oFunctionGroupId->GetValue();

			if(m_oHelp.IsInit())
                ptr->helpTopic = m_oHelp.get();
            else
                ptr->helpTopic.setSize(0xFFFFFFFF);
			if(m_oHidden.IsInit())
                ptr->fHidden = m_oHidden->GetValue();
            else
                ptr->fHidden = false;

			if (m_oLocalSheetId.IsInit())
				ptr->itab = m_oLocalSheetId->GetValue();
			else
				ptr->itab = 0xFFFFFFFF;

			if (m_oName.IsInit())
                ptr->name = m_oName.get();
            else
                ptr->name.setSize(0xFFFFFFFF);
			if (m_oPublishToServer.IsInit())
                ptr->fPublished = m_oPublishToServer->GetValue();
			else
				ptr->fPublished = false;
			if (m_oShortcutKey.IsInit())
                ptr->chKey = std::stoi(m_oShortcutKey.get());
			else
				ptr->chKey = 0;

			if (m_oVbProcedure.IsInit())
                ptr->fOB = m_oVbProcedure->GetValue();
            else
                ptr->fOB = false;
            if(!ptr->fOB)
                ptr->fProc = false;
            if(!ptr->fProc)
            {
                ptr->unusedstring1.setSize(0xFFFFFFFF);
                ptr->unusedstring2.setSize(0xFFFFFFFF);
            }

			if (m_oWorkbookParameter.IsInit())
                ptr->fWorkbookParam = m_oWorkbookParameter->GetValue();
			else
				ptr->fWorkbookParam = false;
			if (m_oXlm.IsInit())
                ptr->fFutureFunction = m_oXlm->GetValue();
			else
				ptr->fFutureFunction = false;
			if (m_oRef.IsInit())
                ptr->rgce = m_oRef.get();
            ptr->fCalcExp = true;
			ptr->fBuiltin = false;
			return objectPtr;
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

			std::wstring ref = ptr->rgce.getAssembledFormula();
			
			if (ref.empty()) return; // ???
			
			m_oComment = ptr->comment.value();
		   // m_oCustomMenu = ptr->.value();
			m_oDescription = ptr->description.value();
			m_oFunction = ptr->fFunc;
			m_oFunctionGroupId = ptr->fGrp;
			m_oHelp = ptr->helpTopic.value();

			if (ptr->fHidden)
				m_oHidden = ptr->fHidden;

			if(ptr->itab != 0xFFFFFFFF)
				m_oLocalSheetId         = ptr->itab;

			m_oName = ptr->name.value();
			m_oPublishToServer = ptr->fPublished;
			m_oShortcutKey = std::to_wstring(ptr->chKey);
			//m_oStatusBar = ;
			m_oVbProcedure = ptr->fOB;
			m_oWorkbookParameter = ptr->fWorkbookParam;
			m_oXlm = ptr->fFutureFunction;
			m_oRef = ref;
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
		std::vector<XLS::BaseObjectPtr> CDefinedNames::AddFutureFunctions(const _UINT32 namesStart)
		{
			std::vector<XLS::BaseObjectPtr> FunctionsVector;
			if(XLS::GlobalWorkbookInfo::arDefineNames_static.empty())
				return FunctionsVector;
			for(auto i = namesStart; i < XLS::GlobalWorkbookInfo::arDefineNames_static.size(); i++)
			{
                if(XLS::GlobalWorkbookInfo::arDefineNames_static[i].substr(0, 6) == L"_xlfn.")
				{
					FunctionsVector.push_back(createFutureFunction(XLS::GlobalWorkbookInfo::arDefineNames_static[i]));
				}
				else
				{
					FunctionsVector.push_back(createCustomFunction(XLS::GlobalWorkbookInfo::arDefineNames_static[i]));
				}
			}
			return FunctionsVector;
		}
		std::vector<XLS::BaseObjectPtr> CDefinedNames::toBin()
		{
			std::vector<XLS::BaseObjectPtr> objectVector;

			for(auto i:m_arrItems)
				objectVector.push_back(i->toBin());
			
			auto functionsVector = AddFutureFunctions(m_arrItems.size());
			if(!functionsVector.empty())
			{
				std::copy(functionsVector.begin(), functionsVector.end(), std::back_inserter(objectVector));
			}

			return objectVector;
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
		XLS::BaseObjectPtr CDefinedNames::createFutureFunction(const std::wstring& funcName)
		{
			auto ptr(new XLSB::Name);
			XLS::BaseObjectPtr objectPtr(ptr);

			ptr->chKey = 0;
			ptr->fHidden = true;
			ptr->fFunc = true;
			ptr->fOB = false;
			ptr->fProc = true;
			ptr->fCalcExp = false;
			ptr->fGrp = true;
			ptr->fPublished = false;
			ptr->fBuiltin = false;
			ptr->fWorkbookParam = false;
			ptr->comment.setSize(0xFFFFFFFF);
			ptr->itab  = 0xFFFFFFFF;

			ptr->fFutureFunction = true;
			ptr->name = funcName;
			ptr->rgce.parseStringFormula(L"#NAME?", L"");
			ptr->description.setSize(0xFFFFFFFF);
			ptr->helpTopic.setSize(0xFFFFFFFF);
			ptr->unusedstring1.setSize(0xFFFFFFFF);
			ptr->unusedstring2.setSize(0xFFFFFFFF);
			return objectPtr;
		}

		XLS::BaseObjectPtr CDefinedNames::createCustomFunction(const std::wstring& funcName)
		{
			auto ptr(new XLSB::Name);
			XLS::BaseObjectPtr objectPtr(ptr);

			ptr->chKey = 0;
			ptr->fHidden = false;
			ptr->fFunc = true;
			ptr->fOB = true;
			ptr->fProc = true;
			ptr->fCalcExp = false;
			ptr->fGrp = false;
			ptr->fPublished = false;
			ptr->fBuiltin = false;
			ptr->fWorkbookParam = false;
			ptr->itab  = 0xFFFFFFFF;
			ptr->comment.setSize(0xFFFFFFFF);

			ptr->description.setSize(0xFFFFFFFF);
			ptr->helpTopic.setSize(0xFFFFFFFF);
			ptr->unusedstring1.setSize(0xFFFFFFFF);
			ptr->unusedstring2.setSize(0xFFFFFFFF);

			ptr->fFutureFunction = false;
			ptr->name = funcName;
			return objectPtr;
		}

	} //Spreadsheet
} // namespace OOX
