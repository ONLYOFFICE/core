#pragma once
#ifndef PPTX_LOGIC_MEDIAFILE_INCLUDE_H_
#define PPTX_LOGIC_MEDIAFILE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "DocxFormat/RId.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class MediaFile : public WrapperWritingElement
		{
		public:
			MediaFile();
			virtual ~MediaFile();			
			explicit MediaFile(const XML::XNode& node);
			const MediaFile& operator =(const XML::XNode& node);
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> name;
			property<OOX::RId> link;
			nullable_property<std::string> contentType;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_MEDIAFILE_INCLUDE_H_