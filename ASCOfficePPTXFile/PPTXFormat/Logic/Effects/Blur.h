#pragma once
#ifndef PPTX_LOGIC_BLUR_INCLUDE_H_
#define PPTX_LOGIC_BLUR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class Blur : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(Blur)

			Blur& operator=(const Blur& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				rad  = oSrc.rad;
				grow = oSrc.grow;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"rad", rad);
				node.ReadAttributeBase(L"grow", grow);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("rad"), rad);
				oAttr.Write(_T("grow"), grow);

				return XmlUtils::CreateNode(_T("a:blur"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_BLUR);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteSize_t2(0, rad);
				pWriter->WriteBool2(1, grow);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			nullable_sizet	rad;
			nullable_bool	grow;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLUR_INCLUDE_H_