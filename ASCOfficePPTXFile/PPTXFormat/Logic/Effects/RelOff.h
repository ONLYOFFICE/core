#pragma once
#ifndef PPTX_LOGIC_RELOFF_INCLUDE_H_
#define PPTX_LOGIC_RELOFF_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class RelOff : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(RelOff)

			RelOff& operator=(const RelOff& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				tx = oSrc.tx;
				ty = oSrc.ty;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"tx", tx);
				node.ReadAttributeBase(L"ty", ty);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("tx"), tx);
				oAttr.Write(_T("ty"), ty);

				return XmlUtils::CreateNode(_T("a:relOff"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_RELOFF);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, tx);
				pWriter->WriteInt2(0, ty);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			nullable_int	tx;
			nullable_int	ty;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RELOFF_INCLUDE_H_