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
#ifndef PPTX_LOGIC_C_NV_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_C_NV_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"

#include "Hyperlink.h"
#include "ExtP.h"

namespace PPTX
{
	namespace Logic
	{
		class CNvPr : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNvPr)

			CNvPr(std::wstring ns = L"p")
			{
				m_namespace = ns;
			}

			virtual OOX::EElementType getType () const
			{
				return OOX::et_p_cNvPr;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				nullable_int id_;
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("id"),	id_)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("name"),	name)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("descr"), descr)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("hidden"), hidden)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("title"), title)
				WritingElement_ReadAttributes_End( oReader )
				
				id = id_.get_value_or(0);
				Normalize();
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_namespace = XmlUtils::GetNamespace(oReader.GetName());

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
					
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (strName == L"hlinkClick")
						hlinkClick = oReader;
					else if (strName == L"hlinkHover")
						hlinkHover = oReader;
					else if (strName == L"extLst")
					{
						if ( oReader.IsEmptyNode() )
							continue;

						int nParentDepth1 = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
						{
							Ext ext;
							ext.fromXML(oReader);
							if (ext.spid.IsInit())
							{
								vmlSpid = ext.spid;
								break;
							}
						}
					}

				}
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_namespace = XmlUtils::GetNamespace(node.GetName());

				id		= node.ReadAttributeInt(L"id");
				name	= node.GetAttribute(L"name");
				
				XmlMacroReadAttributeBase(node, L"descr", descr);
				XmlMacroReadAttributeBase(node, L"hidden", hidden);
				XmlMacroReadAttributeBase(node, L"title", title);

				hlinkClick = node.ReadNode(_T("a:hlinkClick"));
				hlinkHover = node.ReadNode(_T("a:hlinkHover"));

				XmlUtils::CXmlNode list = node.ReadNodeNoNS(_T("extLst"));
				if (list.IsValid())
				{		
					XmlUtils::CXmlNodes oNodes;
					if (list.GetNodes(_T("*"), oNodes))
					{
						int nCount = oNodes.GetCount();
						for (int i = 0; i < nCount; ++i)
						{
							XmlUtils::CXmlNode oNode;
							oNodes.GetAt(i, oNode);	

							Ext ext;
							ext.fromXML(oNode);
							if (ext.spid.IsInit())
							{
								vmlSpid = ext.spid;
								break;
							}
						}
					}
				}

				Normalize();
			}

			
			std::wstring toXML2(std::wstring node_name) const
			{
				XmlUtils::CAttribute oAttr;
									oAttr.Write(_T("id"),		id);
									oAttr.Write(_T("name"),		XmlUtils::EncodeXmlString(name));
                if (descr.IsInit())
                {
                    std::wstring d = XmlUtils::EncodeXmlString(descr.get());
                    XmlUtils::replace_all(d, L"\n", L"&#xA;");

                    oAttr.Write(_T("descr"), d);
                }
									oAttr.Write(_T("hidden"),	hidden);
				if (title.IsInit())	oAttr.Write(_T("title"),	XmlUtils::EncodeXmlString(title.get()));

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(hlinkClick);
				oValue.WriteNullable(hlinkHover);

				return XmlUtils::CreateNode(node_name.empty() ? (m_namespace + L":cNvPr") : node_name, oAttr, oValue);
			}
			virtual std::wstring toXML() const
			{
				return toXML2(L"");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
                std::wstring namespace_ = m_namespace;
				
				if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)			namespace_= L"pic";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_= L"xdr";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_= L"a";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";

                toXmlWriter2(namespace_, pWriter);

			}
			void toXmlWriter2(const std::wstring& strNS, NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(strNS + _T(":cNvPr"));

				int _id = id;
				if (_id < 0)
				{
					_id = pWriter->m_lObjectId;
					++pWriter->m_lObjectId;
				}
				else
				{
					if (pWriter->m_lObjectId <= _id)
					{
						pWriter->m_lObjectId = _id + 1;
					}
				}

				pWriter->StartAttributes();
									pWriter->WriteAttribute (_T("id"),      _id);
									pWriter->WriteAttribute (_T("name"),    XmlUtils::EncodeXmlString(name));
				if (descr.IsInit())
				{
					std::wstring d = XmlUtils::EncodeXmlString(descr.get());
					XmlUtils::replace_all(d, L"\n", L"&#xA;");	

					pWriter->WriteAttribute	(_T("descr"), d);
				}
									pWriter->WriteAttribute (_T("hidden"),  hidden);
				if (title.IsInit())	pWriter->WriteAttribute (_T("title"),   XmlUtils::EncodeXmlString(title.get()));

				pWriter->EndAttributes();

				pWriter->Write(hlinkClick);
				pWriter->Write(hlinkHover);

				pWriter->EndNode(strNS + _T(":cNvPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt1(0, id);
				pWriter->WriteString1(1, name);
				pWriter->WriteBool2(2, hidden);
				pWriter->WriteString2(3, title);
				pWriter->WriteString2(4, descr);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, hlinkClick);
				pWriter->WriteRecord2(1, hlinkHover);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				id = 1;
				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
				
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							id = pReader->GetLong();
							break;
						}
						case 1:
						{
							name = pReader->GetString2();
							break;
						}
						case 2:
						{
							hidden = pReader->GetBool();
							break;
						}
						case 3:
						{
							title = pReader->GetString2();
							break;
						}
						case 4:
						{
							descr = pReader->GetString2();
							break;
						}
						default:
							break;
					}
				}
				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							hlinkClick = new PPTX::Logic::Hyperlink(L"hlinkClick");
							hlinkClick->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							hlinkHover = new PPTX::Logic::Hyperlink(L"hlinkHover");
							hlinkHover->fromPPTY(pReader);
							break;
						}
						default:
						{
							break;
						}
					}
				}

				pReader->Seek(_end_rec);
			}

			std::wstring		m_namespace;

			int					id;
			std::wstring		name;
			nullable_string		descr;
			nullable_bool		hidden;
			nullable_string		title;
			nullable<Hyperlink> hlinkClick;
			nullable<Hyperlink> hlinkHover;
			
			//std::vector<Ext>	extLst;
			nullable_string		vmlSpid;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(hlinkClick.IsInit())
					hlinkClick->SetParentPointer(this);
				if(hlinkHover.IsInit())
					hlinkHover->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				if (id < 0)
					id = 0;
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_C_NV_PROPERTIES_INCLUDE_H
