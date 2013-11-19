#pragma once
#ifndef ODT_CONTENT_AHREF_INCLUDE_H_
#define ODT_CONTENT_AHREF_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/HrefType.h"
#include <boost_filesystem_version.h>
#include <vector>
#include "ParagraphItem.h"


namespace Odt
{
	namespace Content
	{
		class Ahref : public ParagraphItemBase
		{
		public:
			Ahref();
			virtual ~Ahref();
			explicit Ahref(const XML::XNode& node);
			explicit Ahref(const std::string& text);
			const Ahref& operator =(const XML::XNode& node);
			const Ahref& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			const bool isInner() const;

		public:
			nullable_property<std::string, Limit::HrefType>	Type;
			property<std::string>														Href;
			property<std::vector<ParagraphItem> >						Items;

		private:
			bool	m_isInner;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_AHREF_INCLUDE_H_