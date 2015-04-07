#pragma once
#ifndef PPTX_LOGIC_BUSZTX_INCLUDE_H_
#define PPTX_LOGIC_BUSZTX_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuSzTx : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BuSzTx)

			BuSzTx& operator=(const BuSzTx& oSrc)
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
				return _T("<a:buSzTx/>");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(_T("<a:buSzTx/>"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_SIZE_TX);
				pWriter->EndRecord();
			}
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUSZTX_INCLUDE_H