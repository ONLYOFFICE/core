#pragma once
#ifndef PPTX_LOGIC_ATTRNAME_INCLUDE_H_
#define PPTX_LOGIC_ATTRNAME_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class AttrName : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AttrName)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				text = node.GetTextExt();
			}

			virtual CString toXML() const
			{
				return _T("<p:attrName>") + text + _T("</p:attrName>");
			}
		public:
			CString text;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ATTRNAME_INCLUDE_H_