#pragma once
#ifndef PPTX_LOGIC_CLOSE_INCLUDE_H_
#define PPTX_LOGIC_CLOSE_INCLUDE_H_

#include "PathBase.h"

namespace PPTX
{
	namespace Logic
	{

		class Close : public PathBase
		{
		public:
			PPTX_LOGIC_BASE(Close)

			Close& operator=(const Close& oSrc)
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
				return _T("<a:close/>");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(_T("<a:close/>"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_PATH_CLOZE);
				pWriter->EndRecord();
			}

		protected:
			virtual void FillParentPointersForChilds(){};
		
		public:
			virtual CString GetODString()const
			{
				return _T("<close/>");
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CLOSE_INCLUDE_H_