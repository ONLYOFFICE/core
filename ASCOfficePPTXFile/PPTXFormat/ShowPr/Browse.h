#pragma once
#ifndef PPTX_SHOWPR_BROWSE_INCLUDE_H_
#define PPTX_SHOWPR_BROWSE_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsShowPr
	{
		class Browse : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Browse)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(_T("showScrollbar"), showScrollbar);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("showScrollbar"), showScrollbar);

				return XmlUtils::CreateNode(_T("p:browse"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, showScrollbar);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:browse"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("showScrollbar"), showScrollbar);

				pWriter->EndAttributes();	

				pWriter->EndNode(_T("p:browse"));
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						showScrollbar = pReader->GetBool();
					else
						break;
				}

				pReader->Seek(_end_rec);
			}
		public:
			nullable_bool			showScrollbar;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsShowPr
} // namespace PPTX

#endif // PPTX_SHOWPR_BROWSE_INCLUDE_H_
