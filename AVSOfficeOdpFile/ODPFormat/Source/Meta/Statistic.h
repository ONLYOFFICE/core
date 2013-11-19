#pragma once
#ifndef ODP_META_STATISTIC_INCLUDE_H_
#define ODP_META_STATISTIC_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "nullable_property.h"


namespace Odp
{
	namespace Meta
	{
		class Statistic : public Odt::WritingElement
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
} // namespace Odp

#endif // ODP_META_STATISTIC_INCLUDE_H_