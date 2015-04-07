#pragma once
#ifndef PPTX_VIEWPROPS_NORMAL_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_NORMAL_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Restored.h"
#include "./../Limit/SplitterBarState.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class NormalViewPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(NormalViewPr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"horzBarState", attrHorzBarState);
				node.ReadAttributeBase(L"vertBarState", attrVertBarState);

				node.ReadAttributeBase(L"preferSingleView", attrPreferSingleView);
				node.ReadAttributeBase(L"showOutlineIcons", attrShowOutlineIcons);
				node.ReadAttributeBase(L"snapVertSplitter", attrSnapVertSplitter);

				restoredLeft	= node.ReadNodeNoNS(_T("restoredLeft"));
				restoredTop		= node.ReadNodeNoNS(_T("restoredTop"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("horzBarState"), attrHorzBarState->get());
				oAttr.Write(_T("vertBarState"), attrVertBarState->get());

				oAttr.Write(_T("preferSingleView"), attrPreferSingleView);
				oAttr.Write(_T("showOutlineIcons"), attrShowOutlineIcons);
				oAttr.Write(_T("snapVertSplitter"), attrSnapVertSplitter);

				XmlUtils::CNodeValue oValue;
				oValue.Write(restoredTop);
				oValue.Write(restoredLeft);

				return XmlUtils::CreateNode(_T("p:normalViewPr"), oAttr, oValue);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:normalViewPr"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("horzBarState"), attrHorzBarState);
				pWriter->WriteAttribute(_T("vertBarState"), attrVertBarState);
				pWriter->WriteAttribute(_T("preferSingleView"), attrPreferSingleView);
				pWriter->WriteAttribute(_T("showOutlineIcons"), attrShowOutlineIcons);
				pWriter->WriteAttribute(_T("snapVertSplitter"), attrSnapVertSplitter);

				pWriter->EndAttributes();

				pWriter->StartNode(_T("p:restoredLeft"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("sz"), restoredLeft.sz);
				pWriter->WriteAttribute(_T("autoAdjust"), restoredLeft.autoAdjust);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("p:restoredLeft"));

				pWriter->StartNode(_T("p:restoredTop"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("sz"), restoredTop.sz);
				pWriter->WriteAttribute(_T("autoAdjust"), restoredTop.autoAdjust);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("p:restoredTop"));

				pWriter->EndNode(_T("p:normalViewPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, attrPreferSingleView);
				pWriter->WriteBool2(1, attrShowOutlineIcons);
				pWriter->WriteBool2(2, attrSnapVertSplitter);
				pWriter->WriteLimit2(3, attrHorzBarState);
				pWriter->WriteLimit2(4, attrVertBarState);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, restoredLeft);
				pWriter->WriteRecord1(1, restoredTop);
			}

		public:
			nsViewProps::Restored					restoredLeft;
			nsViewProps::Restored					restoredTop;

			nullable_limit<Limit::SplitterBarState> attrHorzBarState;
			nullable_limit<Limit::SplitterBarState> attrVertBarState;
			nullable_bool							attrPreferSingleView;
			nullable_bool							attrShowOutlineIcons;
			nullable_bool							attrSnapVertSplitter;
		protected:
			virtual void FillParentPointersForChilds()
			{
				restoredLeft.SetParentPointer(this);
				restoredTop.SetParentPointer(this);
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_NORMAL_VIEW_PROPERTIES_INCLUDE_H_