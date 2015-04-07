#pragma once
#ifndef PPTX_LOGIC_GLOW_INCLUDE_H_
#define PPTX_LOGIC_GLOW_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class Glow : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Glow)

			Glow& operator=(const Glow& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Color = oSrc.Color;
				rad = oSrc.rad;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Color.GetColorFrom(node);
				node.ReadAttributeBase(L"rad", rad);
				FillParentPointersForChilds();

				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("rad"), rad);

				return XmlUtils::CreateNode(_T("a:glow"), oAttr, Color.toXML());
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_GLOW);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, rad);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, Color);

				pWriter->EndRecord();
			}

		public:
			UniColor		Color;
			nullable_int	rad;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Color.SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				rad.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GLOW_INCLUDE_H_