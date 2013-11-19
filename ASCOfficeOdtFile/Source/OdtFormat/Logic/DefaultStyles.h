#pragma once
#ifndef ODT_LOGIC_DEFAULT_STYLES_INCLUDE_H_
#define ODT_LOGIC_DEFAULT_STYLES_INCLUDE_H_

#include "./../WritingVector.h"
#include "DefaultStyle.h"


namespace Odt
{
	namespace Logic
	{
		class DefaultStyles : public WritingVector<DefaultStyle>
		{
		public:
			DefaultStyles();
			virtual ~DefaultStyles();
			explicit DefaultStyles(const XML::XNode& node);
			const DefaultStyles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& family) const;
			const DefaultStyle& operator[](const std::string& family) const;
			DefaultStyle& operator[](const std::string& family);

			const DefaultStyle& find(const std::string& family) const;
			DefaultStyle& find(const std::string& family);
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_DEFAULT_STYLES_INCLUDE_H_