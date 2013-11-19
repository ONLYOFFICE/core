#pragma once
#ifndef PPTX_LOGIC_TINTEFFECT_INCLUDE_H_
#define PPTX_LOGIC_TINTEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class TintEffect : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TintEffect)

			TintEffect& operator=(const TintEffect& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				amt = oSrc.amt;
				hue = oSrc.hue;
				return *this;
			}
			
		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"amt", amt);
				node.ReadAttributeBase(L"hue", hue);

				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("hue"), hue);
				oAttr.Write(_T("amt"), amt);

				return XmlUtils::CreateNode(_T("a:tint"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_TINTEFFECT);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, amt);
				pWriter->WriteInt2(0, hue);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			nullable_int	amt;
			nullable_int	hue;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				hue.normalize(0, 21600000);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TINTEFFECT_INCLUDE_H_