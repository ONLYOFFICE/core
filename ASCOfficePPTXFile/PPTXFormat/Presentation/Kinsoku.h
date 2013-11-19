#pragma once
#ifndef PPTX_PRESENTATION_KINSOKU_INCLUDE_H_
#define PPTX_PRESENTATION_KINSOKU_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class Kinsoku : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Kinsoku)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				invalEndChars	= node.GetAttribute(_T("invalEndChars"));
				invalStChars	= node.GetAttribute(_T("invalStChars"));

				node.ReadAttributeBase(L"lang", lang);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("invalEndChars"), invalEndChars);
				oAttr.Write(_T("invalStChars"), invalStChars);
				oAttr.Write(_T("lang"), lang);

				return XmlUtils::CreateNode(_T("p:kinsoku"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				// TODO:
			}

		public:
			CString				invalEndChars;
			CString				invalStChars;
			nullable_string		lang;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_KINSOKU_INCLUDE_H_