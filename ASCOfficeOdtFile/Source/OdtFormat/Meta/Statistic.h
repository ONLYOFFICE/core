#pragma once
#ifndef ODT_META_STATISTIC_INCLUDE_H_
#define ODT_META_STATISTIC_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"


namespace Odt
{
	namespace Meta
	{
		class Statistic : public WritingElement
		{
		public:
			Statistic();
			virtual ~Statistic();
			explicit Statistic(const XML::XNode& node);
			const Statistic& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<int, setter::only_positive<int> > TableCount;
			nullable_property<int, setter::only_positive<int> > ImageCount;
			nullable_property<int, setter::only_positive<int> > ObjectCount;
			nullable_property<int, setter::only_positive<int> > PageCount;
			nullable_property<int, setter::only_positive<int> > ParagraphCount;
			nullable_property<int, setter::only_positive<int> > WordCount;
			nullable_property<int, setter::only_positive<int> > CharacterCount;
		};
	} // namespace Meta
} // namespace Odt

#endif // ODT_META_STATISTIC_INCLUDE_H_