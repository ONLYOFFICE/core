#pragma once
#ifndef PPTX_LOGIC_BLEND_INCLUDE_H_
#define PPTX_LOGIC_BLEND_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../EffectDag.h"
#include "./../../Limit/BlendMode.h"

namespace PPTX
{
	namespace Logic
	{

		class Blend : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(Blend)

			Blend& operator=(const Blend& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				cont  = oSrc.cont;
				blend = oSrc.blend;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cont	= node.ReadNode(_T("a:cont"));
				blend	= node.GetAttribute(_T("blend"));
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				return _T("<a:blend blend=\"") + blend.get() + _T("\">") + cont.toXML() + _T("</a:blend>");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_BLEND);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit1(0, blend);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, cont);

				pWriter->EndRecord();
			}

		public:
			EffectDag			cont;
			Limit::BlendMode	blend;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cont.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLEND_INCLUDE_H_