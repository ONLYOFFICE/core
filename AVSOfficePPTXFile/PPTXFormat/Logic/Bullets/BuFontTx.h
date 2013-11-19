#pragma once
#ifndef PPTX_LOGIC_BUFONTTX_INCLUDE_H_
#define PPTX_LOGIC_BUFONTTX_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuFontTx : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BuFontTx)

			BuFontTx& operator=(const BuFontTx& oSrc)
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
				return _T("<a:buFontTx/>");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(_T("<a:buFontTx/>"));				
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_TYPEFACE_TX);
				pWriter->EndRecord();
			}
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUFONTTX_INCLUDE_H