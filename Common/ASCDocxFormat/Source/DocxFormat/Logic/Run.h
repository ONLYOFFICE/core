#pragma once
#ifndef OOX_LOGIC_RUN_INCLUDE_H_
#define OOX_LOGIC_RUN_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "RunProperty.h"
#include "nullable_property.h"
#include <vector>
#include "RunItem.h"
#include "./../RId.h"
#include <boost/filesystem.hpp>


namespace OOX
{
	namespace Logic
	{
		class Run : public ParagraphItemBase
		{
		public:
			Run();
			virtual ~Run();
			explicit Run(const XML::XNode& node);
			explicit Run(const std::string& text);
			Run(const std::string& text, const RunProperty& runProperty);
			const Run& operator =(const XML::XNode& node);
			const Run& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			const bool empty() const;
			void clear();

		public:
			nullable_property<RunProperty>		Property;	
			property<std::vector<RunItem> >		Items;

		public:
			template<typename T> void add() {Items->push_back(RunItem(new T()));}
			template<typename T> void add(const T& item) {Items->push_back(RunItem(new T(item)));};
			template<typename T> void add(T* item) {Items->push_back(RunItem(item));}
			void add(RunItemBase* item);
			void add(const std::string& text);
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_RUN_INCLUDE_H_