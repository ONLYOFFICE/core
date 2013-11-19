#pragma once
#ifndef PPTX_LOGIC_BUNONE_INCLUDE_H_
#define PPTX_LOGIC_BUNONE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuNone : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BuNone)

			BuNone& operator=(const BuNone& oSrc)
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
				return _T("<a:buNone/>");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(_T("<a:buNone/>"));				
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_BULLET_NONE);
				pWriter->EndRecord();
			}
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUNONE_INCLUDE_H