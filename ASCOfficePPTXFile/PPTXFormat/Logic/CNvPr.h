#pragma once
#ifndef PPTX_LOGIC_C_NV_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_C_NV_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Hyperlink.h"


namespace PPTX
{
	namespace Logic
	{
		class CNvPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CNvPr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				id		= node.ReadAttributeInt(L"id");
				name	= node.GetAttribute(L"name");
				node.ReadAttributeBase(L"descr", descr);
				node.ReadAttributeBase(L"hidden", hidden);
				node.ReadAttributeBase(L"title", title);

				hlinkClick = node.ReadNode(_T("a:hlinkClick"));
				hlinkHover = node.ReadNode(_T("a:hlinkHover"));

				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("id"), id);
				oAttr.Write(_T("name"), name);
				oAttr.Write(_T("descr"), descr);
				oAttr.Write(_T("hidden"), hidden);
				oAttr.Write(_T("title"), title);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(hlinkClick);
				oValue.WriteNullable(hlinkHover);

				return XmlUtils::CreateNode(_T("p:cNvPr"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
                CString namespace_;
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
                    namespace_= _T("pic");
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
                    namespace_= _T("xdr");
				else
                    namespace_= _T("p");

                toXmlWriter2(namespace_, pWriter);

			}
			void toXmlWriter2(const CString& strNS, NSBinPptxRW::CXmlWriter* pWriter) const
			{
				//if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX && id == -1)
				//	return;

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
                pWriter->WriteAttribute2(_T("name"),    name);
                pWriter->WriteAttribute2(_T("descr"),   descr);
                pWriter->WriteAttribute (_T("hidden"),  hidden);
                pWriter->WriteAttribute (_T("title"),   title);

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

				// TODO: пока без гиперссылок

				pReader->Seek(_end_rec);
			}

		public:
			int					id;
			CString				name;
			nullable_string		descr;
			nullable_bool		hidden;
			nullable_string		title;
			nullable<Hyperlink> hlinkClick;
			nullable<Hyperlink> hlinkHover;
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
