#pragma once
#ifndef PPTX_VIEWPROPS_RESTORED_INCLUDE_H_
#define PPTX_VIEWPROPS_RESTORED_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Restored : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Restored)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name = XmlUtils::GetNameNoNS(node.GetName());

				sz = node.ReadAttributeInt(L"sz");
				node.ReadAttributeBase(L"autoAdjust", autoAdjust);
				
				Normalize();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("sz"), sz);
				oAttr.Write(_T("autoAdjust"), autoAdjust);

				return XmlUtils::CreateNode(_T("p:") + name, oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteINT(sz);
				pWriter->WriteBool2(0, autoAdjust);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

		public:
			int				sz;
			nullable_bool	autoAdjust;

		public:
			CString name;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				if (sz < 0)
					sz = 0;
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_RESTORED_INCLUDE_H_