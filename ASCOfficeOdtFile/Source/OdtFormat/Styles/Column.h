#pragma once
#ifndef ODT_STYLES_COLUMN_INCLUDE_H_
#define ODT_STYLES_COLUMN_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "Utility.h"
#include "property.h"


namespace Odt
{
	namespace Styles
	{
		class Column : public WritingElement
		{
		public:
			Column();
			virtual ~Column();
			explicit Column(const XML::XNode& node);
			const Column& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<UniversalUnit> RelWidth;
			property<UniversalUnit> StartIndent;
			property<UniversalUnit> EndIndent;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_COLUMN_INCLUDE_H_