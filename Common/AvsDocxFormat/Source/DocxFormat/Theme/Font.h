#pragma once
#ifndef OOX_THEME_FONT_INCLUDE_H_
#define OOX_THEME_FONT_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>


namespace OOX
{
	namespace Theme
	{
		class Font : public WritingElement
		{
		public:
			Font();
			virtual ~Font();
			explicit Font(const XML::XNode& node);
			const Font& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_script;
			std::string m_typeface;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_FONT_INCLUDE_H_