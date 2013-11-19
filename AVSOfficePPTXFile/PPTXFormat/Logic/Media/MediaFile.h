#pragma once
#ifndef PPTX_LOGIC_MEDIAFILE_INCLUDE_H_
#define PPTX_LOGIC_MEDIAFILE_INCLUDE_H_

#include "../../WrapperWritingElement.h"
#include "../../DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{
		class MediaFile : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(MediaFile)

			MediaFile& operator=(const MediaFile& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				name = oSrc.name;
				link = oSrc.link;
				contentType = oSrc.contentType;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name		= XmlUtils::GetNameNoNS(node.GetName());
				link		= node.GetAttribute(_T("r:link"));
				node.ReadAttributeBase(L"contentType", contentType);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("r:link"), link.ToString());
				oAttr.Write(_T("contentType"), contentType);

				return XmlUtils::CreateNode(_T("a:") + name, oAttr);
			}
		public:
			CString				name;
			OOX::RId			link;
			nullable_string		contentType;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_MEDIAFILE_INCLUDE_H_