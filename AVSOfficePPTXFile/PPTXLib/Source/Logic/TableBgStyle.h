#pragma once
#ifndef PPTX_LOGIC_TABLEBGSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TABLEBGSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "UniFill.h"
#include "FillStyle.h"
#include "StyleRef.h"
#include "../Theme.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class TableBgStyle : public WrapperWritingElement
		{
		public:
			TableBgStyle();
			virtual ~TableBgStyle();			
			explicit TableBgStyle(const XML::XNode& node);
			const TableBgStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			// Childs
			//effect (Effect)  §20.1.4.2.7 
			//effectRef (Effect Reference)  §20.1.4.2.8 
			nullable_property<FillStyle> fill;
			nullable_property<StyleRef> fillRef;

			const UniFill GetFillStyle(UniColor& Color)const;
		protected:
			virtual void FillParentPointersForChilds();

		public:
			void SetTheme(const boost::shared_ptr<PPTX::Theme> theme);
		private:
			boost::shared_ptr<PPTX::Theme> m_Theme;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLEBGSTYLE_INCLUDE_H