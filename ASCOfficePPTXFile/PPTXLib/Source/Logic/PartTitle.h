#pragma once
#ifndef PPTX_LOGIC_PART_TITLE_INCLUDE_H_
#define PPTX_LOGIC_PART_TITLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class PartTitle : public WrapperWritingElement
		{
		public:
			PartTitle();
			virtual ~PartTitle();
			explicit PartTitle(const XML::XNode& node);
			const PartTitle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string> m_title;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PART_TITLE_INCLUDE_H_