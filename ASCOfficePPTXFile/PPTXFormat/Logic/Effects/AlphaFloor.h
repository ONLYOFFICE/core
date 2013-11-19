#pragma once
#ifndef PPTX_LOGIC_ALPHAFLOOR_INCLUDE_H_
#define PPTX_LOGIC_ALPHAFLOOR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaFloor : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AlphaFloor)

			AlphaFloor& operator=(const AlphaFloor& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}

			virtual CString toXML() const
			{
				return _T("<a:alphaFloor/>");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_ALPHAFLOOR);
				pWriter->EndRecord();
			}

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAFLOOR_INCLUDE_H_