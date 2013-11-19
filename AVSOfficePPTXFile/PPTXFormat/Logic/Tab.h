#pragma once
#ifndef PPTX_LOGIC_TAB_INCLUDE_H_
#define PPTX_LOGIC_TAB_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/TextTabAlignType.h"

namespace PPTX
{
	namespace Logic
	{
		class Tab : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Tab)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"pos", pos);
				node.ReadAttributeBase(L"algn", algn);

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("pos"), pos);
				oAttr.WriteLimitNullable(_T("algn"), algn);

				return XmlUtils::CreateNode(_T("a:tab"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, algn);
				pWriter->WriteInt2(1, pos);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tab"));
				
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("pos"), pos);
				pWriter->WriteAttribute(_T("algn"), algn);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:tab"));
			}

		public:
			nullable_int								pos;
			nullable_limit<Limit::TextTabAlignType>		algn;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TAB_INCLUDE_H