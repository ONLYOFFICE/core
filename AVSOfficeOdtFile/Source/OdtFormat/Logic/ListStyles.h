#pragma once
#ifndef ODT_LOGIC_LIST_STYLES_INCLUDE_H_
#define ODT_LOGIC_LIST_STYLES_INCLUDE_H_

#include "./../WritingVector.h"
#include "ListStyle.h"


namespace Odt
{
	namespace Logic
	{
		class ListStyles : public WritingVector<ListStyle>
		{
		public:
			ListStyles();
			virtual ~ListStyles();
			explicit ListStyles(const XML::XNode& node);
			const ListStyles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& name) const;
			const ListStyle& operator[](const std::string& name) const;
			ListStyle& operator[](const std::string& name);

			const ListStyle& find(const std::string& name) const;
			ListStyle& find(const std::string& name);

			const int numStyleInVector(const std::string& name) const;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_LIST_STYLES_INCLUDE_H_