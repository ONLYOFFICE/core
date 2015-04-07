#pragma once
#ifndef PPTX_LOGIC_ALPHACEILING_INCLUDE_H_
#define PPTX_LOGIC_ALPHACEILING_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaCeiling : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AlphaCeiling)
			
			AlphaCeiling& operator=(const AlphaCeiling& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				return *this;
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_ALPHACEILING);
				pWriter->EndRecord();
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}

			virtual CString toXML() const
			{
				return _T("<a:alphaCeiling/>");
			}
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHACEILING_INCLUDE_H_