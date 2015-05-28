#pragma once
#ifndef PPTX_LOGIC_RECT_INCLUDE_H_
#define PPTX_LOGIC_RECT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class Rect : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Rect)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				node.ReadAttributeBase(L"t", t);
				node.ReadAttributeBase(L"b", b);
				node.ReadAttributeBase(L"l", l);
				node.ReadAttributeBase(L"r", r);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("l"), l);
				oAttr.Write(_T("t"), t);
				oAttr.Write(_T("r"), r);
				oAttr.Write(_T("b"), b);
				
				return XmlUtils::CreateNode(m_name, oAttr);			
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString _name;
				CString sAttrNamespace;
				if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				{
					_name = _T("w14:") + XmlUtils::GetNameNoNS(m_name);
					sAttrNamespace = _T("w14:");
				}
				else
					_name = m_name;
				pWriter->StartNode(_name);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("l"), l);
				pWriter->WriteAttribute(sAttrNamespace + _T("t"), t);
				pWriter->WriteAttribute(sAttrNamespace + _T("r"), r);
				pWriter->WriteAttribute(sAttrNamespace + _T("b"), b);
				pWriter->EndAttributes();

				pWriter->EndNode(_name);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, l);
				pWriter->WriteString2(1, t);
				pWriter->WriteString2(2, r);
				pWriter->WriteString2(3, b);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							l = pReader->GetString2();
							break;
						}
						case 1:
						{
							t = pReader->GetString2();
							break;
						}
						case 2:
						{
							r = pReader->GetString2();
							break;
						}
						case 3:
						{
							b = pReader->GetString2();
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			nullable_string t;
			nullable_string b;
			nullable_string l;
			nullable_string r;
		//private:
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual CString GetODString()const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("l"), l);
				oAttr.Write(_T("t"), t);
				oAttr.Write(_T("r"), r);
				oAttr.Write(_T("b"), b);
				
				return XmlUtils::CreateNode(XmlUtils::GetNameNoNS(m_name), oAttr);	
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RECT_INCLUDE_H_