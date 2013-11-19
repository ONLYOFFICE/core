#pragma once
#ifndef ODT_LOGIC_FONT_FACE_DECLS_INCLUDE_H_
#define ODT_LOGIC_FONT_FACE_DECLS_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "FontFace.h"
#include "property.h"


namespace Odt
{
	namespace Logic
	{
		class FontFaceDecls : public WritingElement
		{
		public:
			FontFaceDecls();
			virtual ~FontFaceDecls();
			explicit FontFaceDecls(const XML::XNode& node);
			const FontFaceDecls& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			void Add(const FontFace& font);

		public:
			property<std::vector<FontFace> > Fonts;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_Logic_FONT_FACE_DECLS_INCLUDE_H_