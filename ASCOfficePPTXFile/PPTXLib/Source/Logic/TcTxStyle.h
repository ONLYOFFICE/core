#pragma once
#ifndef PPTX_LOGIC_TCTXSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TCTXSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "./../Limit/OnOff.h"
#include "UniColor.h"
#include "FontRef.h"

namespace PPTX
{
	namespace Logic
	{
		class TcTxStyle : public WrapperWritingElement
		{
		public:
			TcTxStyle();
			virtual ~TcTxStyle();			
			explicit TcTxStyle(const XML::XNode& node);
			const TcTxStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string, Limit::OnOff> i;
			nullable_property<std::string, Limit::OnOff> b;
/*
font (Font)  §20.1.4.2.13 
//Здесь имеется ввиду fontCollection (см. ниже), это еще одна альтернатива для Font
//Данный комментарий оставлен для напоминания
*/
			nullable_property<FontRef> FontRef;
			UniColor Color;
		protected:
			virtual void FillParentPointersForChilds();
		};

//		class FontCollection: public WrapperWritingElement
//		{
//		}

	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TCTXSTYLE_INCLUDE_H