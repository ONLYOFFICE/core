#pragma once
#ifndef ODP_STYLES_FILL_IMAGES_INCLUDE_H_
#define ODP_STYLES_FILL_IMAGES_INCLUDE_H_

#include "OdtFormat/WritingVector.h"
#include "FillImage.h"

namespace Odp
{
	namespace Styles
	{
		class FillImages : public Odt::WritingVector<FillImage>
		{
		public:
			FillImages();
			virtual ~FillImages();
			explicit FillImages(const XML::XNode& node);
			const FillImages& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& name) const;
			const FillImage& operator[](const std::string& name) const;
			FillImage& operator[](const std::string& name);
			const FillImage& find(const std::string& name) const;
			FillImage& find(const std::string& name);
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_FILL_IMAGES_INCLUDE_H_