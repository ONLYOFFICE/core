#pragma once
#ifndef PPTX_VIEWPROPS_GUIDE_INCLUDE_H_
#define PPTX_VIEWPROPS_GUIDE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/Orient.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Guide : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Guide)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"orient", orient);
				node.ReadAttributeBase(L"pos", pos);
				
				Normalize();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("pos"), pos);
				oAttr.WriteLimitNullable(_T("orient"), orient);

				return XmlUtils::CreateNode(_T("p:guide"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(g_nodeAttributeStart);

				pWriter->WriteInt2(0, pos);
				pWriter->WriteLimit2(1, orient);

				pWriter->WriteBYTE(g_nodeAttributeEnd);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:guide"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("pos"), pos);
				pWriter->WriteAttribute(_T("orient"), orient);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("p:guide"));
			}

		public:
			nullable_int					pos;
			nullable_limit<Limit::Orient>	orient;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				pos.normalize_positive();
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_GUIDE_INCLUDE_H_