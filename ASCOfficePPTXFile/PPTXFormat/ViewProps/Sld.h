#pragma once
#ifndef PPTX_VIEWPROPS_SLD_INCLUDE_H_
#define PPTX_VIEWPROPS_SLD_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/Orient.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Sld : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Sld)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"id", id);
				node.ReadAttributeBase(L"collapse", collapse);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("id"), id);
				oAttr.Write(_T("collapse"), collapse);

				return XmlUtils::CreateNode(_T("p:sld"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(g_nodeAttributeStart);

				pWriter->WriteString2(0, id);
				pWriter->WriteBool2(1, collapse);

				pWriter->WriteBYTE(g_nodeAttributeEnd);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:sld"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("id"), id);
				pWriter->WriteAttribute(_T("collapse"), collapse);
				pWriter->EndAttributes();
		
				pWriter->EndNode(_T("p:sld"));
			}

		public:
			nullable_string		id;
			nullable_bool		collapse;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_SLD_INCLUDE_H_