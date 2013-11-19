#pragma once
#ifndef PPTX_LOGIC_BUSZPTS_INCLUDE_H_
#define PPTX_LOGIC_BUSZPTS_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuSzPts : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BuSzPts)

			BuSzPts& operator=(const BuSzPts& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				val = oSrc.val;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				val = node.ReadAttributeInt(_T("val"));
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), val);
				return XmlUtils::CreateNode(_T("a:buSzPts"), oAttr);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:buSzPts"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("val"), val);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:buSzPts"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_SIZE_PTS);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt1(0, val);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}
		public:
			int val;
		protected:
			virtual void FillParentPointersForChilds(){};
			AVSINLINE void Normalize()
			{
				normalize_value(val, 100, 400000);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUSZPTS_INCLUDE_H