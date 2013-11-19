#pragma once
#ifndef PPTX_LOGIC_TABLEPARTSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TABLEPARTSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "TcStyle.h"
#include "TcTxStyle.h"
#include "UniFill.h"
#include "UniColor.h"
#include "../Theme.h"
#include <boost/shared_ptr.hpp>


namespace PPTX
{
	namespace Logic
	{
		class TablePartStyle : public WrapperWritingElement
		{
		public:
			TablePartStyle();
			virtual ~TablePartStyle();			
			explicit TablePartStyle(const XML::XNode& node);
			const TablePartStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<TcTxStyle> tcTxStyle;
			nullable_property<TcStyle> tcStyle;

			const UniFill GetFillStyle(UniColor& Color)const;

		//private:
		public:
			property<std::string> m_name;
		protected:
			virtual void FillParentPointersForChilds();

		public:
			void SetTheme(const boost::shared_ptr<PPTX::Theme> theme);
		private:
			boost::shared_ptr<PPTX::Theme> m_Theme;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLEPARTSTYLE_INCLUDE_H