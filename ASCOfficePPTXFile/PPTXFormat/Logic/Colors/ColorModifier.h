#pragma once
#ifndef PPTX_LOGIC_COLORMODIFIER_INCLUDE_H_
#define PPTX_LOGIC_COLORMODIFIER_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class ColorModifier : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ColorModifier)
		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name	= node.GetName();
				node.ReadAttributeBase(L"val", val);

				if (XmlUtils::GetNameNoNS(name) == _T("alpha"))
				{
					nullable_string sTmp;
					node.ReadAttributeBase(L"val", sTmp);

					if (val.is_init() && sTmp.is_init() && sTmp->Find(TCHAR('%')) != -1)
						*val = (*val) * 1000;
				}
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), val);
				return XmlUtils::CreateNode(name, oAttr);
			}

			ColorModifier& operator=(const ColorModifier& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				name		= oSrc.name;
				val			= oSrc.val;
				return *this;
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, name);
				pWriter->WriteInt2(1, val);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{				
				CString _name;
				CString sAttrNamespace;
				if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				{
					_name = _T("w14:") + XmlUtils::GetNameNoNS(name);
					sAttrNamespace = _T("w14:");
				}
				else
					_name = name;
				pWriter->StartNode(_name);
				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("val"), val);
				pWriter->EndAttributes();
				pWriter->EndNode(_name);
			}

		public:
			CString			name;
			nullable_int	val;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_COLORMODIFIER_INCLUDE_H