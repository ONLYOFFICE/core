#pragma once
#ifndef PPTX_LOGIC_CLRMAP_INCLUDE_H_
#define PPTX_LOGIC_CLRMAP_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/ColorSchemeIndex.h"
#include <map>

namespace PPTX
{
	namespace Logic
	{
		class ClrMap : public WrapperWritingElement
		{
		public:
			ClrMap();
			virtual ~ClrMap();
			explicit ClrMap(const XML::XNode& node);
			const ClrMap& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			virtual std::string GetColorSchemeIndex(const std::string& str)const;

		public:
			//nullable_property<std::string, Limit::ColorSchemeIndex> accent1;
			//nullable_property<std::string, Limit::ColorSchemeIndex> accent2;
			//nullable_property<std::string, Limit::ColorSchemeIndex> accent3;
			//nullable_property<std::string, Limit::ColorSchemeIndex> accent4;
			//nullable_property<std::string, Limit::ColorSchemeIndex> accent5;
			//nullable_property<std::string, Limit::ColorSchemeIndex> accent6;
			//nullable_property<std::string, Limit::ColorSchemeIndex> bg1;
			//nullable_property<std::string, Limit::ColorSchemeIndex> bg2;
			//nullable_property<std::string, Limit::ColorSchemeIndex> folHlink;
			//nullable_property<std::string, Limit::ColorSchemeIndex> hlink;
			//nullable_property<std::string, Limit::ColorSchemeIndex> tx1;
			//nullable_property<std::string, Limit::ColorSchemeIndex> tx2;
			property<std::map<std::string, property<std::string, Limit::ColorSchemeIndex> > > ColorMap;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CLRMAP_INCLUDE_H_