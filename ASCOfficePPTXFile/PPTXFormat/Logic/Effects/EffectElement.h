#pragma once
#ifndef PPTX_LOGIC_EFFECTELEMENT_INCLUDE_H_
#define PPTX_LOGIC_EFFECTELEMENT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class EffectElement : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(EffectElement)

			EffectElement& operator=(const EffectElement& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				ref = oSrc.ref;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"ref", ref);
			}

			virtual CString toXML() const
			{
				if (!ref.IsInit())
					return _T("<a:effect/>");

				return _T("<a:effect ref=\"") + *ref + _T("\" />");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_ELEMENT);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, ref);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			nullable_string		ref;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTELEMENT_INCLUDE_H_