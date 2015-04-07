#pragma once
#ifndef PPTX_LOGIC_LUMEFFECT_INCLUDE_H_
#define PPTX_LOGIC_LUMEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class LumEffect : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(LumEffect)

			LumEffect& operator=(const LumEffect& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				bright	 = oSrc.bright;
				contrast = oSrc.contrast;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"bright", bright);
				node.ReadAttributeBase(L"contrast", contrast);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("bright"), bright);
				oAttr.Write(_T("contrast"), contrast);

				return XmlUtils::CreateNode(_T("a:lum"), oAttr);
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_LUM);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, bright);
				pWriter->WriteInt2(0, contrast);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}
		public:
			nullable_int	bright;
			nullable_int	contrast;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LUMEFFECT_INCLUDE_H_