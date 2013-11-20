#pragma once
#ifndef OOX_LOGIC_SDT_CONTENT_INCLUDE_H_
#define OOX_LOGIC_SDT_CONTENT_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "./../Logic/TextItem.h"
#include <vector>


namespace OOX
{
	namespace Logic
	{
		// TODO попробовать перевести в вызываемый класс
		class SdtContent : public WritingElement
		{
		public:
			SdtContent();
			virtual ~SdtContent();
			explicit SdtContent(const XML::XNode& node);
			const SdtContent& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::vector<Logic::TextItem> >	Items;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SDT_CONTENT_INCLUDE_H_