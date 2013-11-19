#pragma once
#ifndef PPTX_LOGIC_HSLEFFECT_INCLUDE_H_
#define PPTX_LOGIC_HSLEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class HslEffect : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(HslEffect)

			HslEffect& operator=(const HslEffect& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				hue = oSrc.hue;
				lum = oSrc.lum;
				sat = oSrc.sat;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"hue", hue);
				node.ReadAttributeBase(L"sat", sat);
				node.ReadAttributeBase(L"lum", lum);

				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("hue"), hue);
				oAttr.Write(_T("sat"), sat);
				oAttr.Write(_T("lum"), lum);

				return XmlUtils::CreateNode(_T("a:hsl"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_HSL);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, hue);
				pWriter->WriteInt2(0, lum);
				pWriter->WriteInt2(0, sat);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}
		public:
			nullable_int hue;
			nullable_int lum;
			nullable_int sat;			
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				hue.normalize(0, 21600000);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_HSLEFFECT_INCLUDE_H_