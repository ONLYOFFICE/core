#pragma once
#ifndef PPTX_LOGIC_BUCLRTX_INCLUDE_H_
#define PPTX_LOGIC_BUCLRTX_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuClrTx : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BuClrTx)

			BuClrTx& operator=(const BuClrTx& oSrc)
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
				return _T("<a:buClrTx/>");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(_T("<a:buClrTx/>"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_COLOR_CLRTX);
				pWriter->EndRecord();
			}

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUCLRTX_INCLUDE_H