#pragma once
#ifndef OOX_LOGIC_RUN_ITEM_INCLUDE_H_
#define OOX_LOGIC_RUN_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include "IItemable.h"
#include <boost/shared_ptr.hpp>
#include "RunItemBase.h"


namespace OOX
{
	namespace Logic
	{
		class RunItem : public WritingElement, public IItemable<RunItemBase>
		{
		public:
			RunItem();
			virtual ~RunItem();
			explicit RunItem(RunItemBase* item);
			explicit RunItem(const XML::XNode& node);
			explicit RunItem(const std::string& text);
			const RunItem& operator =(RunItemBase* item);
			const RunItem& operator =(const XML::XNode& node);
			const RunItem& operator =(const std::string& text);

		public:
			void fromItem(RunItemBase* item);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			void fromTxt(const std::string& text);
			const std::string toTxt() const;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_RUN_ITEM_INCLUDE_H_