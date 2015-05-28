#pragma once
#ifndef PPTX_LOGIC_GS_INCLUDE_H_
#define PPTX_LOGIC_GS_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class Gs : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Gs)

			Gs& operator=(const Gs& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				color	= oSrc.color;
				pos		= oSrc.pos;
				
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				pos		= node.ReadAttributeInt(L"pos");
				color.GetColorFrom(node);

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("pos"), pos);

				return XmlUtils::CreateNode(_T("a:gs"), oAttr, color.toXML());
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString sNodeNamespace;
				CString sAttrNamespace;
				if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				{
					sNodeNamespace = _T("w14:");
					sAttrNamespace = sNodeNamespace;
				}
				else
					sNodeNamespace = _T("a:");
				pWriter->StartNode(sNodeNamespace + _T("gs"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("pos"), pos);
				pWriter->EndAttributes();

				color.toXmlWriter(pWriter);

				pWriter->EndNode(sNodeNamespace + _T("gs"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt1(0, pos);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, color);				
			}

		public:
			UniColor	color;
			int			pos;
		protected:
			virtual void FillParentPointersForChilds()
			{
				color.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GS_INCLUDE_H