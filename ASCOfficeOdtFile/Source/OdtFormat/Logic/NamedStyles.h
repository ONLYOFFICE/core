#pragma once
#ifndef ODT_LOGIC_NAMED_STYLES_INCLUDE_H_
#define ODT_LOGIC_NAMED_STYLES_INCLUDE_H_

#include "./../WritingVector.h"
#include "NamedStyle.h"


namespace Odt
{
	namespace Logic
	{
		class NamedStyles : public WritingVector<NamedStyle>
		{
		public:
			NamedStyles();
			virtual ~NamedStyles();
			explicit NamedStyles(const XML::XNode& node);
			const NamedStyles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& name) const;
			const NamedStyle& operator[](const std::string& name) const;
			NamedStyle& operator[](const std::string& name);

			const NamedStyle& find(const std::string& name) const;
			NamedStyle& find(const std::string& name);
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_NAMED_STYLES_INCLUDE_H_