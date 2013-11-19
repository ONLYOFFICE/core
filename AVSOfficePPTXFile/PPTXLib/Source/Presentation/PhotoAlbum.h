#pragma once
#ifndef PPTX_PRESENTATION_PHOTOALBUM_INCLUDE_H_
#define PPTX_PRESENTATION_PHOTOALBUM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/FrameShape.h"
#include "./../Limit/AlbumLayout.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class PhotoAlbum : public WrapperWritingElement
		{
		public:
			PhotoAlbum();
			virtual ~PhotoAlbum();
			explicit PhotoAlbum(const XML::XNode& node);
			const PhotoAlbum& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<bool> bw;
			nullable_property<std::string, Limit::FrameShape> frame;
			nullable_property<std::string, Limit::AlbumLayout> layout;
			nullable_property<bool> showCaptions;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_PHOTOALBUM_INCLUDE_H_