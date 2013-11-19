#pragma once
#ifndef PPTX_LOGIC_TABLESTYLE_INCLUDE_H_
#define PPTX_LOGIC_TABLESTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "TableBgStyle.h"
#include "TablePartStyle.h"
#include "../Theme.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class TableStyle : public WrapperWritingElement
		{
		public:
			TableStyle();
			virtual ~TableStyle();			
			explicit TableStyle(const XML::XNode& node);
			const TableStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> styleId;
			property<std::string> styleName;

			nullable_property<TableBgStyle> tblBg;
			nullable_property<TablePartStyle> wholeTbl;
			nullable_property<TablePartStyle> band1H;
			nullable_property<TablePartStyle> band2H;
			nullable_property<TablePartStyle> band1V;
			nullable_property<TablePartStyle> band2V;
			nullable_property<TablePartStyle> lastCol;
			nullable_property<TablePartStyle> firstCol;
			nullable_property<TablePartStyle> lastRow;
			nullable_property<TablePartStyle> seCell;
			nullable_property<TablePartStyle> swCell;
			nullable_property<TablePartStyle> firstRow;
			nullable_property<TablePartStyle> neCell;
			nullable_property<TablePartStyle> nwCell;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		public:
			void SetTheme(const boost::shared_ptr<Theme> theme);
		private:
			boost::shared_ptr<PPTX::Theme> m_Theme;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLESTYLE_INCLUDE_H