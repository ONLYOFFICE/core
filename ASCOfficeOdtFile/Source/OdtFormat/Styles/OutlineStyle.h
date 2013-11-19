#pragma once
#ifndef ODT_STYLES_OUTLINE_STYLE_INCLUDE_H_
#define ODT_STYLES_OUTLINE_STYLE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include <vector>
#include "nullable.h"
#include "OutlineLevelStyle.h"


namespace Odt
{
	namespace Styles
	{
		class OutlineStyle : public WritingElement
		{
		public:
			OutlineStyle();
			virtual ~OutlineStyle();
			explicit OutlineStyle(const XML::XNode& node);
			const OutlineStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			nullable<std::string>						m_name;
			std::vector<OutlineLevelStyle>	m_list;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_OUTLINE_STYLE_INCLUDE_H_