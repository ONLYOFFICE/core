#pragma once
#ifndef PPTX_LOGIC_ALPHAOUTSET_INCLUDE_H_
#define PPTX_LOGIC_ALPHAOUTSET_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaOutset : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(AlphaOutset)

			AlphaOutset& operator=(const AlphaOutset& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				rad = oSrc.rad;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"rad", rad);
			}

			virtual CString toXML() const
			{
				if (rad.IsInit())
				{
					CString str = _T("");
					str.Format(_T("<a:alphaOutset rad=\"%u\" />"), *rad);
					return str;
				}

				return _T("<a:alphaOutset/>");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_ALPHAOUTSET);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteSize_t2(0, rad);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			nullable_sizet rad;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAOUTSET_INCLUDE_H_