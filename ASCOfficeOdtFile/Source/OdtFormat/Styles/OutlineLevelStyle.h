#pragma once
#ifndef ODT_STYLES_OUTLINE_LEVEL_STYLE_INCLUDE_H_
#define ODT_STYLES_OUTLINE_LEVEL_STYLE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "ListLevelProperties.h"


namespace Odt
{
	namespace Styles
	{
		class OutlineLevelStyle : public WritingElement
		{
		public:
			OutlineLevelStyle();
			virtual ~OutlineLevelStyle();
			explicit OutlineLevelStyle(const XML::XNode& node);
			const OutlineLevelStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int									m_level;
			std::string					m_numFormat;
			ListLevelProperties	m_properties;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_OUTLINE_LEVEL_STYLE_INCLUDE_H_