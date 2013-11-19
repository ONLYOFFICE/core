#pragma once
#ifndef PPTX_LOGIC_PART_TITLE_INCLUDE_H_
#define PPTX_LOGIC_PART_TITLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class PartTitle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PartTitle)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_title = node.GetTextExt();
			}
			virtual CString toXML() const
			{
				return _T("<vt:lpstr>") + m_title + _T("</vt:lpstr>");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteNodeValue(_T("vt:lpstr"), m_title);	
			}

		public:
			CString	m_title;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PART_TITLE_INCLUDE_H_