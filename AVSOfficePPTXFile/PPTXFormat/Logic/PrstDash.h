#pragma once
#ifndef PPTX_LOGIC_PRSTDASH_INCLUDE_H_
#define PPTX_LOGIC_PRSTDASH_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/PrstDashVal.h"

namespace PPTX
{
	namespace Logic
	{

		class PrstDash : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PrstDash)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"val", val);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("val"), val);

				return XmlUtils::CreateNode(_T("a:prstDash"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, val);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:prstDash"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("val"), val);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:prstDash"));
			}

		public:
			nullable_limit<Limit::PrstDashVal> val;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTDASH_INCLUDE_H_