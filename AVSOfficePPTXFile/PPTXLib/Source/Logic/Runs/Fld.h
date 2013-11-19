#pragma once
#ifndef PPTX_LOGIC_FLD_INCLUDE_H_
#define PPTX_LOGIC_FLD_INCLUDE_H_

#include "RunBase.h"
#include "property.h"
#include "./../RunProperties.h"
#include "./../TextParagraphPr.h"
#include "Encoding.h"

namespace PPTX
{
	namespace Logic
	{
		class Fld : public RunBase
		{
		public:
			Fld();
			virtual ~Fld();			
			explicit Fld(const XML::XNode& node);
			const Fld& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual std::string GetText()const{return text.get_value_or("");};
			virtual std::wstring GetWText()const{return Encoding::utf82unicode(text.get_value_or(""));};
		public:
			property<std::string> id;
			nullable_property<std::string> type;

			nullable_property<RunProperties> rPr;
			nullable_property<TextParagraphPr> pPr;
		private:
			nullable_property<std::string> text;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FLD_INCLUDE_H