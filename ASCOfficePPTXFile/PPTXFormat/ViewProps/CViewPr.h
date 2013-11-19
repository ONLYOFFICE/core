#pragma once
#ifndef PPTX_VIEWPROPS_COMMON_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_COMMON_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Origin.h"
#include "Scale.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class CViewPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CViewPr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"varScale", attrVarScale);

				Scale	= node.ReadNode(_T("p:scale"));
				Origin	= node.ReadNode(_T("p:origin"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("varScale"), attrVarScale);

				XmlUtils::CNodeValue oValue;
				oValue.Write(Scale);
				oValue.Write(Origin);

				return XmlUtils::CreateNode(_T("p:cViewPr"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, attrVarScale);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, Origin);
				pWriter->WriteRecord1(1, Scale);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:cViewPr"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("varScale"), attrVarScale);
				pWriter->EndAttributes();

				Scale.toXmlWriter(pWriter);
				Origin.toXmlWriter(pWriter);				
				
				pWriter->EndNode(_T("p:cViewPr"));
			}

		public:
			nullable_bool			attrVarScale;
			nsViewProps::Origin		Origin;
			nsViewProps::Scale		Scale;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Scale.SetParentPointer(this);
				Origin.SetParentPointer(this);
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_COMMON_VIEW_PROPERTIES_INCLUDE_H_