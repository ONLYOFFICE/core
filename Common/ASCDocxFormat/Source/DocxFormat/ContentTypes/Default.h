#pragma once
#ifndef OOX_CONTENT_TYPES_DEFAULT_INCLUDE_H_
#define OOX_CONTENT_TYPES_DEFAULT_INCLUDE_H_

#include <string>

#include "./../WritingElement.h"

namespace OOX
{
	namespace ContentTypes
	{
		class Default : public WritingElement
		{
		public:
			Default(const std::wstring& extension);
			Default(const wchar_t* extension);
			virtual ~Default();
			explicit Default(const XML::XNode& node);
			const Default& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool operator ==(const std::wstring& rhs) const;

		private:
			std::wstring	m_extension;
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // OOX_CONTENT_TYPES_DEFAULT_INCLUDE_H_