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

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CDefinedName : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDefinedName)
			CDefinedName(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
            virtual ~CDefinedName()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_oRef = oReader.GetText3();
			}

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::Name*>(obj.get());
				if (ptr != nullptr)
				{
					if (m_oComment.IsInit())
						ptr->comment = m_oComment.get();
					else
						ptr->comment = L"";

					if (m_oDescription.IsInit())
						ptr->description = m_oDescription.get();
					else
						ptr->description = L"";

					if (m_oFunctionGroupId.IsInit())
						ptr->fGrp = m_oFunctionGroupId->GetValue();
					else
						ptr->fGrp = 0;

					if (m_oHelp.IsInit())
						ptr->helpTopic = m_oHelp.get();
					else
						ptr->helpTopic = L"";

					if (m_oHidden.IsInit())
						ptr->fHidden = m_oHidden->GetValue();
					else
						ptr->fHidden = false;

					if (m_oFunction.IsInit())
						ptr->fFunc = m_oFunction->GetValue();
					else
						ptr->fFunc = false;

					if (m_oVbProcedure.IsInit())
						ptr->fOB = m_oVbProcedure->GetValue();
					else
						ptr->fOB = false;

					if (m_oXlm.IsInit())
						ptr->fFutureFunction = m_oXlm->GetValue();
					else
						ptr->fFutureFunction = false;

					if (m_oPublishToServer.IsInit())
						ptr->fPublished = m_oPublishToServer->GetValue();
					else
						ptr->fPublished = false;

					if (m_oWorkbookParameter.IsInit())
						ptr->fWorkbookParam = m_oWorkbookParameter->GetValue();
					else
						ptr->fWorkbookParam = false;

					if (m_oLocalSheetId.IsInit())
						ptr->itab = m_oLocalSheetId->GetValue();
					else
						ptr->itab = 0xFFFFFFFF;

					if (m_oName.IsInit())
						ptr->name = m_oName.get();
					else
						ptr->name = L"";

					if (m_oShortcutKey.IsInit())
						ptr->chKey = m_oShortcutKey.get()[0];
					else
						ptr->chKey = 0;

                    if (m_oRef.IsInit())
                        ptr->rgce = m_oRef.get();
                    else
                        ptr->rgce = std::wstring(L"");
				}
			}

			virtual EElementType getType () const
			{
				return et_x_DefinedName;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader); // -> SheetData.cpp

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::Name*>(obj.get());
				if (ptr != nullptr)
				{
					m_oComment = ptr->comment.value();
					// m_oCustomMenu               = ptr->.value();
					m_oDescription = ptr->description.value();
					m_oFunctionGroupId = ptr->fGrp;
					m_oHelp = ptr->helpTopic.value();

					if(ptr->fHidden != false)
						m_oHidden = ptr->fHidden;

					if (ptr->fFunc != false)
						m_oFunction = ptr->fFunc;

					if (ptr->fOB != false)
						m_oVbProcedure = ptr->fOB;

					if (ptr->fFutureFunction != false)
						m_oXlm = ptr->fFutureFunction;

					if (ptr->fPublished != false)
						m_oPublishToServer = ptr->fPublished;

					if (ptr->fWorkbookParam != false)
						m_oWorkbookParameter = ptr->fWorkbookParam;

					if (ptr->itab != 0xFFFFFFFF)
						m_oLocalSheetId = ptr->itab;

					m_oName = ptr->name.value();
					m_oShortcutKey = std::to_wstring(ptr->chKey);
					//m_oStatusBar                = ;
					m_oRef = ptr->rgce.getAssembledFormula();
				}

            }

		public:
				nullable_string									m_oComment;
				nullable_string									m_oCustomMenu;
				nullable_string									m_oDescription;
				nullable<SimpleTypes::COnOff<>>					m_oFunction;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oFunctionGroupId;
				nullable_string									m_oHelp;
				nullable<SimpleTypes::COnOff<>>					m_oHidden;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oLocalSheetId;
				nullable_string									m_oName;
				nullable<SimpleTypes::COnOff<>>					m_oPublishToServer;
				nullable_string									m_oShortcutKey;
				nullable_string									m_oStatusBar;
				nullable<SimpleTypes::COnOff<>>					m_oVbProcedure;
				nullable<SimpleTypes::COnOff<>>					m_oWorkbookParameter;
				nullable<SimpleTypes::COnOff<>>					m_oXlm;

				nullable_string									m_oRef;
		};

		class CDefinedNames : public WritingElementWithChilds<CDefinedName>
		{
		public:
			WritingElement_AdditionConstructors(CDefinedNames)
            WritingElement_XlsbVectorConstructors(CDefinedNames)
			CDefinedNames(OOX::Document *pMain = NULL) : WritingElementWithChilds<CDefinedName>(pMain)
			{
			}            
			virtual ~CDefinedNames()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

            void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
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

			void toBin(std::vector<XLS::BaseObjectPtr>& obj)
			{
				obj.reserve(m_arrItems.size());
				for (size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (m_arrItems[i])
					{
						XLS::BaseObjectPtr item(new XLSB::Name());
						m_arrItems[i]->toBin(item);
						obj.push_back(item);
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_DefinedNames;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX
