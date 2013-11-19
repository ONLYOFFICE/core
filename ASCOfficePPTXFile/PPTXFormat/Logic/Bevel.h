#pragma once
#ifndef PPTX_LOGIC_BEVEL_INCLUDE_H_
#define PPTX_LOGIC_BEVEL_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/BevelType.h"

namespace PPTX
{
	namespace Logic
	{

		class Bevel : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Bevel)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name	= XmlUtils::GetNameNoNS(node.GetName());
				
				node.ReadAttributeBase(L"w", w);
				node.ReadAttributeBase(L"h", h);
				node.ReadAttributeBase(L"prst", prst);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("w"), w);
				oAttr.Write(_T("h"), h);
				oAttr.WriteLimitNullable(_T("prst"), prst);
				
				return XmlUtils::CreateNode(_T("a:") + m_name, oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, w);
				pWriter->WriteInt2(1, h);
				pWriter->WriteLimit2(2, prst);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

		public:
			nullable_int						w;
			nullable_int						h;
			nullable_limit<Limit::BevelType>	prst;
		//private:
		public:
			CString								m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BEVEL_INCLUDE_H_